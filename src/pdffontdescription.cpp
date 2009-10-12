///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdescription.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2008-08-10
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdescription.cpp Implementation of wxPdfFontDescription class

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes

#include "wx/pdffontdescription.h"

#include "wxmemdbg.h"

wxPdfFontDescription::wxPdfFontDescription()
  : m_ascent(0), m_descent(0), m_capHeight(0), m_flags(0),
    m_fontBBox(wxEmptyString), m_italicAngle(0), m_stemV(0),
    m_missingWidth(0), m_xHeight(0),
    m_underlinePosition(-100), m_underlineThickness(50)
{
}

wxPdfFontDescription::wxPdfFontDescription (int ascent, int descent, int capHeight, int flags,
                                            const wxString& fontBBox, int italicAngle, int stemV,
                                            int missingWidth, int xHeight,
                                            int underlinePosition, int underlineThickness)
  : m_ascent(ascent), m_descent(descent), m_capHeight(capHeight), m_flags(flags),
    m_fontBBox(fontBBox), m_italicAngle(italicAngle), m_stemV(stemV),
    m_missingWidth(missingWidth), m_xHeight(xHeight),
    m_underlinePosition(underlinePosition), m_underlineThickness(underlineThickness)
{
}

wxPdfFontDescription::wxPdfFontDescription(const wxPdfFontDescription& desc)
{
  m_ascent             = desc.m_ascent;
  m_descent            = desc.m_descent;
  m_capHeight          = desc.m_capHeight;
  m_flags              = desc.m_flags;
  m_fontBBox           = desc.m_fontBBox;
  m_italicAngle        = desc.m_italicAngle;
  m_stemV              = desc.m_stemV;
  m_missingWidth       = desc.m_missingWidth;
  m_xHeight            = desc.m_xHeight;
  m_underlinePosition  = desc.m_underlinePosition;
  m_underlineThickness = desc.m_underlineThickness;
}

wxPdfFontDescription::~wxPdfFontDescription()
{
}
