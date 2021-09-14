/////////////////////////////////////////////////////////////////////////////
// Name:        samples/printing.cpp
// Purpose:     Printing demo for wxWidgets
// Author:      Julian Smart
// Created:     1995
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef USE_VLD
#include "vld.h"
#endif

#if !wxUSE_PRINTING_ARCHITECTURE
#error "You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h, and recompile the library."
#endif

// Set this to 1 if you want to test PostScript printing under MSW.
// However, you'll also need to edit src/msw/makefile.nt.
#define wxTEST_POSTSCRIPT_IN_MSW 0

#include <ctype.h>
#include <wx/cmdline.h>
#include <wx/filename.h>
#include "wx/mimetype.h"
#include <wx/stdpaths.h>
#include "wx/metafile.h"
#include "wx/print.h"
#include "wx/printdlg.h"
#include "wx/image.h"
#include "wx/accel.h"
#include "wx/sizer.h"

#include "wx/pdfdc.h"
#include "wx/pdffontmanager.h"
#include "wx/pdfprint.h"

#if wxTEST_POSTSCRIPT_IN_MSW
  #include "wx/generic/printps.h"
  #include "wx/generic/prntdlgg.h"
#endif

#ifdef __WXMAC__
  #if wxMAJOR_VERSION > 2 || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION == 9)
    #include <wx/osx/printdlg.h>
  #else
    #include <wx/mac/carbon/printdlg.h>
  #endif
#endif

#if wxUSE_RICHTEXT
  #include "wx/richtext/richtextctrl.h"
  #include "wx/richtext/richtextstyles.h"
  #include "wx/richtext/richtextprint.h"
  #include "zebra.xpm"
  #include "smiley.xpm"
#endif

#if wxUSE_HTML
  #include <wx/html/htmprint.h>
#endif

#include "printing.h"

#ifndef __WXMSW__
#include "mondrian.xpm"
#endif

// Link to gnome_print disabled (causes a linking error when using wxWidgtes 2.9.3)
#if 0
#if wxUSE_LIBGNOMEPRINT
#include "wx/html/forcelnk.h"
FORCE_LINK(gnome_print)
#endif
#endif

// Declare a frame
MyFrame   *frame = (MyFrame *) NULL;
// int orientation = wxPORTRAIT;

// Global print data, to remember settings during the session
wxPrintData *g_printData = (wxPrintData*) NULL ;

// Global page setup data
wxPageSetupDialogData* g_pageSetupData = (wxPageSetupDialogData*) NULL;

// Main proc
IMPLEMENT_APP(MyApp)

// Writes a header on a page. Margin units are in millimetres.
bool WritePageHeader(wxPrintout* printout, wxDC* dc, const wxStringCharType* text, float mmToLogical);

// The `main program' equivalent, creating the windows and returning the
// main frame

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
  { wxCMD_LINE_OPTION, "s", "sampledir", "wxPdfDocument samples directory",  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "f", "fontdir",   "wxPdfDocument font directory",     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, "h", "help",      "Display help",                     wxCMD_LINE_VAL_NONE,   wxCMD_LINE_OPTION_HELP },
  { wxCMD_LINE_NONE }
};

bool MyApp::OnInit(void)
{
  wxInitAllImageHandlers();

  wxCmdLineParser parser(cmdLineDesc, argc, argv);
  wxString logo = wxS("wxPdfDocument Minimal Sample\n");
  parser.SetLogo(logo);
  bool ok = parser.Parse() == 0;
  if (ok)
  {
    parser.Found(wxS("sampledir"), &m_workDirectory);
    parser.Found(wxS("fontdir"), &m_fontDirectory);
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
    if (!wxFileName::IsFileReadable(wxS("smile.jpg")))
    {
      wxLogError(wxS("The working directory seems not to be the directory of wxPdfDocument's printing sample."));
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

  if (!ok)
  {
    return false;
  }

    m_testFont.Create(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxS("Arial"));

    g_printData = new wxPrintData;
    // You could set an initial paper size here
    //    g_printData->SetPaperId(wxPAPER_LETTER); // for Americans
    //    g_printData->SetPaperId(wxPAPER_A4);    // for everyone else

    g_pageSetupData = new wxPageSetupDialogData;
    // copy over initial paper size from print record
    (*g_pageSetupData) = *g_printData;
    // Set some initial page margins in mm.
    g_pageSetupData->SetMarginTopLeft(wxPoint(15, 15));
    g_pageSetupData->SetMarginBottomRight(wxPoint(15, 15));

    // Create the main frame window
    frame = new MyFrame((wxFrame *) NULL, _T("wxWidgets Printing Demo"),
        wxPoint(0, 0), wxSize(400, 400));

#if wxUSE_STATUSBAR
    // Give it a status line
    frame->CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

    // Load icon and bitmap
    frame->SetIcon( wxICON( mondrian) );

    // Make a menubar
    wxMenu *file_menu = new wxMenu;

    file_menu->Append(WXPRINT_PRINT,        _T("&Print..."),       _T("Print"));
    file_menu->Append(WXPRINT_PDF,          _T("PDF..."),          _T("PDF"));
    file_menu->Append(WXPRINT_PDF_TPL,      _T("PDF Template..."), _T("PDF Template"));
    file_menu->Append(WXPRINT_PAGE_SETUP,   _T("Page Set&up..."),  _T("Page setup"));
#ifdef __WXMAC__
    file_menu->Append(WXPRINT_PAGE_MARGINS, _T("Page Margins..."), _T("Page margins"));
#endif
    file_menu->Append(WXPRINT_PREVIEW,      _T("Print Pre&view"),  _T("Preview"));

#if wxUSE_ACCEL
    // Accelerators
    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_CTRL, (int) 'V', WXPRINT_PREVIEW);
    wxAcceleratorTable accel(1, entries);
    frame->SetAcceleratorTable(accel);
#endif

#if defined(__WXMSW__) && wxTEST_POSTSCRIPT_IN_MSW
    file_menu->AppendSeparator();
    file_menu->Append(WXPRINT_PRINT_PS,      _T("Print PostScript..."),      _T("Print (PostScript)"));
    file_menu->Append(WXPRINT_PAGE_SETUP_PS, _T("Page Setup PostScript..."), _T("Page setup (PostScript)"));
    file_menu->Append(WXPRINT_PREVIEW_PS,    _T("Print Preview PostScript"), _T("Preview (PostScript)"));
#endif

    file_menu->AppendSeparator();
    file_menu->Append(WXPRINT_ANGLEUP,       _T("Angle up\tAlt-U"),   _T("Raise rotated text angle"));
    file_menu->Append(WXPRINT_ANGLEDOWN,     _T("Angle down\tAlt-D"), _T("Lower rotated text angle"));
    file_menu->AppendSeparator();
    file_menu->Append(WXPRINT_QUIT,          _T("E&xit"),             _T("Exit program"));

    wxMenu *printing_menu = new wxMenu;
    printing_menu->Append(WXPDFPRINT_PAGE_SETUP_ALL, _T("PDF Page Setup All..."), _T("PDF Page Setup (All)"));
    printing_menu->Append(WXPDFPRINT_PAGE_SETUP_MINIMAL, _T("PDF Page Setup Minimal..."), _T("PDF Page Setup (Minimal)"));
    printing_menu->Append(WXPDFPRINT_PRINT_DIALOG_ALL, _T("PDF Print Dialog All..."), _T("PDF Print Dialog (All)"));
    printing_menu->Append(WXPDFPRINT_PRINT_DIALOG_MINIMAL, _T("PDF Print Dialog Minimal..."), _T("PDF Print Dialog (Minimal)"));
#if wxUSE_RICHTEXT
    printing_menu->Append(WXPDFPRINT_RICHTEXT_PRINT, _T("PDF RichText Print..."), _T("PDF RichText (Print)"));
    printing_menu->Append(WXPDFPRINT_RICHTEXT_PREVIEW, _T("PDF RichText Preview..."), _T("PDF RichText (Preview)"));
#endif
#if wxUSE_HTML
    printing_menu->Append(WXPDFPRINT_HTML_PRINT, _T("PDF Html Print..."), _T("PDF Html (Print)"));
    printing_menu->Append(WXPDFPRINT_HTML_PREVIEW, _T("PDF Html Preview..."), _T("PDF Html (Preview)"));
#endif

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(WXPRINT_ABOUT, _T("&About"), _T("About this demo"));

    wxMenuBar *menu_bar = new wxMenuBar;

    menu_bar->Append(file_menu, _T("&File"));
    menu_bar->Append(printing_menu, _T("wxPdf&Printing"));
    menu_bar->Append(help_menu, _T("&Help"));

    // Associate the menu bar with the frame
    frame->SetMenuBar(menu_bar);

    MyCanvas *canvas = new MyCanvas(frame, wxPoint(0, 0), wxSize(100, 100), wxRETAINED|wxHSCROLL|wxVSCROLL);

    // Give it scrollbars: the virtual canvas is 20 * 50 = 1000 pixels in each direction
    canvas->SetScrollbars(20, 20, 50, 50);

    wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );
    mainsizer->Add(canvas,1,wxALL|wxEXPAND, 0);

    frame->canvas = canvas;
    frame->SetSizer(mainsizer);

    frame->Centre(wxBOTH);
    frame->Show();

#if wxUSE_STATUSBAR
    frame->SetStatusText(_T("Printing demo"));
#endif // wxUSE_STATUSBAR

    SetTopWindow(frame);

    return true;
}

int MyApp::OnExit()
{
    delete g_printData;
    delete g_pageSetupData;
    return 1;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(WXPRINT_QUIT, MyFrame::OnExit)
EVT_MENU(WXPRINT_PRINT, MyFrame::OnPrint)
EVT_MENU(WXPRINT_PDF, MyFrame::OnPDF)
EVT_MENU(WXPRINT_PDF_TPL, MyFrame::OnPDFTemplate)
EVT_MENU(WXPRINT_PREVIEW, MyFrame::OnPrintPreview)
EVT_MENU(WXPRINT_PAGE_SETUP, MyFrame::OnPageSetup)
EVT_MENU(WXPRINT_ABOUT, MyFrame::OnPrintAbout)
#if defined(__WXMSW__) && wxTEST_POSTSCRIPT_IN_MSW
EVT_MENU(WXPRINT_PRINT_PS, MyFrame::OnPrintPS)
EVT_MENU(WXPRINT_PREVIEW_PS, MyFrame::OnPrintPreviewPS)
EVT_MENU(WXPRINT_PAGE_SETUP_PS, MyFrame::OnPageSetupPS)
#endif
#ifdef __WXMAC__
EVT_MENU(WXPRINT_PAGE_MARGINS, MyFrame::OnPageMargins)
#endif
EVT_MENU(WXPRINT_ANGLEUP, MyFrame::OnAngleUp)
EVT_MENU(WXPRINT_ANGLEDOWN, MyFrame::OnAngleDown)
EVT_MENU(WXPDFPRINT_PAGE_SETUP_ALL, MyFrame::OnPdfPageSetupAll)
EVT_MENU(WXPDFPRINT_PAGE_SETUP_MINIMAL, MyFrame::OnPdfPageSetupMinimal)
EVT_MENU(WXPDFPRINT_PRINT_DIALOG_ALL, MyFrame::OnPdfPrintDialogAll)
EVT_MENU(WXPDFPRINT_PRINT_DIALOG_MINIMAL, MyFrame::OnPdfPrintDialogMinimal)
#if wxUSE_RICHTEXT
EVT_MENU(WXPDFPRINT_RICHTEXT_PRINT, MyFrame::OnPdfRichTextPrint)
EVT_MENU(WXPDFPRINT_RICHTEXT_PREVIEW, MyFrame::OnPdfRichTextPreview)
#endif
#if wxUSE_HTML
EVT_MENU(WXPDFPRINT_HTML_PRINT, MyFrame::OnPdfHtmlPrint)
EVT_MENU(WXPDFPRINT_HTML_PREVIEW, MyFrame::OnPdfHtmlPreview)
#endif
END_EVENT_TABLE()

// Define my frame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size):
wxFrame(frame, wxID_ANY, title, pos, size)
{
#ifdef __WXMAC__
    wxString rscPath = wxStandardPaths::Get().GetResourcesDir() + wxFileName::GetPathSeparator();
#else
    wxString rscPath = wxEmptyString;
#endif
    canvas = NULL;
    m_angle = 30;
    wxImage image(rscPath + wxS("smile.jpg"));
    if (image.IsOk())
    {
      image.SetAlpha();
      int i,j;
      for (i = 0; i < image.GetWidth(); i++)
        for (j = 0; j < image.GetHeight(); j++)
          image.SetAlpha( i, j, 50 );
      m_bitmap = wxBitmap(image);
      m_imgUp.LoadFile(rscPath + wxS("up.gif"));
    }
#if wxUSE_RICHTEXT
    m_richtext = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString);
    m_richtext->Show(0);
    m_richtextPrinting = new wxRichTextPrinting();
    WriteRichTextBuffer();
#endif
}

MyFrame::~MyFrame()
{
#if wxUSE_RICHTEXT
  delete m_richtextPrinting;
#endif
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true /*force closing*/);
}

void MyFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
    wxPrintDialogData printDialogData(* g_printData);

    wxPrinter printer(& printDialogData);
    MyPrintout printout(_T("My printout"));
    if (!printer.Print(this, &printout, true /*prompt*/))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
            wxMessageBox(_T("There was a problem printing.\nPerhaps your current printer is not set correctly?"), _T("Printing"), wxOK);
        else
            wxMessageBox(_T("You canceled printing"), _T("Printing"), wxOK);
    }
    else
    {
        (*g_printData) = printer.GetPrintDialogData().GetPrintData();
    }
}

void MyFrame::OnPDF(wxCommandEvent& WXUNUSED(event))
{
  wxFileName fileName;
  fileName.SetPath(wxGetCwd());
  fileName.SetFullName(wxS("default.pdf"));
  wxPrintData printData;
  printData.SetOrientation(wxPORTRAIT);
  printData.SetPaperId(wxPAPER_A4);
  printData.SetFilename(fileName.GetFullPath());
  {
    wxPdfDC dc(printData);
    // set wxPdfDC mapping mode style so
    // we can scale fonts and graphics
    // coords with a single setting
    dc.SetMapModeStyle(wxPDF_MAPMODESTYLE_PDF);
    dc.SetMapMode(wxMM_POINTS);
    bool ok = dc.StartDoc(_("Printing ..."));
    if (ok)
    {
      dc.StartPage();
      Draw(dc);
      dc.EndPage();
      dc.EndDoc();
    }
  }

  wxFileType* fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(wxS("pdf"));
  if (fileType != NULL)
  {
    wxString cmd = fileType->GetOpenCommand(fileName.GetFullPath());
    if (!cmd.IsEmpty())
    {
      wxExecute(cmd);
    }
    delete fileType;
  }
}

void MyFrame::OnPDFTemplate(wxCommandEvent& WXUNUSED(event))
{
  wxFileName fileName;
  fileName.SetPath(wxGetCwd());
  fileName.SetFullName(wxS("template.pdf"));

  wxPdfDocument pdf;
  pdf.AddPage();
  pdf.SetFont(wxS("Helvetica"),wxS("B"),16);
  pdf.Cell(40,10,wxS("Hello World!"));
  double w = /*350*/ 125;
  double h = /*350*/ 125;
  int tpl = pdf.BeginTemplate(0, 0, w, h);

  {
    wxPdfDC dc(&pdf, w, h);
    //dc.SetMapMode(wxMM_METRIC);
    //dc.SetResolution(720);
    bool ok = dc.StartDoc(_("Printing ..."));
    if (ok)
    {
      dc.StartPage();
      Draw(dc);
      dc.EndPage();
      dc.EndDoc();
    }
  }
  pdf.EndTemplate();
  pdf.UseTemplate(tpl, 40, 30, 75);
  pdf.SaveAsFile(fileName.GetFullPath());

  wxFileType* fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(wxS("pdf"));
  if (fileType != NULL)
  {
    wxString cmd = fileType->GetOpenCommand(fileName.GetFullPath());
    if (!cmd.IsEmpty())
    {
      wxExecute(cmd);
    }
    delete fileType;
  }
}

void MyFrame::OnPrintPreview(wxCommandEvent& WXUNUSED(event))
{
    // Pass two printout objects: for preview, and possible printing.
    wxPrintDialogData printDialogData(* g_printData);
    wxPrintPreview *preview = new wxPrintPreview(new MyPrintout, new MyPrintout, & printDialogData);
    if (!preview->Ok())
    {
        delete preview;
        wxMessageBox(_T("There was a problem previewing.\nPerhaps your current printer is not set correctly?"), _T("Previewing"), wxOK);
        return;
    }

    wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _T("Demo Print Preview"), wxPoint(100, 100), wxSize(600, 650));
    frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show();
}

void MyFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
    (*g_pageSetupData) = *g_printData;

    wxPageSetupDialog pageSetupDialog(this, g_pageSetupData);
    pageSetupDialog.ShowModal();

    (*g_printData) = pageSetupDialog.GetPageSetupDialogData().GetPrintData();
    (*g_pageSetupData) = pageSetupDialog.GetPageSetupDialogData();
}

#if defined(__WXMSW__) && wxTEST_POSTSCRIPT_IN_MSW
void MyFrame::OnPrintPS(wxCommandEvent& WXUNUSED(event))
{
    wxPostScriptPrinter printer(g_printData);
    MyPrintout printout(_T("My printout"));
    printer.Print(this, &printout, true/*prompt*/);

    (*g_printData) = printer.GetPrintData();
}

void MyFrame::OnPrintPreviewPS(wxCommandEvent& WXUNUSED(event))
{
    // Pass two printout objects: for preview, and possible printing.
    wxPrintDialogData printDialogData(* g_printData);
    wxPrintPreview *preview = new wxPrintPreview(new MyPrintout, new MyPrintout, & printDialogData);
    wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _T("Demo Print Preview"), wxPoint(100, 100), wxSize(600, 650));
    frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show();
}

void MyFrame::OnPageSetupPS(wxCommandEvent& WXUNUSED(event))
{
    (*g_pageSetupData) = * g_printData;

    wxGenericPageSetupDialog pageSetupDialog(this, g_pageSetupData);
    pageSetupDialog.ShowModal();

    (*g_printData) = pageSetupDialog.GetPageSetupDialogData().GetPrintData();
    (*g_pageSetupData) = pageSetupDialog.GetPageSetupDialogData();
}
#endif


#ifdef __WXMAC__
void MyFrame::OnPageMargins(wxCommandEvent& WXUNUSED(event))
{
    (*g_pageSetupData) = *g_printData;

    wxMacPageMarginsDialog pageMarginsDialog(this, g_pageSetupData);
    pageMarginsDialog.ShowModal();

    (*g_printData) = pageMarginsDialog.GetPageSetupDialogData().GetPrintData();
    (*g_pageSetupData) = pageMarginsDialog.GetPageSetupDialogData();
}
#endif


void MyFrame::OnPrintAbout(wxCommandEvent& WXUNUSED(event))
{
    (void)wxMessageBox(_T("wxWidgets printing demo\nAuthor: Julian Smart"),
        _T("About wxWidgets printing demo"), wxOK|wxCENTRE);
}

void MyFrame::OnAngleUp(wxCommandEvent& WXUNUSED(event))
{
    m_angle += 5;
    canvas->Refresh();
}

void MyFrame::OnAngleDown(wxCommandEvent& WXUNUSED(event))
{
    m_angle -= 5;
    canvas->Refresh();
}

void MyFrame::Draw(wxDC& dc)
{
    // This routine just draws a bunch of random stuff on the screen so that we
    // can check that different types of object are being drawn consistently
    // between the screen image, the print preview image (at various zoom
    // levels), and the printed page.
    double fontScaleX, fontScaleY, coordScaleX, coordScaleY, baseScaleX, baseScaleY;
    double txtPosScaleX, txtPosScaleY;
    dc.GetUserScale(&baseScaleX, &baseScaleY);

    wxClassInfo *cinfo = wxClassInfo::FindClass(wxS("wxPdfDC"));
    if ((cinfo != NULL) &&
        (dc.GetClassInfo()->IsKindOf(cinfo)) &&
        (((wxPdfDC*) &dc)->GetMapModeStyle() == wxPDF_MAPMODESTYLE_PDF))
    {
      // We are in a special wxPdfDC mode where everything
      // is mapped correctly using SetMapMode so we make
      // all scaling factors effectively noops
      fontScaleX = fontScaleY = coordScaleX = txtPosScaleX = txtPosScaleY = coordScaleY = ( 1.0 * baseScaleX );
    }
    else
    {
      // We need to use the base user scale to account for any
      // zoom factor that may have been set before a call here.
      // Our coords are in points
      wxSize devicePPI = dc.GetPPI();
      coordScaleX = baseScaleX * (double) devicePPI.GetWidth()  / 72.0;
      coordScaleY = baseScaleY * (double) devicePPI.GetHeight() / 72.0;

      // The font size will be scaled by the dc to screenres / 72.0
      // we want the font size to be scaled back to points and be
      // the correct size relative to our coord scale
      // Note: wxGetScreenPPI returns the wrong number on wxMSW at least
      wxScreenDC sdc;
      fontScaleX = baseScaleX * (double) devicePPI.GetWidth()  / (double) sdc.GetPPI().GetWidth();
      fontScaleY = baseScaleY * (double) devicePPI.GetHeight() / (double) sdc.GetPPI().GetHeight();

      // When using draw text functions we will be using fontScale, but the x y pos needs
      // to be in coord scale
      txtPosScaleX = coordScaleX / fontScaleX;
      txtPosScaleY = coordScaleY / fontScaleY;
    }
    dc.SetUserScale(coordScaleX, coordScaleY);

    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetFont(wxGetApp().m_testFont);

    dc.SetBackgroundMode(wxTRANSPARENT);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
//    dc.SetBackground(*wxWHITE_BRUSH);

    dc.DrawRectangle(0, 0, 230, 350);
    dc.DrawLine(0, 0, 229, 349);
    dc.DrawLine(229, 0, 0, 349);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.SetBrush(*wxCYAN_BRUSH);
    dc.SetPen(*wxRED_PEN);

    dc.DrawRoundedRectangle(0, 20, 200, 80, 20);

    dc.SetUserScale(fontScaleX, fontScaleY);
    dc.DrawText(wxS("Rectangle 200 by 80"), wxRound(40 * txtPosScaleX), wxRound(40 * txtPosScaleY));
    dc.SetUserScale(coordScaleX, coordScaleY);

    dc.SetPen( wxPen(*wxBLACK,0,wxPENSTYLE_DOT_DASH) );
    dc.SetBrush(wxBrush(wxColour(64, 128, 64), wxBRUSHSTYLE_CROSSDIAG_HATCH));
    dc.DrawEllipse(50, 140, 100, 50);

    dc.SetPen(*wxRED_PEN);
    dc.SetBrush(*wxCYAN_BRUSH);

    dc.SetUserScale(fontScaleX, fontScaleY);
    dc.DrawText(wxS("Test message: this is in 10 point text"), wxRound(10 * txtPosScaleX), wxRound(180 * txtPosScaleY));
    dc.SetUserScale(coordScaleX, coordScaleY);

#if wxUSE_UNICODE
    //char *test = "Hebrew    שלום -- Japanese (日本語)";
    //wxString tmp = wxConvUTF8.cMB2WC( test );
    //dc.DrawText( tmp, 10, 200 );
#endif

    wxPoint points[5];
    points[0].x = 0;
    points[0].y = 0;
    points[1].x = 20;
    points[1].y = 0;
    points[2].x = 20;
    points[2].y = 20;
    points[3].x = 10;
    points[3].y = 20;
    points[4].x = 10;
    points[4].y = -20;
    dc.DrawPolygon( 5, points, 20, 250, wxODDEVEN_RULE );
    dc.DrawPolygon( 5, points, 50, 250, wxWINDING_RULE );

    dc.DrawEllipticArc( 80, 250, 60, 30, 0.0, 270.0 );

    points[0].x = 150;
    points[0].y = 250;
    points[1].x = 180;
    points[1].y = 250;
    points[2].x = 180;
    points[2].y = 220;
    points[3].x = 200;
    points[3].y = 220;
    dc.DrawSpline( 4, points );

    dc.DrawArc( 20,10, 10,10, 25,40 );

    wxString str;
    int i = 0;
    str.Printf( wxS("---- Text at angle %d ----"), i );
    dc.SetUserScale(fontScaleX, fontScaleY);
    dc.DrawRotatedText(str, wxRound(100 * txtPosScaleX), wxRound(300 * txtPosScaleY), i);

    i = m_angle;
    str.Printf( wxS("---- Text at angle %d ----"), i );
    dc.SetUserScale(fontScaleX, fontScaleY);
    dc.DrawRotatedText(str, wxRound(100 * txtPosScaleX), wxRound(300 * txtPosScaleY), i);
    dc.SetUserScale(coordScaleX, coordScaleY);

    wxIcon my_icon = wxICON(mondrian) ;

    dc.DrawIcon( my_icon, 100, 100);

    if (m_bitmap.Ok())
    {
      dc.DrawBitmap(m_bitmap, 10, 25);
    }
    if (m_imgUp.Ok())
    {
      dc.DrawBitmap(m_imgUp, 300, 200);
      dc.DrawBitmap(m_imgUp, 300, 250, true);
    }
    dc.SetUserScale(baseScaleX, baseScaleY);
}

void MyFrame::OnPdfPageSetupAll(wxCommandEvent&  WXUNUSED(event) )
{
   wxPageSetupDialogData* dialogData = new wxPageSetupDialogData;
   dialogData->SetMarginTopLeft(wxPoint(25,25));
   dialogData->SetMarginBottomRight(wxPoint(25,25));
   dialogData->EnableMargins(true);
   dialogData->EnablePaper(true);
   dialogData->EnableOrientation(true);

   wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, dialogData);
   if( dialog->ShowModal() == wxID_OK )
   {
     // dialogData now has user choices
   }

   delete dialog;
   delete dialogData;
}

void MyFrame::OnPdfPageSetupMinimal(wxCommandEvent&  WXUNUSED(event) )
{
   wxPageSetupDialogData* dialogData = new wxPageSetupDialogData;
   dialogData->SetMarginTopLeft(wxPoint(25,25));
   dialogData->SetMarginBottomRight(wxPoint(25,25));
   dialogData->EnableMargins(false);
   dialogData->EnablePaper(true);
   dialogData->EnableOrientation(false);

   wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, dialogData, _T("Minimal PDF Page Setup"));
   if( dialog->ShowModal() == wxID_OK )
   {
     // dialogData now has user choices
   }

   delete dialog;
   delete dialogData;
}

void MyFrame::OnPdfPrintDialogAll(wxCommandEvent&  WXUNUSED(event) )
{
   wxPageSetupDialogData* dialogData = new wxPageSetupDialogData;
   dialogData->SetMarginTopLeft(wxPoint(25,25));
   dialogData->SetMarginBottomRight(wxPoint(25,25));
   dialogData->EnableMargins(true);
   dialogData->EnablePaper(true);
   dialogData->EnableOrientation(true);

   wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, dialogData);
   if( dialog->ShowModal() == wxID_OK )
   {
      wxPdfPrintData* printData = new wxPdfPrintData( dialogData );
      wxPdfPrintDialog* printDialog =  new wxPdfPrintDialog(this, printData );
      if( printDialog->ShowModal() == wxID_OK )
      {
       // printData now has user info
      }
      delete printDialog;
      delete printData;
   }
   delete dialog;
   delete dialogData;
}

void MyFrame::OnPdfPrintDialogMinimal(wxCommandEvent&  WXUNUSED(event) )
{
    wxPdfPrintData* printData = new wxPdfPrintData();
    // restrict what we see in dialog
    printData->SetPrintDialogFlags( wxPDF_PRINTDIALOG_OPENDOC|wxPDF_PRINTDIALOG_FILEPATH );
    wxPdfPrintDialog* printDialog =  new wxPdfPrintDialog(this, printData );
    if( printDialog->ShowModal() == wxID_OK )
    {
      // printData now has user info
    }
    delete printDialog;
   delete printData;
}

#if wxUSE_RICHTEXT
void MyFrame::OnPdfRichTextPrint(wxCommandEvent&  WXUNUSED(event) )
{
   wxPageSetupDialogData dialogData = wxPageSetupDialogData();
   dialogData.SetMarginTopLeft(wxPoint(25,25));
   dialogData.SetMarginBottomRight(wxPoint(25,25));
   dialogData.EnableMargins(true);
   dialogData.EnablePaper(true);
   dialogData.EnableOrientation(true);

   wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, &dialogData);
   if( dialog->ShowModal() == wxID_OK )
   {
      dialogData = dialog->GetPageSetupData();
      wxPdfPrintData printData = wxPdfPrintData( &dialogData );

      wxPdfPrintDialog *printDialog =  new wxPdfPrintDialog(this, &printData );
      if( printDialog->ShowModal() == wxID_OK )
      {
        printData = printDialog->GetPdfPrintData();
        // printData now has user info.
        // We will use richtextprinting as a convenient storage container for
        // the copy richtext buffers. If we did not do this, we would need to handle
        // deletion of the buffers in our code.
        m_richtextPrinting->SetRichTextBufferPrinting(
            new wxRichTextBuffer(m_richtext->GetBuffer())
            );

        wxRichTextPrintout *printPrintout = new wxRichTextPrintout(wxS("Demo PDF Printing"));
        // richtext printout accepts margins in tenths of mm
        printPrintout->SetMargins(
                      10 * dialogData.GetMarginTopLeft().y,
                      10 * dialogData.GetMarginBottomRight().y,
                      10 * dialogData.GetMarginTopLeft().x,
                      10 * dialogData.GetMarginBottomRight().x
                      );

        printPrintout->SetRichTextBuffer(  m_richtextPrinting->GetRichTextBufferPrinting() );

        wxPdfPrinter *printer = new wxPdfPrinter( &printData );
        // don't show a print dialog again - we have already done so
        printer->Print(this, printPrintout, false);
        delete printer;
        delete printPrintout;
      }
      delete printDialog;
   }
   delete dialog;
}

void MyFrame::OnPdfRichTextPreview(wxCommandEvent&  WXUNUSED(event) )
{
   wxPageSetupDialogData dialogData = wxPageSetupDialogData();
   dialogData.SetMarginTopLeft(wxPoint(25,25));
   dialogData.SetMarginBottomRight(wxPoint(25,25));
   dialogData.EnableMargins(true);
   dialogData.EnablePaper(true);
   dialogData.EnableOrientation(true);

   wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, &dialogData);
   if( dialog->ShowModal() == wxID_OK )
   {
      dialogData = dialog->GetPageSetupData();
      wxPdfPrintData printData = wxPdfPrintData( &dialogData );
      // We will use richtextprinting as a convenient storage container for
      // the copy richtext buffers. If we did not do this, we would need to handle
      // deletion of the buffers in our code.
      m_richtextPrinting->SetRichTextBufferPrinting(
        new wxRichTextBuffer(m_richtext->GetBuffer())
      );

      m_richtextPrinting->SetRichTextBufferPreview(
        new wxRichTextBuffer(m_richtext->GetBuffer())
      );

      wxRichTextPrintout *printPrintout = new wxRichTextPrintout(wxS("Demo PDF Printing"));
      // richtext printout accepts margins in tenths of mm
      printPrintout->SetMargins(
                      10 * dialogData.GetMarginTopLeft().y,
                      10 * dialogData.GetMarginBottomRight().y,
                      10 * dialogData.GetMarginTopLeft().x,
                      10 * dialogData.GetMarginBottomRight().x
                      );
      printPrintout->SetRichTextBuffer(  m_richtextPrinting->GetRichTextBufferPrinting() );

      wxRichTextPrintout *previewPrintout = new wxRichTextPrintout(wxS("Demo PDF Printing"));
      // richtext printout accepts margins in tenths of mm
      previewPrintout->SetMargins(
                      10 * dialogData.GetMarginTopLeft().y,
                      10 * dialogData.GetMarginBottomRight().y,
                      10 * dialogData.GetMarginTopLeft().x,
                      10 * dialogData.GetMarginBottomRight().x
                      );
      previewPrintout->SetRichTextBuffer(  m_richtextPrinting->GetRichTextBufferPreview() );

      wxPdfPrintPreview *preview = new wxPdfPrintPreview(previewPrintout, printPrintout, &printData);
      if (preview->IsOk())
      {
        wxPreviewFrame *frame = new wxPreviewFrame(preview, this,
                _("PDF Document RichText Preview"), wxDefaultPosition, wxSize(600,600));
        frame->Centre(wxBOTH);
        frame->Initialize();
        frame->Show(true);
      }
      else
      {
        delete preview;
      }
   }
   delete dialog;
}

void MyFrame::WriteRichTextBuffer()
{
    wxRichTextCtrl& r = *m_richtext;

    r.SetFont(wxGetApp().m_testFont);
    // load our images
    // wxRichText does not know at what scale our bitmaps are meant to be displayed
    // so we must scale them - they are designed at 96 dpi
    wxScreenDC sdc;
    wxImage imgZebra = wxBitmap(zebra_xpm).ConvertToImage();
    wxImage imgSmile = wxBitmap(smiley_xpm).ConvertToImage();

    imgZebra.Rescale(imgZebra.GetWidth() * sdc.GetPPI().GetWidth() / 96,
                     imgZebra.GetHeight() * sdc.GetPPI().GetHeight() / 96,
                     wxIMAGE_QUALITY_HIGH
                    );
    imgSmile.Rescale(imgSmile.GetWidth() * sdc.GetPPI().GetWidth() / 96,
                     imgSmile.GetHeight() * sdc.GetPPI().GetHeight() / 96,
                     wxIMAGE_QUALITY_HIGH
                    );

    r.SetDefaultStyle(wxRichTextAttr());

    r.BeginSuppressUndo();

    r.Freeze();

    r.BeginParagraphSpacing(0, 20);

    r.BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
    r.BeginBold();

    r.BeginFontSize(14);

    wxString lineBreak = wxUniChar(29);

    r.WriteText(wxString(wxS("Welcome to wxRichTextCtrl, a wxWidgets control")) + lineBreak + wxS("for editing and presenting styled text and images\n"));
    r.EndFontSize();

    r.BeginItalic();
    r.WriteText(wxS("by Julian Smart"));
    r.EndItalic();

    r.EndBold();
    r.Newline();

    r.WriteImage(imgZebra);

    r.Newline();
    r.Newline();

    r.EndAlignment();

    r.BeginAlignment(wxTEXT_ALIGNMENT_LEFT);
    wxRichTextAttr imageAttr;
    imageAttr.GetTextBoxAttr().SetFloatMode(wxTEXT_BOX_ATTR_FLOAT_LEFT);
    r.WriteText(wxString(wxS("This is a simple test for a floating left image test. The zebra image should be placed at the left side of the current buffer and all the text should flow around it at the right side. This is a simple test for a floating left image test. The zebra image should be placed at the left side of the current buffer and all the text should flow around it at the right side. This is a simple test for a floating left image test. The zebra image should be placed at the left side of the current buffer and all the text should flow around it at the right side.")));
    r.WriteImage(imgZebra, wxBITMAP_TYPE_PNG, imageAttr);

    imageAttr.GetTextBoxAttr().GetTop().SetValue(200);
    imageAttr.GetTextBoxAttr().GetTop().SetUnits(wxTEXT_ATTR_UNITS_PIXELS);
    imageAttr.GetTextBoxAttr().SetFloatMode(wxTEXT_BOX_ATTR_FLOAT_RIGHT);
    r.WriteImage(imgZebra, wxBITMAP_TYPE_PNG, imageAttr);
    r.WriteText(wxString(wxS("This is a simple test for a floating right image test. The zebra image should be placed at the right side of the current buffer and all the text should flow around it at the left side. This is a simple test for a floating left image test. The zebra image should be placed at the right side of the current buffer and all the text should flow around it at the left side. This is a simple test for a floating left image test. The zebra image should be placed at the right side of the current buffer and all the text should flow around it at the left side.")));
    r.EndAlignment();
    r.Newline();

    int i;
    for ( i = 0; i < 10; ++i)
    {

        r.WriteText(wxS("What can you do with this thing? "));

        r.WriteImage(imgSmile);
        r.WriteText(wxS(" Well, you can change text "));

        r.BeginTextColour(wxColour(255, 0, 0));
        r.WriteText(wxS("colour, like this red bit."));
        r.EndTextColour();

        wxRichTextAttr backgroundColourAttr;
        backgroundColourAttr.SetBackgroundColour(*wxGREEN);
        backgroundColourAttr.SetTextColour(wxColour(0, 0, 255));
        r.BeginStyle(backgroundColourAttr);
        r.WriteText(wxS(" And this blue on green bit."));
        r.EndStyle();

        r.WriteText(wxS(" Naturally you can make things "));
        r.BeginBold();
        r.WriteText(wxS("bold "));
        r.EndBold();
        r.BeginItalic();
        r.WriteText(wxS("or italic "));
        r.EndItalic();
        r.BeginUnderline();
        r.WriteText(wxS("or underlined."));
        r.EndUnderline();

        r.BeginFontSize(14);
        r.WriteText(wxS(" Different font sizes on the same line is allowed, too."));
        r.EndFontSize();

        r.WriteText(wxS(" Next we'll show an indented paragraph."));

        r.Newline();

        r.BeginLeftIndent(60);
        r.WriteText(wxS("It was in January, the most down-trodden month of an Edinburgh winter. An attractive woman came into the cafe, which is nothing remarkable."));
        r.Newline();

        r.EndLeftIndent();

        r.WriteText(wxS("Next, we'll show a first-line indent, achieved using BeginLeftIndent(100, -40)."));

        r.Newline();

        r.BeginLeftIndent(100, -40);

        r.WriteText(wxS("It was in January, the most down-trodden month of an Edinburgh winter. An attractive woman came into the cafe, which is nothing remarkable."));
        r.Newline();

        r.EndLeftIndent();

        r.WriteText(wxS("Numbered bullets are possible, again using subindents:"));
        r.Newline();

        r.BeginNumberedBullet(1, 100, 60);
        r.WriteText(wxS("This is my first item. Note that wxRichTextCtrl can apply numbering and bullets automatically based on list styles, but this list is formatted explicitly by setting indents."));
        r.Newline();
        r.EndNumberedBullet();

        r.BeginNumberedBullet(2, 100, 60);
        r.WriteText(wxS("This is my second item."));
        r.Newline();
        r.EndNumberedBullet();

        r.WriteText(wxS("The following paragraph is right-indented:"));
        r.Newline();

        r.BeginRightIndent(200);

        r.WriteText(wxS("It was in January, the most down-trodden month of an Edinburgh winter. An attractive woman came into the cafe, which is nothing remarkable."));
        r.Newline();

        r.EndRightIndent();

        r.WriteText(wxS("The following paragraph is right-aligned with 1.5 line spacing:"));
        r.Newline();

        r.BeginAlignment(wxTEXT_ALIGNMENT_RIGHT);
        r.BeginLineSpacing(wxTEXT_ATTR_LINE_SPACING_HALF);
        r.WriteText(wxS("It was in January, the most down-trodden month of an Edinburgh winter. An attractive woman came into the cafe, which is nothing remarkable."));
        r.Newline();
        r.EndLineSpacing();
        r.EndAlignment();

        wxArrayInt tabs;
        tabs.Add(400);
        tabs.Add(600);
        tabs.Add(800);
        tabs.Add(1000);
        wxRichTextAttr attr;
        attr.SetFlags(wxTEXT_ATTR_TABS);
        attr.SetTabs(tabs);
        r.SetDefaultStyle(attr);

        r.WriteText(wxS("This line contains tabs:\tFirst tab\tSecond tab\tThird tab"));
        r.Newline();

        r.WriteText(wxS("Other notable features of wxRichTextCtrl include:"));
        r.Newline();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("Compatibility with wxTextCtrl API"));
        r.Newline();
        r.EndSymbolBullet();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("Easy stack-based BeginXXX()...EndXXX() style setting in addition to SetStyle()"));
        r.Newline();
        r.EndSymbolBullet();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("XML loading and saving"));
        r.Newline();
        r.EndSymbolBullet();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("Undo/Redo, with batching option and Undo suppressing"));
        r.Newline();
        r.EndSymbolBullet();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("Clipboard copy and paste"));
        r.Newline();
        r.EndSymbolBullet();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("wxRichTextStyleSheet with named character and paragraph styles, and control for applying named styles"));
        r.Newline();
        r.EndSymbolBullet();

        r.BeginSymbolBullet(wxS('*'), 100, 60);
        r.WriteText(wxS("A design that can easily be extended to other content types, ultimately with text boxes, tables, controls, and so on"));
        r.Newline();
        r.EndSymbolBullet();

        // Make a style suitable for showing a URL
        wxRichTextAttr urlStyle;
        urlStyle.SetTextColour(*wxBLUE);
        urlStyle.SetFontUnderlined(true);

        r.WriteText(wxS("wxRichTextCtrl can also display URLs, such as this one: "));
        r.BeginStyle(urlStyle);
        r.BeginURL(wxS("http://www.wxwidgets.org"));
        r.WriteText(wxS("The wxWidgets Web Site"));
        r.EndURL();
        r.EndStyle();
        r.WriteText(wxS(". Click on the URL to generate an event."));

        r.Newline();

        r.WriteText(wxS("Note: this sample content was generated programmatically from within the MyFrame constructor in the demo. The images were loaded from inline XPMs. Enjoy wxRichTextCtrl!\n"));
    }

    r.EndParagraphSpacing();

    // Add a text box
    if (1)
    {

      r.Newline();

      wxRichTextAttr attr;
      attr.GetTextBoxAttr().GetMargins().GetLeft().SetValue(20, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetMargins().GetTop().SetValue(20, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetMargins().GetRight().SetValue(20, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetMargins().GetBottom().SetValue(20, wxTEXT_ATTR_UNITS_PIXELS);

      attr.GetTextBoxAttr().GetBorder().SetColour(*wxBLACK);
      attr.GetTextBoxAttr().GetBorder().SetWidth(1, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetBorder().SetStyle(wxTEXT_BOX_ATTR_BORDER_SOLID);

      wxRichTextBox* textBox = r.WriteTextBox(attr);
      r.SetFocusObject(textBox);

      r.WriteText(wxS("This is a text box. Just testing! Once more unto the breach, dear friends, once more..."));

      r.SetFocusObject(NULL); // Set the focus back to the main buffer
      r.SetInsertionPointEnd();
    }
    if(1)
    {
      // Add a table

      r.Newline();

      wxRichTextAttr attr;
      attr.GetTextBoxAttr().GetMargins().GetLeft().SetValue(5, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetMargins().GetTop().SetValue(5, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetMargins().GetRight().SetValue(5, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetMargins().GetBottom().SetValue(5, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetPadding() = attr.GetTextBoxAttr().GetMargins();

      attr.GetTextBoxAttr().GetBorder().SetColour(*wxBLACK);
      attr.GetTextBoxAttr().GetBorder().SetWidth(1, wxTEXT_ATTR_UNITS_PIXELS);
      attr.GetTextBoxAttr().GetBorder().SetStyle(wxTEXT_BOX_ATTR_BORDER_SOLID);

      wxRichTextAttr cellAttr = attr;
      cellAttr.GetTextBoxAttr().GetWidth().SetValue(200, wxTEXT_ATTR_UNITS_PIXELS);
      cellAttr.GetTextBoxAttr().GetHeight().SetValue(150, wxTEXT_ATTR_UNITS_PIXELS);

      wxRichTextTable* table = r.WriteTable(3, 2, attr, cellAttr);
      int i, j;
      for (j = 0; j < table->GetRowCount(); j++)
      {
          for (i = 0; i < table->GetColumnCount(); i++)
          {
              wxString msg = wxString::Format(wxS("This is cell %d, %d"), (j+1), (i+1));
              r.SetFocusObject(table->GetCell(j, i));
              r.WriteText(msg);
          }
      }
      r.SetFocusObject(NULL); // Set the focus back to the main buffer
      r.SetInsertionPointEnd();
    }

    r.Thaw();

    r.EndSuppressUndo();
}

#endif

#if wxUSE_HTML
void MyFrame::OnPdfHtmlPrint(wxCommandEvent&  WXUNUSED(event) )
{
  wxPageSetupDialogData dialogData = wxPageSetupDialogData();
  dialogData.SetMarginTopLeft(wxPoint(25,25));
  dialogData.SetMarginBottomRight(wxPoint(25,25));
  dialogData.EnableMargins(false);
  dialogData.EnablePaper(true);
  dialogData.EnableOrientation(true);

  wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, &dialogData);
  if( dialog->ShowModal() == wxID_OK )
  {
      dialogData = dialog->GetPageSetupData();
      wxPdfPrintData printData = wxPdfPrintData( &dialogData );

      // restrict user choices in printdialog
      printData.SetPrintDialogFlags( wxPDF_PRINTDIALOG_OPENDOC|wxPDF_PRINTDIALOG_FILEPATH );
      // set launchviewer default to true
      printData.SetLaunchDocumentViewer(true);

      wxPdfPrintDialog *printDialog =  new wxPdfPrintDialog(this, &printData );
      if( printDialog->ShowModal() == wxID_OK )
      {
        printData = printDialog->GetPdfPrintData();

        wxHtmlPrintout *printPrintout = new wxHtmlPrintout(wxS("Demo PDF Printing"));
        printPrintout->SetMargins(
                        dialogData.GetMarginTopLeft().y,
                        dialogData.GetMarginBottomRight().y,
                        dialogData.GetMarginTopLeft().x,
                        dialogData.GetMarginBottomRight().x
                        );
        printPrintout->SetHtmlFile(wxS("test.html"));
        printPrintout->SetStandardFonts(10, wxS("Arial"), wxS("Courier New"));
        wxPdfPrinter *printer = new wxPdfPrinter( &printData );
        // don't show a print dialog again - we have already done so
        printer->Print(this, printPrintout, false);
        delete printer;
        delete printPrintout;
      }
      delete printDialog;
  }
  delete dialog;
}

void MyFrame::OnPdfHtmlPreview(wxCommandEvent&  WXUNUSED(event) )
{
  wxPageSetupDialogData dialogData = wxPageSetupDialogData();
  dialogData.SetMarginTopLeft(wxPoint(25,25));
  dialogData.SetMarginBottomRight(wxPoint(25,25));
  dialogData.EnableMargins(true);
  dialogData.EnablePaper(true);
  dialogData.EnableOrientation(true);

  wxPdfPageSetupDialog* dialog = new wxPdfPageSetupDialog(this, &dialogData);
  if( dialog->ShowModal() == wxID_OK )
  {
      dialogData = dialog->GetPageSetupData();
      wxPdfPrintData printData = wxPdfPrintData( &dialogData );

      wxHtmlPrintout *printPrintout = new wxHtmlPrintout(wxS("Demo PDF Printing"));
      printPrintout->SetMargins(
                      dialogData.GetMarginTopLeft().y,
                      dialogData.GetMarginBottomRight().y,
                      dialogData.GetMarginTopLeft().x,
                      dialogData.GetMarginBottomRight().x
                      );
      printPrintout->SetHtmlFile(wxS("test.html"));
      printPrintout->SetStandardFonts(10, wxS("Arial"), wxS("Courier New"));

      wxHtmlPrintout *previewPrintout = new wxHtmlPrintout(wxS("Demo PDF Printing"));
      previewPrintout->SetMargins(
                      dialogData.GetMarginTopLeft().y,
                      dialogData.GetMarginBottomRight().y,
                      dialogData.GetMarginTopLeft().x,
                      dialogData.GetMarginBottomRight().x
                      );
      previewPrintout->SetHtmlFile(wxS("test.html"));
      previewPrintout->SetStandardFonts(10, wxS("Arial"), wxS("Courier New"));

      wxPdfPrintPreview *preview = new wxPdfPrintPreview(previewPrintout, printPrintout, &printData);
      if (preview->IsOk())
      {
        wxPreviewFrame *frame = new wxPreviewFrame(preview, this,
                _("PDF Document Html Preview"), wxDefaultPosition, wxSize(600,600));
        frame->Centre(wxBOTH);
        frame->Initialize();
        frame->Show(true);
      }
      else
      {
        delete preview;
      }
  }
  delete dialog;
}
#endif

void MyFrame::OnSize(wxSizeEvent& event )
{
    wxFrame::OnSize(event);
}

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
EVT_MOUSE_EVENTS(MyCanvas::OnEvent)
END_EVENT_TABLE()

MyCanvas::MyCanvas(wxFrame *frame, const wxPoint& pos, const wxSize& size, long style):
    wxScrolledWindow(frame, wxID_ANY, pos, size, style)
{
    SetBackgroundColour(* wxWHITE);
}

void MyCanvas::OnDraw(wxDC& dc)
{
    frame->Draw(dc);
}

void MyCanvas::OnEvent(wxMouseEvent& WXUNUSED(event))
{
}

bool MyPrintout::OnPrintPage(int page)
{
    wxDC *dc = GetDC();
    if (dc)
    {
        if (page == 1)
            DrawPageOne();
        else if (page == 2)
            DrawPageTwo();

        // Draw page numbers at top left corner of printable area, sized so that
        // screen size of text matches paper size.
        MapScreenSizeToPage();
        wxStringCharType buf[200];
        wxSprintf(buf, wxS("PAGE %d"), page);
        dc->DrawText(buf, 0, 0);

        return true;
    }
    else
        return false;
}

bool MyPrintout::OnBeginDocument(int startPage, int endPage)
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage))
        return false;

    return true;
}

void MyPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
    *minPage = 1;
    *maxPage = 2;
    *selPageFrom = 1;
    *selPageTo = 2;
}

bool MyPrintout::HasPage(int pageNum)
{
    return (pageNum == 1 || pageNum == 2);
}

void MyPrintout::DrawPageOne()
{
    // You might use THIS code if you were scaling graphics of known size to fit
    // on the page. The commented-out code illustrates different ways of scaling
    // the graphics.

    // We know the graphic is 230x350. If we didn't know this, we'd need to
    // calculate it.
    wxSize devicePPI = GetDC()->GetPPI();
    wxCoord maxX = wxRound(230.0 * (double)devicePPI.GetWidth()  / 72.0);
    wxCoord maxY = wxRound(350.0 * (double)devicePPI.GetHeight() / 72.0);

    // This sets the user scale and origin of the DC so that the image fits
    // within the paper rectangle (but the edges could be cut off by printers
    // that can't print to the edges of the paper -- which is most of them. Use
    // this if your image already has its own margins.
//    FitThisSizeToPaper(wxSize(maxX, maxY));
//    wxRect fitRect = GetLogicalPaperRect();

    // This sets the user scale and origin of the DC so that the image fits
    // within the page rectangle, which is the printable area on Mac and MSW
    // and is the entire page on other platforms.
//    FitThisSizeToPage(wxSize(maxX, maxY));
//    wxRect fitRect = GetLogicalPageRect();

    // This sets the user scale and origin of the DC so that the image fits
    // within the page margins as specified by g_PageSetupData, which you can
    // change (on some platforms, at least) in the Page Setup dialog. Note that
    // on Mac, the native Page Setup dialog doesn't let you change the margins
    // of a wxPageSetupDialogData object, so you'll have to write your own dialog or
    // use the Mac-only wxMacPageMarginsDialog, as we do in this program.
    FitThisSizeToPageMargins(wxSize(maxX, maxY), *g_pageSetupData);
    wxRect fitRect = GetLogicalPageMarginsRect(*g_pageSetupData);

    // This sets the user scale and origin of the DC so that the image appears
    // on the paper at the same size that it appears on screen (i.e., 10-point
    // type on screen is 10-point on the printed page) and is positioned in the
    // top left corner of the page rectangle (just as the screen image appears
    // in the top left corner of the window).
//    MapScreenSizeToPage();
//    wxRect fitRect = GetLogicalPageRect();

    // You could also map the screen image to the entire paper at the same size
    // as it appears on screen.
//    MapScreenSizeToPaper();
//    wxRect fitRect = GetLogicalPaperRect();

    // You might also wish to do you own scaling in order to draw objects at
    // full native device resolution. In this case, you should do the following.
    // Note that you can use the GetLogicalXXXRect() commands to obtain the
    // appropriate rect to scale to.
//    MapScreenSizeToDevice();
//    wxRect fitRect = GetLogicalPageRect();

    // Each of the preceding Fit or Map routines positions the origin so that
    // the drawn image is positioned at the top left corner of the reference
    // rectangle. You can easily center or right- or bottom-justify the image as
    // follows.

    // This offsets the image so that it is centered within the reference
    // rectangle defined above.
    wxCoord xoff = (fitRect.width - maxX) / 2;
    wxCoord yoff = (fitRect.height - maxY) / 2;
    OffsetLogicalOrigin(xoff, yoff);

    // This offsets the image so that it is positioned at the bottom right of
    // the reference rectangle defined above.
//    wxCoord xoff = (fitRect.width - maxX);
//    wxCoord yoff = (fitRect.height - maxY);
//    OffsetLogicalOrigin(xoff, yoff);

    frame->Draw(*GetDC());
}

void MyPrintout::DrawPageTwo()
{
    // You might use THIS code to set the printer DC to ROUGHLY reflect
    // the screen text size. This page also draws lines of actual length
    // 5cm on the page.

    // Compare this to DrawPageOne(), which uses the really convenient routines
    // from wxPrintout to fit the screen image onto the printed page. This page
    // illustrates how to do all the scaling calculations yourself, if you're so
    // inclined.

    wxDC *dc = GetDC();

    // Get the logical pixels per inch of screen and printer
    int ppiScreenX, ppiScreenY;
    GetPPIScreen(&ppiScreenX, &ppiScreenY);
    int ppiPrinterX, ppiPrinterY;
    GetPPIPrinter(&ppiPrinterX, &ppiPrinterY);

    // This scales the DC so that the printout roughly represents the the screen
    // scaling. The text point size _should_ be the right size but in fact is
    // too small for some reason. This is a detail that will need to be
    // addressed at some point but can be fudged for the moment.
    float scale = (float)((float)ppiPrinterX/(float)ppiScreenX);

    // Now we have to check in case our real page size is reduced (e.g. because
    // we're drawing to a print preview memory DC)
    int pageWidth, pageHeight;
    int w, h;
    dc->GetSize(&w, &h);
    GetPageSizePixels(&pageWidth, &pageHeight);

    // If printer pageWidth == current DC width, then this doesn't change. But w
    // might be the preview bitmap width, so scale down.
    float overallScale = scale * (float)(w/(float)pageWidth);
    dc->SetUserScale(overallScale, overallScale);

    // Calculate conversion factor for converting millimetres into logical
    // units. There are approx. 25.4 mm to the inch. There are ppi device units
    // to the inch. Therefore 1 mm corresponds to ppi/25.4 device units. We also
    // divide by the screen-to-printer scaling factor, because we need to
    // unscale to pass logical units to DrawLine.

    // Draw 50 mm by 50 mm L shape
    float logUnitsFactor = (float)(ppiPrinterX/(scale*25.4));
    float logUnits = (float)(50*logUnitsFactor);
    dc->SetPen(* wxBLACK_PEN);
    dc->DrawLine(50, 250, (long)(50.0 + logUnits), 250);
    dc->DrawLine(50, 250, 50, (long)(250.0 + logUnits));

    dc->SetBackgroundMode(wxTRANSPARENT);
    dc->SetBrush(*wxTRANSPARENT_BRUSH);

    { // GetTextExtent demo:
        wxString words[7] = {_T("This "), _T("is "), _T("GetTextExtent "), _T("testing "), _T("string. "), _T("Enjoy "), _T("it!")};
        wxCoord w, h;
        wxCoord x = 200, y= 250;
        wxFont fnt(15, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

        dc->SetFont(fnt);

        for (int i = 0; i < 7; i++)
        {
            wxString word = words[i];
            word.Remove( word.Len()-1, 1 );
            dc->GetTextExtent(word, &w, &h);
            dc->DrawRectangle(x, y, w, h);
            dc->GetTextExtent(words[i], &w, &h);
            dc->DrawText(words[i], x, y);
            x += w;
        }

    }

    dc->SetFont(wxGetApp().m_testFont);

    dc->DrawText(_T("Some test text"), 200, 300 );

    // TESTING

    int leftMargin = 20;
    int rightMargin = 20;
    int topMargin = 20;
    int bottomMargin = 20;

    int pageWidthMM, pageHeightMM;
    GetPageSizeMM(&pageWidthMM, &pageHeightMM);

    float leftMarginLogical = (float)(logUnitsFactor*leftMargin);
    float topMarginLogical = (float)(logUnitsFactor*topMargin);
    float bottomMarginLogical = (float)(logUnitsFactor*(pageHeightMM - bottomMargin));
    float rightMarginLogical = (float)(logUnitsFactor*(pageWidthMM - rightMargin));

    dc->SetPen(* wxRED_PEN);
    dc->DrawLine( (long)leftMarginLogical, (long)topMarginLogical,
        (long)rightMarginLogical, (long)topMarginLogical);
    dc->DrawLine( (long)leftMarginLogical, (long)bottomMarginLogical,
        (long)rightMarginLogical,  (long)bottomMarginLogical);

    WritePageHeader(this, dc, _T("A header"), logUnitsFactor);
}

// Writes a header on a page. Margin units are in millimetres.
bool WritePageHeader(wxPrintout* printout, wxDC* dc, const wxStringCharType* text, float mmToLogical)
{
/*
static wxFont *headerFont = (wxFont *) NULL;
if (!headerFont)
{
headerFont = wxTheFontList->FindOrCreateFont(16, wxSWISS, wxNORMAL, wxBOLD);
}
dc->SetFont(headerFont);
    */

    int pageWidthMM, pageHeightMM;

    printout->GetPageSizeMM(&pageWidthMM, &pageHeightMM);
    wxUnusedVar(pageHeightMM);

    int leftMargin = 10;
    int topMargin = 10;
    int rightMargin = 10;

    float leftMarginLogical = (float)(mmToLogical*leftMargin);
    float topMarginLogical = (float)(mmToLogical*topMargin);
    float rightMarginLogical = (float)(mmToLogical*(pageWidthMM - rightMargin));

    wxCoord xExtent, yExtent;
    dc->GetTextExtent(text, &xExtent, &yExtent);
    float xPos = (float)(((((pageWidthMM - leftMargin - rightMargin)/2.0)+leftMargin)*mmToLogical) - (xExtent/2.0));
    dc->DrawText(text, (long)xPos, (long)topMarginLogical);

    dc->SetPen(* wxBLACK_PEN);
    dc->DrawLine( (long)leftMarginLogical, (long)(topMarginLogical+yExtent),
        (long)rightMarginLogical, (long)topMarginLogical+yExtent );

    return true;
}
