///////////////////////////////////////////////////////////////////////////////
// Name:        protection.cpp
// Purpose:     Demonstration of access permissions in wxPdfDocument
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

#include "wx/pdfdoc.h"

/**
* Protection1
*
* This example demonstrates how you could protect your PDF document
* against copying text via cut & paste.
*/

int
protection1(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.SetProtection(wxPDF_PERMISSION_PRINT);
  pdf.Open();
  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"));
  pdf.Cell(0,10,wxString(wxS("You can print me but not copy my text.")),0,1);
  pdf.SaveAsFile(wxS("protection1.pdf"));
  return 0;
}

/**
* Protection1
*
* This example demonstrates how you could protect your PDF document
* against unauthorized access by using passwords with 128-bit encryption key.
*/

int
protection2(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.SetProtection(wxPDF_PERMISSION_NONE, wxS("Hello"), wxS("World"), wxPDF_ENCRYPTION_RC4V2, 128);
  pdf.Open();
  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"));
  pdf.Cell(0,10,wxString(wxS("You can only view me on screen.")),0,1);
  pdf.SaveAsFile(wxS("protection2.pdf"));
  return 0;
}

