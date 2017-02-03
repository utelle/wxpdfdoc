///////////////////////////////////////////////////////////////////////////////
// Name:        cjktest.cpp
// Purpose:     Demonstration of CJK fonts in wxPdfDocument
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

#include "wx/pdfdoc.h"

/**
* Chinese, Japanese and Korean fonts
*
* This example demonstrates the use of CJK fonts.
*
* Remark: Only available in Unicode build. 
*/

int
cjktest(bool testMode)
{
  int rc = 0;
  wxCSConv conv_we(wxS("cp-1252"));
  wxString s_we("\xE4\xF6\xFC",conv_we);
  wxCSConv conv_cn(wxS("cp-950"));
  wxString s_cn("\xB2\x7B\xAE\xC9\xAE\xF0\xB7\xC5\x20\x31\x38\x20\x43\x20\xC0\xE3\xAB\xD7\x20\x38\x33\x20\x25",conv_cn);
  s_cn += s_we;
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  if (pdf.AddFontCJK(wxS("Big5")))
  {
    pdf.Open();
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"), wxS(""),24);
    pdf.Write(10,wxS("Chinese"));
    pdf.Ln(12);
    pdf.SetFont(wxS("Big5"),wxS(""),20);
    pdf.Write(10,s_cn);

    wxCSConv conv_jp(wxS("cp-932"));
    wxString s_jp("\x39\x83\x96\x8C\x8E\x82\xCC\x8C\xF6\x8A\x4A\x83\x65\x83\x58\x83\x67\x82\xF0\x8C\x6F\x82\xC4\x50\x48\x50\x20\x33\x2E\x30\x82\xCD\x31\x39\x39\x38\x94\x4E\x36\x8C\x8E\x82\xC9\x8C\xF6\x8E\xAE\x82\xC9\x83\x8A\x83\x8A\x81\x5B\x83\x58\x82\xB3\x82\xEA\x82\xDC\x82\xB5\x82\xBD\x81\x42",conv_jp);
    pdf.AddFontCJK(wxS("SJIS"));
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"), wxS(""),24);
    pdf.Write(10,wxS("Japanese"));
    pdf.Ln(12);
    pdf.SetFont(wxS("SJIS"),wxS(""),18);
    pdf.Write(8,s_jp);

    wxCSConv conv_kr(wxS("cp-949"));
    wxString s_kr("\x50\x48\x50\x20\x33\x2E\x30\xC0\xBA\x20\x31\x39\x39\x38\xB3\xE2\x20\x36\xBF\xF9\xBF\xA1\x20\xB0\xF8\xBD\xC4\xC0\xFB\xC0\xB8\xB7\xCE\x20\xB8\xB1\xB8\xAE\xC1\xEE\xB5\xC7\xBE\xFA\xB4\xD9\x2E\x20\xB0\xF8\xB0\xB3\xC0\xFB\xC0\xCE\x20\xC5\xD7\xBD\xBA\xC6\xAE\x20\xC0\xCC\xC8\xC4\xBE\xE0\x20\x39\xB0\xB3\xBF\xF9\xB8\xB8\xC0\xCC\xBE\xFA\xB4\xD9\x2E",conv_kr);
    pdf.AddFontCJK(wxS("UHC"));
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"), wxS(""),24);
    pdf.Write(10,wxS("Korean"));
    pdf.Ln(12);
    pdf.SetFont(wxS("UHC"),wxS(""),18);
    pdf.Write(8,s_kr);

    pdf.SaveAsFile(wxS("cjktest.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}
