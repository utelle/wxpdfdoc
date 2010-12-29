///////////////////////////////////////////////////////////////////////////////
// Name:        jsform.cpp
// Purpose:     Demonstration of JavaScript and interactive forms in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-01-21
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
* JavaScript
*
* This example demonstrates how you could start the print dialog on opening
* your PDF document using a bit of JavaScript.
*/

class PdfAutoPrint : public wxPdfDocument
{
public:
  void AutoPrint(bool showDialog = false)
  {
    // Embed some JavaScript to show the print dialog or start printing immediately
    wxString param = (showDialog) ? wxT("true") : wxT("false");
    AppendJavascript(wxString(wxT("print(")) + param + wxString(wxT(");")));
  }
};

void javascript()
{
  PdfAutoPrint pdf;
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT(""), 20);
  pdf.Text(90, 50, wxT("Print me!"));
  // Launch the print dialog
  pdf.AutoPrint(true);
  pdf.SaveAsFile(wxT("javascript.pdf"));
}

/**
* Interactive forms
*
* This example demonstrates how create interactive forms in your PDF document.
*/

void form()
{
  wxPdfDocument pdf;
//  pdf.SetProtection(wxPDF_PERMISSION_PRINT | wxPDF_PERMISSION_MODIFY | wxPDF_PERMISSION_ANNOT);
  pdf.SetFormColours(wxPdfColour(255,0,0), wxPdfColour(250,235,186), wxPdfColour(0,0,255));
  pdf.AddPage();

  // Title
  pdf.SetFont(wxT("Helvetica"), wxT("U"), 16);
  pdf.Cell(0, 5, wxT("Subscription form"), 0, 1, wxPDF_ALIGN_CENTER);
  pdf.Ln(10);
  pdf.SetFont(wxT(""), wxT(""), 12);

  // First name
  pdf.Cell(35, 5, wxT("First name:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_UNDERLINE);
  pdf.TextField(wxT("firstname"), pdf.GetX(), pdf.GetY(), 50, 5, wxT(""));
  pdf.Ln(6);

  // Last name
  pdf.Cell(35, 5, wxT("Last name:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_UNDERLINE);
  pdf.TextField(wxT("lastname"), pdf.GetX(), pdf.GetY(), 50, 5, wxT(""));
  pdf.Ln(6);

  // Title
  pdf.Cell(35, 5, wxT("Title:"));
  wxArrayString options;
  options.Add(wxT(""));
  options.Add(wxT("Dr."));
  options.Add(wxT("Prof."));
  pdf.SetFormBorderStyle();
  pdf.ComboBox(wxT("titlecombo"), pdf.GetX(), pdf.GetY(), 20,5, options);
  pdf.Ln(8);

  // Gender
  pdf.Cell(35, 5, wxT("Gender:"), 0, 0);
  double x = pdf.GetX();
  double y = pdf.GetY();
  pdf.RadioButton(wxT("gender"), wxT("male"), x, y, 4);
  pdf.RadioButton(wxT("gender"), wxT("female"), x+25, y, 4);
  pdf.SetXY(x+6, y);
  pdf.Cell(20, 5, wxT("male"), 0, 0);
  pdf.SetXY(x+31, y);
  pdf.Cell(20, 5, wxT("female"), 0, 0);
  pdf.Ln(8);

  // Adress
  pdf.Cell(35, 5, wxT("Address:"));
  pdf.SetFormBorderStyle();
  pdf.TextField(wxT("address"), pdf.GetX(), pdf.GetY(), 60, 18, wxT(""), true);
  pdf.Ln(19);

  // E-mail
  pdf.Cell(35, 5, wxT("E-mail:"));
  pdf.SetFormBorderStyle();
  pdf.TextField(wxT("email"), pdf.GetX(), pdf.GetY(), 50, 5, wxT(""));
  pdf.Ln(6);

  // Newsletter
  pdf.Cell(35, 5, wxT("Receive our"), 0, 1);
  pdf.Cell(35, 5, wxT("newsletter:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_DASHED);
  pdf.CheckBox(wxT("newsletter"), pdf.GetX(), pdf.GetY(), 5, false);
  pdf.Ln(10);

  // Date of the day
  pdf.Cell(35, 5, wxT("Date: "));
  pdf.SetFormBorderStyle();
  pdf.TextField(wxT("date"), pdf.GetX(), pdf.GetY(), 30, 5, wxDateTime::Today().FormatISODate());
  pdf.Ln(5);
  pdf.Cell(35, 5,wxT("Signature:"));
  pdf.Ln(12);

  // Button to print
  pdf.SetX(95);
  pdf.SetFormBorderStyle();
  pdf.PushButton(wxT("print"), pdf.GetX(), pdf.GetY(), 20,8, wxT("Print"), wxT("print(true);"));

  pdf.SaveAsFile(wxT("form.pdf"));
}

