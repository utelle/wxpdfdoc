///////////////////////////////////////////////////////////////////////////////
// Name:        pdfxml.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2005-12-03
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfxml.h Interface of the wxPdfDocument markup

#ifndef _PDF_XML_H_
#define _PDF_XML_H_

// wxWidgets headers
#include <wx/string.h>
#include <wx/wfstream.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfdocument.h"

class WXDLLIMPEXP_FWD_PDFDOC wxPdfTable;

/// Class representing cell context objects. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfCellContext
{
public:
  /// Constructor
  /**
  * \param maxWidth Maximum allowed width for the cell
  * \param hAlign Horizontal alignment
  * \param vAlign Vertical alignment
  */
  wxPdfCellContext(double maxWidth, wxPdfAlignment hAlign = wxPDF_ALIGN_JUSTIFY, wxPdfAlignment vAlign = wxPDF_ALIGN_TOP);

  /// Destructor
  virtual ~wxPdfCellContext();

  /// Increase total height of cell
  /**
  * \param height Height to add
  */
  void AddHeight(double height) { m_height += height; }

  /// Get total height of cell
  /**
  * \return Total height
  */
  double GetHeight() { return m_height; }

  /// Get maximum width of cell
  /**
  * \return Maximum width
  */
  double GetMaxWidth() { return m_maxWidth; }

  /// Get horizontal alignment
  /**
  * \return Horizontal alignment
  */
  wxPdfAlignment GetHAlign() { return m_hAlign; }

  /// Set vertical alignment
  /**
  * \param vAlign Vertical alignment
  */
  void SetVAlign(wxPdfAlignment vAlign) { m_vAlign = vAlign; }

  /// Get vertical alignment
  /**
  * \return Vertical alignment
  */
  wxPdfAlignment GetVAlign() { return m_vAlign; }

  /// Add a line to cell
  void AddLine();

  /// Get number of lines in cell
  /**
  * \return Number of lines
  */
  unsigned int GetLineCount() { return (unsigned int) m_linewidth.GetCount(); }

  /// Add width and number of spaces of the last line of the current context
  /**
  * \param width Line width
  * \param spaces Number of spaces
  */
  void AddLastLineValues(double width, int spaces);

  /// Get the width of the last line
  /**
  * \return Last line width
  */
  double GetLastLineWidth();

  /// Mark the current line as the last line
  void MarkLastLine();

  /// Check whether current line is marked as last line
  /**
  * \return TRUE if current line is the last line, FALSE otherwise
  */
  bool IsCurrentLineMarked();

  /// Adjust width of current line
  /**
  * \param width Width to add
  */
  void AddCurrentLineWidth(double width);

  /// Adjust number of spaces of current line
  /**
  * \param spaces Number of spaces to add
  */
  void AddCurrentLineSpaces(int spaces);

  /// Get width of current line
  /**
  * \return Current line width
  */
  double GetCurrentLineWidth();

  /// Get number of spaces of current line
  /**
  * \return Number of spaces
  */
  int    GetCurrentLineSpaces();

  /// Get number of current line
  /**
  * \return Current line index
  */
  unsigned int GetCurrentLine() { return m_currentLine; }

  /// Increment line counter
  void IncrementCurrentLine();

  /// Get line delta measure
  /**
  * \return Line delta
  */
  double GetLineDelta();

  /// Check whether alignment has been taken care of
  /**
  * \return TRUE if aligned, FALSE otherwise
  */
  bool GetAligned() { return m_aligned; }

  /// Set flag that alignment has been taken care of
  void SetAligned() { m_aligned = true; }

  /// Get cell fill style
  /**
  * \return Fill style
  */
  int GetFillStyle() { return m_fillStyle; }

  /// Set cell fill style
  /**
  * \param fillStyle Fill style
  */
  void SetFillStyle(int fillStyle) { m_fillStyle = fillStyle; }

  /// Get the number of contexts
  /**
  * \return Number of contexts
  */
  unsigned int GetContextCount() { return (unsigned int) m_contexts.GetCount(); }

  /// Increment current context
  void IncrementCurrentContext();

  /// Get current context
  /**
  * \return Pointer to current cell context
  */
  wxPdfCellContext* GetCurrentContext();

  /// Append context to context list
  /**
  * \param context Pointer to cell context to append
  */
  void AppendContext(wxPdfCellContext* context);

  /// Remember the last character of the last chunk
  /**
  * \param c Character to remember
  */
  void SetLastChar(wxUniChar c) { m_lastChar = c; }

  /// Get last character of previous chunk
  /**
  * \return Last character
  */
  wxUniChar GetLastChar() { return m_lastChar; }

  /// Remember the width of the last space character
  /**
  * \param w Space width
  */
  void SetLastSpaceWidth(double w) { m_spaceWidth = w; }

  /// Get width of last space character
  /**
  * \return Space width
  */
  double GetLastSpaceWidth() { return m_spaceWidth; }

  /// Remember the character spacing
  /**
  * \param charSpacing Character spacing
  */
  void SetCharacterSpacing(double charSpacing) { m_charSpacing = charSpacing; }

  /// Get character spacing
  /**
  * \return Character spacing
  */
  double GetCharacterSpacing() { return m_charSpacing; }

  /// Set hyper link reference
  /**
  * \param href Hyperlink reference
  */
  void SetHRef(const wxString& href) { m_href = href; }

  /// Get hyper link reference
  /**
  * \return Reference to hyperlink string
  */
  wxString& GetHRef() { return m_href; }

  /// Set reference to an embedded markup table
  /**
  * \param table Pointer to the embedded table object
  */
  void SetTable(wxPdfTable* table) { m_table = table; }

  /// Get reference to an embedded markup table
  /**
  * \return Pointer to the embedded table object
  */
  wxPdfTable* GetTable() { return m_table; }

private:
  
  double           m_maxWidth;        ///< maximum line width
  double           m_lineDelta;       ///< line delta measure
  wxPdfAlignment   m_hAlign;          ///< horizontal alignment
  wxPdfAlignment   m_vAlign;          ///< vertical alignment
  double           m_height;          ///< height of cell
  unsigned int     m_currentContext;  ///< index of current context
  wxArrayPtrVoid   m_contexts;        ///< list of contexts
  unsigned int     m_currentLine;     ///< index of current line
  bool             m_aligned;         ///< alignment flag
  int              m_fillStyle;       ///< cell fill style
  wxPdfArrayDouble m_linewidth;       ///< list of line widths
  wxArrayInt       m_spaces;          ///< list of space counters
  wxUniChar        m_lastChar;        ///< last character of a chunk
  double           m_spaceWidth;      ///< width of space character
  double           m_charSpacing;     ///< extra character spacing (default: 0)
  wxString         m_href;            ///< hyper link reference
  wxPdfTable*      m_table;           ///< table reference
};

/// Class representing table cells. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfTableCell
{
public:
  /// Constructor
  /**
  * \param cell Pointer to the XML node of the cell
  * \param row Row index
  * \param col Column index
  * \param rows Row span
  * \param cols Column span
  */
  wxPdfTableCell(wxXmlNode* cell, unsigned int row, unsigned int col, unsigned int rows, unsigned int cols);

  /// Destructor
  virtual ~wxPdfTableCell();

  /// Get row of cell
  /**
  * \return Row index
  */
  unsigned int GetRow() const { return m_row; }

  /// Get column of cell
  /**
  * \return Column index
  */
  unsigned int GetCol() const { return m_col; }

  /// Get row span of cell
  /**
  * \return Row span
  */
  unsigned int GetRowSpan() const { return m_rowSpan;};

  /// Get column span of cell
  /**
  * \return Column span
  */
  unsigned int GetColSpan() const { return m_colSpan;};

  /// Set width of cell
  /**
  * \param w Width
  */
  void SetWidth(double w) { m_width = w;};

  /// Set height of cell
  /**
  * \param h Height
  */
  void SetHeight(double h) { m_height = h;};

  /// Get height of cell
  /**
  * \return Height
  */
  double GetHeight() const { return m_height;};

  /// Get width of cell
  /**
  * \return Width
  */
  double GetWidth() const { return m_width;};

  /// Set cell context
  /**
  * \param context Pointer to cell context
  */
  void SetContext(wxPdfCellContext* context) { m_context = context; }

  /// Get cell context
  /**
  * \return Pointer to cell context
  */
  wxPdfCellContext* GetContext() { return m_context; }

  /// Set horizontal alignment
  /**
  * \param hAlign Horizontal alignment
  */
  void SetHAlign(wxPdfAlignment hAlign) { m_hAlign = hAlign; }

  /// Get horizontal alignment
  /**
  * \return Horizontal alignment
  */
  wxPdfAlignment GetHAlign() { return m_hAlign; }

  /// Set vertical alignment
  /**
  * \param vAlign Vertical alignment
  */
  void SetVAlign(wxPdfAlignment vAlign) { m_vAlign = vAlign; }

  /// Get vertical alignment
  /**
  * \return Vertical alignment
  */
  wxPdfAlignment GetVAlign() { return m_vAlign; }

  /// Set border
  /**
  * \param border Border flags
  */
  void SetBorder(int border) { m_border = border; }

  /// Get border
  /**
  * \return Border flags
  */
  int GetBorder() { return m_border; }

  /// Check whether cell has a coloured or transparent background
  /**
  * \return TRUE if cell has background colour, FALSE otherwise
  */
  bool HasCellColour() const { return m_hasCellColour; };

  /// Set background colour of cell
  /**
  * \param colour Background colour
  */
  void SetCellColour(wxPdfColour colour) { m_hasCellColour = true; m_colourCell = colour;};

  /// Get background colour of cell
  /**
  * \return Background colour
  */
  wxPdfColour GetCellColour() const { return m_colourCell; };

  /// Get root node of cell
  /**
  * \return Pointer to XML node
  */
  wxXmlNode* GetXmlNode() { return m_cell; }

private:
  unsigned int      m_row;           ///< row index
  unsigned int      m_col;           ///< column index
  unsigned int      m_rowSpan;       ///< row span
  unsigned int      m_colSpan;       ///< column span

  wxPdfCellContext* m_context;       ///< table cell context
  wxXmlNode*        m_cell;          ///< xml root node of cell
  wxPdfAlignment    m_hAlign;        ///< horizontal alignment
  wxPdfAlignment    m_vAlign;        ///< vertical alignment
  int               m_border;        ///< border
  double            m_width;         ///< cell width
  double            m_height;        ///< cell height

  bool              m_hasCellColour;  ///< flag whether cell has background colour or is transparent
  wxPdfColour       m_colourCell;     ///< cell background colour
};

/// Hashmap class for document links
WX_DECLARE_HASH_MAP_WITH_DECL(long, wxPdfTableCell*, wxIntegerHash, wxIntegerEqual, wxPdfCellHashMap, class WXDLLIMPEXP_PDFDOC);

/// Class representing tables within a cell. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfTable
{
public:
  /// Constructor
  /**
  * \param document Pointer to the associated PDF document
  */
  wxPdfTable(wxPdfDocument* document);

  /// Destructor
  virtual ~wxPdfTable();

  /// Set minimum required row height
  /**
  * \param row Row index
  * \param height Minimum height
  */
  void SetMinRowHeight(int row, double height) { m_minHeights[row] = height; }

  /// Set maximum allowed row height
  /**
  * \param row Row index
  * \param height Maximum height
  */
  void SetMaxRowHeight(int row, double height) { m_maxHeights[row] = height; }

  /// Set width of column
  /**
  * \param col Column index
  * \param width Column width
  */
  void SetColumnWidth(int col, double width);

  /// Calculate cell dimensions respecting a maximum allowed width
  /**
  * \param maxWidth Maximum allowed width
  */
  void SetCellDimensions(double maxWidth);

  /// Insert a cell into the cell array
  /**
  * \param c Pointer to the table cell to insert
  */
  void InsertCell(wxPdfTableCell* c);

  /// Write table header on current page
  /**
  * \param writeHeader Flag whether to write the header
  * \param x Abscissa of the top left corner
  * \param y Ordinate of the top left corner
  * \return Vertical position after writing
  */
  double WriteOnPage(bool writeHeader, double x, double y);

  /// Write range of rows on current page
  /**
  * \param firstRow Index of first row
  * \param lastRow Index of last row
  * \param x Abscissa of the top left corner
  * \param y Ordinate of the top left corner
  * \param writeHeader Flag whether to write the header
  * \return Vertical position after writing
  */
  double WriteRowsOnPage(unsigned firstRow, unsigned lastRow, double x, double y, bool writeHeader);

  /// Add a page and return last table row on page
  /**
  * \param iter Iterator to current row index
  * \param endIter Iterator to end of row index list
  * \return Index of last row on page
  */
  unsigned int AddPage(wxArrayInt::const_iterator iter, wxArrayInt::const_iterator endIter);

  /// Write the whole table
  /**
  * \param writeHeader Flag whether to write the header
  * \param lastRowsOnPage List of last rows on each page
  * \param x Abscissa of the top left corner
  * \param y Ordinate of the top left corner
  * \return Vertical position after writing
  */
  double WriteTable(bool writeHeader, const wxArrayInt& lastRowsOnPage, double x, double y);

  /// Get height of row
  /**
  * \param row Row index
  * \return Row height
  */
  double GetRowHeight(int row) { const double height = m_rowHeights[row]; return height; };

  /// Write table to document
  void Write();

  /// Write range of rows
  /**
  * \param firstRow Index of first row
  * \param lastRow Index of last row
  * \param x Abscissa of the top left corner
  * \param y Ordinate of the top left corner
  * \param isHeaderRow Flag whether the rows are header rows
  * \return Vertical position after writing
  */
  double WriteRows(unsigned int firstRow, unsigned int lastRow, double x, double y, bool isHeaderRow);

  /// Set cell padding
  /**
  * \param pad Padding
  */
  void SetPad(double pad) { m_pad = pad; }

  /// Get cell padding
  /**
  * \return Padding
  */
  double GetPad() { return m_pad; }

  /// Set border flag
  /**
  * \param border Border flag
  */
  void SetBorder(bool border) { m_border = border; }

  /// Set border size
  /**
  * \param borderWidth Border width
  */
  void SetBorderWidth(double borderWidth) { m_borderWidth = borderWidth; }

  /// Set border colour
  /**
  * \param borderColour Border colour
  */
  void SetBorderColour(wxPdfColour borderColour) { m_borderColour = borderColour; }

  /// Check whether border should be drawn
  /**
  * \return TRUE if border should be drawn, FALSE otherwise
  */
  bool HasBorder() { return m_border; }

  /// Get total width of table
  /**
  * \return Total width
  */
  double GetTotalWidth() { return m_totalWidth; }

  /// Get height of table head
  /**
  * \return Header height
  */
  double GetHeadHeight() { return m_headHeight; }

  /// Get height of table body
  /**
  * \return Body height
  */
  double GetBodyHeight() { return m_bodyHeight; }

  /// Get total height of table
  /**
  * \return Total height
  */
  double GetTotalHeight() { return m_headHeight + m_bodyHeight; }

  /// Set index of first header row
  /**
  * \param row Row index
  */
  void SetHeadRowFirst(unsigned int row) { m_headRowFirst = row; }

  /// Set index of last header row
  /**
  * \param row Row index
  */
  void SetHeadRowLast(unsigned int row) { m_headRowLast = row; }

  /// Set index of first body row
  /**
  * \param row Row index
  */
  void SetBodyRowFirst(unsigned int row) { m_bodyRowFirst = row; }

  /// Set index of last body row
  /**
  * \param row Row index
  */
  void SetBodyRowLast(unsigned int row) { m_bodyRowLast = row; }

private: 
  /// write filling
  void WriteFillingOfCell(unsigned int row, unsigned int col, double x, double y) const;
  /// write filling
  void WriteFillingOfRow(unsigned int row, double x, double y) const;
  /// write filling
  void WriteFillingOfRows(unsigned int firstRow, unsigned int lastRow, double x, double y) const;

  /// write borders
  void WriteBordersOfCell(unsigned row, unsigned int col, double x, double y);
  /// write borders
  void WriteBordersOfRow(unsigned int row, double x, double y);
  /// write borders
  void WriteBordersOfRows(unsigned int firstRow, unsigned int lastRow, double x, double y);

  /// write content
  void WriteContentOfCell(unsigned int row, unsigned int col, double x, double y, bool isHeaderRow);
  /// write content
  void WriteContentOfRow(unsigned int row, double x, double y, bool isHeaderRow);
  /// write content
  double WriteContentOfRows(unsigned int firstRow, unsigned int lastRow, double x, double y, bool isHeaderRow);

  ///Calculate Rows after a page break in a table
  ///In case of line breaks the function returns an array containing first body rows on a next page
  ///In case of no new pages, the array is empty
  wxArrayInt GetLastRowsOnPage() const;
  /// Draw cell borders
  void DrawCellBorders(double x, double y, double w, double h, wxPdfTableCell* cell) const;
  /// Draw cell filling
  void DrawCellFilling(double x, double y, double w, double h, wxPdfTableCell* cell) const;
  /// Draw cell content
  void DrawCellContent(double x, double y, bool isHeaderRow, double w, double h, wxPdfTableCell* cell);
  /// calculate cell dimensions
  void CalculateCellDimension(unsigned row, unsigned col, double& w, double& h,
    wxPdfTableCell* cell) const;

  wxPdfDocument* m_document;     ///< document reference
  wxPdfDoubleHashMap m_minHeights;   ///< array of minimum row heights
  wxPdfDoubleHashMap m_rowHeights;   ///< array of row heights
  wxPdfDoubleHashMap m_colWidths;    ///< array of column widths
  wxPdfDoubleHashMap m_maxHeights;   ///< array of maximum row heights including row span heights

  double             m_totalWidth;   ///< total width
  double             m_bodyHeight;   ///< total height of table body
  double             m_headHeight;   ///< total height of table header

  unsigned int       m_headRowFirst; ///< index of first header row
  unsigned int       m_headRowLast;  ///< index of last header row
  unsigned int       m_bodyRowFirst; ///< index of first body row
  unsigned int       m_bodyRowLast;  ///< index of last body row

  unsigned int       m_nRows;        ///< number of rows
  unsigned int       m_nCols;        ///< number of columns
  wxPdfCellHashMap   m_table;        ///< array of table cells
  double             m_pad;          ///< cell padding
  bool               m_border;       ///< border flag
  double             m_borderWidth;  ///< border width (use -1 for current line width)
  wxPdfColour        m_borderColour; ///< border colour
};

#endif
