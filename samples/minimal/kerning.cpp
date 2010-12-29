///////////////////////////////////////////////////////////////////////////////
// Name:        kerning.cpp
// Purpose:     Demonstration of kerning in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2009-07-07
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
#include "wx/pdffontmanager.h"

/**
* Kerning
*
* This example demonstrates the use of kerning.
*/

void
kerning()
{
  wxPdfDocument pdf;
  pdf.Open();
  pdf.SetFont(wxT("Helvetica"),wxT(""),15);

  // Page 1
  pdf.AddPage();
  pdf.SetKerning(false);
  pdf.Cell(0,6,wxT("WATER AWAY (without kerning)"));
  pdf.Ln(6);
  pdf.SetKerning(true);
  pdf.Cell(0,6,wxT("WATER AWAY (with kerning)"));

  pdf.SaveAsFile(wxT("kerning.pdf"));
}
