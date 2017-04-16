///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial3.cpp
// Purpose:     Tutorial 3: Test program for wxPdfDocument
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

#include <wx/wfstream.h>

#include "wx/pdfdoc.h"

/**
* Line breaks and colors
*
* Let's continue with an example which prints justified paragraphs.
* It also illustrates the use of colors. 
* 
* The GetStringWidth() method allows to determine the length of a string in the
* current font, which is used here to calculate the position and the width of
* the frame surrounding the title. Then colors are set (via SetDrawColor(),
* SetFillColor() and SetTextColor()) and the thickness of the line is set to
* 1 mm (against 0.2 by default) with SetLineWidth(). Finally, we output the cell
* (the last parameter to 1 indicates that the background must be filled). 
* 
* The method used to print the paragraphs is MultiCell(). Each time a line reaches
* the right extremity of the cell or a carriage-return character is met, a line
* break is issued and a new cell automatically created under the current one.
* Text is justified by default. 
* 
* Two document properties are defined: title (SetTitle()) and author (SetAuthor()).
* Properties can be viewed by two means. First is open the document directly with
* Acrobat Reader, go to the File menu, Document info, General. Second, also available
* from the plug-in, is click on the triangle just above the right scrollbar and
* choose Document info. 
*/

class PdfTuto3 : public wxPdfDocument
{
public:
  void SetMyTitle(const wxString& title)
  {
    m_myTitle = title;
    SetTitle(title);
  }

  void Header()
  {
    // Helvetica bold 15
    SetFont(wxS("Helvetica"),wxS("B"),15);
    // Calculate width of title and position
    double w = GetStringWidth(m_myTitle)+6;
    SetX((210-w)/2);
    // Colors of frame, background and text
    SetDrawColour(wxColour(0,80,180));
    SetFillColour(wxColour(230,230,0));
    SetTextColour(wxColour(220,50,50));
    // Thickness of frame (1 mm)
    SetLineWidth(1);
    // Title
    Cell(w,9,m_myTitle,wxPDF_BORDER_FRAME,1,wxPDF_ALIGN_CENTER,1);
    // Line break
    Ln(10);
  }

  void Footer()
  {
    // Position at 1.5 cm from bottom
    SetY(-15);
    // Helvetica italic 8
    SetFont(wxS("Helvetica"),wxS("I"),8);
    // Text color in gray
    SetTextColour(128);
    // Page number
    Cell(0,10,wxString::Format(wxS("Page %d"),PageNo()),0,0,wxPDF_ALIGN_CENTER);
  }

  void ChapterTitle(int num, const wxString& label)
  {
    // Helvetica 12
    SetFont(wxS("Helvetica"),wxS(""),12);
    // Background color
    SetFillColour(wxColour(200,220,255));
    // Title
    Cell(0,6,wxString::Format(wxS("Chapter %d : "),num) + label,0,1,wxPDF_ALIGN_LEFT,1);
    // Line break
    Ln(4);
  }

  void ChapterBody(const wxString& file)
  {
    // Read text file
    wxFileInputStream f(file);
    int len = f.GetLength();
    char* ctxt = new char[len+1];
    f.Read(ctxt,len);
    ctxt[len] = '\0';
    wxString txt(ctxt,*wxConvCurrent);
    // Times 12
    SetFont(wxS("Times"),wxS(""),12);
    // Output justified text
    MultiCell(0,5,txt);
    // Line break
    Ln();
    // Mention in italics
    SetFont(wxS(""),wxS("I"));
    Cell(0,5,wxS("(end of excerpt)"));
    delete [] ctxt;
  }

  void PrintChapter(int num, const wxString& title, const wxString& file)
  {
    AddPage();
    ChapterTitle(num,title);
    ChapterBody(file);
  }

  wxString m_myTitle;
};

int
tutorial3(bool testMode)
{
  PdfTuto3 pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.SetMyTitle(wxS("20000 Leagues Under the Seas"));
  pdf.SetAuthor(wxS("Jules Verne"));
  pdf.PrintChapter(1,wxS("A RUNAWAY REEF"),wxS("20k_c1.txt"));
  pdf.PrintChapter(2,wxS("THE PROS AND CONS"),wxS("20k_c2.txt"));
  pdf.SaveAsFile(wxS("tutorial3.pdf"));
  return 0;
} 

