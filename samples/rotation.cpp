///////////////////////////////////////////////////////////////////////////////
// Name:        rotation.cpp
// Purpose:     Demonstration of rotation in wxPdfDocument
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
* Rotations
* 
* This example shows the effects of rotating text and an image.
*/

void
rotation()
{
  wxPdfDocument pdf;
  pdf.AddPage();
  pdf.SetFont(_T("Arial"),_T(""),20);
  pdf.RotatedImage(_T("circle.png"),85,60,40,16,45);
  pdf.RotatedText(100,60,_T("Hello!"),45);
  pdf.SaveAsFile(_T("rotation.pdf"));
}

