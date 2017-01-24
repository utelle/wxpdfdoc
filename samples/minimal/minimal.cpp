///////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Test program for the wxPdfDocument class
// Author:      Ulrich Telle
// Created:     2005-08-04
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

#include <wx/cmdline.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "wx/pdfdoc.h"
#include "wx/pdffontmanager.h"
#include <ctime>
#include <iostream>

using namespace std;

int tutorial1(bool testMode = false);
int tutorial2(bool testMode = false);
int tutorial3(bool testMode = false);
int tutorial4(bool testMode = false);
int tutorial5(bool testMode = false);
int tutorial6(bool testMode = false);
int tutorial7(bool testMode = false);

int bookmark(bool testMode = false);

#if wxUSE_UNICODE
int cjktest(bool testMode = false);
int indicfonts(bool testMode = false);
#endif

int protection1(bool testMode = false);
int protection2(bool testMode = false);
int clipping(bool testMode = false);
int drawing(bool testMode = false);
int rotation(bool testMode = false);
int wmf(bool testMode = false);
int transformation(bool testMode = false);
int gradients(bool testMode = false);
int charting(bool testMode = false);

int labels(bool testMode = false);
int barcodes(bool testMode = false);
int javascript(bool testMode = false);
int form(bool testMode = false);
int xmlwrite(bool testMode = false);
int transparency(bool testMode = false);
int templates1(bool testMode = false);
int templates2(bool testMode = false);
int layers(bool testMode = false);

int kerning(bool testMode = false);
int attachment(bool testMode = false);

#if defined(__WXMSW__)
#if wxUSE_UNICODE
int glyphwriting(bool testMode = false);
#endif
#endif

int
runTests(bool testMode)
{
  int failed = 0;

  // Group 1
  failed += tutorial1(testMode);
  failed += tutorial2(testMode);
  failed += tutorial3(testMode);
  failed += tutorial4(testMode);
  failed += tutorial5(testMode);
  failed += tutorial6(testMode);
  failed += tutorial7(testMode);
  failed += bookmark(testMode);
#if wxUSE_UNICODE
  failed += cjktest(testMode);
#endif

  // Group 2
  failed += protection1(testMode);
  failed += protection2(testMode);
  failed += clipping(testMode);
  failed += drawing(testMode);
  failed += rotation(testMode);
  failed += wmf(testMode);
  failed += transformation(testMode);
  failed += gradients(testMode);
  failed += charting(testMode);

  // Group 3
  failed += labels(testMode);
  failed += barcodes(testMode);
  failed += javascript(testMode);
  failed += form(testMode);
  failed += xmlwrite(testMode);
  failed += transparency(testMode);
  failed += templates1(testMode);
  failed += templates2(testMode);
  failed += layers(testMode);

  // Group 4
  failed += kerning(testMode);
#if 0  // Do not include in test mode
#if defined(__WXMSW__)
#if wxUSE_UNICODE
  failed += glyphwriting(testMode);
#endif
#endif
#endif
#if wxUSE_UNICODE
  failed += indicfonts(testMode);
#endif
  failed += attachment(testMode);

  return failed;
}

class PdfDocTutorial : public wxAppConsole
{
public:
  bool OnInit();
  int OnRun();
  int OnExit();
  void ShowGroup(int group);

  wxString m_workDirectory;
  wxString m_fontDirectory;
  bool     m_testMode;

  int      m_rc;
};

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
#if wxCHECK_VERSION(2,9,0)
  { wxCMD_LINE_OPTION, "s", "sampledir", "wxPdfDocument samples directory",  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "f", "fontdir",   "wxPdfDocument font directory",     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, "t", "testmode",  "Non-interactive testmode",         wxCMD_LINE_VAL_NONE,   wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, "h", "help",      "Display help",                     wxCMD_LINE_VAL_NONE,   wxCMD_LINE_OPTION_HELP },
#else
  { wxCMD_LINE_OPTION, wxS("s"), wxS("sampledir"), wxS("wxPdfDocument samples directory"),  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, wxS("f"), wxS("fontdir"),   wxS("wxPdfDocument font directory"),     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, wxS("t"), wxS("testmode"),  wxS("Non-interactive testmode"),         wxCMD_LINE_VAL_NONE,   wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, wxS("h"), wxS("help"),      wxS("Display help"),                     wxCMD_LINE_VAL_NONE,   wxCMD_LINE_OPTION_HELP },
#endif
  { wxCMD_LINE_NONE }
};


bool
PdfDocTutorial::OnInit()
{
  // Gets the parameters from cmd line
  wxCmdLineParser parser(cmdLineDesc, argc, argv);
  wxString logo = wxS("wxPdfDocument Minimal Sample\n");
  parser.SetLogo(logo);
  bool ok = parser.Parse() == 0;
  if (ok)
  {
    parser.Found(wxS("sampledir"), &m_workDirectory);
    parser.Found(wxS("fontdir"), &m_fontDirectory);
    m_testMode = parser.Found(wxS("testmode"));
    m_rc = 0;
  }
  else
  {
    parser.Usage();
    m_rc = -1;
  }

  if (ok)
  {
    // Set the font path and working directory
    if (!m_workDirectory.IsEmpty())
    {
      wxSetWorkingDirectory(m_workDirectory);
    }
    m_workDirectory = wxGetCwd();

    if (m_fontDirectory.IsEmpty())
    {
      m_fontDirectory = wxS("../../lib/fonts");
    }
    wxPdfFontManager::GetFontManager()->AddSearchPath(m_fontDirectory);

    // Check directories
    if (!wxFileName::IsFileReadable(wxS("wxpdfdoc.png")))
    {
      wxLogError(wxS("The working directory seems not to be the directory of wxPdfDocument's minimal sample."));
      m_rc = -2;
      ok = false;
    }

    wxPathList fontPaths;
    fontPaths.Add(m_fontDirectory);
    fontPaths.AddEnvList(wxS("WXPDF_FONTPATH"));
    wxString testFontFileName = fontPaths.FindValidPath(wxS("big5.xml"));
    wxFileName testFont(testFontFileName);
    if (testFontFileName.IsEmpty() || !testFont.IsOk() || !testFont.IsFileReadable())
    {
      wxLogError(wxS("The font directory of wxPdfDocument seems not to be accessible."));
      m_rc = -3;
      ok = false;
    }
  }

  return ok;
}

int
PdfDocTutorial::OnExit()
{
  wxStockGDI::DeleteAll();
  return m_rc;
}

#include<locale.h>
#include<stdio.h>

void
PdfDocTutorial::ShowGroup(int group)
{
  // Show a menu on the console
  char c(' ');
  do
  {
    cout << endl << "*** wxPdfDocument sample programs: Group " << group << " ***" << endl;
    cout << endl;

    switch (group)
    {
      case 4:
        cout << "(1) Kerning example" << endl;
        cout << "(2) Glyph writing example" << endl;
        cout << "(3) Indic Fonts and Languages example" << endl;
        cout << "(4) File attachment example" << endl;
        break;
      case 3:
        cout << "(1) Label printing example" << endl;
        cout << "(2) Barcode examples" << endl;
        cout << "(3) Javascript example" << endl;
        cout << "(4) Form example" << endl;
        cout << "(5) Markup example" << endl;
        cout << "(6) Transparency example" << endl;
        cout << "(7) Internal templates example" << endl;
        cout << "(8) External templates example" << endl;
        cout << "(9) Layer examples" << endl;
        break;
      case 2:
        cout << "(1) Protection 1: Demonstrate setting access permissions" << endl;
        cout << "(2) Protection 2: Demonstrate setting passwords" << endl;
        cout << "(3) Clipping: Demonstrate the use of clipping areas" << endl;
        cout << "(4) Drawing: Demonstrate the graphics primitives" << endl;
        cout << "(5) Rotation: Demonstrate the rotation of text and images" << endl;
        cout << "(6) WMF: Embedding a Windows Meta File image" << endl;
        cout << "(7) Transformation: Demonstrates various transformations" << endl;
        cout << "(8) Gradients: Linear and radial gradients shading" << endl;
        cout << "(9) Charting examples" << endl;
        break;
      case 1:
      default:
        cout << "(1) Tutorial 1: Minimal example" << endl;
        cout << "(2) Tutorial 2: Header, footer, page break and image" << endl;
        cout << "(3) Tutorial 3: Line breaks and colors" << endl;
        cout << "(4) Tutorial 4: Multi-columns" << endl;
        cout << "(5) Tutorial 5: Tables" << endl;
        cout << "(6) Tutorial 6: Links and flowing text" << endl;
        cout << "(7) Tutorial 7: Adding new fonts and encoding support" << endl;
        cout << "(8) Bookmarks" << endl;
        cout << "(9) Chinese/Japanese/Korean font support (Unicode only)" << endl;
        break;
    }

    cout << endl;
    cout << "(x) Leave group " << group << endl << endl << "Select program (Enter 1..9 or x): ";

    cin >> c;

    switch (group)
    {
    case 1:
      switch (c)
      {
        case '1': cout << endl << "Running 'tutorial 1' ..." << endl; tutorial1(); break;
        case '2': cout << endl << "Running 'tutorial 2' ..." << endl; tutorial2(); break;
        case '3': cout << endl << "Running 'tutorial 3' ..." << endl; tutorial3(); break;
        case '4': cout << endl << "Running 'tutorial 4' ..." << endl; tutorial4(); break;
        case '5': cout << endl << "Running 'tutorial 5' ..." << endl; tutorial5(); break;
        case '6': cout << endl << "Running 'tutorial 6' ..." << endl; tutorial6(); break;
        case '7': cout << endl << "Running 'tutorial 7' ..." << endl; tutorial7(); break;
        case '8': cout << endl << "Running 'bookmark' ..." << endl; bookmark(); break;
#if wxUSE_UNICODE
        case '9': cout << endl << "Running 'cjktest' ..." << endl; cjktest(); break;
#else
        case '9': cout << endl << "'cjktest' is available in Unicode build only!" << endl; break;
#endif
        case 'X':
        case 'x': break;
        default:
          cout << endl << "Invalid choice!" << endl;
      }
      break;
    case 2:
      switch (c)
      {
        case '1': cout << endl << "Running 'protection1' ..." << endl; protection1(); break;
        case '2': cout << endl << "Running 'protection2' ..." << endl; protection2(); break;
        case '3': cout << endl << "Running 'clipping' ..." << endl; clipping(); break;
        case '4': cout << endl << "Running 'drawing' ..." << endl; drawing(); break;
        case '5': cout << endl << "Running 'rotation' ..." << endl; rotation(); break;
        case '6': cout << endl << "Running 'wmf' ..." << endl; wmf(); break;
        case '7': cout << endl << "Running 'transformation' ..." << endl; transformation(); break;
        case '8': cout << endl << "Running 'gradients' ..." << endl; gradients(); break;
        case '9': cout << endl << "Running 'charting' ..." << endl; charting(); break;
        case 'X':
        case 'x': break;
        default:
          cout << endl << "Invalid choice!" << endl;
      }
      break;
    case 3:
      switch (c)
      {
        case '1': cout << endl << "Running 'labels' ..." << endl; labels(); break;
        case '2': cout << endl << "Running 'barcodes' ..." << endl; barcodes(); break;
        case '3': cout << endl << "Running 'javascript' ..." << endl; javascript(); break;
        case '4': cout << endl << "Running 'form' ..." << endl; form(); break;
        case '5': cout << endl << "Running 'markup' ..." << endl; xmlwrite(); break;
        case '6': cout << endl << "Running 'transparency' ..." << endl; transparency(); break;
        case '7': cout << endl << "Running 'templates1' ..." << endl; templates1(); break;
        case '8': cout << endl << "Running 'templates2' ..." << endl; templates2(); break;
        case '9': cout << endl << "Running 'layers' ..." << endl; layers(); break;
        case 'X':
        case 'x': break;
        default:
          cout << endl << "Invalid choice!" << endl;
      }
      break;
    case 4:
      switch (c)
      {
        case '1': cout << endl << "Running 'kerning' ..." << endl; kerning(); break;
#if defined(__WXMSW__)
#if wxUSE_UNICODE
        case '2': cout << endl << "Running 'glyphwriting' ..." << endl; glyphwriting(); break;
#else
        case '2': cout << endl << "'glyphwriting' is available in Unicode build only!" << endl; break;
#endif
#else
        case '2': cout << endl << "'glyphwriting' is currently available in wxMSW build only!" << endl; break;
#endif
#if wxUSE_UNICODE
        case '3': cout << endl << "Running 'indicfonts' ..." << endl; indicfonts(); break;
#else
        case '3': cout << endl << "'indicfonts' is available in Unicode build only!" << endl; break;
#endif
        case '4': cout << endl << "Running 'attachment' ..." << endl; attachment(); break;

#if 0
        case 'z': cout << endl << "Running 'w' ..." << endl; w(); break;
#endif
        case 'X':
        case 'x': break;
        default:
          cout << endl << "Invalid choice!" << endl;
      }
    }
  }
  while (c != 'x' && c != 'X');
}

int
PdfDocTutorial::OnRun()
{
  if (wxImage::FindHandler(wxBITMAP_TYPE_PNG) == NULL)
  {
    wxImage::AddHandler(new wxPNGHandler());
  }

  if (m_testMode)
  {
    // Non-interactive test mode
    m_rc = runTests(m_testMode);
  }
  else
  {
    // Interactive mode
    // Show a menu on the console
    char c(' ');
    do
    {
      cout << "*** wxPdfDocument sample programs ***" << endl;
      cout << endl;

      cout << "(1) Group 1: Tutorial 1-7, Bookmarks, CJK" << endl;
      cout << "(2) Group 2: Protection, Diverse graphic operations, Charting" << endl;
      cout << "(3) Group 3: Label, Barcode, Javascript, Forms, Markup, Transparency," << endl;
      cout << "             Templates, Layers (ordering, grouping, nesting ...)" << endl;
      cout << "(4) Group 4: Kerning, Direct glyph writing, Indic fonts, Attachments" << endl;
      cout << endl;
      cout << "(x) Exit program" << endl << endl << "Select program (Enter 1..4 or x): ";

      cin >> c;

      switch (c)
      {
      case '1':
      case '2':
      case '3':
      case '4':
        ShowGroup(c - '1' + 1);
        break;
      case 'X':
      case 'x': break;

      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      default:
        cout << endl << "Invalid choice!" << endl;
      }
    } while (c != 'x' && c != 'X');
  }

  return m_rc;
}

IMPLEMENT_APP_CONSOLE(PdfDocTutorial)

