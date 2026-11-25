///////////////////////////////////////////////////////////////////////////////
// Name:        testtagging.cpp
// Purpose:     Automated tests for tagged (accessible) PDF output
// Author:      wxPdfDocument contributors
// Created:     2026-11-23
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/*
 * The tests generate small uncompressed PDF documents and inspect the
 * generated PDF syntax directly: the document catalog, the structure tree,
 * the parent tree, the associated MathML files and the marked-content
 * operators in the page content streams.
 *
 * If the environment variable WXPDFDOC_TEST_OUTPUT names a directory, every
 * generated document is also saved there, so that it can be checked with
 * external validators (e.g. "qpdf --check" or veraPDF).
 *
 * The program returns 0 if all tests pass and 1 otherwise.
 */

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/init.h>
#include <wx/log.h>
#include <wx/mstream.h>

#include <cstdio>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "wx/pdfdocument.h"
#include "wx/pdflayer.h"

// --- Minimal test framework

static int gs_failures = 0;
static int gs_checks = 0;
static std::string gs_currentTest;

#define CHECK(cond) \
  do { \
    ++gs_checks; \
    if (!(cond)) { \
      ++gs_failures; \
      fprintf(stderr, "%s:%d: [%s] check failed: %s\n", __FILE__, __LINE__, gs_currentTest.c_str(), #cond); \
    } \
  } while (0)

#define CHECK_EQUAL(expected, actual) \
  do { \
    ++gs_checks; \
    if (!((expected) == (actual))) { \
      ++gs_failures; \
      std::ostringstream os_; \
      os_ << "expected <" << (expected) << "> but got <" << (actual) << ">"; \
      fprintf(stderr, "%s:%d: [%s] %s == %s failed: %s\n", __FILE__, __LINE__, gs_currentTest.c_str(), \
              #expected, #actual, os_.str().c_str()); \
    } \
  } while (0)

// --- Helpers for inspecting generated PDF

/// Generated PDF document, with simple accessors for its objects
class PdfFile
{
public:
  explicit PdfFile(const std::string& data) : m_data(data)
  {
    // Split the file into indirect objects "n 0 obj ... endobj". Stream
    // contents are skipped using their /Length, as they may contain anything.
    size_t pos = 0;
    while ((pos = m_data.find(" 0 obj", pos)) != std::string::npos)
    {
      size_t start = m_data.rfind('\n', pos);
      start = (start == std::string::npos) ? 0 : start + 1;
      int id = atoi(m_data.substr(start, pos - start).c_str());
      size_t body = pos + 6;
      size_t end;
      size_t streamPos = m_data.find("stream\n", body);
      size_t endObj = m_data.find("endobj", body);
      if (streamPos != std::string::npos && streamPos < endObj && m_data.compare(streamPos - 3, 3, "end") != 0)
      {
        size_t length = (size_t) atol(m_data.c_str() + m_data.find("/Length ", body) + 8);
        m_streams[id] = m_data.substr(streamPos + 7, length);
        end = m_data.find("endobj", streamPos + 7 + length);
      }
      else
      {
        end = endObj;
      }
      m_objects[id] = m_data.substr(body, end - body);
      pos = end;
    }
  }

  const std::string& GetData() const { return m_data; }

  /// The dictionary part of an object (stream data excluded)
  std::string GetObject(int id) const
  {
    std::map<int, std::string>::const_iterator it = m_objects.find(id);
    if (it == m_objects.end()) return std::string();
    size_t streamPos = it->second.find("stream\n");
    return (streamPos == std::string::npos) ? it->second : it->second.substr(0, streamPos);
  }

  /// The (unfiltered) stream data of an object
  std::string GetStream(int id) const
  {
    std::map<int, std::string>::const_iterator it = m_streams.find(id);
    return (it == m_streams.end()) ? std::string() : it->second;
  }

  /// Ids of all objects whose dictionary contains the given text
  std::vector<int> FindObjects(const std::string& text) const
  {
    std::vector<int> ids;
    for (std::map<int, std::string>::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
      if (GetObject(it->first).find(text) != std::string::npos) ids.push_back(it->first);
    }
    return ids;
  }

  /// The single object containing the given text, 0 if none or several
  int FindObject(const std::string& text) const
  {
    std::vector<int> ids = FindObjects(text);
    return (ids.size() == 1) ? ids[0] : 0;
  }

  /// The catalog dictionary
  std::string GetCatalog() const { return GetObject(FindObject("/Type /Catalog")); }

  /// Ids of the page objects in page order (pages are written in order)
  std::vector<int> GetPages() const { return FindObjects("<</Type /Page\n"); }

  /// Content stream of a page (0-based index)
  std::string GetPageContent(size_t page) const
  {
    std::vector<int> pages = GetPages();
    if (page >= pages.size()) return std::string();
    return GetStream(RefAfter(GetObject(pages[page]), "/Contents "));
  }

  /// Object number of the indirect reference following a key
  static int RefAfter(const std::string& dict, const std::string& key)
  {
    size_t pos = dict.find(key);
    return (pos == std::string::npos) ? 0 : atoi(dict.c_str() + pos + key.length());
  }

  /// Decoded value of a text string entry (UTF-16BE with BOM, or ASCII)
  static wxString TextAfter(const std::string& dict, const std::string& key)
  {
    size_t pos = dict.find(key + "(");
    if (pos == std::string::npos) return wxS("<missing>");
    std::string bytes;
    for (size_t j = pos + key.length() + 1; j < dict.length() && dict[j] != ')'; ++j)
    {
      char c = dict[j];
      if (c == '\\')
      {
        c = dict[++j];
        switch (c)
        {
          case 'n': c = '\n'; break;
          case 'r': c = '\r'; break;
          case 't': c = '\t'; break;
          case 'b': c = '\b'; break;
          case 'f': c = '\f'; break;
          default: break;
        }
      }
      bytes += c;
    }
    if (bytes.length() >= 2 && bytes[0] == '\xfe' && bytes[1] == '\xff')
    {
      return wxString(bytes.data() + 2, wxMBConvUTF16BE(), bytes.length() - 2);
    }
    return wxString::FromAscii(bytes.c_str());
  }

private:
  std::string m_data;
  std::map<int, std::string> m_objects;
  std::map<int, std::string> m_streams;
};

/// Split content into whitespace separated tokens
static std::vector<std::string>
Tokens(const std::string& content)
{
  std::vector<std::string> tokens;
  std::istringstream is(content);
  std::string token;
  while (is >> token) tokens.push_back(token);
  return tokens;
}

/// Check that marked-content operators are balanced and never close more than was opened
static bool
IsMarkedContentBalanced(const std::string& content)
{
  int depth = 0;
  std::vector<std::string> tokens = Tokens(content);
  for (size_t j = 0; j < tokens.size(); ++j)
  {
    if (tokens[j] == "BDC" || tokens[j] == "BMC") ++depth;
    if (tokens[j] == "EMC" && --depth < 0) return false;
  }
  return depth == 0;
}

/// Count occurrences of a substring
static int
Count(const std::string& text, const std::string& what)
{
  int n = 0;
  for (size_t pos = text.find(what); pos != std::string::npos; pos = text.find(what, pos + 1)) ++n;
  return n;
}

static std::string
ToStd(const wxString& s)
{
  return std::string(s.utf8_str().data());
}

/// Fixed settings so that generated documents are reproducible
static void
PrepareDocument(wxPdfDocument& pdf)
{
  pdf.SetCompression(false);
  pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2026, 12, 0, 0));
  pdf.SetFont(wxS("Helvetica"), wxS(""), 12);
}

/// Close the document, optionally save it for external validation, and return it
static PdfFile
Finish(wxPdfDocument& pdf, const char* name)
{
  const wxMemoryOutputStream& mos = pdf.CloseAndGetBuffer();
  size_t len = mos.GetLength();
  std::string data(len, '\0');
  mos.CopyTo(&data[0], len);

  wxString outDir;
  if (wxGetEnv(wxS("WXPDFDOC_TEST_OUTPUT"), &outDir) && !outDir.IsEmpty())
  {
    wxFileName fileName(outDir, wxString::FromAscii(name) + wxS(".pdf"));
    FILE* f = fopen(fileName.GetFullPath().mb_str(), "wb");
    if (f != NULL)
    {
      fwrite(data.data(), 1, len, f);
      fclose(f);
    }
  }
  return PdfFile(data);
}

/// Captures error messages logged during a test
class CapturingLog : public wxLog
{
public:
  std::vector<wxString> m_messages;
protected:
  virtual void DoLogTextAtLevel(wxLogLevel level, const wxString& msg) wxOVERRIDE
  {
    if (level <= wxLOG_Warning) m_messages.push_back(msg);
  }
};

// --- Tests

/// Without SetTagged() nothing changes, even when the tagging methods are called
static void
TestUntaggedDocumentUnchanged()
{
  wxPdfDocument plain;
  PrepareDocument(plain);
  plain.AddPage();
  plain.Cell(0, 10, wxS("Hello"));
  PdfFile plainPdf = Finish(plain, "untagged-plain");

  wxPdfDocument calls;
  PrepareDocument(calls);
  calls.AddPage();
  calls.BeginTag(wxS("P"));
  CHECK_EQUAL(0, calls.GetTagDepth());
  calls.BeginArtifact();
  calls.Cell(0, 10, wxS("Hello"));
  calls.EndArtifact();
  calls.EndTag();
  PdfFile callsPdf = Finish(calls, "untagged-with-calls");

  CHECK(!calls.IsTagged());
  CHECK(plainPdf.GetData() == callsPdf.GetData());
  CHECK(plainPdf.GetData().find("/StructTreeRoot") == std::string::npos);
  CHECK(plainPdf.GetData().find("/MarkInfo") == std::string::npos);
  CHECK(plainPdf.GetData().find("BDC") == std::string::npos);
  CHECK(plainPdf.GetData().find("BMC") == std::string::npos);
  CHECK(plainPdf.GetData().compare(0, 8, "%PDF-1.3") == 0);
}

/// Catalog, structure tree, parent tree and marked content of a simple document
static void
TestBasicStructure()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  pdf.SetLanguage(wxS("en-US"));
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.Line(10, 10, 100, 10);           // not tagged: becomes an artifact
  pdf.BeginTag(wxS("H1"));
  pdf.Cell(0, 10, wxS("Heading"), 0, 1);
  pdf.EndTag();
  pdf.BeginTag(wxS("P"), wxEmptyString, wxS("A paragraph"));
  pdf.Cell(0, 10, wxS("Paragraph"), 0, 1);
  pdf.EndTag();
  PdfFile f = Finish(pdf, "basic-structure");

  CHECK(f.GetData().compare(0, 8, "%PDF-1.7") == 0);

  std::string catalog = f.GetCatalog();
  CHECK(catalog.find("/MarkInfo <</Marked true>>") != std::string::npos);
  CHECK(PdfFile::TextAfter(catalog, "/Lang ") == wxS("en-US"));
  int rootId = PdfFile::RefAfter(catalog, "/StructTreeRoot ");
  std::string root = f.GetObject(rootId);
  CHECK(root.find("/Type /StructTreeRoot") != std::string::npos);
  CHECK(root.find("/ParentTreeNextKey 1") != std::string::npos);

  // Document > H1, P
  int docId = PdfFile::RefAfter(root, "/K [");
  std::string doc = f.GetObject(docId);
  CHECK(doc.find("/S /Document") != std::string::npos);
  int h1Id = f.FindObject("/S /H1");
  int pId = f.FindObject("/S /P ");
  CHECK(h1Id > 0 && pId > 0);
  std::ostringstream kids;
  kids << "/K [" << h1Id << " 0 R " << pId << " 0 R ]";
  CHECK(doc.find(kids.str()) != std::string::npos);

  std::vector<int> pages = f.GetPages();
  CHECK_EQUAL(1u, pages.size());
  std::ostringstream pg;
  pg << "/Pg " << pages[0] << " 0 R";
  std::string h1 = f.GetObject(h1Id);
  CHECK(h1.find(pg.str()) != std::string::npos);
  CHECK(h1.find("/K [0 ]") != std::string::npos);
  CHECK(PdfFile::RefAfter(h1, "/P ") == docId);
  std::string p = f.GetObject(pId);
  CHECK(p.find("/K [1 ]") != std::string::npos);
  CHECK(PdfFile::TextAfter(p, "/ActualText ") == wxS("A paragraph"));

  std::string page = f.GetObject(pages[0]);
  CHECK(page.find("/StructParents 0") != std::string::npos);
  CHECK(page.find("/Tabs /S") != std::string::npos);

  // Parent tree maps MCID 0 -> H1, MCID 1 -> P on page 0
  std::string parentTree = f.GetObject(PdfFile::RefAfter(root, "/ParentTree "));
  std::ostringstream nums;
  nums << "0 [" << h1Id << " 0 R " << pId << " 0 R ]";
  CHECK(parentTree.find(nums.str()) != std::string::npos);

  // Content: artifact for the untagged line, then the two sequences
  std::string content = f.GetPageContent(0);
  CHECK(IsMarkedContentBalanced(content));
  size_t artifact = content.find("/Artifact BMC");
  size_t heading = content.find("/H1 <</MCID 0>> BDC");
  size_t headingText = content.find("(Heading)");
  size_t paragraph = content.find("/P <</MCID 1>> BDC");
  size_t paragraphText = content.find("(Paragraph)");
  CHECK(artifact != std::string::npos);
  CHECK(heading != std::string::npos && artifact < heading);
  CHECK(headingText != std::string::npos && heading < headingText);
  CHECK(paragraph != std::string::npos && headingText < paragraph);
  CHECK(paragraphText != std::string::npos && paragraph < paragraphText);
  CHECK_EQUAL(0, pdf.GetTagDepth());
}

/// Formula with alternate text, actual text and MathML as associated file
static void
TestFormulaWithMathML()
{
  const wxString mathML =
    wxS("<math xmlns=\"http://www.w3.org/1998/Math/MathML\"><msup><mi>x</mi><mn>2</mn></msup>")
    wxS("<mo>+</mo><mi>α</mi></math>");
  const wxString alt = wxS("x squared plus alpha");

  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.BeginFormula(alt, mathML, wxS("x^2+α"));
  CHECK_EQUAL(1, pdf.GetTagDepth());
  pdf.Text(20, 20, wxS("x"));
  pdf.Text(23, 17, wxS("2"));
  pdf.EndFormula();
  pdf.BeginTag(wxS("Formula"), wxS("no MathML here"));
  pdf.Text(20, 40, wxS("y"));
  pdf.EndTag();
  PdfFile f = Finish(pdf, "formula-mathml");

  int formulaId = f.FindObject("/S /Formula /P");
  std::vector<int> formulas = f.FindObjects("/S /Formula");
  CHECK_EQUAL(2u, formulas.size());
  formulaId = formulas.empty() ? 0 : formulas[0];
  std::string formula = f.GetObject(formulaId);
  CHECK(PdfFile::TextAfter(formula, "/Alt ") == alt);
  CHECK(PdfFile::TextAfter(formula, "/ActualText ") == wxS("x^2+α"));

  int fileSpecId = PdfFile::RefAfter(formula, "/AF [");
  CHECK(fileSpecId > 0);
  std::string fileSpec = f.GetObject(fileSpecId);
  CHECK(fileSpec.find("/Type /Filespec") != std::string::npos);
  CHECK(fileSpec.find("/AFRelationship /Supplement") != std::string::npos);
  CHECK(PdfFile::TextAfter(fileSpec, "/F ") == wxS("mathml-1.xml"));
  CHECK(PdfFile::TextAfter(fileSpec, "/UF ") == wxS("mathml-1.xml"));
  int embeddedId = PdfFile::RefAfter(fileSpec, "/EF <</F ");
  std::string embedded = f.GetObject(embeddedId);
  CHECK(embedded.find("/Type /EmbeddedFile") != std::string::npos);
  CHECK(embedded.find("/Subtype /application#2Fmathml+xml") != std::string::npos);
  CHECK_EQUAL(ToStd(mathML), f.GetStream(embeddedId));
  std::ostringstream size;
  size << "/Size " << ToStd(mathML).length() << ">>";
  CHECK(embedded.find(size.str()) != std::string::npos);

  // The second formula has no associated file
  std::string second = f.GetObject(formulas.size() > 1 ? formulas[1] : 0);
  CHECK(second.find("/AF") == std::string::npos);
  CHECK(PdfFile::TextAfter(second, "/Alt ") == wxS("no MathML here"));
  CHECK_EQUAL(1u, f.FindObjects("/Type /EmbeddedFile").size());

  std::string content = f.GetPageContent(0);
  CHECK(content.find("/Formula <</MCID 0>> BDC") != std::string::npos);
  CHECK(content.find("/Formula <</MCID 1>> BDC") != std::string::npos);
  CHECK(IsMarkedContentBalanced(content));
}

/// Nested structure elements
static void
TestNesting()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  CHECK_EQUAL(0, pdf.GetTagDepth());
  pdf.BeginTag(wxPdfTag(wxS("Sect")).SetTitle(wxS("Section 1")).SetLanguage(wxS("de")));
  CHECK_EQUAL(1, pdf.GetTagDepth());
  pdf.Cell(0, 10, wxS("Before"), 0, 1);
  pdf.BeginTag(wxPdfTag(wxS("Span")).SetExpansion(wxS("for example")));
  CHECK_EQUAL(2, pdf.GetTagDepth());
  pdf.Cell(0, 10, wxS("e.g."), 0, 1);
  pdf.EndTag();
  CHECK_EQUAL(1, pdf.GetTagDepth());
  pdf.Cell(0, 10, wxS("After"), 0, 1);
  pdf.EndTag();
  CHECK_EQUAL(0, pdf.GetTagDepth());
  PdfFile f = Finish(pdf, "nesting");

  int sectId = f.FindObject("/S /Sect");
  int spanId = f.FindObject("/S /Span");
  std::string sect = f.GetObject(sectId);
  std::string span = f.GetObject(spanId);
  // Reading order: MCID 0, the Span element, MCID 2
  std::ostringstream kids;
  kids << "/K [0 " << spanId << " 0 R 2 ]";
  CHECK(sect.find(kids.str()) != std::string::npos);
  CHECK(span.find("/K [1 ]") != std::string::npos);
  CHECK(PdfFile::RefAfter(span, "/P ") == sectId);
  CHECK(PdfFile::TextAfter(sect, "/T ") == wxS("Section 1"));
  CHECK(PdfFile::TextAfter(sect, "/Lang ") == wxS("de"));
  CHECK(PdfFile::TextAfter(span, "/E ") == wxS("for example"));

  std::string content = f.GetPageContent(0);
  CHECK(IsMarkedContentBalanced(content));
  CHECK(content.find("/Sect <</MCID 0>> BDC") < content.find("(Before)"));
  CHECK(content.find("/Span <</MCID 1>> BDC") < content.find("(e.g.)"));
  CHECK(content.find("/Sect <</MCID 2>> BDC") < content.find("(After)"));
}

/// A structure element spanning a page break gets a sequence on each page
static void
TestPageBreakInsideElement()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.BeginTag(wxS("P"));
  for (int j = 0; j < 40; ++j)
  {
    pdf.Cell(0, 10, wxString::Format(wxS("Line %d"), j), 0, 1);   // triggers an automatic page break
  }
  pdf.EndTag();
  PdfFile f = Finish(pdf, "page-break");

  std::vector<int> pages = f.GetPages();
  CHECK_EQUAL(2u, pages.size());
  if (pages.size() != 2) return;
  int pId = f.FindObject("/S /P ");
  std::string p = f.GetObject(pId);
  std::ostringstream expected;
  expected << "/Pg " << pages[0] << " 0 R\n/K [0 <</Type /MCR /Pg " << pages[1] << " 0 R /MCID 0>> ]";
  CHECK(p.find(expected.str()) != std::string::npos);

  CHECK(f.GetObject(pages[1]).find("/StructParents 1") != std::string::npos);
  std::string root = f.GetObject(PdfFile::RefAfter(f.GetCatalog(), "/StructTreeRoot "));
  CHECK(root.find("/ParentTreeNextKey 2") != std::string::npos);
  std::string parentTree = f.GetObject(PdfFile::RefAfter(root, "/ParentTree "));
  std::ostringstream nums;
  nums << "0 [" << pId << " 0 R ]\n1 [" << pId << " 0 R ]";
  CHECK(parentTree.find(nums.str()) != std::string::npos);

  for (size_t j = 0; j < pages.size(); ++j)
  {
    std::string content = f.GetPageContent(j);
    CHECK(IsMarkedContentBalanced(content));
    CHECK_EQUAL(1, Count(content, "/P <</MCID 0>> BDC"));
  }
}

/// Document with header and footer
class HeaderFooterDocument : public wxPdfDocument
{
public:
  virtual void Header() wxOVERRIDE
  {
    Cell(0, 10, wxS("Running header"), 0, 1);
  }
  virtual void Footer() wxOVERRIDE
  {
    SetY(-15);
    Cell(0, 10, wxString::Format(wxS("Page %d"), PageNo()));
  }
};

/// Page headers and footers are pagination artifacts, even inside an open element
static void
TestHeaderFooterArtifacts()
{
  HeaderFooterDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.BeginTag(wxS("P"));
  pdf.Cell(0, 10, wxS("Body text"), 0, 1);
  pdf.AddPage();                       // footer of page 1 and header of page 2 inside P
  pdf.Cell(0, 10, wxS("More body text"), 0, 1);
  pdf.EndTag();
  PdfFile f = Finish(pdf, "header-footer");

  for (size_t j = 0; j < 2; ++j)
  {
    std::string content = f.GetPageContent(j);
    CHECK(IsMarkedContentBalanced(content));
    size_t header = content.find("(Running header)");
    size_t footer = content.find(j == 0 ? "(Page 1)" : "(Page 2)");
    CHECK(header != std::string::npos && footer != std::string::npos);
    // The nearest marked-content operator before header and footer is a pagination artifact
    CHECK(content.rfind("BDC", header) == content.rfind("/Artifact <</Type /Pagination>> BDC", header) + 32);
    CHECK(content.rfind("BDC", footer) == content.rfind("/Artifact <</Type /Pagination>> BDC", footer) + 32);
  }
  CHECK(f.GetPageContent(0).find("/P <</MCID 0>> BDC") < f.GetPageContent(0).find("(Body text)"));
  CHECK(f.GetPageContent(1).find("/P <</MCID 0>> BDC") < f.GetPageContent(1).find("(More body text)"));
}

/// Explicit artifacts inside a structure element
static void
TestExplicitArtifact()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.BeginTag(wxS("P"));
  pdf.Cell(0, 10, wxS("Real content"), 0, 1);
  pdf.BeginArtifact();
  pdf.Rect(5, 5, 100, 30);
  pdf.EndArtifact();
  pdf.Cell(0, 10, wxS("More content"), 0, 1);
  pdf.EndTag();
  PdfFile f = Finish(pdf, "explicit-artifact");

  std::string content = f.GetPageContent(0);
  CHECK(IsMarkedContentBalanced(content));
  std::vector<std::string> tokens = Tokens(content);
  // Sequence of marked-content operators after the page set-up: P, EMC, Artifact, EMC, P, EMC
  std::vector<std::string> ops;
  for (size_t j = 0; j < tokens.size(); ++j)
  {
    if (tokens[j] == "/P" || (!ops.empty() && (tokens[j] == "/Artifact" || tokens[j] == "EMC")))
    {
      ops.push_back(tokens[j]);
    }
  }
  const char* expected[] = { "/P", "EMC", "/Artifact", "EMC", "/P", "EMC" };
  CHECK_EQUAL(6u, ops.size());
  for (size_t j = 0; j < ops.size() && j < 6; ++j)
  {
    CHECK_EQUAL(std::string(expected[j]), ops[j]);
  }
  std::string p = f.GetObject(f.FindObject("/S /P "));
  CHECK(p.find("/K [0 1 ]") != std::string::npos);
}

/// Layers and structure elements may interleave; operators stay properly nested
static void
TestLayerInterleaving()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  wxPdfLayer* layer = pdf.AddLayer(wxS("Layer"));
  pdf.AddPage();
  pdf.Cell(0, 10, wxS("Untagged"), 0, 1);
  pdf.BeginTag(wxS("P"));
  pdf.Cell(0, 10, wxS("Tagged outside layer"), 0, 1);
  pdf.EnterLayer(layer);
  pdf.Cell(0, 10, wxS("Tagged inside layer"), 0, 1);
  pdf.EndTag();
  pdf.Cell(0, 10, wxS("Untagged inside layer"), 0, 1);
  pdf.LeaveLayer();
  PdfFile f = Finish(pdf, "layers");

  std::string content = f.GetPageContent(0);
  CHECK(IsMarkedContentBalanced(content));
  // The layer's /OC sequence must enclose the P sequence opened inside it
  size_t oc = content.find("/OC /L1 BDC");
  CHECK(oc != std::string::npos);
  CHECK(content.find("/OC /L1 BDC\n/P <</MCID 1>> BDC", oc) == oc);
  std::string p = f.GetObject(f.FindObject("/S /P "));
  CHECK(p.find("/K [0 1 ]") != std::string::npos);
}

/// Unbalanced EndTag()/EndArtifact() are reported and ignored
static void
TestUnbalancedCalls()
{
  CapturingLog* log = new CapturingLog();
  wxLog* old = wxLog::SetActiveTarget(log);

  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.EndTag();
  pdf.EndArtifact();
  CHECK_EQUAL(0, pdf.GetTagDepth());
  pdf.BeginTag(wxS("P"));
  pdf.Cell(0, 10, wxS("Never closed"));
  PdfFile f = Finish(pdf, "unbalanced");

  wxLog::SetActiveTarget(old);
  CHECK_EQUAL(2u, log->m_messages.size());
  delete log;

  // The open element is still written; the content stream is balanced
  CHECK(f.FindObject("/S /P ") > 0);
  CHECK(IsMarkedContentBalanced(f.GetPageContent(0)));
}

/// Structure type names are written as valid PDF names
static void
TestNameEscaping()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  PrepareDocument(pdf);
  pdf.AddPage();
  pdf.BeginTag(wxS("My Type#1"));
  pdf.Cell(0, 10, wxS("Custom"));
  pdf.EndTag();
  pdf.BeginTag(wxEmptyString);
  pdf.Cell(0, 10, wxS("Default"));
  pdf.EndTag();
  PdfFile f = Finish(pdf, "name-escaping");

  CHECK(f.FindObject("/S /My#20Type#231") > 0);
  CHECK(f.FindObject("/S /Span") > 0);
  CHECK(f.GetPageContent(0).find("/My#20Type#231 <</MCID 0>> BDC") != std::string::npos);
}

/// The document language does not require tagging
static void
TestLanguageWithoutTagging()
{
  wxPdfDocument pdf;
  PrepareDocument(pdf);
  pdf.SetLanguage(wxS("fr"));
  CHECK(pdf.GetLanguage() == wxS("fr"));
  pdf.AddPage();
  PdfFile f = Finish(pdf, "language-only");
  std::string catalog = f.GetCatalog();
  CHECK(PdfFile::TextAfter(catalog, "/Lang ") == wxS("fr"));
  CHECK(catalog.find("/MarkInfo") == std::string::npos);
}

/// Tagging works together with compression and encryption
static void
TestCompressedAndEncrypted()
{
  wxPdfDocument pdf;
  pdf.SetTagged();
  pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2026, 12, 0, 0));
  pdf.SetProtection(wxPDF_PERMISSION_PRINT | wxPDF_PERMISSION_EXTRACT, wxS(""), wxS("owner"));
  pdf.SetFont(wxS("Helvetica"), wxS(""), 12);
  pdf.AddPage();
  pdf.BeginFormula(wxS("a plus b"), wxS("<math><mi>a</mi><mo>+</mo><mi>b</mi></math>"));
  pdf.Cell(0, 10, wxS("a + b"));
  pdf.EndFormula();
  PdfFile f = Finish(pdf, "compressed-encrypted");

  CHECK(f.GetCatalog().find("/StructTreeRoot") != std::string::npos);
  CHECK(f.FindObject("/S /Formula") > 0);
  CHECK(f.FindObject("/Subtype /application#2Fmathml+xml /Filter /FlateDecode") > 0);
}

// --- Main

typedef void (*TestFunction)();

struct TestCase
{
  const char*  m_name;
  TestFunction m_function;
};

int
main(int argc, char** argv)
{
  wxInitializer initializer(argc, argv);
  if (!initializer.IsOk())
  {
    fprintf(stderr, "Failed to initialize wxWidgets.\n");
    return 1;
  }

  static const TestCase tests[] =
  {
    { "UntaggedDocumentUnchanged", TestUntaggedDocumentUnchanged },
    { "BasicStructure",            TestBasicStructure },
    { "FormulaWithMathML",         TestFormulaWithMathML },
    { "Nesting",                   TestNesting },
    { "PageBreakInsideElement",    TestPageBreakInsideElement },
    { "HeaderFooterArtifacts",     TestHeaderFooterArtifacts },
    { "ExplicitArtifact",          TestExplicitArtifact },
    { "LayerInterleaving",         TestLayerInterleaving },
    { "UnbalancedCalls",           TestUnbalancedCalls },
    { "NameEscaping",              TestNameEscaping },
    { "LanguageWithoutTagging",    TestLanguageWithoutTagging },
    { "CompressedAndEncrypted",    TestCompressedAndEncrypted },
  };

  for (size_t j = 0; j < sizeof(tests) / sizeof(tests[0]); ++j)
  {
    gs_currentTest = tests[j].m_name;
    int failuresBefore = gs_failures;
    tests[j].m_function();
    printf("%-28s %s\n", tests[j].m_name, (gs_failures == failuresBefore) ? "ok" : "FAILED");
  }
  printf("%d checks, %d failures\n", gs_checks, gs_failures);
  return (gs_failures == 0) ? 0 : 1;
}
