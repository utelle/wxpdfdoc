///////////////////////////////////////////////////////////////////////////////
// Name:        layers.cpp
// Purpose:     Demonstration of layers in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2009-07-06
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/pdfdoc.h"
#include "wx/pdflayer.h"

/**
* Layers
*
* These examples demonstrate the use of layers.
*/

/**
 * OrderedLayers demonstrates how to order optional content groups.
 */
void OrderedLayers()
{
  wxPdfDocument pdf;
  pdf.Open();
  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l1 = pdf.AddLayer(wxT("Layer 1"));
	wxPdfLayer* l2 = pdf.AddLayer(wxT("Layer 2"));
	wxPdfLayer* l3 = pdf.AddLayer(wxT("Layer 3"));
	wxPdfLayerMembership* m1 = pdf.AddLayerMembership();
	m1->AddMember(l2);
	m1->AddMember(l3);

  pdf.SetTextColour(wxPdfColour(wxString(wxT("red"))));
  pdf.SetFont(wxT("Helvetica"),wxT("B"),20);
  pdf.Cell(0,6,wxT("Ordered layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxT("black"))));
  pdf.SetFont(wxT("Helvetica"),wxT(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxT("Text in layer 1"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(m1);
  pdf.Cell(0,6,wxT("Text in layer 2 or layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxT("Text in layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();

  pdf.SaveAsFile(wxT("layersordered.pdf"));
}

/**
 * GroupedLayers demonstrates how to group optional content.
 */
void GroupedLayers()
{
  wxPdfDocument pdf;
  pdf.Open();

  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l1 = pdf.AddLayer(wxT("Layer 1"));
	wxPdfLayer* l2 = pdf.AddLayer(wxT("Layer 2"));
	wxPdfLayer* l3 = pdf.AddLayer(wxT("Layer 3"));
  wxPdfLayer* l0 = pdf.AddLayerTitle(wxT("A group of two"));
  l0->AddChild(l2);
  l0->AddChild(l3);

  wxPdfLayerMembership* m1 = pdf.AddLayerMembership();
	m1->AddMember(l2);
	m1->AddMember(l3);

  pdf.SetTextColour(wxPdfColour(wxString(wxT("red"))));
  pdf.SetFont(wxT("Helvetica"),wxT("B"),20);
  pdf.Cell(0,6,wxT("Grouping layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxT("black"))));
  pdf.SetFont(wxT("Helvetica"),wxT(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxT("Text in layer 1"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(m1);
  pdf.Cell(0,6,wxT("Text in layer 2 or layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxT("Text in layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();
            
  pdf.SaveAsFile(wxT("layersgrouped.pdf"));
}

/**
 * NestedLayers demonstrates the use of nested layers.
 */
void NestedLayers()
{
  wxPdfDocument pdf;
  pdf.Open();

  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l1 = pdf.AddLayer(wxT("Layer 1"));
  wxPdfLayer* l23 = pdf.AddLayer(wxT("Top Layer 2 3"));
	wxPdfLayer* l2 = pdf.AddLayer(wxT("Layer 2"));
	wxPdfLayer* l3 = pdf.AddLayer(wxT("Layer 3"));
  l23->AddChild(l2);
  l23->AddChild(l3);

  pdf.SetTextColour(wxPdfColour(wxString(wxT("red"))));
  pdf.SetFont(wxT("Helvetica"),wxT("B"),20);
  pdf.Cell(0,6,wxT("Nesting layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxT("black"))));
  pdf.SetFont(wxT("Helvetica"),wxT(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxT("Text in layer 1"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l23);
  pdf.EnterLayer(l2);
  pdf.Cell(0,6,wxT("Text in layer 2"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxT("Text in layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();
	pdf.LeaveLayer();

  pdf.SaveAsFile(wxT("layersnested.pdf"));
}

/**
 * AutomaticLayers demonstrates automatic layer grouping and nesting
 */
void AutomaticLayers()
{
  wxPdfDocument pdf;
  pdf.Open();
  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l12 = pdf.AddLayer(wxT("Layer nesting"));
  wxPdfLayer* l1 = pdf.AddLayer(wxT("Layer 1"));
	wxPdfLayer* l2 = pdf.AddLayer(wxT("Layer 2"));
  wxPdfLayer* l34 = pdf.AddLayerTitle(wxT("Layer grouping"));
	wxPdfLayer* l3 = pdf.AddLayer(wxT("Layer 3"));
	wxPdfLayer* l4 = pdf.AddLayer(wxT("Layer 4"));
  l12->AddChild(l1);
  l12->AddChild(l2);
  l34->AddChild(l3);
  l34->AddChild(l4);

  pdf.SetTextColour(wxPdfColour(wxString(wxT("red"))));
  pdf.SetFont(wxT("Helvetica"),wxT("B"),20);
  pdf.Cell(0,6,wxT("Automatic grouping and nesting of layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxT("black"))));
  pdf.SetFont(wxT("Helvetica"),wxT(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxT("Text in layer 1"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l2);
  pdf.Cell(0,6,wxT("Text in layer 2"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxT("Text in layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l4);
  pdf.Cell(0,6,wxT("Text in layer 4"));
  pdf.Ln(15);
	pdf.LeaveLayer();

  pdf.SaveAsFile(wxT("layersautomatic.pdf"));
}

/**
 * LayerRadioGroup demonstrates radio group and zoom.
 */
void LayerRadioGroup()
{
  wxPdfDocument pdf;
  pdf.Open();
  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* lrg = pdf.AddLayerTitle(wxT("Layer radio group"));
  wxPdfLayer* l1 = pdf.AddLayer(wxT("Layer 1"));
	wxPdfLayer* l2 = pdf.AddLayer(wxT("Layer 2"));
	wxPdfLayer* l3 = pdf.AddLayer(wxT("Layer 3"));
	wxPdfLayer* l4 = pdf.AddLayer(wxT("Layer 4"));
  lrg->AddChild(l1);
  lrg->AddChild(l2);
  lrg->AddChild(l3);

  l4->SetZoom(2, -1);
  l4->SetOnPanel(false);
  l4->SetPrint(wxT("Print"), true);
  l2->SetOn(false);
  l3->SetOn(false);

  wxPdfLayerGroup radio;
  radio.Add(l1);
  radio.Add(l2);
  radio.Add(l3);
  pdf.AddLayerRadioGroup(radio);

  pdf.SetTextColour(wxPdfColour(wxString(wxT("red"))));
  pdf.SetFont(wxT("Helvetica"),wxT("B"),20);
  pdf.Cell(0,6,wxT("Layer radio group and zoom"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxT("black"))));
  pdf.SetFont(wxT("Helvetica"),wxT(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxT("Text in layer 1"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l2);
  pdf.Cell(0,6,wxT("Text in layer 2"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxT("Text in layer 3"));
  pdf.Ln(15);
	pdf.LeaveLayer();
  pdf.EnterLayer(l4);
  pdf.Cell(30,6,wxT("Text in layer 4"));
	pdf.LeaveLayer();
  pdf.SetTextColour(wxPdfColour(wxString(wxT("blue"))));
  pdf.SetFont(wxT("Courier"),wxT(""),12);
  pdf.Cell(0, 6, wxT("<< Zoom here (200% or more)!"));
  pdf.Ln(15);

  pdf.SaveAsFile(wxT("layersradiogroup.pdf"));
}

void
layers()
{
  OrderedLayers();
  GroupedLayers();
  NestedLayers();
  AutomaticLayers();
  LayerRadioGroup();
}
