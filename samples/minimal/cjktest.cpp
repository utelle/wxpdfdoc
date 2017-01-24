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
  wxString s_we("äöü",conv_we);
  wxCSConv conv_cn(wxS("cp-950"));
  wxString s_cn("²{®É®ğ·Å 18 C Àã«× 83 %",conv_cn);
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
    wxString s_jp("9ƒ–Œ‚ÌŒöŠJƒeƒXƒg‚ğŒo‚ÄPHP 3.0‚Í1998”N6Œ‚ÉŒö®‚ÉƒŠƒŠ[ƒX‚³‚ê‚Ü‚µ‚½B",conv_jp);
    pdf.AddFontCJK(wxS("SJIS"));
    pdf.AddPage();
    pdf.SetFont(wxS("Helvetica"), wxS(""),24);
    pdf.Write(10,wxS("Japanese"));
    pdf.Ln(12);
    pdf.SetFont(wxS("SJIS"),wxS(""),18);
    pdf.Write(8,s_jp);

    wxCSConv conv_kr(wxS("cp-949"));
    wxString s_kr("PHP 3.0Àº 1998³â 6¿ù¿¡ °ø½ÄÀûÀ¸·Î ¸±¸®ÁîµÇ¾ú´Ù. °ø°³ÀûÀÎ Å×½ºÆ® ÀÌÈÄ¾à 9°³¿ù¸¸ÀÌ¾ú´Ù.",conv_kr);
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
