///////////////////////////////////////////////////////////////////////////////
// Name:        glyphwriting.cpp
// Purpose:     WMF: Test program for the wxPdfDocument class
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

#include "wx/tokenzr.h"
//#include "wx/mstream.h"
#include "wx/txtstrm.h"
#include "wx/wfstream.h"

#include "wx/pdfdoc.h"
#include "wx/pdfutility.h"

/**
* Glyph writing
*
* This example shows writing a list of glyphs at specific positions.
*
* The sample text - a sample from ICU - was preprocessed with ICU under Windows.
*
* Note: the sample needs the fonts "Arial Unicode" and "Angsana New". The first
* two lines in the file glyphsample.txt specify the location of the font files
* on disk. Without these fonts the sample will not show proper results.
*/

int
glyphwriting(bool testMode)
{
  int rc = 0;
  double scaleFactor = 0.15;
  wxFileInputStream sampleFile(wxS("glyphsample.txt"));
  if (sampleFile.Ok())
  {
    wxTextInputStream text(sampleFile);

    wxPdfDocument pdf;
    if (testMode)
    {
      pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
      pdf.SetCompression(false);
    }
    pdf.AddPage();
    pdf.SetFont(wxS("helvetica"), wxS(""), 16);
    pdf.Text(10, 10, wxS("Direct glyph writing (sample text from the ICU project)"));
    pdf.SetFont(wxS("helvetica"), wxS(""), 10);
    pdf.Text(10, 15, wxS("Unicode text based on complex scripts may be preprocessed for PDF output with ICU or similar tools."));

    double xp = 0, yp = 0;
    double x, y;
    long gid, count;
    wxString line, token;
    wxPdfArrayDouble xOffsets;
    wxPdfArrayDouble yOffsets;
    wxPdfArrayUint32 gNumbers;
    while (!sampleFile.Eof())
    {
      line = text.ReadLine();
      line.Trim();
      wxStringTokenizer tkz(line, wxS(" "));
      int n = (int) tkz.CountTokens();
      if (n == 1)
      {
        wxStringTokenizer tkz2(line, wxS("="));
        wxString fontName = tkz2.GetNextToken();
        wxString fontFile = tkz2.GetNextToken();
        if (!pdf.AddFont(fontName, wxS(""), fontFile))
        {
          wxLogMessage(wxString(wxS("Error: Unable to load font file '")) + fontFile + wxString(wxS("'.")));
          break;
        }
      }
      else if (n == 5)
      {
        // Write glyph array if not empty
        if (gNumbers.GetCount() > 0)
        {
          pdf.SetXY(xp+10, yp+20);
          pdf.WriteGlyphArray(xOffsets, yOffsets, gNumbers);
          xOffsets.Empty();
          yOffsets.Empty();
          gNumbers.Empty();
        }
        // Start a new run
        token = tkz.GetNextToken();
        token.ToLong(&gid);
        token = tkz.GetNextToken();
        xp = scaleFactor * wxPdfUtility::String2Double(token);
        token = tkz.GetNextToken();
        yp = scaleFactor * wxPdfUtility::String2Double(token);
        token = tkz.GetNextToken();
        token.ToLong(&count);
        token = tkz.GetNextToken();
        if (count > 0)
        {
          pdf.SetFont(token, wxS(""), 14);
        }
      }
      else if (n == 3)
      {
        // glyph number, x offset, y offset
        token = tkz.GetNextToken();
        token.ToLong(&gid);
        token = tkz.GetNextToken();
        x = scaleFactor * wxPdfUtility::String2Double(token);
        token = tkz.GetNextToken();
        y = scaleFactor * wxPdfUtility::String2Double(token);
        xOffsets.Add(x);
        yOffsets.Add(y);
        gNumbers.Add(gid);
      }
    }

    pdf.SaveAsFile(wxS("glyphwriting.pdf"));
  }
  else
  {
    wxLogMessage(_("Error: Unable to read 'glyphsample.txt'."));
    rc = 1;
  }
  return rc;
}

