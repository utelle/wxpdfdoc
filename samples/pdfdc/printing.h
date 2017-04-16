/////////////////////////////////////////////////////////////////////////////
// Name:        samples/printing.h

// Purpose:     Printing demo for wxWidgets
// Author:      Julian Smart
// Modified by:
// Created:     1995
// RCS-ID:      $Id: printing.h 42522 2006-10-27 13:07:40Z JS $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// Define a new application
class MyApp: public wxApp
{
  public:
    MyApp(){};
    bool OnInit();
    int OnExit();

    wxString m_workDirectory;
    wxString m_fontDirectory;
    int      m_rc;

    wxFont m_testFont;
};

DECLARE_APP(MyApp)

class MyCanvas;

// Define a new canvas and frame
class MyFrame: public wxFrame
{
  public:
    MyCanvas *canvas;
    wxBitmap m_bitmap;
    wxImage  m_imgUp;
    int      m_angle;
    MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();

// Need a richtext ctrl to create a richtext buffer.
// Use wxRichTextPrinting as a convenient manager
// for richtext buffers
#if wxUSE_RICHTEXT
    wxRichTextCtrl *m_richtext;
    wxRichTextPrinting *m_richtextPrinting;
#endif
    void Draw(wxDC& dc);
    void OnAngleUp(wxCommandEvent& event);
    void OnAngleDown(wxCommandEvent& event);

    void OnSize(wxSizeEvent& event);
    void OnPrint(wxCommandEvent& event);
    void OnPDF(wxCommandEvent& event);
    void OnPDFTemplate(wxCommandEvent& event);
    void OnPrintPreview(wxCommandEvent& event);
    void OnPageSetup(wxCommandEvent& event);
#if defined(__WXMSW__) && wxTEST_POSTSCRIPT_IN_MSW
    void OnPrintPS(wxCommandEvent& event);
    void OnPrintPreviewPS(wxCommandEvent& event);
    void OnPageSetupPS(wxCommandEvent& event);
#endif
#ifdef __WXMAC__
    void OnPageMargins(wxCommandEvent& event);
#endif

    void OnExit(wxCommandEvent& event);
    void OnPrintAbout(wxCommandEvent& event);

// printing framework
    void OnPdfPageSetupAll(wxCommandEvent& event);
    void OnPdfPageSetupMinimal(wxCommandEvent& event);
    void OnPdfPrintDialogAll(wxCommandEvent& event);
    void OnPdfPrintDialogMinimal(wxCommandEvent& event);
#if wxUSE_RICHTEXT
    void OnPdfRichTextPrint(wxCommandEvent& event);
    void OnPdfRichTextPreview(wxCommandEvent& event);
#endif
#if wxUSE_HTML
    void OnPdfHtmlPrint(wxCommandEvent& event);
    void OnPdfHtmlPreview(wxCommandEvent& event);
#endif

DECLARE_EVENT_TABLE()

#if wxUSE_RICHTEXT
private:
    void WriteRichTextBuffer();
#endif
};

// Define a new canvas which can receive some events
class MyCanvas: public wxScrolledWindow
{
  public:
    MyCanvas(wxFrame *frame, const wxPoint& pos, const wxSize& size, long style = wxRETAINED);
    ~MyCanvas(void){};

    virtual void OnDraw(wxDC& dc);
    void OnEvent(wxMouseEvent& event);

DECLARE_EVENT_TABLE()
};

class MyPrintout: public wxPrintout
{
 public:
  MyPrintout(const wxChar *title = _T("My printout")):wxPrintout(title) {}
  bool OnPrintPage(int page);
  bool HasPage(int page);
  bool OnBeginDocument(int startPage, int endPage);
  void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

  void DrawPageOne();

  void DrawPageTwo();

};

enum
{
  WXPRINT_QUIT = 100,
  WXPRINT_PRINT,
  WXPRINT_PDF,
  WXPRINT_PDF_TPL,
  WXPRINT_PAGE_SETUP,
  WXPRINT_PREVIEW,
  WXPRINT_PRINT_PS,
  WXPRINT_PAGE_SETUP_PS,
  WXPRINT_PREVIEW_PS,
  WXPRINT_ABOUT,
  WXPRINT_ANGLEUP,
  WXPRINT_ANGLEDOWN,
  WXPRINT_PAGE_MARGINS,
  WXPDFPRINT_PAGE_SETUP_ALL,
  WXPDFPRINT_PAGE_SETUP_MINIMAL,
  WXPDFPRINT_PRINT_DIALOG_ALL,
  WXPDFPRINT_PRINT_DIALOG_MINIMAL,
  WXPDFPRINT_RICHTEXT_PRINT,
  WXPDFPRINT_RICHTEXT_PREVIEW,
  WXPDFPRINT_HTML_PRINT,
  WXPDFPRINT_HTML_PREVIEW
};
