///////////////////////////////////////////////////////////////////////////////
// Name:        pdfutility.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-05-20
// Copyright:   (c) 2009-2026 Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfutility.h Definition of several general utility methods

#ifndef _PDF_UTILITY_H_
#define _PDF_UTILITY_H_

// wxWidgets headers
#include <wx/font.h>
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfproperties.h"

/// Class implementing several static utility methods
class WXDLLIMPEXP_PDFDOC wxPdfUtility
{
public:
  /// Create a unique ID
  /**
  * \param prefix Optional prefix for the ID
  * \return A unique ID string
  */
  static wxString GetUniqueId(const wxString& prefix = wxEmptyString);

  /// Formats a floating point number with a fixed precision
  /**
  * \param value the value to be formatted
  * \param precision the number of decimal places
  * \return the string representation of the number
  */
  static wxString Double2String(double value, int precision = 0);

  /// Parses a floating point number
  /**
  * \param str the string to be parsed
  * \return the value of floating point number given by the string representation,
  * 0 if the string could not be parsed.
  */
  static double String2Double(const wxString& str);

  /// Parses a floating point number taking units into account
  /**
  * \param str the string to be parsed
  * \param defaultUnit the unit that should be assumed if the string includes no explicit unit
  * \param scaleFactor the scale factor that should be applied for unit conversions involving pixels
  * \return the value of floating point number given by the string representation,
  * 0 if the string could not be parsed.
  */
  static double String2Double(const wxString& str, const wxString& defaultUnit, double scaleFactor = 1.0);

  /// Converts an integer number to a roman number
  /**
  * \param value integer value to be converted
  * \return the string representation of the integer value as a roman number
  */
  static wxString Convert2Roman(int value);

  /// Forces a floating point number into a fixed range
  /**
  * \param value value to be forced into range
  * \param minValue lower limit
  * \param maxValue upper limit
  * \return value conforming to the given range:
  *   \li the minValue if the value falls below the lower limit
  *   \li the value itself if it is within range
  *   \li the maxValue if the value exceeds the upper limit
  */
  static double ForceRange(double value, double minValue, double maxValue);

  /// Converts a wxColour to the corresponding PDF specification
  /**
  * \param colour colour to be converted to a hexadecimal string representation
  * \return the hexadecimal string representation of the colour
  */
  static wxString RGB2String(const wxColour& colour);

  /// Map font weight to font style
  /**
  * \param fontWeight weight of the font
  * \return the font style as style flags
  */
  static int MapFontWeight2FontStyle(wxPdfFontWeight fontWeight);
  /// \overload
  static int MapFontWeight2FontStyle(wxFontWeight fontWeight);

  /// Map font weight to font style in string representation
  /**
  * \param fontWeight weight of the font
  * \return the font style in string representation
  */
  static wxString MapFontWeight2FontStyleString(wxPdfFontWeight fontWeight);
  /// \overload
  static wxString MapFontWeight2FontStyleString(wxFontWeight fontWeight);

  /// Map font style to font weight
  /**
  * \param fontStyle style of the font
  * \return the font weight
  */
  static wxPdfFontWeight MapFontStyle2FontWeight(int fontStyle);
  /// \overload
  static wxPdfFontWeight MapFontStyle2FontWeight(const wxString& fontStyle);

  /// Map font style to font weight
  /**
  * \param fontStyle style of the font consisting of a sequence of one-letter codes
  * \return the font styles
  */
  static int MapFontStyle2StyleFlags(const wxString& fontStyle);

  /// Map wxFont to font style
  /**
  * \note This method only takes font properties into account that are not related to the font weight.
  * 
  * \param font the wxFont
  * \return the font style
  */
  static int MapFont2FontStyle(const wxFont& font);

  /// Map font name to font style
  /**
  * This method tries to deduce the font style from the font name.
  *
  * \param fontName the name of the font
  * \return the font style
  */
  static int MapFontName2FontStyle(const wxString& fontName);

  /// Map font styles to string representation
  /**
  * This method converts the font style flags to a human-readable string representation.
  *
  * \param fontName the name of the font
  * \return the font styles in string representation
  */
  static wxString MapFontStyle2String(int fontStyle);

private:
  static bool ms_seeded;  ///< flag whether random number generator is seeded
  static int  ms_s1;      ///< Random number generator seed 1
  static int  ms_s2;      ///< Random number generator seed 2
};

#endif
