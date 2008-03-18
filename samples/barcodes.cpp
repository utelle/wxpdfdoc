///////////////////////////////////////////////////////////////////////////////
// Name:        barcodes.cpp
// Purpose:     Demonstration of barcodes in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
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

void
barcodes()
{
  wxPdfDocument pdf;
  wxPdfBarCodeCreator barcode(pdf);
  pdf.AddPage();

  barcode.EAN13(80, 40, _T("123456789012"));

  barcode.UPC_A(80, 70, _T("1234567890"));

  barcode.Code39(60, 100, _T("Code 39"));

  barcode.I25(90, 140, _T("12345678"));

  wxString zipcode = _T("48109-1109");
  barcode.PostNet(40,180,zipcode);
  pdf.Text(40,185,zipcode);

  pdf.SaveAsFile(_T("barcodes.pdf"));
}

