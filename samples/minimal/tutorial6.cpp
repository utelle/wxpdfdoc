///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial6.cpp
// Purpose:     Tutorial 6: Test program for wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-02-12
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
* Links and flowing text
*
* This tutorial explains how to insert links (internal and external) and shows
* a new text writing mode. It also contains a rudimentary HTML parser.
* The new method to print text is Write(). It is very close to MultiCell();
* the differences are: The end of line is at the right margin and the next line
* begins at the left one. The current position moves at the end of the text. 
* So it allows to write a chunk of text, alter the font style, then continue from
* the exact place we left it. On the other hand, you cannot full justify it. 
*
* The method is used on the first page to put a link pointing to the second one.
* The beginning of the sentence is written in regular style, then we switch to
* underline and finish it. The link is created with AddLink(), which returns a
* link identifier. The identifier is passed as third parameter of Write(). Once
* the second page is created, we use SetLink() to make the link point to the
* beginning of the current page. 
*
* Then we put an image with a link on it. An external link points to an URL
* (HTTP, mailto...). The URL is simply passed as last parameter of Image().
*
* Finally, the left margin is moved after the image with SetLeftMargin() and some
* text in XML format is output. 
* Recognized tags are <B>, <I>, <U>, <A> and <BR>; the others are ignored.
*
*/

void
tutorial6()
{
  wxString xmlString =
    wxString(wxT("You can now easily print text mixing different styles : <b>bold</b>, <i>italic</i>, ")) +
    wxString(wxT("<u>underlined</u>, or <b><i><u>all at once</u></i></b>!<br/>You can also insert links ")) +
    wxString(wxT("on text, such as <a href=\"http://www.fpdf.org\">www.fpdf.org</a>, or on an image: click on the logo."));

  wxPdfDocument pdf;
  // First page
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT(""), 20.0);
  pdf.StartTransform();
  pdf.Write(5, wxT("To find out what's new in this tutorial, click "));
  pdf.SetFont(wxT(""), wxT("U"));
  int link = pdf.AddLink();
  pdf.Write(5, wxT("here"), wxPdfLink(link));
  pdf.SetFont(wxT(""));
  pdf.StopTransform();
  // Second page
  pdf.AddPage();
  pdf.SetLink(link);
  pdf.Image(wxT("logo.png"), 10, 10, 30, 0, wxT(""),wxPdfLink(wxT("http://www.fpdf.org")));
  pdf.SetLeftMargin(45);
  pdf.SetFontSize(14);
  pdf.WriteXml(xmlString);

  pdf.SaveAsFile(wxT("tutorial6.pdf"));
}

