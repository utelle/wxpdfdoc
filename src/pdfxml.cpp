///////////////////////////////////////////////////////////////////////////////
// Name:        pdfxml.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-12-03
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence + RDS Data Security license
///////////////////////////////////////////////////////////////////////////////

/// \file pdfxml.cpp Implementation of the wxPdfDocument markup

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes
#include <wx/sstream.h>

#include "wx/pdfdoc.h"
#include "wx/pdfxml.h"

static wxString
GetNodeContent(const wxXmlNode *node)
{
  const wxXmlNode *n = node;
  if (n == NULL) return wxEmptyString;
  n = n->GetChildren();
  while (n)
  {
    if (n->GetType() == wxXML_TEXT_NODE ||
        n->GetType() == wxXML_CDATA_SECTION_NODE)
      return n->GetContent();
    n = n->GetNext();
  }
  return wxEmptyString;
}

// --- wxPdfCellContext

wxPdfCellContext::wxPdfCellContext(double maxWidth, wxPdfAlignment hAlign, wxPdfAlignment vAlign)
{
  m_maxWidth = maxWidth;
  m_hAlign = hAlign;
  m_vAlign = vAlign;
  m_currentLine = 0;
  m_currentContext = 0;
  m_aligned = false;
  m_lastChar = 0;
  m_spaceWidth = 0;
  m_lineDelta = 0;
  m_height = 0;
  m_table = NULL;
  AddLine();
}

wxPdfCellContext::~wxPdfCellContext()
{
  size_t j;
  for (j = 0; j < m_contexts.GetCount(); j++)
  {
    wxPdfCellContext* context = static_cast<wxPdfCellContext*>(m_contexts[j]);
    delete context;
  }
  if (m_table != NULL)
  {
    delete m_table;
  }
}

void
wxPdfCellContext::AddLine()
{
  m_linewidth.Add(0);
  m_spaces.Add(0);
}

void
wxPdfCellContext::AddLastLineValues(double width, int spaces)
{
  m_linewidth.Last() += width;
  m_spaces.Last() += spaces;
}

void
wxPdfCellContext::AddCurrentLineWidth(double width)
{
  if (m_currentLine < m_linewidth.GetCount())
  {
    m_linewidth[m_currentLine] += width;
  }
}

void
wxPdfCellContext::AddCurrentLineSpaces(int spaces)
{
  if (m_currentLine < m_spaces.GetCount())
  {
    m_spaces[m_currentLine] += spaces;
  }
}

void
wxPdfCellContext::MarkLastLine()
{
  if (m_spaces.Last() > 0)
  {
    m_spaces.Last() *= -1;
  }
}

bool
wxPdfCellContext::IsCurrentLineMarked()
{
  bool marked = false;
  if (m_currentLine < m_linewidth.GetCount())
  {
    marked = (m_spaces[m_currentLine] < 0);
  }
  return marked;
}


void
wxPdfCellContext::IncrementCurrentLine()
{
  if (m_currentLine < m_linewidth.GetCount()-1)
  {
    m_currentLine++;
  }
  m_aligned = false;
}

double
wxPdfCellContext::GetLineDelta()
{
  if (!m_aligned)
  {
    m_lineDelta = m_maxWidth - GetCurrentLineWidth();
  }
  return m_lineDelta;
}

double
wxPdfCellContext::GetCurrentLineWidth()
{
  double linewidth = 0;
  if (m_currentLine < m_linewidth.GetCount())
  {
    linewidth = m_linewidth[m_currentLine];
  }
  return linewidth;
}

double
wxPdfCellContext::GetLastLineWidth()
{
  return m_linewidth.Last();
}

int
wxPdfCellContext::GetCurrentLineSpaces()
{
  int spaces = 0;
  if (m_currentLine < m_spaces.GetCount())
  {
    spaces = m_spaces[m_currentLine];
    if (spaces < 0) spaces = -spaces;
  }
  return spaces;
}

void
wxPdfCellContext::IncrementCurrentContext()
{
  if (m_currentContext < m_contexts.GetCount()-1)
  {
    m_currentContext++;
  }
}

wxPdfCellContext*
wxPdfCellContext::GetCurrentContext()
{
  wxPdfCellContext* context = NULL;
  if (m_currentContext < m_contexts.GetCount())
  {
    context = static_cast<wxPdfCellContext*>(m_contexts[m_currentContext]);
  }
  return context;
}

void
wxPdfCellContext::AppendContext(wxPdfCellContext* context)
{
  m_contexts.Add(context);
}

// --- wxPdfTableCell

wxPdfTableCell::wxPdfTableCell(wxXmlNode* cell, int row, int col, int rows, int cols)
{
  m_cell   = cell;
  m_width  = -1.0;
  m_height = -1.0;
  m_row = row;
  m_col = col;
  m_rowSpan = rows;
  m_colSpan = cols;
  
  SetBorder(wxPDF_BORDER_NONE);
  m_context = NULL;
  m_hasCellColor = false;
}

wxPdfTableCell::~wxPdfTableCell()
{
  if (m_context != NULL)
  {
    delete m_context;
  }
}

// --- wxPdfTable

wxPdfTable::wxPdfTable(wxPdfDocument* document)
{
  m_document = document;
  m_nRows = 0;
  m_nCols = 0;
  m_totalWidth  = 0;
  m_totalHeight = 0;
  m_headHeight  = 0;
  m_pad = 0;
  m_headRowFirst = 0;
  m_headRowLast  = 0;
  m_bodyRowFirst = 0;
  m_bodyRowLast  = 0;
}

wxPdfTable::~wxPdfTable()
{
  // Free all cells
  wxPdfCellHashMap::iterator cell;
  for (cell = m_table.begin(); cell != m_table.end(); cell++)
  {
    if (cell->second != NULL)
    {
      delete cell->second;
    }
  }
}

void
wxPdfTable::SetCellDimensions(double maxWidth)
{
  size_t row, col;
  double defaultWidth = (m_nCols > 0) ? maxWidth / m_nCols : 0;
  for (col = 0; col < m_nCols; col++)
  {
    if (col >= m_colWidths.size() || m_colWidths[col] <= 0)
    {
      SetColumnWidth(col, defaultWidth);
    }
  }

  if (m_totalWidth > maxWidth)
  {
    double factor = maxWidth / m_totalWidth;
    for (col = 0; col < m_colWidths.size(); col++)
    {
      m_colWidths[col] /= factor;
    }
  }

  double rowHeight;
  wxPdfBoolHashMap rowSpans;
  int rowSpan;
  int maxRowSpan = 1;
  for (row = 0; row < m_nRows; row++)
  {
    rowHeight = 0;
    for (col = 0; col < m_nCols; col++)
    {
      wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
      if (foundCell != m_table.end())
      {
        wxPdfTableCell* cell = foundCell->second;
        int span;
        double cellWidth = 0;
        for (span = 0; span < cell->GetColSpan(); span++)
        {
          cellWidth += m_colWidths[col+span];
        }
        if (cellWidth > 2*m_pad) cellWidth -= (2*m_pad);
        cell->SetWidth(cellWidth);
        wxPdfCellContext* cellContext = new wxPdfCellContext(cellWidth, cell->GetHAlign());
        cell->SetContext(cellContext);
        m_document->PrepareXmlCell(cell->GetXmlNode(), *cellContext);
        double height = cellContext->GetHeight() + 2*m_pad;
        cell->SetHeight(height);
        rowSpan = cell->GetRowSpan();
        rowSpans[rowSpan] = true;
        if (rowSpan == 1 && height > rowHeight)
        {
          rowHeight = height;
        }
        else
        {
          if (rowSpan > maxRowSpan)
          {
            maxRowSpan = rowSpan;
          }
        }
      }
    }
    m_rowHeights[row] = (rowHeight < m_minHeights[row]) ? m_minHeights[row] : rowHeight;
  }

  for (rowSpan = 2; rowSpan <= maxRowSpan; rowSpan++)
  {
    wxPdfBoolHashMap::iterator currentRowSpan = rowSpans.find(rowSpan);
    if (currentRowSpan != rowSpans.end())
    {
      for (row = 0; row < m_nRows; row++)
      {
        rowHeight = 0;
        for (col = 0; col < m_nCols; col++)
        {
          wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
          if (foundCell != m_table.end())
          {
            wxPdfTableCell* cell = foundCell->second;
            if (rowSpan == cell->GetRowSpan())
            {
              rowHeight = 0;
              int span;
              for (span = cell->GetRowSpan()-1; span >= 0; span--)
              {
                rowHeight += m_rowHeights[row+span];
              }
              if (cell->GetHeight() > rowHeight)
              {
                double delta = (cell->GetHeight() - rowHeight) / cell->GetRowSpan();
                for (span = cell->GetRowSpan()-1; span >= 0; span--)
                {
                  m_rowHeights[row+span] += delta;
                }
              }
            }
          }
        }
      }
    }
  }
  m_headHeight = 0;
  for (row = m_headRowFirst; row < m_headRowLast; row++)
  {
    m_headHeight += m_rowHeights[row];
  }
  m_totalHeight = 0;
  for (row = m_bodyRowFirst; row < m_bodyRowLast; row++)
  {
    m_totalHeight += m_rowHeights[row];
  }
}

void
wxPdfTable::InsertCell(wxPdfTableCell* cell)
{
  size_t x = cell->GetCol();
  size_t y = cell->GetRow();
  size_t w = cell->GetColSpan();
  size_t h = cell->GetRowSpan();
  m_table[(y << 16)|x] = cell;
  if (x + w > m_nCols)
  {
    m_nCols = x + w;
  }
  if (y + h > m_nRows)
  {
    m_nRows = y + h;
  }
}

void
wxPdfTable::Write()
{
  bool writeHeader = m_headRowLast > m_headRowFirst;
  bool newPage = false;
  double saveLeftMargin = m_document->GetLeftMargin();
  double x, y;
  size_t row, headRow;
  y = m_document->GetY();
  double breakMargin = m_document->GetBreakMargin();
  double pageHeight = m_document->GetPageHeight();
  double yMax = pageHeight - breakMargin;
  if (y + m_headHeight + m_rowHeights[m_bodyRowFirst] > yMax)
  {
    newPage = true;
  }
  for (row = m_bodyRowFirst; row < m_bodyRowLast; row++)
  {
    if (!newPage && (y + m_rowHeights[row] > yMax))
    {
      newPage = true;
    }
    if (newPage)
    {
      newPage = false;
      m_document->AddPage();
      writeHeader = m_headRowLast > m_headRowFirst;
      y = m_document->GetY();
    }
    if (writeHeader)
    {
      writeHeader = false;
      for (headRow = m_headRowFirst; headRow < m_headRowLast; headRow++)
      {
        x = saveLeftMargin;
        WriteRow(headRow, x, y);
        y += m_rowHeights[headRow];
      }
    }
    x = saveLeftMargin;
    WriteRow(row, x, y);
    y += m_rowHeights[row];
  }
}

void
wxPdfTable::WriteRow(size_t row, double x, double y)
{
  bool isHeaderRow = (row >= m_headRowFirst && row < m_headRowLast);
  size_t col;
  int rowspan, colspan;
  double w, h;
  m_document->SetXY(x, y+m_pad);
  for (col = 0; col < m_nCols; col++)
  {
    wxPdfCellHashMap::iterator foundCell = m_table.find((row << 16) | col);
    if (foundCell != m_table.end())
    {
      wxPdfTableCell* cell = foundCell->second;
      w = 0;
      for (colspan = 0; colspan < cell->GetColSpan(); colspan++)
      {
        w += m_colWidths[col+colspan];
      }
      h = 0;
      for (rowspan = 0; rowspan < cell->GetRowSpan(); rowspan++)
      {
        h += m_rowHeights[row+rowspan];
      }
      if (cell->HasCellColor())
      {
        wxPdfColour saveFillColor = m_document->GetFillColor();
        m_document->SetFillColor(cell->GetCellColor());
        m_document->Rect(x, y, w, h, wxPDF_STYLE_FILL);
        m_document->SetFillColor(saveFillColor);
      }
      int border = cell->GetBorder();
      if ((border & wxPDF_BORDER_FRAME) == wxPDF_BORDER_FRAME)
      {
        m_document->Rect(x, y, w, h);
      }
      else
      {
        if (border & wxPDF_BORDER_LEFT)   m_document->Line(x,   y,   x,   y+h);
        if (border & wxPDF_BORDER_TOP)    m_document->Line(x,   y,   x+w, y);
        if (border & wxPDF_BORDER_BOTTOM) m_document->Line(x,   y+h, x+w, y+h);
        if (border & wxPDF_BORDER_RIGHT)  m_document->Line(x+w, y,   x+w, y+h);
      }
      m_document->SetLeftMargin(x+m_pad);
      double delta = h - cell->GetHeight();
      switch (cell->GetVAlign())
      {
        case wxPDF_ALIGN_BOTTOM:
          m_document->SetXY(x+m_pad, y+m_pad+delta);
          break;
        case wxPDF_ALIGN_MIDDLE:
          m_document->SetXY(x+m_pad, y+m_pad+0.5*delta);
          break;
        case wxPDF_ALIGN_TOP:
        default:
          m_document->SetXY(x+m_pad, y+m_pad);
          break;
      }
      m_document->WriteXmlCell(cell->GetXmlNode(), *(cell->GetContext()));
      if (isHeaderRow)
      {
        // For header rows it is necessary to prepare the cells for reprocessing
        delete cell->GetContext();
        wxPdfCellContext* cellContext = new wxPdfCellContext(cell->GetWidth(), cell->GetHAlign());
        cell->SetContext(cellContext);
        m_document->PrepareXmlCell(cell->GetXmlNode(), *cellContext);
      }
    }
    x += m_colWidths[col];
  }
}

void
wxPdfTable::SetColumnWidth(int col, double width)
{ 
  m_colWidths[col] = width;
  m_totalWidth += width;
}

// ----- wxPdfDocument

void
wxPdfDocument::PrepareXmlTable(wxXmlNode* node, wxPdfCellContext& context)
{
  wxPdfTable* table = context.GetTable();
  double maxWidth = context.GetMaxWidth();
  wxPdfBoolHashMap cellused;
  int coldef = 0;
  int colundef = 0;
  int row = 0;
  int col;
  int i, j;

  wxXmlNode *child = node->GetChildren();
  while (child)
  {
    wxString name = (child->GetName()).Lower();
    if (name == wxT("colgroup"))
    {
      wxXmlNode *colChild = child->GetChildren();
      while (colChild)
      {
        if ((colChild->GetName()).Lower() == wxT("col"))
        {
          long colspan;
          wxString span = colChild->GetPropVal(_T("span"), _T("1"));
          if (span.Length() > 0 && span.ToLong(&colspan))
          {
            if (colspan < 1) colspan = 1;
          }
          else
          {
            colspan = 1;
          }
          double colwidth = 0;
          wxString width = colChild->GetPropVal(_T("width"), _T("0"));
          if (width.Length() > 0)
          {
            colwidth = String2Double(width);
            if (colwidth < 0) colwidth = 0;
          }
          for (col = 0; col < colspan; col++)
          {
            table->SetColumnWidth(coldef++, colwidth);
            if (colwidth <= 0) colundef++;
          }
        }
        colChild = colChild->GetNext();
      }
    }
    else if (name == wxT("thead") || name == wxT("tbody"))
    {
      wxString oddColor = (child->GetPropVal(_T("odd"), _T(""))).Lower();
      wxString evenColor = (child->GetPropVal(_T("even"), _T(""))).Lower();
      bool isHeader = name == wxT("thead");
      if (isHeader)
      {
        table->SetHeadRowFirst(row);
      }
      else
      {
        table->SetBodyRowFirst(row);
      }
      wxXmlNode *rowChild = child->GetChildren();
      int rowCount = 0;
      while (rowChild)
      {
        if ((rowChild->GetName()).Lower() == wxT("tr"))
        {
          wxString rowColor = (rowChild->GetPropVal(_T("bgcolor"), _T(""))).Lower();
          rowCount++;
          if (rowColor.Length() == 0)
          {
            if (rowCount % 2 == 0)
            {
              // even row numbers
              if (evenColor.Length() > 0) rowColor = evenColor;
            }
            else
            {
              // odd row numbers
              if (oddColor.Length() > 0) rowColor = oddColor;
            }
          }
          double rowHeight = 0;
          wxString height = (rowChild->GetPropVal(_T("height"), _T("0"))).Lower();
          if (height.Length() > 0)
          {
            rowHeight = String2Double(height);
            if (rowHeight < 0) rowHeight = 0;
          }
          table->SetMinRowHeight(row, rowHeight);
          wxXmlNode *colChild = rowChild->GetChildren();
          col = 0;
          while (colChild)
          {
            if ((colChild->GetName()).Lower() == wxT("td"))
            {
              int border = wxPDF_BORDER_NONE;
              wxString strBorder = (colChild->GetPropVal(_T("border"), _T(""))).Upper();
              if (strBorder.Length() > 0)
              {
                if (strBorder.Contains(_T("L"))) border |= wxPDF_BORDER_LEFT;
                if (strBorder.Contains(_T("T"))) border |= wxPDF_BORDER_TOP;
                if (strBorder.Contains(_T("B"))) border |= wxPDF_BORDER_BOTTOM;
                if (strBorder.Contains(_T("R"))) border |= wxPDF_BORDER_RIGHT;
              }
              else if (table->HasBorder())
              {
                border = wxPDF_BORDER_FRAME;
              }

              wxString align = (colChild->GetPropVal(_T("align"), _T("left"))).Lower();
              wxPdfAlignment hAlignment = wxPDF_ALIGN_LEFT;
              if (align == _T("right"))       hAlignment = wxPDF_ALIGN_RIGHT;
              else if (align == _T("center")) hAlignment = wxPDF_ALIGN_CENTER;
              align = (colChild->GetPropVal(_T("valign"), _T("top"))).Lower();
              wxPdfAlignment vAlignment = wxPDF_ALIGN_TOP;
              if (align == _T("bottom"))      vAlignment = wxPDF_ALIGN_BOTTOM;
              else if (align == _T("middle")) vAlignment = wxPDF_ALIGN_MIDDLE;
              else if (align == _T("center")) vAlignment = wxPDF_ALIGN_MIDDLE;

              wxString bgColor = (colChild->GetPropVal(_T("bgcolor"), _T(""))).Lower();
              long rs;
              wxString rowspan = colChild->GetPropVal(_T("rowspan"), _T("1"));
              if (rowspan.Length() > 0 && rowspan.ToLong(&rs))
              {
                if (rs < 1) rs = 1;
              }
              else
              {
                rs = 1;
              }
              long cs;
              wxString colspan = colChild->GetPropVal(_T("colspan"), _T("1"));
              if (colspan.Length() > 0 && colspan.ToLong(&cs))
              {
                if (cs < 1) cs = 1;
              }
              else
              {
                cs = 1;
              }
              while (cellused.find((row<<16)|col) != cellused.end())
              {
                ++col;
              }
              for (i = 0; i < cs; i++)
              {
                for (j = 0; j < rs; j++)
                {
                  cellused[((j+row)<<16)|(i+col)] = true;
                }
              }
              wxPdfTableCell* cell = new wxPdfTableCell(colChild, row, col, rs, cs);
              cell->SetHAlign(hAlignment);
              cell->SetVAlign(vAlignment);
              cell->SetBorder(border);
              if (bgColor.Length() > 0)
              {
                cell->SetCellColor(wxPdfColour(bgColor));
              }
              else if (rowColor.Length() > 0)
              {
                cell->SetCellColor(wxPdfColour(rowColor));
              }
              table->InsertCell(cell);
            }
            colChild = colChild->GetNext();
          }
          row++;
        }
        if (isHeader)
        {
          table->SetHeadRowLast(row);
        }
        else
        {
          table->SetBodyRowLast(row);
        }
        rowChild = rowChild->GetNext();
      }
    }
    child = child->GetNext();
  }
  table->SetCellDimensions(maxWidth);
  context.AddHeight(table->GetTotalHeight());
}

void
wxPdfDocument::WriteXmlTable(wxPdfCellContext& context)
{
  double saveLeftMargin = GetLeftMargin();
  double saveRightMargin = GetRightMargin();
  wxPdfTable* table = context.GetTable();
  double maxWidth = context.GetMaxWidth();
  double tableWidth = table->GetTotalWidth();
  double delta = 0;
  if (tableWidth < maxWidth)
  {
    wxPdfAlignment align = context.GetHAlign();
    switch (align)
    {
      case wxPDF_ALIGN_CENTER:
        delta = 0.5 * (maxWidth - tableWidth);
        break;
      case wxPDF_ALIGN_RIGHT:
        delta = maxWidth - tableWidth;
        break;
      case wxPDF_ALIGN_LEFT:
      default:
        delta = 0;
        break;
    }
  }
  // Set left and right margin
  SetLeftMargin(saveLeftMargin+delta);
  SetRightMargin(GetPageWidth() - saveLeftMargin - tableWidth - delta);
  SetXY(saveLeftMargin+delta,GetY());

  // Check alignment

  table->Write();

  SetLeftMargin(saveLeftMargin);
  SetRightMargin(saveRightMargin);
}

void
wxPdfDocument::PrepareXmlCell(wxXmlNode* node, wxPdfCellContext& context)
{
  wxPdfCellContext* newContext;
  wxXmlNode *child = node->GetChildren();
  while (child)
  {
    wxString name = (child->GetName()).Lower();

    if (name == wxT("b") || name == wxT("i") || name == wxT("u") ||
        name == wxT("o") || name == wxT("s") ||
        name == wxT("strong") || name == wxT("em"))
    {
      // --- Bold, Italic, Underline, Overline, Strikeout
      // --- Strong (= bold), Emphasize (= italic)
      wxString addStyle = name.Upper();
      if (name.Length() > 1)
      {
        if (name == wxT("strong"))  addStyle = wxT("B");
        else if (name == wxT("em")) addStyle = wxT("I");
      }
      wxString style = GetFontStyle();
      if (!style.Contains(addStyle))
      {
        SelectFont(_T(""), style + addStyle, 0, false);
        PrepareXmlCell(child, context);
        SelectFont(_T(""), style, 0, false);
      }
      else
      {
        PrepareXmlCell(child, context);
      }
    }
    else if (name == wxT("small"))
    {
      // --- Small font size
      static double ratio = 2./3.;
      double currentSize = GetFontSize();
      SelectFont(_T(""), _T(""), currentSize * ratio, false);
      double delta = (currentSize - GetFontSize()) * 0.5 * ratio / GetScaleFactor();
      SetXY(GetX(), GetY() + delta);
      PrepareXmlCell(child, context);
      SetXY(GetX(), GetY() - delta);
      SelectFont(_T(""), _T(""), currentSize, false);
    }
    else if (name == wxT("sup"))
    {
      // --- Superscript
      static double ratio = 2./3.;
      double currentSize = GetFontSize();
      SelectFont(_T(""), _T(""), currentSize * ratio, false);
      double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
      SetXY(GetX(), GetY() - delta);
      PrepareXmlCell(child, context);
      SetXY(GetX(), GetY() + delta);
      SelectFont(_T(""), _T(""), currentSize, false);
    }
    else if (name == wxT("sub"))
    {
      // --- Subscript
      static double ratio = 2./3.;
      double currentSize = GetFontSize();
      SelectFont(_T(""), _T(""), currentSize * ratio, false);
      double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
      SetXY(GetX(), GetY() + delta);
      PrepareXmlCell(child, context);
      SetXY(GetX(), GetY() - delta);
      SelectFont(_T(""), _T(""), currentSize, false);
    }
    else if (name == wxT("ul"))
    {
      // --- Unordered list
      double leftMargin = GetLeftMargin();
      double indent = GetFontSize() / GetScaleFactor();
      SetLeftMargin(leftMargin + indent);
      wxXmlNode *listChild = child->GetChildren();
      while (listChild)
      {
        if ((listChild->GetName()).Lower() == wxT("li"))
        {
          Ln();
          newContext = new wxPdfCellContext(context.GetMaxWidth(), wxPDF_ALIGN_LEFT);
          //# newContext->AddHeight(GetLineHeight());
          context.AppendContext(newContext);
          PrepareXmlCell(listChild, *newContext);
          context.AddHeight(newContext->GetHeight());
          Ln();
        }
        listChild = listChild->GetNext();
      }
      SetLeftMargin(leftMargin);
    }
    else if (name == wxT("ol"))
    {
      // --- Ordered list
      double leftMargin = GetLeftMargin();
      double indent = GetStringWidth(_T(" 00. "));
      wxString type = child->GetPropVal(_T("type"), _T("1"));
      if (type.Length() > 0)
      {
        if (type[0] == _T('a'))      
          indent = GetStringWidth(_T(" xx. "));
        else if (type[0] == _T('A')) 
          indent = GetStringWidth(_T(" XX. "));
        else if (type[0] == _T('i')) 
          indent = GetStringWidth(_T(" xxx. "));
        else if (type[0] == _T('I')) 
          indent = GetStringWidth(_T(" XXX. "));
        else if (type[0] == _T('z') && 
                 type.Length() > 1 && type[1] >= _T('1') && type[1] <= _T('4'))
          indent = 1.1 * GetFontSize() / GetScaleFactor();
      }
      SetLeftMargin(leftMargin + indent);
      wxXmlNode *listChild = child->GetChildren();
      while (listChild)
      {
        if ((listChild->GetName()).Lower() == wxT("li"))
        {
          Ln();
          newContext = new wxPdfCellContext(context.GetMaxWidth(), wxPDF_ALIGN_LEFT);
          //# newContext->AddHeight(GetLineHeight());
          context.AppendContext(newContext);
          PrepareXmlCell(listChild, *newContext);
          context.AddHeight(newContext->GetHeight());
          Ln();
        }
        listChild = listChild->GetNext();
      }
      SetLeftMargin(leftMargin);
    }
    else if (name == wxT("br"))
    {
      // --- Line break
      Ln();
      //# context.AddHeight(GetLineHeight());
      context.MarkLastLine();
      context.AddLine();
    }
    else if (name == wxT("p"))
    {
      // --- Paragraph
      wxString align = (child->GetPropVal(_T("align"), _T("left"))).Lower();
      wxPdfAlignment alignment = wxPDF_ALIGN_LEFT;
      if (align == _T("right"))        alignment = wxPDF_ALIGN_RIGHT;
      else if (align == _T("center"))  alignment = wxPDF_ALIGN_CENTER;
      else if (align == _T("justify")) alignment = wxPDF_ALIGN_JUSTIFY;

      Ln();
      newContext = new wxPdfCellContext(context.GetMaxWidth(), alignment);
      context.AppendContext(newContext);
      PrepareXmlCell(child, *newContext);
      newContext->MarkLastLine();
      context.AddHeight(newContext->GetHeight()+GetLineHeight());
      Ln();
      Ln();
    }
    else if (name == wxT("hr"))
    {
      // --- Horizontal rule
//      double hrWidth = GetPageWidth() - GetLeftMargin() - GetRightMargin();
//      long widthAttr;
      Ln();
//      wxString strWidth = child->GetPropVal(_T("width"), _T(""));
//      if (strWidth.Length() > 0 && strWidth.ToLong(&widthAttr))
//      {
//        hrWidth = hrWidth * 0.01 * widthAttr;
//      }
//      double x = GetX();
//      double y = GetY();
//      double wLine = GetLineWidth();
//      SetLineWidth(0.2);
//      Line(x, y, x + hrWidth, y);
//      SetLineWidth(wLine);
//      Ln();
      context.AddHeight(GetLineHeight());
      //context.AddLine();
    }
    else if (name == wxT("a"))
    {
      // --- Anchor
      PrepareXmlCell(child, context);
    }
    else if (name == wxT("img"))
    {
      // --- Image
      wxString src = child->GetPropVal(_T("src"), _T(""));
      if (src.Length() > 0)
      { 
//        long width;
        long height;
//        wxString strWidth = child->GetPropVal(_T("width"), _T("0"));
        wxString strHeight = child->GetPropVal(_T("height"), _T("0"));
//        if (!strWidth.ToLong(&width)) width = 0;
        if (!strHeight.ToLong(&height)) height = 0;
        double h = ((double) height) / (GetImageScale() * GetScaleFactor());
        // TODO: handle image
        // line height, position, margins etc.
        context.AddHeight(h);
      }
    }
    else if (name == wxT("font"))
    {
      // --- Font
      wxString    saveFamily = GetFontFamily();
      wxString    saveStyle  = GetFontStyle();
      double      saveSize   = GetFontSize();
      wxString strFace  = child->GetPropVal(_T("face"), _T(""));
      double size = 0;
      wxString strSize  = child->GetPropVal(_T("size"), _T(""));
      if (strSize.Length() > 0)
      {
        size = String2Double(strSize);
      }
      if (size <= 0) size = saveSize;
      SelectFont(strFace, saveStyle, size, false);
      PrepareXmlCell(child, context);
      SelectFont(saveFamily, saveStyle, saveSize, false);
    }
    else if (name == wxT("code"))
    {
      // --- Code section
      wxString    saveFamily = GetFontFamily();
      wxString    saveStyle  = GetFontStyle();
      double      saveSize   = GetFontSize();
      wxString strFace  = child->GetPropVal(_T("face"), _T("courier"));
      double size = 0;
      wxString strSize  = child->GetPropVal(_T("size"), _T("10"));
      if (strSize.Length() > 0)
      {
        size = String2Double(strSize);
      }
      if (size <= 0) size = saveSize;
      SelectFont(strFace, _T(""), size, false);
      Ln();
      context.MarkLastLine();
      context.AddLine();
      PrepareXmlCell(child, context);
      Ln();
      SelectFont(saveFamily, saveStyle, saveSize, false);
    }
    else if (name == wxT("h1") || name == wxT("h2") || name == wxT("h3") || 
             name == wxT("h4") || name == wxT("h5") || name == wxT("h6"))
    {
      // --- Header
      wxString align = (child->GetPropVal(_T("align"), _T("left"))).Lower();
      wxPdfAlignment alignment = wxPDF_ALIGN_LEFT;
      if (align == _T("right"))        alignment = wxPDF_ALIGN_RIGHT;
      else if (align == _T("center"))  alignment = wxPDF_ALIGN_CENTER;
      else if (align == _T("justify")) alignment = wxPDF_ALIGN_JUSTIFY;

      if (context.GetLastLineWidth() > 0)
      {
        Ln();
      }
      Ln();
      context.AddHeight(GetLineHeight());
      newContext = new wxPdfCellContext(context.GetMaxWidth(), alignment);
      context.AppendContext(newContext);

      double headsize = (wxT('4') - name[1]) * 2;
      double currentFontSize = GetFontSize();
      SelectFont(_T(""), _T(""), currentFontSize + headsize, false);
      wxString addStyle = wxT("B");
      wxString style = GetFontStyle();
      if (!style.Contains(addStyle))
      {
        SelectFont(_T(""), style + addStyle, 0, false);
        PrepareXmlCell(child, *newContext);
        SelectFont(_T(""), style, 0, false);
      }
      else
      {
        PrepareXmlCell(child, *newContext);
      }
      context.AddHeight(newContext->GetHeight());
      // reset
      SelectFont(_T(""), _T(""), currentFontSize, false);
      Ln();
    }
    else if (name == wxT("table"))
    {
      // --- Table
      wxString border = (child->GetPropVal(_T("border"), _T("0"))).Lower();
      bool hasBorder = (border != _T("0"));
      wxString align = (child->GetPropVal(_T("align"), _T(""))).Lower();
      wxPdfAlignment hAlignment = context.GetHAlign();
      if (align == _T("right"))        hAlignment = wxPDF_ALIGN_RIGHT;
      else if (align == _T("center"))  hAlignment = wxPDF_ALIGN_CENTER;
      else if (align == _T("justify")) hAlignment = wxPDF_ALIGN_JUSTIFY;
      align = (child->GetPropVal(_T("valign"), _T("top"))).Lower();
      wxPdfAlignment vAlignment = wxPDF_ALIGN_TOP;
      if (align == _T("bottom"))      vAlignment = wxPDF_ALIGN_BOTTOM;
      else if (align == _T("middle")) vAlignment = wxPDF_ALIGN_MIDDLE;
      else if (align == _T("center")) vAlignment = wxPDF_ALIGN_MIDDLE;
      double pad = 1.5 / GetScaleFactor();
      wxString padding = (child->GetPropVal(_T("cellpadding"), _T(""))).Lower();
      if (padding.Length() > 0)
      {
        pad = String2Double(padding);
        if (pad < 0) pad = 0;
      }

      wxPdfTable* table = new wxPdfTable(this);
      table->SetPad(pad);
      table->SetBorder(hasBorder);
      newContext = new wxPdfCellContext(context.GetMaxWidth(), hAlignment, vAlignment);
      context.AppendContext(newContext);
      newContext->SetTable(table);
      PrepareXmlTable(child, *newContext);
      context.AddHeight(newContext->GetHeight());
    }
    else
    {
      // --- Content
      if (child->GetType() == wxXML_TEXT_NODE || name == wxT("msg"))
      {
        if (context.GetLastLineWidth() == 0)
        {
          context.AddHeight(GetLineHeight());
        }
        double wmax = context.GetMaxWidth()-context.GetLastLineWidth();
        wxString s;
        if (name == wxT("msg"))
        {
          s = wxGetTranslation(GetNodeContent(child));
        }
        else
        {
          s = child->GetContent();
        }
        s.Replace(_T("\r"),_T("")); // remove carriage returns
        int nb = s.Length();
        if (nb > 0 && s[nb-1] == _T('\n'))
        {
          nb--;
        }
        int sep = -1;
        int i = 0;
        int j = 0;
        double len = 0;
        double ls = 0;
        int ns = 0;
        int nl = 1;
        wxChar c = 0;
        while (i < nb)
        {
          // Get next character
          c = s[i];
          if (c == _T('\n'))
          {
            // Explicit line break
            i++;
            context.AddLastLineValues(len, ns);
            sep = -1;
            j = i;
            len = 0;
            ns = 0;
            nl++;
            context.MarkLastLine();
            context.AddLine();
            context.AddHeight(GetLineHeight());
            wmax = context.GetMaxWidth();
            continue;
          }
          if (c == _T(' '))
          {
            sep = i;
            ls = len;
            ns++;
          }
          len = GetStringWidth(s.SubString(j, i));

          if (len > wmax)
          {
            // Automatic line break
            if (sep == -1)
            {
              if (context.GetLastLineWidth() > 0)
              {
                if (context.GetLastChar() == _T(' '))
                {
                  context.AddLastLineValues(-context.GetLastSpaceWidth(), -1);
                }
                i = j;
              }
              else
              {
                if (i == j)
                {
                  i++;
                }
              }
            }
            else
            {
              i = sep + 1;
              ns--;
            }
            context.AddLastLineValues(ls, ns);
            sep = -1;
            j = i;
            len = 0;
            ns = 0;
            nl++;
            context.AddLine();
            context.AddHeight(GetLineHeight());
            wmax = context.GetMaxWidth();
          }
          else
          {
            i++;
          }
        }
        // Last chunk
        context.AddLastLineValues(len, ns);
        context.SetLastChar(c);
        context.SetLastSpaceWidth(GetStringWidth(_T(" ")));
      }
    }
    child = child->GetNext();
  }
}

void
wxPdfDocument::WriteXml(const wxString& xmlString)
{
  if (GetLineHeight() == 0)
  {
    SetLineHeight(GetFontSize()*1.25 / GetScaleFactor());
  }
  wxStringInputStream xmlStream(_T("<xml>")+xmlString+_T("</xml>"));
  wxXmlDocument xmlDocument;
  bool loaded = xmlDocument.Load(xmlStream);
  if (loaded)
  {
    if (xmlDocument.IsOk())
    {
      wxXmlNode* root = xmlDocument.GetRoot();
      double maxWidth = GetPageWidth() - GetRightMargin() - GetX();
      wxPdfCellContext context(maxWidth, wxPDF_ALIGN_LEFT);
      double saveX = GetX();
      double saveY = GetY();
      PrepareXmlCell(root, context);
      SetXY(saveX, saveY);
      WriteXmlCell(root, context);
    }
    else
    {
      wxLogDebug(_T("wxPdfDocument::WriteXml: Markup invalid."));
      return /* false */;
    }
  }
  else
  {
    // TODO: Error handling
    wxLogDebug(_T("wxPdfDocument::WriteXml: Unable to load markup string."));
  }
}

void
wxPdfDocument::WriteXmlCell(wxXmlNode* node, wxPdfCellContext& context)
{
  wxPdfCellContext* newContext;
  wxXmlNode *child = node->GetChildren();
  while (child)
  {
    wxString name = (child->GetName()).Lower();
    // parse the children

    if (name == wxT("b") || name == wxT("i") || name == wxT("u") ||
        name == wxT("o") || name == wxT("s") ||
        name == wxT("strong") || name == wxT("em"))
    {
      // --- Bold, Italic, Underline, Overline, Strikeout
      // --- Strong (= bold), Emphasize (= italic)
      wxString addStyle = name.Upper();
      if (name.Length() > 1)
      {
        if (name == wxT("strong"))  addStyle = wxT("B");
        else if (name == wxT("em")) addStyle = wxT("I");
      }
      wxString style = GetFontStyle();
      if (!style.Contains(addStyle))
      {
        SetFont(_T(""), style + addStyle);
        WriteXmlCell(child, context);
        SetFont(_T(""), style);
      }
      else
      {
        WriteXmlCell(child, context);
      }
    }
    else if (name == wxT("small"))
    {
      // --- Small font
      static double ratio = 2./3.;
      double currentSize = GetFontSize();
      SetFontSize(currentSize * ratio);
      double delta = (currentSize - GetFontSize()) * 0.5 * ratio / GetScaleFactor();
      SetXY(GetX(), GetY() + delta);
      WriteXmlCell(child, context);
      SetXY(GetX(), GetY() - delta);
      SetFontSize(currentSize);
    }
    else if (name == wxT("sup"))
    {
      // --- Superscript
      static double ratio = 2./3.;
      double currentSize = GetFontSize();
      SetFontSize(currentSize * ratio);
      double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
      SetXY(GetX(), GetY() - delta);
      WriteXmlCell(child, context);
      SetXY(GetX(), GetY() + delta);
      SetFontSize(currentSize);
    }
    else if (name == wxT("sub"))
    {
      // --- Subscript
      static double ratio = 2./3.;
      double currentSize = GetFontSize();
      SetFontSize(currentSize * ratio);
      double delta = (currentSize - GetFontSize()) * ratio / GetScaleFactor();
      SetXY(GetX(), GetY() + delta);
      WriteXmlCell(child, context);
      SetXY(GetX(), GetY() - delta);
      SetFontSize(currentSize);
    }
    else if (name == wxT("ul"))
    {
      // --- Unordered list
#if wxUSE_UNICODE
      static wxChar bulletChar = 0x2022;
      static wxChar dashChar   = 0x2013;
#else
      static wxChar bulletChar = '\x95'; // dec 149
      static wxChar dashChar   = '\x96'; // dec 150
#endif
      wxChar itemChar = bulletChar;
      bool useZapfDingBats = false;
      long zapfChar = 0;
      wxString type = child->GetPropVal(_T("type"), _T("bullet"));

      if (type.IsNumber() && type.ToLong(&zapfChar))
      {
        if (zapfChar >= 0 && zapfChar <= 255)
        {
          itemChar = zapfChar;
          useZapfDingBats = true;
        }
      }
      else if (type.Lower() == wxT("dash"))
      {
        itemChar = dashChar;
      }
      double leftMargin = GetLeftMargin();
      double indent = GetFontSize() / GetScaleFactor();
      SetLeftMargin(leftMargin + indent);
      wxXmlNode *listChild = child->GetChildren();
      while (listChild)
      {
        if ((listChild->GetName()).Lower() == wxT("li"))
        {
          wxString saveFont = GetFontFamily();
          wxString saveStyle = GetFontStyle();
          double saveSize = GetFontSize();
          if (useZapfDingBats)
          {
            SetFont(_T("zapfdingbats"), _T(""), 0.7*saveSize);
          }
          else
          {
            SetFont(_T("arial"), _T(""), saveSize);
          }
          SetLeftMargin(leftMargin);
          SetXY(leftMargin, GetY());
          WriteCell(GetLineHeight(), wxString(itemChar));
          SetLeftMargin(leftMargin+indent);
          SetXY(leftMargin+indent, GetY()); 
          SetFont(saveFont, saveStyle, saveSize);
          //Ln();
          newContext = context.GetCurrentContext();
          context.IncrementCurrentContext();
          WriteXmlCell(listChild, *newContext);
          Ln();
        }
        listChild = listChild->GetNext();
      }
      SetLeftMargin(leftMargin);
      SetXY(leftMargin, GetY());
    }
    else if (name == wxT("ol"))
    {
      // --- Ordered list
      bool useZapfDingBats = false;
      int listCount = 0;
      int listType = 1;
      int listCycle = 0;
      long listStart;
      wxString start = child->GetPropVal(_T("start"), _T("1"));
      if (start.IsNumber() && start.ToLong(&listStart))
      {
        listCount += (listStart - 1);
      }
      double indent = GetStringWidth(_T(" 00. "));
      wxString type = child->GetPropVal(_T("type"), _T("1"));
      if (type.Length() > 0)
      {
        if (type[0] == _T('1'))
        {
          listType = 1;
        }
        else if (type[0] == _T('a'))
        {
          listType = 2;
          listCycle = 26;
          indent = GetStringWidth(_T(" xx. "));
        }
        else if (type[0] == _T('A'))
        {
          listType = 3;
          listCycle = 26;
          indent = GetStringWidth(_T(" XX. "));
        }
        else if (type[0] == _T('i'))
        {
          listType = 4;
          indent = GetStringWidth(_T(" xxx. "));
        }
        else if (type[0] == _T('I'))
        {
          listType = 5;
          indent = GetStringWidth(_T(" XXX. "));
        }
        else if (type[0] == _T('z'))
        {
          if (type.Length() > 1 && type[1] >= _T('1') && type[1] <= _T('4'))
          {
            useZapfDingBats = true;
            listType = type[1] - _T('1');
            listCycle = 10;
            indent = 1.1 * GetFontSize() / GetScaleFactor();
          }
        }
      }
      double leftMargin = GetLeftMargin();
      SetLeftMargin(leftMargin + indent);
      Ln();
      wxXmlNode *listChild = child->GetChildren();
      while (listChild)
      {
        if ((listChild->GetName()).Lower() == wxT("li"))
        {
          wxString saveFont = GetFontFamily();
          wxString saveStyle = GetFontStyle();
          double saveSize = GetFontSize();
          SetLeftMargin(leftMargin);
          SetXY(leftMargin, GetY());
          if (useZapfDingBats)
          {
            SetFont(_T("zapfdingbats"), _T(""), 0.85*saveSize);
            wxChar itemChar = 172 + 10 * listType + listCount % listCycle;
            WriteCell(GetLineHeight(), wxString(itemChar));
          }
          else
          {
            wxChar itemChar;
            SetFont(_T("arial"), _T(""), saveSize);
            wxString item;
            switch (listType)
            {
              case 2:
                itemChar = _T('a') + listCount % listCycle;
                item = wxString(itemChar) + _T(". ");
                break;
              case 3:
                itemChar = _T('A') + listCount % listCycle;
                item = wxString(itemChar) + _T(". ");
                break;
              case 4:
                item = Convert2Roman(listCount+1).Lower() + _T(". ");
                break;
              case 5:
                item = Convert2Roman(listCount+1) + _T(". ");
                break;
              case 1:
              default:
                item = wxString::Format(_T("%d. "), listCount+1);
                break;
            }
            Cell(indent, GetLineHeight(), item, wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_RIGHT);
          }

          SetLeftMargin(leftMargin+indent);
          SetXY(leftMargin+indent, GetY()); 
          SetFont(saveFont, saveStyle, saveSize);
          listCount++;
          newContext = context.GetCurrentContext();
          context.IncrementCurrentContext();
          WriteXmlCell(listChild, *newContext);
          Ln();
        }
        listChild = listChild->GetNext();
      }
      SetLeftMargin(leftMargin);
      SetXY(leftMargin, GetY());
    }
    else if (name == wxT("br"))
    {
      // --- Line break
      Ln();
      context.IncrementCurrentLine();
    }
    else if (name == wxT("p"))
    {
      // --- Paragraph
      if (GetX() > GetLeftMargin())
      {
        Ln();
      }
      newContext = context.GetCurrentContext();
      context.IncrementCurrentContext();
      WriteXmlCell(child, *newContext);
      if (GetX() > GetLeftMargin())
      {
        Ln();
      }
      Ln();
    }
    else if (name == wxT("hr"))
    {
      // --- Horizontal rule
      double hrWidth = GetPageWidth() - GetLeftMargin() - GetRightMargin();
      long widthAttr;
      Ln();
      wxString strWidth = child->GetPropVal(_T("width"), _T(""));
      if (strWidth.Length() > 0 && strWidth.ToLong(&widthAttr))
      {
        hrWidth = hrWidth * 0.01 * widthAttr;
      }
      double x = GetX();
      double y = GetY();
      double wLine = GetLineWidth();
      SetLineWidth(0.2);
      Line(x, y, x + hrWidth, y);
      SetLineWidth(wLine);
      Ln();
    }
    else if (name == wxT("a"))
    {
      // --- Anchor
      wxString href = child->GetPropVal(_T("href"), _T(""));
      wxString nameAttr = child->GetPropVal(_T("name"), _T(""));
      if (href.Length() > 0)
      {
         if (href[0] == _T('#'))
        {
          wxString nameAttr = href.Right(href.Length()-1);
          if (nameAttr.Length() > 0)
          {
            if (m_namedLinks->find(nameAttr) == m_namedLinks->end())
            {
              (*m_namedLinks)[nameAttr] = AddLink();
            }
          }
        }
        context.SetHRef(href);
        wxPdfColour saveColor = GetTextColor();
        SetTextColor(0, 0, 255);
        wxString style = GetFontStyle();
        if (!style.Contains(wxT("U")))
        {
          SetFont(_T(""), style + wxT("U"));
          WriteXmlCell(child, context);
          SetFont(_T(""), style);
        }
        else
        {
          WriteXmlCell(child, context);
        }
        SetTextColor(saveColor);
        context.SetHRef(_T(""));
      }
      else if (nameAttr.Length() > 0)
      { 
        int link;
        if (m_namedLinks->find(nameAttr) == m_namedLinks->end())
        {
          link = AddLink();
          (*m_namedLinks)[nameAttr] = link;
        }
        else
        {
          link = (*m_namedLinks)[nameAttr];
        }
        SetLink(link, -1, -1);
        WriteXmlCell(child, context);        
      }
      else
      {
        WriteXmlCell(child, context);
      }
    }
    else if (name == wxT("img"))
    {
      // --- Image
      wxString src = child->GetPropVal(_T("src"), _T(""));
      if (src.Length() > 0)
      { 
        long width;
        long height;
        wxString strWidth = child->GetPropVal(_T("width"), _T("0"));
        wxString strHeight = child->GetPropVal(_T("height"), _T("0"));
        if (!strWidth.ToLong(&width)) width = 0;
        if (!strHeight.ToLong(&height)) height = 0;
        double x = GetX();
        double y = GetY();
        double w = ((double) width) / (GetImageScale() * GetScaleFactor());
        double h = ((double) height) / (GetImageScale() * GetScaleFactor());
        wxString align = (child->GetPropVal(_T("align"), _T("left"))).Lower();
        double delta;
        if (align == _T("right"))
        {
          delta = context.GetMaxWidth() - w;
        }
        else if (align == _T("center"))
        {
          delta = 0.5 * (context.GetMaxWidth() - w);
        }
        else
        {
          delta = 0;
        }
        // line height, position, margins etc.
        if (GetX() > GetLeftMargin())
        {
          Ln();
        }
        Image(src, x+delta, y, w, h);
        SetXY(x, y+h);
      }
    }
    else if (name == wxT("font"))
    {
      // --- Font
      wxString    saveFamily = GetFontFamily();
      wxString    saveStyle  = GetFontStyle();
      double      saveSize   = GetFontSize();
      wxPdfColour saveColor  = GetTextColor();
      wxString strFace  = child->GetPropVal(_T("face"), _T(""));
      wxString strSize  = child->GetPropVal(_T("size"), _T(""));
      wxString strColor = child->GetPropVal(_T("color"), _T(""));
      double size = 0;
      if (strSize.Length() > 0)
      {
        size = String2Double(strSize);
      }
      if (size <= 0) size = saveSize;
      SetFont(strFace, saveStyle, size);
      if (strColor.Length() > 0)
      {
        SetTextColor(wxPdfColour(strColor));
      }
      WriteXmlCell(child, context);
      if (strColor.Length() > 0)
      {
        SetTextColor(saveColor);
      }
      SetFont(saveFamily, saveStyle, saveSize);
    }
    else if (name == wxT("code"))
    {
      // --- Code section
      wxString    saveFamily = GetFontFamily();
      wxString    saveStyle  = GetFontStyle();
      double      saveSize   = GetFontSize();
      wxPdfColour saveColor  = GetTextColor();
      wxString strFace  = child->GetPropVal(_T("face"), _T("courier"));
      wxString strSize  = child->GetPropVal(_T("size"), _T("10"));
      wxString strColor = child->GetPropVal(_T("color"), _T(""));
      double size = 0;
      if (strSize.Length() > 0)
      {
        size = String2Double(strSize);
      }
      if (size <= 0) size = saveSize;
      SetFont(strFace, _T(""), size);
      if (strColor.Length() > 0)
      {
        SetTextColor(wxPdfColour(strColor));
      }
      Ln();
      context.IncrementCurrentLine();
      WriteXmlCell(child, context);
      Ln();
      if (strColor.Length() > 0)
      {
        SetTextColor(saveColor);
      }
      SetFont(saveFamily, saveStyle, saveSize);
    }
    else if (name == wxT("h1") || name == wxT("h2") || name == wxT("h3") || 
             name == wxT("h4") || name == wxT("h5") || name == wxT("h6"))
    {
      // --- Header
      newContext = context.GetCurrentContext();
      context.IncrementCurrentContext();

      double headsize = (wxT('4') - name[1]) * 2;
      double currentFontSize = GetFontSize();
      if (GetX() > GetLeftMargin())
      {
        Ln();
      }
      Ln();
      wxString addStyle = wxT("B");
      wxString style = GetFontStyle();
      if (!style.Contains(addStyle))
      {
        SetFont(_T(""), style + addStyle, currentFontSize + headsize);
        WriteXmlCell(child, *newContext);
        SetFont(_T(""), style, currentFontSize);
      }
      else
      {
        SetFontSize(currentFontSize + headsize);
        WriteXmlCell(child, *newContext);
        SetFontSize(currentFontSize);
      }
      Ln();
    }
    else if (name == wxT("table"))
    {
      // --- Table
      newContext = context.GetCurrentContext();
      context.IncrementCurrentContext();
      WriteXmlTable(*newContext);
    }
    else
    {
      // --- Content
      if (child->GetType() == wxXML_TEXT_NODE || name == wxT("msg"))
      {
        wxPdfLink link = wxPdfLink(-1);
        wxString href = context.GetHRef();
        if (href.Length() > 0)
        {
          if (href[0] == _T('#'))
          {
            wxString nameAttr = href.Right(href.Length()-1);
            if (nameAttr.Length() > 0)
            {
              if (m_namedLinks->find(nameAttr) != m_namedLinks->end())
              {
                link = wxPdfLink((*m_namedLinks)[nameAttr]);
              }
            }
          }
          else
          {
            link = wxPdfLink(href);
          }
        }

        double h = GetLineHeight();
        double wmax = context.GetCurrentLineWidth() + wxPDF_EPSILON;
        DoXmlAlign(context);

        wxString s;
        if (name == wxT("msg"))
        {
          s = wxGetTranslation(GetNodeContent(child));
        }
        else
        {
          s = child->GetContent();
        }
        s.Replace(_T("\r"),_T("")); // remove carriage returns
        int nb = s.Length();
        if (nb > 0 && s[nb-1] == _T('\n'))
        {
          nb--;
        }

        int sep = -1;
        int i = 0;
        int j = 0;
        double len = 0;
        double ls = 0;
        int ns = 0;
        int nl = 1;
        wxChar c;
        while (i < nb)
        {
          // Get next character
          c = s[i];
          if (c == _T('\n'))
          {
            // Explicit line break
            if (m_ws > 0)
            {
              m_ws = 0;
              Out("0 Tw");
            }
            WriteCell(h,s.SubString(j,i-1), wxPDF_BORDER_NONE, 0, link);
            i++;
            sep = -1;
            j = i;
            len = 0;
            ns = 0;
            nl++;
            context.IncrementCurrentLine();
            Ln();
            DoXmlAlign(context);
            wmax = context.GetCurrentLineWidth();
            continue;
          }
          if (c == _T(' '))
          {
            sep = i;
            ls = len;
            ns++;
          }
          len = GetStringWidth(s.SubString(j, i));

          if (len > wmax)
          {
            // Automatic line break
            if (sep == -1)
            {
              if (wmax == context.GetCurrentLineWidth())
              {
                if (i == j)
                {
                  i++;
                }
                WriteCell(h,s.SubString(j,i-1), wxPDF_BORDER_NONE, 0, link);
              }
              else
              {
                i = j;
              }
            }
            else
            {
              WriteCell(h,s.SubString(j,sep-1), wxPDF_BORDER_NONE, 0, link);
              i = sep + 1;
            }
            sep = -1;
            j = i;
            len = 0;
            ns = 0;
            nl++;
            context.IncrementCurrentLine();
            Ln();
            DoXmlAlign(context);
            wmax = context.GetCurrentLineWidth();
          }
          else
          {
            i++;
          }
        }
        // Last chunk
        WriteCell(h,s.SubString(j,i-1), wxPDF_BORDER_NONE, 0, link);
        context.AddCurrentLineWidth(-len);
        if (context.GetHAlign() == wxPDF_ALIGN_JUSTIFY && m_ws > 0)
        {
          double delta = ns * m_ws;
          SetXY(GetX()+delta, GetY());
        }
      }
    }
    child = child->GetNext();
  }
}

void
wxPdfDocument::DoXmlAlign(wxPdfCellContext& context)
{
  if (!context.GetAligned())
  {
    if (m_ws > 0 && context.GetHAlign() != wxPDF_ALIGN_JUSTIFY)
    {
      m_ws = 0;
      Out("0 Tw");
    }
    switch (context.GetHAlign())
    {
      case wxPDF_ALIGN_JUSTIFY:
        {
          m_ws = (!context.IsCurrentLineMarked() && context.GetCurrentLineSpaces() > 0) ? (context.GetMaxWidth() - context.GetCurrentLineWidth())/context.GetCurrentLineSpaces() : 0;
          OutAscii(Double2String(m_ws*m_k,3)+wxString(_T(" Tw")));
        }
        break;
      case wxPDF_ALIGN_CENTER:
        {
          double delta = 0.5 * (context.GetMaxWidth() - context.GetCurrentLineWidth());
          SetXY(GetX()+delta,GetY());
        }
        break;
      case wxPDF_ALIGN_RIGHT:
        {
          double delta = context.GetMaxWidth() - context.GetCurrentLineWidth();
          SetXY(GetX()+delta,GetY());
        }
        break;
      case wxPDF_ALIGN_LEFT:
      default:
        break;
    }
  }
  context.SetAligned();
}

