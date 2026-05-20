///////////////////////////////////////////////////////////////////////////////
// Name:        pdflinks.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-06-24
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdflinks.h Interface of the wxPdfLink and wxPdfPageLink classes

#ifndef _PDF_LINKS_H_
#define _PDF_LINKS_H_

// wxWidgets headers
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

/// Class representing internal or external links.
class WXDLLIMPEXP_PDFDOC wxPdfLink
{
public:
  /// Constructor for internal link
  /**
  * Use this constructor to create an \b internal link reference.
  * \see wxPdfDocument::Link(), wxPdfDocument::Write(), wxPdfDocument::Cell(), wxPdfDocument::ClippedCell(), wxPdfDocument::Image(), wxPdfDocument::RotatedImage()
  */
  wxPdfLink(int linkRef);

  /// Constructor for external link
  /**
  * Use this constructor to create an \b external link reference.
  * \see wxPdfDocument::Link(), wxPdfDocument::Write(), wxPdfDocument::Cell(), wxPdfDocument::ClippedCell(), wxPdfDocument::Image(), wxPdfDocument::RotatedImage()
  */
  wxPdfLink(const wxString& linkURL);

  /// Copy constructor
  wxPdfLink(const wxPdfLink& pdfLink);

  /// Assignment operator
  wxPdfLink& operator=(const wxPdfLink& pdfLink);

  /// Destructor
  virtual ~wxPdfLink();

  /// Check whether this instance is a valid link reference
  bool  IsValid() const { return m_isValid; }

  /// Check whether this instance is an internal reference
  bool  IsLinkRef() const { return m_isRef; }

  /// Get the internal link reference
  int   GetLinkRef() const { return m_linkRef; }

  /// Get the external link reference
  const wxString GetLinkURL() const { return m_linkURL; }

  /// Set page number and position on page
  void   SetLink(int page, double ypos) { m_page = page; m_ypos = ypos; }

  /// Get the page this link refers to
  /**
  * \return The page number
  */
  int    GetPage() { return m_page; }

  /// Get the page position this link refers to
  /**
  * \return The Y position on the page
  */
  double GetPosition() { return m_ypos; }

private:
  bool     m_isValid;   ///< Flag whether this instance is valid
  bool     m_isRef;     ///< Flag whether this is an internal link reference
  int      m_linkRef;   ///< Internal link reference
  wxString m_linkURL;   ///< External link reference
  int      m_page;      ///< Page number this link refers to
  double   m_ypos;      ///< Position on page this link refers to
};

/// Class representing the sensitive area of links referring to a page. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfPageLink : public wxPdfLink
{
public:
  /// Constructor
  wxPdfPageLink(double x, double y, double w, double h, const wxPdfLink& pdfLink);

  /// Destructor
  virtual ~wxPdfPageLink();

  /// Get the X offset
  /**
  * \return The X offset
  */
  double GetX() { return m_x; }

  /// Get the Y offset
  /**
  * \return The Y offset
  */
  double GetY() { return m_y; }

  /// Get the width
  /**
  * \return The width
  */
  double GetWidth() { return m_w; }

  /// Get the height
  /**
  * \return The height
  */
  double GetHeight() { return m_h; }

private:
  double m_x;   ///< X offset of sensitive area
  double m_y;   ///< Y offset of sensitive area
  double m_w;   ///< Width of sensitive area
  double m_h;   ///< Height of sensitive area
};

#endif
