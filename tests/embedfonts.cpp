///////////////////////////////////////////////////////////////////////////////
// Name:        embedfonts.cpp
// Purpose:     Automated tests for font embedding in PDF/A documents
//              (all fonts are embedded, non-embeddable ones are replaced via
//              the embeddable font replacements of wxPdfFontManager)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// The tests create small PDF documents in memory with compression disabled,
// so that the font dictionaries can be inspected as plain text:
//
//  - a font that is not embedded is written as "/BaseFont /Helvetica" without
//    a font descriptor referencing font data;
//  - embedded fonts reference their font program via /FontFile (Type1),
//    /FontFile2 (TrueType) or /FontFile3 (OpenType/CFF).
//
// Tests depending on fonts installed on the system are skipped (with a
// message) if those fonts are missing. The program returns 0 if all checks
// pass and 1 otherwise. The directory containing the test fonts shipped with
// wxPdfDocument (lib/fonts) can be given as the first argument.

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/log.h>
#include <wx/mstream.h>
#include <wx/filename.h>

#include "wx/pdfdocument.h"
#include "wx/pdffont.h"
#include "wx/pdffontmanager.h"

#include <cstdio>
#include <functional>
#include <vector>

#ifndef WXPDFDOC_TEST_FONTDIR
#define WXPDFDOC_TEST_FONTDIR "../lib/fonts"
#endif

namespace
{

int gs_failures = 0;
int gs_checks = 0;

#define CHECK(cond) \
  do { \
    ++gs_checks; \
    if (!(cond)) { \
      ++gs_failures; \
      std::fprintf(stderr, "  FAILED: %s (%s:%d)\n", #cond, __FILE__, __LINE__); \
    } \
  } while (0)

/// Log target collecting warnings and errors, so that tests can check them
class CollectingLog : public wxLog
{
public:
  wxArrayString m_messages;

protected:
  virtual void DoLogTextAtLevel(wxLogLevel level, const wxString& msg)
  {
    if (level <= wxLOG_Warning)
    {
      m_messages.Add(msg);
    }
  }
};

/// Count the messages containing a given text
int
CountMessages(const CollectingLog& log, const wxString& text)
{
  int count = 0;
  for (size_t j = 0; j < log.m_messages.GetCount(); ++j)
  {
    if (log.m_messages[j].Find(text) != wxNOT_FOUND)
    {
      ++count;
    }
  }
  return count;
}

/// Create a one page document, let the test fill it, and return the PDF as text
wxString
RenderPdf(const std::function<void(wxPdfDocument&)>& fill)
{
  wxPdfDocument pdf;
  pdf.SetCompression(false);
  fill(pdf);
  const wxMemoryOutputStream& out = pdf.CloseAndGetBuffer();
  size_t len = out.GetLength();
  std::vector<char> buffer(len);
  out.CopyTo(buffer.data(), len);
  // Latin-1 keeps every byte, binary font data does not disturb the search
  return wxString(buffer.data(), wxConvISO8859_1, len);
}

bool
Contains(const wxString& pdf, const wxString& text)
{
  return pdf.Find(text) != wxNOT_FOUND;
}

/// Check whether the PDF contains any embedded font program
bool
HasEmbeddedFont(const wxString& pdf)
{
  return Contains(pdf, wxS("/FontFile"));
}

/// Count how many font programs are embedded in the PDF
int
CountEmbeddedFonts(const wxString& pdf)
{
  int count = 0;
  size_t pos = pdf.find(wxS("/FontFile"));
  while (pos != wxString::npos)
  {
    ++count;
    pos = pdf.find(wxS("/FontFile"), pos + 1);
  }
  return count;
}

/// Check whether the PDF uses one of the 14 standard fonts (never embedded)
bool
HasStandardFont(const wxString& pdf, const wxString& name)
{
  return Contains(pdf, wxS("/BaseFont /") + name + wxS("\n")) ||
         Contains(pdf, wxS("/BaseFont /") + name + wxS("\r")) ||
         Contains(pdf, wxS("/BaseFont /") + name + wxS("/"));
}

/// Name of the family of the TrueType test font shipped with wxPdfDocument
wxString gs_testFontFamily;

/// Restores global font manager state changed by a test
class ReplacementGuard
{
public:
  ReplacementGuard(const wxString& family, const wxString& replacement)
    : m_family(family)
  {
    m_previous = wxPdfFontManager::GetFontManager()->GetEmbeddableFontReplacement(family);
    wxPdfFontManager::GetFontManager()->SetEmbeddableFontReplacement(family, replacement);
  }
  ~ReplacementGuard()
  {
    wxPdfFontManager::GetFontManager()->SetEmbeddableFontReplacement(m_family, m_previous);
  }

private:
  wxString m_family;
  wxString m_previous;
};

// --- Tests

void
TestDefaultModeLeavesStandardFontsAlone()
{
  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.AddPage();
    doc.SetFont(wxS("Helvetica"), wxS(""), 12);
    doc.Cell(0, 10, wxS("Hello"));
  });
  CHECK(HasStandardFont(pdf, wxS("Helvetica")));
  CHECK(!HasEmbeddedFont(pdf));
}

void
TestReplacementRegistry()
{
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  CHECK(fm->GetEmbeddableFontReplacement(wxS("NoSuchFamily")).IsEmpty());
  fm->SetEmbeddableFontReplacement(wxS("NoSuchFamily"), wxS("Some Family"));
  // Family names are case insensitive
  CHECK(fm->GetEmbeddableFontReplacement(wxS("nosuchfamily")) == wxS("Some Family"));
  fm->SetEmbeddableFontReplacement(wxS("NOSUCHFAMILY"), wxEmptyString);
  CHECK(fm->GetEmbeddableFontReplacement(wxS("NoSuchFamily")).IsEmpty());
}

void
TestEmbeddableFontIsReturnedUnchanged()
{
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  wxPdfFont font = fm->GetFont(gs_testFontFamily);
  CHECK(font.IsValid());
  wxPdfFont embeddable = fm->GetEmbeddableFont(font);
  CHECK(embeddable.IsValid());
  CHECK(embeddable.GetName() == font.GetName());
  CHECK(embeddable.GetEmbed());
}

void
TestExplicitReplacementOfStandardFont()
{
  ReplacementGuard guard(wxS("Times"), gs_testFontFamily);
  wxPdfFont replacement = wxPdfFontManager::GetFontManager()->GetEmbeddableFont(wxS("Times"));
  CHECK(replacement.IsValid());
  CHECK(replacement.GetFamily() == gs_testFontFamily);
  // TestPdfA1ImpliesEmbeddingAllFonts() checks that a document uses it
}

void
TestFontWithDisabledEmbeddingGetsEmbedded()
{
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  wxPdfFont type1 = fm->RegisterFont(wxS("urw-nimbus-cyr.pfb"));
  if (!type1.IsValid())
  {
    type1 = fm->GetFont(wxS("NimbusSanL-Regu"));
  }
  CHECK(type1.IsValid());
  CHECK(type1.EmbedSupported());
  CHECK(!type1.EmbedRequired());
  type1.SetEmbed(false);
  CHECK(!type1.GetEmbed());

  // Outside PDF/A the user's choice is respected ...
  wxString pdf = RenderPdf([&type1](wxPdfDocument& doc)
  {
    doc.AddPage();
    doc.SetFont(type1, wxPDF_FONTSTYLE_REGULAR, 12);
    doc.Cell(0, 10, wxS("Hello"));
  });
  CHECK(!HasEmbeddedFont(pdf));

  // ... PDF/A forbids non-embedded fonts, so there it is embedded nevertheless
  pdf = RenderPdf([&type1](wxPdfDocument& doc)
  {
    doc.SetPdfA1Conformance(true);
    doc.AddPage();
    doc.SetFont(type1, wxPDF_FONTSTYLE_REGULAR, 12);
    doc.Cell(0, 10, wxS("Hello"));
  });
  CHECK(Contains(pdf, wxS("/FontFile ")));
}

void
TestFontWithoutReplacementIsWarnedAboutOnce()
{
  CollectingLog* log = new CollectingLog();
  wxLog* oldLog = wxLog::SetActiveTarget(log);

  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.SetPdfA1Conformance(true);
    CHECK(doc.AddFontCJK(wxS("SJIS")));
    doc.AddPage();
    // Adobe CJK fonts can't be embedded, and no replacement is defined
    doc.SetFont(wxS("SJIS"), wxS(""), 12);
    doc.Cell(0, 10, wxS("abc"));
    doc.SetFont(wxS("SJIS"), wxS(""), 14);
    doc.Cell(0, 10, wxS("abc"));
  });

  wxLog::SetActiveTarget(oldLog);
  // The font is still used, just not embedded
  CHECK(!HasEmbeddedFont(pdf));
  CHECK(CountMessages(*log, wxS("can't be embedded")) == 1);
  delete log;
}

void
TestStandardFontWithoutReplacementInPdfA1IsWarnedAbout()
{
  CollectingLog* log = new CollectingLog();
  wxLog* oldLog = wxLog::SetActiveTarget(log);

  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.SetPdfA1Conformance(true);
    doc.AddPage();
    // Symbol has a font specific encoding, so there is no built-in replacement
    doc.SetFont(wxS("Symbol"), wxS(""), 12);
    doc.Cell(0, 10, wxS("abc"));
    doc.SetFont(wxS("Symbol"), wxS(""), 14);
    doc.Cell(0, 10, wxS("abc"));
  });

  wxLog::SetActiveTarget(oldLog);
  CHECK(HasStandardFont(pdf, wxS("Symbol")));
  CHECK(CountMessages(*log, wxS("'Symbol'")) == 1);
  delete log;
}

void
TestPdfA1ImpliesEmbeddingAllFonts()
{
  ReplacementGuard guard(wxS("Times"), gs_testFontFamily);
  CollectingLog* log = new CollectingLog();
  wxLog* oldLog = wxLog::SetActiveTarget(log);

  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.SetPdfA1Conformance(true);
    doc.AddPage();
    doc.SetFont(wxS("Times"), wxS(""), 12);
    doc.Cell(0, 10, wxS("Hello"));
  });

  wxLog::SetActiveTarget(oldLog);
  CHECK(!HasStandardFont(pdf, wxS("Times-Roman")));
  CHECK(Contains(pdf, wxS("/FontFile2")));
  // No "core font not allowed" error
  CHECK(log->m_messages.IsEmpty());
  delete log;
}

/// PDF/A forbids non-embedded fonts, so standard fonts are replaced automatically.
/// Uses the operating system's fonts, so it is skipped if none of the
/// built-in replacements of Helvetica is installed.
bool
TestAutomaticReplacementOfHelvetica()
{
  wxPdfFont replacement = wxPdfFontManager::GetFontManager()->GetEmbeddableFont(wxS("Helvetica"));
  if (!replacement.IsValid())
  {
    std::printf("  SKIPPED: no embeddable replacement for Helvetica installed\n");
    return false;
  }
  std::printf("  Helvetica is replaced by '%s'\n", (const char*) replacement.GetFamily().mb_str());
  CHECK(replacement.EmbedSupported() || replacement.EmbedRequired());

  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.SetPdfA1Conformance(true);
    doc.AddPage();
    doc.SetFont(wxS("Helvetica"), wxS(""), 12);
    doc.Cell(0, 10, wxS("Regular"));
    // Change only the style: the current family is the replacement family now
    doc.SetFont(wxS(""), wxS("B"));
    doc.Cell(0, 10, wxS("Bold"));
    // "Arial" is an alias of the standard font Helvetica
    doc.SetFont(wxS("Arial"), wxS("I"), 12);
    doc.Cell(0, 10, wxS("Italic"));
  });
  CHECK(!HasStandardFont(pdf, wxS("Helvetica")));
  CHECK(!HasStandardFont(pdf, wxS("Helvetica-Bold")));
  CHECK(!HasStandardFont(pdf, wxS("Helvetica-Oblique")));
  // Regular, bold and italic are three different embedded fonts
  CHECK(CountEmbeddedFonts(pdf) == 3);
  return true;
}

} // anonymous namespace

int
main(int argc, char** argv)
{
  wxInitializer initializer(argc, argv);
  if (!initializer.IsOk())
  {
    std::fprintf(stderr, "Failed to initialize wxWidgets.\n");
    return 1;
  }
  // Don't show message boxes or timestamps for log messages
  wxLog::SetActiveTarget(new wxLogStderr());
  wxLog::DisableTimestamp();

  wxString fontDir(WXPDFDOC_TEST_FONTDIR);
  if (argc > 1)
  {
    fontDir = wxString(argv[1]);
  }
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  fm->AddSearchPath(fontDir);

  wxPdfFont testFont = fm->RegisterFont(wxS("TexturaModern.ttf"));
  if (!testFont.IsValid())
  {
    std::fprintf(stderr, "Test font TexturaModern.ttf not found in '%s'.\n", (const char*) fontDir.mb_str());
    return 1;
  }
  gs_testFontFamily = testFont.GetFamily();

  struct
  {
    const char* name;
    void (*test)();
  } tests[] =
  {
    { "DefaultModeLeavesStandardFontsAlone",   TestDefaultModeLeavesStandardFontsAlone },
    { "ReplacementRegistry",                   TestReplacementRegistry },
    { "EmbeddableFontIsReturnedUnchanged",     TestEmbeddableFontIsReturnedUnchanged },
    { "ExplicitReplacementOfStandardFont",     TestExplicitReplacementOfStandardFont },
    { "FontWithDisabledEmbeddingGetsEmbedded", TestFontWithDisabledEmbeddingGetsEmbedded },
    { "FontWithoutReplacementIsWarnedAboutOnce", TestFontWithoutReplacementIsWarnedAboutOnce },
    { "StandardFontWithoutReplacementInPdfA1IsWarnedAbout", TestStandardFontWithoutReplacementInPdfA1IsWarnedAbout },
    { "PdfA1ImpliesEmbeddingAllFonts",         TestPdfA1ImpliesEmbeddingAllFonts },
  };
  for (size_t j = 0; j < sizeof(tests) / sizeof(tests[0]); ++j)
  {
    std::printf("%s\n", tests[j].name);
    tests[j].test();
  }
  std::printf("AutomaticReplacementOfHelvetica\n");
  TestAutomaticReplacementOfHelvetica();

  std::printf("%d checks, %d failures\n", gs_checks, gs_failures);
  return (gs_failures == 0) ? 0 : 1;
}
