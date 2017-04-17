///////////////////////////////////////////////////////////////////////////////
// Name:        attachment.cpp
// Purpose:     Demonstration of file attachments in wxPdfDocument
// Author:      Ulrich Telle
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

#include <wx/filename.h>

#include "wx/pdfdoc.h"

/**
* Attachments
*
* This example shows how to attach a file to a PDF document.
*/

int
attachment(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("attached.txt")))
  {
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AttachFile(wxS("attached.txt"), wxS(""), wxS("A simple text file"));
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"),wxS(""),14);
    pdf.Write(5,wxS("This PDF contains an attached file."));
    pdf.SaveAsFile(wxS("attachment.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}
