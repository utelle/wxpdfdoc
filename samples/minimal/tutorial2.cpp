///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial2.cpp
// Purpose:     Tutorial 2: Test program for wxPdfDocument
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
* Here is a two page example with header, footer and logo: 
*
* This example makes use of the Header() and Footer() methods to process page
* headers and footers. They are called automatically. They already exist in the
* wxPdfDocument class but do nothing, therefore we have to extend the class and override
* them. 
* 
* The logo is printed with the Image() method by specifying its upper-left corner
* and its width. The height is calculated automatically to respect the image
* proportions. 
* 
* To print the page number, a null value is passed as the cell width. It means
* that the cell should extend up to the right margin of the page; it is handy to
* center text. The current page number is returned by the PageNo() method; as for
* the total number of pages, it is obtained by means of the special value {nb}
* which will be substituted on document closure (provided you first called
* AliasNbPages()). 
* 
* Note the use of the SetY() method which allows to set position at an absolute
* location in the page, starting from the top or the bottom. 

* Another interesting feature is used here: the automatic page breaking. As soon
* as a cell would cross a limit in the page (at 2 centimeters from the bottom by
* default), a break is performed and the font restored. Although the header and
* footer select their own font (Helvetica), the body continues with Times. This mechanism
* of automatic restoration also applies to colors and line width. The limit which
* triggers page breaks can be set with SetAutoPageBreak(). 
*/

class PdfTuto2 : public wxPdfDocument
{
  // Page header
  void Header()
  {
    // Logo
    Image(wxT("wxpdfdoc.png"),10,8,28);
    // Helvetica bold 15
    SetFont(wxT("Helvetica"),wxT("B"),15);
    // Move to the right
    Cell(80);
    // Title
    Cell(30,10,wxT("Title"),wxPDF_BORDER_FRAME,0,wxPDF_ALIGN_CENTER);
    // Line break
    Ln(20);
  }

  // Page footer
  void Footer()
  {
    // Position at 1.5 cm from bottom
    SetY(-15);
    // Helvetica italic 8
    SetFont(wxT("Helvetica"),wxT("I"),8);
    // Page number
    Cell(0,10,wxString::Format(wxT("Page %d/{nb}"),PageNo()),0,0,wxPDF_ALIGN_CENTER);
  }
};

void
tutorial2()
{
  // Instantiation of inherited class
  PdfTuto2 pdf;
  pdf.AliasNbPages();
  pdf.AddPage();
  pdf.Image(wxT("smile.jpg"),70,40,12);
  pdf.Image(wxT("apple.gif"),110,40,25);
  pdf.SetFont(wxT("Times"),wxT(""),12);
  int i;
  for (i = 1; i <= 40; i++)
  {
    pdf.Cell(0,10,wxString::Format(wxT("Printing line number %d"),i),0,1);
  }
  pdf.SaveAsFile(wxT("tutorial2.pdf"));
}

