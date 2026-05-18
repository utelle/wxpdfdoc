/////////////////////////////////////////////////////////////////////////////
// Name:        wx/pdfprint.h
// Purpose:     wxPdfPrinter, wxPdfPrintNativeData
// Author:      Mark Dootson
// Created:     2012-05-11
// Copyright:   (c) 2012 Ulrich Telle, Mark Dooston
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PRINTING_H_
#define _PDF_PRINTING_H_

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

#include <wx/defs.h>

#include <wx/dialog.h>
#include <wx/cmndata.h>
#include <wx/prntbase.h>
#include <wx/printdlg.h>
#include <wx/listctrl.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>

#include "wx/pdfdc.h"

#define wxPDF_PRINTER_DEFAULT_RESOLUTION 600

//----------------------------------------------------------------------------
// wxPdfPrintData
//----------------------------------------------------------------------------

#define wxPDF_PRINTDIALOG_ALLOWNONE  0
#define wxPDF_PRINTDIALOG_FILEPATH   0x01
#define wxPDF_PRINTDIALOG_PROPERTIES 0x02
#define wxPDF_PRINTDIALOG_PROTECTION 0x04
#define wxPDF_PRINTDIALOG_OPENDOC    0x08
#define wxPDF_PRINTDIALOG_ALLOWALL   wxPDF_PRINTDIALOG_FILEPATH   \
                                    |wxPDF_PRINTDIALOG_PROPERTIES \
                                    |wxPDF_PRINTDIALOG_PROTECTION \
                                    |wxPDF_PRINTDIALOG_OPENDOC

/// Class for holding print configuration data.
class WXDLLIMPEXP_PDFDOC wxPdfPrintData : public wxObject
{
public:
  /// Default constructor
  wxPdfPrintData();

  /// Copy constructor
  /**
  * \param pdfPrintData The data to copy from
  */
  wxPdfPrintData(wxPdfPrintData* pdfPrintData);

  /// Constructor from wxPrintData
  /**
  * \param printData The print data to initialize from
  */
  wxPdfPrintData(wxPrintData* printData);

  /// Constructor from wxPrintDialogData
  /**
  * \param printDialogData The print dialog data to initialize from
  */
  wxPdfPrintData(wxPrintDialogData* printDialogData);

  /// Constructor from wxPageSetupDialogData
  /**
  * \param pageSetupDialogData The page setup dialog data to initialize from
  */
  wxPdfPrintData(wxPageSetupDialogData* pageSetupDialogData);

  /// Check whether the print data is valid
  /**
  * \return @c true if valid, @c false otherwise
  */
  bool Ok() const { return IsOk(); }

  /// Check whether the print data is valid
  /**
  * \return @c true if valid, @c false otherwise
  */
  bool IsOk() const { return true; }

  // wxPrintData compatibility

  /// Get page orientation
  /**
  * \return The print orientation
  */
  wxPrintOrientation GetOrientation() const { return m_printOrientation; }

  /// Set page orientation
  /**
  * \param orient The print orientation
  */
  void SetOrientation(wxPrintOrientation orient) { m_printOrientation = orient; }

  /// Get paper size ID
  /**
  * \return The paper size ID
  */
  wxPaperSize GetPaperId() const { return m_paperId; }

  /// Set paper size ID
  /**
  * \param sizeId The paper size ID
  */
  void SetPaperId(wxPaperSize sizeId) { m_paperId = sizeId; }

  /// Get print quality
  /**
  * \return The print quality
  */
  wxPrintQuality GetQuality() const { return m_printQuality; }

  /// Set print quality
  /**
  * \param quality The print quality
  */
  void SetQuality(wxPrintQuality quality) { m_printQuality = quality; }

  /// Get output filename
  /**
  * \return The output filename
  */
  wxString GetFilename() const { return m_filename; }

  /// Set output filename
  /**
  * \param filename The output filename
  */
  void SetFilename( const wxString &filename ) { m_filename = filename; }

  // wxPrintDialogData compatibility

  /// Get first page to print
  /**
  * \return The first page number
  */
  int GetFromPage() const { return m_printFromPage; }

  /// Get last page to print
  /**
  * \return The last page number
  */
  int GetToPage() const { return m_printToPage; }

  /// Get minimum page number
  /**
  * \return The minimum page number
  */
  int GetMinPage() const { return m_printMinPage; }

  /// Get maximum page number
  /**
  * \return The maximum page number
  */
  int GetMaxPage() const { return m_printMaxPage; }

  /// Set first page to print
  /**
  * \param v The first page number
  */
  void SetFromPage(int v) { m_printFromPage = v; }

  /// Set last page to print
  /**
  * \param v The last page number
  */
  void SetToPage(int v) { m_printToPage = v; }

  /// Set minimum page number
  /**
  * \param v The minimum page number
  */
  void SetMinPage(int v) { m_printMinPage = v; }

  /// Set maximum page number
  /**
  * \param v The maximum page number
  */
  void SetMaxPage(int v) { m_printMaxPage = v; }

  // wxPdfPrintDialog flags

  /// Get print dialog flags
  /**
  * \return The print dialog flags
  */
  int GetPrintDialogFlags() const { return m_printDialogFlags; }

  /// Set print dialog flags
  /**
  * \param flags The print dialog flags
  */
  void SetPrintDialogFlags(int flags) { m_printDialogFlags = flags; }

  // wxPdfDocument specific

  /// Set template document
  /**
  * \param pdfDocument The template document
  * \param templateWidth The template width
  * \param templateHeight The template height
  */
  void SetTemplate(wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);

  /// Get template document
  /**
  * \return The template document
  */
  wxPdfDocument* GetTemplateDocument() const { return m_templateDocument; }

  /// Get template width
  /**
  * \return The template width
  */
  double GetTemplateWidth() const { return m_templateWidth; }

  /// Get template height
  /**
  * \return The template height
  */
  double GetTemplateHeight() const { return m_templateHeight; }

  /// Get template mode
  /**
  * \return @c true if in template mode, @c false otherwise
  */
  bool GetTemplateMode() const { return m_templateMode; }

  /// Get print resolution
  /**
  * \return The print resolution in DPI
  */
  int GetPrintResolution() const;

  /// Set print resolution
  /**
  * \param resolution The print resolution in DPI
  */
  void SetPrintResolution( int resolution ) { m_printQuality = resolution; }

  /// Get document viewer launch flag
  /**
  * \return @c true if document viewer should be launched, @c false otherwise
  */
  bool GetLaunchDocumentViewer() const { return m_launchViewer; }

  /// Set document viewer launch flag
  /**
  * \param enable @c true if document viewer should be launched, @c false otherwise
  */
  void SetLaunchDocumentViewer( bool enable ) { m_launchViewer = enable; }

  /// Get document title
  /**
  * \return The document title
  */
  const wxString& GetDocumentTitle() const { return m_documentTitle; }

  /// Get document subject
  /**
  * \return The document subject
  */
  const wxString& GetDocumentSubject() const { return m_documentSubject; }

  /// Get document author
  /**
  * \return The document author
  */
  const wxString& GetDocumentAuthor() const { return m_documentAuthor; }

  /// Get document keywords
  /**
  * \return The document keywords
  */
  const wxString& GetDocumentKeywords() const { return m_documentKeywords; }

  /// Get document creator
  /**
  * \return The document creator
  */
  const wxString& GetDocumentCreator() const { return m_documentCreator; }

  /// Set document title
  /**
  * \param title The document title
  */
  void SetDocumentTitle(const wxString& title) { m_documentTitle = title; }

  /// Set document subject
  /**
  * \param subject The document subject
  */
  void SetDocumentSubject(const wxString& subject) { m_documentSubject = subject; }

  /// Set document author
  /**
  * \param author The document author
  */
  void SetDocumentAuthor(const wxString& author) { m_documentAuthor = author; }

  /// Set document keywords
  /**
  * \param keywords The document keywords
  */
  void SetDocumentKeywords(const wxString& keywords) { m_documentKeywords = keywords; }

  /// Set document creator
  /**
  * \param creator The document creator
  */
  void SetDocumentCreator(const wxString& creator) { m_documentCreator = creator; }

  /// Check whether document protection is enabled
  /**
  * \return @c true if protection is enabled, @c false otherwise
  */
  bool IsProtectionEnabled() const { return  m_protectionEnabled; }

  /// Set document protection
  /**
  * \param permissions The permissions bitmask
  * \param userPassword The user password
  * \param ownerPassword The owner password
  * \param encryptionMethod The encryption method
  * \param keyLength The encryption key length
  */
  void SetDocumentProtection(int permissions,
                             const wxString& userPassword = wxEmptyString,
                             const wxString& ownerPassword = wxEmptyString,
                             wxPdfEncryptionMethod encryptionMethod = wxPDF_ENCRYPTION_AESV3R6,
                             int keyLength = 0);

  /// Get user password
  /**
  * \return The user password
  */
  const wxString& GetUserPassword() const { return m_userPassword; }

  /// Get owner password
  /**
  * \return The owner password
  */
  const wxString& GetOwnerPassword() const { return m_ownerPassword; }

  /// Get permissions
  /**
  * \return The permissions bitmask
  */
  int GetPermissions() const { return m_permissions; }

  /// Get encryption method
  /**
  * \return The encryption method
  */
  wxPdfEncryptionMethod GetEncryptionMethod() const { return m_encryptionMethod; }

  /// Get key length
  /**
  * \return The encryption key length
  */
  int GetKeyLength() const { return m_keyLength; }

  /// Clear document protection settings
  void ClearDocumentProtection();

  /// Update document metadata and protection settings
  /**
  * \param pdfDoc The document to update
  */
  void UpdateDocument(wxPdfDocument* pdfDoc);

  /// Create wxPrintData from this instance
  /**
  * \return A new wxPrintData instance
  */
  wxPrintData* CreatePrintData() const;

private:
  void Init();

  wxString              m_documentTitle;
  wxString              m_documentSubject;
  wxString              m_documentAuthor;
  wxString              m_documentKeywords;
  wxString              m_documentCreator;
  bool                  m_protectionEnabled;
  wxString              m_userPassword;
  wxString              m_ownerPassword;
  int                   m_permissions;
  wxPdfEncryptionMethod m_encryptionMethod;
  int                   m_keyLength;
  wxPrintOrientation    m_printOrientation;
  int                   m_printQuality;
  wxPaperSize           m_paperId;
  wxString              m_filename;

  int                   m_printFromPage;
  int                   m_printToPage;
  int                   m_printMinPage;
  int                   m_printMaxPage;

  int                   m_printDialogFlags;
  bool                  m_launchViewer;

  wxPdfDocument*        m_templateDocument;
  double                m_templateHeight;
  double                m_templateWidth;
  bool                  m_templateMode;

private:
  DECLARE_DYNAMIC_CLASS(wxPdfPrintData)
};


//----------------------------------------------------------------------------
// wxPdfPrinter
//----------------------------------------------------------------------------

/// Class for managing the printing process to PDF.
class WXDLLIMPEXP_PDFDOC wxPdfPrinter : public wxPrinterBase
{
public:
  /// Default constructor
  wxPdfPrinter();

  /// Constructor from wxPrintDialogData
  /**
  * \param data The print dialog data
  */
  wxPdfPrinter(wxPrintDialogData* data);

  /// Constructor from wxPdfPrintData
  /**
  * \param data The PDF print data
  */
  wxPdfPrinter(wxPdfPrintData* data);

  /// Constructor from wxPrintData
  /**
  * \param data The print data
  */
  wxPdfPrinter(wxPrintData* data);

  /// Print a printout
  /**
  * \param parent The parent window
  * \param printout The printout to print
  * \param prompt @c true to show the print dialog, @c false otherwise
  * \return @c true if successful, @c false otherwise
  */
  virtual bool Print(wxWindow* parent, wxPrintout* printout, bool prompt = true);

  /// Show the print dialog
  /**
  * \param parent The parent window
  * \return The device context if successful, NULL otherwise
  */
  virtual wxDC* PrintDialog(wxWindow* parent);

  /// Show the setup dialog
  /**
  * \param parent The parent window
  * \return @c true if successful, @c false otherwise
  */
  virtual bool Setup(wxWindow* parent);

  /// Show or hide the progress dialog
  /**
  * \param show @c true to show the progress dialog, @c false otherwise
  */
  void ShowProgressDialog(bool show) { m_showProgressDialog = show; }

private:

  void GetPdfScreenPPI(int* x, int* y);

  bool            m_showProgressDialog;
  wxPdfPrintData  m_pdfPrintData;

  DECLARE_DYNAMIC_CLASS(wxPdfPrinter)
};

// ----------------------------------------------------------------------------
// wxPdfPrintPreview: programmer creates an object of this class to preview a
// wxPrintout.
// ----------------------------------------------------------------------------

/// Class for managing the print preview process for PDF documents.
class WXDLLIMPEXP_PDFDOC wxPdfPrintPreview : public wxPrintPreviewBase
{
public:
  /// Constructor
  /**
  * \param printout The printout to preview
  * \param printoutForPrinting The printout to use for printing
  */
  wxPdfPrintPreview(wxPrintout* printout,
                    wxPrintout* printoutForPrinting );

  /// Constructor from wxPrintDialogData
  /**
  * \param printout The printout to preview
  * \param printoutForPrinting The printout to use for printing
  * \param data The print dialog data
  */
  wxPdfPrintPreview(wxPrintout* printout,
                    wxPrintout* printoutForPrinting,
                    wxPrintDialogData* data);

  /// Constructor from wxPrintData
  /**
  * \param printout The printout to preview
  * \param printoutForPrinting The printout to use for printing
  * \param data The print data
  */
  wxPdfPrintPreview(wxPrintout* printout,
                    wxPrintout* printoutForPrinting,
                    wxPrintData* data);

  /// Constructor from wxPdfPrintData
  /**
  * \param printout The printout to preview
  * \param printoutForPrinting The printout to use for printing
  * \param data The PDF print data
  */
  wxPdfPrintPreview(wxPrintout* printout,
                    wxPrintout* printoutForPrinting,
                    wxPdfPrintData* data);

  /// Destructor
  virtual ~wxPdfPrintPreview();

  /// Set the current page
  /**
  * \param pageNum The page number
  * \return @c true if successful, @c false otherwise
  */
  virtual bool SetCurrentPage(int pageNum);

  /// Get the current page
  /**
  * \return The current page number
  */
  virtual int GetCurrentPage() const;

  /// Set the printout
  /**
  * \param printout The printout
  */
  virtual void SetPrintout(wxPrintout* printout);

  /// Get the preview printout
  /**
  * \return The preview printout
  */
  virtual wxPrintout* GetPrintout() const;

  /// Get the printing printout
  /**
  * \return The printing printout
  */
  virtual wxPrintout* GetPrintoutForPrinting() const;

  /// Set the frame
  /**
  * \param frame The frame
  */
  virtual void SetFrame(wxFrame* frame);

  /// Set the canvas
  /**
  * \param canvas The canvas
  */
  virtual void SetCanvas(wxPreviewCanvas* canvas);

  /// Get the frame
  /**
  * \return The frame
  */
  virtual wxFrame* GetFrame() const;

  /// Get the canvas
  /**
  * \return The canvas
  */
  virtual wxPreviewCanvas* GetCanvas() const;

  /// Paint the page
  /**
  * \param canvas The canvas
  * \param dc The device context
  * \return @c true if successful, @c false otherwise
  */
  virtual bool PaintPage(wxPreviewCanvas* canvas, wxDC& dc);

  /// Update page rendering
  /**
  * \return @c true if successful, @c false otherwise
  */
  virtual bool UpdatePageRendering();

  /// Draw a blank page
  /**
  * \param canvas The canvas
  * \param dc The device context
  * \return @c true if successful, @c false otherwise
  */
  virtual bool DrawBlankPage(wxPreviewCanvas* canvas, wxDC& dc);

  /// Adjust scrollbars
  /**
  * \param canvas The canvas
  */
  virtual void AdjustScrollbars(wxPreviewCanvas* canvas);

  /// Render the page
  /**
  * \param pageNum The page number
  * \return @c true if successful, @c false otherwise
  */
  virtual bool RenderPage(int pageNum);

  /// Set the zoom factor
  /**
  * \param percent The zoom factor in percent
  */
  virtual void SetZoom(int percent);

  /// Get the zoom factor
  /**
  * \return The zoom factor in percent
  */
  virtual int GetZoom() const;

  /// Print the document
  /**
  * \param interactive @c true to show the print dialog, @c false otherwise
  * \return @c true if successful, @c false otherwise
  */
  virtual bool Print(bool interactive);

  /// Determine scaling
  virtual void DetermineScaling();

  /// Get the print dialog data
  /**
  * \return The print dialog data
  */
  virtual wxPrintDialogData& GetPrintDialogData();

  /// Get the maximum page number
  /**
  * \return The maximum page number
  */
  virtual int GetMaxPage() const;

  /// Get the minimum page number
  /**
  * \return The minimum page number
  */
  virtual int GetMinPage() const;

  /// Check whether the preview is valid
  /**
  * \return @c true if valid, @c false otherwise
  */
  virtual bool Ok() const { return IsOk(); }

  /// Check whether the preview is valid
  /**
  * \return @c true if valid, @c false otherwise
  */
  virtual bool IsOk() const;

  /// Set the preview valid flag
  /**
  * \param ok @c true if valid, @c false otherwise
  */
  virtual void SetOk(bool ok);

private:
  wxPrintPreviewBase* m_pimpl;

private:
  DECLARE_CLASS(wxPdfPrintPreview)
  DECLARE_NO_COPY_CLASS(wxPdfPrintPreview);
};


class WXDLLIMPEXP_PDFDOC wxPdfPrintPreviewImpl : public wxPrintPreviewBase
{
public:
  wxPdfPrintPreviewImpl(wxPrintout* printout,
                        wxPrintout* printoutForPrinting );

  wxPdfPrintPreviewImpl(wxPrintout* printout,
                        wxPrintout* printoutForPrinting,
                        wxPrintDialogData* data);

  wxPdfPrintPreviewImpl(wxPrintout* printout,
                        wxPrintout* printoutForPrinting,
                        wxPrintData* data);

  wxPdfPrintPreviewImpl(wxPrintout* printout,
                        wxPrintout* printoutForPrinting,
                        wxPdfPrintData* data);

  virtual ~wxPdfPrintPreviewImpl();

  virtual bool Print(bool interactive);
  virtual void DetermineScaling();

protected:
  virtual bool RenderPageIntoBitmap(wxBitmap& bmp, int pageNum);

private:

  void GetPdfScreenPPI(int* x, int* y);

private:

  wxPdfPrintData*  m_pdfPrintData;
  wxPdfDC*         m_pdfPreviewDC;
  wxPdfDocument*   m_pdfPreviewDoc;

  DECLARE_CLASS(wxPdfPrintPreviewImpl)
};

enum
{
  wxPDF_PRINTDIALOG_CTRLID_FILEPICKER = 30,
  wxPDF_PRINTDIALOG_CTRLID_PROTECT
};

// -------------------------------------------------------------------------
// wxPdfPrintDialog: populate wxPdfPrintData with user choices
// -------------------------------------------------------------------------

/// Class for the PDF print dialog.
class WXDLLIMPEXP_PDFDOC wxPdfPrintDialog : public wxPrintDialogBase
{
public:
  /// Constructor
  /**
  * \param parent The parent window
  * \param data The PDF print data
  */
  wxPdfPrintDialog(wxWindow* parent, wxPdfPrintData* data);

  /// Destructor
  virtual ~wxPdfPrintDialog();

  /// Handle OK button press
  /**
  * \param event The command event
  */
  void OnOK(wxCommandEvent& event);

  /// Transfer data from window controls
  /**
  * \return @c true if successful, @c false otherwise
  */
  virtual bool TransferDataFromWindow();

  /// Transfer data to window controls
  /**
  * \return @c true if successful, @c false otherwise
  */
  virtual bool TransferDataToWindow();

  /// Show the dialog modally
  /**
  * \return @c wxID_OK if successful, @c wxID_CANCEL otherwise
  */
  virtual int ShowModal();

  /// Get the print device context
  /**
  * \return The print device context
  */
  wxPdfDC* GetPrintDC();

  /// Get the print data
  /**
  * \return The print data
  */
  wxPrintData& GetPrintData() { return m_printDialogData.GetPrintData(); }

  /// Get the print dialog data
  /**
  * \return The print dialog data
  */
  wxPrintDialogData& GetPrintDialogData() { return m_printDialogData; }

  /// Get the PDF print data
  /**
  * \return The PDF print data
  */
  wxPdfPrintData& GetPdfPrintData() { return m_pdfPrintData; }

private:

  wxTextCtrl*         m_title;
  wxTextCtrl*         m_subject;
  wxTextCtrl*         m_author;
  wxTextCtrl*         m_creator;
  wxTextCtrl*         m_keywords;

  wxTextCtrl*         m_filepath;
  wxFilePickerCtrl*   m_filepicker;
  wxCheckBox*         m_launchViewer;

  wxTextCtrl*         m_ownerpwd;
  wxTextCtrl*         m_userpwd;
  wxTextCtrl*         m_ownerpwdconfirm;
  wxTextCtrl*         m_userpwdconfirm;

  wxChoice*           m_compat;

  wxCheckBox*         m_protect;
  wxCheckBox*         m_canprint;
  wxCheckBox*         m_canmodify;
  wxCheckBox*         m_cancopy;
  wxCheckBox*         m_canannot;
  wxCheckBox*         m_canform;
  wxCheckBox*         m_canextract;
  wxCheckBox*         m_canassemble;

  wxPrintDialogData   m_printDialogData;
  wxPdfPrintData      m_pdfPrintData;

protected:
  void Init(wxWindow* parent);

private:

  void UpdateProtectionControls();
  void OnProtectCheck(wxCommandEvent&);
  void OnFilepathChanged(wxFileDirPickerEvent&);

  DECLARE_EVENT_TABLE()
  DECLARE_DYNAMIC_CLASS(wxPdfPrintDialog)
};

// ----------------------------------------------------------------------------
// wxPdfPageSetupDialogCanvas: internal use only
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_PDFDOC wxPdfPageSetupDialogCanvas : public wxWindow
{

  DECLARE_CLASS(wxPdfPageSetupDialogCanvas)

public:
  wxPdfPageSetupDialogCanvas(wxWindow *parent);

  virtual ~wxPdfPageSetupDialogCanvas();

  void OnPaint(wxPaintEvent& event);

  void UpdatePageMetrics(int paperWidth, int paperHeight, int marginLeft,
                         int marginRight, int marginTop, int marginBottom)
  {
    m_paperWidth   = paperWidth;
    m_paperHeight  = paperHeight;
    m_marginLeft   = marginLeft;
    m_marginRight  = marginRight;
    m_marginTop    = marginTop;
    m_marginBottom = marginBottom;
  }

private:

  int m_paperWidth;
  int m_paperHeight;
  int m_marginLeft;
  int m_marginRight;
  int m_marginTop;
  int m_marginBottom;

  DECLARE_EVENT_TABLE()
  DECLARE_NO_COPY_CLASS(wxPdfPageSetupDialogCanvas)
};

// ----------------------------------------------------------------------------
// wxPdfPageSetupDialog: a wxPdfDocument friendly page setup dialog
// ----------------------------------------------------------------------------

enum
{
  wxPDF_PAGEDIALOG_CTRLID_MARGINUNIT = 30,
  wxPDF_PAGEDIALOG_CTRLID_MARGINLEFT,
  wxPDF_PAGEDIALOG_CTRLID_MARGINRIGHT,
  wxPDF_PAGEDIALOG_CTRLID_MARGINTOP,
  wxPDF_PAGEDIALOG_CTRLID_MARGINBOTTOM,
  wxPDF_PAGEDIALOG_CTRLID_PAPER,
  wxPDF_PAGEDIALOG_CTRLID_ORIENTATION,
};

/// Class for the PDF page setup dialog.
class WXDLLIMPEXP_PDFDOC wxPdfPageSetupDialog : public wxDialog
{
  DECLARE_CLASS(wxPdfPageSetupDialog)

public:
  /// Constructor
  /**
  * \param parent The parent window
  * \param data The page setup dialog data
  * \param title The dialog title
  */
  wxPdfPageSetupDialog(wxWindow* parent,
                       wxPageSetupDialogData* data,
                       const wxString& title = wxEmptyString);

  /// Destructor
  virtual ~wxPdfPageSetupDialog();

  /// Handle OK button press
  /**
  * \param event The command event
  */
  void OnOK(wxCommandEvent& event);

  /// Handle margin unit change
  /**
  * \param event The command event
  */
  void OnMarginUnit(wxCommandEvent& event);

  /// Handle paper type change
  /**
  * \param event The command event
  */
  void OnPaperType(wxCommandEvent& event);

  /// Handle orientation change
  /**
  * \param event The command event
  */
  void OnOrientation(wxCommandEvent& event);

  /// Handle margin text change
  /**
  * \param event The command event
  */
  void OnMarginText(wxCommandEvent& event);

  /// Transfer data from window controls
  /**
  * \return @c true if successful, @c false otherwise
  */
  virtual bool TransferDataFromWindow();

  /// Transfer data to window controls
  /**
  * \return @c true if successful, @c false otherwise
  */
  virtual bool TransferDataToWindow();

  /// Get the page setup dialog data
  /**
  * \return The page setup dialog data
  */
  virtual wxPageSetupDialogData& GetPageSetupDialogData();

  /// Get the page setup dialog data (compatibility name)
  /**
  * \return The page setup dialog data
  */
  wxPageSetupDialogData& GetPageSetupData();

protected:

  wxChoice*       m_orientationChoice;
  wxChoice*       m_marginUnits;
  wxTextCtrl*     m_marginLeftText;
  wxTextCtrl*     m_marginTopText;
  wxTextCtrl*     m_marginRightText;
  wxTextCtrl*     m_marginBottomText;
  wxChoice*       m_paperTypeChoice;

private:
  int             m_marginLeft;
  int             m_marginTop;
  int             m_marginRight;
  int             m_marginBottom;
  wxPrintOrientation m_orientation;
  wxPaperSize     m_paperId;
  wxPaperSize     m_defaultPaperId;
  int             m_defaultUnitSelection;
  int             m_pageWidth;
  int             m_pageHeight;

  wxPdfPageSetupDialogCanvas*       m_paperCanvas;
  wxPageSetupDialogData             m_pageData;

  void Init();

  void TransferMarginsToControls();
  void TransferControlsToMargins();
  void UpdatePaperCanvas();

  DECLARE_EVENT_TABLE()
  DECLARE_NO_COPY_CLASS(wxPdfPageSetupDialog)
};


// ----------------------------------------------------------------------------
// wxPdfPreviewDC allows to return text extents from a wxPdfDC
// Internal use Only
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_PDFDOC wxPdfPreviewDCImpl : public wxDCImpl
{
public:

  wxPdfPreviewDCImpl(wxDC* owner, wxDCImpl& dc, wxPdfDC* pdfdc)
    : wxDCImpl(owner), m_dc(dc)
  {
    m_pdfdc = pdfdc;
  }

  //////////////////////////////////////////////////////////////////
  // Overrides passed to wxPdfDC
  //////////////////////////////////////////////////////////////////

  wxPdfDocument* GetPdfDocument() const
  {
    return m_pdfdc->GetPdfDocument();
  }

  virtual wxRect GetPaperRect() const
  {
    int w = 0;
    int h = 0;
    m_pdfdc->GetImpl()->DoGetSize(&w, &h);
    return wxRect(0,0,w,h);
  }

  virtual int GetResolution() const { return m_pdfdc->GetResolution(); }

  virtual bool CanGetTextExtent() const { return m_pdfdc->CanGetTextExtent(); }

  virtual wxCoord GetCharHeight() const { return m_pdfdc->GetCharHeight(); }
  virtual wxCoord GetCharWidth() const { return m_pdfdc->GetCharWidth(); }

  virtual void DoGetFontMetrics(int* height,
                                int* ascent,
                                int* descent,
                                int* internalLeading,
                                int* externalLeading,
                                int* averageWidth) const
  {
    m_pdfdc->GetImpl()->DoGetFontMetrics(height, ascent, descent, internalLeading, externalLeading, averageWidth);
  }

  virtual void DoGetTextExtent(const wxString& string,
                               wxCoord* x, wxCoord* y,
                               wxCoord* descent = NULL,
                               wxCoord* externalLeading = NULL,
                               const wxFont* theFont = NULL) const
  {
    m_pdfdc->GetTextExtent(string, x, y, descent, externalLeading, theFont);
  }

  virtual bool DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const
  {
    return m_pdfdc->GetPartialTextExtents(text, widths);
  }

  virtual void GetMultiLineTextExtent(const wxString& string,
                                      wxCoord* width,
                                      wxCoord* height,
                                      wxCoord* heightLine = NULL,
                                      const wxFont* font = NULL) const
  {
    m_pdfdc->GetMultiLineTextExtent(string, width, height, heightLine, font);
  }

  virtual wxSize GetPPI() const { return m_pdfdc->GetPPI(); }

private:

  void UpdateBoundingBox() const
  {
    ((wxDCImpl*)this)->wxDCImpl::CalcBoundingBox(m_dc.MinX(), m_dc.MinY());
    ((wxDCImpl*)this)->wxDCImpl::CalcBoundingBox(m_dc.MaxX(), m_dc.MaxY());
  }

public:

  //////////////////////////////////////////////////////////////////
  // Overrides passed to wxMemory DC
  //////////////////////////////////////////////////////////////////

  virtual void DoGetSize(int* w, int* h) const
  {
    m_dc.DoGetSize(w,h);
  }

  virtual void DoGetSizeMM(int* w, int* h) const
  {
    m_dc.DoGetSizeMM(w,h);
  }

  virtual bool IsOk() const { return m_dc.IsOk(); }

  // wxDCBase operations
  virtual bool CanDrawBitmap() const { return m_dc.CanDrawBitmap(); }

  virtual void Clear() { m_dc.Clear(); }

  virtual int GetDepth() const { return m_dc.GetDepth(); }

  virtual void CalcBoundingBox(wxCoord x, wxCoord y)
  {
    m_dc.CalcBoundingBox( x, y);
    UpdateBoundingBox();
  }

  virtual void SetFont(const wxFont& font)
  {
    m_dc.SetFont(font);
    m_pdfdc->SetFont(font);
  }

  virtual const wxFont& GetFont() const { return m_dc.GetFont(); }

  virtual void SetPen(const wxPen& pen) { m_dc.SetPen(pen); }
  virtual const wxPen& GetPen() const { return m_dc.GetPen(); }
  virtual void SetBrush(const wxBrush& brush) { m_dc.SetBrush(brush); }
  virtual const wxBrush& GetBrush() const { return m_dc.GetBrush(); }
  virtual void SetBackground(const wxBrush& brush) { m_dc.SetBackground(brush); }
  virtual const wxBrush& GetBackground() const { return m_dc.GetBackground(); }
  virtual void SetBackgroundMode(int mode){ m_dc.SetBackgroundMode(mode); }
  virtual int GetBackgroundMode() const { return m_dc.GetBackgroundMode(); }

  virtual void SetTextForeground(const wxColour& colour) { m_dc.SetTextForeground(colour); }
  virtual const wxColour& GetTextForeground() const { return m_dc.GetTextForeground(); }

  virtual void SetTextBackground(const wxColour& colour) { m_dc.SetTextBackground(colour); }
  virtual const wxColour& GetTextBackground() const { return m_dc.GetTextBackground(); }

#if wxUSE_PALETTE
  virtual void SetPalette(const wxPalette& palette) { m_dc.SetPalette(palette); }
#endif // wxUSE_PALETTE

  virtual void InheritAttributes(wxWindow* win) { m_dc.InheritAttributes(win); }

  virtual void SetLogicalFunction(wxRasterOperationMode function) { m_dc.SetLogicalFunction(function); }
  virtual wxRasterOperationMode GetLogicalFunction() const { return m_dc.GetLogicalFunction(); }

  virtual void DoSetClippingRegion(wxCoord x, wxCoord y, wxCoord w, wxCoord h)
  {
    m_dc.DoSetClippingRegion(x,y,w,h);
    UpdateBoundingBox();
  }

  virtual void DoSetDeviceClippingRegion(const wxRegion& region)
  {
    m_dc.DoSetDeviceClippingRegion(region);
    UpdateBoundingBox();
  }

#if wxCHECK_VERSION(3, 1, 2)
  virtual bool DoGetClippingRect(wxRect& rect) const
  {
    return m_dc.DoGetClippingRect(rect);
  }
#else // wx < 3.1.2
  virtual void DoGetClippingBox(wxCoord* x, wxCoord* y, wxCoord* w, wxCoord* h) const
  {
    m_dc.DoGetClippingBox(x, y, w, h);
    UpdateBoundingBox();
  }
#endif // wx 3.1.2 or not

  virtual void DestroyClippingRegion()
  {
    m_dc.DestroyClippingRegion();
    UpdateBoundingBox();
  }

  virtual wxCoord DeviceToLogicalX(wxCoord x) const { return m_dc.DeviceToLogicalX(x); }
  virtual wxCoord DeviceToLogicalY(wxCoord y) const { return m_dc.DeviceToLogicalY(y); }
  virtual wxCoord DeviceToLogicalXRel(wxCoord x) const { return m_dc.DeviceToLogicalXRel(x); }
  virtual wxCoord DeviceToLogicalYRel(wxCoord y) const { return m_dc.DeviceToLogicalYRel(y); }
  virtual wxCoord LogicalToDeviceX(wxCoord x) const { return m_dc.LogicalToDeviceX(x); }
  virtual wxCoord LogicalToDeviceY(wxCoord y) const { return m_dc.LogicalToDeviceY(y); }
  virtual wxCoord LogicalToDeviceXRel(wxCoord x) const { return m_dc.LogicalToDeviceXRel(x); }
  virtual wxCoord LogicalToDeviceYRel(wxCoord y) const { return m_dc.LogicalToDeviceYRel(y); }

  virtual void SetMapMode(wxMappingMode mode) { m_dc.SetMapMode(mode); }
  virtual wxMappingMode GetMapMode() const { return m_dc.GetMapMode(); }

  virtual void SetUserScale(double x, double y) { m_dc.SetUserScale(x,y); }
  virtual void GetUserScale(double* x, double* y) const { m_dc.GetUserScale(x, y); }

  virtual void SetLogicalScale(double x, double y) { m_dc.SetLogicalScale(x,y); }
  virtual void GetLogicalScale(double* x, double* y) const { m_dc.GetLogicalScale(x,y); }

  virtual void SetLogicalOrigin(wxCoord x, wxCoord y) { m_dc.SetLogicalOrigin(x,y); }
  virtual void DoGetLogicalOrigin(wxCoord* x, wxCoord* y) const { m_dc.DoGetLogicalOrigin(x,y); }

  virtual void SetDeviceOrigin(wxCoord x, wxCoord y) { m_dc.SetDeviceOrigin(x,y); }
  virtual void DoGetDeviceOrigin(wxCoord* x, wxCoord* y) const { m_dc.DoGetDeviceOrigin(x,y); }

  virtual void SetDeviceLocalOrigin( wxCoord x, wxCoord y ) { m_dc.SetDeviceLocalOrigin(x,y); }
  virtual void ComputeScaleAndOrigin() { m_dc.ComputeScaleAndOrigin(); }
  virtual void SetAxisOrientation(bool x, bool y) { m_dc.SetAxisOrientation(x,y); }

  virtual bool DoFloodFill(wxCoord x, wxCoord y, const wxColour& col,
                           wxFloodFillStyle style = wxFLOOD_SURFACE)
  {
    bool rval = m_dc.DoFloodFill(x, y, col, style);
    UpdateBoundingBox();
    return rval;
  }

  virtual void DoGradientFillLinear(const wxRect& rect,
                                    const wxColour& initialColour,
                                    const wxColour& destColour,
                                    wxDirection nDirection = wxEAST)
  {
    m_dc.DoGradientFillLinear(rect, initialColour, destColour, nDirection);
    UpdateBoundingBox();
  }

  virtual void DoGradientFillConcentric(const wxRect& rect,
                                        const wxColour& initialColour,
                                        const wxColour& destColour,
                                        const wxPoint& circleCenter)
  {
    m_dc.DoGradientFillConcentric(rect, initialColour, destColour, circleCenter);
    UpdateBoundingBox();
  }

  virtual bool DoGetPixel(wxCoord x, wxCoord y, wxColour* col) const
  {
    bool rval = m_dc.DoGetPixel(x, y, col);
    UpdateBoundingBox();
    return rval;
  }

  virtual void DoDrawPoint(wxCoord x, wxCoord y)
  {
    m_dc.DoDrawPoint(x,y);
    UpdateBoundingBox();
  }

  virtual void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
  {
    m_dc.DoDrawLine(x1,y1,x2,y2);
    UpdateBoundingBox();
  }

  virtual void DoDrawArc(wxCoord x1, wxCoord y1,
                         wxCoord x2, wxCoord y2,
                         wxCoord xc, wxCoord yc)
  {
    m_dc.DoDrawArc(x1,y1, x2,y2, xc, yc);
    UpdateBoundingBox();
  }

  virtual void DoDrawCheckMark(wxCoord x, wxCoord y,
                               wxCoord w, wxCoord h)
  {
    m_dc.DoDrawCheckMark(x,y,w,h);
    UpdateBoundingBox();
  }

  virtual void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                                 double sa, double ea)
  {
    m_dc.DoDrawEllipticArc(x,y,w,h,sa, ea);
    UpdateBoundingBox();
  }

  virtual void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord w, wxCoord h)
  {
    m_dc.DoDrawRectangle(x,y,w,h);
    UpdateBoundingBox();
  }

  virtual void DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                      wxCoord w, wxCoord h,
                                      double radius)
  {
    m_dc.DoDrawRoundedRectangle(x,y,w,h,radius);
    UpdateBoundingBox();
  }

  virtual void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord w, wxCoord h)
  {
    m_dc.DoDrawEllipse(x,y,w,h);
    UpdateBoundingBox();
  }

  virtual void DoCrossHair(wxCoord x, wxCoord y)
  {
    m_dc.DoCrossHair(x,y);
    UpdateBoundingBox();
  }

  virtual void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
  {
    m_dc.DoDrawIcon(icon, x,y);
    UpdateBoundingBox();
  }

  virtual void DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y,
                            bool useMask = false)
  {
    m_dc.DoDrawBitmap(bmp, x,y, useMask);
    UpdateBoundingBox();
  }

  virtual void DoDrawText(const wxString& text, wxCoord x, wxCoord y)
  {
    m_dc.DoDrawText(text, x, y);
    UpdateBoundingBox();
  }

  virtual void DoDrawRotatedText(const wxString& text,
                                 wxCoord x, wxCoord y, double angle)
  {
    m_dc.DoDrawRotatedText(text, x, y, angle);
    UpdateBoundingBox();
  }

  virtual bool DoBlit(wxCoord xdest, wxCoord ydest,
                      wxCoord w, wxCoord h,
                      wxDC* source, wxCoord xsrc, wxCoord ysrc,
                      wxRasterOperationMode rop = wxCOPY,
                      bool useMask = false,
                      wxCoord xsrcMask = wxDefaultCoord, wxCoord ysrcMask = wxDefaultCoord)
  {
    bool rval = m_dc.DoBlit(xdest, ydest, w, h, source, xsrc, ysrc, rop, useMask, xsrcMask, ysrcMask);
    UpdateBoundingBox();
    return rval;
  }

  virtual bool DoStretchBlit(wxCoord xdest, wxCoord ydest,
                             wxCoord dstWidth, wxCoord dstHeight,
                             wxDC* source,
                             wxCoord xsrc, wxCoord ysrc,
                             wxCoord srcWidth, wxCoord srcHeight,
                             wxRasterOperationMode rop = wxCOPY,
                             bool useMask = false,
                             wxCoord xsrcMask = wxDefaultCoord,
                             wxCoord ysrcMask = wxDefaultCoord)
  {
    bool rval = m_dc.DoStretchBlit(xdest, ydest, dstWidth, dstHeight, source, xsrc, ysrc,
                                   srcWidth, srcHeight, rop, useMask, xsrcMask, ysrcMask);
    UpdateBoundingBox();
    return rval;
  }

  virtual void DoDrawLines(int n, const wxPoint points[],
                           wxCoord xoffset, wxCoord yoffset)
  {
    m_dc.DoDrawLines(n, points,xoffset, yoffset);
    UpdateBoundingBox();
  }

  virtual void DoDrawPolygon(int n, const wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset,
                             wxPolygonFillMode fillStyle = wxODDEVEN_RULE)
  {
    m_dc.DoDrawPolygon(n, points, xoffset, yoffset, fillStyle);
    UpdateBoundingBox();
  }

  virtual void DoSetClippingRegionAsRegion(const wxRegion& region)
  {
    wxCoord x, y, w, h;
    region.GetBox(x, y, w, h);
    m_dc.DoSetClippingRegion(x, y, w, h);
    UpdateBoundingBox();
  }

private:
  wxDCImpl&    m_dc;
  wxPdfDC*  m_pdfdc;

  wxDECLARE_NO_COPY_CLASS(wxPdfPreviewDCImpl);
};

class WXDLLIMPEXP_PDFDOC wxPdfPreviewDC : public wxDC
{
public:
  wxPdfPreviewDC(wxDC& dc, wxPdfDC* pdfdc)
    : wxDC(new wxPdfPreviewDCImpl(this, *dc.GetImpl(), pdfdc)) { }

private:

    wxDECLARE_NO_COPY_CLASS(wxPdfPreviewDC);
};

#endif //  _PDF_PRINTING_H_


