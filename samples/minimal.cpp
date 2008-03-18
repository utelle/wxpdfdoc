///////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Test program for the wxPdfDocument class
// Author:      Ulrich Telle
// Modified by:
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

#include <wx/tokenzr.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "wx/pdfdoc.h"
#include <ctime>
#include <iostream>

using namespace std;

void tutorial1();
void tutorial2();
void tutorial3();
void tutorial4();
void tutorial5();
void tutorial6();
void tutorial7();

void bookmark();

#if wxUSE_UNICODE
void cjktest();
#endif

void protection1();
void protection2();
void clipping();
void drawing();
void rotation();
void wmf();
void transformation();
void gradients();
void charting();

void labels();
void barcodes();
void javascript();
void form();
void xmlwrite();
void transparency();
void templates1();
void templates2();

class PdfDocTutorial : public wxAppConsole
{
public:
  bool OnInit();
  int OnRun();
  int OnExit();
  void ShowGroup(int group);
};

bool
PdfDocTutorial::OnInit()
{
  return true;
}

int
PdfDocTutorial::OnExit()
{
  return 0;
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
      case 3:
        cout << "(1) Label printing example" << endl;
        cout << "(2) Barcode examples" << endl;
        cout << "(3) Javascript example" << endl;
        cout << "(4) Form example" << endl;
        cout << "(5) Markup example" << endl;
        cout << "(6) Transparency example" << endl;
        cout << "(7) Internal templates example" << endl;
        cout << "(8) External templates example" << endl;
#if 0
        cout << "(9) Dataset examples" << endl;
#endif
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
#if 0
        case '9': cout << endl << "Running 'ds_examples' ..." << endl; ds_examples(); break;
        case '8': cout << endl << "Running 'w' ..." << endl; w(); break;
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
  wxSetWorkingDirectory(wxGetCwd() + _T("/../samples"));
  // Set the font path
  wxString fontPath = wxGetCwd() + _T("/../lib/fonts");
  wxSetEnv(_T("WXPDF_FONTPATH"), fontPath);

  wxPdfColour color;
  color.SetColor(wxString(_T("mintcream")));

  // Show a menu on the console
  char c(' ');
  do
  {
    cout << "*** wxPdfDocument sample programs ***" << endl;
    cout << endl;

    cout << "(1) Group 1: Tutorial 1-7, Bookmarks, CJK" << endl;
    cout << "(2) Group 2: Protection, Diverse graphic operations, Charting" << endl;
    cout << "(3) Group 3: Label, Barcode, Javascript, Forms, Markup, Transparency, Templates" << endl;
    cout << endl;
    cout << "(x) Exit program" << endl << endl << "Select program (Enter 1..3 or x): ";

    cin >> c;

    switch (c)
    {
      case '1':
      case '2':
      case '3':
        ShowGroup(c - '1' + 1);
        break;
      case 'X':
      case 'x': break;

      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      default:
        cout << endl << "Invalid choice!" << endl;
    }
  }
  while (c != 'x' && c != 'X');
  return 0;
}

IMPLEMENT_APP_CONSOLE(PdfDocTutorial)

