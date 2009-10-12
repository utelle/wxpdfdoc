///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatacore.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2008-08-07
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatacore.cpp Implementation of PDF core fonts

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes

#include "wx/pdfcorefontdata.h"
#include "wx/pdffontdatacore.h"
#include "wx/pdffontdescription.h"

#include "wxmemdbg.h"

wxPdfFontDataCore::wxPdfFontDataCore(const wxString& family, const wxString& alias, const wxString& name,
                                     short* cwArray, const wxPdfKernPairDesc* kpArray,
                                     const wxPdfFontDescription& desc)
  : wxPdfFontData()
{
  m_type   = wxT("core");
  m_family = family;
  m_alias  = alias;
  m_name   = name;
  m_fullNames.Add(name);
  m_desc  = desc;
  m_style = FindStyleFromName(name);

  if (cwArray != NULL)
  {
    m_cw = new wxPdfGlyphWidthMap();
    int j;
    for (j = 0; j <256; j++)
    {
      (*m_cw)[j] = cwArray[j];
    }
  }

  if (kpArray != NULL)
  {
    m_kp = new wxPdfKernPairMap();
    wxPdfKernPairMap::iterator kp;
    wxPdfKernWidthMap* kwMap = NULL;
    wxPdfKernWidthMap::iterator kw;
    wxUint32 u1, u2;
    wxUint32 u1prev = 0;
    size_t k = 0;
    while ((u1 = kpArray[k].unicode1) != 0 && (u2 = kpArray[k].unicode2) != 0)
    {
      if (u1 != u1prev)
      {
        u1prev = u1;
        wxPdfKernPairMap::iterator kp = (*m_kp).find(u1);
        if (kp == (*m_kp).end())
        {
          kwMap = new wxPdfKernWidthMap();
          (*m_kp)[u1] = kwMap;
        }
        else
        {
          kwMap = kp->second;
        }
      }
      (*kwMap)[u2] = kpArray[k].kerning;
      ++k;
    }
  }

  m_initialized = true;
}

wxPdfFontDataCore::~wxPdfFontDataCore()
{
}

wxString
wxPdfFontDataCore::GetWidthsAsString(bool subset, wxPdfSortedArrayInt* usedGlyphs, wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(subset);
  wxUnusedVar(usedGlyphs);
  wxUnusedVar(subsetGlyphs);
  wxString s = wxString(wxT("["));
  int i;
  for (i = 32; i <= 255; i++)
  {
    s += wxString::Format(wxT("%d "), (*m_cw)[i]);
  }
  s += wxString(wxT("]"));
  return s;
}

#if wxUSE_UNICODE
wxMBConv*
wxPdfFontDataCore::GetEncodingConv() const
{
  wxMBConv* conv;
  if (m_name.IsSameAs(wxT("Symbol")) || m_name.IsSameAs(wxT("ZapfDingbats")))
  {
    conv = &wxConvISO8859_1;
  }
  else
  {
    conv = GetWinEncodingConv();
  }
  return conv;
}
#endif

double
wxPdfFontDataCore::GetStringWidth(const wxString& s, wxPdfChar2GlyphMap* convMap, bool withKerning) const
{
  wxUnusedVar(convMap);
  double w = 0;
  // Get width of a string in the current font
#if wxUSE_UNICODE
  wxCharBuffer wcb(s.mb_str(*GetEncodingConv()));
  const char* str = (const char*) wcb;
#else
  const char* str = s.c_str();
#endif

  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    w += (*m_cw)[(unsigned char) str[i]];
  }
  if (withKerning)
  {
    int kerningWidth = GetKerningWidth(s);
    if (kerningWidth != 0)
    {
      w += (double) kerningWidth;
    }
  }
  return w / 1000;
}

wxString
wxPdfFontDataCore::ConvertCID2GID(const wxString& s, wxPdfChar2GlyphMap* convMap, 
                                  wxPdfSortedArrayInt* usedGlyphs, 
                                  wxPdfChar2GlyphMap* subsetGlyphs)
{
  // No conversion from cid to gid
  wxUnusedVar(convMap);
  wxUnusedVar(usedGlyphs);
  wxUnusedVar(subsetGlyphs);
  return s;
}
