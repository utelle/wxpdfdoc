///////////////////////////////////////////////////////////////////////////////
// Name:        layers.cpp
// Purpose:     Demonstration of layers in wxPdfDocument
// Author:      Ulrich Telle
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
void OrderedLayers(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();
  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l1 = pdf.AddLayer(wxS("Layer 1"));
  wxPdfLayer* l2 = pdf.AddLayer(wxS("Layer 2"));
  wxPdfLayer* l3 = pdf.AddLayer(wxS("Layer 3"));
  wxPdfLayerMembership* m1 = pdf.AddLayerMembership();
  m1->AddMember(l2);
  m1->AddMember(l3);

  pdf.SetTextColour(wxPdfColour(wxString(wxS("red"))));
  pdf.SetFont(wxS("Helvetica"),wxS("B"),20);
  pdf.Cell(0,6,wxS("Ordered layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxS("black"))));
  pdf.SetFont(wxS("Helvetica"),wxS(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxS("Text in layer 1"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(m1);
  pdf.Cell(0,6,wxS("Text in layer 2 or layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxS("Text in layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();

  pdf.SaveAsFile(wxS("layersordered.pdf"));
}

/**
 * GroupedLayers demonstrates how to group optional content.
 */
void GroupedLayers(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();

  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l1 = pdf.AddLayer(wxS("Layer 1"));
  wxPdfLayer* l2 = pdf.AddLayer(wxS("Layer 2"));
  wxPdfLayer* l3 = pdf.AddLayer(wxS("Layer 3"));
  wxPdfLayer* l0 = pdf.AddLayerTitle(wxS("A group of two"));
  l0->AddChild(l2);
  l0->AddChild(l3);

  wxPdfLayerMembership* m1 = pdf.AddLayerMembership();
  m1->AddMember(l2);
  m1->AddMember(l3);

  pdf.SetTextColour(wxPdfColour(wxString(wxS("red"))));
  pdf.SetFont(wxS("Helvetica"),wxS("B"),20);
  pdf.Cell(0,6,wxS("Grouping layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxS("black"))));
  pdf.SetFont(wxS("Helvetica"),wxS(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxS("Text in layer 1"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(m1);
  pdf.Cell(0,6,wxS("Text in layer 2 or layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxS("Text in layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();

  pdf.SaveAsFile(wxS("layersgrouped.pdf"));
}

/**
 * NestedLayers demonstrates the use of nested layers.
 */
void NestedLayers(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();

  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l1 = pdf.AddLayer(wxS("Layer 1"));
  wxPdfLayer* l23 = pdf.AddLayer(wxS("Top Layer 2 3"));
  wxPdfLayer* l2 = pdf.AddLayer(wxS("Layer 2"));
  wxPdfLayer* l3 = pdf.AddLayer(wxS("Layer 3"));
  l23->AddChild(l2);
  l23->AddChild(l3);

  pdf.SetTextColour(wxPdfColour(wxString(wxS("red"))));
  pdf.SetFont(wxS("Helvetica"),wxS("B"),20);
  pdf.Cell(0,6,wxS("Nesting layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxS("black"))));
  pdf.SetFont(wxS("Helvetica"),wxS(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxS("Text in layer 1"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l23);
  pdf.EnterLayer(l2);
  pdf.Cell(0,6,wxS("Text in layer 2"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxS("Text in layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.LeaveLayer();

  pdf.SaveAsFile(wxS("layersnested.pdf"));
}

/**
 * AutomaticLayers demonstrates automatic layer grouping and nesting
 */
void AutomaticLayers(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();
  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* l12 = pdf.AddLayer(wxS("Layer nesting"));
  wxPdfLayer* l1 = pdf.AddLayer(wxS("Layer 1"));
  wxPdfLayer* l2 = pdf.AddLayer(wxS("Layer 2"));
  wxPdfLayer* l34 = pdf.AddLayerTitle(wxS("Layer grouping"));
  wxPdfLayer* l3 = pdf.AddLayer(wxS("Layer 3"));
  wxPdfLayer* l4 = pdf.AddLayer(wxS("Layer 4"));
  l12->AddChild(l1);
  l12->AddChild(l2);
  l34->AddChild(l3);
  l34->AddChild(l4);

  pdf.SetTextColour(wxPdfColour(wxString(wxS("red"))));
  pdf.SetFont(wxS("Helvetica"),wxS("B"),20);
  pdf.Cell(0,6,wxS("Automatic grouping and nesting of layers"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxS("black"))));
  pdf.SetFont(wxS("Helvetica"),wxS(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxS("Text in layer 1"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l2);
  pdf.Cell(0,6,wxS("Text in layer 2"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxS("Text in layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l4);
  pdf.Cell(0,6,wxS("Text in layer 4"));
  pdf.Ln(15);
  pdf.LeaveLayer();

  pdf.SaveAsFile(wxS("layersautomatic.pdf"));
}

/**
 * LayerRadioGroup demonstrates radio group and zoom.
 */
void LayerRadioGroup(bool testMode)
{
  wxPdfDocument pdf;
  if (testMode)
  {
    pdf.SetCreationDate(wxDateTime(1, wxDateTime::Jan, 2017));
    pdf.SetCompression(false);
  }
  pdf.Open();
  pdf.AddPage();

  // Layers appear in the order in that they were added to the document
  wxPdfLayer* lrg = pdf.AddLayerTitle(wxS("Layer radio group"));
  wxPdfLayer* l1 = pdf.AddLayer(wxS("Layer 1"));
  wxPdfLayer* l2 = pdf.AddLayer(wxS("Layer 2"));
  wxPdfLayer* l3 = pdf.AddLayer(wxS("Layer 3"));
  wxPdfLayer* l4 = pdf.AddLayer(wxS("Layer 4"));
  lrg->AddChild(l1);
  lrg->AddChild(l2);
  lrg->AddChild(l3);

  l4->SetZoom(2, -1);
  l4->SetOnPanel(false);
  l4->SetPrint(wxS("Print"), true);
  l2->SetOn(false);
  l3->SetOn(false);

  wxPdfLayerGroup radio;
  radio.Add(l1);
  radio.Add(l2);
  radio.Add(l3);
  pdf.AddLayerRadioGroup(radio);

  pdf.SetTextColour(wxPdfColour(wxString(wxS("red"))));
  pdf.SetFont(wxS("Helvetica"),wxS("B"),20);
  pdf.Cell(0,6,wxS("Layer radio group and zoom"));
  pdf.Ln(25);
  pdf.SetTextColour(wxPdfColour(wxString(wxS("black"))));
  pdf.SetFont(wxS("Helvetica"),wxS(""),12);

  pdf.EnterLayer(l1);
  pdf.Cell(0,6,wxS("Text in layer 1"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l2);
  pdf.Cell(0,6,wxS("Text in layer 2"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l3);
  pdf.Cell(0,6,wxS("Text in layer 3"));
  pdf.Ln(15);
  pdf.LeaveLayer();
  pdf.EnterLayer(l4);
  pdf.Cell(30,6,wxS("Text in layer 4"));
  pdf.LeaveLayer();
  pdf.SetTextColour(wxPdfColour(wxString(wxS("blue"))));
  pdf.SetFont(wxS("Courier"),wxS(""),12);
  pdf.Cell(0, 6, wxS("<< Zoom here (200% or more)!"));
  pdf.Ln(15);

  pdf.SaveAsFile(wxS("layersradiogroup.pdf"));
}

int
layers(bool testMode)
{
  OrderedLayers(testMode);
  GroupedLayers(testMode);
  NestedLayers(testMode);
  AutomaticLayers(testMode);
  LayerRadioGroup(testMode);
  return 0;
}
