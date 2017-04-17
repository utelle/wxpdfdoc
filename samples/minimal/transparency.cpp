///////////////////////////////////////////////////////////////////////////////
// Name:        transparency.cpp
// Purpose:     Demonstration of transparency in wxPdfDocument
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

/**
* Transparency
*
* This example shows transparency effects and image masking.
*/

int
transparency(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("mask.png")) &&
      wxFileName::IsFileReadable(wxS("image.png")) &&
      wxFileName::IsFileReadable(wxS("image_with_alpha.png")) &&
      wxFileName::IsFileReadable(wxS("flower.jpg")))
  {
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"),wxS(""),16);

    int j;
    wxString txt;
    for (j = 0; j < 180; j++) txt += wxS("Hello World! ");
    pdf.MultiCell(0,8, txt);

    // A) provide image + separate 8-bit mask (best quality!)

    // first embed mask image (w, h, x and y will be ignored, the image will be scaled to the target image's size)
    int maskImg = pdf.ImageMask(wxS("mask.png"));

    // embed image, masked with previously embedded mask
    pdf.Image(wxS("image.png"), 55, 10, 100, 0, wxS("png"), wxPdfLink(-1), maskImg);

    // B) use alpha channel from PNG
    //pdf->ImagePngWithAlpha('image_with_alpha.png',55,100,100);

    // C) same as B), but using Image() method that recognizes the alpha channel
    pdf.Image(wxS("image_with_alpha.png"), 55, 190, 100);

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
    pdf.Image(wxS("flower.jpg"), 30, 30, 40);

    // restore full opacity
    pdf.SetAlpha();

    // print name
    pdf.SetFont(wxS("Helvetica"), wxS(""), 12);
    pdf.Text(46,68,wxS("Flower"));

    pdf.SaveAsFile(wxS("transparency.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}
