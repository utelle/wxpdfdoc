///////////////////////////////////////////////////////////////////////////////
// Name:        jsform.cpp
// Purpose:     Demonstration of JavaScript and interactive forms in wxPdfDocument
// Author:      Ulrich Telle
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
    wxString param = (showDialog) ? wxS("true") : wxS("false");
    AppendJavascript(wxString(wxS("print(")) + param + wxString(wxS(");")));
  }
};

int
javascript(bool testMode)
{
  PdfAutoPrint pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"), wxS(""), 20);
  pdf.Text(90, 50, wxS("Print me!"));
  // Launch the print dialog
  pdf.AutoPrint(true);
  pdf.SaveAsFile(wxS("javascript.pdf"));

  return 0;
}

/**
* Interactive forms
*
* This example demonstrates how create interactive forms in your PDF document.
*/

int
form(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  //  pdf.SetProtection(wxPDF_PERMISSION_PRINT | wxPDF_PERMISSION_MODIFY | wxPDF_PERMISSION_ANNOT);
  pdf.SetFormColours(wxPdfColour(255,0,0), wxPdfColour(250,235,186), wxPdfColour(0,0,255));
  pdf.AddPage();

  // Title
  pdf.SetFont(wxS("Helvetica"), wxS("U"), 16);
  pdf.Cell(0, 5, wxS("Subscription form"), 0, 1, wxPDF_ALIGN_CENTER);
  pdf.Ln(10);
  pdf.SetFont(wxS(""), wxS(""), 12);

  // First name
  pdf.Cell(35, 5, wxS("First name:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_UNDERLINE);
  pdf.TextField(wxS("firstname"), pdf.GetX(), pdf.GetY(), 50, 5, wxS(""));
  pdf.Ln(6);

  // Last name
  pdf.Cell(35, 5, wxS("Last name:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_UNDERLINE);
  pdf.TextField(wxS("lastname"), pdf.GetX(), pdf.GetY(), 50, 5, wxS(""));
  pdf.Ln(6);

  // Title
  pdf.Cell(35, 5, wxS("Title:"));
  wxArrayString options;
  options.Add(wxS(""));
  options.Add(wxS("Dr."));
  options.Add(wxS("Prof."));
  pdf.SetFormBorderStyle();
  pdf.ComboBox(wxS("titlecombo"), pdf.GetX(), pdf.GetY(), 20,5, options);
  pdf.Ln(8);

  // Gender
  pdf.Cell(35, 5, wxS("Gender:"), 0, 0);
  double x = pdf.GetX();
  double y = pdf.GetY();
  pdf.RadioButton(wxS("gender"), wxS("male"), x, y, 4);
  pdf.RadioButton(wxS("gender"), wxS("female"), x+25, y, 4);
  pdf.SetXY(x+6, y);
  pdf.Cell(20, 5, wxS("male"), 0, 0);
  pdf.SetXY(x+31, y);
  pdf.Cell(20, 5, wxS("female"), 0, 0);
  pdf.Ln(8);

  // Adress
  pdf.Cell(35, 5, wxS("Address:"));
  pdf.SetFormBorderStyle();
  pdf.TextField(wxS("address"), pdf.GetX(), pdf.GetY(), 60, 18, wxS(""), true);
  pdf.Ln(19);

  // E-mail
  pdf.Cell(35, 5, wxS("E-mail:"));
  pdf.SetFormBorderStyle();
  pdf.TextField(wxS("email"), pdf.GetX(), pdf.GetY(), 50, 5, wxS(""));
  pdf.Ln(6);

  // Newsletter
  pdf.Cell(35, 5, wxS("Receive our"), 0, 1);
  pdf.Cell(35, 5, wxS("newsletter:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_DASHED);
  pdf.CheckBox(wxS("newsletter"), pdf.GetX(), pdf.GetY(), 5, false);
  pdf.Ln(10);

  // Date of the day
  pdf.Cell(35, 5, wxS("Date: "));
  pdf.SetFormBorderStyle();
  pdf.TextField(wxS("date"), pdf.GetX(), pdf.GetY(), 30, 5, wxDateTime::Today().FormatISODate());
  pdf.Ln(5);
  pdf.Cell(35, 5,wxS("Signature:"));
  pdf.Ln(12);

  // Button to print
  pdf.SetX(95);
  pdf.SetFormBorderStyle();
  pdf.PushButton(wxS("print"), pdf.GetX(), pdf.GetY(), 20,8, wxS("Print"), wxS("print(true);"));

  pdf.SaveAsFile(wxS("form.pdf"));

  return 0;
}

