///////////////////////////////////////////////////////////////////////////////
// Name:        rotation.cpp
// Purpose:     Demonstration of rotation in wxPdfDocument
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
* Rotations
*
* This example shows the effects of rotating text and an image.
*/

int
rotation(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("circle.png")))
  {
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"),wxS(""),20);
    pdf.RotatedImage(wxS("circle.png"),85,60,40,16,45);
    pdf.RotatedText(100,60,wxS("Hello!"),45);
    pdf.SaveAsFile(wxS("rotation.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}

