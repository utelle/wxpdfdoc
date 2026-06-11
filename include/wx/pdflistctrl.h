///////////////////////////////////////////////////////////////////////////////
// Name:        pdflistctrl.h
// Purpose:     
// Author:      Blake Madden
// Created:     2026-06-10
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdflistctrl.h Interface of the wxPdfListCtrlOptions class

#ifndef _PDF_LISTCTRL_H_
#define _PDF_LISTCTRL_H_

// wxWidgets headers
#include <wx/defs.h>
#include <wx/font.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfcolour.h"

class wxListCtrl;

/// Table rendering style for wxListCtrl export.
enum wxPdfListCtrlStyle
{
    wxPDF_LISTCTRL_STYLE_GRID,   ///< Full grid with borders and optional cell fills (default)
    wxPDF_LISTCTRL_STYLE_SIMPLE  ///< Horizontal rules only — no vertical lines or cell fills (LaTeX \e booktabs style)
};

/// Class representing options for wxListCtrl export.
class WXDLLIMPEXP_PDFDOC wxPdfListCtrlOptions
{
public:
  /// Default constructor
  wxPdfListCtrlOptions();

  /// Set the font for the column headers
  /**
  * \param font The font to use for headers
  */
  void SetHeaderFont(const wxFont& font) { m_headerFont = font; }

  /// Get the font for the column headers
  /**
  * \return The header font
  */
  wxFont GetHeaderFont() const { return m_headerFont; }

  /// Set the font for the list items
  /**
  * \param font The font to use for items
  */
  void SetBodyFont(const wxFont& font) { m_bodyFont = font; }

  /// Get the font for the list items
  /**
  * \return The body font
  */
  wxFont GetBodyFont() const { return m_bodyFont; }

  /// Set the background colour for the column headers
  /**
  * \param colour The background colour
  */
  void SetHeaderBackgroundColour(const wxPdfColour& colour) { m_headerBackgroundColour = colour; }

  /// Get the background colour for the column headers
  /**
  * \return The header background colour
  */
  wxPdfColour GetHeaderBackgroundColour() const { return m_headerBackgroundColour; }

  /// Set the text colour for the column headers
  /**
  * \param colour The text colour
  */
  void SetHeaderTextColour(const wxPdfColour& colour) { m_headerTextColour = colour; }

  /// Get the text colour for the column headers
  /**
  * \return The header text colour
  */
  wxPdfColour GetHeaderTextColour() const { return m_headerTextColour; }

  /// Set the background colour for alternate rows
  /**
  * \param colour The background colour for even rows
  */
  void SetAlternateRowBackgroundColour(const wxPdfColour& colour) { m_alternateRowBackgroundColour = colour; }

  /// Get the background colour for alternate rows
  /**
  * \return The alternate row background colour
  */
  wxPdfColour GetAlternateRowBackgroundColour() const { return m_alternateRowBackgroundColour; }

  /// Set the colour for borders
  /**
  * \param colour The border colour
  */
  void SetBorderColour(const wxPdfColour& colour) { m_borderColour = colour; }

  /// Get the colour for borders
  /**
  * \return The border colour
  */
  wxPdfColour GetBorderColour() const { return m_borderColour; }

  /// Set whether to draw horizontal borders between rows
  /**
  * \param draw true to draw row borders
  */
  void SetDrawRowBorders(bool draw) { m_drawRowBorders = draw; }

  /// Check whether to draw horizontal borders between rows
  /**
  * \return true if row borders should be drawn
  */
  bool GetDrawRowBorders() const { return m_drawRowBorders; }

  /// Set whether to draw vertical borders between columns
  /**
  * \param draw true to draw column borders
  */
  void SetDrawColumnBorders(bool draw) { m_drawColumnBorders = draw; }

  /// Check whether to draw vertical borders between columns
  /**
  * \return true if column borders should be drawn
  */
  bool GetDrawColumnBorders() const { return m_drawColumnBorders; }

  /// Set whether to show "Continued on next page" and "(continued)" labels
  /**
  * \param show true to show the labels
  */
  void SetShowContinued(bool show) { m_showContinued = show; }

  /// Check whether to show "Continued on next page" and "(continued)" labels
  /**
  * \return true if labels should be shown
  */
  bool GetShowContinued() const { return m_showContinued; }

  /// Set whether to stretch the table to the full printable page width.
  /**
  * When enabled, all columns are scaled proportionally so that the table
  * fills the available width between the left and right margins, analogous
  * to LaTeX's \c tabularx or \c tabular* with \c \\linewidth. Multi-column
  * layout is disabled when this option is active.
  * \param fit true to stretch to full page width
  */
  void SetFitToPage(bool fit) { m_fitToPage = fit; }

  /// Check whether the table is stretched to the full printable page width.
  /**
  * \return true if full-page-width mode is active
  */
  bool GetFitToPage() const { return m_fitToPage; }

  /// Set the table rendering style.
  /**
  * \c wxPDF_LISTCTRL_STYLE_GRID (default) draws full cell borders and honours fill colours.
  * \c wxPDF_LISTCTRL_STYLE_SIMPLE renders only a top rule, a rule below the header, and a
  * bottom rule with no vertical lines or cell fills, analogous to the LaTeX \c booktabs package.
  * \param style The rendering style
  */
  void SetStyle(wxPdfListCtrlStyle style) { m_style = style; }

  /// Get the table rendering style.
  /**
  * \return The current rendering style
  */
  wxPdfListCtrlStyle GetStyle() const { return m_style; }

private:
  wxFont      m_headerFont;
  wxFont      m_bodyFont;
  wxPdfColour m_headerBackgroundColour;
  wxPdfColour m_headerTextColour;
  wxPdfColour m_alternateRowBackgroundColour;
  wxPdfColour m_borderColour;
  bool                m_drawRowBorders;
  bool                m_drawColumnBorders;
  bool                m_showContinued;
  bool                m_fitToPage;
  wxPdfListCtrlStyle  m_style;
};

#endif
