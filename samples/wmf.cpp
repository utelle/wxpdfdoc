///////////////////////////////////////////////////////////////////////////////
// Name:        wmf.cpp
// Purpose:     WMF: Test program for the wxPdfDocument class
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
* WMF images
* 
* This example shows the embedding of a WMF image file.
*/

void wmf()
{
  wxPdfDocument pdf;
  pdf.AddPage();
  pdf.Image(wxT("ringmaster.wmf"), 50, 10, 110, 0, wxT("wmf"));
  pdf.SaveAsFile(wxT("wmf.pdf"));
}

