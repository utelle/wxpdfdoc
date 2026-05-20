///////////////////////////////////////////////////////////////////////////////
// Name:        pdfutility.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-05-20
// Copyright:   (c) 2009-2026 Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfutility.cpp Implementation of wxPdfUtility methods

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes
#include "wx/intl.h"

#include "wx/pdfutility.h"

bool wxPdfUtility::ms_seeded = false;
int  wxPdfUtility::ms_s1     = 0;
int  wxPdfUtility::ms_s2     = 0;

#define MODMULT(a, b, c, m, s) q = s / a; s = b * (s - a * q) - c * q; if (s < 0) s += m

wxString
wxPdfUtility::GetUniqueId(const wxString& prefix)
{
  wxString uid = (prefix.Length() <= 114) ? prefix : prefix.Left(114);

  wxDateTime ts;
  ts.SetToCurrent();

  int q;
  int z;
  if (!ms_seeded)
  {
    ms_seeded = true;
    ms_s1 = ts.GetSecond() ^ (~ts.GetMillisecond());
    if (ms_s1 == 0) ms_s1 = 1;
    ms_s2 = wxGetProcessId();
  }
  MODMULT(53668, 40014, 12211, 2147483563L, ms_s1);
  MODMULT(52774, 40692,  3791, 2147483399L, ms_s2);

  z = ms_s1 - ms_s2;
  if (z < 1)
  {
    z += 2147483562;
  }

  uid += wxString::Format(wxS("%08x%05x"), ts.GetSecond(), ts.GetMillisecond());
  uid += Double2String(z * 4.656613e-9,8);

  return uid;
}

wxString
wxPdfUtility::Double2String(double value, int precision)
{
  wxString number;
  number = wxString::FromCDouble(value, precision);
  return number;
}

double
wxPdfUtility::String2Double(const wxString& str)
{
  double value = 0;
  str.ToCDouble(&value);
  return value;
}

double
wxPdfUtility::String2Double(const wxString& str, const wxString& defaultUnit, double scaleFactor)
{
  static double convFactor[5][5] = {
    /*            pt        mm        cm       in       px */
    /* 0: pt */ { 1.,       25.4/72., 2.54/72, 1./72.,  1.       },
    /* 1: mm */ { 72./25.4, 1.,       1./10.,  1./25.4, 72./25.4 },
    /* 2: cm */ { 72./2.54, 10.,      1.,      1./2.54, 72./2.54 },
    /* 3: in */ { 72.,      25.4,     2.54,    1.,      72.      },
    /* 4: px */ { 1.,       25.4/72., 2.54/72, 1./72.,  1.       }
  };
  static wxString allowed[] = { "pt", "mm", "cm", "in", "px" };
  static wxArrayString allowedUnits(5, allowed);

  wxString strValue = str.Strip(wxString::both);
  wxString valueUnit = (strValue.length() > 2) ? strValue.Right(2) : defaultUnit;

  int ixDefaultUnit = allowedUnits.Index(defaultUnit);
  ixDefaultUnit = (ixDefaultUnit != wxNOT_FOUND) ? ixDefaultUnit : 1;

  int ixValueUnit = allowedUnits.Index(valueUnit);
  ixValueUnit = (ixValueUnit != wxNOT_FOUND) ? ixValueUnit : ixDefaultUnit;

  double value = 0;
  strValue.ToCDouble(&value);

  if (ixValueUnit != ixDefaultUnit)
  {
    value *= convFactor[ixValueUnit][ixDefaultUnit];
    if (ixDefaultUnit == 4)
    {
      value *= scaleFactor;
    }
  }
  return value;
}

wxString
wxPdfUtility::Convert2Roman(int value)
{
  wxString result = wxEmptyString;

  if (value > 0 && value < 4000)
  {
    static wxString romans = wxS("MDCLXVI");
    int pos = 6;  // Point to LAST character in 'romans'
    int currentDigit;

    while (value > 0)
    {
      currentDigit = value % 10;
      if (currentDigit == 4 || currentDigit == 9)
      {
        result.Prepend(romans.Mid(pos  - currentDigit / 4, 1));
        result.Prepend(romans.Mid(pos, 1));
      }
      else
      {
        int x = currentDigit % 5;
        while (x-- > 0)
        {
          result.Prepend(romans.Mid(pos, 1));
        }
        if (currentDigit >= 5)
        {
          result.Prepend(romans.Mid(pos - 1, 1));
        }
      }
      value /= 10;
      pos -= 2;
    }
  }
  else
  {
    result = wxS("???");
  }
  return result;
}

double
wxPdfUtility::ForceRange(double value, double minValue, double maxValue)
{
  if (value < minValue)
  {
    value = minValue;
  }
  else if (value > maxValue)
  {
    value = maxValue;
  }
  return value;
}

wxString
wxPdfUtility::RGB2String(const wxColour& colour)
{
  double r = colour.Red();
  double g = colour.Green();
  double b = colour.Blue();
  wxString rgb = Double2String(r/255.,3) + wxS(" ") +
                 Double2String(g/255.,3) + wxS(" ") +
                 Double2String(b/255.,3);
  return rgb;
}

int
wxPdfUtility::MapFontWeight2FontStyle(wxPdfFontWeight fontWeight)
{
  static std::vector<int> fontStyles =
  {
    wxPDF_FONTSTYLE_REGULAR,  wxPDF_FONTSTYLE_THIN,      wxPDF_FONTSTYLE_EXTRALIGHT,
    wxPDF_FONTSTYLE_LIGHT,    wxPDF_FONTSTYLE_REGULAR,   wxPDF_FONTSTYLE_MEDIUM,
    wxPDF_FONTSTYLE_SEMIBOLD, wxPDF_FONTSTYLE_BOLD,      wxPDF_FONTSTYLE_EXTRABOLD,
    wxPDF_FONTSTYLE_HEAVY,    wxPDF_FONTSTYLE_EXTRAHEAVY
  };
  int fontStyle = wxPDF_FONTSTYLE_REGULAR;
  int weight = (int)fontWeight;
  if (weight > 0)
  {
    weight += 50;
    int weightIndex = std::min((weight - weight % 100) / 100, (int)fontStyles.size() - 1);
    return fontStyles[weightIndex];
  }
  return fontStyle;
}

int
wxPdfUtility::MapFontWeight2FontStyle(wxFontWeight fontWeight)
{
  static std::vector<int> fontStyles =
  {
    wxPDF_FONTSTYLE_REGULAR,  wxPDF_FONTSTYLE_THIN,      wxPDF_FONTSTYLE_EXTRALIGHT,
    wxPDF_FONTSTYLE_LIGHT,    wxPDF_FONTSTYLE_REGULAR,   wxPDF_FONTSTYLE_MEDIUM,
    wxPDF_FONTSTYLE_SEMIBOLD, wxPDF_FONTSTYLE_BOLD,      wxPDF_FONTSTYLE_EXTRABOLD,
    wxPDF_FONTSTYLE_HEAVY,    wxPDF_FONTSTYLE_EXTRAHEAVY
  };
  int fontStyle = wxPDF_FONTSTYLE_REGULAR;

#if wxCHECK_VERSION(3,1,2)
  int weight = (int)fontWeight;
  if (weight > 0)
  {
    weight += 50;
    int weightIndex = std::min((weight - weight % 100) / 100, (int)fontStyles.size() - 1);
    fontStyle = fontStyles[weightIndex];
  }
#else
  // wxWidgets below version 3.1.2 had only "normal", "bold", and "light" font weights
  if (fontWeight == wxFONTWEIGHT_BOLD)
  {
    fontStyle = wxPDF_FONTSTYLE_BOLD;
  }
  else if (fontWeight == wxFONTWEIGHT_LIGHT)
  {
    fontStyle = wxPDF_FONTSTYLE_LIGHT;
  }
#endif

  return fontStyle;
}

wxString
wxPdfUtility::MapFontWeight2FontStyleString(wxFontWeight fontWeight)
{
#if wxCHECK_VERSION(3,1,2)
  wxPdfFontWeight weight = static_cast<wxPdfFontWeight>(fontWeight);
  return MapFontWeight2FontStyleString(weight);
#else
  if (fontWeight == wxFONTWEIGHT_BOLD)
    return wxString("B");
  else if (fontWeight == wxFONTWEIGHT_LIGHT)
    return wxString("L");
  else
      return wxString();
#endif
}

wxString
wxPdfUtility::MapFontWeight2FontStyleString(wxPdfFontWeight fontWeight)
{
  static std::vector<wxString> fontStyleStrings =
  { // 0   100  200  300  400  500  600  700  800  900  1000
       "", "T", "E", "L", "",  "M", "D", "B", "X", "H", "A"
  };

  wxString styleString = "";
  int weight = (int)fontWeight;
  if (weight > 0)
  {
    weight += 50;
    int weightIndex = std::min((weight - weight % 100) / 100, (int)fontStyleStrings.size() - 1);
    styleString = fontStyleStrings[weightIndex];
  }
  return styleString;
}

wxPdfFontWeight
wxPdfUtility::MapFontStyle2FontWeight(int fontStyle)
{
  static std::vector<wxPdfFontWeight> fontWeights =
  {
    wxPDF_FONTWEIGHT_THIN,       wxPDF_FONTWEIGHT_EXTRALIGHT, wxPDF_FONTWEIGHT_LIGHT,
    wxPDF_FONTWEIGHT_EXTRAHEAVY, wxPDF_FONTWEIGHT_HEAVY,      wxPDF_FONTWEIGHT_EXTRABOLD,
    wxPDF_FONTWEIGHT_MEDIUM,     wxPDF_FONTWEIGHT_SEMIBOLD,   wxPDF_FONTWEIGHT_BOLD
  };

  static std::vector<int> fontStyleFlags =
  {
    wxPDF_FONTSTYLE_THIN,      wxPDF_FONTSTYLE_EXTRALIGHT, wxPDF_FONTSTYLE_LIGHT,
    wxPDF_FONTSTYLE_MEDIUM,    wxPDF_FONTSTYLE_SEMIBOLD,   wxPDF_FONTSTYLE_BOLD,
    wxPDF_FONTSTYLE_EXTRABOLD, wxPDF_FONTSTYLE_HEAVY,      wxPDF_FONTSTYLE_EXTRAHEAVY
  };
  static int lightMask = wxPDF_FONTSTYLE_THIN      | wxPDF_FONTSTYLE_EXTRALIGHT | wxPDF_FONTSTYLE_LIGHT;
  static int boldMask  = wxPDF_FONTSTYLE_MEDIUM    | wxPDF_FONTSTYLE_SEMIBOLD   | wxPDF_FONTSTYLE_BOLD |
                         wxPDF_FONTSTYLE_EXTRABOLD | wxPDF_FONTSTYLE_HEAVY      | wxPDF_FONTSTYLE_EXTRAHEAVY;
  static int lightBoldMask = wxPDF_FONTSTYLE_MEDIUM    | wxPDF_FONTSTYLE_SEMIBOLD;
  static int heavyBoldMask = wxPDF_FONTSTYLE_EXTRABOLD | wxPDF_FONTSTYLE_HEAVY | wxPDF_FONTSTYLE_EXTRAHEAVY;

  wxPdfFontWeight fontWeight = wxPDF_FONTWEIGHT_REGULAR;
  bool hasLightStyle = fontStyle & lightMask;
  bool hasBoldStyle = fontStyle & boldMask;
  bool hasLightBoldStyle = fontStyle & lightBoldMask;
  bool hasHeavyBoldStyle = fontStyle & heavyBoldMask;

  if (hasLightStyle && hasBoldStyle)
  {
    // Flags for at least one light style and one bold style is set.
    // In this case we return regular weight.
    fontWeight = wxPDF_FONTWEIGHT_REGULAR;
  }
  else if (hasLightBoldStyle && hasHeavyBoldStyle)
  {
    // Flags for at least one light bold style (medium or semibold) and
    // one heavy bold style (extrabold, heavy, extraheavy) is set.
    // In this case we return bold weight.
    fontWeight = wxPDF_FONTWEIGHT_BOLD;
  }
  else
  {
    // Check individual style flags which correspond to weights
    // If more than one style flag is set, the font weight closer 
    // to regular weight is chosen for light styles, and closer to
    // bold weight for bold styles.
    for (size_t j = 0; j < fontStyleFlags.size(); ++j)
    {
      if (fontStyle && fontStyleFlags[j])
      {
        fontWeight = fontWeights[j];
      }
    }
  }
  return fontWeight;
}

wxPdfFontWeight
wxPdfUtility::MapFontStyle2FontWeight(const wxString& fontStyle)
{
  return MapFontStyle2FontWeight(MapFontStyle2StyleFlags(fontStyle));
}

int
wxPdfUtility::MapFontStyle2StyleFlags(const wxString& fontStyle)
{
  static std::vector<int> fontStyleFlags =
  {
    // Weight styles
    wxPDF_FONTSTYLE_THIN,      wxPDF_FONTSTYLE_EXTRALIGHT, wxPDF_FONTSTYLE_LIGHT,
    wxPDF_FONTSTYLE_MEDIUM,    wxPDF_FONTSTYLE_SEMIBOLD,   wxPDF_FONTSTYLE_BOLD,
    wxPDF_FONTSTYLE_EXTRABOLD, wxPDF_FONTSTYLE_HEAVY,      wxPDF_FONTSTYLE_EXTRAHEAVY,
    // Other styles
    wxPDF_FONTSTYLE_ITALIC,    wxPDF_FONTSTYLE_UNDERLINE,  wxPDF_FONTSTYLE_OVERLINE,
    wxPDF_FONTSTYLE_STRIKEOUT
  };
  static wxString validStyles("TELMDBXHAIUOS");
  int styleFlags = wxPDF_FONTSTYLE_REGULAR;
  wxString styleString = fontStyle.Upper();
  // TODO: Should we ignore invalid style codes?
  if (styleString.find_first_not_of(validStyles) == wxString::npos)
  {
    for (size_t j = 0; j < validStyles.length(); ++j)
    {
      if (styleString.Find(validStyles[j]) != wxNOT_FOUND)
      {
        styleFlags |= fontStyleFlags[j];
      }
    }
  }
  return styleFlags;
}

int
wxPdfUtility::MapFont2FontStyle(const wxFont& font)
{
  int styles = 0;
  if (font.GetStyle() == wxFONTSTYLE_ITALIC)
  {
    styles |= wxPDF_FONTSTYLE_ITALIC;
  }
  if (font.GetUnderlined())
  {
    styles |= wxPDF_FONTSTYLE_UNDERLINE;
  }
  if (font.GetStrikethrough())
  {
    styles |= wxPDF_FONTSTYLE_STRIKEOUT;
  }
  return styles;
}

int
wxPdfUtility::MapFontName2FontStyle(const wxString& fontName)
{
  // TODO: Support widths: Condensed, Narrow, Compressed, Expanded, Extended?
  static std::vector<wxString> styleStrings =
  {
    "regular",    "book",
    "italic",     "oblique",     "slanted",
    "underline",  "overline",    "strikethrough",
    "thin",
    "extralight", "extra-light", "ultralight", "ultra-light",
    "light",
    "medium",
    "semibold",   "semi-bold",   "demi",
    "bold",
    "extrabold",  "extra-bold",  "ultrabold",  "ultra-bold",
    "heavy",      "black",
    "extraheavy", "extra-heavy"
  };
  static std::vector<int> fontStyleFlags =
  {
    wxPDF_FONTSTYLE_REGULAR,    wxPDF_FONTSTYLE_REGULAR,
    wxPDF_FONTSTYLE_ITALIC,     wxPDF_FONTSTYLE_ITALIC,     wxPDF_FONTSTYLE_ITALIC,
    wxPDF_FONTSTYLE_UNDERLINE,  wxPDF_FONTSTYLE_OVERLINE,   wxPDF_FONTSTYLE_STRIKEOUT,
    wxPDF_FONTSTYLE_THIN,
    wxPDF_FONTSTYLE_EXTRALIGHT, wxPDF_FONTSTYLE_EXTRALIGHT, wxPDF_FONTSTYLE_EXTRALIGHT, wxPDF_FONTSTYLE_EXTRALIGHT,
    wxPDF_FONTSTYLE_LIGHT,
    wxPDF_FONTSTYLE_MEDIUM,
    wxPDF_FONTSTYLE_SEMIBOLD,   wxPDF_FONTSTYLE_SEMIBOLD,   wxPDF_FONTSTYLE_SEMIBOLD,
    wxPDF_FONTSTYLE_BOLD,
    wxPDF_FONTSTYLE_EXTRABOLD,  wxPDF_FONTSTYLE_EXTRABOLD,  wxPDF_FONTSTYLE_EXTRABOLD,  wxPDF_FONTSTYLE_EXTRABOLD,
    wxPDF_FONTSTYLE_HEAVY,      wxPDF_FONTSTYLE_HEAVY,
    wxPDF_FONTSTYLE_EXTRAHEAVY
  };

  wxString localStyle = fontName.Lower();
  int style = wxPDF_FONTSTYLE_REGULAR;
  for (size_t j = 0; j < styleStrings.size(); ++j)
  {
    if (localStyle.Find(styleStrings[j]) != wxNOT_FOUND)
    {
      style |= fontStyleFlags[j];
    }
  }
  return style;
}

wxString
wxPdfUtility::MapFontStyle2String(int fontStyle)
{
  static std::vector<int> fontStyles =
  {
    wxPDF_FONTSTYLE_ITALIC,    wxPDF_FONTSTYLE_UNDERLINE,  wxPDF_FONTSTYLE_OVERLINE,
    wxPDF_FONTSTYLE_STRIKEOUT,
    wxPDF_FONTSTYLE_THIN,      wxPDF_FONTSTYLE_EXTRALIGHT, wxPDF_FONTSTYLE_LIGHT,
    wxPDF_FONTSTYLE_MEDIUM,    wxPDF_FONTSTYLE_SEMIBOLD,   wxPDF_FONTSTYLE_BOLD,
    wxPDF_FONTSTYLE_EXTRABOLD, wxPDF_FONTSTYLE_HEAVY,      wxPDF_FONTSTYLE_EXTRAHEAVY
  };
  static std::vector<wxString> styleStrings =
  {
    wxString(_("Italic")),        wxString(_("Underline")),  wxString(_("Overline")),
    wxString(_("StrikeThrough")),
    wxString(_("Thin")),          wxString(_("ExtraLight")), wxString(_("Light")),
    wxString(_("Medium")),        wxString(_("SemiBold")),   wxString(_("Bold")),
    wxString(_("ExtraBold")),     wxString(_("Heavy")),      wxString(_("ExtraHeavy"))
  };

  wxString styleString = wxEmptyString;
  for (size_t j = 0; j < fontStyles.size(); ++j)
  {
    if (fontStyle & fontStyles[j])
    {
      styleString += ((j > 0) ? wxString("|") : wxString("")) + styleStrings[j];
    }
  }
  if (styleString.IsEmpty())
  {
    styleString = wxString(_("Regular"));
  }
  return styleString;
}
