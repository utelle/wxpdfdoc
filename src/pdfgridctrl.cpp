///////////////////////////////////////////////////////////////////////////////
// Name:        pdfgridctrl.cpp
// Purpose:
// Author:      Blake Madden
// Created:     2026-06-22
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_GRID

#include "wx/pdfgridctrl.h"
#include <wx/grid.h>
#include <wx/log.h>
#include "wx/pdfdocument.h"
#include "wx/pdfdc.h"

#include <vector>
#include <algorithm>

wxPdfGridOptions::wxPdfGridOptions()
{
  m_drawRowBorders = true;
  m_drawColumnBorders = true;
  m_showContinued = true;
  m_fitToPage = false;
  m_style = wxPDF_GRID_STYLE_GRID;
  m_headerBackgroundColour = wxPdfColour(195, 220, 240);
  m_headerTextColour = wxPdfColour(0, 0, 0);
  m_borderColour = wxPdfColour();
  m_fromRow = 1;
  m_fromColumn = 1;
  m_toRow = -1;
  m_toColumn = -1;
  m_includeColumnHeaders = true;
  m_includeRowLabels = false;
  m_useCellAttributes = true;
  m_bitmapProvider = nullptr;
}

// --- Exporter Engine

class wxPdfGridExporter
{
public:
  wxPdfGridExporter(wxPdfDocument* doc, const wxGrid* grid, const wxPdfGridOptions& options)
    : m_doc(doc), m_grid(grid), m_options(options), m_dc(nullptr), m_exportId(ms_exportCounter++)
  {
  }

  wxPdfGridExporter(wxPdfDC* dc, const wxGrid* grid, const wxPdfGridOptions& options)
    : m_doc(dc->GetPdfDocument()), m_grid(grid), m_options(options), m_dc(dc), m_exportId(ms_exportCounter++)
  {
  }

  void Export(double x, double y)
  {
    if (!m_doc || !m_grid)
      return;

    const int gridColCount = m_grid->GetNumberCols();
    const int gridRowCount = m_grid->GetNumberRows();

    if (gridColCount == 0 || gridRowCount == 0)
      return;

    const int fromCol = std::max(0, m_options.GetFromColumn() - 1);
    const int toCol = m_options.GetToColumn() == -1 ?
        gridColCount - 1 : std::min(gridColCount - 1, m_options.GetToColumn() - 1);

    const int fromRowBase = std::max(0, m_options.GetFromRow() - 1);
    const int toRowBase = m_options.GetToRow() == -1 ?
        gridRowCount - 1 : std::min(gridRowCount - 1, m_options.GetToRow() - 1);

    std::vector<int> visibleRows;
    visibleRows.reserve(toRowBase - fromRowBase + 1);
    for (int r = fromRowBase; r <= toRowBase; ++r)
    {
      if (m_grid->IsRowShown(r))
        visibleRows.push_back(r);
    }

    std::vector<int> visibleCols;
    visibleCols.reserve(toCol - fromCol + 1);
    for (int c = fromCol; c <= toCol; ++c)
    {
      if (m_grid->IsColShown(c))
        visibleCols.push_back(c);
    }

    if (visibleCols.empty() || visibleRows.empty())
      return;

    const int exportColCount = static_cast<int>(visibleCols.size());
    const int exportRowCount = static_cast<int>(visibleRows.size());

    // Save current state
    wxPdfColour saveDrawColour = m_doc->GetDrawColour();
    wxPdfColour saveFillColour = m_doc->GetFillColour();
    wxPdfColour saveTextColour = m_doc->GetTextColour();
    double saveLineWidth = m_doc->GetLineWidth();
    wxPdfFont saveFont;
    int saveStyle = m_doc->GetFontStyles();
    double saveSize = m_doc->GetFontSize();
    { wxLogNull noLog; saveFont = m_doc->GetCurrentFont(); }

    if (m_options.GetBodyPdfFont().IsValid())
      m_doc->SetFont(m_options.GetBodyPdfFont(), wxPDF_FONTSTYLE_REGULAR, 0);
    else
      m_doc->SetFont(m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_grid->GetDefaultCellFont());

    const double cellPadding = 4.0 / m_doc->GetScaleFactor();

    double dpi = m_dc ? static_cast<double>(m_dc->GetResolution()) : 96.0;
    double pxToDoc = 72.0 / (dpi * m_doc->GetScaleFactor());

    double rowLabelWidth = 0.0;
    if (m_options.GetIncludeRowLabels())
      rowLabelWidth = m_grid->GetRowLabelSize() * pxToDoc;

    std::vector<double> colWidths(gridColCount, 0.0);
    double totalWidth = rowLabelWidth;
    for (int col : visibleCols)
    {
      colWidths[col] = m_grid->GetColSize(col) * pxToDoc;
      totalWidth += colWidths[col];
    }

    // Measure header labels with the bold header font so wide labels aren't clipped
    if (m_options.GetHeaderPdfFont().IsValid())
      m_doc->SetFont(m_options.GetHeaderPdfFont(), wxPDF_FONTSTYLE_BOLD, m_doc->GetFontSize());
    else
    {
      wxFont hf = m_options.GetHeaderFont().IsOk() ? m_options.GetHeaderFont() : m_grid->GetLabelFont();
      hf.SetWeight(wxFONTWEIGHT_BOLD);
      m_doc->SetFont(hf);
    }
    for (int col : visibleCols)
    {
      double hdrW = m_doc->GetStringWidth(m_grid->GetColLabelValue(col)) + cellPadding;
      if (hdrW > colWidths[col])
      {
        totalWidth += hdrW - colWidths[col];
        colWidths[col] = hdrW;
      }
    }

    // Restore body font, then sample cell content
    if (m_options.GetBodyPdfFont().IsValid())
      m_doc->SetFont(m_options.GetBodyPdfFont(), wxPDF_FONTSTYLE_REGULAR, 0);
    else
      m_doc->SetFont(m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_grid->GetDefaultCellFont());

    // Just preview first 100 rows for huge grids
    const int checkCount = std::min(static_cast<int>(visibleRows.size()), 100);
    for (int col : visibleCols)
    {
      double maxW = colWidths[col];
      for (int ri = 0; ri < checkCount; ++ri)
      {
        double w = m_doc->GetStringWidth(m_grid->GetCellValue(visibleRows[ri], col)) + cellPadding;
        if (w > maxW) maxW = w;
      }
      if (maxW > colWidths[col])
      {
        totalWidth += maxW - colWidths[col];
        colWidths[col] = maxW;
      }
    }

    // Scale font down if content overflows page width
    double pageWidth = m_doc->GetPageWidth() - m_doc->GetLeftMargin() - m_doc->GetRightMargin();
    double scaledBodySize = m_doc->GetFontSize();

    if (totalWidth > pageWidth)
    {
      const double textOnlyWidth  = totalWidth  - rowLabelWidth - static_cast<double>(exportColCount) * cellPadding;
      const double textOnlyTarget = pageWidth   - rowLabelWidth - static_cast<double>(exportColCount) * cellPadding;
      if (textOnlyWidth > 0 && textOnlyTarget > 0)
      {
        const double fontScale = textOnlyTarget / textOnlyWidth;
        scaledBodySize = std::max(6.0, scaledBodySize * fontScale);
        const double actualFontScale = scaledBodySize / m_doc->GetFontSize();
        m_doc->SetFontSize(scaledBodySize);
        totalWidth = rowLabelWidth;
        for (int col : visibleCols)
        {
          colWidths[col] = std::max(0.0, colWidths[col] - cellPadding) * actualFontScale + cellPadding;
          totalWidth += colWidths[col];
        }
      }
    }

    int blocksPerPage = 1;

    if (m_options.GetFitToPage())
    {
      double scale = pageWidth / totalWidth;
      rowLabelWidth *= scale;
      for (int col : visibleCols)
        colWidths[col] *= scale;
      totalWidth = pageWidth;
    }
    else
    {
      if (totalWidth < pageWidth / 2.0)
      {
        blocksPerPage = static_cast<int>(pageWidth / (totalWidth + 5.0 / m_doc->GetScaleFactor()));
        if (blocksPerPage > 3) blocksPerPage = 3;
      }
      if (totalWidth > pageWidth)
      {
        double scale = pageWidth / totalWidth;
        rowLabelWidth *= scale;
        for (int col : visibleCols)
          colWidths[col] *= scale;
        totalWidth = pageWidth;
      }
    }

    if (m_options.GetStyle() == wxPDF_GRID_STYLE_SIMPLE)
      blocksPerPage = 1;

    const bool isSimple = (m_options.GetStyle() == wxPDF_GRID_STYLE_SIMPLE);
    double rowHeight = m_doc->GetFontSize() / m_doc->GetScaleFactor() * 1.5;
    double blockWidth = totalWidth + 5.0 / m_doc->GetScaleFactor();
    double startX = x;
    double startY = y;
    double pageHeight = m_doc->GetPageHeight() - m_doc->GetBreakMargin();

    const double thickRule = 0.5 / m_doc->GetScaleFactor();
    const double thinRule  = 0.3 / m_doc->GetScaleFactor();
    double lastDrawnY = startY;

    const auto DrawHRule = [&](double ruleX, double ruleY, double ruleLineWidth) {
      if (m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
        m_doc->SetDrawColour(m_options.GetBorderColour());
      m_doc->SetLineWidth(ruleLineWidth);
      m_doc->Line(ruleX, ruleY, ruleX + totalWidth, ruleY);
      m_doc->SetLineWidth(saveLineWidth);
      m_doc->SetDrawColour(saveDrawColour);
    };

    const auto CalcRowsPerBlock = [&](double sy, int blockRowIdx) {
      int rpb = static_cast<int>((pageHeight - sy) / rowHeight);
      if (m_options.GetIncludeColumnHeaders())
        rpb -= 1;
      if (m_options.GetShowContinued() && blockRowIdx + rpb * blocksPerPage < exportRowCount)
        rpb -= 1;
      return (rpb < 1) ? 1 : rpb;
    };

    int rowsPerBlock = CalcRowsPerBlock(startY, 0);
    int nextPageRowIdx = rowsPerBlock * blocksPerPage;

    // Map wxALIGN -> wxPDF_ALIGN
    const auto MapAlign = [](int wxAlign) -> int {
      switch (wxAlign)
      {
        case wxALIGN_RIGHT:  return wxPDF_ALIGN_RIGHT;
        case wxALIGN_CENTRE: return wxPDF_ALIGN_CENTER;
        default:             return wxPDF_ALIGN_LEFT;
      }
    };

    int colLabelHAlign = wxALIGN_CENTRE, colLabelVAlign = wxALIGN_CENTRE;
    m_grid->GetColLabelAlignment(&colLabelHAlign, &colLabelVAlign);
    int hdrPdfAlign = MapAlign(colLabelHAlign);

    const auto DrawIcon = [&](const wxBitmap& bmp, double cellX, double cellY,
                              const wxString& imgName) {
      if (!bmp.IsOk()) return;
      double imgW = bmp.GetWidth()  * pxToDoc;
      double imgH = bmp.GetHeight() * pxToDoc;
      double scale = (rowHeight * 0.8) / imgH;
      if (scale < 1.0) { imgW *= scale; imgH *= scale; }
      double iconX = cellX + 1.0 / m_doc->GetScaleFactor();
      double iconY = cellY + (rowHeight - imgH) / 2.0;
      m_doc->Image(wxString::Format(wxS("gridicon_%d_%s"), m_exportId, imgName),
                   bmp.ConvertToImage(), iconX, iconY, imgW, imgH);
    };

    const auto DrawHeader = [&](double headerX, double headerY)
    {
      m_doc->SetXY(headerX, headerY);

      if (m_options.GetHeaderPdfFont().IsValid())
        m_doc->SetFont(m_options.GetHeaderPdfFont(), wxPDF_FONTSTYLE_BOLD, scaledBodySize);
      else
      {
        wxFont hf = m_options.GetHeaderFont().IsOk() ? m_options.GetHeaderFont() : m_grid->GetLabelFont();
        hf.SetWeight(wxFONTWEIGHT_BOLD);
        m_doc->SetFont(hf);
        m_doc->SetFontSize(scaledBodySize);
      }

      if (!isSimple)
      {
        if (m_options.GetHeaderBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
          m_doc->SetFillColour(m_options.GetHeaderBackgroundColour());
        if (m_options.GetHeaderTextColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
          m_doc->SetTextColour(m_options.GetHeaderTextColour());
      }

      if (!isSimple && m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
        m_doc->SetDrawColour(m_options.GetBorderColour());

      int hdrFill = (!isSimple && m_options.GetHeaderBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN) ? 1 : 0;

      // Row-label corner cell
      if (m_options.GetIncludeRowLabels() && rowLabelWidth > 0)
      {
        m_doc->Cell(rowLabelWidth, rowHeight, m_grid->GetCornerLabelValue(),
                    isSimple ? 0 : wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER, hdrFill);
      }

      for (int col : visibleCols)
      {
        wxString hdrText = m_grid->GetColLabelValue(col);
        double cellX = m_doc->GetX();
        double cellY = m_doc->GetY();

        if (m_options.GetBitmapProvider())
        {
          wxBitmap bmp = m_options.GetBitmapProvider()->GetColHeaderBitmap(col);
          if (bmp.IsOk() && hdrPdfAlign == wxPDF_ALIGN_LEFT)
            hdrText = wxS("     ") + hdrText;
          m_doc->Cell(colWidths[col], rowHeight, hdrText, isSimple ? 0 : wxPDF_BORDER_FRAME,
                      0, hdrPdfAlign, hdrFill);
          if (bmp.IsOk())
            DrawIcon(bmp, cellX, cellY,
                     wxString::Format(wxS("hdr%d"), col));
        }
        else
        {
          m_doc->Cell(colWidths[col], rowHeight, hdrText, isSimple ? 0 : wxPDF_BORDER_FRAME,
                      0, hdrPdfAlign, hdrFill);
        }
      }

      // Reset to body font
      if (m_options.GetBodyPdfFont().IsValid())
        m_doc->SetFont(m_options.GetBodyPdfFont(), wxPDF_FONTSTYLE_REGULAR, scaledBodySize);
      else
      {
        m_doc->SetFont(m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_grid->GetDefaultCellFont());
        m_doc->SetFontSize(scaledBodySize);
      }
    };

    for (int rowIdx = 0; rowIdx < exportRowCount; )
    {
      if (rowIdx > 0 && rowIdx >= nextPageRowIdx)
      {
        // "Continued on next page" footer
        if (m_options.GetShowContinued())
        {
          if (m_options.GetBodyPdfFont().IsValid())
            m_doc->SetFont(m_options.GetBodyPdfFont(), wxPDF_FONTSTYLE_ITALIC, scaledBodySize);
          else
          {
            wxFont ff = m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_grid->GetDefaultCellFont();
            ff.SetStyle(wxFONTSTYLE_ITALIC);
            m_doc->SetFont(ff);
            m_doc->SetFontSize(scaledBodySize);
          }
          m_doc->SetTextColour(saveTextColour);
          const wxString footerText = _("Continued on next page");
          const double footerW = m_doc->GetStringWidth(footerText) + 2.0 / m_doc->GetScaleFactor();
          m_doc->SetXY(m_doc->GetPageWidth() - m_doc->GetRightMargin() - footerW,
                       startY + rowHeight * (rowsPerBlock + (m_options.GetIncludeColumnHeaders() ? 1 : 0)));
          m_doc->Cell(footerW, rowHeight, footerText, 0, 0, wxPDF_ALIGN_RIGHT);
        }

        if (isSimple)
          DrawHRule(startX, lastDrawnY, thickRule);
        m_doc->AddPage();
        startY = m_doc->GetTopMargin();

        if (m_options.GetShowContinued())
        {
          if (m_options.GetBodyPdfFont().IsValid())
            m_doc->SetFont(m_options.GetBodyPdfFont(), wxPDF_FONTSTYLE_ITALIC, scaledBodySize);
          else
          {
            wxFont cf = m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_grid->GetDefaultCellFont();
            cf.SetStyle(wxFONTSTYLE_ITALIC);
            m_doc->SetFont(cf);
            m_doc->SetFontSize(scaledBodySize);
          }
          m_doc->SetTextColour(saveTextColour);
          m_doc->Cell(0, rowHeight, _("(continued)"), 0, 1, wxPDF_ALIGN_LEFT);
          startY += rowHeight;
        }

        rowsPerBlock = CalcRowsPerBlock(startY, rowIdx);
        nextPageRowIdx = rowIdx + rowsPerBlock * blocksPerPage;
      }

      // Draw blocks for this set of rows
      for (int block = 0; block < blocksPerPage; ++block)
      {
        int blockRowIdx = rowIdx + block * rowsPerBlock;
        if (blockRowIdx >= exportRowCount) break;

        double curX = startX + block * blockWidth;
        double curY = startY;

        if (isSimple)
          DrawHRule(curX, startY, thickRule);
        if (m_options.GetIncludeColumnHeaders())
        {
          DrawHeader(curX, startY);
          curY += rowHeight;
          if (isSimple)
            DrawHRule(curX, curY, thinRule);
        }

        int rowsInBlock = std::min(rowsPerBlock, exportRowCount - blockRowIdx);
        for (int i = 0; i < rowsInBlock; ++i)
        {
          int currentRowIdx = blockRowIdx + i;
          int currentRow = visibleRows[currentRowIdx];

          m_doc->SetTextColour(saveTextColour);

          bool useAlt = !isSimple && (currentRow % 2 != 0) &&
                        m_options.GetAlternateRowBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN;

          m_doc->SetXY(curX, curY);

          // Row label cell
          if (m_options.GetIncludeRowLabels() && rowLabelWidth > 0)
          {
            if (!isSimple)
            {
              int border = 0;
              if (m_options.GetDrawRowBorders())    border |= wxPDF_BORDER_TOP | wxPDF_BORDER_BOTTOM;
              if (m_options.GetDrawColumnBorders()) border |= wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT;
              if (m_options.GetHeaderBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
                m_doc->SetFillColour(m_options.GetHeaderBackgroundColour());
              if (m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
                m_doc->SetDrawColour(m_options.GetBorderColour());
              int rlFill = (m_options.GetHeaderBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN) ? 1 : 0;
              m_doc->Cell(rowLabelWidth, rowHeight, m_grid->GetRowLabelValue(currentRow),
                          border, 0, wxPDF_ALIGN_CENTER, rlFill);
            }
            else
            {
              m_doc->Cell(rowLabelWidth, rowHeight, m_grid->GetRowLabelValue(currentRow), 0, 0, wxPDF_ALIGN_CENTER, 0);
            }
          }

          for (int col : visibleCols)
          {
            int border = 0;
            if (!isSimple)
            {
              if (m_options.GetDrawRowBorders())    border |= wxPDF_BORDER_TOP | wxPDF_BORDER_BOTTOM;
              if (m_options.GetDrawColumnBorders()) border |= wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT;
            }

            wxString text = m_grid->GetCellValue(currentRow, col);
            int cellAlign = wxPDF_ALIGN_LEFT;
            bool fill = false;

            // Per-cell attributes
            if (m_options.GetUseCellAttributes())
            {
              int hAlign = wxALIGN_LEFT, vAlign = wxALIGN_CENTRE;
              m_grid->GetCellAlignment(currentRow, col, &hAlign, &vAlign);
              cellAlign = MapAlign(hAlign);

              if (!isSimple)
              {
                wxColour bgCol = m_grid->GetCellBackgroundColour(currentRow, col);
                if (bgCol.IsOk() && bgCol != m_grid->GetDefaultCellBackgroundColour())
                {
                  m_doc->SetFillColour(bgCol);
                  fill = true;
                }
                else if (useAlt)
                {
                  m_doc->SetFillColour(m_options.GetAlternateRowBackgroundColour());
                  fill = true;
                }
              }

              wxColour fgCol = m_grid->GetCellTextColour(currentRow, col);
              if (fgCol.IsOk() && fgCol != m_grid->GetDefaultCellTextColour())
                m_doc->SetTextColour(fgCol);
              else
                m_doc->SetTextColour(saveTextColour);
            }
            else if (useAlt && !isSimple)
            {
              m_doc->SetFillColour(m_options.GetAlternateRowBackgroundColour());
              fill = true;
            }

            if (!isSimple && m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
              m_doc->SetDrawColour(m_options.GetBorderColour());

            double cellX = m_doc->GetX();
            double cellY = m_doc->GetY();

            // Bitmap provider for cell icons
            if (m_options.GetBitmapProvider())
            {
              wxBitmap bmp = m_options.GetBitmapProvider()->GetCellBitmap(currentRow, col);
              if (bmp.IsOk() && cellAlign == wxPDF_ALIGN_LEFT)
                text = wxS("     ") + text;
              m_doc->Cell(colWidths[col], rowHeight, text, border, 0, cellAlign, fill ? 1 : 0);
              if (bmp.IsOk())
                DrawIcon(bmp, cellX, cellY,
                         wxString::Format(wxS("cell%d_%d"), currentRow, col));
            }
            else
            {
              m_doc->Cell(colWidths[col], rowHeight, text, border, 0, cellAlign, fill ? 1 : 0);
            }
          }
          curY += rowHeight;
          lastDrawnY = curY;
        }
      }
      rowIdx += rowsPerBlock * blocksPerPage;
    }

    if (isSimple)
      DrawHRule(startX, lastDrawnY, thickRule);

    // Restore state
    m_doc->SetDrawColour(saveDrawColour);
    m_doc->SetFillColour(saveFillColour);
    m_doc->SetTextColour(saveTextColour);
    m_doc->SetLineWidth(saveLineWidth);
    if (saveFont.IsValid())
      m_doc->SetFont(saveFont, saveStyle, saveSize);
  }

private:
  wxPdfDocument*      m_doc;
  const wxGrid*       m_grid;
  const wxPdfGridOptions& m_options;
  wxPdfDC*            m_dc;
  int                 m_exportId;

  static int ms_exportCounter;
};

int wxPdfGridExporter::ms_exportCounter = 0;

// --- API Implementation

void wxPdfDocument::AddGrid(const wxGrid* grid, const wxPdfGridOptions& options)
{
  if (PageNo() == 0)
    AddPage();
  wxPdfGridExporter exporter(this, grid, options);
  exporter.Export(GetX(), GetY());
}

void wxPdfDC::DrawGrid(const wxGrid* grid, wxCoord x, wxCoord y, const wxPdfGridOptions& options)
{
  wxPdfDCImpl* impl = static_cast<wxPdfDCImpl*>(GetImpl());
  wxPdfGridExporter exporter(this, grid, options);
  exporter.Export(impl->ScaleLogicalToPdfX(x), impl->ScaleLogicalToPdfY(y));
}

#endif // wxUSE_GRID
