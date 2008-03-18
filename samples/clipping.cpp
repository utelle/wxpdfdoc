///////////////////////////////////////////////////////////////////////////////
// Name:        clipping.cpp
// Purpose:     Demonstration of clipping in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
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

#include "wx/pdfdoc.h"

/**
* Clipping
*
* This example shows several clipping options provided by wxPdfDocument.
* A clipping area restricts the display and prevents any elements from showing outside
* of it. 3 shapes are available: text, rectangle and ellipse. For each one, you can
* choose whether to draw the outline or not.
*/

void
clipping()
{
  wxPdfDocument pdf;
  pdf.AddPage();

  // example of clipped cell
  pdf.SetFont(_T("Arial"),_T(""),14);
  pdf.SetX(72);
  pdf.ClippedCell(60,6,_T("These are clipping examples"),wxPDF_BORDER_FRAME);

  // example of clipping text
  pdf.SetFont(_T("Arial"),_T("B"),120);
  //set the outline color
  pdf.SetDrawColor(0);
  // set the outline width (note that only its outer half will be shown)
  pdf.SetLineWidth(2);
  // draw the clipping text
  pdf.ClippingText(40,55,_T("CLIPS"),true);
  //fill it with the image
  pdf.Image(_T("clips.jpg"),40,10,130);
  // remove the clipping
  pdf.UnsetClipping();

  // example of clipping rectangle
  pdf.ClippingRect(45,65,116,20,true);
  pdf.Image(_T("clips.jpg"),40,10,130);
  pdf.UnsetClipping();

  // example of clipping ellipse
  pdf.ClippingEllipse(102,104,16,10,true);
  pdf.Image(_T("clips.jpg"),40,10,130);
  pdf.UnsetClipping();

  // example of clipping polygon
  wxPdfArrayDouble x, y;
  x.Add( 30); y.Add(135);
  x.Add( 60); y.Add(155);
  x.Add( 40); y.Add(155);
  x.Add( 70); y.Add(160);
  x.Add( 30); y.Add(165);

  pdf.ClippingPolygon(x,y,true);
  pdf.Image(_T("clips.jpg"),20,100,130);
  pdf.UnsetClipping();

  // example of clipping using a shape
  wxPdfShape shape;
  shape.MoveTo(135,140);
  shape.CurveTo(135,137,130,125,110,125);
  shape.CurveTo(80,125,80,162.5,80,162.5);
  shape.CurveTo(80,180,100,202,135,220);
  shape.CurveTo(170,202,190,180,190,162.5);
  shape.CurveTo(190,162.5,190,125,160,125);
  shape.CurveTo(145,125,135,137,135,140);

  pdf.SetLineWidth(1);
  pdf.SetFillColor(wxPdfColour(wxString(_T("red"))));
  pdf.ClippingPath(shape, wxPDF_STYLE_FILLDRAW);
  pdf.UnsetClipping();

  pdf.SaveAsFile(_T("clipping.pdf"));
}

