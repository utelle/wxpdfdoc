///////////////////////////////////////////////////////////////////////////////
// Name:        bookmark.cpp
// Purpose:     Demonstration of bookmarks in wxPdfDocument
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
* Bookmarks
*
* This example demonstrates the use of bookmarks.
*/

int
bookmark(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();
  pdf.SetFont(wxS("Helvetica"),wxS(""),15);
  // Page 1
  pdf.AddPage();
  pdf.Bookmark(wxS("Page 1"));
  pdf.Bookmark(wxS("Paragraph 1"),1,-1);
  pdf.Cell(0,6,wxS("Paragraph 1"));
  pdf.Ln(50);
  pdf.Bookmark(wxS("Paragraph 2"),1,-1);
  pdf.Cell(0,6,wxS("Paragraph 2"));
  pdf.Annotate(60,30,wxS("First annotation on first page"));
  pdf.Annotate(60,60,wxS("Second annotation on first page"));
  // Page 2
  pdf.AddPage();
  pdf.Bookmark(wxS("Page 2"));
  pdf.Bookmark(wxS("Paragraph 3"),1,-1);
  pdf.Cell(0,6,wxS("Paragraph 3"));
  pdf.Annotate(60,40,wxS("First annotation on second page"));
  pdf.Annotate(90,40,wxS("Second annotation on second page"));
  pdf.SaveAsFile(wxS("bookmark.pdf"));
  return 0;
}
