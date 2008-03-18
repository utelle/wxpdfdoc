///////////////////////////////////////////////////////////////////////////////
// Name:        tutorial7.cpp
// Purpose:     Tutorial 7: Test program for wxPdfDocument
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
* \li Generation of the metric file (.afm) 
* \li Generation of the font definition file (.php) 
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
* pdf.AddFont(_T("Comic"),_T(""),_T("comic.xml"));
*  
* or simply: 
* 
* pdf.AddFont(_T("Comic"));
*  
* And the font is now available (in regular and underlined styles), usable like the others.
* If we had worked with Comic Sans MS Bold (comicbd.ttf), we would have put: 
* 
* pdf.AddFont(_T("Comic"),_T("B"),_T("comicbd.xml"));
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

void
tutorial7()
{
  wxPdfDocument pdf;

  pdf.AddFont(_T("Calligrapher"),_T(""),_T("calligra.xml"));
  pdf.AddFont(_T("Feta"),_T(""),_T("feta16.xml"));
  pdf.AddFont(_T("FetaAlphabet"),_T(""),_T("feta-alphabet16.xml"));

  pdf.AddPage();
  pdf.SetFont(_T("Calligrapher"),_T(""),30);
  pdf.Cell(0,10,_T("Enjoy new fonts with wxPdfDocument!"));
  pdf.Ln(25);
#if wxUSE_UNICODE
  pdf.AddFont(_T("Vera"),_T(""),_T("vera.xml"));
  pdf.SetFont(_T("Vera"),_T(""),16);
  pdf.Cell(0,10,_T("This is the TrueType Unicode font BitstreamVeraSans!"));
  pdf.Ln(25);

  pdf.AddFont(_T("GFSDidot"),_T(""),_T("GFSDidot.xml"));
  pdf.SetFont(_T("GFSDidot"),_T(""),16);
  pdf.Cell(0,10,_T("And this is OpenType Unicode font GFSDidot:"));
  pdf.Ln(10);
  pdf.Cell(0,10,_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  pdf.Ln(10);
  pdf.Cell(0,10,_T("abcdefghijklmnopqrstuvwxyz"));
  pdf.Ln(25);
#endif

  pdf.SetFont(_T("Feta"),_T(""),35);
  pdf.SetLineWidth(0.6);
  pdf.Rect(10,150,95,16);
  pdf.Line(10,154,105,154);
  pdf.Line(10,158,105,158);
  pdf.Line(10,162,105,162);
  pdf.Text(14,162,_T("\xae"));
  pdf.Text(24,158,_T("\x33"));
  pdf.SetFont(_T("FetaAlphabet"),_T(""),35);
  pdf.Text(30,157,_T("\x36"));
  pdf.Text(30,165,_T("\x32"));
  pdf.SetFont(_T("Feta"),_T(""),35);
  pdf.Text(40,158,_T("\x3e"));
  pdf.Text(48,156,_T("\x3e"));
  pdf.Text(56,154,_T("\x3d"));
  pdf.Text(66,156,_T("\x3d"));
  pdf.Text(76,158,_T("\x3d"));
  pdf.Text(86,162,_T("\x3d"));
  pdf.Text(96,168,_T("\x3d"));
  pdf.Line(40,159,40,168);
  pdf.Line(48,157,48,166);
  pdf.Line(56,155,56,164);
  pdf.Line(66,157,66,166);
  pdf.Line(76,159,76,168);
  pdf.Line(90,161,90,151);
  pdf.Line(100,167,100,157);

  pdf.SaveAsFile(_T("tutorial7.pdf"));
}

