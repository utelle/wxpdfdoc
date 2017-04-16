///////////////////////////////////////////////////////////////////////////////
// Name:        templates.cpp
// Purpose:     Demonstration of rotation in wxPdfDocument
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

#include <wx/filename.h>

#include "wx/pdfdoc.h"
#include "wx/pdfinfo.h"

/**
* Templates 1
* 
* This example shows the creation and use of internal templates.
*/

int
templates1(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("glasses.png")))
  {
    int i;
    double w, h;
    int tpl1, tpl2;
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();

    // Generate a template clip on  x=0, y=0, width=180, height=350
    // Take care, that the margins of the Template are set to the
    // original margins.
    tpl1 = pdf.BeginTemplate(0, 0, 180, 350);
    pdf.SetFont(wxS("Helvetica"), wxS(""), 14);
    pdf.SetTextColour(0);
    for(i = 0; i < 200; i++)
    {
      pdf.Write(10, wxString::Format(wxS("dummy text %d "), i));
    }
    pdf.Image(wxS("glasses.png"), 100, 60, 100);
    pdf.EndTemplate();

    // Generate a template that will hold the whole page
    tpl2 = pdf.BeginTemplate();
    pdf.SetFont(wxS("Helvetica"), wxS(""), 14);

    // demonstrate how to lay text in background of an existing template
    pdf.SetXY(115, 55);
    pdf.Write(10, wxS("write behind it..."));

    // Now we use our first created template on position x=10, y=10 and
    // give it a width of 50mm (height is calculated automaticaly) and draw a border around it
    pdf.UseTemplate(tpl1, 10, 10, 50);
    w = 50;
    h = 0;
    pdf.GetTemplateSize(tpl1, w, h);
    pdf.Rect(10, 10, w, h);

    // Same as above, but another size
    pdf.UseTemplate(tpl1, 70, 10, 100);
    w = 100;
    h = 0;
    pdf.GetTemplateSize(tpl1, w, h);
    pdf.Rect(70, 10, w, h);
    pdf.EndTemplate();

    // Till now, there is no output to the PDF-File
    // We draw Template No. 2, that includes 2 Versions of the first
    pdf.UseTemplate(tpl2);

    pdf.AddPage();

    // Here we reuse Template No. 2
    // For example I used the rotate-script
    // to show u how, easy it is to use the created templates
    pdf.SetFillColour(255);
    for (i = 90; i >= 0; i -= 30)
    {
      pdf.StartTransform();
      pdf.Rotate(i, 10, 120);
      w = 100;
      h = 0;
      pdf.GetTemplateSize(tpl2, w, h);
      pdf.Rect(10, 120, w, h, wxPDF_STYLE_FILLDRAW);
      pdf.UseTemplate(tpl2, 10, 120, 100);
      pdf.StopTransform();
    }

    pdf.SaveAsFile(wxS("templates1.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}

/**
* Templates 2
* 
* This example shows the use of external templates.
*/

int
templates2(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("chart2d.pdf")))
  {
    double x, y, w, h;
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();
    pdf.SetTextColour(wxPdfColour(wxString(wxS("black"))));
    int pages = pdf.SetSourceFile(wxS("chart2d.pdf"));

    // Get the document information from the imported PDF file
    wxPdfInfo info;
    pdf.GetSourceInfo(info);

    int tpl = pdf.ImportPage(1);

    // Add some extra white space around the template
    pdf.GetTemplateBBox(tpl, x, y, w, h);
    pdf.SetTemplateBBox(tpl, x-10, y-10, w+20, h+20);
    pdf.UseTemplate(tpl, 20, 20, 160);

    // Draw a rectangle around the template
    w = 160;
    h = 0;
    pdf.GetTemplateSize(tpl, w, h);
    pdf.Rect(20, 20, w, h);

    pdf.SetXY(30,30+h);
    pdf.SetFont(wxS("Helvetica"),wxS(""), 10);
    pdf.SetLeftMargin(30);
    pdf.Cell(0, 5, wxString(wxS("Title: "))+info.GetTitle());
    pdf.Ln(5);
    pdf.Cell(0, 5, wxString(wxS("Creator: "))+info.GetCreator());
    pdf.Ln(5);
    pdf.Cell(0, 5, wxString(wxS("Producer: "))+info.GetProducer());

    pdf.SaveAsFile(wxS("templates2.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}
