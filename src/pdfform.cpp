///////////////////////////////////////////////////////////////////////////////
// Name:        pdfform.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-01-18
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfform.cpp Implementation of the wxPdfDocument form fields

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes

#include "wx/pdfdoc.h"
#include "wx/pdfform.h"

wxPdfIndirectObject::wxPdfIndirectObject(int objectId, int generationId)
{
  m_type = wxPDF_OBJECT_INDIRECT;
  m_objectId = objectId;
  m_generationId = generationId;
}

wxPdfIndirectObject::~wxPdfIndirectObject()
{
}

wxPdfAnnotationObject::wxPdfAnnotationObject(int objectId, int generationId)
  : wxPdfIndirectObject(objectId, generationId)
{
  SetType(wxPDF_OBJECT_ANNOTATION);
}

wxPdfAnnotationObject::~wxPdfAnnotationObject()
{
}

void
wxPdfAnnotationObject::SetRectangle(double x, double y, double width, double height)
{
  m_x = x;
  m_y = y;
  m_w = width;
  m_h = height;
}

wxPdfAnnotationWidget::wxPdfAnnotationWidget(int objectId, int generationId)
  : wxPdfAnnotationObject(objectId, generationId)
{
  SetType(wxPDF_OBJECT_WIDGET);
}

wxPdfAnnotationWidget::~wxPdfAnnotationWidget()
{
}

wxPdfCheckBox::wxPdfCheckBox(int objectId, int generationId)
  : wxPdfAnnotationWidget(objectId, generationId)
{
  SetType(wxPDF_OBJECT_WIDGET_CHECKBOX);
}

wxPdfCheckBox::~wxPdfCheckBox()
{
}

wxPdfComboBox::wxPdfComboBox(int objectId,
                             int fontindex, double fontsize, 
                             int generationId)
  : wxPdfAnnotationWidget(objectId, generationId)
{
  SetType(wxPDF_OBJECT_WIDGET_COMBOBOX);
  m_fontindex = fontindex;
  m_fontsize  = fontsize;
}

wxPdfComboBox::~wxPdfComboBox()
{
}

wxPdfPushButton::wxPdfPushButton(int objectId,
                                 int fontindex, double fontsize, 
                                 int generationId)
  : wxPdfAnnotationWidget(objectId, generationId)
{
  SetType(wxPDF_OBJECT_WIDGET_PUSHBUTTON);
  m_fontindex = fontindex;
  m_fontsize  = fontsize;
}

wxPdfPushButton::~wxPdfPushButton()
{
}

wxPdfRadioButton::wxPdfRadioButton(int objectId, int index, int generationId)
  : wxPdfAnnotationWidget(objectId, generationId)
{
  SetType(wxPDF_OBJECT_WIDGET_RADIOBUTTON);
  m_index = index;
}

wxPdfRadioButton::~wxPdfRadioButton()
{
}

wxPdfRadioGroup::wxPdfRadioGroup(int objectId, const wxString& groupName, int generationId)
  :  wxPdfIndirectObject(objectId, generationId)
{
  SetType(wxPDF_OBJECT_RADIOGROUP);
  m_groupName = groupName;
}

wxPdfRadioGroup::~wxPdfRadioGroup()
{
}

void
wxPdfRadioGroup::Add(wxPdfRadioButton* radio)
{
  m_radios.Add(radio);
  radio->SetParent(this);
}

wxPdfTextField::wxPdfTextField(int objectId,
                               int fontindex, double fontsize, 
                               const wxString& value, int generationId)
  : wxPdfAnnotationWidget(objectId, generationId)
{
  SetType(wxPDF_OBJECT_WIDGET_TEXTFIELD);
  m_fontindex = fontindex;
  m_fontsize  = fontsize;
  m_value = value;
}

wxPdfTextField::~wxPdfTextField()
{
}

// wxPdfDocument methods for forms

void
wxPdfDocument::OutIndirectObject(wxPdfIndirectObject* object)
{
  int objectId = object->GetObjectId();
  int generationId = object->GetGenerationId();
  if (m_offsets->count(objectId-1) == 0)
  {
    (*m_offsets)[objectId-1] = m_buffer.TellO();
    OutAscii(wxString::Format(_T("%d %d obj"), objectId, generationId));
    switch (object->GetType())
    {
      case wxPDF_OBJECT_RADIOGROUP:
        {
          wxPdfRadioGroup* obj = static_cast<wxPdfRadioGroup*>(object);
/// TODO: support for radio button groups
// Currently radio button groups do not function
          Out("<<");
          Out("/FT /Btn");
          OutAscii(wxString::Format(_T("/Ff %d"), (1 << 14) | (1 << 15)));
//          Out("/F 4");
          Out("/T ", false);
          OutAsciiTextstring(obj->GetName());
#if 0
          OutAscii(wxString(_T("/BS << /W ") + 
                   Double2String(obj->GetBorderWidth() +
                   wxString(_T("/S/")) + obj->GetBorderStyle() + 
                   wxString(_T(">>")));
#endif
//          OutAscii(wxString(_T("/MK <</BC [")) + obj->GetBorder() +
//                   wxString(_T("]/BG [")) + obj->GetBackground() + wxString(_T("] /CA ")), false);
//          OutAsciiTextstring(wxString(_T("4")), false);
//          Out(">>");
          Out("/DR 2 0 R");
          Out("/DA ", false);
//          OutAsciiTextstring(wxString::Format(_T("/F%d 9 Tf "), m_zapfdingbats) + obj->GetTextColor());
//          Out("/AP <</N <</Yes <<>>>> >>");
//          Out("/AS /Off");
          Out("/V /V1");
          Out("/Kids [", false);
          size_t j;
          wxPdfRadioButton* radio;
          wxArrayPtrVoid kids = obj->GetKids();
          for (j = 0; j < kids.GetCount(); j++)
          {
            radio = static_cast<wxPdfRadioButton*>(kids[j]);
            OutAscii(wxString::Format(_T("%d %d R "), radio->GetObjectId(), radio->GetGenerationId()), false);
          }
          Out("]");
          Out(">>");
          Out("endobj");
          for (j = 0; j < kids.GetCount(); j++)
          {
            wxPdfRadioButton* radio = static_cast<wxPdfRadioButton*>(kids[j]);
            OutIndirectObject(radio);
          }
        }
        break;

      case wxPDF_OBJECT_WIDGET_CHECKBOX:
        {
          wxPdfCheckBox* obj = static_cast<wxPdfCheckBox*>(object);
          Out("<<");
          Out("/Type /Annot");
          Out("/Subtype /Widget");
          OutAscii(wxString(_T("/Rect [")) +
                   Double2String(obj->GetX()*m_k,2) + wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((obj->GetX() + obj->GetWidth())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY() - obj->GetHeight())*m_k,2) + wxString(_T("]")));
          Out("/FT /Btn");
          Out("/Ff 0");
          Out("/F 4");
          Out("/T ", false);
          OutAsciiTextstring(obj->GetName());
          // Border style
          OutAscii(wxString(_T("/BS << /W ")) + 
                   Double2String(obj->GetBorderWidth(), 2) +
                   wxString(_T("/S/")) + obj->GetBorderStyle() + 
                   wxString(_T(">>")));
          OutAscii(wxString(_T("/MK <</BC [")) + obj->GetBorderColor() +
                   wxString(_T("]/BG [")) + obj->GetBackgroundColor() + wxString(_T("] /CA ")), false);
          OutAsciiTextstring(wxString(_T("4")), false);
          Out(">>");
          Out("/DR 2 0 R");
          Out("/DA ", false);
          OutAsciiTextstring(wxString::Format(_T("/F%d 9 Tf "), m_zapfdingbats) + obj->GetTextColor());
          Out("/AP <</N <</Yes <<>>>> >>");
          Out("/AS /Off");
          if (obj->GetValue())
          {
            Out("/DV /Yes");
            Out("/V /Yes");
          }
          else
          {
            Out("/DV /Off");
            Out("/V /Off");
          }
          Out(">>");
          Out("endobj");
        }
        break;

      case wxPDF_OBJECT_WIDGET_COMBOBOX:
        {
          wxPdfComboBox* obj = static_cast<wxPdfComboBox*>(object);
          Out("<<");
          Out("/Type /Annot");
          Out("/Subtype /Widget");
          OutAscii(wxString(_T("/Rect [")) +
                   Double2String(obj->GetX()*m_k,2) + wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((obj->GetX() + obj->GetWidth())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY() - obj->GetHeight())*m_k,2) + wxString(_T("]")));
          Out("/FT /Ch");
          Out("/Ff 67764224");
          Out("/F 4");
          Out("/T ", false);
          OutAsciiTextstring(obj->GetName());
          OutAscii(wxString(_T("/MK <</BC [")) + obj->GetBorderColor() +
                   wxString(_T("]/BG [")) + obj->GetBackgroundColor() + wxString(_T("]>>")));
          // Border style
          OutAscii(wxString(_T("/BS << /W ")) + 
                   Double2String(obj->GetBorderWidth(), 2) +
                   wxString(_T("/S/")) + obj->GetBorderStyle() + 
                   wxString(_T(">>")));
          Out("/DR 2 0 R");
          Out("/DA ", false);
          OutAsciiTextstring(wxString::Format(_T("/F%d "), obj->GetFontIndex()) +
                             Double2String(obj->GetFontSize(),2) +
                             wxString(_T(" Tf ")) + obj->GetTextColor()); 
          wxArrayString options = obj->GetValue();
          Out("/DV ", false);
          OutTextstring(options[0]);
          Out("/V ", false);
          OutTextstring(options[0]);
// Option list alternative (transfer value, display value)
//   /Op t [ [(1)(Socks)][(2)(Shoes)] [(3)(Pants)][(4)(Shirt)][(5)(Tie)][(6)(Belt)][(7)(Shorts)]]
//
          Out("/Opt [", false);
          size_t j;
          for (j = 0; j < options.GetCount(); j++)
          {
            OutTextstring(options[j], false);
          }
          Out("]");
          Out(">>");
          Out("endobj");
        }
        break;

      case wxPDF_OBJECT_WIDGET_PUSHBUTTON:
        {
          wxPdfPushButton* obj = static_cast<wxPdfPushButton*>(object);
          Out("<<");
          Out("/Type /Annot");
          Out("/Subtype /Widget");
          OutAscii(wxString(_T("/Rect [")) +
                   Double2String(obj->GetX()*m_k,2) + wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((obj->GetX() + obj->GetWidth())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY() - obj->GetHeight())*m_k,2) + wxString(_T("]")));
          Out("/FT /Btn");
          Out("/Ff 65536"); // (1 << 16),
          Out("/F 4");
// Highlight button
//        Out("/H");
          Out("/T ", false);
          OutAsciiTextstring(obj->GetName());
          Out("/BS << /W 1 /S /B >>");
          OutAscii(wxString(_T("/MK <</BC [")) + obj->GetBorderColor() +
                   wxString(_T("]/BG [")) + obj->GetBackgroundColor() + 
                   wxString(_T("] /CA ")), false);
          OutTextstring(obj->GetCaption(), false);
          Out(">>");
          Out("/DR 2 0 R");
          Out("/DA ", false);
          OutAsciiTextstring(wxString::Format(_T("/F%d "), obj->GetFontIndex()) +
                             Double2String(obj->GetFontSize(),2) + 
                             wxString(_T(" Tf ")) + obj->GetTextColor()); 
          Out("/A <</S /JavaScript /JS", false);
          OutTextstring(obj->GetAction());
          Out(">>");
          Out(">>");
          Out("endobj");
        }
        break;

      case wxPDF_OBJECT_WIDGET_RADIOBUTTON:
        {
          wxPdfRadioButton* obj = static_cast<wxPdfRadioButton*>(object);
          Out("<<");
          Out("/Type /Annot");
          Out("/Subtype /Widget");
          OutAscii(wxString(_T("/Rect [")) +
                   Double2String(obj->GetX()*m_k,2) + wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((obj->GetX() + obj->GetWidth())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY() - obj->GetHeight())*m_k,2) + wxString(_T("]")));
          Out("/FT /Btn");
          Out("/Ff 49152");
          Out("/F 4");
          wxPdfRadioGroup* parent = obj->GetParent();
#if 0
// TODO: integrate radio button groups into PDF document
          OutAscii(wxString::Format(_T("/Parent [%d %d R]"), parent->GetObjectId(), parent->GetGenerationId()));
//          Out("/P ??? 0 R /H /T ");
#endif
          Out("/T ", false);
          OutAsciiTextstring(parent->GetName());
          Out("/AS /Off");
          OutAscii(wxString(_T("/MK <</BC [")) + obj->GetBorderColor() +
                   wxString(_T("]/BG [")) + obj->GetBackgroundColor() + 
                   wxString(_T("] /CA ")), false);
          OutAsciiTextstring(wxString(_T("l")), false);
          Out(">>");
          // Border style
          OutAscii(wxString(_T("/BS << /W ")) + 
                   Double2String(obj->GetBorderWidth(), 2) +
                   wxString(_T("/S/")) + obj->GetBorderStyle() + 
                   wxString(_T(">>")));
          Out("/DR 2 0 R");
          Out("/DA ", false);
          OutAsciiTextstring(wxString::Format(_T("/F%d 9 Tf "), m_zapfdingbats) + obj->GetTextColor());
          OutAscii(wxString::Format(_T("/AP <</N <</V%d <<>> >> >>"), obj->GetIndex()));
          // Set first button in group as selected
          if (obj->GetIndex() == 1)
          {
            OutAscii(wxString::Format(_T("/V /V%d"), obj->GetIndex()));
          }
          Out(">>");
          Out("endobj");
        }
        break;

      case wxPDF_OBJECT_WIDGET_TEXTFIELD:
        {
          wxPdfTextField* obj = static_cast<wxPdfTextField*>(object);
          Out("<<");
          Out("/Type /Annot");
          Out("/Subtype /Widget");
          OutAscii(wxString(_T("/Rect [")) +
                   Double2String(obj->GetX()*m_k,2) + wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((obj->GetX() + obj->GetWidth())*m_k,2) +  wxString(_T(" ")) +
                   Double2String((m_h - obj->GetY() - obj->GetHeight())*m_k,2) + wxString(_T("]")));
          Out("/FT /Tx");
          if (obj->GetMultiLine())
          {
            Out("/Ff 4096");
          }
          Out("/F 4");
          Out("/T ", false);
          OutAsciiTextstring(obj->GetName());
          Out("/V ", false);
          OutTextstring(obj->GetValue()); // Current value
          Out("/DV ", false);
          OutTextstring(obj->GetValue()); // Default value
// Maximal length of text input
//          OutAscii(wxString::Format(_T("/MaxLen %d"), obj->GetMaxLength());
          OutAscii(wxString(_T("/MK <</BC [")) + obj->GetBorderColor() +
                   wxString(_T("]/BG [")) + obj->GetBackgroundColor() + wxString(_T("]>>")));
          // Border style
          OutAscii(wxString(_T("/BS << /W ")) + 
                   Double2String(obj->GetBorderWidth(), 2) +
                   wxString(_T("/S/")) + obj->GetBorderStyle() + 
                   wxString(_T(">>")));
          Out("/DR 2 0 R");
          Out("/DA ", false);
          OutAsciiTextstring(wxString::Format(_T("/F%d "), obj->GetFontIndex()) +
                             Double2String(obj->GetFontSize(),2) + 
                             wxString(_T(" Tf ")) + obj->GetTextColor()); 
          Out(">>");
          Out("endobj");
        }
        break;

      case wxPDF_OBJECT_WIDGET:
      case wxPDF_OBJECT_ANNOTATION:
      case wxPDF_OBJECT_INDIRECT:
      default:
        Out("endobj");
        break;
    }
  }
}

void
wxPdfDocument::CheckBox(const wxString& name, double width, bool checked)
{
  CheckBox(name, m_x, m_y, width, checked);
}

void
wxPdfDocument::CheckBox(const wxString& name, double x, double y, double width, bool checked)
{
  wxPdfCheckBox* field = new wxPdfCheckBox(GetNewObjId());
  field->SetName(name);
  field->SetValue(checked);
  field->SetRectangle(x, y, width, width);
  AddFormField(field);
}

void
wxPdfDocument::ComboBox(const wxString& name, double width, double height, const wxArrayString& values)
{
  ComboBox(name, m_x, m_y, width, height, values);
}

void
wxPdfDocument::ComboBox(const wxString& name, 
                        double x, double y, double width, double height, 
                        const wxArrayString& values)
{
  wxPdfComboBox* field = new wxPdfComboBox(GetNewObjId(), m_currentFont->GetIndex(), m_fontSizePt);
  field->SetName(name);
  field->SetValue(values);
  field->SetRectangle(x, y, width, height);
  AddFormField(field);
}

void
wxPdfDocument::PushButton(const wxString& name, double width, double height, 
                          const wxString& caption, const wxString& action)
{
  PushButton(name, m_x, m_y, width, height, caption, action);
}

void
wxPdfDocument::PushButton(const wxString& name, 
                          double x, double y, double w, double h, 
                          const wxString& caption, const wxString& action)
{
  wxPdfPushButton* field = new wxPdfPushButton(GetNewObjId(), m_currentFont->GetIndex(), m_fontSizePt);
  field->SetName(name);
  field->SetCaption(caption);
  field->SetAction(action);
  field->SetRectangle(x, y, w, h);
  AddFormField(field);
}

void
wxPdfDocument::RadioButton(const wxString& group, const wxString& name, double width)
{
  RadioButton(group, name, m_x, m_y, width);
}

void
wxPdfDocument::RadioButton(const wxString& group, const wxString& name, 
                           double x, double y, double width)
{
  wxPdfRadioGroup* currentGroup;
  wxPdfRadioGroupMap::iterator radioGroup = (*m_radioGroups).find(group);
  if (radioGroup != (*m_radioGroups).end())
  {
    currentGroup = static_cast<wxPdfRadioGroup*>(radioGroup->second);
  }
  else
  {
//    currentGroup = new wxPdfRadioGroup(GetNewObjId(), group);
    currentGroup = new wxPdfRadioGroup(0, group);
    (*m_radioGroups)[group] = currentGroup;
#if 0
// TODO: integrate radio button groups into PDF document
    int n = (*m_formFields).size()+1;
    (*m_formFields)[n] = currentGroup;
#endif
  }

  wxPdfRadioButton* field = new wxPdfRadioButton(GetNewObjId(), currentGroup->GetCount()+1);
  field->SetName(name);
  field->SetRectangle(x, y, width, width);
// TODO: integrate radio button groups into PDF document
  AddFormField(field /*, false*/);
  currentGroup->Add(field);
}

void
wxPdfDocument::TextField(const wxString& name, double width, double height,
                         const wxString& value, bool multiline)
{
  TextField(name, m_x, m_y, width, height, value, multiline);
}

void
wxPdfDocument::TextField(const wxString& name, 
                         double x, double y, double width, double height,
                         const wxString& value, bool multiline)
{
  wxPdfTextField* field = new wxPdfTextField(GetNewObjId(), m_currentFont->GetIndex(), m_fontSizePt, value);
  field->SetName(name);
  field->SetValue(value);
  field->SetMultiLine(multiline);
  field->SetRectangle(x, y, width, height);
  AddFormField(field);
}

void
wxPdfDocument::AddFormField(wxPdfAnnotationWidget* field, bool setFormField)
{
  field->SetBorderColor(m_formBorderColor);
  field->SetBackgroundColor(m_formBackgroundColor);
  field->SetTextColor(m_formTextColor);
  field->SetBorderStyle(m_formBorderStyle);
  field->SetBorderWidth(m_formBorderWidth);
  if (setFormField)
  {
    int n = (*m_formFields).size()+1;
    (*m_formFields)[n] = field;
  }

  wxArrayPtrVoid* annotationArray = NULL;
  wxPdfFormAnnotsMap::iterator formAnnots = (*m_formAnnotations).find(m_page);
  if (formAnnots != (*m_formAnnotations).end())
  {
    annotationArray = formAnnots->second;
  }
  else
  {
    annotationArray = new wxArrayPtrVoid;
    (*m_formAnnotations)[m_page] = annotationArray;
  }
  annotationArray->Add(field);
}

void
wxPdfDocument::SetFormColors(const wxPdfColour& borderColor,
                             const wxPdfColour& backgroundColor, 
                             const wxPdfColour& textColor)
{
  m_formBorderColor     = borderColor.GetColor(false).BeforeLast(wxT(' '));
  m_formBackgroundColor = backgroundColor.GetColor(false).BeforeLast(wxT(' '));
  m_formTextColor       = textColor.GetColor(false);
}

void
wxPdfDocument::SetFormBorderStyle(wxPdfBorderStyle borderStyle, double borderWidth)
{
  switch (borderStyle)
  {
    case wxPDF_BORDER_DASHED:    m_formBorderStyle = wxString(_T("D")); break;
    case wxPDF_BORDER_BEVELED:   m_formBorderStyle = wxString(_T("B")); break;
    case wxPDF_BORDER_INSET:     m_formBorderStyle = wxString(_T("I")); break;
    case wxPDF_BORDER_UNDERLINE: m_formBorderStyle = wxString(_T("U")); break;
    case wxPDF_BORDER_SOLID:
    default:                     m_formBorderStyle = wxString(_T("S")); break;
  }
  m_formBorderWidth = (borderWidth >= 0) ? borderWidth * m_k : 1;
}
