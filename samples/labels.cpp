///////////////////////////////////////////////////////////////////////////////
// Name:        labels.cpp
// Purpose:     Example of label printing
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-11-26
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

/**
* Labels
* 
* This example demonstrates the PDF label printing.
*
* The code is based on the PHP script PDF_Label of Laurent PASSEBECQ.
*
* Following are the comments from the original PHP code:
*
* ////////////////////////////////////////////////////
* // PDF_Label 
* //
* // Class to print labels in Avery or custom formats
* //
* // Copyright (C) 2003 Laurent PASSEBECQ (LPA)
* // Based on code by Steve Dillon : steved@mad.scientist.com
* //
* //-------------------------------------------------------------------
* // VERSIONS :
* // 1.0  : Initial release
* // 1.1  : + : Added unit in the constructor
* //        + : Now Positions start @ (1,1).. then the first image @top-left of a page is (1,1)
* //        + : Added in the description of a label : 
* //        font-size  : defaut char size (can be changed by calling Set_Char_Size(xx);
* //        paper-size  : Size of the paper for this sheet (thanx to Al Canton)
* //        metric    : type of unit used in this description
* //                You can define your label properties in inches by setting metric to 'in'
* //                and printing in millimiter by setting unit to 'mm' in constructor.
* //        Added some labels :
* //        5160, 5161, 5162, 5163,5164 : thanx to Al Canton : acanton@adams-blake.com
* //        8600             : thanx to Kunal Walia : kunal@u.washington.edu
* //        + : Added 3mm to the position of labels to avoid errors 
* // 1.2  : + : Added Set_Font_Name method
* //        = : Bug of positioning
* //        = : Set_Font_Size modified -> Now, just modify the size of the font
* //        = : Set_Char_Size renamed to Set_Font_Size
* ////////////////////////////////////////////////////
*/

typedef struct tPdfLabelFormat
{
  wxChar*     name;
  wxPaperSize paperSize;
  wxChar*     metric;
  double      marginLeft;
  double      marginTop;
  int         nx;
  int         ny;
  double      xSpace;
  double      ySpace;
  double      width;
  double      height;
  double      fontSize;
} PdfLabelFormat;

static PdfLabelFormat s_averyLabels[] =
{
  {  wxT("5160"),  wxPAPER_LETTER, wxT("mm"), 1.762, 10.7,    3, 10,  3.175,  0.,   66.675, 25.4,   8 },
  {  wxT("5161"),  wxPAPER_LETTER, wxT("mm"), 0.967, 10.7,    2, 10,  3.967,  0.,  101.6,   25.4,   8 },
  {  wxT("5162"),  wxPAPER_LETTER, wxT("mm"), 0.97,  20.224,  2,  7,  4.762,  0.,  100.807, 35.72,  8 },
  {  wxT("5163"),  wxPAPER_LETTER, wxT("mm"), 1.762, 10.7,   2,  5,  3.175,  0.,  101.6,   50.8,   8 },
  {  wxT("5164"),  wxPAPER_LETTER, wxT("in"), 0.148,  0.5,    2,  3,  0.2031, 0.,    4.0,    3.33, 12 },
  {  wxT("8600"),  wxPAPER_LETTER, wxT("mm"), 7.1,   19,     3, 10,  9.5,    3.1,  66.6,   25.4,   8 },
  {  wxT("L7163"), wxPAPER_A4,     wxT("mm"), 5.0,   15,     2,  7, 25.0,    0.,   99.1,   38.1,   9 },
  // Last sentinel entry (do not delete)
  { wxT(""),      wxPAPER_NONE,   wxT("mm"), 0.0,    0,     0,  0,  0.0,    0.,    0.0,    0.0,   0 },
};

class PdfLabel : public wxPdfDocument
{
public:
  /// Constructor
  PdfLabel (const PdfLabelFormat& format, const wxString& unit = _T("mm"), int posX = 1, int  posY = 1)
    : wxPdfDocument(wxPORTRAIT, format.metric, format.paperSize)
  {
    m_charSize   = 10;
    m_lineHeight = 10;
    m_xCount     = 1;
    m_yCount     = 1;
    
    SetFormat(format);
    SetFontName(_T("Arial"));
    SetMargins(0,0); 
    SetAutoPageBreak(false); 

    m_metricDoc = unit;
    // Start at the given label position
    m_xCount = (posX >= m_xNumber) ? m_xNumber-1 : ((posX > 1) ? posX-1 : 0);
    m_yCount = (posY >= m_yNumber) ? m_yNumber-1 : ((posY > 1) ? posY-1 : 0);
  }

  /// Print a label
  void AddLabel(const wxString& texte)
  {
    // We are in a new page, then we must add a page
    if ((m_xCount == 0) && (m_yCount == 0))
    {
      AddPage();
    }

    double posX = m_marginLeft+(m_xCount*(m_width+m_xSpace));
    double posY = m_marginTop+(m_yCount*(m_height+m_ySpace));
    SetXY(posX+3, posY+3);
    MultiCell(m_width, m_lineHeight, texte);

    m_yCount++;

    if (m_yCount == m_yNumber)
    {
      // End of column reached, we start a new one
      m_xCount++;
      m_yCount = 0;
    }

    if (m_xCount == m_xNumber)
    {
      // Page full, we start a new one
      m_xCount = 0;
      m_yCount = 0;
    }
  }

  /// Changes the font
  void SetFontName(const wxString& fontname)
  {
    if (fontname != _T(""))
    {
      m_fontName = fontname;
      SetFont(m_fontName);
    }
  }

  /// Sets the font size
  /**
  * This changes the line height too
  */
  void SetFontSize(double pt)
  {
    if (pt > 3)
    {
      m_charSize = pt;
      m_lineHeight = GetHeightChars(pt);
      wxPdfDocument::SetFontSize(m_charSize);
    }
  }

  /// Finds a predefined label format by name
  static const PdfLabelFormat& FindFormat(const wxString& format)
  {
    PdfLabelFormat* tFormat = &s_averyLabels[0];
    int j = 0;
    for (j = 0; wxString(_T("")).Cmp(s_averyLabels[j].name) != 0; j++)
    {
      if (format == s_averyLabels[j].name)
      {
        tFormat = &s_averyLabels[j];
        break;
      }
    }
    return *tFormat;
  }

protected:
  /// Convert units (in to mm, mm to in)
  /**
  * src and dest must be 'in' or 'mm'
  */
  double ConvertMetric (double value, const wxString& src, const wxString& dest)
  {
    double retValue = value;
    if (src == _T("in") && dest == _T("mm"))
    {
      retValue *= (1000./ 39.37008);
    }
    else if (src == _T("mm") && dest == _T("in"))
    {
      retValue *= (39.37008 / 1000.);
    }
    return retValue;
  }

  /// Gives the height for a char size given.
  double GetHeightChars(double pt)
  {
    double height = 100;
    // Array matching character sizes and line heights
    static const int nTable = 10;
    static double cTable[nTable] = { 6, 7,   8, 9, 10, 11, 12, 13, 14, 15 };
    static double hTable[nTable] = { 2, 2.5, 3, 4,  5,  6,  7,  8,  9, 10 };
    int i;
    for (i = 0; i < nTable; i++)
    {
      if (pt == cTable[i])
      {
        height = hTable[i];
        break;
      }
    }
    return height;
  }

  /// Sets the label format
  void SetFormat(const PdfLabelFormat& format)
  {
    m_metric     = format.metric;
    m_averyName  = format.name;
    m_marginLeft = ConvertMetric(format.marginLeft, m_metric, m_metricDoc);
    m_marginTop  = ConvertMetric(format.marginTop,  m_metric, m_metricDoc);
    m_xSpace     = ConvertMetric(format.xSpace,     m_metric, m_metricDoc);
    m_ySpace     = ConvertMetric(format.ySpace,     m_metric, m_metricDoc);
    m_xNumber    = format.nx;
    m_yNumber    = format.ny;
    m_width      = ConvertMetric(format.width,      m_metric, m_metricDoc);
    m_height     = ConvertMetric(format.height,     m_metric, m_metricDoc);
    SetFontSize(format.fontSize);
  }

private:
  wxString m_averyName;    //< Name of format
  double   m_marginLeft;  //< Left margin of labels
  double   m_marginTop;    //< Top margin of labels
  double   m_xSpace;      //< Horizontal space between 2 labels
  double   m_ySpace;      //< Vertical space between 2 labels
  int      m_xNumber;      //< Number of labels horizontally
  int      m_yNumber;      //< Number of labels vertically
  double   m_width;        //< Width of label
  double   m_height;      //< Height of label
  double   m_charSize;    //< Character size
  double   m_lineHeight;  //< Default line height
  wxString m_metric;      //< Type of metric for labels.. Will help to calculate good values
  wxString m_metricDoc;    //< Type of metric for the document
  wxString m_fontName;    //< Name of the font

  int      m_xCount;
  int      m_yCount;
};

void
labels()
{
  // To create the object, 2 possibilities:
  // either pass a custom format via an array
  // or use a built-in AVERY name
  //
  // Example of custom format; we start at the second column
  // PdfLabelFormat perso1 = { wxT("perso1"), wxPAPER_A4, wxT("mm"), 1.,         1.,        2,  7,  0.,     0.,     99.1,  38.1,   14 },
  // PdfLabel pdf(perso1, _T("mm"), 1, 2);
  //
  // Standard format
  PdfLabel pdf(PdfLabel::FindFormat(_T("L7163")), _T("mm"), 1, 2);
  pdf.AddPage();

  // Print labels
  int i;
  for (i = 1; i <= 40; i++)
  {
    pdf.AddLabel(wxString::Format(_T("Laurent %d\nImmeuble Titi\nav. fragonard\n06000, NICE, FRANCE"), i));
  }
  pdf.SaveAsFile(_T("labels.pdf"));
}

