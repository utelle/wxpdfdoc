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
  pdf.SetFont(_T("Arial"),_T(""),15);
  //Page 1
  pdf.AddPage();
  pdf.Bookmark(_T("Page 1"));
  pdf.Bookmark(_T("Paragraph 1"),1,-1);
  pdf.Cell(0,6,_T("Paragraph 1"));
  pdf.Ln(50);
  pdf.Bookmark(_T("Paragraph 2"),1,-1);
  pdf.Cell(0,6,_T("Paragraph 2"));
  pdf.Annotate(60,30,_T("First annotation on first page"));
  pdf.Annotate(60,60,_T("Second annotation on first page"));
  //Page 2
  pdf.AddPage();
  pdf.Bookmark(_T("Page 2"));
  pdf.Bookmark(_T("Paragraph 3"),1,-1);
  pdf.Cell(0,6,_T("Paragraph 3"));
  pdf.Annotate(60,40,_T("First annotation on second page"));
  pdf.Annotate(90,40,_T("Second annotation on second page"));
  pdf.SaveAsFile(_T("bookmark.pdf"));
}

