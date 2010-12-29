///////////////////////////////////////////////////////////////////////////////
// Name:        transparency.cpp
// Purpose:     Demonstration of transparency in wxPdfDocument
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
* Transparency
* 
* This example shows transparency effects and image masking.
*/

void
transparency()
{
  wxPdfDocument pdf;
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"),wxT(""),16);

  int j;
  wxString txt;
  for (j = 0; j < 180; j++) txt += wxT("Hello World! ");
  pdf.MultiCell(0,8, txt);

  // A) provide image + separate 8-bit mask (best quality!)

  // first embed mask image (w, h, x and y will be ignored, the image will be scaled to the target image's size)
  int maskImg = pdf.ImageMask(wxT("mask.png"));

  // embed image, masked with previously embedded mask
  pdf.Image(wxT("image.png"), 55, 10, 100, 0, wxT("png"), wxPdfLink(-1), maskImg);

  // B) use alpha channel from PNG
  //pdf->ImagePngWithAlpha('image_with_alpha.png',55,100,100);

  // C) same as B), but using Image() method that recognizes the alpha channel
  pdf.Image(wxT("image_with_alpha.png"), 55, 190, 100);

  pdf.AddPage();
  pdf.SetLineWidth(1.5);

  // draw opaque red square
  pdf.SetAlpha();
  pdf.SetFillColour(wxPdfColour(255,0,0));
  pdf.Rect(10, 10, 40, 40, wxPDF_STYLE_FILLDRAW);

  // set alpha to semi-transparency
  pdf.SetAlpha(1, 0.5);

  // draw green square
  pdf.SetFillColour(wxPdfColour(0,255,0));
  pdf.Rect(20, 20, 40, 40, wxPDF_STYLE_FILLDRAW);

  // draw jpeg image
  pdf.Image(wxT("lena.jpg"), 30, 30, 40);

  // restore full opacity
  pdf.SetAlpha();

  // print name
  pdf.SetFont(wxT("Helvetica"), wxT(""), 12);
  pdf.Text(46,68,wxT("Lena"));

  pdf.SaveAsFile(wxT("transparency.pdf"));
}
