/////////////////////////////////////////////////////////////////////////////
// Name:        samples/pdfgc/pdfgc.cpp
// Purpose:     wxPdfGraphicsContext sample.
//
//              A single DrawScene() routine renders the same wxGraphicsContext
//              calls to three back-ends:
//                * the on-screen canvas (default platform renderer),
//                * a PDF file via wxPdfDC::GetGraphicsContext(),
//                * the system printer via wxPrinterDC.
//
// Author:      Blake Madden
// Created:     2026-05-09
// Copyright:   (c) 2026 Blake Madden
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if !wxUSE_GRAPHICS_CONTEXT
#error "wxUSE_GRAPHICS_CONTEXT must be enabled to build the pdfgc sample."
#endif

#include <wx/filename.h>
#include <wx/graphics.h>
#include <wx/mimetype.h>
#include <wx/print.h>
#include <wx/printdlg.h>

#include "wx/pdfdc.h"
#include "wx/pdfgc.h"

#include "pdfgc.h"
#include "pdfgc_bitmap.xpm"

#ifndef __WXMSW__
#include "mondrian.xpm"
#endif

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------

static wxPrintData* g_printData = nullptr;

// ---------------------------------------------------------------------------
// Drawing routine — runs against any wxGraphicsContext.
// ---------------------------------------------------------------------------

namespace
{

void DrawSectionLabel(wxGraphicsContext& gc, const wxString& label,
                      double x, double y)
{
    gc.SetFont(wxFontInfo(9).FaceName("Arial").Italic(),
               wxColour(80, 80, 80));
    gc.DrawText(label, x, y);
}

// Five-point star, centred on (0,0), drawn with point at top.
const wxPoint2DDouble STAR_POINTS[] = {
    {  0.0, -32.0 },
    {  9.4, -10.0 },
    { 30.4, -10.0 },
    { 13.5,   3.8 },
    { 18.8,  26.0 },
    {  0.0,  13.0 },
    {-18.8,  26.0 },
    {-13.5,   3.8 },
    {-30.4, -10.0 },
    { -9.4, -10.0 }
};
constexpr size_t STAR_COUNT = sizeof(STAR_POINTS) / sizeof(STAR_POINTS[0]);

wxGraphicsPath BuildStarPath(wxGraphicsContext& gc)
{
    wxGraphicsPath path = gc.CreatePath();
    path.MoveToPoint(STAR_POINTS[0]);
    for (size_t i = 1; i < STAR_COUNT; ++i)
        path.AddLineToPoint(STAR_POINTS[i]);
    path.CloseSubpath();
    return path;
}

} // namespace

void DrawScene(wxGraphicsContext& gc, const wxSize& size)
{
    gc.SetPen(*wxTRANSPARENT_PEN);
    gc.SetBrush(*wxWHITE_BRUSH);
    gc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    // Title
    gc.SetFont(wxFontInfo(14).FaceName("Arial").Bold(), *wxBLACK);
    gc.DrawText(wxS("wxPdfDocument — wxGraphicsContext stress test"), 10, 10);

    // Shapes
    DrawSectionLabel(gc, wxS("Shapes (rect, rounded-rect, ellipse, stroked rect)"),
                     10, 35);
    gc.SetPen(*wxBLACK_PEN);
    gc.SetBrush(*wxCYAN_BRUSH);
    gc.DrawRectangle(10, 50, 100, 70);

    gc.SetBrush(wxBrush("LIGHT BLUE"));
    gc.DrawRoundedRectangle(120, 50, 100, 70, 15);

    gc.SetBrush(wxBrush("GOLDENROD"));
    gc.DrawEllipse(230, 50, 100, 70);

    gc.SetBrush(*wxTRANSPARENT_BRUSH);
    gc.SetPen(wxPenInfo(wxColour("MAGENTA")).Width(3));
    gc.DrawRectangle(340, 50, 100, 70);

    // Paths
    //------
    DrawSectionLabel(gc, wxS("Paths (DrawPath star, StrokePath zigzag, mixed curves)"),
                     10, 140);

    gc.PushState();
    gc.Translate(60, 200);
    gc.SetPen(wxPenInfo(wxColour("FOREST GREEN")).Width(2));
    gc.SetBrush(wxBrush(wxColour(46, 159, 69, 200)));
    gc.DrawPath(BuildStarPath(gc));
    gc.PopState();

    {
        wxGraphicsPath zig = gc.CreatePath();
        zig.MoveToPoint(120, 175);
        for (int i = 0; i < 6; ++i)
        {
            const double dx = 20.0 * (i + 1);
            const double dy = (i % 2 == 0) ? 30.0 : -30.0;
            zig.AddLineToPoint(120 + dx, 200 + dy);
        }
        gc.SetPen(wxPenInfo(wxColour("ORANGE")).Width(2));
        gc.StrokePath(zig);
    }

    {
        wxGraphicsPath bez = gc.CreatePath();
        bez.MoveToPoint(260, 230);
        bez.AddCurveToPoint(290, 150, 340, 270, 380, 175);
        bez.AddQuadCurveToPoint(410, 145, 440, 230);
        bez.AddArc(430, 200, 18, 0, wxDegToRad(360), true);
        gc.SetPen(wxPenInfo(wxColour("MAGENTA")).Width(3));
        gc.SetBrush(*wxTRANSPARENT_BRUSH);
        gc.StrokePath(bez);
    }

    // Gradients
    //----------
    DrawSectionLabel(gc, wxS("Gradients (linear/radial brush, gradient path fill, linear/radial pen)"),
                     10, 250);

    wxGraphicsGradientStops linStops(wxColour(0, 80, 200),
                                     wxColour(255, 240, 80));
    linStops.Add(wxColour(220, 60, 60), 0.5f);
    gc.SetPen(*wxTRANSPARENT_PEN);
    gc.SetBrush(gc.CreateLinearGradientBrush(10, 270, 160, 270, linStops));
    gc.DrawRectangle(10, 270, 150, 60);

    wxGraphicsGradientStops radStops(wxColour(255, 255, 255),
                                     wxColour(20, 60, 130));
    radStops.Add(wxColour(100, 180, 255), 0.5f);
    gc.SetBrush(gc.CreateRadialGradientBrush(220, 300, 220, 300, 45, radStops));
    gc.DrawEllipse(180, 270, 90, 60);

    gc.PushState();
    gc.Translate(65, 365);
    gc.SetBrush(gc.CreateLinearGradientBrush(-30, -32, 30, 26, linStops));
    gc.DrawPath(BuildStarPath(gc));
    gc.PopState();

    gc.PushState();
    gc.Translate(175, 365);
    gc.SetBrush(gc.CreateRadialGradientBrush(0, 0, 0, 0, 35, radStops));
    gc.DrawPath(BuildStarPath(gc));
    gc.PopState();

    gc.PushState();
    gc.Translate(290, 365);
    gc.Rotate(wxDegToRad(30.0));
    gc.SetBrush(gc.CreateLinearGradientBrush(-30, -32, 30, 26, linStops));
    gc.DrawPath(BuildStarPath(gc));
    gc.PopState();

    {
        wxGraphicsPath p = gc.CreatePath();
        p.MoveToPoint(290, 330);
        p.AddLineToPoint(440, 270);
        gc.SetPen(gc.CreatePen(wxGraphicsPenInfo(*wxBLACK).Width(6)
                      .LinearGradient(290, 270, 440, 330, linStops)));
        gc.StrokePath(p);
    }

    {
        wxGraphicsPath ring = gc.CreatePath();
        ring.AddCircle(500, 300, 28);
        gc.SetPen(gc.CreatePen(wxGraphicsPenInfo(*wxBLACK).Width(6)
                      .RadialGradient(475, 278, 475, 278, 60, radStops)));
        gc.SetBrush(*wxTRANSPARENT_BRUSH);
        gc.StrokePath(ring);
    }

    gc.PushState();
    gc.Translate(390, 365);
    gc.Rotate(wxDegToRad(-15.0));
    {
        wxGraphicsPath p = gc.CreatePath();
        p.MoveToPoint(-40, 0);
        p.AddLineToPoint(40, 0);
        gc.SetPen(gc.CreatePen(wxGraphicsPenInfo(*wxBLACK).Width(6)
                      .LinearGradient(-40, 0, 40, 0, linStops)));
        gc.StrokePath(p);
    }
    gc.PopState();

    gc.PushState();
    gc.Translate(490, 365);
    {
        wxGraphicsPath ring = gc.CreatePath();
        ring.AddCircle(0, 0, 20);
        gc.SetPen(gc.CreatePen(wxGraphicsPenInfo(*wxBLACK).Width(5)
                      .RadialGradient(-8, -8, -8, -8, 32, radStops)));
        gc.SetBrush(*wxTRANSPARENT_BRUSH);
        gc.StrokePath(ring);
    }
    gc.PopState();

    gc.PushState();
    gc.Translate(545, 365);
    gc.Scale(0.8, 0.8);
    gc.SetPen(*wxTRANSPARENT_PEN);
    gc.SetBrush(gc.CreateRadialGradientBrush(0, 0, 0, 0, 35, radStops));
    gc.DrawPath(BuildStarPath(gc));
    gc.PopState();

    // Transforms
    //----------------
    DrawSectionLabel(gc, wxS("Transforms (Push/Pop, Translate, Rotate, Scale, "
                         "matrix-applied path)"), 10, 410);

    gc.SetPen(wxPenInfo(*wxBLACK).Width(1));
    gc.SetBrush(wxBrush(wxColour(255, 200, 80)));

    auto drawStarAt = [&](double tx, double ty, double angleDeg,
                          double sx, double sy)
    {
        gc.PushState();
        gc.Translate(tx, ty);
        if (angleDeg != 0.0) gc.Rotate(wxDegToRad(angleDeg));
        if (sx != 1.0 || sy != 1.0) gc.Scale(sx, sy);
        gc.DrawPath(BuildStarPath(gc));
        gc.PopState();
    };

    drawStarAt( 60, 480,   0.0, 1.0, 1.0); // identity
    drawStarAt(160, 480,  30.0, 1.0, 1.0); // rotated
    drawStarAt(260, 480,   0.0, 0.5, 0.5); // uniform scale
    drawStarAt(340, 480,   0.0, 1.5, 0.5); // anisotropic scale

    {
        // Same star transformed via a wxGraphicsMatrix applied directly to
        // the path (instead of mutating the GC's CTM).
        wxGraphicsPath p = BuildStarPath(gc);
        wxGraphicsMatrix m = gc.CreateMatrix();
        m.Translate(460, 480);
        m.Scale(0.7, 0.7);
        m.Rotate(wxDegToRad(-20.0));
        p.Transform(m);
        gc.SetBrush(wxBrush(wxColour("LIGHT BLUE")));
        gc.DrawPath(p);
    }

    // Clipping, layers, composition modes
    //------------------------------------
    DrawSectionLabel(gc, wxS("Clipping, layer alpha, composition modes"),
                     10, 540);

    gc.PushState();
    gc.Clip(20, 560, 90, 50);
    gc.SetPen(*wxBLACK_PEN);
    gc.SetBrush(wxBrush(wxColour("GOLDENROD")));
    gc.DrawEllipse(10, 550, 130, 90);
    gc.ResetClip();
    gc.PopState();

    gc.SetPen(*wxBLACK_PEN);
    gc.SetBrush(*wxTRANSPARENT_BRUSH);
    gc.DrawRectangle(20, 560, 90, 50);

    gc.BeginLayer(0.5);
    gc.SetPen(*wxTRANSPARENT_PEN);
    gc.SetBrush(*wxRED_BRUSH);
    gc.DrawRectangle(160, 560, 50, 50);
    gc.SetBrush(*wxBLUE_BRUSH);
    gc.DrawRectangle(180, 580, 50, 50);
    gc.EndLayer();

    {
        struct ModeEntry { wxCompositionMode mode; const char* label; };
        const ModeEntry modes[] = {
            { wxCOMPOSITION_OVER, "OVER" },
            { wxCOMPOSITION_ADD,  "ADD"  },
            { wxCOMPOSITION_DIFF, "DIFF" }
        };
        double cx = 280;
        for (const auto& entry : modes)
        {
            gc.SetCompositionMode(wxCOMPOSITION_OVER);
            gc.SetBrush(wxBrush(wxColour(0, 120, 220, 200)));
            gc.SetPen(*wxTRANSPARENT_PEN);
            gc.DrawRectangle(cx, 560, 50, 50);

            gc.SetCompositionMode(entry.mode);
            gc.SetBrush(wxBrush(wxColour(220, 60, 60, 200)));
            gc.DrawRectangle(cx + 18, 578, 50, 50);

            gc.SetCompositionMode(wxCOMPOSITION_OVER);
            gc.SetFont(wxFontInfo(8).FaceName("Arial"), *wxBLACK);
            gc.DrawText(entry.label, cx + 4, 635);
            cx += 90;
        }
    }

    // ---- Bitmaps + rotated text ------------------------------------------
    DrawSectionLabel(gc, wxS("Bitmaps (32, 48, 64 px) and rotated text"),
                     10, 670);

    {
        wxBitmap bmp(pdfgc_bitmap_xpm);
        wxGraphicsBitmap gbmp = gc.CreateBitmap(bmp);
        gc.DrawBitmap(gbmp,  10, 690, 32, 32);
        gc.DrawBitmap(gbmp,  60, 690, 48, 48);
        gc.DrawBitmap(gbmp, 125, 690, 64, 64);

        // Same bitmap inside a clipped region, to combine the two features.
        gc.PushState();
        gc.Clip(210, 700, 60, 50);
        gc.DrawBitmap(gbmp, 200, 690, 80, 80);
        gc.ResetClip();
        gc.PopState();
        gc.SetPen(wxPenInfo(*wxBLACK).Width(1));
        gc.SetBrush(*wxTRANSPARENT_BRUSH);
        gc.DrawRectangle(210, 700, 60, 50);
    }

    gc.SetFont(wxFontInfo(11).FaceName("Arial").Italic(),
               wxColour(40, 40, 80));
    gc.DrawText("Rotated text", 320, 720, wxDegToRad(-12.0));
    gc.DrawText("Rotated text", 460, 720, wxDegToRad(-12.0 + 180.0));
}

// ---------------------------------------------------------------------------
// Application
// ---------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    g_printData = new wxPrintData();
    g_printData->SetOrientation(wxPORTRAIT);
    g_printData->SetPaperId(wxPAPER_LETTER);

    MyFrame* frame = new MyFrame(
        wxS("wxPdfDocument — pdfgc sample"),
        wxDefaultPosition, wxSize(700, 850));
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

int MyApp::OnExit()
{
    delete g_printData;
    g_printData = nullptr;
    return wxApp::OnExit();
}

// ---------------------------------------------------------------------------
// Frame
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(PdfGc_SavePdf,           MyFrame::OnSavePdf)
    EVT_MENU(PdfGc_SavePdfStandAlone, MyFrame::OnSavePdfStandAlone)
    EVT_MENU(PdfGc_Print,             MyFrame::OnPrint)
    EVT_MENU(PdfGc_PageSetup,         MyFrame::OnPageSetup)
    EVT_MENU(wxID_EXIT,               MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT,              MyFrame::OnAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    SetIcon(wxICON(mondrian));

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(PdfGc_SavePdf,           "Save as PDF (via wxPdfDC)...\tCtrl+S");
    fileMenu->Append(PdfGc_SavePdfStandAlone, "Save as PDF (Stand-alone GC)...\tCtrl+Shift+S");
    fileMenu->AppendSeparator();
    fileMenu->Append(PdfGc_Print,        "&Print...\tCtrl+P");
    fileMenu->Append(PdfGc_PageSetup,    "Page Set&up...");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt+F4");

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About...");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Save the scene as a PDF, or print it.");

    m_canvas = new MyCanvas(this);
}

void MyFrame::OnSavePdf(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog dlg(this, "Save as PDF", wxEmptyString, "pdfgc.pdf",
                     "PDF files (*.pdf)|*.pdf",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() != wxID_OK)
        return;

    const wxString outPath = dlg.GetPath();

    wxPrintData printData;
    printData.SetOrientation(wxPORTRAIT);
    printData.SetPaperId(wxPAPER_LETTER);
    printData.SetFilename(outPath);

    {
        wxPdfDC dc(printData);
        // Map mode style PDF + MM_POINTS gives us 1 logical unit == 1 pt,
        // matching DrawScene's coordinate assumptions.
        dc.SetMapModeStyle(wxPDF_MAPMODESTYLE_PDF);
        dc.SetMapMode(wxMM_POINTS);

        if (dc.StartDoc("pdfgc demo"))
        {
            dc.StartPage();

            // wxPdfDC owns its wxPdfGraphicsContext — do not delete.
            wxGraphicsContext* gc = dc.GetGraphicsContext();
            if (gc)
            {
                DrawScene(*gc,
                          wxSize(PDFGC_SCENE_WIDTH, PDFGC_SCENE_HEIGHT));
            }
            else
            {
                wxLogError("wxPdfDC::GetGraphicsContext() returned null.");
            }

            dc.EndPage();
            dc.EndDoc();
        }
    }

    SetStatusText("Saved: " + outPath);

    wxFileType* ft = wxTheMimeTypesManager->GetFileTypeFromExtension("pdf");
    if (ft)
    {
        const wxString cmd = ft->GetOpenCommand(outPath);
        if (!cmd.IsEmpty())
            wxExecute(cmd);
        delete ft;
    }
}

void MyFrame::OnSavePdfStandAlone(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog dlg(this, "Save as PDF (Stand-alone GC)", wxEmptyString, "pdfgc_standalone.pdf",
                     "PDF files (*.pdf)|*.pdf",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() != wxID_OK)
        return;

    const wxString outPath = dlg.GetPath();

    {
        wxPdfDocument doc(wxPORTRAIT, "pt", wxPAPER_LETTER);
        doc.Open();
        doc.AddPage();

        // Create a stand-alone graphics context for the document.
        wxGraphicsContext* gc = wxPdfGraphicsRenderer::GetPdfRenderer()->CreateContext(&doc);
        if (gc)
        {
            DrawScene(*gc,
                      wxSize(PDFGC_SCENE_WIDTH, PDFGC_SCENE_HEIGHT));
            delete gc;
        }
        else
        {
            wxLogError("wxPdfGraphicsRenderer::CreateContext(&doc) returned null.");
        }

        doc.SaveAsFile(outPath);
    }

    SetStatusText("Saved (Stand-alone): " + outPath);

    wxFileType* ft = wxTheMimeTypesManager->GetFileTypeFromExtension("pdf");
    if (ft)
    {
        const wxString cmd = ft->GetOpenCommand(outPath);
        if (!cmd.IsEmpty())
            wxExecute(cmd);
        delete ft;
    }
}

void MyFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
    wxPrintDialogData printDialogData(*g_printData);
    wxPrinter printer(&printDialogData);
    MyPrintout printout("pdfgc sample");

    if (!printer.Print(this, &printout, true))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
            wxLogError("Printing failed.");
    }
    else
    {
        *g_printData = printer.GetPrintDialogData().GetPrintData();
    }
}

void MyFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
    wxPageSetupDialogData pageSetupData(*g_printData);
    wxPageSetupDialog dlg(this, &pageSetupData);
    if (dlg.ShowModal() == wxID_OK)
    {
        *g_printData = dlg.GetPageSetupDialogData().GetPrintData();
    }
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(
        "pdfgc sample\n\n"
        "Demonstrates wxPdfGraphicsContext by running a single drawing routine against three "
        "renderers: the on-screen canvas, a PDF file via wxPdfDC::GetGraphicsContext(), "
        "and the system printer.",
        "About pdfgc", wxOK | wxICON_INFORMATION, this);
}

// ---------------------------------------------------------------------------
// Canvas
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
wxEND_EVENT_TABLE()

MyCanvas::MyCanvas(wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                       wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetScrollbars(10, 10,
                  PDFGC_SCENE_WIDTH  / 10 + 2,
                  PDFGC_SCENE_HEIGHT / 10 + 2);
    SetVirtualSize(PDFGC_SCENE_WIDTH, PDFGC_SCENE_HEIGHT);
}

void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (!gc)
        return;

    DrawScene(*gc, wxSize(PDFGC_SCENE_WIDTH, PDFGC_SCENE_HEIGHT));
    delete gc;
}

// ---------------------------------------------------------------------------
// Printout
// ---------------------------------------------------------------------------

bool MyPrintout::OnPrintPage(int page)
{
    if (page != 1)
        return false;

    wxDC* dc = GetDC();
    if (!dc)
        return false;

    int dcW, dcH;
    dc->GetSize(&dcW, &dcH);

    // Fit the 580 x 760-pt logical scene into the printable area while
    // preserving aspect ratio.
    const double sx = double(dcW) / PDFGC_SCENE_WIDTH;
    const double sy = double(dcH) / PDFGC_SCENE_HEIGHT;
    const double scale = wxMin(sx, sy);

    // wxGraphicsContext::Create has no wxDC& overload; dispatch through
    // the concrete wxPrinterDC overload.
    wxPrinterDC* pdc = wxDynamicCast(dc, wxPrinterDC);
    if (!pdc)
        return false;
    wxGraphicsContext* gc = wxGraphicsContext::Create(*pdc);
    if (!gc)
        return false;

    gc->Scale(scale, scale);
    DrawScene(*gc, wxSize(PDFGC_SCENE_WIDTH, PDFGC_SCENE_HEIGHT));
    delete gc;
    return true;
}

bool MyPrintout::HasPage(int page)
{
    return page == 1;
}

void MyPrintout::GetPageInfo(int* minPage, int* maxPage,
                             int* selPageFrom, int* selPageTo)
{
    *minPage = *maxPage = *selPageFrom = *selPageTo = 1;
}
