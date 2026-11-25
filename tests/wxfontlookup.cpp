///////////////////////////////////////////////////////////////////////////////
// Name:        wxfontlookup.cpp
// Purpose:     Automated tests for mapping wxFont objects to PDF fonts
//              (wxPdfFontManager::GetFont(const wxFont&), used by
//              wxPdfDocument::SetFont(const wxFont&), wxPdfDC and
//              wxPdfGraphicsContext)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// wxPdfDocument registers the 14 Adobe standard fonts under their family names
// and aliases (Helvetica alias Arial, Times, Courier, Symbol, ZapfDingbats).
// A wxFont with such a face name stands for the font the system renders on
// screen, so it has to be mapped to the installed font - which is embedded and
// covers Unicode - and not to the standard font, which is never embedded and
// covers only the WinAnsi character set (Greek letters would be lost).
//
// The tests create small PDF documents in memory with compression disabled,
// so that the font dictionaries can be inspected as plain text. Tests that
// need fonts located via the operating system are skipped with a message if
// that isn't possible on the platform. The program returns 0 if all checks
// pass and 1 otherwise.

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/log.h>
#include <wx/mstream.h>

#include "wx/pdfdocument.h"
#include "wx/pdffont.h"
#include "wx/pdffontmanager.h"

#include <cstdio>
#include <functional>
#include <vector>

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

/// Check whether the PDF uses one of the 14 standard fonts (never embedded)
bool
HasStandardFont(const wxString& pdf, const wxString& name)
{
  return Contains(pdf, wxS("/BaseFont /") + name + wxS("\n")) ||
         Contains(pdf, wxS("/BaseFont /") + name + wxS("\r")) ||
         Contains(pdf, wxS("/BaseFont /") + name + wxS("/"));
}

wxFont
MakeFont(const wxString& faceName, bool bold = false, bool italic = false)
{
  // An explicit size avoids querying the GUI's default font, which would
  // require a display connection on wxGTK
  wxFontInfo info(12);
  info.FaceName(faceName);
  if (bold)
  {
    info.Bold();
  }
  if (italic)
  {
    info.Italic();
  }
  return wxFont(info);
}

bool
IsStandardFont(const wxPdfFont& font)
{
  return font.GetType().IsSameAs(wxS("core"));
}

// --- Tests

/// Whether the operating system can provide a font for the face name "Arial".
/// Determined by the first test; the others are skipped if it can't.
bool gs_systemFontsAvailable = false;

void
TestStandardFontFaceNameMapsToInstalledFont()
{
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  // Precondition: by name, "Arial" is the standard font Helvetica
  CHECK(IsStandardFont(fm->GetFont(wxS("Arial"))));

  wxPdfFont font = fm->GetFont(MakeFont(wxS("Arial")));
  CHECK(font.IsValid());
#if defined(__WXGTK__)
  // Fontconfig always provides a font (for Arial usually Liberation Sans)
  CHECK(!IsStandardFont(font));
#endif
  if (!font.IsValid() || IsStandardFont(font))
  {
    std::printf("  SKIPPED: the operating system provides no font for 'Arial'\n");
    return;
  }
  gs_systemFontsAvailable = true;
  std::printf("  'Arial' is mapped to '%s'\n", (const char*) font.GetName().mb_str());
  CHECK(font.EmbedSupported() || font.EmbedRequired());
}

void
TestStandardFontFaceNamesAreEmbeddedWithUnicode()
{
  if (!gs_systemFontsAvailable)
  {
    std::printf("  SKIPPED\n");
    return;
  }
  CollectingLog* log = new CollectingLog();
  wxLog* oldLog = wxLog::SetActiveTarget(log);

  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.AddPage();
    const wxString faces[] = { wxS("Arial"), wxS("Helvetica"), wxS("Times"), wxS("Courier") };
    for (size_t j = 0; j < sizeof(faces) / sizeof(faces[0]); ++j)
    {
      CHECK(doc.SetFont(MakeFont(faces[j])));
      doc.Cell(0, 10, faces[j] + wxString::FromUTF8(": \xce\xb1\xce\xb2\xce\xb3"), 0, 1);
    }
    CHECK(doc.SetFont(MakeFont(wxS("Arial"), true, true)));
    doc.Cell(0, 10, wxS("Bold italic"), 0, 1);
  });

  wxLog::SetActiveTarget(oldLog);
  CHECK(!HasStandardFont(pdf, wxS("Helvetica")));
  CHECK(!HasStandardFont(pdf, wxS("Helvetica-BoldOblique")));
  CHECK(!HasStandardFont(pdf, wxS("Times-Roman")));
  CHECK(!HasStandardFont(pdf, wxS("Courier")));
  CHECK(Contains(pdf, wxS("/FontFile")));
  // No "family alias already assigned" errors or similar
  CHECK(log->m_messages.IsEmpty());
  for (size_t j = 0; j < log->m_messages.GetCount(); ++j)
  {
    std::fprintf(stderr, "  logged: %s\n", (const char*) log->m_messages[j].mb_str());
  }
  delete log;
}

void
TestLookupIsCached()
{
  if (!gs_systemFontsAvailable)
  {
    std::printf("  SKIPPED\n");
    return;
  }
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  wxPdfFont first = fm->GetFont(MakeFont(wxS("Times")));
  size_t count = fm->GetFontCount();
  wxPdfFont second = fm->GetFont(MakeFont(wxS("Times")));
  CHECK(first.GetName() == second.GetName());
  CHECK(fm->GetFontCount() == count);
}

void
TestNameBasedSelectionIsUnchanged()
{
  // Selecting by family name still means the standard font
  wxString pdf = RenderPdf([](wxPdfDocument& doc)
  {
    doc.AddPage();
    doc.SetFont(wxS("Helvetica"), wxS(""), 12);
    doc.Cell(0, 10, wxS("Hello"));
    doc.SetFont(wxS("Arial"), wxS("B"), 12);
    doc.Cell(0, 10, wxS("Hello"));
  });
  CHECK(HasStandardFont(pdf, wxS("Helvetica")));
  CHECK(HasStandardFont(pdf, wxS("Helvetica-Bold")));
  CHECK(!Contains(pdf, wxS("/FontFile")));
}

void
TestOtherFaceNamesAreUnchanged()
{
  if (!gs_systemFontsAvailable)
  {
    std::printf("  SKIPPED\n");
    return;
  }
  // A face name that isn't a standard font is registered under that name, as before
  wxPdfFontManager* fm = wxPdfFontManager::GetFontManager();
  wxPdfFont font = fm->GetFont(MakeFont(wxS("DejaVu Sans")));
  CHECK(font.IsValid());
  CHECK(!IsStandardFont(font));
  CHECK(fm->GetFont(wxS("DejaVu Sans")).IsValid());
}

void
TestInvalidFont()
{
  CHECK(!wxPdfFontManager::GetFontManager()->GetFont(wxFont()).IsValid());
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

  struct
  {
    const char* name;
    void (*test)();
  } tests[] =
  {
    { "StandardFontFaceNameMapsToInstalledFont",  TestStandardFontFaceNameMapsToInstalledFont },
    { "StandardFontFaceNamesAreEmbeddedWithUnicode", TestStandardFontFaceNamesAreEmbeddedWithUnicode },
    { "LookupIsCached",                           TestLookupIsCached },
    { "NameBasedSelectionIsUnchanged",            TestNameBasedSelectionIsUnchanged },
    { "OtherFaceNamesAreUnchanged",               TestOtherFaceNamesAreUnchanged },
    { "InvalidFont",                              TestInvalidFont },
  };
  for (size_t j = 0; j < sizeof(tests) / sizeof(tests[0]); ++j)
  {
    std::printf("%s\n", tests[j].name);
    tests[j].test();
  }

  std::printf("%d checks, %d failures\n", gs_checks, gs_failures);
  return (gs_failures == 0) ? 0 : 1;
}
