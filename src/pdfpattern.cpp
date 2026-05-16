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

// Image pattern
wxPdfPattern::wxPdfPattern(int index, double width, double height)
  : m_objIndex(0), m_index(index), m_patternStyle(wxPDF_PATTERNSTYLE_IMAGE),
    m_image(nullptr), m_templateId(0),
    m_drawColour(), m_fillColour(), m_hasFillColour(false),
    m_width(width), m_height(height),
    m_x1(0.0), m_y1(0.0), m_x2(0.0), m_y2(0.0), m_radius1(0.0), m_radius2(0.0),
    m_stops(), m_matrix()
{
}

// Template pattern
wxPdfPattern::wxPdfPattern(int index, double width, double height, int templateId)
  : m_objIndex(0), m_index(index), m_patternStyle(wxPDF_PATTERNSTYLE_TEMPLATE),
    m_image(nullptr), m_templateId(templateId),
    m_drawColour(), m_fillColour(), m_hasFillColour(false),
    m_width(width), m_height(height),
    m_x1(0.0), m_y1(0.0), m_x2(0.0), m_y2(0.0), m_radius1(0.0), m_radius2(0.0),
    m_stops(), m_matrix()
{
}

// Hatch pattern
wxPdfPattern::wxPdfPattern(int index, double width, double height, wxPdfPatternStyle patternStyle, const wxColour& drawColour, const wxColour& fillColour)
  : m_objIndex(0), m_index(index), m_patternStyle(patternStyle),
    m_image(nullptr), m_templateId(0),
    m_drawColour(drawColour), m_fillColour(fillColour), m_hasFillColour(fillColour.IsOk()),
    m_width(width), m_height(height),
    m_x1(0.0), m_y1(0.0), m_x2(0.0), m_y2(0.0), m_radius1(0.0), m_radius2(0.0),
    m_stops(), m_matrix()
{
}

// Linear gradient
wxPdfPattern::wxPdfPattern(int index, double x1, double y1, double x2, double y2, const wxGraphicsGradientStops& stops, const wxAffineMatrix2D& matrix)
  : m_objIndex(0), m_index(index), m_patternStyle(wxPDF_PATTERNSTYLE_LINEAR_GRADIENT),
    m_image(nullptr), m_templateId(0),
    m_drawColour(), m_fillColour(), m_hasFillColour(false),
    m_width(1.0), m_height(1.0),
    m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_radius1(0.0), m_radius2(0.0),
    m_stops(stops), m_matrix(matrix)
{
}

// Radial gradient
wxPdfPattern::wxPdfPattern(int index, double startX, double startY, double startRadius, double endX, double endY, double endRadius,
                           const wxGraphicsGradientStops& stops, const wxAffineMatrix2D& matrix)
  : m_objIndex(0), m_index(index), m_patternStyle(wxPDF_PATTERNSTYLE_RADIAL_GRADIENT),
    m_image(nullptr), m_templateId(0),
    m_drawColour(), m_fillColour(), m_hasFillColour(false),
    m_width(1.0), m_height(1.0),
    m_x1(startX), m_y1(startY), m_x2(endX), m_y2(endY), m_radius1(startRadius), m_radius2(endRadius),
    m_stops(stops), m_matrix(matrix)
{
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

  m_x1 = pattern.m_x1;
  m_y1 = pattern.m_y1;
  m_x2 = pattern.m_x2;
  m_y2 = pattern.m_y2;
  m_radius1 = pattern.m_radius1;
  m_radius2 = pattern.m_radius2;
  m_stops = pattern.m_stops;
  m_matrix = pattern.m_matrix;
}
