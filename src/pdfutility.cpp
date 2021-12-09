///////////////////////////////////////////////////////////////////////////////
// Name:        pdfutility.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-05-20
// Copyright:   (c) Ulrich Telle
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
  static int ixPixel = allowedUnits.Index("px");

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
