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
	  //Embed some JavaScript to show the print dialog or start printing immediately
    wxString param = (showDialog) ? _T("true") : _T("false");
    AppendJavascript(wxString(_T("print(")) + param + wxString(_T(");")));
  }
};

void javascript()
{
  PdfAutoPrint pdf;
  pdf.AddPage();
  pdf.SetFont(_T("Arial"), _T(""), 20);
  pdf.Text(90, 50, _T("Print me!"));
  // Launch the print dialog
  pdf.AutoPrint(true);
  pdf.SaveAsFile(_T("javascript.pdf"));
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
  pdf.SetFormColors(wxPdfColour(255,0,0), wxPdfColour(250,235,186), wxPdfColour(0,0,255));
  pdf.AddPage();

  // Title
  pdf.SetFont(_T("Arial"), _T("U"), 16);
  pdf.Cell(0, 5, _T("Subscription form"), 0, 1, wxPDF_ALIGN_CENTER);
  pdf.Ln(10);
  pdf.SetFont(_T(""), _T(""), 12);

  // First name
  pdf.Cell(35, 5, _T("First name:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_UNDERLINE);
  pdf.TextField(_T("firstname"), pdf.GetX(), pdf.GetY(), 50, 5, _T(""));
  pdf.Ln(6);

  // Last name
  pdf.Cell(35, 5, _T("Last name:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_UNDERLINE);
  pdf.TextField(_T("lastname"), pdf.GetX(), pdf.GetY(), 50, 5, _T(""));
  pdf.Ln(6);

  // Title
  pdf.Cell(35, 5, _T("Title:"));
  wxArrayString options;
  options.Add(_T(""));
  options.Add(_T("Dr."));
  options.Add(_T("Prof."));
  pdf.SetFormBorderStyle();
  pdf.ComboBox(_T("titlecombo"), pdf.GetX(), pdf.GetY(), 20,5, options);
  pdf.Ln(8);

  // Gender
  pdf.Cell(35, 5, _T("Gender:"), 0, 0);
  double x = pdf.GetX();
  double y = pdf.GetY();
  pdf.RadioButton(_T("gender"), _T("male"), x, y, 4);
  pdf.RadioButton(_T("gender"), _T("female"), x+25, y, 4);
  pdf.SetXY(x+6, y);
  pdf.Cell(20, 5, _T("male"), 0, 0);
  pdf.SetXY(x+31, y);
  pdf.Cell(20, 5, _T("female"), 0, 0);
  pdf.Ln(8);

  // Adress
  pdf.Cell(35, 5, _T("Address:"));
  pdf.SetFormBorderStyle();
  pdf.TextField(_T("address"), pdf.GetX(), pdf.GetY(), 60, 18, _T(""), true);
  pdf.Ln(19);

  // E-mail
  pdf.Cell(35, 5, _T("E-mail:"));
  pdf.SetFormBorderStyle();
  pdf.TextField(_T("email"), pdf.GetX(), pdf.GetY(), 50, 5, _T(""));
  pdf.Ln(6);

  // Newsletter
  pdf.Cell(35, 5, _T("Receive our"), 0, 1);
  pdf.Cell(35, 5, _T("newsletter:"));
  pdf.SetFormBorderStyle(wxPDF_BORDER_DASHED);
  pdf.CheckBox(_T("newsletter"), pdf.GetX(), pdf.GetY(), 5, false);
  pdf.Ln(10);

  // Date of the day
  pdf.Cell(35, 5, _T("Date: "));
  pdf.SetFormBorderStyle();
  pdf.TextField(_T("date"), pdf.GetX(), pdf.GetY(), 30, 5, wxDateTime::Today().FormatISODate());
  pdf.Ln(5);
  pdf.Cell(35, 5,_T("Signature:"));
  pdf.Ln(12);

  // Button to print
  pdf.SetX(95);
  pdf.SetFormBorderStyle();
  pdf.PushButton(_T("print"), pdf.GetX(), pdf.GetY(), 20,8, _T("Print"), _T("print(true);"));

  pdf.SaveAsFile(_T("form.pdf"));
}

