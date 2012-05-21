///////////////////////////////////////////////////////////////////////////////
// Name:        attachment.cpp
// Purpose:     Demonstration of file attachments in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2012-05-20
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
* Attachments
* 
* This example shows how to attach a file to a PDF document.
*/

void
attachment()
{
  wxPdfDocument pdf;
  pdf.AttachFile(wxT("attached.txt"), wxT(""), wxT("A simple text file"));
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"),wxT(""),14);
  pdf.Write(5,wxT("This PDF contains an attached file."));
  pdf.SaveAsFile(wxT("attachment.pdf"));
}
