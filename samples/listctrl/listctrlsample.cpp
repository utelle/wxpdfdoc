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
  EVT_MENU(wxID_EXIT,            MyFrame::OnExit)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_EXPORT_DIRECT,     "&Export to PDF (Direct)\tCtrl-E",     "Export using wxPdfDocument::AddList");
  menuFile->Append(ID_EXPORT_DC,         "Export to PDF (via &DC)\tCtrl-D",     "Export using wxPdfDC::DrawList");
  menuFile->Append(ID_EXPORT_FULL_WIDTH, "Export to PDF (&Full Width)\tCtrl-F", "Export with columns stretched to page width");
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
