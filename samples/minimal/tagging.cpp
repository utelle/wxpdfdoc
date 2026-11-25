///////////////////////////////////////////////////////////////////////////////
// Name:        tagging.cpp
// Purpose:     Demonstration of tagged (accessible) PDF in wxPdfDocument
// Author:      wxPdfDocument contributors
// Created:     2026-11-23
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
* Tagged PDF
*
* This example shows how to produce a tagged PDF document, the basis of
* accessible PDF: headings, paragraphs, a figure with alternate text, and a
* mathematical formula that is drawn from several pieces (glyphs and a
* fraction bar) but described by an alternate text, a replacement text and
* a MathML representation.
*/

namespace
{

/// Page header and footer; in a tagged document they automatically become artifacts
class TaggedDocument : public wxPdfDocument
{
public:
  virtual void Header() wxOVERRIDE
  {
    SetFont(wxS("Helvetica"), wxS("I"), 8);
    Cell(0, 6, wxS("wxPdfDocument - tagged PDF sample"), 0, 1, wxPDF_ALIGN_RIGHT);
    Ln(6);
  }

  virtual void Footer() wxOVERRIDE
  {
    SetY(-15);
    SetFont(wxS("Helvetica"), wxS("I"), 8);
    Cell(0, 10, wxString::Format(wxS("Page %d"), PageNo()), 0, 0, wxPDF_ALIGN_CENTER);
  }
};

} // namespace

int
tagging(bool testMode)
{
  TaggedDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }

  // Enable tagging and set the properties an accessible document needs
  pdf.SetTagged();
  pdf.SetLanguage(wxS("en-US"));
  pdf.SetTitle(wxS("Tagged PDF sample"));
  pdf.SetViewerPreferences(wxPDF_VIEWER_DISPLAYDOCTITLE);

  pdf.AddPage();

  pdf.BeginTag(wxS("H1"));
  pdf.SetFont(wxS("Helvetica"), wxS("B"), 16);
  pdf.Cell(0, 10, wxS("Solving a quadratic equation"), 0, 1);
  pdf.EndTag();

  pdf.BeginTag(wxS("P"));
  pdf.SetFont(wxS("Helvetica"), wxS(""), 12);
  pdf.MultiCell(0, 6, wxS("The solutions of the equation a x^2 + b x + c = 0 are given by the ")
                      wxS("following formula. A screen reader announces it using the alternate text, ")
                      wxS("or presents it semantically if it understands the embedded MathML."));
  pdf.EndTag();
  pdf.Ln(4);

  // A formula drawn from several pieces, as a computer algebra system would do
  const wxString mathML =
    wxS("<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\">")
    wxS("<mi>x</mi><mo>=</mo>")
    wxS("<mfrac><mrow><mo>-</mo><mi>b</mi><mo>&#xB1;</mo>")
    wxS("<msqrt><msup><mi>b</mi><mn>2</mn></msup><mo>-</mo><mn>4</mn><mi>a</mi><mi>c</mi></msqrt></mrow>")
    wxS("<mrow><mn>2</mn><mi>a</mi></mrow></mfrac></math>");
  pdf.BeginFormula(wxS("x equals minus b plus or minus the square root of b squared minus 4 a c, all over 2 a"),
                   mathML,
                   wxS("x = (-b ± sqrt(b^2 - 4ac)) / (2a)"));
  double x = pdf.GetX() + 40;
  double y = pdf.GetY();
  pdf.SetFont(wxS("Times"), wxS("I"), 14);
  pdf.Text(x, y + 9, wxS("x"));
  pdf.SetFont(wxS("Times"), wxS(""), 14);
  pdf.Text(x + 5, y + 9, wxS("="));
  pdf.Text(x + 12, y + 5, wxS("-b ±"));
  // Square root sign and bar drawn with lines
  pdf.Line(x + 24, y + 3, x + 25.5, y + 5.5);
  pdf.Line(x + 25.5, y + 5.5, x + 27, y);
  pdf.Line(x + 27, y, x + 46, y);
  pdf.SetFont(wxS("Times"), wxS("I"), 14);
  pdf.Text(x + 28, y + 5, wxS("b"));
  pdf.SetFont(wxS("Times"), wxS(""), 9);
  pdf.Text(x + 31, y + 2.5, wxS("2"));
  pdf.SetFont(wxS("Times"), wxS(""), 14);
  pdf.Text(x + 34, y + 5, wxS("- 4"));
  pdf.SetFont(wxS("Times"), wxS("I"), 14);
  pdf.Text(x + 41, y + 5, wxS("ac"));
  // Fraction bar
  pdf.Line(x + 11, y + 7.5, x + 47, y + 7.5);
  pdf.SetFont(wxS("Times"), wxS(""), 14);
  pdf.Text(x + 25, y + 13, wxS("2"));
  pdf.SetFont(wxS("Times"), wxS("I"), 14);
  pdf.Text(x + 28, y + 13, wxS("a"));
  pdf.EndFormula();
  pdf.SetY(y + 20);

  // Decoration without meaning is marked as artifact
  pdf.BeginArtifact();
  pdf.Line(pdf.GetX(), pdf.GetY(), pdf.GetX() + 190, pdf.GetY());
  pdf.EndArtifact();
  pdf.Ln(4);

  // A figure needs an alternate text
  pdf.BeginTag(wxPdfTag(wxS("Figure")).SetAltText(wxS("The wxPdfDocument logo")));
  pdf.Image(wxS("wxpdfdoc.png"), pdf.GetX(), pdf.GetY(), 40);
  pdf.EndTag();

  pdf.SaveAsFile(wxS("tagging.pdf"));
  return 0;
}
