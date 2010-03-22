///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatatype0.h
// Purpose:    Definition of font data for Type0 fonts
// Author:      Ulrich Telle
// Modified by:
// Created:     2009-05-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatatype0.h Definition of font data for Type0 fonts

#ifndef _PDF_FONT_DATA_TYPE0_H_
#define _PDF_FONT_DATA_TYPE0_H_

// Type0 fonts are only available in Unicode build
#if wxUSE_UNICODE

// wxWidgets headers
#include <wx/strconv.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

/// Class representing Type 0 fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDataType0 : public wxPdfFontData
{
public:
  ///< Default constructor
  wxPdfFontDataType0();

  /// Default destructor
  virtual ~wxPdfFontDataType0();

  /// Get the character width array as string
  /**
  * \param subset flag whether subsetting is enabled
  * \param usedGlyphs the list of used glyphs
  * \param subsetGlyphs the mapping of glyphs to subset glyphs
  * \return the string representation of the character widths
  */
  virtual wxString GetWidthsAsString(bool subset, wxPdfSortedArrayInt* usedGlyphs = NULL, wxPdfChar2GlyphMap* subsetGlyphs = NULL) const;

  /// Get the width of a string
  /**
  * \param s the string for which the width should be calculated
  * \param convMap the character to glyph mapping
  * \param withKerning flag indicating whether kerning should be taken into account
  * \return the width of the string
  */
  virtual double GetStringWidth(const wxString& s, wxPdfChar2GlyphMap* convMap = NULL, bool withKerning = false) const;

  /// Convert character codes to glyph numbers
  /**
  * \param s the string to be converted
  * \param convMap the character to glyph mapping
  * \param usedGlyphs the list of used glyphs
  * \param subsetGlyphs the mapping of glyphs to subset glyphs
  * \return the converted string
  */
  virtual wxString ConvertCID2GID(const wxString& s, wxPdfChar2GlyphMap* convMap = NULL, wxPdfSortedArrayInt* usedGlyphs = NULL, wxPdfChar2GlyphMap* subsetGlyphs = NULL);

  /// Load the font metrics XML file
  /**
  * \param root the root node of the XML font metric file
  * \return TRUE if the metric file could be processed successfully, FALSE otherwise
  */
  virtual bool LoadFontMetrics(wxXmlNode* root);

  /// Get the associated encoding converter
  /**
  * \return the encoding converter associated with this font
  */
  virtual wxMBConv* GetEncodingConv() const { return m_conv; }

  /// Create the associated default encoding converter
  virtual void CreateDefaultEncodingConv();

  // Set half-width ranges (ie for Japan)
  /**
  * \param hwRange flag whether font has a half-width range
  * \param hwFirst CID of the first half-width character
  * \param hwLast CID of the last half-width character
  */
  void SetHalfWidthRanges(bool hwRange, wxChar hwFirst, wxChar hwLast);

protected:
  /// Check whether the font has a half width range
  bool HasHalfWidthRange() const { return m_hwRange; }

  /// Get the first character in the half width range
  wxChar HalfWidthRangeFirst() const { return m_hwFirst; }

  /// Get the last character in the half width range
  wxChar HalfWidthRangeLast() const { return m_hwLast; }

  bool      m_hwRange;  ///< Flag whether the font has a half width range
  wxChar    m_hwFirst;  ///< CID of the first half width character
  wxChar    m_hwLast;   ///< CID of the last half width character

  wxMBConv* m_conv;     ///< Assocated encoding converter
};

#endif // wxUSE_UNICODE

#endif
