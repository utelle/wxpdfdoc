///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial4.cpp
// Purpose:     Tutorial 4: Test program for wxPdfDocument
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

#include <wx/wfstream.h>

#include "wx/pdfdoc.h"

/**
* Multi-columns
* 
* This example is a variant of the previous one showing how to lay the text
* across multiple columns. 
* 
* The key method used is AcceptPageBreak(). It allows to accept or not an
* automatic page break. By refusing it and altering the margin and current
* position, the desired column layout is achieved. 
* For the rest, not much change; two properties have been added to the class
* to save the current column number and the position where columns begin, and
* the MultiCell() call specifies a 6 centimeter width. 
*/

class PdfTuto4 : public wxPdfDocument
{
public:
  PdfTuto4() { m_col = 0; }
  void SetMyTitle(const wxString& title)
  {
    m_myTitle = title;
    SetTitle(title);
  }

  void Header()
  {
    // Page header
    SetFont(wxT("Arial"),wxT("B"),15);
    double w = GetStringWidth(m_myTitle)+6;
    SetX((210-w)/2);
    SetDrawColour(wxColour(0,80,180));
    SetFillColour(wxColour(230,230,0));
    SetTextColour(wxColour(220,50,50));
    SetLineWidth(1);
    Cell(w,9,m_myTitle,wxPDF_BORDER_FRAME,1,wxPDF_ALIGN_CENTER,1);
    Ln(10);
    // Save ordinate
    m_y0 = GetY();
  }

  void Footer()
  {
    // Page footer
    SetY(-15);
    SetFont(wxT("Arial"),wxT("I"),8);
    SetTextColour(128);
    Cell(0,10,wxString::Format(wxT("Page %d"),PageNo()),0,0,wxPDF_ALIGN_CENTER);
  }

  void SetCol(int col)
  {
    // Set position at a given column
    m_col = col;
    double x = 10 + col * 65;
    SetLeftMargin(x);
    SetX(x);
  }

  bool AcceptPageBreak()
  {
    // Method accepting or not automatic page break
    if (m_col < 2)
    {
        // Go to next column
        SetCol(m_col+1);
        // Set ordinate to top
        SetY(m_y0);
        // Keep on page
        return false;
    }
    else
    {
        // Go back to first column
        SetCol(0);
        // Page break
        return true;
    }
  }

  void ChapterTitle(int num, const wxString& label)
  {
    // Title
    SetFont(wxT("Arial"),wxT(""),12);
    SetFillColour(wxColour(200,220,255));
    Cell(0,6,wxString::Format(wxT("Chapter  %d : "),num)+label,0,1,wxPDF_ALIGN_LEFT,1);
    Ln(4);
    // Save ordinate
    m_y0 = GetY();
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
    // Font
    SetFont(wxT("Times"),wxT(""),12);
    // Output text in a 6 cm width column
    MultiCell(60,5,txt);
    Ln();
    // Mention
    SetFont(wxT(""),wxT("I"));
    Cell(0,5,wxT("(end of excerpt)"));
    // Go back to first column
    SetCol(0);
    delete ctxt;
  }

  void PrintChapter(int num, const wxString& title, const wxString& file)
  {
    // Add chapter
    AddPage();
    ChapterTitle(num,title);
    ChapterBody(file);
  }

private:
  // Current column
  int m_col;
  // Ordinate of column start
  double m_y0;

  wxString m_myTitle;
};

void
tutorial4()
{
  PdfTuto4 pdf;
  pdf.SetMyTitle(wxT("20000 Leagues Under the Seas"));
  pdf.SetAuthor(wxT("Jules Verne"));
  pdf.PrintChapter(1,wxT("A RUNAWAY REEF"),wxT("20k_c1.txt"));
  pdf.PrintChapter(2,wxT("THE PROS AND CONS"),wxT("20k_c2.txt"));
  pdf.SaveAsFile(wxT("tutorial4.pdf"));
}

