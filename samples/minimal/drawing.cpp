///////////////////////////////////////////////////////////////////////////////
// Name:        drawing.cpp
// Purpose:     Demonstration of graphics primitives in wxPdfDocument
// Author:      Ulrich Telle
// Created:     2005-08-29
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/filename.h>

#include "wx/pdfdoc.h"
#include "wx/pdfshape.h"

/**
* Drawing of geometric figures
*
* This example shows how to draw lines, rectangles, ellipses, polygons and curves with line style.
*/

int
drawing(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("pattern1.png")) &&
      wxFileName::IsFileReadable(wxS("pattern2.png")))
  {
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.SetFont(wxS("Helvetica"), wxS(""), 10);

    wxImage pattern1(wxS("pattern1.png"));
    wxImage pattern2(wxS("pattern2.png"));
    pdf.AddPattern(wxS("pattern1"), pattern1, 5, 5);
    pdf.AddPattern(wxS("pattern2"), pattern2, 10, 10);

    pdf.AddPage();

    wxPdfArrayDouble dash1;
    dash1.Add(3.5);
    dash1.Add(7.0);
    dash1.Add(1.75);
    dash1.Add(3.5);
    wxPdfLineStyle style(0.5, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash1, 3.5, wxColour(255, 0, 0));

    wxPdfArrayDouble dash2;
    wxPdfLineStyle style2(0.5, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash2, 0., wxColour(255, 0, 0));

    wxPdfArrayDouble dash3;
    dash3.Add(0.7);
    dash3.Add(3.5);
    wxPdfLineStyle style3(1.0, wxPDF_LINECAP_ROUND, wxPDF_LINEJOIN_ROUND, dash3, 0., wxColour(255, 0, 0));

    wxPdfLineStyle style4(0.5, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash1, 3.5, wxColour(255, 0, 0));

    wxPdfArrayDouble dash5;
    wxPdfLineStyle style5(0.25, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash5, 0., wxColour(0, 0, 0));

    wxPdfArrayDouble dash6;
    dash6.Add(3.5);
    dash6.Add(3.5);
    wxPdfLineStyle style6(0.5, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash6, 0., wxColour(0, 255, 0));

    wxPdfArrayDouble dash7;
    wxPdfLineStyle style7(2.5, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash7, 3.5, pdf.GetPatternColour(wxS("pattern1")) /*wxColour(200, 200, 0)*/);

    wxPdfArrayDouble dash8;
    wxPdfLineStyle style8(0.5, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash8, 0., wxColour(0, 0, 0));

    // Line
    pdf.Text(5, 7, wxS("Line examples"));
    pdf.SetLineStyle(style);
    pdf.Line(5, 10, 80, 30);
    pdf.SetLineStyle(style2);
    pdf.Line(5, 10, 5, 30);
    pdf.SetLineStyle(style3);
    pdf.Line(5, 10, 80, 10);

    // Rect
    pdf.Text(100, 7, wxS("Rectangle examples"));
    pdf.SetLineStyle(style5);
    pdf.SetFillColour(wxColour(220, 220, 200));
    pdf.Rect(100, 10, 40, 20, wxPDF_STYLE_FILLDRAW);
    pdf.SetLineStyle(style3);
    pdf.Rect(145, 10, 40, 20, wxPDF_STYLE_DRAW);

    // Curve
    pdf.Text(5, 37, wxS("Curve examples"));
    pdf.SetLineStyle(style6);
    pdf.Curve(5, 40, 30, 55, 70, 45, 60, 75, wxPDF_STYLE_DRAW);
    pdf.Curve(80, 40, 70, 75, 150, 45, 100, 75, wxPDF_STYLE_FILL);
    pdf.SetFillColour(wxColour(200, 220, 200));
    pdf.Curve(140, 40, 150, 55, 180, 45, 200, 75, wxPDF_STYLE_FILLDRAW);

    // Circle and ellipse
    pdf.Text(5, 82, wxS("Circle and ellipse examples"));
    pdf.SetLineStyle(style5);
    pdf.Circle(25,105,20);
    pdf.SetLineStyle(style6);
    pdf.Circle(25,105,10, 90, 180, wxPDF_STYLE_DRAW);
    pdf.Circle(25,105,10, 270, 360, wxPDF_STYLE_FILL);
    pdf.Circle(25,105,10, 270, 360, wxPDF_STYLE_DRAWCLOSE);

    pdf.SetLineStyle(style5);
    pdf.Ellipse(100,105,40,20);
    pdf.SetLineStyle(style6);
    pdf.Ellipse(100,105,20,10, 0, 90, 180, wxPDF_STYLE_DRAW);
    pdf.Ellipse(100,105,20,10, 0, 270, 360, wxPDF_STYLE_FILLDRAW);

    pdf.SetLineStyle(style5);
    pdf.Ellipse(175,105,30,15, 45);
    pdf.SetLineStyle(style6);
    pdf.Ellipse(175,105,15,7.50, 45, 90, 180, wxPDF_STYLE_DRAW);
    pdf.SetFillColour(wxColour(220, 200, 200));
    pdf.Ellipse(175,105,15,7.50, 45, 270, 360, wxPDF_STYLE_FILL);

    // Polygon
    pdf.Text(5, 132, wxS("Polygon examples"));
    pdf.SetLineStyle(style8);

    wxPdfArrayDouble x1, y1;
    x1.Add(5);  y1.Add(135);
    x1.Add(45); y1.Add(135);
    x1.Add(15); y1.Add(165);
    pdf.Polygon(x1, y1);

    wxPdfArrayDouble x2, y2;
    x2.Add(60); y2.Add(135);
    x2.Add(80); y2.Add(135);
    x2.Add(80); y2.Add(155);
    x2.Add(70); y2.Add(165);
    x2.Add(50); y2.Add(155);
    pdf.SetLineStyle(style6);
    pdf.Polygon(x2, y2, wxPDF_STYLE_FILLDRAW);

    wxPdfArrayDouble x3, y3;
    x3.Add(120); y3.Add(135);
    x3.Add(140); y3.Add(135);
    x3.Add(150); y3.Add(155);
    x3.Add(110); y3.Add(155);
    pdf.SetLineStyle(style7);
    pdf.Polygon(x3, y3, wxPDF_STYLE_DRAW);

    wxPdfArrayDouble x4, y4;
    x4.Add(160); y4.Add(135);
    x4.Add(190); y4.Add(155);
    x4.Add(170); y4.Add(155);
    x4.Add(200); y4.Add(160);
    x4.Add(160); y4.Add(165);
    pdf.SetLineStyle(style6);
    //pdf.SetFillColour(wxColour(220, 220, 220));
    pdf.SetFillPattern(wxS("pattern2"));
    pdf.Polygon(x4, y4, wxPDF_STYLE_FILLDRAW);

    // Regular polygon
    pdf.Text(5, 172, wxS("Regular polygon examples"));
    pdf.SetLineStyle(style5);
    pdf.SetFillColour(wxColour(220, 220, 220));
    pdf.RegularPolygon(20, 190, 15, 6, 0, 1, wxPDF_STYLE_FILL);
    pdf.RegularPolygon(55, 190, 15, 6);
    pdf.SetLineStyle(style7);
    pdf.RegularPolygon(55, 190, 10, 6, 45, 0, wxPDF_STYLE_FILLDRAW);
    pdf.SetLineStyle(style5);
    pdf.SetFillColour(wxColour(200, 220, 200));
    pdf.RegularPolygon(90, 190, 15, 3, 0, 1, wxPDF_STYLE_FILLDRAW,
                                             wxPDF_STYLE_FILL, wxPdfLineStyle(), wxColour(255, 200, 200));
    pdf.RegularPolygon(125, 190, 15, 4, 30, 1, wxPDF_STYLE_DRAW, wxPDF_STYLE_DRAW, style6);
    pdf.RegularPolygon(160, 190, 15, 10);

    // Star polygon
    pdf.Text(5, 212, wxS("Star polygon examples"));
    pdf.SetLineStyle(style5);
    pdf.StarPolygon(20, 230, 15, 20, 3, 0, 1, wxPDF_STYLE_FILL);
    pdf.StarPolygon(55, 230, 15, 12, 5);
    pdf.SetLineStyle(style7);
    pdf.StarPolygon(55, 230, 7, 12, 5, 45, 0, wxPDF_STYLE_FILLDRAW);
    pdf.SetLineStyle(style5);
    pdf.SetFillColour(wxColour(220, 220, 200));
    pdf.StarPolygon(90, 230, 15, 20, 6, 0, 1, wxPDF_STYLE_FILLDRAW,
                                              wxPDF_STYLE_FILL, wxPdfLineStyle(), wxColour(255, 200, 200));
    pdf.StarPolygon(125, 230, 15, 5, 2, 30, 1, wxPDF_STYLE_DRAW, wxPDF_STYLE_DRAW, style6);
    pdf.StarPolygon(160, 230, 15, 10, 3);
    pdf.StarPolygon(160, 230, 7, 50, 26);

    // Rounded rectangle
    pdf.Text(5, 252, wxS("Rounded rectangle examples"));
    pdf.SetLineStyle(style8);
    pdf.RoundedRect(5, 255, 40, 30, 3.50, wxPDF_CORNER_ALL, wxPDF_STYLE_FILLDRAW);
    pdf.RoundedRect(50, 255, 40, 30, 6.50, wxPDF_CORNER_TOP_LEFT);
    pdf.SetLineStyle(style6);
    pdf.RoundedRect(95, 255, 40, 30, 10.0, wxPDF_CORNER_ALL, wxPDF_STYLE_DRAW);
    pdf.SetFillColour(wxColour(200, 200, 200));
    pdf.RoundedRect(140, 255, 40, 30, 8.0, wxPDF_CORNER_TOP_RIGHT | wxPDF_CORNER_BOTTOM_RIGHT, wxPDF_STYLE_FILLDRAW);

    // Examples of various fill patterns
    pdf.AddPage();

    pdf.Text(10, 15, wxS("Fill pattern examples"));
    pdf.SetDrawColour(wxColour(0, 0, 0));

    int tplHatch = pdf.BeginTemplate(0, 0, 4, 4);
    pdf.SetTextColour(0);
    pdf.SetDrawColour(wxColour(0, 0, 0));
    pdf.SetFillColour(wxColour(128, 128, 255));
    pdf.Circle(2, 2, 1.5, 0, 360, wxPDF_STYLE_FILLDRAW);
    pdf.SetFillColour(wxColour(128, 255, 128));
    pdf.Rect(2, 2, 1.75, 1.75, wxPDF_STYLE_FILLDRAW);
    pdf.EndTemplate();

    pdf.AddPattern(wxS("hatch1"), wxPDF_PATTERNSTYLE_BDIAGONAL_HATCH, 1, 1, wxColour(224, 0, 0));
    pdf.AddPattern(wxS("hatch2"), wxPDF_PATTERNSTYLE_FDIAGONAL_HATCH, 2, 2, wxColour(160, 160, 0));
    pdf.AddPattern(wxS("hatch3"), wxPDF_PATTERNSTYLE_CROSSDIAG_HATCH, 4, 4, wxColour(224, 0, 224));
    pdf.AddPattern(wxS("hatch4"), wxPDF_PATTERNSTYLE_HORIZONTAL_HATCH, 1, 1, wxColour(255, 0, 0));
    pdf.AddPattern(wxS("hatch5"), wxPDF_PATTERNSTYLE_VERTICAL_HATCH, 2, 2, wxColour(0, 0, 255));
    pdf.AddPattern(wxS("hatch6"), wxPDF_PATTERNSTYLE_CROSS_HATCH, 4, 4, wxColour(0, 96, 0));
    pdf.AddPattern(wxS("hatch7"), wxPDF_PATTERNSTYLE_BRICK_HATCH, 2, 2, wxColour(96, 96, 96), wxColour(255, 192, 128));
    pdf.AddPattern(wxS("hatch8"), wxPDF_PATTERNSTYLE_HERRINGBONE_HATCH, 2, 2, wxColour(128, 128, 128));
    pdf.AddPattern(wxS("hatch9"), wxPDF_PATTERNSTYLE_BASKETWEAVE_HATCH, 2, 2, wxColour(224, 160, 96));

    pdf.AddPattern(wxS("hatch10"), tplHatch, 4, 4);
    pdf.AddPattern(wxS("hatch11"), tplHatch, 6, 6);

    pdf.Text(25, 25, wxS("BDiagonal"));
    pdf.SetFillPattern(wxS("hatch1"));
    pdf.Rect(25, 30, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(75, 25, wxS("FDiagonal"));
    pdf.SetFillPattern(wxS("hatch2"));
    pdf.Rect(75, 30, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(125, 25, wxS("CrossDiag"));
    pdf.SetFillPattern(wxS("hatch3"));
    pdf.Rect(125, 30, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(25, 65, wxS("Horizontal"));
    pdf.SetFillPattern(wxS("hatch4"));
    pdf.Rect(25, 70, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(75, 65, wxS("Vertical"));
    pdf.SetFillPattern(wxS("hatch5"));
    pdf.Rect(75, 70, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(125, 65, wxS("Cross"));
    pdf.SetFillPattern(wxS("hatch6"));
    pdf.Rect(125, 70, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(25, 105, wxS("Brick"));
    pdf.SetFillPattern(wxS("hatch7"));
    pdf.Rect(25, 110, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(75, 105, wxS("HerringBone"));
    pdf.SetFillPattern(wxS("hatch8"));
    pdf.Rect(75, 110, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(125, 105, wxS("BasketWeave"));
    pdf.SetFillPattern(wxS("hatch9"));
    pdf.Rect(125, 110, 25, 25, wxPDF_STYLE_FILLDRAW);

    pdf.Text(25, 145, wxS("Template 1x"));
    pdf.SetFillPattern(wxS("hatch10"));
    pdf.Rect(25, 150, 40, 40, wxPDF_STYLE_FILLDRAW);

    pdf.Text(75, 145, wxS("Template 2x"));
    pdf.SetFillPattern(wxS("hatch11"));
    pdf.Rect(75, 150, 40, 40, wxPDF_STYLE_FILLDRAW);

    pdf.AddPage();

    pdf.SetFont(wxS("Helvetica"), wxS("B"), 20);
    pdf.SetLineWidth(1);

    pdf.SetDrawColour(50, 0, 0, 0);
    pdf.SetFillColour(100, 0, 0, 0);
    pdf.SetTextColour(100, 0, 0, 0);
    pdf.Rect(10, 10, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(10, 40, wxS("Cyan"));

    pdf.SetDrawColour(0, 50, 0, 0);
    pdf.SetFillColour(0, 100, 0, 0);
    pdf.SetTextColour(0, 100, 0, 0);
    pdf.Rect(40, 10, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(40, 40, wxS("Magenta"));

    pdf.SetDrawColour(0, 0, 50, 0);
    pdf.SetFillColour(0, 0, 100, 0);
    pdf.SetTextColour(0, 0, 100, 0);
    pdf.Rect(70, 10, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(70, 40, wxS("Yellow"));

    pdf.SetDrawColour(0, 0, 0, 50);
    pdf.SetFillColour(0, 0, 0, 100);
    pdf.SetTextColour(0, 0, 0, 100);
    pdf.Rect(100, 10, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(100, 40, wxS("Black"));

    pdf.SetDrawColour(128, 0, 0);
    pdf.SetFillColour(255, 0, 0);
    pdf.SetTextColour(255, 0, 0);
    pdf.Rect(10, 50, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(10, 80, wxS("Red"));

    pdf.SetDrawColour(0, 127, 0);
    pdf.SetFillColour(0, 255, 0);
    pdf.SetTextColour(0, 255, 0);
    pdf.Rect(40, 50, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(40, 80, wxS("Green"));

    pdf.SetDrawColour(0, 0, 127);
    pdf.SetFillColour(0, 0, 255);
    pdf.SetTextColour(0, 0, 255);
    pdf.Rect(70, 50, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(70, 80, wxS("Blue"));

    pdf.SetDrawColour(127);
    pdf.SetFillColour(0);
    pdf.SetTextColour(0);
    pdf.Rect(10, 90, 20, 20, wxPDF_STYLE_FILLDRAW);
    pdf.Text(10, 120, wxS("Gray"));

    pdf.AddSpotColour(wxS("PANTONE 404 CVC"), 0, 9.02, 23.14, 56.08);
    pdf.SetFillColour(wxString(wxS("PANTONE 404 CVC")));
    pdf.Rect(10, 130, 20, 20, wxPDF_STYLE_FILL);
    pdf.Text(10, 160, wxS("PANTONE 404 CVC"));

    pdf.SetLineWidth(0.2);
    pdf.SetFont(wxS("Helvetica"), wxS(""), 48);
    pdf.SetTextRenderMode(wxPDF_TEXT_RENDER_FILLSTROKE);
    pdf.SetDrawColour(31);
    pdf.SetTextPattern(wxS("pattern2"));
    pdf.Text(10, 200, wxS("Text with Pattern"));
    pdf.SetTextRenderMode();
    pdf.SetTextColour(0);

    pdf.AddPage();
    pdf.SetAutoPageBreak(false);
    pdf.SetFont(wxS("Helvetica"), wxS(""), 10);
    pdf.SetLineWidth(0.2);
    pdf.SetDrawColour(0);

    pdf.Curve(25, 40, 50, 55, 90, 45, 80, 75, wxPDF_STYLE_DRAW);
    wxPdfShape shape1;
    shape1.MoveTo(25,40);
    shape1.CurveTo(50, 55, 90, 45, 80, 75);
    pdf.ShapedText(shape1, wxS("This is a simple text string along a shaped line."));

    pdf.Curve(80, 175, 90, 145, 50, 155, 25, 140, wxPDF_STYLE_DRAW);
    wxPdfShape shape2;
    shape2.MoveTo(80, 175);
    shape2.CurveTo(90, 145, 50, 155, 25, 140);
    pdf.ShapedText(shape2, wxS("This is a simple text string along a shaped line."));

    pdf.Curve(125, 40, 150, 55, 190, 45, 180, 75, wxPDF_STYLE_DRAW);
    wxPdfShape shape3;
    shape3.MoveTo(125,40);
    shape3.CurveTo(150, 55, 190, 45, 180, 75);
    pdf.ShapedText(shape3, wxS("Repeat me! "), wxPDF_SHAPEDTEXTMODE_REPEAT);

    wxPdfShape shape4;
    shape4.MoveTo(125, 130);
    shape4.LineTo(150, 130);
    shape4.LineTo(150, 150);
    shape4.ClosePath();
    shape4.MoveTo(125, 175);
    shape4.CurveTo(150, 145, 190, 155, 180, 140);
    pdf.Shape(shape4, wxPDF_STYLE_FILL | wxPDF_STYLE_DRAWCLOSE);

    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"), wxS(""), 10);

    static double pi = 4. * atan(1.0);

    pdf.Text(130, 40, wxS("Closed Bezier spline"));
    wxPdfArrayDouble xp, yp;
    int nseg = 10;
    double radius = 30;
    double step = 2 * pi / nseg;
    int i;
    for (i = 0; i < nseg; ++i)
    {
      double angle = i * step;
      xp.Add(20 + radius * (sin(angle) + 1));
      yp.Add(20 + radius * (cos(angle) + 1));
      pdf.Marker(xp[i], yp[i], wxPDF_MARKER_CIRCLE, 2.0);
    }
    pdf.ClosedBezierSpline(xp, yp, wxPDF_STYLE_DRAW);

    pdf.Text(130, 120, wxS("Bezier spline for Sine function"));
    // Sinus points in [0,2PI].
    // Fill point array with scaled in X,Y Sin values in [0, PI].
    wxPdfArrayDouble xpSin, ypSin;
    double scaleX = 20;
    double scaleY = 20;
    step = 2 * pi / nseg;
    for (i = 0; i < nseg; ++i)
    {
      double angle = i * step;
      xpSin.Add(20 + scaleX * angle);
      ypSin.Add(100 + scaleY * (1 - sin(angle)));
      pdf.Marker(xpSin[i], ypSin[i], wxPDF_MARKER_CIRCLE, 2.0);
    }
    pdf.BezierSpline(xpSin, ypSin, wxPDF_STYLE_DRAW);

    pdf.Text(130, 180, wxS("Bezier spline for Runge function"));
    wxPdfArrayDouble xpRunge, ypRunge;
    step = 2.0 / (nseg - 1);
    for (i = 0; i < nseg; ++i)
    {
      double xstep = -1 + i * step;
      xpRunge.Add(20 + scaleX * (xstep+1));
      ypRunge.Add(160 + scaleY * (1 - 1 / (1 + 25 * xstep * xstep)));
      pdf.Marker(xpRunge[i], ypRunge[i], wxPDF_MARKER_CIRCLE, 2.0);
    }
    pdf.BezierSpline(xpRunge, ypRunge, wxPDF_STYLE_DRAW);

    pdf.Text(130, 240, wxS("Bezier spline for arc from 0 to 270 degree"));
    wxPdfArrayDouble xpArc, ypArc;
    step = 270.0 / (nseg - 1);
    for (i = 0; i < nseg; ++i)
    {
      double angle = pi * i * step / 180;
      xpArc.Add(20 + scaleX * (cos(angle) + 1));
      ypArc.Add(220 + scaleY * (sin(angle) + 1));
      pdf.Marker(xpArc[i], ypArc[i], wxPDF_MARKER_CIRCLE, 2.0);
    }
    pdf.BezierSpline(xpArc, ypArc, wxPDF_STYLE_DRAW);

    pdf.SaveAsFile(wxS("drawing.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}

