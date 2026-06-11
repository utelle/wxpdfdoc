///////////////////////////////////////////////////////////////////////////////
// Name:        pdflistctrl.cpp
// Purpose:     
// Author:      Ulrich Telle
// Created:     2026-06-10
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_LISTCTRL

#include "wx/pdflistctrl.h"
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include "wx/pdfdocument.h"
#include "wx/pdfdc.h"

#include <vector>
#include <numeric>
#include <algorithm>

wxPdfListCtrlOptions::wxPdfListCtrlOptions()
{
  m_drawRowBorders = true;
  m_drawColumnBorders = true;
  m_showContinued = true;
  m_fitToPage = false;
  m_style = wxPDF_LISTCTRL_STYLE_GRID;
  m_borderColour = wxPdfColour();
}

// --- Exporter Engine ---

class wxPdfListCtrlExporter
{
public:
  wxPdfListCtrlExporter(wxPdfDocument* doc, wxListCtrl* list, const wxPdfListCtrlOptions& options)
    : m_doc(doc), m_list(list), m_options(options), m_dc(NULL)
  {
  }

  wxPdfListCtrlExporter(wxPdfDC* dc, wxListCtrl* list, const wxPdfListCtrlOptions& options)
    : m_doc(dc->GetPdfDocument()), m_list(list), m_options(options), m_dc(dc)
  {
  }

  void Export(double x, double y)
  {
    if (!m_doc || !m_list || m_list->GetColumnCount() == 0)
        return;

    // Save current state
    wxPdfColour saveDrawColour = m_doc->GetDrawColour();
    wxPdfColour saveFillColour = m_doc->GetFillColour();
    wxPdfColour saveTextColour = m_doc->GetTextColour();
    double saveLineWidth = m_doc->GetLineWidth();
    wxPdfFont saveFont;
    int saveStyle = m_doc->GetFontStyles();
    double saveSize = m_doc->GetFontSize();
    { wxLogNull noLog; saveFont = m_doc->GetCurrentFont(); }

    // Calculate column widths
    size_t colCount = m_list->GetColumnCount();
    std::vector<double> colWidths(colCount);
    
    m_doc->SetFont(m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_list->GetFont());
    // Cell padding in user units: 4pt converted to the document's unit system
    const double cellPadding = 4.0 / m_doc->GetScaleFactor();
    double totalWidth = 0;
    for (size_t col = 0; col < colCount; ++col)
    {
      wxListItem item;
      item.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_WIDTH);
      m_list->GetColumn(col, item);

      double headerWidth = m_doc->GetStringWidth(item.GetText()) + cellPadding;
      double maxContentWidth = 0;

      // Check first 100 items for width optimization
      int rowCount = m_list->GetItemCount();
      int checkRows = std::min(rowCount, 100);
      wxImageList* imgList = m_list->GetImageList(wxIMAGE_LIST_SMALL);
      for (int row = 0; row < checkRows; ++row)
      {
        double w = m_doc->GetStringWidth(m_list->GetItemText(row, col)) + cellPadding;
        
        // Add icon width if present
        if (imgList)
        {
          wxListItem info;
          info.SetId(row);
          info.SetColumn(col);
          info.SetMask(wxLIST_MASK_IMAGE);
          if (m_list->GetItem(info) && info.GetImage() != -1)
          {
            w += 20.0 / m_doc->GetScaleFactor(); // Approx icon width + padding in user units
          }
        }
        
        if (w > maxContentWidth) maxContentWidth = w;
      }
      
      colWidths[col] = std::max(headerWidth, maxContentWidth);
      totalWidth += colWidths[col];
    }

    // Adjust column widths to fit the available printable width
    double pageWidth = m_doc->GetPageWidth() - m_doc->GetLeftMargin() - m_doc->GetRightMargin();
    int blocksPerPage = 1;

    // When content overflows the page, shrink the font so every cell's text fits within
    // its measured column width after scaling. String widths scale linearly with font size,
    // so one pass is exact (padding is fixed, so we account for it separately).
    double scaledBodySize = m_doc->GetFontSize();
    if (totalWidth > pageWidth)
    {
      const double textOnlyWidth  = totalWidth - static_cast<double>(colCount) * cellPadding;
      const double textOnlyTarget = pageWidth  - static_cast<double>(colCount) * cellPadding;
      if (textOnlyWidth > 0 && textOnlyTarget > 0)
      {
        const double fontScale = textOnlyTarget / textOnlyWidth;
        scaledBodySize = std::max(6.0, scaledBodySize * fontScale);
        const double actualFontScale = scaledBodySize / m_doc->GetFontSize();
        m_doc->SetFontSize(scaledBodySize);
        totalWidth = 0;
        for (size_t col = 0; col < colCount; ++col)
        {
          colWidths[col] = std::max(0.0, colWidths[col] - cellPadding) * actualFontScale + cellPadding;
          totalWidth += colWidths[col];
        }
      }
    }

    if (m_options.GetFitToPage())
    {
      // Stretch (or shrink) all columns proportionally to fill the full page width.
      // Multi-column layout is incompatible with full-width mode.
      double scale = pageWidth / totalWidth;
      for (size_t col = 0; col < colCount; ++col)
        colWidths[col] *= scale;
      totalWidth = pageWidth;
    }
    else
    {
      if (totalWidth < pageWidth / 2.0)
      {
        blocksPerPage = static_cast<int>(pageWidth / (totalWidth + 5.0 / m_doc->GetScaleFactor()));
        if (blocksPerPage > 3) blocksPerPage = 3; // Limit to 3 blocks
      }

      if (totalWidth > pageWidth)
      {
        double scale = pageWidth / totalWidth;
        for (size_t col = 0; col < colCount; ++col)
          colWidths[col] *= scale;
        totalWidth = pageWidth;
      }
    }

    // SIMPLE style suppresses multi-column layout (rules span the full table width)
    if (m_options.GetStyle() == wxPDF_LISTCTRL_STYLE_SIMPLE)
      blocksPerPage = 1;

    // Drawing — font size is in points; divide by scale factor to get user units
    double rowHeight = m_doc->GetFontSize() / m_doc->GetScaleFactor() * 1.5;
    double blockWidth = totalWidth + 5.0 / m_doc->GetScaleFactor(); // Margin between blocks in user units
    double startX = x;
    double startY = y;
    double pageHeight = m_doc->GetPageHeight() - m_doc->GetBreakMargin();

    const bool isSimple = (m_options.GetStyle() == wxPDF_LISTCTRL_STYLE_SIMPLE);
    // booktabs-style rule weights in user units (toprule/bottomrule thicker than midrule)
    const double thickRule = 0.5 / m_doc->GetScaleFactor();
    const double thinRule  = 0.3 / m_doc->GetScaleFactor();
    double lastDrawnY = startY;

    auto DrawHRule = [&](double ruleX, double ruleY, double ruleLineWidth) {
      if (m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
        m_doc->SetDrawColour(m_options.GetBorderColour());
      m_doc->SetLineWidth(ruleLineWidth);
      m_doc->Line(ruleX, ruleY, ruleX + totalWidth, ruleY);
      m_doc->SetLineWidth(saveLineWidth);
      m_doc->SetDrawColour(saveDrawColour);
    };

    int totalRows = m_list->GetItemCount();

    auto CalcRowsPerBlock = [&](double sy, int currentRow) {
      int rpb = static_cast<int>((pageHeight - sy) / rowHeight) - 1; // -1 for header
      if (m_options.GetShowContinued() && currentRow + rpb * blocksPerPage < totalRows)
        rpb -= 1; // Reserve space for "Continued" footer only when another page follows
      return (rpb < 1) ? totalRows : rpb;
    };

    int rowsPerBlock = CalcRowsPerBlock(startY, 0);
    int nextPageRow = rowsPerBlock * blocksPerPage;

    auto DrawHeader = [&](double headerX, double headerY)
    {
      m_doc->SetXY(headerX, headerY);
      
      // Determine header font
      wxFont headerFont = m_options.GetHeaderFont().IsOk() ? m_options.GetHeaderFont() : m_list->GetFont();
      headerFont.SetWeight(wxFONTWEIGHT_BOLD);
      m_doc->SetFont(headerFont);
      m_doc->SetFontSize(scaledBodySize);

      if (!isSimple)
      {
        if (m_options.GetHeaderBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
          m_doc->SetFillColour(m_options.GetHeaderBackgroundColour());
        if (m_options.GetHeaderTextColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
          m_doc->SetTextColour(m_options.GetHeaderTextColour());
      }

      for (size_t col = 0; col < colCount; ++col)
      {
        wxListItem item;
        item.SetMask(wxLIST_MASK_TEXT);
        m_list->GetColumn(col, item);

        if (!isSimple && m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
          m_doc->SetDrawColour(m_options.GetBorderColour());

        int hdrFill = (!isSimple && m_options.GetHeaderBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN) ? 1 : 0;
        m_doc->Cell(colWidths[col], rowHeight, item.GetText(), isSimple ? 0 : wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER, hdrFill);
      }
      
      // Reset to body font
      m_doc->SetFont(m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_list->GetFont());
      m_doc->SetFontSize(scaledBodySize);
    };

    for (int row = 0; row < totalRows; )
    {
      // Check if we need a new page
      if (row > 0 && row >= nextPageRow)
      {
        // Add "Continued on next page" if another page follows
        if (m_options.GetShowContinued())
        {
          wxFont footerFont = m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_list->GetFont();
          footerFont.SetStyle(wxFONTSTYLE_ITALIC);
          m_doc->SetFont(footerFont);
          m_doc->SetFontSize(scaledBodySize);
          m_doc->SetTextColour(saveTextColour);
          const wxString footerText = _("Continued on next page");
          const double footerW = m_doc->GetStringWidth(footerText) + 2.0 / m_doc->GetScaleFactor();
          m_doc->SetXY(m_doc->GetPageWidth() - m_doc->GetRightMargin() - footerW,
                       startY + rowHeight * (rowsPerBlock + 1));
          m_doc->Cell(footerW, rowHeight, footerText, 0, 0, wxPDF_ALIGN_RIGHT);
        }

        if (isSimple)
          DrawHRule(startX, lastDrawnY, thickRule);
        m_doc->AddPage();
        startY = m_doc->GetTopMargin();

        if (m_options.GetShowContinued())
        {
            wxFont continuedFont = m_options.GetBodyFont().IsOk() ? m_options.GetBodyFont() : m_list->GetFont();
            continuedFont.SetStyle(wxFONTSTYLE_ITALIC);
            m_doc->SetFont(continuedFont);
            m_doc->SetFontSize(scaledBodySize);
            m_doc->SetTextColour(saveTextColour);
            m_doc->Cell(0, rowHeight, _("(continued)"), 0, 1, wxPDF_ALIGN_LEFT);
            startY += rowHeight;
        }

        // Recalculate rows per block for this page's available height,
        // then advance the page-break threshold by the new stride
        rowsPerBlock = CalcRowsPerBlock(startY, row);
        nextPageRow += rowsPerBlock * blocksPerPage;
      }

      // Draw blocks for this set of rows
      for (int block = 0; block < blocksPerPage; ++block)
      {
        int rowIdx = row + block * rowsPerBlock;
        if (rowIdx >= totalRows) break;

        double curX = startX + block * blockWidth;
        double curY = startY;

        if (isSimple)
          DrawHRule(curX, startY, thickRule);
        DrawHeader(curX, startY);
        curY += rowHeight;
        if (isSimple)
          DrawHRule(curX, curY, thinRule);

        // Draw rows for this block
        int rowsInBlock = std::min(rowsPerBlock, totalRows - rowIdx);
        for (int i = 0; i < rowsInBlock; ++i)
        {
          int currentRow = rowIdx + i;
          
          m_doc->SetTextColour(saveTextColour); // Ensure text color is reset for each row
          
          bool useAlt = !isSimple && (currentRow % 2 != 0) && m_options.GetAlternateRowBackgroundColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN;
          if (useAlt)
          {
            m_doc->SetFillColour(m_options.GetAlternateRowBackgroundColour());
          }

          m_doc->SetXY(curX, curY);
          for (size_t col = 0; col < colCount; ++col)
          {
            int border = 0;
            if (!isSimple)
            {
              if (m_options.GetDrawRowBorders())
                border |= wxPDF_BORDER_TOP | wxPDF_BORDER_BOTTOM;
              if (m_options.GetDrawColumnBorders())
                border |= wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT;
            }

            wxString text = m_list->GetItemText(currentRow, col);

            // Set cell colors
            bool fill = false;
            if (!isSimple)
            {
              wxColour bgColour = m_list->GetItemBackgroundColour(currentRow);
              if (bgColour.IsOk())
              {
                m_doc->SetFillColour(bgColour);
                fill = true;
              }
              else if (useAlt)
              {
                m_doc->SetFillColour(m_options.GetAlternateRowBackgroundColour());
                fill = true;
              }

              if (m_options.GetBorderColour().GetColourType() != wxPDF_COLOURTYPE_UNKNOWN)
                m_doc->SetDrawColour(m_options.GetBorderColour());
            }
            
            wxColour textColour = m_list->GetItemTextColour(currentRow);
            if (textColour.IsOk())
            {
              m_doc->SetTextColour(textColour);
            }
            else
            {
              m_doc->SetTextColour(saveTextColour);
            }
            
            // Draw icon if available
            wxImageList* imgList = m_list->GetImageList(wxIMAGE_LIST_SMALL);
            if (imgList)
            {
              wxListItem info;
              info.SetId(currentRow);
              info.SetColumn(col);
              info.SetMask(wxLIST_MASK_IMAGE);
              m_list->GetItem(info);
              if (info.GetImage() != -1)
              {
                wxBitmap bmp = imgList->GetBitmap(info.GetImage());
                if (bmp.IsOk())
                {
                  // bmp.GetWidth/Height() are already in device pixels; convert to
                  // document user units via DPI and the document's scale factor
                  double dpi = m_dc ? static_cast<double>(m_dc->GetResolution()) : 96.0;
                  double docScale = 72.0 / (dpi * m_doc->GetScaleFactor());
                  double imgW = bmp.GetWidth()  * docScale;
                  double imgH = bmp.GetHeight() * docScale;
                  double scale = (rowHeight * 0.8) / imgH;
                  if (scale < 1.0) { imgW *= scale; imgH *= scale; }
                  
                  double iconX = m_doc->GetX() + 1;
                  double iconY = m_doc->GetY() + (rowHeight - imgH) / 2;
                  m_doc->Image(wxString::Format(wxS("listicon_%d"), info.GetImage()),
                                                bmp.ConvertToImage(), iconX, iconY, imgW, imgH);
                  text = wxS("     ") + text;
                }
              }
            }

            m_doc->Cell(colWidths[col], rowHeight, text, border, 0, wxPDF_ALIGN_LEFT, fill ? 1 : 0);
          }
          curY += rowHeight;
          lastDrawnY = curY;
        }
      }
      row += rowsPerBlock * blocksPerPage;
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
  wxPdfDocument* m_doc;
  wxListCtrl*    m_list;
  const wxPdfListCtrlOptions& m_options;
  wxPdfDC*       m_dc;
};

// --- API Implementation ---

void wxPdfDocument::AddList(wxListCtrl* list, const wxPdfListCtrlOptions& options)
{
  if (PageNo() == 0)
    AddPage();
  wxPdfListCtrlExporter exporter(this, list, options);
  exporter.Export(GetX(), GetY());
}

void wxPdfDC::DrawList(wxListCtrl* list, wxCoord x, wxCoord y, const wxPdfListCtrlOptions& options)
{
  wxPdfDCImpl* impl = static_cast<wxPdfDCImpl*>(GetImpl());
  wxPdfListCtrlExporter exporter(this, list, options);
  exporter.Export(impl->ScaleLogicalToPdfX(x), impl->ScaleLogicalToPdfY(y));
}

#endif // wxUSE_LISTCTRL
