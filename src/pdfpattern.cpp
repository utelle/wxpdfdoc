///////////////////////////////////////////////////////////////////////////////
// Name:        pdfpattern.cpp
// Purpose:     Implementation of pattern handling
// Author:      Ulrich Telle
// Created:     2009-06-18
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfpattern.cpp Implementation of the pattern handling

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/pdfpattern.h"

wxPdfPattern::wxPdfPattern(int index, double width, double height)
  : m_objIndex(0), m_index(index), m_width(width), m_height(height), m_patternStyle(wxPDF_PATTERNSTYLE_IMAGE)
{
}

wxPdfPattern::wxPdfPattern(int index, double width, double height, int templateId)
  : m_objIndex(0), m_index(index), m_width(width), m_height(height), m_patternStyle(wxPDF_PATTERNSTYLE_TEMPLATE), m_templateId(templateId)
{
}

wxPdfPattern::wxPdfPattern(int index, double width, double height, wxPdfPatternStyle patternStyle, const wxColour& drawColour, const wxColour& fillColour)
  : m_objIndex(0), m_index(index), m_width(width), m_height(height), m_patternStyle(patternStyle), m_drawColour(drawColour)
{
  m_hasFillColour = fillColour.IsOk();
  if (m_hasFillColour)
  {
    m_fillColour = fillColour;
  }
}

wxPdfPattern::wxPdfPattern(const wxPdfPattern& pattern)
{
  m_objIndex = pattern.m_objIndex;
  m_index    = pattern.m_index;
  m_width    = pattern.m_width;
  m_height   = pattern.m_height;
  m_image    = pattern.m_image;
  m_templateId = pattern.m_templateId;
  m_patternStyle = pattern.m_patternStyle;
  m_drawColour = pattern.m_drawColour;
  m_fillColour = pattern.m_fillColour;
}
