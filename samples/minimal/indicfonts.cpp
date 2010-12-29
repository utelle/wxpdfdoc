///////////////////////////////////////////////////////////////////////////////
// Name:        indictest.cpp
// Purpose:     Demonstration of Indic fonts in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2010-12-15
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
* Indic fonts and languages
*
* This example demonstrates the use of Indic fonts and languages.
*
* Remark: Only available in Unicode build. 
*/

static void
writeFullMoon(wxPdfDocument& pdf, const wxString& txt1, const wxString& txt2, const wxString& fontName)
{
  pdf.AddFont(fontName);
  pdf.SetFont(fontName, wxT(""), 22);
  pdf.Cell(40, 12, txt1);
  pdf.Cell(100, 12, txt2);
  pdf.Ln(13);
}

static void
fullmoon()
{
  wxString hindi1("११११", wxConvUTF8);
  wxString hindi2("पूर्ण चाँद", wxConvUTF8);
  wxString bengali1("১১১১", wxConvUTF8);
  wxString bengali2("পূরো চাঁদ", wxConvUTF8);
  wxString punjabi1("੧੧੧੧", wxConvUTF8);
  wxString punjabi2("ਪੂਰਾ ਚੰਦ", wxConvUTF8);
  wxString gujarati1("૧૧૧૧", wxConvUTF8);
  wxString gujarati2("પૂર્ણ ચંદ્ર", wxConvUTF8);
  wxString oriya1("୧୧୧୧", wxConvUTF8);
  wxString oriya2("ପୂର୍ଣ୍ଣ ଚନ୍ଦ୍ର", wxConvUTF8);
  wxString tamil1("௧௧௧௧", wxConvUTF8);
  wxString tamil2("முழு நிலவு", wxConvUTF8);
  wxString telugu1("౧౧౧౧", wxConvUTF8);
  wxString telugu2("పూర్ణచంద్రుడు", wxConvUTF8);
  wxString kannada1("೧೧೧೧", wxConvUTF8);
  wxString kannada2("ಪೂರ್ಣ ಚನ್ದ್ರ", wxConvUTF8);
  wxString malayalam1("൧൧൧൧", wxConvUTF8);
  wxString malayalam2("പൂറ്ണ്ണചന്ദ്റന്", wxConvUTF8);

  wxPdfDocument pdf;
  pdf.Open();
  pdf.SetTopMargin(20);
  pdf.SetLeftMargin(20);
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT("B"),24);
  pdf.Write(10,wxT("Full Moon in 9 Indic Scripts"));
  pdf.Ln(15);
  writeFullMoon(pdf, hindi1, hindi2, wxT("RaghuHindi"));
  writeFullMoon(pdf, bengali1, bengali2, wxT("RaghuBengali"));
  writeFullMoon(pdf, punjabi1, punjabi2, wxT("RaghuPunjabi"));
  writeFullMoon(pdf, gujarati1, gujarati2, wxT("RaghuGujarati"));
  writeFullMoon(pdf, oriya1, oriya2, wxT("RaghuOriya"));
  writeFullMoon(pdf, tamil1, tamil2, wxT("RaghuTamil"));
  writeFullMoon(pdf, telugu1, telugu2, wxT("RaghuTelugu"));
  writeFullMoon(pdf, kannada1, kannada2, wxT("RaghuKannada"));
  writeFullMoon(pdf, malayalam1, malayalam2, wxT("RaghuMalayalam"));

  pdf.SaveAsFile(wxT("fullmoon.pdf"));
}

static void
writesample(wxPdfDocument& pdf, const wxString& header, const wxString& sampleFile, const wxString& fontName)
{
  pdf.SetFont(wxT("Helvetica"), wxT("B"), 16);
  pdf.Write(10,header);
  pdf.Ln(14);

  wxFileInputStream f(sampleFile);
  int len = f.GetLength();
  char* ctxt = new char[len+1];
  f.Read(ctxt,len);
  ctxt[len] = '\0';
  wxString sampleText(ctxt, wxConvUTF8);
  delete [] ctxt;

  pdf.AddFont(fontName);
  pdf.SetFont(fontName, wxT(""), 15);
  pdf.MultiCell(160, 7, sampleText, 0, wxPDF_ALIGN_LEFT);
  pdf.Ln();
}

static void
samples()
{
  wxPdfDocument pdf;
  pdf.Open();
  pdf.SetTopMargin(30);
  pdf.SetLeftMargin(30);
  pdf.AddPage();
  pdf.SetFont(wxT("Helvetica"), wxT("B"),32);
  pdf.Write(10,wxT("Indic Fonts and Languages"));
  pdf.Ln(17);
  writesample(pdf, wxT("Assamese (as)"), wxT("indic-assamese.txt"), wxT("RaghuBengali"));
  writesample(pdf, wxT("Bengali (bn)"), wxT("indic-bengali.txt"), wxT("RaghuBengali"));
  writesample(pdf, wxT("Gujarati (gu)"), wxT("indic-gujarati.txt"), wxT("RaghuGujarati"));
  writesample(pdf, wxT("Hindi (hi)"), wxT("indic-hindi.txt"), wxT("RaghuHindi"));
  writesample(pdf, wxT("Kannada (kn)"), wxT("indic-kannada.txt"), wxT("RaghuKannada"));
  writesample(pdf, wxT("Malayalam (ml)"), wxT("indic-malayalam.txt"), wxT("RaghuMalayalam"));
  writesample(pdf, wxT("Nepali (ne) - Devanagari"), wxT("indic-nepali.txt"), wxT("RaghuHindi"));
  writesample(pdf, wxT("Oriya (or)"), wxT("indic-oriya.txt"), wxT("RaghuOriya"));
  writesample(pdf, wxT("Punjabi (pa)"), wxT("indic-punjabi.txt"), wxT("RaghuPunjabi"));
  writesample(pdf, wxT("Tamil (ta)"), wxT("indic-tamil.txt"), wxT("RaghuTamil"));
  writesample(pdf, wxT("Telugu (te)"), wxT("indic-telugu.txt"), wxT("RaghuTelugu"));

  pdf.SaveAsFile(wxT("indicfonts.pdf"));
}

void
indicfonts()
{
  fullmoon();
  samples();
}
