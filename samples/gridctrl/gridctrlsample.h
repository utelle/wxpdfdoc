///////////////////////////////////////////////////////////////////////////////
// Name:        samples/gridctrl/gridctrlsample.h
// Purpose:     wxGrid PDF export demo
// Author:      Blake Madden
// Created:     2026-06-22
// Copyright:   (c) Blake Madden
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _GRIDCTRL_SAMPLE_H_
#define _GRIDCTRL_SAMPLE_H_

#include <wx/wx.h>
#include <wx/grid.h>

class MyFrame : public wxFrame
{
public:
  MyFrame(const wxString& title);

private:
  void OnExportDirect(wxCommandEvent& event);
  void OnExportDC(wxCommandEvent& event);
  void OnExportFullWidth(wxCommandEvent& event);
  void OnExportSimple(wxCommandEvent& event);
  void OnExportWithRowLabels(wxCommandEvent& event);
  void OnExportRange(wxCommandEvent& event);
  void OnExportFinancial(wxCommandEvent& event);
  void OnExportWithIcons(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

  wxGrid* m_grid{ nullptr };

  void PopulateGrid();

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
  ID_EXPORT_ROW_LABELS,
  ID_EXPORT_RANGE,
  ID_EXPORT_FINANCIAL,
  ID_EXPORT_WITH_ICONS
};

#endif
