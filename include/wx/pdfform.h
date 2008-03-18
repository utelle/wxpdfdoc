///////////////////////////////////////////////////////////////////////////////
// Name:        pdfform.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-01-18
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfform.h Interface of the wxPdfDocument form field handling

#ifndef _PDFFORM_H_
#define _PDFFORM_H_

// wxWidgets headers
#include "wx/pdfdocdef.h"

/// Object types
enum wxPdfObjectType
{
  wxPDF_OBJECT_INDIRECT,
  wxPDF_OBJECT_RADIOGROUP,

  wxPDF_OBJECT_ANNOTATION,
  wxPDF_OBJECT_TEXTANNOTATION,

  wxPDF_OBJECT_WIDGET,
  wxPDF_OBJECT_WIDGET_CHECKBOX,
  wxPDF_OBJECT_WIDGET_COMBOBOX,
  wxPDF_OBJECT_WIDGET_PUSHBUTTON,
  wxPDF_OBJECT_WIDGET_RADIOBUTTON,
  wxPDF_OBJECT_WIDGET_TEXTFIELD
};

/// Base class for indirect PDF objects (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfIndirectObject
{
public:
  /// Constructor
  wxPdfIndirectObject(int objectId, int generationId);

  /// Destructor
  virtual ~wxPdfIndirectObject();

  /// Get type of this object
  wxPdfObjectType GetType() { return m_type; }

  /// Set type of object
  void SetType(wxPdfObjectType type) { m_type = type; }

  /// Get identifier of this object
  int GetObjectId() { return m_objectId; }

  /// Get generation identifier of this object
  int GetGenerationId() { return m_generationId; }

private:
  wxPdfObjectType m_type;          ///< type of object
  int             m_objectId;      ///< object identifier
  int             m_generationId;  ///< generation identifier
};

/// Base class for annotation objects (For internal use only)
class wxPdfAnnotationObject : public wxPdfIndirectObject
{
public:
  wxPdfAnnotationObject(int objectId, int generationId);
  
  ~wxPdfAnnotationObject();

  /// Set the associated rectangular area
  void SetRectangle(double x, double y, double width, double height);

  /// Get the X offset
  double GetX() { return m_x; }

  /// Get the Y offset
  double GetY() { return m_y; }

  /// Get the width
  double GetWidth() { return m_w; }

  /// Get the height
  double GetHeight() { return m_h; }

private:
  double m_x;   ///< X offset of associated area
  double m_y;   ///< Y offset of associated area
  double m_w;   ///< Width of associated area
  double m_h;   ///< Height of associated area
};

/// Base class for PDF form fields (For internal use only)
class wxPdfAnnotationWidget : public wxPdfAnnotationObject
{
public:
  /// Constructor
  wxPdfAnnotationWidget(int objectId, int generationId = 0);
  
  /// Destructor
  virtual ~wxPdfAnnotationWidget();

  /// Set form field name
  void SetName(const wxString& name) { m_name = name; }
  
  /// Get form field name
  wxString GetName() { return m_name; }

  /// Set border color representation of form field
  void SetBorderColor(const wxString& borderColor) { m_borderColor = borderColor; }
  
  /// Get border color representation of form field
  wxString GetBorderColor() { return m_borderColor; }

  /// Set background color representation of form field
  void SetBackgroundColor(const wxString& backgroundColor) { m_backgroundColor = backgroundColor; }
  
  /// Get background color representation of form field
  wxString GetBackgroundColor() { return m_backgroundColor; }

  /// Set text color representation of form field
  void SetTextColor(const wxString& textColor) { m_textColor = textColor; }
  
  /// Get text color representation of form field
  wxString GetTextColor() { return m_textColor; }

  /// Set border style of form field
  void SetBorderStyle(const wxString& borderStyle) { m_borderStyle = borderStyle; }
  
  /// Get border style of form field
  wxString GetBorderStyle() { return m_borderStyle; }

  /// Set border width of form field
  void SetBorderWidth(double borderWidth) { m_borderWidth = borderWidth; }

  /// Get border width of form field
  double GetBorderWidth() { return m_borderWidth; }

private:
  wxString m_name;             ///< name of form field
  wxString m_borderColor;      ///< border color representation
  wxString m_backgroundColor;  ///< background color representation
  wxString m_textColor;        ///< text color representation
  double   m_borderWidth;      ///< border width in points
  wxString m_borderStyle;      ///< border style
};

/// Class representing check box form fields (For internal use only)
class wxPdfCheckBox : public wxPdfAnnotationWidget
{
public:
  /// Constructor
  wxPdfCheckBox(int objectId, int generationId = 0);

  /// Destructor
  virtual ~wxPdfCheckBox();

  /// Set status of form field (checked/unchecked)
  void SetValue(bool checked) { m_checked = checked; }
  
  /// Get status of form field (checked/unchecked)
  bool GetValue() { return m_checked; }

private:
  bool m_checked;  ///< status of check box
};

/// Class representing combo box form field (For internal use only)
class wxPdfComboBox : public wxPdfAnnotationWidget
{
public:
  /// Constructor
  wxPdfComboBox(int objectId, 
                int fontindex, double fontsize, 
                int generationId = 0);

  /// Destructor
  virtual ~wxPdfComboBox();

  /// Set value list of combo box
  void SetValue(const wxArrayString& values) { m_values = values; }
  
  /// Get value list of combo box
  wxArrayString GetValue() { return m_values; }

  /// Get object index of font used for displaying the value list
  int GetFontIndex() { return m_fontindex; }
  
  /// Get size of font used for displaying the value list
  double GetFontSize() { return m_fontsize; }

private:
  int           m_fontindex;  ///< index of combo box font
  double        m_fontsize;   ///< size of combo box font
  wxArrayString m_values;     ///< array of combo box values
};

/// Class representing push button form field (For internal use only)
class wxPdfPushButton : public wxPdfAnnotationWidget
{
public:
  /// Constructor
  wxPdfPushButton(int objectId,
                  int fontindex, double fontsize, 
                  int generationId = 0);

  /// Destructor
  virtual ~wxPdfPushButton();

  /// Set caption string of button
  void SetCaption(const wxString& caption) { m_caption = caption; }
  
  /// Get caption string of button
  wxString GetCaption() { return m_caption; }
  
  /// Set JavaScript action  associated with the button
  void SetAction(const wxString& action) { m_action = action; }
  
  /// Get JavaScript action  associated with the button
  wxString GetAction() { return m_action; }

  /// Get object index of font used for displaying the value list
  int GetFontIndex() { return m_fontindex; }
  
  /// Get size of font used for displaying the value list
  double GetFontSize() { return m_fontsize; }

private:
  int      m_fontindex;  ///< index of combo box font
  double   m_fontsize;   ///< size of combo box font
  wxString m_caption;    ///< caption string
  wxString m_action;     ///< JavaScript action
};

// Forward declaration of radio button group
class wxPdfRadioGroup;

/// Class representing radio button form field (For internal use only)
class wxPdfRadioButton : public wxPdfAnnotationWidget
{
public:
  /// Constructor
  wxPdfRadioButton(int objectId, int index, int generationId = 0);
  
  /// Destructor
  virtual ~wxPdfRadioButton();

  /// Set parent (radio button group) of this button
  void SetParent(wxPdfRadioGroup* parent) { m_parent = parent; }
  
  /// Get parent (radio button group) of this button
  wxPdfRadioGroup* GetParent() { return m_parent; }
  
  /// Get index of this button within the radio button group
  int GetIndex() { return m_index; }

private:
  wxPdfRadioGroup* m_parent;  ///< pointer to parent group
  int              m_index;   ///< index within button group
};

/// Class representing radio button groups (For internal use only)
class wxPdfRadioGroup : public wxPdfIndirectObject
{
public:
  /// Constructor
  wxPdfRadioGroup(int objectId, const wxString& groupName, int generationId = 0);
  
  /// Destructor
  virtual ~wxPdfRadioGroup();

  /// Add a button to the group
  void Add(wxPdfRadioButton* radio);

  /// Get name of the button group
  wxString GetName() { return m_groupName; }

  /// Get the number of buttons in the group
  int GetCount() { return m_radios.GetCount(); }
  
  /// Get the array of buttons in the group
  wxArrayPtrVoid GetKids() { return m_radios; }

private:
  wxString       m_groupName;  ///< name of the radio button group
  wxArrayPtrVoid m_radios;     //< array of all radio buttons in this group
};

/// Class representing text form fields (For internal use only)
class wxPdfTextField : public wxPdfAnnotationWidget
{
public:
  /// Constructor
  wxPdfTextField(int objectId,
                 int fontindex, double fontsize, 
                 const wxString& value, int generation_id = 0);

  /// Destructor
  virtual ~wxPdfTextField();

  /// Set default value of the text field
  void SetValue(const wxString& value) { m_value = value; }

  /// Get default value of the text field
  wxString GetValue() { return m_value; }

  /// Set multi line property of the text field
  void SetMultiLine(bool multiline) { m_multiline = multiline; }

  /// Get multi line property of the text field
  bool GetMultiLine() { return m_multiline; }

  /// Get object index of font used for displaying the value list
  int GetFontIndex() { return m_fontindex; }
  
  /// Get size of font used for displaying the value list
  double GetFontSize() { return m_fontsize; }

private:
  int      m_fontindex;  ///< index of text field font
  double   m_fontsize;   ///< size of text field font
  wxString m_value;      ///< default value of text field
  bool     m_multiline;  ///< multi line property of text field
};

#endif
