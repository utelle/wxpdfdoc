///////////////////////////////////////////////////////////////////////////////
// Name:        samples/listctrl/listctrlsample.cpp
// Purpose:     wxListCtrl export demo
// Author:      Blake Madden
// Created:     2026-06-10
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>

#include "wx/pdfdoc.h"
#include "wx/pdflistctrl.h"
#include "wx/pdfdc.h"

#include "listctrlsample.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(ID_EXPORT_DIRECT,     MyFrame::OnExportDirect)
  EVT_MENU(ID_EXPORT_DC,         MyFrame::OnExportDC)
  EVT_MENU(ID_EXPORT_FULL_WIDTH, MyFrame::OnExportFullWidth)
  EVT_MENU(ID_EXPORT_SIMPLE,     MyFrame::OnExportSimple)
  EVT_MENU(ID_EXPORT_FINANCIAL,  MyFrame::OnExportFinancial)
  EVT_MENU(wxID_EXIT,            MyFrame::OnExit)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_EXPORT_DIRECT,     "&Export to PDF (Direct)\tCtrl-E",     "Export using wxPdfDocument::AddList");
  menuFile->Append(ID_EXPORT_DC,         "Export to PDF (via &DC)\tCtrl-D",     "Export using wxPdfDC::DrawList");
  menuFile->Append(ID_EXPORT_FULL_WIDTH, "Export to PDF (&Full Width)\tCtrl-F",  "Export with columns stretched to page width");
  menuFile->Append(ID_EXPORT_SIMPLE,     "Export to PDF (&Simple/LaTeX)\tCtrl-L",    "Export with LaTeX booktabs-style rules only");
  menuFile->Append(ID_EXPORT_FINANCIAL,  "Export Financial Data (Lan&dscape)\tCtrl-G", "Export sample ledger with long note cells");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  SetMenuBar(menuBar);

  m_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);
  
  m_imageList = new wxImageList(16, 16);
  m_imageList->Add(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_MENU, wxSize(16, 16)));
  m_imageList->Add(wxArtProvider::GetIcon(wxART_WARNING, wxART_MENU, wxSize(16, 16)));
  m_imageList->Add(wxArtProvider::GetIcon(wxART_ERROR, wxART_MENU, wxSize(16, 16)));
  m_list->SetImageList(m_imageList, wxIMAGE_LIST_SMALL);

  PopulateList();
}

void MyFrame::PopulateList()
{
  m_list->InsertColumn(0, "ID", wxLIST_FORMAT_LEFT, 50);
  m_list->InsertColumn(1, "Name", wxLIST_FORMAT_LEFT, 200);
  m_list->InsertColumn(2, "Status", wxLIST_FORMAT_CENTER, 100);
  m_list->InsertColumn(3, "Value", wxLIST_FORMAT_RIGHT, 100);
  m_list->InsertColumn(4, "Notes", wxLIST_FORMAT_LEFT, 250);

  for (int i = 0; i < 200; ++i)
  {
    long index = m_list->InsertItem(i, wxString::Format("%d", i + 1));
    m_list->SetItem(index, 1, wxString::Format("Item Name %d", i + 1));
    
    wxString status;
    int imgIndex = -1;
    if (i % 3 == 0)
        { status = "OK"; imgIndex = 0; }
    else if (i % 3 == 1)
        { status = "Warning"; imgIndex = 1; }
    else
        { status = "Error"; imgIndex = 2; }
    
    m_list->SetItem(index, 2, status, imgIndex);
    
    // Set cell styling
    if (status == "OK") {
        m_list->SetItemBackgroundColour(index, wxColour(200, 255, 200)); // Light green
    } else if (status == "Warning") {
        m_list->SetItemTextColour(index, *wxRED); // Red font
    }
    
    m_list->SetItem(index, 3, wxString::Format("%.2f", (double)i * 1.23));
    m_list->SetItem(index, 4, "This is a note.");

    if (i % 2 == 0 && status != "OK") // Zebra striping
    {
      m_list->SetItemBackgroundColour(index, *wxLIGHT_GREY);
    }
  }
}

void MyFrame::OnExportDirect(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog saveFileDialog(this, _("Save PDF file"), "", "list_direct.pdf",
    "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxPdfDocument doc;
  doc.AddPage();
  
  wxPdfListCtrlOptions options;
  options.SetHeaderBackgroundColour(wxPdfColour(230, 200, 180));
  options.SetHeaderTextColour(*wxBLACK);
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(200, 200, 200));
  
  doc.AddList(m_list, options);
  doc.SaveAsFile(saveFileDialog.GetPath());
}

void MyFrame::OnExportDC(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog saveFileDialog(this, _("Save PDF file"), "", "list_dc.pdf",
    "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxPrintData printData;
  printData.SetFilename(saveFileDialog.GetPath());
  
  wxPdfDC dc(printData);
  dc.StartDoc("List Export via DC");
  dc.StartPage();
  
  dc.SetFont(*wxSWISS_FONT);
  dc.DrawText("Exported List via wxPdfDC", 10, 10);
  
  wxPdfListCtrlOptions options;
  options.SetHeaderBackgroundColour(wxPdfColour(230, 200, 180));
  options.SetHeaderTextColour(*wxBLACK);
  options.SetBorderColour(wxPdfColour(200, 200, 200));
  
  dc.DrawList(m_list, 10, 30, options);
  
  dc.EndPage();
  dc.EndDoc();
}

void MyFrame::OnExportSimple(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog saveFileDialog(this, _("Save PDF file"), "", "list_simple.pdf",
    "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfListCtrlOptions options;
  options.SetStyle(wxPDF_LISTCTRL_STYLE_SIMPLE);
  options.SetFitToPage(true);

  doc.AddList(m_list, options);
  doc.SaveAsFile(saveFileDialog.GetPath());
}

void MyFrame::OnExportFullWidth(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog saveFileDialog(this, _("Save PDF file"), "", "list_fullwidth.pdf",
    "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxPdfDocument doc;
  doc.AddPage();

  wxPdfListCtrlOptions options;
  options.SetHeaderBackgroundColour(wxPdfColour(230, 200, 180));
  options.SetHeaderTextColour(*wxBLACK);
  options.SetAlternateRowBackgroundColour(wxPdfColour(240, 240, 240));
  options.SetBorderColour(wxPdfColour(200, 200, 200));
  options.SetFitToPage(true);

  doc.AddList(m_list, options);
  doc.SaveAsFile(saveFileDialog.GetPath());
}

void MyFrame::OnExportFinancial(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog saveFileDialog(this, _("Save PDF file"), "", "list_financial.pdf",
    "PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;

  wxListCtrl* fin = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxLC_REPORT);
  fin->Hide();

  fin->InsertColumn(0, "Date",        wxLIST_FORMAT_LEFT,   80);
  fin->InsertColumn(1, "Ref #",       wxLIST_FORMAT_LEFT,   70);
  fin->InsertColumn(2, "Account",     wxLIST_FORMAT_LEFT,  130);
  fin->InsertColumn(3, "Description", wxLIST_FORMAT_LEFT,  160);
  fin->InsertColumn(4, "Debit ($)",   wxLIST_FORMAT_RIGHT,  80);
  fin->InsertColumn(5, "Credit ($)",  wxLIST_FORMAT_RIGHT,  80);
  fin->InsertColumn(6, "Balance ($)", wxLIST_FORMAT_RIGHT,  90);
  fin->InsertColumn(7, "Notes",       wxLIST_FORMAT_LEFT,  300);

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
    long idx = fin->InsertItem(fin->GetItemCount(), r.date);
    fin->SetItem(idx, 1, r.ref);
    fin->SetItem(idx, 2, r.account);
    fin->SetItem(idx, 3, r.description);
    fin->SetItem(idx, 4, r.debit);
    fin->SetItem(idx, 5, r.credit);
    fin->SetItem(idx, 6, r.balance);
    fin->SetItem(idx, 7, r.notes);
  }

  wxPdfDocument doc(wxLANDSCAPE);
  doc.AddPage();

  wxPdfListCtrlOptions options;
  options.SetStyle(wxPDF_LISTCTRL_STYLE_SIMPLE);
  options.SetFitToPage(true);

  doc.AddList(fin, options);
  doc.SaveAsFile(saveFileDialog.GetPath());
  fin->Destroy();
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

bool MyApp::OnInit()
{
  MyFrame* frame = new MyFrame("wxPdfDocument wxListCtrl Export Sample");
  frame->Show(true);
  return true;
}

IMPLEMENT_APP(MyApp)
