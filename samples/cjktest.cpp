///////////////////////////////////////////////////////////////////////////////
// Name:        cjktest.cpp
// Purpose:     Demonstration of CJK fonts in wxPdfDocument
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

#include "wx/pdfdoc.h"

/**
* Chinese, Japanese and Korean fonts
*
* This example demonstrates the use of CJK fonts.
*
* Remark: Only available in Unicode build. 
*/

void
cjktest()
{
  wxCSConv conv_cn(_T("cp950"));
  wxString s_cn("²{®É®ğ·Å 18 C Àã«× 83 %",conv_cn);
  wxPdfDocument pdf;
  pdf.AddFontCJK(_T("Big5"));
  pdf.Open();
  pdf.AddPage();
  pdf.SetFont(_T("Arial"), _T(""),24);
  pdf.Write(10,_T("Chinese"));
  pdf.Ln(12);
  pdf.SetFont(_T("Big5"),_T(""),20);
  pdf.Write(10,s_cn);

  wxCSConv conv_jp(_T("cp932"));
  wxString s_jp("9ƒ–Œ‚ÌŒöŠJƒeƒXƒg‚ğŒo‚ÄPHP 3.0‚Í1998”N6Œ‚ÉŒö®‚ÉƒŠƒŠ[ƒX‚³‚ê‚Ü‚µ‚½B",conv_jp);
  pdf.AddFontCJK(_T("SJIS"));
  pdf.AddPage();
  pdf.SetFont(_T("Arial"), _T(""),24);
  pdf.Write(10,_T("Japanese"));
  pdf.Ln(12);
  pdf.SetFont(_T("SJIS"),_T(""),18);
  pdf.Write(8,s_jp);

  wxCSConv conv_kr(_T("cp949"));
  wxString s_kr("PHP 3.0Àº 1998³â 6¿ù¿¡ °ø½ÄÀûÀ¸·Î ¸±¸®ÁîµÇ¾ú´Ù. °ø°³ÀûÀÎ Å×½ºÆ® ÀÌÈÄ¾à 9°³¿ù¸¸ÀÌ¾ú´Ù.",conv_kr);
  pdf.AddFontCJK(_T("UHC"));
  pdf.AddPage();
  pdf.SetFont(_T("Arial"), _T(""),24);
  pdf.Write(10,_T("Korean"));
  pdf.Ln(12);
  pdf.SetFont(_T("UHC"),_T(""),18);
  pdf.Write(8,s_kr);

  pdf.SaveAsFile(_T("cjktest.pdf"));
}
