///////////////////////////////////////////////////////////////////////////////
// Name:        protection.cpp
// Purpose:     Demonstration of access permissions in wxPdfDocument
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
* Protection1
*
* This example demonstrates how you could protect your PDF document
* against copying text via cut & paste.
*/

void
protection1()
{
  wxPdfDocument pdf;
  pdf.SetProtection(wxPDF_PERMISSION_PRINT);
  pdf.Open();
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"));
  pdf.Cell(0,10,wxString(wxT("You can print me but not copy my text.")),0,1);
  pdf.SaveAsFile(wxT("protection1.pdf"));
}

/**
* Protection1
*
* This example demonstrates how you could protect your PDF document
* against unauthorized access by using passwords with 128-bit encryption key.
*/

void
protection2()
{
  wxPdfDocument pdf;
  pdf.SetProtection(wxPDF_PERMISSION_NONE, wxT("Hello"), wxT("World"), wxPDF_ENCRYPTION_RC4V2, 128);
  pdf.Open();
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"));
  pdf.Cell(0,10,wxString(wxT("You can only view me on screen.")),0,1);
  pdf.SaveAsFile(wxT("protection2.pdf"));
}

