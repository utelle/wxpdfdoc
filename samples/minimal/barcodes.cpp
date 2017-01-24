///////////////////////////////////////////////////////////////////////////////
// Name:        barcodes.cpp
// Purpose:     Demonstration of barcodes in wxPdfDocument
// Author:      Ulrich Telle
// Created:     2005-11-23
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

#include "wx/pdfbarcode.h"
#include "wx/pdfdoc.h"

/**
* Barcodes
*
* This example shows how to use the barcode creator add-on .
*/

int
barcodes(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  wxPdfBarCodeCreator barcode(pdf);
  pdf.AddPage();

  barcode.EAN13(80, 40, wxS("123456789012"));

  barcode.UPC_A(80, 70, wxS("1234567890"));

  barcode.Code39(60, 100, wxS("Code 39"));

  barcode.I25(90, 140, wxS("12345678"));

  wxString zipcode = wxS("48109-1109");
  barcode.PostNet(40,180,zipcode);
  pdf.Text(40,185,zipcode);

  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"), wxS(""), 10);

  // A set
  wxString code128 = wxS("CODE 128");
  barcode.Code128A(50, 20, code128, 20);
  pdf.SetXY(50, 45);
  pdf.Write(5, wxS("A set: ")+code128);

  // B set
  code128 = wxS("Code 128");
  barcode.Code128B(50, 70, code128, 20);
  pdf.SetXY(50,95);
  pdf.Write(5, wxS("B set: ")+code128);

  // C set
  code128 = wxS("12345678901234567890");
  barcode.Code128C(50, 120, code128, 20);
  pdf.SetXY(50, 145);
  pdf.Write(5, wxS("C set: ")+code128);

  // A,B,C sets
  code128 = wxS("ABCDEFG1234567890AbCdEf");
  barcode.Code128(50, 170, code128, 20);
  pdf.SetXY(50, 195);
  pdf.Write(5, wxS("ABC sets combined: ")+code128);

  // EAN with AIs
  code128 = wxS("(01)00000090311314(10)ABC123(15)060916");
  barcode.EAN128(50, 220, code128, 20);
  pdf.SetXY(50, 245);
  pdf.Write(5, wxS("EAN with AIs: ")+code128);

  pdf.SaveAsFile(wxS("barcodes.pdf"));

  return 0;
}

