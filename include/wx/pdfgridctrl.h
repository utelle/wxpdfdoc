///////////////////////////////////////////////////////////////////////////////
// Name:        pdfgridctrl.h
// Purpose:
// Author:      Blake Madden
// Created:     2026-06-22
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfgridctrl.h Interface of the wxPdfGridOptions class

#ifndef _PDF_GRIDCTRL_H_
#define _PDF_GRIDCTRL_H_

#if wxUSE_GRID

// wxWidgets headers
#include <wx/defs.h>
#include <wx/bitmap.h>
#include <wx/font.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfcolour.h"
#include "wx/pdffont.h"

class wxGrid;

/// Table rendering style for wxGrid export.
enum wxPdfGridStyle
{
    wxPDF_GRID_STYLE_GRID,   ///< Full grid with borders and optional cell fills (default)
    wxPDF_GRID_STYLE_SIMPLE  ///< Horizontal rules only — no vertical lines or cell fills (LaTeX \e booktabs style)
};

/// Optional interface to supply bitmaps for cells and column headers to the PDF exporter.
/**
 * Derive from this class and pass an instance to wxPdfGridOptions::SetBitmapProvider()
 * to draw icons alongside text in grid cells or column headers.
 *
 * Both methods return wxNullBitmap by default, so you only need to override
 * whichever you need (cell bitmaps, column header bitmaps, or both).
 *
 * The provider pointer is \e not owned by wxPdfGridOptions; the caller must
 * keep it alive for the duration of the export call.
 *
 * \par Example
 * \code
 * class MyIconProvider : public wxPdfGridBitmapProvider
 * {
 * public:
 *     wxBitmap GetColHeaderBitmap(int col) const override
 *     {
 *         // Return the bitmap your attribute provider already uses for this column
 *         return m_headerIcons[col];
 *     }
 * private:
 *     std::vector<wxBitmap> m_headerIcons;
 * };
 * \endcode
 *
 * \see wxPdfGridOptions::SetBitmapProvider()
 */
class WXDLLIMPEXP_PDFDOC wxPdfGridBitmapProvider
{
public:
    /// Returns a bitmap to draw to the left of the cell text, or \c wxNullBitmap for text only.
    virtual wxBitmap GetCellBitmap(int WXUNUSED(row), int WXUNUSED(col)) const { return wxNullBitmap; }

    /// Returns a bitmap to draw to the left of the column header text, or \c wxNullBitmap for text only.
    virtual wxBitmap GetColHeaderBitmap(int WXUNUSED(col)) const { return wxNullBitmap; }

    virtual ~wxPdfGridBitmapProvider() = default;
};

/// Options that control how a wxGrid is rendered into a PDF document.
/**
 * Pass an instance to wxPdfDocument::AddGrid() or wxPdfDC::DrawGrid() to
 * configure colours, fonts, borders, and layout behaviour.
 *
 * \par Example
 * \code
 * // Standard grid export
 * wxPdfDocument doc;
 * doc.AddPage();
 *
 * wxPdfGridOptions options;
 * options.SetHeaderBackgroundColour(wxPdfColour(220, 220, 255));
 * options.SetAlternateRowBackgroundColour(wxPdfColour(245, 245, 245));
 * doc.AddGrid(myGrid, options);
 * doc.SaveAsFile("grid.pdf");
 *
 * // LaTeX booktabs-style export stretched to page width
 * wxPdfDocument doc2;
 * doc2.AddPage();
 *
 * wxPdfGridOptions simple;
 * simple.SetStyle(wxPDF_GRID_STYLE_SIMPLE);
 * simple.SetFitToPage(true);
 * doc2.AddGrid(myGrid, simple);
 * doc2.SaveAsFile("simple.pdf");
 * \endcode
 *
 * \see wxPdfDocument::AddGrid(), wxPdfDC::DrawGrid()
 */
class WXDLLIMPEXP_PDFDOC wxPdfGridOptions
{
public:
  /// Default constructor
  wxPdfGridOptions();

  /// Set the font for the column headers
  void SetHeaderFont(const wxFont& font) { m_headerFont = font; }
  /// Get the font for the column headers
  wxFont GetHeaderFont() const { return m_headerFont; }

  /// Set the font for the grid cells
  void SetBodyFont(const wxFont& font) { m_bodyFont = font; }
  /// Get the font for the grid cells
  wxFont GetBodyFont() const { return m_bodyFont; }

  /// Set the PDF font for the column headers (takes precedence over SetHeaderFont())
  void SetHeaderPdfFont(const wxPdfFont& font) { m_headerPdfFont = font; }
  /// Get the PDF font for the column headers
  wxPdfFont GetHeaderPdfFont() const { return m_headerPdfFont; }

  /// Set the PDF font for the grid cells (takes precedence over SetBodyFont())
  void SetBodyPdfFont(const wxPdfFont& font) { m_bodyPdfFont = font; }
  /// Get the PDF font for the grid cells
  wxPdfFont GetBodyPdfFont() const { return m_bodyPdfFont; }

  /// Set the background colour for the column headers
  void SetHeaderBackgroundColour(const wxPdfColour& colour) { m_headerBackgroundColour = colour; }
  /// Get the background colour for the column headers
  wxPdfColour GetHeaderBackgroundColour() const { return m_headerBackgroundColour; }

  /// Set the text colour for the column headers
  void SetHeaderTextColour(const wxPdfColour& colour) { m_headerTextColour = colour; }
  /// Get the text colour for the column headers
  wxPdfColour GetHeaderTextColour() const { return m_headerTextColour; }

  /// Set the background colour for alternate rows
  void SetAlternateRowBackgroundColour(const wxPdfColour& colour) { m_alternateRowBackgroundColour = colour; }
  /// Get the background colour for alternate rows
  wxPdfColour GetAlternateRowBackgroundColour() const { return m_alternateRowBackgroundColour; }

  /// Set the colour for borders
  void SetBorderColour(const wxPdfColour& colour) { m_borderColour = colour; }
  /// Get the colour for borders
  wxPdfColour GetBorderColour() const { return m_borderColour; }

  /// Set whether to draw horizontal borders between rows
  void SetDrawRowBorders(bool draw) { m_drawRowBorders = draw; }
  /// Check whether to draw horizontal borders between rows
  bool GetDrawRowBorders() const { return m_drawRowBorders; }

  /// Set whether to draw vertical borders between columns
  void SetDrawColumnBorders(bool draw) { m_drawColumnBorders = draw; }
  /// Check whether to draw vertical borders between columns
  bool GetDrawColumnBorders() const { return m_drawColumnBorders; }

  /// Set whether to show "Continued on next page" and "(continued)" labels
  void SetShowContinued(bool show) { m_showContinued = show; }
  /// Check whether to show "Continued on next page" and "(continued)" labels
  bool GetShowContinued() const { return m_showContinued; }

  /// Set whether to stretch the table to the full printable page width.
  /**
  * When enabled, all columns are scaled proportionally so that the table
  * fills the available width between the left and right margins.
  * Multi-column layout is disabled when this option is active.
  * \param fit true to stretch to full page width
  */
  void SetFitToPage(bool fit) { m_fitToPage = fit; }
  /// Check whether the table is stretched to the full printable page width.
  bool GetFitToPage() const { return m_fitToPage; }

  /// Set the table rendering style.
  /**
  * \c wxPDF_GRID_STYLE_GRID (default) draws full cell borders and honours fill colours.
  * \c wxPDF_GRID_STYLE_SIMPLE renders only a top rule, a rule below the header, and a
  * bottom rule with no vertical lines or cell fills, analogous to the LaTeX \c booktabs package.
  * \param style The rendering style
  */
  void SetStyle(wxPdfGridStyle style) { m_style = style; }
  /// Get the table rendering style.
  wxPdfGridStyle GetStyle() const { return m_style; }

  /// Set the starting row (1-indexed)
  void SetFromRow(int row) { m_fromRow = row; }
  /// Get the starting row (1-indexed)
  int GetFromRow() const { return m_fromRow; }

  /// Set the starting column (1-indexed)
  void SetFromColumn(int column) { m_fromColumn = column; }
  /// Get the starting column (1-indexed)
  int GetFromColumn() const { return m_fromColumn; }

  /// Set the ending row (1-indexed, -1 for all)
  void SetToRow(int row) { m_toRow = row; }
  /// Get the ending row (1-indexed, -1 for all)
  int GetToRow() const { return m_toRow; }

  /// Set the ending column (1-indexed, -1 for all)
  void SetToColumn(int column) { m_toColumn = column; }
  /// Get the ending column (1-indexed, -1 for all)
  int GetToColumn() const { return m_toColumn; }

  /// Set whether column headers should be exported
  void SetIncludeColumnHeaders(bool include) { m_includeColumnHeaders = include; }
  /// Check whether column headers should be exported
  bool GetIncludeColumnHeaders() const { return m_includeColumnHeaders; }

  /// Set whether the row label column (left-most) should be exported
  /**
  * When enabled, a column of width proportional to the grid's GetRowLabelSize()
  * is prepended containing each row's label (e.g. row numbers).
  * \param include true to include row labels (default: false)
  */
  void SetIncludeRowLabels(bool include) { m_includeRowLabels = include; }
  /// Check whether the row label column should be exported
  bool GetIncludeRowLabels() const { return m_includeRowLabels; }

  /// Set whether per-cell background and text colours from wxGridCellAttr should be applied
  /**
  * When enabled (the default), the exporter reads each cell's wxGridCellAttr and
  * applies its background colour as a fill and its text colour to the cell.
  * \param use true to honour per-cell attributes (default: true)
  */
  void SetUseCellAttributes(bool use) { m_useCellAttributes = use; }
  /// Check whether per-cell colours are applied
  bool GetUseCellAttributes() const { return m_useCellAttributes; }

  /// Set an optional bitmap provider for cell and column-header icons.
  /**
  * The provider is \e not owned by this object; the caller must keep it alive
  * for the duration of the export call.
  * Pass \c nullptr (the default) to disable bitmap rendering.
  * \param provider Pointer to a wxPdfGridBitmapProvider, or nullptr
  */
  void SetBitmapProvider(const wxPdfGridBitmapProvider* provider) { m_bitmapProvider = provider; }
  /// Get the current bitmap provider (may be nullptr)
  const wxPdfGridBitmapProvider* GetBitmapProvider() const { return m_bitmapProvider; }

private:
  wxFont        m_headerFont;
  wxFont        m_bodyFont;
  wxPdfFont     m_headerPdfFont;
  wxPdfFont     m_bodyPdfFont;
  wxPdfColour   m_headerBackgroundColour;
  wxPdfColour   m_headerTextColour;
  wxPdfColour   m_alternateRowBackgroundColour;
  wxPdfColour   m_borderColour;
  bool          m_drawRowBorders;
  bool          m_drawColumnBorders;
  bool          m_showContinued;
  bool          m_fitToPage;
  wxPdfGridStyle m_style;
  int           m_fromRow;
  int           m_fromColumn;
  int           m_toRow;
  int           m_toColumn;
  bool          m_includeColumnHeaders;
  bool          m_includeRowLabels;
  bool          m_useCellAttributes;
  const wxPdfGridBitmapProvider* m_bitmapProvider;
};

#endif // wxUSE_GRID

#endif // _PDF_GRIDCTRL_H_
