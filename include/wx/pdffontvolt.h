///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontvolt.h
// Purpose:     Definition of VOLT font data
// Author:      Ulrich Telle
// Modified by:
// Created:     2010-09-21
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontvolt.h Definition of VOLT font data

#ifndef _PDF_FONT_VOLT_H_
#define _PDF_FONT_VOLT_H_

// wxWidgets headers
#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

// Forward declarations
//class WXDLLIMPEXP_FWD_PDFDOC wxPdfFontDescription;

//class wxPdfKernPairDesc;

/// Class representing the Adobe core fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfVolt
{
public :
  /// Default constructor
  wxPdfVolt();

#if 0
  /// Constructor
  /**
  * \param family the family name of the font
  * \param name the font name
  * \param alias the alias name of the font or an empty string
  * \param cwArray an array with the character widths 
  * \param kpArray an array with kerning pairs
  * \param desc a font description
  */
  wxPdfVolt(const wxString& family, const wxString& name, const wxString& alias,
                    short* cwArray, const wxPdfKernPairDesc* kpArray,
                    const wxPdfFontDescription& desc);
#endif

  /// Default destructor
  virtual ~wxPdfVolt();

  void LoadVoltData(wxXmlNode* volt);

  wxString ProcessRules(const wxString& text);

protected:

private:
  wxArrayPtrVoid m_rules;
};

#endif
