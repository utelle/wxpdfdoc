///////////////////////////////////////////////////////////////////////////////
// Name:        showfont.cpp
// Purpose:     Utility for creating a font sample
// Author:      Ulrich Telle
// Created:     2011-01-04
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

#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/log.h>

#include "wx/pdfdoc.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffont.h"
#include "wx/pdffontmanager.h"
#include "wx/pdffontparsertruetype.h"
#include "wx/pdffontdata.h"
#include "wx/pdffontdataopentype.h"
#include "wx/pdffontdatatruetype.h"
#include "wx/pdffontdatatype1.h"

#include "unicodeblocks.h"
#include "unicoderanges.h"

// Dimensions given in millimeters
const int A4_WIDTH     = 210;
const int A4_HEIGHT    = 298;
const int CELL_WIDTH   = 10;
const int CELL_HEIGHT  = 15;
const int TOP_MARGIN   = 30;
const int TABLE_HEIGHT = CELL_HEIGHT * 16;

class Range
{
public:
  wxUint32 first;
  wxUint32 last;
  bool     include;
  Range*   next;
};

class ShowFont : public wxAppConsole
{
public:
  /// Initialize application
  bool OnInit();

  /// Run application
  int OnRun();

  /// Terminate application
  int OnExit();

protected:
  /// Add code range for inclusion or exclusion
  bool AddRange(const wxString& range, bool include);

  /// Process sample font
  bool ProcessFont(const wxString& fontFileName);

  /// Initialize fonts used for headers and table descriptions
  void InitFonts();

  /// Draw all character codes of the requested font
  void DrawCharCodes();

  /// Locate unicode block that contains given character code.
  const UnicodeBlock* FindUnicodeBlock(wxUint32 charCode);

  /// Draw table for all character codes in the given Unicode block
  /**
  * \param charIndex index of the current character code,
  *                  on return the last character code in the given block
  * \param block the current Unicode block
  */
  void DrawUnicodeBlock(size_t& charIndex, const UnicodeBlock* block);

  /// Draw header of a page
  void DrawPageHeader(const wxString& blockName);

  /// Fill empty cell
  /**
  * The fill colour depends on the character code properties
  */
  void FillEmptyCell(double x, double y, wxUint32 charCode);

  /// Check whether character code is defined in Unicode
  bool IsDefined(wxUint32 charCode);

  /// Check whether a character code belongs to the given Unicode block
  bool IsInBlock(wxUint32 charCode, const UnicodeBlock* block);

  /// Draw label with character code.
  void DrawCharCode(double x, double y, wxUint32 charCode);

  /// Draw table grid with row and column numbers
  void DrawTableGrid(wxUint32 x_cells, wxUint32 block_start);

  /// Check whether a character code belongs to the requested output range
  bool IsInRange(wxUint32 charCode);

  /// Determine the left origin of a cell
  static double LeftCellOrigin(double xMin, int cellPos);

  /// Determine the top origin of a cell
  static double TopCellOrigin(int cellPos);

private:
  wxString         m_version;     ///< Version of the font sampler
  wxString         m_fontFile;    ///< File name of the requested font
  wxString         m_outputFile;  ///< Name of the output file
  bool             m_hasEncoding; ///< Flag whether an encoding was specified
  wxString         m_encoding;    ///< Name of specified encoding
  long             m_fontIndex;   ///< Index of the requested font within a font collection
  wxString         m_includes;    ///< Include ranges
  wxString         m_excludes;    ///< Exclude ranges
  Range*           m_ranges;      ///< Character code ranges for inclusion or exclusion
  Range*           m_lastRange;   ///< Pointer to last range added

  wxString         m_fontName;    ///< Name of the requested font
  wxPdfFont        m_sampleFont;  ///< Requested font
  double           m_fontSize;    ///< Font size
  wxPdfArrayUint32 m_cids;        ///< List of Unicode characters supported by the font
  wxPdfDocument*   m_pdf;         ///< PDF document

  wxPdfFont        m_headerFont;       ///< Font used for Unicode block descriptions
  wxPdfFont        m_fontNameFont;     ///< Font used for font name
  wxPdfFont        m_tableNumbersFont; ///< Font used for numbers in table headers
  wxPdfFont        m_cellNumbersFont;  ///< Font used for numbers in cells
};

bool
ShowFont::AddRange(const wxString& range, bool include)
{
  bool ok = false;
  Range* r;
  unsigned long first = 0, last = 0xffffffff;
  wxString tail = range;
  wxString head, rlo, rhi;
  do
  {
    head = tail.BeforeFirst(wxS(','));
    tail = tail.AfterFirst(wxS(','));
    rlo = head.BeforeFirst(wxS('-'));
    rhi = head.AfterFirst(wxS('-'));
    bool isRange = head.Find(wxS('-')) != wxNOT_FOUND;
    if (!rlo.IsEmpty())
    {
      ok = rlo.ToULong(&first,0);
    }
    else
    {
      ok = isRange;
    }
    if (ok)
    {
      if (!rhi.IsEmpty())
      {
        ok = rhi.ToULong(&last,0);
      }
      else
      {
        if (!isRange)
        {
          last = first;
        }
      }
      if (ok && first <= last)
      {
        r = new Range();
        r->first = first;
        r->last = last;
        r->include = include;
        r->next = NULL;
        if (m_ranges)
        {
          m_lastRange->next = r;
        }
        else
        {
          m_ranges = r;
        }
        m_lastRange = r;
      }
      else
      {
        ok = false;
      }
    }
  }
  while (ok && !tail.IsEmpty());

  return ok;
}

bool
ShowFont::IsInRange(wxUint32 charCode)
{
  bool inRange = (m_ranges != NULL) ? (!m_ranges->include) : true;
  Range* r;
  for (r = m_ranges; r != NULL; r = r->next)
  {
    if ((charCode >= r->first) && (charCode <= r->last))
    {
      inRange = r->include;
    }
  }
  return inRange;
}

bool
ShowFont::ProcessFont(const wxString& fontFileName)
{
  m_sampleFont = wxPdfFontManager::GetFontManager()->RegisterFont(m_fontFile, wxS("SampleFont"), m_fontIndex);
  bool ok = m_sampleFont.IsValid();
  if (ok)
  {
    m_fontName = m_sampleFont.GetName();
    if (m_hasEncoding && m_sampleFont.GetType().IsSameAs(wxS("Type1")))
    {
      ok = m_sampleFont.SetEncoding(m_encoding);
      if (!ok)
      {
        wxLogMessage(wxS("Error: Invalid encoding '") + m_encoding + wxS("'."));
      }
    }
    if (ok && !m_sampleFont.GetSupportedUnicodeCharacters(m_cids))
    {
      wxLogMessage(wxS("Error: Unable to determine supported Unicode character list."));
      ok = false;
    }
  }
  else
  {
    wxLogMessage(wxS("Error: Unable to read font file '") + fontFileName + wxS("'."));
  }
  if (ok)
  {
    double ascent = m_sampleFont.GetDescription().GetAscent();
    double descent = m_sampleFont.GetDescription().GetDescent();
    m_fontSize = (int) (0.85 * (CELL_WIDTH / 25.4 * 72.) / ((ascent-descent) / 1000.));
  }
  return ok;
}

void
ShowFont::InitFonts()
{
  wxPdfFontManager* fontManager = wxPdfFontManager::GetFontManager();
  m_headerFont       = fontManager->GetFont(wxS("Helvetica"), wxPDF_FONTSTYLE_BOLD);
  m_fontNameFont     = fontManager->GetFont(wxS("Times"),     wxPDF_FONTSTYLE_BOLD);
  m_tableNumbersFont = fontManager->GetFont(wxS("Helvetica"), wxPDF_FONTSTYLE_REGULAR);
  m_cellNumbersFont  = fontManager->GetFont(wxS("Courier"),   wxPDF_FONTSTYLE_REGULAR);
}

void
ShowFont::DrawCharCodes()
{
  const UnicodeBlock* block;
  size_t charIndex = 0;
  size_t maxCharIndex = m_cids.Count();
  m_pdf->Bookmark(m_fontName, 0);
  while (charIndex < maxCharIndex && !IsInRange(m_cids[charIndex])) ++charIndex;
  while (charIndex < maxCharIndex)
  {
    if (charIndex < maxCharIndex)
    {
      block = FindUnicodeBlock(m_cids[charIndex]);
      if (block != NULL)
      {
        DrawUnicodeBlock(charIndex, block);
      }
    }
    ++charIndex;
    while (charIndex < maxCharIndex && !IsInRange(m_cids[charIndex])) ++charIndex;
  }
}

const UnicodeBlock*
ShowFont::FindUnicodeBlock(wxUint32 charCode)
{
  const UnicodeBlock* block;
  for (block = unicodeBlocks; block->name; block++)
  {
    if ((charCode >= block->start) && (charCode <= block->end))
    {
      return block;
    }
  }
  return NULL;
}

void
ShowFont::DrawUnicodeBlock(size_t& charIndex, const UnicodeBlock* block)
{
  bool first = true;
  size_t prevCharIndex;
  wxUint32 prevCell;
  size_t maxCharIndex = m_cids.size();

  // Draw all character codes belonging to the given Unicode block
  do
  {
    m_pdf->AddPage();
    if (first)
    {
      // Bookmark for the first page of a Unicode block
      m_pdf->Bookmark(wxString::FromAscii(block->name), 1);
      first = false;
    }
    wxUint32 offset = ((m_cids[charIndex] - block->start) / 0x100) * 0x100;
    wxUint32 tableStart = block->start + offset;
    wxUint32 tableEnd = tableStart + 0xFF > block->end ? block->end + 1 : tableStart + 0x100;
    wxUint32 rows = (tableEnd - tableStart) / 16;
    double xMin = (A4_WIDTH - rows * CELL_WIDTH) / 2;
    wxUint32 j;
    bool filledCells[256];
    for (j = 0; j < 256; ++j)
    {
      filledCells[j] = false;
    }

    DrawPageHeader(wxString::FromAscii(block->name));
    prevCell = tableStart - 1;

    // Fill all empty cells preceding the current character code
    do
    {
      int charPos = m_cids[charIndex] - tableStart;

      for (j = prevCell + 1; j < m_cids[charIndex]; ++j)
      {
        int pos = j - tableStart;
        FillEmptyCell(LeftCellOrigin(xMin, pos), TopCellOrigin(pos), j);
      }
      filledCells[charPos] = true;

      prevCharIndex = charIndex;
      prevCell = m_cids[charIndex];
      ++charIndex;
      while (charIndex < maxCharIndex && !IsInRange(m_cids[charIndex])) ++charIndex;
    }
    while (charIndex < maxCharIndex && (m_cids[charIndex] < tableEnd) && IsInBlock(m_cids[charIndex], block));

    // Fill all empty cells following the current character code
    for (j = prevCell + 1; j < tableEnd; ++j)
    {
      int pos = j - tableStart;
      FillEmptyCell(LeftCellOrigin(xMin, pos), TopCellOrigin(pos), j);
    }

    // Draw all character codes belonging to the current table
    for (j = 0; j < tableEnd - tableStart; ++j)
    {
      if (filledCells[j])
      {
        DrawCharCode(LeftCellOrigin(xMin, j), TopCellOrigin(j), j + tableStart);
      }
    }

    // Draw table grid
    DrawTableGrid(rows, tableStart);
  }
  while (charIndex < maxCharIndex && IsInBlock(m_cids[charIndex], block));

  charIndex = prevCharIndex;
}

void
ShowFont::DrawPageHeader(const wxString& blockName)
{
  // Font name
  m_pdf->SetXY(20, 10);
  m_pdf->SetFont(m_fontNameFont, wxPDF_FONTSTYLE_BOLD, 16);
  m_pdf->Cell(170, 5., m_fontName, wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_CENTER);

  // Unicode block name
  m_pdf->SetXY(20, 18);
  m_pdf->SetFont(m_headerFont, wxPDF_FONTSTYLE_BOLD, 12);
  m_pdf->Cell(170, 5., blockName, wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_CENTER);

  // Page number
  wxString pageNumber = wxString::Format(wxS("%d"), m_pdf->PageNo());
  m_pdf->SetXY(95, 280);
  m_pdf->SetFont(m_headerFont, wxPDF_FONTSTYLE_BOLD, 10);
  m_pdf->Cell(20, 5., pageNumber, wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_CENTER);
}

bool
ShowFont::IsDefined(wxUint32 charCode)
{
  int lo = 0;
  int hi = gs_unicodeRangeCount - 1;
  int mid = 0;
  while (hi - lo > 1)
  {
    mid = (lo + hi) / 2;
    if (charCode < gs_unicodeRanges[mid].first)
    {
      hi = mid;
    }
    else
    {
      lo = mid;
    }
  }
  bool isDefined = (charCode <= gs_unicodeRanges[lo].last);
  return isDefined;
}

void
ShowFont::FillEmptyCell(double x, double y, wxUint32 charCode)
{
  if (charCode < 0x20 || (charCode >= 0x7f && charCode < 0xa0))
  {
    m_pdf->SetFillColour(160,192,232);
  }
  else if (IsDefined(charCode))
  {
    m_pdf->SetFillColour(224,224,224);
  }
  else
  {
    m_pdf->SetFillColour(128,128,128);
  }
  m_pdf->Rect(x, y, CELL_WIDTH, CELL_HEIGHT, wxPDF_STYLE_FILL);
}

bool
ShowFont::IsInBlock(wxUint32 charCode, const UnicodeBlock* block)
{
  return ((charCode >= block->start) && (charCode <= block->end));
}

void
ShowFont::DrawCharCode(double x, double y, wxUint32 charCode)
{
  wxString hex = wxString::Format(wxS("%04X"), charCode);
  m_pdf->SetFont(m_cellNumbersFont, wxPDF_FONTSTYLE_REGULAR, 8);
  m_pdf->SetXY(x, y + CELL_HEIGHT - 3.5);
  m_pdf->Cell(CELL_WIDTH, 3, hex, 0, 0, wxPDF_ALIGN_CENTER);
  m_pdf->SetFont(m_sampleFont, wxPDF_FONTSTYLE_REGULAR, m_fontSize);
  m_pdf->SetXY(x, y + 1);
  wxString glyph;
#if wxCHECK_VERSION(2,9,0)
  glyph.Append(wxUniChar(charCode));
#else
#if SIZEOF_WCHAR_T == 2
  if (charCode <= 0xffff)
  {
    glyph.Append(wxChar(charCode));
  }
  else if (charCode >= 0x110000)
  {
    glyph.Append(wxChar(0));
  }
  else
  {
    wchar_t surrogate[3];
    surrogate[2] = 0;
    surrogate[0] = (wxUint16) ((charCode >> 10) + 0xd7c0);
    surrogate[1] = (wxUint16) ((charCode & 0x3ff) + 0xdc00);
    glyph.Append(wxString(surrogate, wxMBConvUTF16(), 2));
  }
#else
  glyph.Append(wxChar(charCode));
#endif
#endif
  m_pdf->Cell(CELL_WIDTH, CELL_WIDTH, glyph, 0, 0, wxPDF_ALIGN_CENTER);
}

void
ShowFont::DrawTableGrid(wxUint32 colCount, wxUint32 blockStart)
{
  double xMin = (A4_WIDTH - colCount * CELL_WIDTH) / 2;
  double xMax = (A4_WIDTH + colCount * CELL_WIDTH) / 2;
  m_pdf->SetLineWidth(0.35);
  m_pdf->Rect(xMin, TOP_MARGIN, xMax - xMin, TABLE_HEIGHT);
  m_pdf->Line(xMin, TOP_MARGIN, xMin, TOP_MARGIN - 5);
  m_pdf->Line(xMax, TOP_MARGIN, xMax, TOP_MARGIN - 5);

  m_pdf->SetLineWidth(0.2);

  // Draw horizontal lines
  wxUint32 j;
  for (j = 1; j < 16; ++j)
  {
    m_pdf->Line(xMin, TOP_MARGIN + j * CELL_HEIGHT, xMax, TOP_MARGIN + j * CELL_HEIGHT);
  }

  // Draw vertical lines
  for (j = 1; j < colCount; ++j)
  {
    m_pdf->Line(xMin + j * CELL_WIDTH, TOP_MARGIN, xMin + j * CELL_WIDTH, TOP_MARGIN + 16 * CELL_HEIGHT);
  }

  // Draw character code numbers
  m_pdf->SetFont(m_tableNumbersFont, wxPDF_FONTSTYLE_REGULAR, 10);
  wxString hex;
  for (j = 0; j < 16; ++j)
  {
    hex = wxString::Format(wxS("%01X"), j);
    m_pdf->SetXY(xMin - CELL_WIDTH - 1, TOP_MARGIN + (j + 0.35) * CELL_HEIGHT);
    m_pdf->Cell(CELL_WIDTH, 5, hex, 0, 0, wxPDF_ALIGN_RIGHT);
    m_pdf->SetXY(xMax + 1, TOP_MARGIN + (j + 0.35) * CELL_HEIGHT);
    m_pdf->Cell(CELL_WIDTH, 5, hex, 0, 0, wxPDF_ALIGN_LEFT);
  }

  for (j = 0; j < colCount; ++j)
  {
    hex = wxString::Format(wxS("%03X"), blockStart/16 + j);
    m_pdf->SetXY(xMin + j * CELL_WIDTH, TOP_MARGIN - 5);
    m_pdf->Cell(CELL_WIDTH, 5, hex, 0, 0, wxPDF_ALIGN_CENTER);
  }
}

double
ShowFont::LeftCellOrigin(double xMin, int cellPos)
{
  return xMin + CELL_WIDTH * (cellPos / 16);
}

double
ShowFont::TopCellOrigin(int cellPos)
{
  return TOP_MARGIN + CELL_HEIGHT * (cellPos % 16);
}


static const wxCmdLineEntryDesc cmdLineDesc[] =
{
#if wxCHECK_VERSION(2,9,0)
  { wxCMD_LINE_OPTION, "f", "font",          "Create samples for FONT file",                             wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
  { wxCMD_LINE_OPTION, "o", "output",        "Save samples to OUTPUT file",                              wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
  { wxCMD_LINE_OPTION, "e", "encoding",      "Encoding of FONT (required for Type1 fonts)",              wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, "h", "help",          "Show this information message and exit",                   wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, "n", "index",         "Font index in FONT",                                       wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, "i", "include-range", "Show characters in range(s) (s1[-e1][,s2[-e2]...])",       wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, "x", "exclude-range", "Don't show characters in range(s) (s1[-e1][,s2[-e2]...])", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
#else
  { wxCMD_LINE_OPTION, wxS("f"), wxS("font"),          wxS("Create samples for FONT file"),                             wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
  { wxCMD_LINE_OPTION, wxS("o"), wxS("output"),        wxS("Save samples to OUTPUT file"),                              wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
  { wxCMD_LINE_OPTION, wxS("e"), wxS("encoding"),      wxS("Encoding of FONT (required for Type1 fonts)"),              wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, wxS("h"), wxS("help"),          wxS("Show this information message and exit"),                   wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, wxS("n"), wxS("index"),         wxS("Font index in FONT"),                                       wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, wxS("i"), wxS("include-range"), wxS("Show characters in range(s) (s1[-e1][,s2[-e2]...])"),       wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
  { wxCMD_LINE_OPTION, wxS("x"), wxS("exclude-range"), wxS("Don't show characters in range(s) (s1[-e1][,s2[-e2]...])"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL   },
#endif
  { wxCMD_LINE_NONE }
};

bool
ShowFont::OnInit()
{
  // Set the font path and working directory
  wxFileName exePath = wxStandardPaths::Get().GetExecutablePath();
  wxString fontPath = exePath.GetPathWithSep() + wxS("../lib/fonts");
  wxString cwdPath  = exePath.GetPath();
  wxPdfFontManager::GetFontManager()->AddSearchPath(fontPath);
  wxSetWorkingDirectory(cwdPath);

  m_version = wxS("1.1.0 (January 2017)");
  m_ranges = NULL;
  bool valid = false;
  //gets the parameters from cmd line
  wxCmdLineParser parser (cmdLineDesc, argc, argv);
  wxString logo = wxS("wxPdfDocument ShowFont Utility Version ") + m_version + 
                  wxS("\n\nCreate font samples.\n") +
                  wxS("Please specify file extensions in ALL file name parameters.\n");
  parser.SetLogo(logo);
  if (parser.Parse() == 0)
  {
    if (!parser.Found(wxS("help")))
    {
      valid = parser.Found(wxS("font"), &m_fontFile) && 
              parser.Found(wxS("output"), &m_outputFile);
      if (valid)
      {
        valid = m_fontFile != m_outputFile;
      }
      m_hasEncoding = parser.Found(wxS("encoding"), &m_encoding);
      if (!parser.Found(wxS("index"), &m_fontIndex))
      {
        m_fontIndex = 0;
      }
      wxString range;
      if (valid && parser.Found(wxS("include-range"), &range))
      {
        valid = AddRange(range, true);
        if (valid) m_includes = range;
      }
      if (valid && parser.Found(wxS("exclude-range"), &range))
      {
        valid = AddRange(range, false);
        if (valid) m_excludes = range;
      }
    }
    if (!valid)
    {
      parser.Usage();
    }
  }
  return valid;
}

int
ShowFont::OnExit()
{
  if (m_ranges != NULL)
  {
    Range* succ;
    do
    {
      succ = m_ranges->next;
      delete m_ranges;
      m_ranges = succ;
    }
    while (succ != NULL);
  }
  return 0;
}

int
ShowFont::OnRun()
{
  wxLogMessage(wxS("wxPdfDocument ShowFont Utility Version ") + m_version);
  wxLogMessage(wxS("*** Starting to create font sample for ..."));

  if (ProcessFont(m_fontFile))
  {
    InitFonts();
    m_pdf = new wxPdfDocument();
    m_pdf->SetAutoPageBreak(false);
    m_pdf->SetCreator(wxS("ShowFont ")+m_version);
    m_pdf->SetTitle(wxS("Unicode coverage of font ")+m_fontName);
    wxString subject;
    if (!m_includes.IsEmpty())
    {
      subject += wxS("Included code ranges: ") + m_includes;
    }
    if (!m_excludes.IsEmpty())
    {
      if (!m_includes.IsEmpty())
      {
        subject += wxS(" / ");
      }
      subject += wxS("Excluded code ranges: ") + m_excludes;
    }
    if (!subject.IsEmpty())
    {
      m_pdf->SetSubject(subject);
    }
    m_pdf->Open();
    DrawCharCodes();
    m_pdf->SaveAsFile(m_outputFile);
    delete m_pdf;
  }

  wxLogMessage(wxS("*** wxPdfDocument ShowFont finished."));

  return 0;
}

IMPLEMENT_APP_CONSOLE(ShowFont)
