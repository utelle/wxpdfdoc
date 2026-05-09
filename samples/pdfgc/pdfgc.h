/////////////////////////////////////////////////////////////////////////////
// Name:        samples/pdfgc/pdfgc.h
// Purpose:     wxPdfGraphicsContext sample
// Author:      Blake Madden
// Created:     2026-05-09
// Copyright:   (c) 2026 Blake Madden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef SAMPLES_PDFGC_PDFGC_H_
#define SAMPLES_PDFGC_PDFGC_H_

#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/scrolwin.h>

class MyApp : public wxApp
{
public:
    bool OnInit() wxOVERRIDE;
    int  OnExit() wxOVERRIDE;
};

DECLARE_APP(MyApp)

class MyCanvas;

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnSavePdf(wxCommandEvent& event);
    void OnPrint(wxCommandEvent& event);
    void OnPageSetup(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    MyCanvas* m_canvas;

    wxDECLARE_EVENT_TABLE();
};

class MyCanvas : public wxScrolledWindow
{
public:
    MyCanvas(wxWindow* parent);

    void OnPaint(wxPaintEvent& event);

private:
    wxDECLARE_EVENT_TABLE();
};

class MyPrintout : public wxPrintout
{
public:
    MyPrintout(const wxString& title) : wxPrintout(title) {}

    bool OnPrintPage(int page) wxOVERRIDE;
    bool HasPage(int page) wxOVERRIDE;
    void GetPageInfo(int* minPage, int* maxPage,
                     int* selPageFrom, int* selPageTo) wxOVERRIDE;
};

enum
{
    PdfGc_SavePdf = wxID_HIGHEST + 1,
    PdfGc_Print,
    PdfGc_PageSetup
};

// Logical drawing surface size (in points: 1 pt = 1/72 inch).
constexpr int PDFGC_SCENE_WIDTH  = 580;
constexpr int PDFGC_SCENE_HEIGHT = 760;

// Renders the demo scene.
void DrawScene(wxGraphicsContext& gc, const wxSize& size);

#endif // SAMPLES_PDFGC_PDFGC_H_
