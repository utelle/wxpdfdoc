///////////////////////////////////////////////////////////////////////////////
// Name:        bookmark.cpp
// Purpose:     Demonstration of bookmarks in wxPdfDocument
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
* Bookmarks
*
* This example demonstrates the use of bookmarks.
*/

void
bookmark()
{
  wxPdfDocument pdf;
  pdf.Open();
  pdf.SetFont(wxT("Helvetica"),wxT(""),15);
  // Page 1
  pdf.AddPage();
  pdf.Bookmark(wxT("Page 1"));
  pdf.Bookmark(wxT("Paragraph 1"),1,-1);
  pdf.Cell(0,6,wxT("Paragraph 1"));
  pdf.Ln(50);
  pdf.Bookmark(wxT("Paragraph 2"),1,-1);
  pdf.Cell(0,6,wxT("Paragraph 2"));
  pdf.Annotate(60,30,wxT("First annotation on first page"));
  pdf.Annotate(60,60,wxT("Second annotation on first page"));
  // Page 2
  pdf.AddPage();
  pdf.Bookmark(wxT("Page 2"));
  pdf.Bookmark(wxT("Paragraph 3"),1,-1);
  pdf.Cell(0,6,wxT("Paragraph 3"));
  pdf.Annotate(60,40,wxT("First annotation on second page"));
  pdf.Annotate(90,40,wxT("Second annotation on second page"));
  pdf.SaveAsFile(wxT("bookmark.pdf"));
}
