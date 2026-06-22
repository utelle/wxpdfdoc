///////////////////////////////////////////////////////////////////////////////
// Name:        samples/gridctrl/gridctrlsample.cpp
// Purpose:     wxGrid PDF export demo
// Author:      Blake Madden
// Created:     2026-06-22
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/artprov.h>

#include "wx/pdfdoc.h"
#include "wx/pdfgridctrl.h"
#include "wx/pdfdc.h"

#include "gridctrlsample.h"

#ifndef __WXMSW__
#include "mondrian.xpm"
#endif

// ---------------------------------------------------------------------------
// Icon provider for OnExportWithIcons: supplies column-header bitmaps sourced
// from wxArtProvider so the sample has no external image dependencies.
// ---------------------------------------------------------------------------
class SampleHeaderIconProvider : public wxPdfGridBitmapProvider
{
public:
  explicit SampleHeaderIconProvider(const wxGrid* grid)
  {
    // Build one bitmap per column using standard art icons
    static const wxArtID icons[] = {
      wxART_INFORMATION, wxART_WARNING, wxART_ERROR,
      wxART_TICK_MARK,   wxART_QUESTION
    };
    const wxSize sz(16, 16);
    const int nCols = grid->GetNumberCols();
    m_bitmaps.resize(nCols);
    for (int c = 0; c < nCols; ++c)
    {
      wxBitmap bmp = wxArtProvider::GetBitmap(icons[c % 5], wxART_MENU, sz);
      if (bmp.IsOk())
        m_bitmaps[c] = bmp;
    }
  }

  wxBitmap GetColHeaderBitmap(int col) const override
  {
    if (col >= 0 && col < static_cast<int>(m_bitmaps.size()))
      return m_bitmaps[col];
    return wxNullBitmap;
  }

private:
  std::vector<wxBitmap> m_bitmaps;
};

// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(ID_EXPORT_DIRECT,     MyFrame::OnExportDirect)
  EVT_MENU(ID_EXPORT_DC,         MyFrame::OnExportDC)
  EVT_MENU(ID_EXPORT_FULL_WIDTH, MyFrame::OnExportFullWidth)
  EVT_MENU(ID_EXPORT_SIMPLE,     MyFrame::OnExportSimple)
  EVT_MENU(ID_EXPORT_ROW_LABELS, MyFrame::OnExportWithRowLabels)
  EVT_MENU(ID_EXPORT_RANGE,      MyFrame::OnExportRange)
  EVT_MENU(ID_EXPORT_FINANCIAL,  MyFrame::OnExportFinancial)
  EVT_MENU(ID_EXPORT_WITH_ICONS, MyFrame::OnExportWithIcons)
  EVT_MENU(wxID_EXIT,            MyFrame::OnExit)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600))
{
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_EXPORT_DIRECT,     "&Export to PDF (Direct)\tCtrl-E",
                   "Export using wxPdfDocument::AddGrid");
  menuFile->Append(ID_EXPORT_DC,         "Export to PDF (via &DC)\tCtrl-D",
                   "Export using wxPdfDC::DrawGrid");
  menuFile->Append(ID_EXPORT_FULL_WIDTH, "Export to PDF (&Full Width)\tCtrl-F",
                   "Export with columns stretched to page width");
  menuFile->Append(ID_EXPORT_SIMPLE,     "Export to PDF (&Simple/LaTeX)\tCtrl-L",
                   "Export with LaTeX booktabs-style rules only");
  menuFile->Append(ID_EXPORT_ROW_LABELS, "Export with &Row Labels\tCtrl-W",
                   "Export including the row label column");
  menuFile->Append(ID_EXPORT_RANGE,      "Export &Range\tCtrl-R",
                   "Export a specific row/column range");
  menuFile->Append(ID_EXPORT_FINANCIAL,  "Export Financial &Ledger (Landscape)\tCtrl-G",
                   "Export a sample financial ledger with long note cells");
  menuFile->Append(ID_EXPORT_WITH_ICONS, "Export with Column Header &Icons\tCtrl-I",
                   "Export with bitmaps in column headers via wxPdfGridBitmapProvider");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  SetMenuBar(menuBar);

  m_grid = new wxGrid(this, wxID_ANY);
  PopulateGrid();
}

void MyFrame::PopulateGrid()
{
  m_grid->CreateGrid(0, 5);

  m_grid->SetColLabelValue(0, "ID");
  m_grid->SetColLabelValue(1, "Name");
  m_grid->SetColLabelValue(2, "Status");
  m_grid->SetColLabelValue(3, "Value");
  m_grid->SetColLabelValue(4, "Notes");

  m_grid->SetColSize(0, 50);
  m_grid->SetColSize(1, 200);
  m_grid->SetColSize(2, 100);
  m_grid->SetColSize(3, 100);
  m_grid->SetColSize(4, 250);

  m_grid->SetColFormatNumber(0);
  m_grid->SetColFormatFloat(3, -1, 2);

  // Column alignments
  m_grid->SetColAttr(0, []{
    auto* a = new wxGridCellAttr; a->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE); return a; }());
  m_grid->SetColAttr(3, []{
    auto* a = new wxGridCellAttr; a->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE); return a; }());
  m_grid->SetColAttr(2, []{
    auto* a = new wxGridCellAttr; a->SetAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE); return a; }());

  for (int i = 0; i < 200; ++i)
  {
    m_grid->AppendRows(1);
    int row = m_grid->GetNumberRows() - 1;

    m_grid->SetCellValue(row, 0, wxString::Format("%d", i + 1));
    m_grid->SetCellValue(row, 1, wxString::Format("Item Name %d", i + 1));

    wxString status;
    if (i % 3 == 0)
      status = "OK";
    else if (i % 3 == 1)
      status = "Warning";
    else
      status = "Error";
    m_grid->SetCellValue(row, 2, status);

    m_grid->SetCellValue(row, 3, wxString::Format("%.2f", static_cast<double>(i) * 1.23));
    m_grid->SetCellValue(row, 4, "This is a note.");

    // Per-cell colours
    if (status == "OK")
    {
      m_grid->SetCellBackgroundColour(row, 2, wxColour(200, 255, 200));
    }
    else if (status == "Warning")
    {
      m_grid->SetCellTextColour(row, 2, *wxRED);
    }
    else
    {
      m_grid->SetCellTextColour(row, 2, wxColour(180, 0, 0));
      m_grid->SetCellBackgroundColour(row, 2, wxColour(255, 220, 220));
    }
  }

  m_grid->AutoSizeColumns(false);
}

void MyFrame::OnExportDirect(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_direct.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfGridOptions options;
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(200, 200, 200));

  doc.AddGrid(m_grid, options);
  doc.SaveAsFile(dlg.GetPath());
}

void MyFrame::OnExportDC(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_dc.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPrintData printData;
  printData.SetFilename(dlg.GetPath());

  wxPdfDC dc(printData);
  dc.StartDoc("Grid Export via DC");
  dc.StartPage();

  dc.SetFont(*wxSWISS_FONT);
  dc.DrawText("Exported Grid via wxPdfDC", 10, 10);

  wxPdfGridOptions options;
  options.SetBorderColour(wxPdfColour(200, 200, 200));

  dc.DrawGrid(m_grid, 10, 30, options);

  dc.EndPage();
  dc.EndDoc();
}

void MyFrame::OnExportFullWidth(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_fullwidth.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfGridOptions options;
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(200, 200, 200));
  options.SetFitToPage(true);

  doc.AddGrid(m_grid, options);
  doc.SaveAsFile(dlg.GetPath());
}

void MyFrame::OnExportSimple(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_simple.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfGridOptions options;
  options.SetStyle(wxPDF_GRID_STYLE_SIMPLE);
  options.SetFitToPage(true);

  doc.AddGrid(m_grid, options);
  doc.SaveAsFile(dlg.GetPath());
}

void MyFrame::OnExportWithRowLabels(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_rowlabels.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfGridOptions options;
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(200, 200, 200));
  options.SetIncludeRowLabels(true);

  doc.AddGrid(m_grid, options);
  doc.SaveAsFile(dlg.GetPath());
}

void MyFrame::OnExportRange(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_range.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfGridOptions options;
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(200, 200, 200));
  // Export only rows 10-20 (1-indexed), columns 2-4 (Name, Status, Value)
  options.SetFromRow(10);
  options.SetToRow(20);
  options.SetFromColumn(2);
  options.SetToColumn(4);
  options.SetIncludeColumnHeaders(false);

  doc.AddGrid(m_grid, options);
  doc.SaveAsFile(dlg.GetPath());
}

void MyFrame::OnExportFinancial(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_financial.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxGrid* fin = new wxGrid(this, wxID_ANY);
  fin->Hide();
  fin->CreateGrid(0, 8);

  fin->SetColLabelValue(0, "Date");
  fin->SetColLabelValue(1, "Ref #");
  fin->SetColLabelValue(2, "Account");
  fin->SetColLabelValue(3, "Description");
  fin->SetColLabelValue(4, "Debit ($)");
  fin->SetColLabelValue(5, "Credit ($)");
  fin->SetColLabelValue(6, "Balance ($)");
  fin->SetColLabelValue(7, "Notes");

  fin->SetColSize(0,  80);
  fin->SetColSize(1,  70);
  fin->SetColSize(2, 130);
  fin->SetColSize(3, 160);
  fin->SetColSize(4,  80);
  fin->SetColSize(5,  80);
  fin->SetColSize(6,  90);
  fin->SetColSize(7, 300);

  // Right-align the numeric columns
  for (int c : { 4, 5, 6 })
  {
    auto* attr = new wxGridCellAttr;
    attr->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    fin->SetColAttr(c, attr);
  }

  struct FinRow
  {
    const char* date;
    const char* ref;
    const char* account;
    const char* description;
    const char* debit;
    const char* credit;
    const char* balance;
    const char* notes;
  };

  static const FinRow rows[] =
  {
    { "2026-01-03", "REF-0001", "Cash - Operating",  "January office rent",
      "4,500.00",  "",          "95,500.00",
      "Monthly lease payment to the building landlord for the primary office space at Suite 400. "
      "Annual term ending December 2027 with a 3% escalation clause at renewal." },
    { "2026-01-05", "REF-0002", "Accounts Rec.",     "Invoice #1042 - Client A",
      "",          "12,750.00", "108,250.00",
      "Full settlement of invoice #1042 for software consulting services delivered in Q4 2025. "
      "Payment received 15 days before the net-30 due date; no early-payment discount applied." },
    { "2026-01-08", "REF-0003", "Payroll Expense",   "Bi-weekly payroll",
      "18,320.50", "",          "89,929.50",
      "Regular bi-weekly payroll for 12 salaried employees and 3 hourly contractors. "
      "Includes employer FICA of $1,402.32 and group health insurance premiums of $2,160.00." },
    { "2026-01-10", "REF-0004", "Equipment",         "Laptops - IT dept.",
      "3,280.00",  "",          "86,649.50",
      "Purchase of 2 laptops for new engineering hires per capital expenditure request CE-2026-004, "
      "approved by CFO on 2025-12-18. Depreciation schedule: 3 years straight-line." },
    { "2026-01-12", "REF-0005", "Accounts Rec.",     "Invoice #1038 - Client B",
      "",          "8,400.00",  "95,049.50",
      "Partial payment of 70% on invoice #1038. Remaining balance of $3,600.00 due 2026-02-01 "
      "per amended payment plan agreed with the client on 2025-12-22." },
    { "2026-01-15", "REF-0006", "Utilities",         "Monthly utilities - Q1",
      "1,240.75",  "",          "93,808.75",
      "Combined electric, gas, and water charges for the main facility. Electricity usage up 8% "
      "vs. prior month due to HVAC load from a cold snap; flagged for facilities review." },
    { "2026-01-19", "REF-0007", "Professional Svcs", "Legal retainer - January",
      "2,500.00",  "",          "91,308.75",
      "Monthly retainer to outside legal counsel for contract review and IP advisory services. "
      "Covers up to 10 billable hours; overages billed separately at $350 per hour." },
    { "2026-01-22", "REF-0008", "Cash - Operating",  "Vendor deposit - SaaS provider",
      "5,000.00",  "",          "86,308.75",
      "Advance deposit for a 12-month SaaS infrastructure contract signed 2026-01-20. "
      "Remaining balance of $55,000.00 due 30 days after the agreed go-live milestone." },
    { "2026-01-26", "REF-0009", "Revenue",           "Subscription renewals - Jan",
      "",          "24,600.00", "110,908.75",
      "Aggregate monthly renewals from 82 active enterprise accounts, including 3 new accounts "
      "onboarded in January. Churn rate for the period was 1.2%; flagged for retention review." },
    { "2026-01-31", "REF-0010", "Cash - Operating",  "Bank service fees - January",
      "87.50",     "",          "110,821.25",
      "Monthly account maintenance and wire transfer charges; fee schedule unchanged from 2025. "
      "Under review as part of the Q1 banking relationship assessment initiated by the CFO." },
  };

  for (const auto& r : rows)
  {
    fin->AppendRows(1);
    int row = fin->GetNumberRows() - 1;
    fin->SetCellValue(row, 0, r.date);
    fin->SetCellValue(row, 1, r.ref);
    fin->SetCellValue(row, 2, r.account);
    fin->SetCellValue(row, 3, r.description);
    fin->SetCellValue(row, 4, r.debit);
    fin->SetCellValue(row, 5, r.credit);
    fin->SetCellValue(row, 6, r.balance);
    fin->SetCellValue(row, 7, r.notes);
  }

  wxPdfDocument doc(wxLANDSCAPE);
  doc.AddPage();

  wxPdfGridOptions options;
  options.SetStyle(wxPDF_GRID_STYLE_SIMPLE);
  options.SetFitToPage(true);

  doc.AddGrid(fin, options);
  doc.SaveAsFile(dlg.GetPath());
  fin->Destroy();
}

void MyFrame::OnExportWithIcons(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog dlg(this, _("Save PDF file"), "", "grid_icons.pdf",
                   "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (dlg.ShowModal() == wxID_CANCEL) return;

  wxPdfDocument doc;
  doc.AddPage();

  SampleHeaderIconProvider iconProvider(m_grid);

  wxPdfGridOptions options;
  options.SetHeaderBackgroundColour(wxPdfColour(195, 220, 240));
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(180, 180, 180));
  options.SetFitToPage(true);
  options.SetBitmapProvider(&iconProvider);
  options.SetFromRow(1);
  options.SetToRow(10);

  doc.AddGrid(m_grid, options);

  doc.AddPage();
  options.SetFromRow(11);
  options.SetToRow(20);

  doc.AddGrid(m_grid, options);

  doc.SaveAsFile(dlg.GetPath());
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

bool MyApp::OnInit()
{
  MyFrame* frame = new MyFrame("wxPdfDocument wxGrid Export Sample");
  frame->SetIcon(wxICON(mondrian));
  frame->Show(true);
  return true;
}

IMPLEMENT_APP(MyApp)
