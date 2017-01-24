///////////////////////////////////////////////////////////////////////////////
// Name:        wmf.cpp
// Purpose:     WMF: Test program for the wxPdfDocument class
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
* WMF images
* 
* This example shows the embedding of a WMF image file.
*/

int
wmf(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("ringmaster.wmf")))
  {
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();
    pdf.Image(wxS("ringmaster.wmf"), 50, 10, 110, 0, wxS("wmf"));
    pdf.SaveAsFile(wxS("wmf.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}

