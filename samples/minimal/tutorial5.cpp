///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial5.cpp
// Purpose:     Tutorial 5: Test program for wxPdfDocument
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
#include <wx/tokenzr.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "wx/pdfdoc.h"

/**
* Tables
*
* This tutorial shows how to make tables easily. 
* 
* A table being just a collection of cells, it is natural to build one from
* them. The first example is achieved in the most basic way possible: simple
* framed cells, all of the same size and left aligned. The result is rudimentary
* but very quick to obtain. 
* 
* The second table brings some improvements: each column has its own width,
* titles are centered and figures right aligned. Moreover, horizontal lines
* have been removed. This is done by means of the border parameter of the Cell()
* method, which specifies which sides of the cell must be drawn. Here we want the
* left (L) and right (R) ones. It remains the problem of the horizontal line to
* finish the table. There are two possibilities: either check for the last line in
* the loop, in which case we use LRB for the border parameter; or, as done here,
* add the line once the loop is over. 
* 
* The third table is similar to the second one but uses colors. Fill, text and
* line colors are simply specified. Alternate coloring for rows is obtained by
* using alternatively transparent and filled cells. 
*/

class PdfTuto5 : public wxPdfDocument
{
public:
  // Load data
  void LoadData(const wxString& file, wxArrayPtrVoid& data)
  {
    // Read file lines
    wxFileInputStream f(file);
    wxTextInputStream text( f );
    wxString line;
    while (!f.Eof())
    {
      text >> line;
      if (!f.Eof() && line.Length() > 0)
      {
        wxArrayString* row = new wxArrayString;
        data.Add(row);
        wxStringTokenizer tkz(line, wxS(";"));
        while ( tkz.HasMoreTokens() )
        {
          wxString token = tkz.GetNextToken();
          row->Add(token);
        }
      }
    }
  }

  // Simple table
  void BasicTable(wxArrayString& header,wxArrayPtrVoid& data)
  {
    size_t j;
    // Header
    for (j = 0; j < header.GetCount(); j++)
    {
      Cell(40,7,header[j],wxPDF_BORDER_FRAME);
    }
    Ln();
    // Data
    for (j = 0; j < data.GetCount(); j++)
    {
      wxArrayString* row = (wxArrayString*) data[j];
      size_t k;
      for (k = 0; k < (*row).GetCount(); k++)
      {
        Cell(40,6,(*row)[k],wxPDF_BORDER_FRAME);
      }
      Ln();
    }
  }

  // Better table
  void ImprovedTable(wxArrayString& header,wxArrayPtrVoid& data)
  {
    // Column widths
    double w[4] = {40,35,40,45};
    // Header
    size_t i;
    for (i = 0; i <header.GetCount(); i++)
    {
      Cell(w[i],7,header[i],wxPDF_BORDER_FRAME,0,wxPDF_ALIGN_CENTER);
    }
    Ln();
    // Data
    size_t j;
    for (j = 0; j < data.GetCount(); j++)
    {
      wxArrayString* row = (wxArrayString*) data[j];
      Cell(w[0],6,(*row)[0],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT);
      Cell(w[1],6,(*row)[1],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT);
      Cell(w[2],6,(*row)[2],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,0,wxPDF_ALIGN_RIGHT);
      Cell(w[3],6,(*row)[3],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,0,wxPDF_ALIGN_RIGHT);
      Ln();
    }
    // Closure line
    Cell((w[0]+w[1]+w[2]+w[3]),0,wxS(""),wxPDF_BORDER_TOP);
  }

  // Colored table
  void FancyTable(wxArrayString& header, wxArrayPtrVoid& data)
  {
    // Colors, line width and bold font
    SetFillColour(wxColour(255,0,0));
    SetTextColour(255);
    SetDrawColour(wxColour(128,0,0));
    SetLineWidth(.3);
    SetFont(wxS(""),wxS("B"));
    //Header
    double w[4] = {40,35,40,45};
    size_t i;
    for (i = 0; i < header.GetCount(); i++)
    {
      Cell(w[i],7,header[i],wxPDF_BORDER_FRAME, 0, wxPDF_ALIGN_CENTER, 1);
    }
    Ln();
    // Color and font restoration
    SetFillColour(wxColour(224,235,255));
    SetTextColour(0);
    SetFont(wxS(""));
    // Data
    int fill = 0;
    size_t j;
    for (j = 0; j < data.GetCount(); j++)
    {
      wxArrayString* row = (wxArrayString*) data[j];
      Cell(w[0],6,(*row)[0],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,0,wxPDF_ALIGN_LEFT,fill);
      Cell(w[1],6,(*row)[1],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,0,wxPDF_ALIGN_LEFT,fill);
      Cell(w[2],6,(*row)[2],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,0,wxPDF_ALIGN_RIGHT,fill);
      Cell(w[3],6,(*row)[3],wxPDF_BORDER_LEFT | wxPDF_BORDER_RIGHT,0,wxPDF_ALIGN_RIGHT,fill);
      Ln();
      fill = 1 - fill;
    }
    Cell((w[0]+w[1]+w[2]+w[3]),0,wxS(""),wxPDF_BORDER_TOP);
  }
};

int
tutorial5(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("countries.txt")))
  {
    PdfTuto5 pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    // Column titles
    wxArrayString header;
    wxArrayPtrVoid data;
    header.Add(wxS("Country"));
    header.Add(wxS("Capital"));
    header.Add(wxS("Area (sq km)"));
    header.Add(wxS("Pop. (thousands)"));
    // Data loading
    pdf.LoadData(wxS("countries.txt"),data);
    pdf.SetFont(wxS("Helvetica"),wxS(""),14);
    pdf.AddPage();
    pdf.BasicTable(header,data);
    pdf.AddPage();
    pdf.ImprovedTable(header,data);
    pdf.AddPage();
    pdf.FancyTable(header,data);
    pdf.SaveAsFile(wxS("tutorial5.pdf"));
    size_t j;
    for (j = 0; j < data.GetCount(); j++)
    {
      wxArrayString* row = (wxArrayString*) data[j];
      delete row;
    }
  }
  else
  {
    rc = 1;
  }
  return rc;
}

