///////////////////////////////////////////////////////////////////////////////
// Name:        gradients.cpp
// Purpose:     Demonstration of gradients in wxPdfDocument
// Author:      Ulrich Telle
// Created:     2005-09-07
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
#include "wx/pdfcoonspatchmesh.h"

/**
* Gradients
* 
* This example shows examples of linear and radial gradient shadings
*/

int
gradients(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.AddPage();

  // set colors for gradients (r,g,b) or (grey 0-255)
  wxPdfColour blue(0,0,255);
  wxPdfColour green(0,255,0);
  wxPdfColour red(255,0,0);
  wxPdfColour yellow(255,255,0);
  wxPdfColour magenta(255,0,255);
  wxPdfColour cyan(0,255,255);
  wxPdfColour white(255,255,255);
  wxPdfColour black(0,0,0);
  wxPdfColour navy(wxString(wxS("navy")));
  wxPdfColour lightsteelblue(wxString(wxS("lightsteelblue")));
  wxPdfColour light(wxString(wxS("#EEEEEE")));
  wxPdfColour fcol(wxString(wxS("#440000")));
  wxPdfColour tcol(wxString(wxS("#FF9090")));

  // paint a linear gradient
  int grad1 = pdf.LinearGradient(red,blue);
  pdf.SetFillGradient(10,10,90,90,grad1);

  // paint a radial gradient
  int grad2 = pdf.RadialGradient(white,black,0.5,0.5,0,1,1,1.2);
  pdf.SetFillGradient(110,10,90,90,grad2);

  int grad3 = pdf.LinearGradient(navy, lightsteelblue, wxPDF_LINEAR_GRADIENT_MIDHORIZONTAL);
  pdf.SetFillGradient(10,200,10,20,grad3);

  int grad4 = pdf.LinearGradient(navy, lightsteelblue, wxPDF_LINEAR_GRADIENT_MIDVERTICAL);
  pdf.SetFillGradient(30,200,10,20,grad4);

  int grad5 = pdf.LinearGradient(navy, lightsteelblue, wxPDF_LINEAR_GRADIENT_HORIZONTAL);
  pdf.SetFillGradient(50,200,10,20,grad5);
  
  int grad6 = pdf.LinearGradient(navy, lightsteelblue, wxPDF_LINEAR_GRADIENT_VERTICAL);
  pdf.SetFillGradient(70,200,10,20,grad6);

  int grad7 = pdf.MidAxialGradient(navy, lightsteelblue, 0, 0, 1, 0, 0.5, 0.75);
  pdf.SetFillGradient(90,200,10,20,grad7);

  int grad8a = pdf.LinearGradient(fcol, tcol, wxPDF_LINEAR_GRADIENT_REFLECTION_LEFT);
  pdf.SetFillGradient(110,200,10,20,grad8a);

  int grad8b = pdf.LinearGradient(fcol, tcol, wxPDF_LINEAR_GRADIENT_REFLECTION_RIGHT);
  pdf.SetFillGradient(130,200,10,20,grad8b);

  int grad8c = pdf.LinearGradient(fcol, tcol, wxPDF_LINEAR_GRADIENT_REFLECTION_TOP);
  pdf.SetFillGradient(150,200,10,20,grad8c);

  int grad8d = pdf.LinearGradient(fcol, tcol, wxPDF_LINEAR_GRADIENT_REFLECTION_BOTTOM);
  pdf.SetFillGradient(170,200,10,20,grad8d);

  //example of clipping polygon
  wxPdfArrayDouble x1, x2, y;
  x1.Add(30); x2.Add(120); y.Add(135);
  x1.Add(60); x2.Add(150); y.Add(155);
  x1.Add(40); x2.Add(130); y.Add(155);
  x1.Add(70); x2.Add(160); y.Add(160);
  x1.Add(30); x2.Add(120); y.Add(165);

  pdf.ClippingPolygon(x1,y,false);
  pdf.SetFillGradient(20,135,50,30,grad1);
  pdf.UnsetClipping();
  pdf.ClippingPolygon(x2,y,false);
  pdf.SetFillGradient(110,125,60,50,grad2);
  pdf.UnsetClipping();

  // coons patches
  pdf.AddPage();

  // paint a coons patch mesh with default coordinates
  wxPdfCoonsPatchMesh mesh1;
  wxPdfColour colors1[] = { yellow, blue, green, red };
  double x1a[] = { 0.00, 0.33, 0.67, 1.00, 1.00, 1.00, 1.00, 0.67, 0.33, 0.00, 0.00, 0.00 };
  double y1a[] = { 0.00, 0.00, 0.00, 0.00, 0.33, 0.67, 1.00, 1.00, 1.00, 1.00, 0.67, 0.33 };
  mesh1.AddPatch(0, colors1, x1a, y1a);

  int coons1 = pdf.CoonsPatchGradient(mesh1);
  pdf.SetFillGradient(20,115,80,80,coons1);

  // set the coordinates for the cubic Bézier points x1,y1 ... x12, y12 of the patch
  // (see coons_patch_mesh_coords.jpg)
  wxPdfCoonsPatchMesh mesh2;
  wxPdfColour colors2[] = { yellow, blue, green, red };
  double x2a[] = { 0.00, 0.33, 0.67, 1.00, 0.80, 0.80, 1.00, 0.67, 0.33, 0.00, 0.20, 0.00 };
  double y2a[] = { 0.00, 0.20, 0.00, 0.00, 0.33, 0.67, 1.00, 0.80, 1.00, 1.00, 0.67, 0.33 };
  mesh2.AddPatch(0, colors2, x2a, y2a);

  double minCoord2 = 0; // minimum value of the coordinates
  double maxCoord2 = 1; // maximum value of the coordinates
  int coons2 = pdf.CoonsPatchGradient(mesh2, minCoord2, maxCoord2);
  pdf.SetFillGradient(110,115,80,80,coons2);

  // second page
  pdf.AddPage();
  //pdf.Cell(0,5,wxS("Page 2"),0,1,'C');
  pdf.Ln();

  wxPdfCoonsPatchMesh mesh3;
  double minCoord3 = 0; // minimum value of the coordinates
  double maxCoord3 = 2; // maximum value of the coordinates

  // first patch: f = 0
  wxPdfColour colors3a[] = { yellow, blue, green, red };
  double x3a[] = { 0.00, 0.33, 0.67, 1.00, 1.00, 0.80, 1.00, 0.67, 0.33, 0.00, 0.00, 0.00 };
  double y3a[] = { 0.00, 0.00, 0.00, 0.00, 0.33, 0.67, 1.00, 0.80, 1.80, 1.00, 0.67, 0.33 };
  mesh3.AddPatch(0, colors3a, x3a, y3a);

  // second patch - above the other: f = 2
  wxPdfColour colors3b[] = { black, magenta };
  double x3b[] = { 0.00, 0.00, 0.00, 0.33, 0.67, 1.00, 1.00, 1.50 };
  double y3b[] = { 1.33, 1.67, 2.00, 2.00, 2.00, 2.00, 1.67, 1.33 };
  mesh3.AddPatch(2, colors3b, x3b, y3b);

  // third patch - right of the above: f = 3
  wxPdfColour colors3c[] = { cyan, black };
  double x3c[] = { 1.33, 1.67, 2.00, 2.00, 2.00, 2.00, 1.67, 1.33 };
  double y3c[] = { 0.80, 1.50, 1.00, 1.33, 1.67, 2.00, 2.00, 2.00 };
  mesh3.AddPatch(3, colors3c, x3c, y3c);

  // fourth patch - below the above, which means left(?) of the above: f = 1
  wxPdfColour colors3d[] = { black, blue };
  double x3d[] = { 2.00, 2.00, 2.00, 1.67, 1.33, 1.00, 1.00, 0.80 };
  double y3d[] = { 0.67, 0.33, 0.00, 0.00, 0.00, 0.00, 0.33, 0.67 };
  mesh3.AddPatch(1, colors3d, x3d, y3d);

  int coons3 = pdf.CoonsPatchGradient(mesh3, minCoord3, maxCoord3);
  pdf.SetFillGradient(10,25,190,200,coons3);

  pdf.SaveAsFile(wxS("gradients.pdf"));

  return 0;
}

