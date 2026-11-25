/*
** Name:        fontfallback.cpp
** Purpose:     Automated tests for the font fallback feature of wxPdfDocument
** Author:      wxPdfDocument contributors
** Created:     2026-11-23
** Copyright:   (c) 2026 wxPdfDocument contributors
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

// The tests create small PDF documents without compression and inspect the
// resulting PDF code. They only use fonts shipped with wxPdfDocument (lib/fonts)
// and the PDF core fonts, so their outcome doesn't depend on the system - with
// the exception of the system font fallback test, which is skipped if the
// system has no suitable font.
//
// Usage: fontfallback [font directory]
// The font directory defaults to WXPDFDOC_TEST_FONTDIR (set by the build system).
// The exit code is the number of failed checks.

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/init.h>
#include <wx/mstream.h>

#include <cmath>
#include <cstdio>
#include <string>

#include "wx/pdfdoc.h"
#include "wx/pdffontmanager.h"

#ifndef WXPDFDOC_TEST_FONTDIR
#define WXPDFDOC_TEST_FONTDIR "../lib/fonts"
#endif

// ----------------------------------------------------------------------------
// Minimal test harness
// ----------------------------------------------------------------------------

static int gs_failures = 0;
static int gs_checks = 0;

#define CHECK(cond) CheckImpl((cond), #cond, __FILE__, __LINE__)

static void
CheckImpl(bool ok, const char* expr, const char* file, int line)
{
  ++gs_checks;
  if (!ok)
  {
    ++gs_failures;
    std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", file, line, expr);
  }
}

static bool
NearlyEqual(double a, double b)
{
  return std::fabs(a - b) < 1e-6;
}

// Number of non-overlapping occurrences of needle in haystack
static int
Count(const std::string& haystack, const std::string& needle)
{
  int n = 0;
  for (size_t pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + needle.size()))
  {
    ++n;
  }
  return n;
}

static bool
Contains(const std::string& haystack, const std::string& needle)
{
  return haystack.find(needle) != std::string::npos;
}

// Finish the document and return the PDF code
static std::string
GetPdf(wxPdfDocument& pdf)
{
  const wxMemoryOutputStream& stream = pdf.CloseAndGetBuffer();
  wxStreamBuffer* buffer = stream.GetOutputStreamBuffer();
  return std::string((const char*) buffer->GetBufferStart(), buffer->GetBufferSize());
}

// Returns the content stream of the first page, i.e. the part with the
// text operators. Font resources are output after the page content.
static std::string
GetPageContent(const std::string& pdf)
{
  size_t start = pdf.find("stream\n");
  size_t end = pdf.find("endstream", start);
  if (start == std::string::npos || end == std::string::npos)
  {
    return std::string();
  }
  return pdf.substr(start, end - start);
}

// A document with the settings shared by all tests
class TestDocument : public wxPdfDocument
{
public:
  TestDocument()
  {
    SetCompression(false);
    // Fixed dates make the output reproducible (handy for debugging)
    SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2026));
    AddPage();
  }
};

// Characters used below:
// U+03C0 (pi) and U+2211 (n-ary sum) are part of Bitstream Vera, but not of the
// PDF core font Helvetica (WinAnsi encoding).
// U+03B1 (alpha) is part of GFSDidot, but not of Vera.
// U+4E00 (a CJK ideograph) is contained in none of these fonts.
static const wxString PI    = wxString(wxUniChar(0x03C0));
static const wxString SUM   = wxString(wxUniChar(0x2211));
static const wxString ALPHA = wxString(wxUniChar(0x03B1));
static const wxString CJK   = wxString(wxUniChar(0x4E00));

// ----------------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------------

// Without any fallback configuration the output must not change: missing
// characters stay with the current font.
static void
TestDisabledByDefault()
{
  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.SetFont(wxS("Vera"), wxS(""), 12));
  CHECK(!pdf.GetSystemFontFallback());
  pdf.Text(10, 20, wxS("a") + ALPHA + wxS("b"));
  std::string content = GetPageContent(GetPdf(pdf));
  // Only the font selection by SetFont, no switching inside the text object
  CHECK(Count(content, " Tf") == 1);
}

// The core idea: characters missing in the current font are rendered with the
// first fallback font that has them, and the current font is restored afterwards.
static void
TestExplicitFallback()
{
  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.AddFont(wxS("GFSDidot"), wxS(""), wxS("GFSDidot.xml")));
  CHECK(pdf.SetFont(wxS("Vera"), wxS(""), 12));
  CHECK(pdf.AddFallbackFont(wxS("GFSDidot")));
  pdf.Text(10, 20, wxS("a") + ALPHA + wxS("b"));
  std::string doc = GetPdf(pdf);
  std::string content = GetPageContent(doc);

  // Font 1 is Vera (selected by SetFont), font 2 is GFSDidot (used for alpha).
  // The text object written by Text() is the one positioned with Tm.
  size_t bt = content.find(" Tm ");
  size_t switchToFallback = content.find("/F2 12.00 Tf", bt);
  size_t switchBack = content.find("/F1 12.00 Tf", bt);
  size_t et = content.find("ET", bt);
  CHECK(bt != std::string::npos);
  CHECK(switchToFallback != std::string::npos);
  CHECK(switchBack != std::string::npos);
  // a (Vera) | alpha (GFSDidot) | b (Vera): three text show operators in one text object
  CHECK(switchToFallback < switchBack && switchBack < et);
  CHECK(Count(content.substr(bt, et - bt), "Tj") == 3);
  // Both fonts are embedded
  CHECK(Contains(doc, "BitstreamVeraSans-Roman"));
  CHECK(Contains(doc, "GFSDidot"));
}

// Fonts are tried in the order they were added; the first one containing the
// character wins.
static void
TestFallbackOrder()
{
  {
    TestDocument pdf;
    CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
    CHECK(pdf.AddFont(wxS("GFSDidot"), wxS(""), wxS("GFSDidot.xml")));
    CHECK(pdf.SetFont(wxS("Helvetica"), wxS(""), 10));
    // Both contain pi, only GFSDidot contains alpha.
    CHECK(pdf.AddFallbackFont(wxS("Vera")));
    CHECK(pdf.AddFallbackFont(wxS("GFSDidot")));
    pdf.Text(10, 20, PI + ALPHA);
    std::string doc = GetPdf(pdf);
    std::string content = GetPageContent(doc);
    // F1 = Helvetica, F2 = Vera (pi), F3 = GFSDidot (alpha), then back to F1
    size_t f2 = content.find("/F2 10.00 Tf");
    size_t f3 = content.find("/F3 10.00 Tf");
    size_t f1 = content.rfind("/F1 10.00 Tf");
    CHECK(f2 != std::string::npos && f3 != std::string::npos && f1 != std::string::npos);
    CHECK(f2 < f3 && f3 < f1);
    CHECK(Contains(doc, "BitstreamVeraSans-Roman"));
    CHECK(Contains(doc, "GFSDidot"));
  }
  {
    // Same text, reversed order: GFSDidot now covers both characters
    TestDocument pdf;
    CHECK(pdf.SetFont(wxS("Helvetica"), wxS(""), 10));
    CHECK(pdf.AddFallbackFont(wxS("GFSDidot")));
    CHECK(pdf.AddFallbackFont(wxS("Vera")));
    pdf.Text(10, 20, PI + ALPHA);
    std::string doc = GetPdf(pdf);
    std::string content = GetPageContent(doc);
    CHECK(Count(content, "/F2 10.00 Tf") == 1);
    CHECK(!Contains(content, "/F3 "));
    CHECK(!Contains(doc, "BitstreamVeraSans-Roman"));
  }
}

// Text in a PDF core font (single byte encoding) can be mixed with text in an
// embedded Unicode font (two byte glyph ids) within one text object.
static void
TestCoreFontPrimary()
{
  TestDocument pdf;
  CHECK(pdf.SetFont(wxS("Helvetica"), wxS(""), 12));
  CHECK(pdf.AddFallbackFont(wxS("Vera")));
  pdf.Text(10, 20, wxS("x=") + PI);
  std::string doc = GetPdf(pdf);
  std::string content = GetPageContent(doc);
  CHECK(Contains(content, "(x=) Tj /F2 12.00 Tf ("));
  // Without fallback pi would be replaced by a question mark
  CHECK(!Contains(content, "?"));
  CHECK(Contains(doc, "/BaseFont /Helvetica"));
  CHECK(Contains(doc, "BitstreamVeraSans-Roman"));
}

// GetStringWidth must measure each character with the font it is shown with;
// otherwise centering, right alignment and line breaking would be off.
static void
TestStringWidth()
{
  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.SetFont(wxS("Helvetica"), wxS(""), 10));
  wxPdfFont helvetica = wxPdfFontManager::GetFontManager()->GetFont(wxS("Helvetica"));
  wxPdfFont vera = wxPdfFontManager::GetFontManager()->GetFont(wxS("Vera"));
  CHECK(helvetica.IsValid() && vera.IsValid());

  const wxString text = wxS("ab") + PI + SUM + wxS("c");
  double widthWithoutFallback = pdf.GetStringWidth(text);

  CHECK(pdf.AddFallbackFont(vera));
  double widthWithFallback = pdf.GetStringWidth(text);

  // wxPdfFont::GetStringWidth returns the width for a font size of one user unit
  double fontSize = 10 / pdf.GetScaleFactor();
  double expected = (helvetica.GetStringWidth(wxS("abc")) + vera.GetStringWidth(PI + SUM)) * fontSize;
  CHECK(NearlyEqual(widthWithFallback, expected));
  CHECK(!NearlyEqual(widthWithFallback, widthWithoutFallback));

  // Removing the fallback fonts restores the old behaviour
  pdf.ClearFallbackFonts();
  CHECK(NearlyEqual(pdf.GetStringWidth(text), widthWithoutFallback));
}

// Bold text should fall back to the bold variant of the fallback family.
static void
TestStyleVariant()
{
  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.AddFont(wxS("Vera"), wxS("B"), wxS("verab.xml")));
  CHECK(pdf.AddFallbackFont(wxS("Vera")));
  CHECK(pdf.SetFont(wxS("Helvetica"), wxS("B"), 12));
  pdf.Text(10, 20, PI);
  CHECK(pdf.SetFont(wxS("Helvetica"), wxS(""), 12));
  pdf.Text(10, 30, PI);
  std::string doc = GetPdf(pdf);
  // Helvetica-Bold, Vera Bold, Helvetica, Vera
  CHECK(Contains(doc, "BitstreamVeraSans-Bold"));
  CHECK(Contains(doc, "BitstreamVeraSans-Roman"));
}

// If no font contains a character, nothing changes compared to disabled fallback.
static void
TestNoFontHasGlyph()
{
  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.AddFont(wxS("GFSDidot"), wxS(""), wxS("GFSDidot.xml")));
  CHECK(pdf.SetFont(wxS("Vera"), wxS(""), 12));
  CHECK(pdf.AddFallbackFont(wxS("GFSDidot")));
  pdf.Text(10, 20, wxS("a") + CJK);
  std::string content = GetPageContent(GetPdf(pdf));
  CHECK(Count(content, " Tf") == 1);
}

// A combining mark following a fallback character stays with the fallback font
// even if the current font has the mark, so that it is positioned correctly.
// None of the fonts shipped with wxPdfDocument allows to construct this case,
// so an installed font is used (the test is skipped if there is none).
static void
TestCombiningMark()
{
  const wxString zhe = wxString(wxUniChar(0x0436));   // Cyrillic small letter zhe
  const wxString tilde = wxString(wxUniChar(0x0303)); // combining tilde
  wxPdfFont didot = wxPdfFontManager::GetFontManager()->GetFont(wxS("GFSDidot"));
  wxPdfFont cyrillic = wxPdfFontManager::GetFontManager()->FindFallbackFont(0x0436);
  if (!cyrillic.IsValid() || !cyrillic.CanShow(tilde))
  {
    std::printf("SKIP TestCombiningMark: no installed font with Cyrillic letters and combining marks\n");
    return;
  }
  // Preconditions: GFSDidot has the combining tilde, but no Cyrillic letters
  CHECK(didot.CanShow(tilde) && !didot.CanShow(zhe));

  TestDocument pdf;
  CHECK(pdf.SetFont(didot, wxPDF_FONTSTYLE_REGULAR, 12));
  CHECK(pdf.AddFallbackFont(cyrillic));
  pdf.Text(10, 20, zhe + tilde + wxS("x"));
  std::string content = GetPageContent(GetPdf(pdf));
  // zhe + tilde with F2, then x with F1: one switch to the fallback font and one back
  CHECK(Count(content, "/F2 12.00 Tf") == 1);
  size_t tm = content.find(" Tm ");
  CHECK(Count(content.substr(tm), "Tj") == 2);
}

// Kerning and justified text use TJ arrays; these must work per run as well.
static void
TestKerningAndJustification()
{
  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.SetFont(wxS("Vera"), wxS(""), 12));
  CHECK(pdf.AddFallbackFont(wxS("GFSDidot")));
  pdf.SetKerning(true);
  pdf.Text(10, 20, wxS("AVA ") + ALPHA + wxS(" AVA"));
  wxString longText;
  for (int j = 0; j < 20; ++j)
  {
    longText += wxS("Lorem ipsum ") + ALPHA + wxS(" dolor ");
  }
  pdf.MultiCell(100, 5, longText, 0, wxPDF_ALIGN_JUSTIFY);
  std::string content = GetPageContent(GetPdf(pdf));
  CHECK(Contains(content, "TJ"));
  // One switch per alpha: 1 in Text() and 20 in MultiCell()
  CHECK(Count(content, "/F2 12.00 Tf") == 21);
}

// Text in templates is output to the template's own content stream and
// resource dictionary; the fallback font must be listed there.
static void
TestTemplate()
{
  TestDocument pdf;
  CHECK(pdf.SetFont(wxS("Helvetica"), wxS(""), 12));
  CHECK(pdf.AddFallbackFont(wxS("Vera")));
  int tpl = pdf.BeginTemplate(0, 0, 100, 100);
  pdf.Text(10, 20, PI);
  pdf.EndTemplate();
  pdf.UseTemplate(tpl);
  std::string doc = GetPdf(pdf);
  // The form XObject's resources must reference the fallback font
  size_t form = doc.find("/Subtype /Form");
  CHECK(form != std::string::npos);
  size_t formEnd = doc.find("endobj", form);
  CHECK(Contains(doc.substr(form, formEnd - form), "/F2 "));
}

// Fallback also works in PDF/A documents, where fallback fonts are
// subject to the same embedding rules as the current font.
// (Unicode TrueType/OpenType fonts are always embedded, so this mainly
// checks that both features work together. A standard font can't be the
// current font here: PDF/A replaces Helvetica by an installed Unicode font,
// which usually has the Greek letters itself.)
static void
TestPdfA1EmbedsFallbackFonts()
{
  TestDocument pdf;
  pdf.SetPdfA1Conformance(true);
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.AddFont(wxS("GFSDidot"), wxS(""), wxS("GFSDidot.xml")));
  CHECK(pdf.SetFont(wxS("Vera"), wxS(""), 12));
  CHECK(pdf.AddFallbackFont(wxS("GFSDidot")));
  pdf.Text(10, 20, wxS("x") + ALPHA);
  std::string doc = GetPdf(pdf);
  // Vera is font 1, GFSDidot (used for alpha) font 2; both are embedded
  CHECK(Contains(GetPageContent(doc), "/F2 12.00 Tf"));
  CHECK(Contains(doc, "BitstreamVeraSans-Roman"));
  CHECK(Contains(doc, "GFSDidot"));
  CHECK(Contains(doc, "/FontFile2"));
  CHECK(Contains(doc, "/FontFile3"));
  CHECK(!Contains(doc, "/Helvetica"));
}

// The fonts installed on the system are only searched if requested.
static void
TestSystemFallback()
{
  wxPdfFontManager* fontManager = wxPdfFontManager::GetFontManager();

  // Negative results must be reported (and cached) gracefully
  CHECK(!fontManager->FindFallbackFont(0x10FFFD).IsValid());
  CHECK(!fontManager->FindFallbackFont(0x10FFFD).IsValid());

  wxPdfFont systemFont = fontManager->FindFallbackFont(0x03B1);
  if (!systemFont.IsValid())
  {
    std::printf("SKIP: no installed font with Greek letters found\n");
    return;
  }
  CHECK(systemFont.CanShow(ALPHA));
  // The second lookup is answered from the cache and yields the same font
  CHECK(fontManager->FindFallbackFont(0x03B1).GetName() == systemFont.GetName());

  TestDocument pdf;
  CHECK(pdf.AddFont(wxS("Vera"), wxS(""), wxS("vera.xml")));
  CHECK(pdf.SetFont(wxS("Vera"), wxS(""), 12));
  pdf.SetSystemFontFallback(true);
  CHECK(pdf.GetSystemFontFallback());
  pdf.Text(10, 20, wxS("a") + ALPHA);
  std::string content = GetPageContent(GetPdf(pdf));
  CHECK(Contains(content, "/F2 12.00 Tf"));
}

// ----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
  wxInitializer initializer(argc, argv);
  if (!initializer.IsOk())
  {
    std::fprintf(stderr, "Failed to initialize wxWidgets\n");
    return 1;
  }

  wxString fontDir = (argc > 1) ? wxString(argv[1]) : wxString(wxS(WXPDFDOC_TEST_FONTDIR));
  wxPdfFontManager::GetFontManager()->AddSearchPath(fontDir);

  TestDisabledByDefault();
  TestExplicitFallback();
  TestFallbackOrder();
  TestCoreFontPrimary();
  TestStringWidth();
  TestStyleVariant();
  TestNoFontHasGlyph();
  TestCombiningMark();
  TestKerningAndJustification();
  TestTemplate();
  TestPdfA1EmbedsFallbackFonts();
  TestSystemFallback();

  std::printf("%d checks, %d failures\n", gs_checks, gs_failures);
  return gs_failures;
}
