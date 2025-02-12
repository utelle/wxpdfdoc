///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial7.cpp
// Purpose:     Tutorial 7: Test program for wxPdfDocument
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
#include "wx/pdfencoding.h"
#include "wx/pdffontmanager.h"

/**
* Adding new fonts and encoding support
*
* This tutorial explains how to use TrueType or Type1 fonts so that you are not
* limited to the standard fonts any more. The other interest is that you can
* choose the font encoding, which allows you to use other languages than the
* Western ones (the standard fonts having too few available characters).
*
* There are two ways to use a new font: embedding it in the PDF or not. When a
* font is not embedded, it is sought in the system. The advantage is that the
* PDF file is lighter; on the other hand, if it is not available, a substitution
* font is used. So it is preferable to ensure that the needed font is installed
* on the client systems. If the file is to be viewed by a large audience, it is
* better to embed.
*
* Adding a new font requires three steps for TrueTypes:
*
* \li Generation of the metric file (.afm/.ufm)
* \li Generation of the font definition file (.xml)
* \li Declaration of the font in the program
*
* For Type1, the first one is theoretically not necessary because the AFM file is
* usually shipped with the font. In case you have only a metric file in PFM format,
* it must be converted to AFM first.
*
* Generation of the metric file
*
* The first step for a TrueType consists in generating the AFM file. A utility exists
* to do this task: ttf2pt1. The Windows binary is available here. The command line
* to use is the following:
*
* ttf2pt1 -a font.ttf font
*
* For example, for Comic Sans MS Regular:
*
* ttf2pt1 -a c:\windows\fonts\comic.ttf comic
*
* Two files are created; the one we are interested in is comic.afm.
*
* Generation of the font definition file
*
* The second step consists in generating a wxPdfDocument font metrics XML file containing
* all the information needed by wxPdfDocument; in addition, the font file is compressed.
* To do this, a utility program, makefont, will be provided in a future release.
*
* You have to copy the generated file(s) to the font directory. If the font file could
* not be compressed, copy the .ttf or .pfb instead of the .z.
*
* Declaration of the font in the script
*
* The last step is the most simple. You just need to call the AddFont() method. For instance:
*
* pdf.AddFont(wxS("Comic"),wxS(""),wxS("comic.xml"));
*
* or simply:
*
* pdf.AddFont(wxS("Comic"));
*
* And the font is now available (in regular and underlined styles), usable like the others.
* If we had worked with Comic Sans MS Bold (comicbd.ttf), we would have put:
*
* pdf.AddFont(wxS("Comic"),wxS("B"),wxS("comicbd.xml"));
*
* Reducing the size of TrueType fonts
*
* Font files are often quite voluminous (more than 100, even 200KB); this is due to the
* fact that they contain the characters corresponding to many encodings. zlib compression
* reduces them but they remain fairly big. A technique exists to reduce them further.
* It consists in converting the font to the Type1 format with ttf2pt1 by specifying the
* encoding you are interested in; all other characters will be discarded.
* For instance, the arial.ttf font shipped with Windows 98 is 267KB (it contains 1296
* characters). After compression it gives 147. Let's convert it to Type1 by keeping
* only cp1250 characters:
*
* ttf2pt1 -b -L cp1250.map c:\windows\fonts\arial.ttf arial
*
* The .map files are located in the font/makefont/ directory of the package.
* The command produces arial.pfb and arial.afm. The arial.pfb file is only 35KB,
* and 30KB after compression.
*
* It is possible to go even further. If you are interested only by a subset of the
* encoding (you probably don't need all 217 characters), you can open the .map file
* and remove the lines you are not interested in. This will reduce the file size
* accordingly.
*/

int
createFontList(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"),wxS(""),12);

  wxPdfFontManager::GetFontManager()->RegisterSystemFonts();
  size_t n = wxPdfFontManager::GetFontManager()->GetFontCount();
  size_t j;
  for (j = 0; j < n; ++j)
  {
    wxPdfFont font = wxPdfFontManager::GetFontManager()->GetFont(j);
    pdf.Write(5, font.GetName());
    pdf.Ln();
  }
  pdf.SaveAsFile(wxS("fontlist.pdf"));
  return 0;
}

int
twoEncodings(bool testMode)
{
  int rc = 0;
  wxString cyrillicText("Tchaikovsky - \xd0\xa7\xd0\xb0\xd0\xb9\xd0\xbao\xd0\xb2\xd1\x81\xd0\xba\xd0\xb8\xd0\xb9", wxConvUTF8);
  wxPdfFont nimbus1 = wxPdfFontManager::GetFontManager()->RegisterFont(wxS("urw-nimbus-cyr.pfb"), wxS("nimbus"));
  if (nimbus1.IsValid())
  {
    wxPdfFont nimbus2(nimbus1);
    // Set cyrillic encoding
    nimbus1.SetEncoding(wxS("cp-1251"));

    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();
    pdf.SetFont(nimbus2, wxPDF_FONTSTYLE_REGULAR, 16);
    pdf.Cell(0, 10, wxS("This is font URW Nimbus Cyrillic (WinAnsi encoding)"));
    pdf.Ln(10);
    pdf.SetFont(nimbus1, wxPDF_FONTSTYLE_REGULAR, 16);
    pdf.Cell(0, 10, wxS("This is font URW Nimbus Cyrillic (CP-1251 encoding)"));
    pdf.Ln(10);
    pdf.Cell(0, 10, cyrillicText);
    pdf.Ln(10);

    pdf.SaveAsFile(wxS("twoencodings.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}

// TexturaModern font
#include "texturamodern.h"

int
fontSample(bool testMode)
{
  int rc = 0;
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }

  wxPdfFontManager::GetFontManager()->RegisterFont("Amaranth-Regular.woff", "Amaranth");
  wxPdfFontManager::GetFontManager()->RegisterFont("HussarBoldWeb.woff2", "Hussar");
  wxPdfFontManager::GetFontManager()->RegisterFont((const char*) texturamodern_ttf, texturamodern_ttf_size, "TexturaModern");

#if wxUSE_UNICODE
#if defined(__WXMSW__) || defined(__WXMAC__)
  // TODO: using wxFont objects does not work in console application under Linux
  const wxFont* myFont = wxNORMAL_FONT;
  wxPdfFontManager::GetFontManager()->RegisterFont(*myFont, wxS("Normal"));
#endif
  if (!pdf.AddFont(wxS("liz"), wxS(""), wxS("liz.otf"))) ++rc;
  if (!pdf.AddFont(wxS("GFSDidot"), wxS(""), wxS("GFSDidot.xml"))) ++rc;
#endif

  if (!pdf.AddFont(wxS("Calligrapher"), wxS(""), wxS("calligra.xml"))) ++rc;
  if (!pdf.AddFont(wxS("Feta"), wxS(""), wxS("feta16.xml"))) ++rc;
  if (!pdf.AddFont(wxS("FetaAlphabet"), wxS(""), wxS("feta-alphabet16.xml"))) ++rc;
  if (rc == 0)
  {
    pdf.AddPage();
    pdf.SetFont(wxS("Calligrapher"),wxS(""),30);
    pdf.Cell(0,10,wxS("Enjoy new fonts with wxPdfDocument!"));
    pdf.Ln(25);
#if wxUSE_UNICODE
    pdf.AddFont(wxS("Vera"),wxS(""),wxS("vera.xml"));
    pdf.SetFont(wxS("Vera"),wxS(""),16);
    pdf.Cell(0,10,wxS("This is the TrueType Unicode font BitstreamVeraSans!"));
    pdf.Ln(20);

    pdf.SetFont(wxS("GFSDidot"),wxS(""),16);
    pdf.Cell(0,10,wxS("And this is OpenType Unicode font GFSDidot:"));
    pdf.Ln(10);
    pdf.Cell(0,10,wxS("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    pdf.Ln(10);
    pdf.Cell(0,10,wxS("abcdefghijklmnopqrstuvwxyz"));
    pdf.Ln(20);
#endif

#if wxUSE_UNICODE
    pdf.Ln(10);
    pdf.SetFont(wxS("liz"),wxS(""),16);
    pdf.Cell(0,10,wxS("And this is OpenType font LIZ:"));
    pdf.Ln(10);
    pdf.Cell(0,10,wxS("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    pdf.Ln(20);
#endif

    pdf.SetFont(wxS("Helvetica"), wxS(""), 16);
    pdf.Cell(0, 10, wxS("And this is font Feta for music notes:"));
    pdf.SetFont(wxS("Feta"),wxS(""),35);
    pdf.SetLineWidth(0.6);
    pdf.Rect(10,150,95,16);
    pdf.Line(10,154,105,154);
    pdf.Line(10,158,105,158);
    pdf.Line(10,162,105,162);
    pdf.Text(14,162,wxS("\xae"));
    pdf.Text(24,158,wxS("\x33"));
    pdf.SetFont(wxS("FetaAlphabet"),wxS(""),35);
    pdf.Text(30,157,wxS("\x36"));
    pdf.Text(30,165,wxS("\x32"));
    pdf.SetFont(wxS("Feta"),wxS(""),35);
    pdf.Text(40,158,wxS("\x3e"));
    pdf.Text(48,156,wxS("\x3e"));
    pdf.Text(56,154,wxS("\x3d"));
    pdf.Text(66,156,wxS("\x3d"));
    pdf.Text(76,158,wxS("\x3d"));
    pdf.Text(86,162,wxS("\x3d"));
    pdf.Text(96,168,wxS("\x3d"));
    pdf.Line(40,159,40,168);
    pdf.Line(48,157,48,166);
    pdf.Line(56,155,56,164);
    pdf.Line(66,157,66,166);
    pdf.Line(76,159,76,168);
    pdf.Line(90,161,90,151);
    pdf.Line(100,167,100,157);

    pdf.SetXY(20, 175);

#if wxUSE_UNICODE
#if defined(__WXMSW__) || defined(__WXMAC__)
    // TODO: using wxFont objects does not work in console application under Linux
    pdf.SetFont(wxS("Normal"),wxS(""),16);
    pdf.Cell(0,10,wxS("And this is wxFont wxNORMAL_FONT:"));
    pdf.Ln(10);
    pdf.Cell(0,10,wxS("abcdefghijklmnopqrstuvwxyz"));
    pdf.Ln(20);
#endif
#endif

    pdf.SetFont(wxS("Hussar"), wxS(""), 16);
    pdf.Cell(0, 10, wxS("Enjoy WOFF2 font HussarBoldWeb with wxPdfDocument!"));
    pdf.Ln(20);
    pdf.SetFont(wxS("Amaranth"), wxS(""), 25);
    pdf.Cell(0, 10, wxS("Enjoy WOFF font Amaranth with wxPdfDocument!"));
    pdf.Ln(20);
    pdf.SetFont(wxS("TexturaModern"), wxS(""), 30);
    pdf.Cell(0, 10, wxS("Enjoy byte stream font TexturaModern with wxPdfDocument!"));

    pdf.SaveAsFile(wxS("tutorial7.pdf"));
  }
  else
  {
    rc = 1;
  }
  return rc;
}

int
tutorial7(bool testMode)
{
  int failed = 0;
  failed += fontSample(testMode);
#if wxUSE_UNICODE
  failed += twoEncodings(testMode);
#endif
  failed += createFontList(testMode);
  return failed;
}

