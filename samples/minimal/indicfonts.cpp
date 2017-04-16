///////////////////////////////////////////////////////////////////////////////
// Name:        indictest.cpp
// Purpose:     Demonstration of Indic fonts in wxPdfDocument
// Author:      Ulrich Telle
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

static int
writeFullMoon(wxPdfDocument& pdf, const wxString& txt1, const wxString& txt2, const wxString& fontName)
{
  int rc = 0;
  if (pdf.AddFont(fontName))
  {
    pdf.SetFont(fontName, wxS(""), 22);
    pdf.Cell(40, 12, txt1);
    pdf.Cell(100, 12, txt2);
    pdf.Ln(13);
  }
  else
  {
    rc = 1;
  }
  return rc;
}

static int
fullmoon(bool testMode)
{
  int rc = 0;
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

  int failed = 0;
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();
  pdf.SetTopMargin(20);
  pdf.SetLeftMargin(20);
  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"), wxS("B"),24);
  pdf.Write(10,wxS("Full Moon in 9 Indic Scripts"));
  pdf.Ln(15);
  failed += writeFullMoon(pdf, hindi1, hindi2, wxS("RaghuHindi"));
  failed += writeFullMoon(pdf, bengali1, bengali2, wxS("RaghuBengali"));
  failed += writeFullMoon(pdf, punjabi1, punjabi2, wxS("RaghuPunjabi"));
  failed += writeFullMoon(pdf, gujarati1, gujarati2, wxS("RaghuGujarati"));
  failed += writeFullMoon(pdf, oriya1, oriya2, wxS("RaghuOriya"));
  failed += writeFullMoon(pdf, tamil1, tamil2, wxS("RaghuTamil"));
  failed += writeFullMoon(pdf, telugu1, telugu2, wxS("RaghuTelugu"));
  failed += writeFullMoon(pdf, kannada1, kannada2, wxS("RaghuKannada"));
  failed += writeFullMoon(pdf, malayalam1, malayalam2, wxS("RaghuMalayalam"));

  pdf.SaveAsFile(wxS("fullmoon.pdf"));

  if (failed > 0)
  {
    rc = 1;
  }
  return rc;
}

static int
writesample(wxPdfDocument& pdf, const wxString& header, const wxString& sampleFile, const wxString& fontName)
{
  int rc = 0;
  pdf.SetFont(wxS("Helvetica"), wxS("B"), 16);
  pdf.Write(10,header);
  pdf.Ln(14);

  wxFileInputStream f(sampleFile);
  int len = f.GetLength();
  char* ctxt = new char[len+1];
  f.Read(ctxt,len);
  ctxt[len] = '\0';
  wxString sampleText(ctxt, wxConvUTF8);
  delete [] ctxt;

  if (pdf.AddFont(fontName))
  {
    pdf.SetFont(fontName, wxS(""), 15);
    pdf.MultiCell(160, 7, sampleText, 0, wxPDF_ALIGN_LEFT);
    pdf.Ln();
  }
  else
  {
    rc = 1;
  }

  return rc;
}

static int
samples(bool testMode)
{
  int rc = 0;
  if (wxFileName::IsFileReadable(wxS("indic-assamese.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-bengali.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-gujarati.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-hindi.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-kannada.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-malayalam.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-nepali.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-oriya.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-punjabi.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-tamil.txt")) &&
      wxFileName::IsFileReadable(wxS("indic-telugu.txt")))
  {
    int failed = 0;
    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.Open();
    pdf.SetTopMargin(30);
    pdf.SetLeftMargin(30);
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"), wxS("B"),32);
    pdf.Write(10,wxS("Indic Fonts and Languages"));
    pdf.Ln(17);
    failed += writesample(pdf, wxS("Assamese (as)"), wxS("indic-assamese.txt"), wxS("RaghuBengali"));
    failed += writesample(pdf, wxS("Bengali (bn)"), wxS("indic-bengali.txt"), wxS("RaghuBengali"));
    failed += writesample(pdf, wxS("Gujarati (gu)"), wxS("indic-gujarati.txt"), wxS("RaghuGujarati"));
    failed += writesample(pdf, wxS("Hindi (hi)"), wxS("indic-hindi.txt"), wxS("RaghuHindi"));
    failed += writesample(pdf, wxS("Kannada (kn)"), wxS("indic-kannada.txt"), wxS("RaghuKannada"));
    failed += writesample(pdf, wxS("Malayalam (ml)"), wxS("indic-malayalam.txt"), wxS("RaghuMalayalam"));
    failed += writesample(pdf, wxS("Nepali (ne) - Devanagari"), wxS("indic-nepali.txt"), wxS("RaghuHindi"));
    failed += writesample(pdf, wxS("Oriya (or)"), wxS("indic-oriya.txt"), wxS("RaghuOriya"));
    failed += writesample(pdf, wxS("Punjabi (pa)"), wxS("indic-punjabi.txt"), wxS("RaghuPunjabi"));
    failed += writesample(pdf, wxS("Tamil (ta)"), wxS("indic-tamil.txt"), wxS("RaghuTamil"));
    failed += writesample(pdf, wxS("Telugu (te)"), wxS("indic-telugu.txt"), wxS("RaghuTelugu"));

    pdf.SaveAsFile(wxS("indicfonts.pdf"));

    if (failed > 0)
    {
      rc = 1;
    }
  }
  else
  {
    rc = 1;
  }
  return rc;
}

int
indicfonts(bool testMode)
{
  int failed = 0;
  failed += fullmoon(testMode);
  failed += samples(testMode);
  return failed;
}
