///////////////////////////////////////////////////////////////////////////////
// Name:        samples/listctrl/listctrlsample.h
// Purpose:     wxListCtrl export demo
// Author:      Blake Madden
// Created:     2026-06-10
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _LISTCTRL_SAMPLE_H_
#define _LISTCTRL_SAMPLE_H_

#include <wx/wx.h>
#include <wx/listctrl.h>

class MyFrame : public wxFrame
{
public:
  MyFrame(const wxString& title);

private:
  void OnExportDirect(wxCommandEvent& event);
  void OnExportDC(wxCommandEvent& event);
  void OnExportFullWidth(wxCommandEvent& event);
  void OnExportSimple(wxCommandEvent& event);
  void OnExportFinancial(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

  wxListCtrl* m_list;
  wxImageList* m_imageList;

  void PopulateList();

  wxDECLARE_EVENT_TABLE();
};

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
};

enum
{
  ID_EXPORT_DIRECT = wxID_HIGHEST + 1,
  ID_EXPORT_DC,
  ID_EXPORT_FULL_WIDTH,
  ID_EXPORT_SIMPLE,
  ID_EXPORT_FINANCIAL
};

#endif
