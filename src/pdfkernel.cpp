/*
** Name:        pdfkernel.cpp
** Purpose:     Implementation of wxPdfDocument (internal methods)
** Author:      Ulrich Telle
** Created:     2006-01-27
** Copyright:   (c) 2006-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file pdfkernel.cpp Implementation of the wxPdfDocument class (internal methods)

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/tokenzr.h>
#include <wx/wfstream.h>
#include <wx/zstream.h>

#include "wx/pdfannotation.h"
#include "wx/pdfbookmark.h"
#include "wx/pdfdocument.h"
#include "wx/pdffontmanager.h"
#include "wx/pdffontdetails.h"
#include "wx/pdfform.h"
#include "wx/pdfgradient.h"
#include "wx/pdfgraphics.h"
#include "wx/pdflayer.h"
#include "wx/pdfobjects.h"
#include "wx/pdfparser.h"
#include "wx/pdfpattern.h"
#include "wx/pdfspotcolour.h"
#include "wx/pdftemplate.h"
#include "wx/pdfutility.h"

#include "pdfcorefontdata.inc"

class wxPdfGraphicState
{
public:
  wxString          m_fontFamily;
  int               m_fontStyle;
  double            m_fontSizePt;
  wxPdfFontDetails* m_fontDetails;

  wxPdfColour       m_drawColour;
  wxPdfColour       m_fillColour;
  wxPdfColour       m_textColour;
  bool              m_colourFlag;

  double            m_lineWidth;
  wxPdfLineStyle    m_lineStyle;
  int               m_extGState;
};

void
wxPdfDocument::SaveGraphicState()
{
  wxPdfGraphicState* state = new wxPdfGraphicState();
  // Save font state
  state->m_fontFamily = m_fontFamily;
  state->m_fontStyle = m_fontStyle;
  state->m_fontSizePt = m_fontSizePt;
  state->m_fontDetails = m_currentFont;
  // Save colour state
  state->m_drawColour = m_drawColour;
  state->m_fillColour = m_fillColour;
  state->m_textColour = m_textColour;
  state->m_colourFlag = m_colourFlag;
  // Save graphics state
  state->m_lineWidth = m_lineWidth;
  state->m_lineStyle = m_lineStyle;
  state->m_extGState = m_currentExtGState;
  m_graphicStates.Add(state);
}

void
wxPdfDocument::RestoreGraphicState()
{
  wxPdfGraphicState* state = NULL;
  if (!m_graphicStates.IsEmpty())
  {
    state = (wxPdfGraphicState*) m_graphicStates.Last();
    m_graphicStates.RemoveAt(m_graphicStates.GetCount()-1);
  }
  if (state != NULL)
  {
    // Restore font state
    m_fontFamily = state->m_fontFamily;
    m_fontStyle = state->m_fontStyle;
    m_fontSizePt = state->m_fontSizePt;
    m_fontSize    = m_fontSizePt / m_k;
    m_currentFont = state->m_fontDetails;
    // Restore colour state
    m_drawColour = state->m_drawColour;
    m_fillColour = state->m_fillColour;
    m_textColour = state->m_textColour;
    m_colourFlag = state->m_colourFlag;
    // Restore graphics state
    m_lineWidth = state->m_lineWidth;
    m_lineStyle = state->m_lineStyle;
    m_currentExtGState = state->m_extGState;
    delete state;
  }
}

void
wxPdfDocument::ClearGraphicState()
{
  wxPdfGraphicState* state = NULL;
  size_t n = m_graphicStates.GetCount();
  size_t j;
  for (j = 0; j < n; ++j)
  {
    state = (wxPdfGraphicState*) m_graphicStates[j];
    delete state;
  }
  m_graphicStates.Clear();
}

// ----------------------------------------------------------------------------
// wxPdfDocument: class representing a PDF document
// ----------------------------------------------------------------------------


bool
wxPdfDocument::SelectFont(const wxString& family, const wxString& style, double size, bool setFont)
{
  int styles = wxPDF_FONTSTYLE_REGULAR;
  wxString ucStyle = style.Upper();
  if (ucStyle.Find(wxS('B')) >= 0)
  {
    styles |= wxPDF_FONTSTYLE_BOLD;
  }
  if (ucStyle.Find(wxS('I')) >= 0)
  {
    styles |= wxPDF_FONTSTYLE_ITALIC;
  }
  if (ucStyle.Find(wxS('U')) >= 0)
  {
    styles |= wxPDF_FONTSTYLE_UNDERLINE;
  }
  if (ucStyle.Find(wxS('O')) >= 0)
  {
    styles |= wxPDF_FONTSTYLE_OVERLINE;
  }
  if (ucStyle.Find(wxS('S')) >= 0)
  {
    styles |= wxPDF_FONTSTYLE_STRIKEOUT;
  }
  return SelectFont(family, styles, size, setFont);
}

bool
wxPdfDocument::SelectFont(const wxString& family, int style, double size, bool setFont)
{
  wxString fontFamily = (!family.IsEmpty()) ? family : (m_currentFont != NULL) ? m_currentFont->GetFontFamily() : wxString();
  bool selected = false;
  wxPdfFont regFont = wxPdfFontManager::GetFontManager()->GetFont(fontFamily, style);
  if (regFont.IsValid())
  {
    selected = SelectFont(regFont, style, size, setFont);
  }
  else
  {
    wxLogError(wxString(wxS("wxPdfDocument::SetFont: ")) +
               wxString::Format(_("No font registered for font family '%s' with style '%d'."), fontFamily.c_str(), style));
  }
  return selected;
}

bool
wxPdfDocument::SelectFont(const wxFont& font, bool setFont)
{
#if wxUSE_UNICODE
  int styles = wxPDF_FONTSTYLE_REGULAR;
  if (font.GetWeight() == wxFONTWEIGHT_BOLD)
  {
    styles |= wxPDF_FONTSTYLE_BOLD;
  }
  if (font.GetStyle() == wxFONTSTYLE_ITALIC)
  {
    styles |= wxPDF_FONTSTYLE_ITALIC;
  }
  if (font.GetUnderlined())
  {
    styles |= wxPDF_FONTSTYLE_UNDERLINE;
  }
  wxPdfFont regFont = wxPdfFontManager::GetFontManager()->GetFont(font.GetFaceName(), styles);
  bool ok = regFont.IsValid();
  if (!ok)
  {
    regFont = wxPdfFontManager::GetFontManager()->RegisterFont(font, font.GetFaceName());
    ok = regFont.IsValid();
  }
  if (ok)
  {
    double size = (double) font.GetPointSize();
    ok = SelectFont(regFont, styles, size, setFont);
  }
#else
  wxString family;
  switch (font.GetFamily())
  {
    case wxFONTFAMILY_TELETYPE:
    case wxFONTFAMILY_MODERN:
      family = wxS("Courier");
      break;
    case wxFONTFAMILY_ROMAN:
      family = wxS("Times");
      break;
#if 0
      // PDF does not define fonts for the following font families:
    case wxFONTFAMILY_SCRIPT:
      family = wxS("ZapfChancery");
      break;
    case wxFONTFAMILY_DECORATIVE:
      family = wxS("");
      break;
#endif
    case wxFONTFAMILY_SWISS:
    default:
      family = wxS("Helvetica");
      break;
  }

  wxString style = wxEmptyString;
  if (font.GetWeight() == wxFONTWEIGHT_BOLD)
  {
    style += wxString(wxS("B"));
  }
  if (font.GetStyle() == wxFONTSTYLE_ITALIC)
  {
    style += wxString(wxS("I"));
  }
  if (font.GetUnderlined())
  {
    style += wxString(wxS("U"));
  }

  double size = (double) font.GetPointSize();

  bool ok = SelectFont(family, style, size, setFont);
#endif
  return ok;
}

bool
wxPdfDocument::SelectFont(const wxPdfFont& font, int style, double size, bool setFont)
{
  bool selected = false;
  if (font.IsValid())
  {
    m_decoration = style & wxPDF_FONTSTYLE_DECORATION_MASK;
    int fontStyle = font.GetStyle();
    fontStyle |= m_decoration;
    if (size <= 0)
    {
      size = m_fontSizePt;
    }
    if (wxPdfFontManager::GetFontManager()->InitializeFontData(font))
    {
      wxString fontKey = MakeFontKey(font.GetName(), font.GetEncoding());
      // Test if font is already selected
      if (m_currentFont != NULL)
      {
        wxString currentFontKey = MakeFontKey(m_currentFont->GetOriginalName(), m_currentFont->GetFont().GetEncoding());
        selected = (fontKey.IsSameAs(currentFontKey) && m_fontStyle == (style & wxPDF_FONTSTYLE_BOLDITALIC) && m_fontSizePt == size && !m_inTemplate);
      }
      if (!selected)
      {
        selected = true;
        // Test if used for the first time
        wxPdfFontDetails* currentFont = NULL;
        wxPdfFontHashMap::iterator fontIter = (*m_fonts).find(fontKey);
        if (fontIter != (*m_fonts).end())
        {
          currentFont = fontIter->second;
        }
        else
        {
          // Register font in document
          int i = (int) (*m_fonts).size() + 1;
          currentFont = new wxPdfFontDetails(i, font);
          (*m_fonts)[fontKey] = currentFont;

          if (currentFont->HasDiffs())
          {
            // Search existing encodings
            int d = 0;
            int nb = (int) (*m_diffs).size();
            for (i = 1; i <= nb; i++)
            {
              if (*(*m_diffs)[i] == currentFont->GetDiffs())
              {
                d = i;
                break;
              }
            }
            if (d == 0)
            {
              d = nb + 1;
              (*m_diffs)[d] = new wxString(currentFont->GetDiffs());
              (*m_winansi)[d] = currentFont->GetBaseEncoding().IsSameAs(wxS("WinAnsiEncoding"));
            }
            currentFont->SetDiffIndex(d);
          }
        }

        if (selected)
        {
          // Set current font state
          m_fontFamily  = font.GetFamily().Lower();
          m_fontStyle   = font.GetStyle();
          m_fontSizePt  = size;
          m_fontSize    = size / m_k;
          m_currentFont = currentFont;
          if (setFont && m_page > 0)
          {
            OutAscii(wxString::Format(wxS("BT /F%d "),m_currentFont->GetIndex()) +
                     wxPdfUtility::Double2String(m_fontSizePt,2) + wxString(wxS(" Tf ET")));
          }
          if (m_inTemplate)
          {
            (*(m_currentTemplate->m_fonts))[fontKey] = currentFont;
          }
        }
      }
    }
  }
  else
  {
    // Undefined font
    wxLogError(wxString(wxS("wxPdfDocument::SelectFont: ")) +
               wxString(_("Undefined font.")));
  }
  return selected;
}

void
wxPdfDocument::ForceCurrentFont()
{
  if (m_currentFont != NULL)
  {
    wxPdfFont currentFont = m_currentFont->GetUserFont();
    m_currentFont = NULL;
    SelectFont(currentFont, m_fontStyle, m_fontSizePt);
  }
}

wxString
wxPdfDocument::ApplyVisualOrdering(const wxString& txt)
{
  wxString s;
  if (m_currentFont != NULL)
  {
    wxPdfFontExtended font = m_currentFont->GetFont();
    if (font.HasVoltData())
    {
      s = font.ApplyVoltData(txt);
    }
    else
    {
      s = txt;
    }
  }
  else
  {
    s = txt;
  }
  return s;
}

void
wxPdfDocument::EndDoc()
{
  if ((m_isPdfA1 || m_extGStates->size() > 0) && m_PDFVersion < wxS("1.4"))
  {
    m_PDFVersion = wxS("1.4");
  }
  if (m_ocgs->size() > 0 && m_PDFVersion < wxS("1.5"))
  {
    m_PDFVersion = wxS("1.5");
  }
  if (m_importVersion > m_PDFVersion)
  {
    m_PDFVersion = m_importVersion;
  }

  PutHeader();
  PutPages();

  PutResources();

  // Info
  NewObj();
  Out("<<");
  PutInfo();
  Out(">>");
  Out("endobj");

  // Form fields
  PutFormFields();

  // Catalog
  NewObj();
  Out("<<");
  PutCatalog();
  Out(">>");
  Out("endobj");

  // Cross-Reference
  int o = m_buffer->TellO();
  Out("xref");
  OutAscii(wxString(wxS("0 ")) + wxString::Format(wxS("%d"),(m_n+1)));
  Out("0000000000 65535 f ");
  int i;
  for (i = 0; i < m_n; i++)
  {
    OutAscii(wxString::Format(wxS("%010d 00000 n "),(*m_offsets)[i]));
  }

  // Trailer
  Out("trailer");
  Out("<<");
  PutTrailer();
  Out(">>");
  Out("startxref");
  OutAscii(wxString::Format(wxS("%d"),o));
  Out("%%EOF");
  m_state = 3;
}

void
wxPdfDocument::BeginPage(int orientation, wxSize pageSize)
{
  m_page++;
  (*m_pages)[m_page] = new wxMemoryOutputStream();
  m_state = 2;

  // Page orientation
  if (orientation < 0)
  {
    orientation = m_defOrientation;
  }
  if (orientation != m_defOrientation || pageSize != m_defPageSize)
  {
    (*m_orientationChanges)[m_page] = orientation != m_defOrientation;
    if (orientation == wxPORTRAIT)
    {
      (*m_pageSizes)[m_page] = pageSize;
    }
    else
    {
      (*m_pageSizes)[m_page] = wxSize(pageSize.GetHeight(), pageSize.GetWidth());
    }
  }
  if (orientation != m_curOrientation || pageSize != m_curPageSize)
  {
    double fwPt = pageSize.GetWidth() / 254. * 72.;
    double fhPt = pageSize.GetHeight() / 254. * 72.;
    double fw = fwPt / m_k;
    double fh = fhPt / m_k;

    // Change orientation
    if (orientation == wxPORTRAIT)
    {
      m_wPt = fwPt;
      m_hPt = fhPt;
      m_w = fw;
      m_h = fh;
    }
    else
    {
      m_wPt = fhPt;
      m_hPt = fwPt;
      m_w = fh;
      m_h = fw;
    }
    m_pageBreakTrigger = (m_yAxisOriginTop) ? m_h - m_bMargin : m_bMargin;
    m_curOrientation = orientation;
    m_curPageSize = pageSize;
  }
  if (m_yAxisOriginTop)
  {
    Transform(1.0, 0.0, 0.0, -1.0, 0.0, m_h*m_k);
  }
  m_x = m_lMargin;
  m_y = (m_yAxisOriginTop) ? m_tMargin : m_h - m_tMargin;
  m_fontFamily = wxS("");
}

void
wxPdfDocument::EndPage()
{
  // End of page contents
  while (m_inTransform > 0)
  {
    StopTransform();
  }
  m_state = 1;
  ClearGraphicState();
}

void
wxPdfDocument::PutHeader()
{
  OutAscii(wxString(wxS("%PDF-")) + m_PDFVersion);
  Out("%\xE2\xE3\xCF\xD3");
}

void
wxPdfDocument::PutTrailer()
{
  OutAscii(wxString(wxS("/Size ")) + wxString::Format(wxS("%d"),(m_n+1)));
  OutAscii(wxString(wxS("/Root ")) + wxString::Format(wxS("%d"),m_n) + wxString(wxS(" 0 R")));
  OutAscii(wxString(wxS("/Info ")) + wxString::Format(wxS("%d"),(m_n-1)) + wxString(wxS(" 0 R")));

  if (m_encrypted)
  {
    OutAscii(wxString::Format(wxS("/Encrypt %d 0 R"), m_encObjId));
    Out("/ID [", false);
    m_encrypted = false;
    OutHexTextstring(m_encryptor->GetDocumentId(), false);
    OutHexTextstring(m_encryptor->GetDocumentId(), false);
    m_encrypted = true;
    Out("]");
  }
  else if (m_isPdfA1)
  {
    wxString id = wxPdfEncrypt::CreateDocumentId();
    Out("/ID [", false);
    OutHexTextstring(id, false);
    OutHexTextstring(id, false);
    Out("]");
  }
}

int
wxPdfDocument::GetNewObjId()
{
  m_n++;
  return m_n;
}

void
wxPdfDocument::NewObj(int objId)
{
  // Begin a new object
  int id = (objId > 0) ? objId : GetNewObjId();
  (*m_offsets)[id-1] = m_buffer->TellO();
  OutAscii(wxString::Format(wxS("%d"),id) + wxString(wxS(" 0 obj")));
}

void
wxPdfDocument::PutFormFields()
{
  wxPdfFormFieldsMap::iterator formField = m_formFields->begin();
  for (formField = m_formFields->begin(); formField != m_formFields->end(); formField++)
  {
    OutIndirectObject(formField->second);
  }
}

void
wxPdfDocument::PutInfo()
{
  Out("/Producer ",false);
  OutTextstring(wxString(wxPDF_PRODUCER));
  if (m_title.Length() > 0)
  {
    Out("/Title ",false);
    OutTextstring(m_title);
  }
  if (m_subject.Length() > 0)
  {
    Out("/Subject ",false);
    OutTextstring(m_subject);
  }
  if (m_author.Length() > 0)
  {
    Out("/Author ",false);
    OutTextstring(m_author);
  }
  if (m_keywords.Length() > 0)
  {
    Out("/Keywords ",false);
    OutTextstring(m_keywords);
  }
  if (m_creator.Length() > 0)
  {
    Out("/Creator ",false);
    OutTextstring(m_creator);
  }
  Out("/CreationDate ",false);
  if (m_creationDateSet)
  {
    OutRawTextstring(wxString(wxS("D:") + m_creationDate.Format(wxS("%Y%m%d%H%M%SZ"))));
  }
  else
  {
    wxDateTime now = wxDateTime::Now();
    OutRawTextstring(wxString(wxS("D:") + now.Format(wxS("%Y%m%d%H%M%SZ"))));
  }
}

void
wxPdfDocument::PutCatalog()
{
  Out("/Type /Catalog");
  Out("/Pages 1 0 R");

  if (!m_attachments->empty())
  {
    OutAscii(wxString::Format(wxS("/Names <</EmbeddedFiles %d 0 R>>"), m_nAttachments));
  }

  if (m_zoomMode == wxPDF_ZOOM_FULLPAGE)
  {
    OutAscii(wxString::Format(wxS("/OpenAction [%d 0 R /Fit]"), m_firstPageId));
  }
  else if (m_zoomMode == wxPDF_ZOOM_FULLWIDTH)
  {
    OutAscii(wxString::Format(wxS("/OpenAction [%d 0 R /FitH null]"), m_firstPageId));
  }
  else if (m_zoomMode == wxPDF_ZOOM_REAL)
  {
    OutAscii(wxString::Format(wxS("/OpenAction [%d 0 R /XYZ null null 1]"), m_firstPageId));
  }
  else if (m_zoomMode == wxPDF_ZOOM_FACTOR)
  {
    OutAscii(wxString::Format(wxS("/OpenAction [%d 0 R /XYZ null null "), m_firstPageId) +
             wxPdfUtility::Double2String(m_zoomFactor/100.,3) + wxString(wxS("]")));
  }

  if (m_layoutMode == wxPDF_LAYOUT_SINGLE)
  {
    Out("/PageLayout /SinglePage");
  }
  else if (m_layoutMode == wxPDF_LAYOUT_CONTINUOUS)
  {
    Out("/PageLayout /OneColumn");
  }
  else if (m_layoutMode == wxPDF_LAYOUT_TWO)
  {
    Out("/PageLayout /TwoColumnLeft");
  }

  if(m_outlines.GetCount() > 0)
  {
    OutAscii(wxString::Format(wxS("/Outlines %d 0 R"), m_outlineRoot));
  }
  if (m_ocgs->size() > 0)
  {
    Out("/PageMode /UseOC");
  }
  else if(m_outlines.GetCount() > 0)
  {
    Out("/PageMode /UseOutlines");
  }


  if ((m_viewerPrefs > 0) || (m_paperHandling != wxPDF_PAPERHANDLING_DEFAULT))
  {
    Out("/ViewerPreferences <<");
    if (m_viewerPrefs & wxPDF_VIEWER_HIDETOOLBAR)
    {
      Out("/HideToolbar true");
    }
    if (m_viewerPrefs & wxPDF_VIEWER_HIDEMENUBAR)
    {
      Out("/HideMenubar true");
    }
    if (m_viewerPrefs & wxPDF_VIEWER_HIDEWINDOWUI)
    {
      Out("/HideWindowUI true");
    }
    if (m_viewerPrefs & wxPDF_VIEWER_FITWINDOW)
    {
      Out("/FitWindow true");
    }
    if (m_viewerPrefs & wxPDF_VIEWER_CENTERWINDOW)
    {
      Out("/CenterWindow true");
    }
    if (m_viewerPrefs & wxPDF_VIEWER_DISPLAYDOCTITLE)
    {
      Out("/DisplayDocTitle true");
    }
    if (m_viewerPrefs & wxPDF_VIEWER_NOPRINTSCALING)
    {
      Out("/PrintScaling /None");
    }

    switch (m_paperHandling)
    {
      case wxPDF_PAPERHANDLING_SIMPLEX:
        Out("/Duplex /Simplex");
        break;
      case wxPDF_PAPERHANDLING_DUPLEX_FLIP_SHORT_EDGE:
        Out("/Duplex /DuplexFlipShortEdge");
        break;
      case wxPDF_PAPERHANDLING_DUPLEX_FLIP_LONG_EDGE:
        Out("/Duplex /DuplexFlipLongEdge");
        break;
      default:
        break;
    }

    Out(">>");
  }

  if (m_javascript.Length() > 0)
  {
    OutAscii(wxString::Format(wxS("/Names <</JavaScript %d 0 R>>"), m_nJS));
  }

  if ((*m_formFields).size() > 0)
  {
    Out("/AcroForm <<");
    Out("/Fields [", false);
    wxPdfFormFieldsMap::iterator formField = m_formFields->begin();
    for (formField = m_formFields->begin(); formField != m_formFields->end(); formField++)
    {
      wxPdfIndirectObject* field = formField->second;
      OutAscii(wxString::Format(wxS("%d %d R "), field->GetObjectId(), field->GetGenerationId()), false);
    }
    Out("]");
    Out("/DR 2 0 R");
//    Out("/DR [ 2 0 R << /Font << /ZaDb << /Type /Font /Subtype /Type1 /Name /ZaDb /BaseFont /ZapfDingbats >> >> >>]");
    Out("/NeedAppearances true");
    Out(">>");

     // << /DR << /Font << /ZaDb << /Type /Font /Subtype /Type1 /Name /ZaDb /BaseFont /ZapfDingbats >>
     //  /Helv 3 0 R >> >> /DA (/Helv 10 Tf 0 g )
     //  /NeedAppearances true >>

  }

  if (!m_ocgs->empty())
  {
    PutOCProperties();
  }

  if (m_isPdfA1)
  {
    Out("/OutputIntents [", false);
    Out("<</Type /OutputIntent /S /GTS_PDFA1 ", false);
    Out("/OutputConditionIdentifier (sRGB2014.icc) /Info (sRGB2014.icc) /RegistryName (http://www.color.org) ", false);
    OutAscii(wxString::Format(wxS("/DestOutputProfile %d 0 R>>]"), m_nColourProfile));
    OutAscii(wxString::Format(wxS("/Metadata %d 0 R"), m_nMetaData));
  }
}

// --- Fast string search (KMP method) for page number alias replacement

static size_t*
makeFail(const char* target, size_t tlen)
{
  size_t t = 0;
  size_t s, m;
  m = tlen;
  size_t* f = new size_t[m+1];
  f[1] = 0;
  for (s = 1; s < m; s++)
  {
    while ((t > 0) && (target[s] != target[t]))
    {
      t = f[t];
    }
    if (target[t] == target[s])
    {
      t++;
      f[s+1] = t;
    }
    else
    {
      f[s+1] = 0;
    }
  }
  return f;
}

static size_t
findString(const char* src, size_t slen, const char* target, size_t tlen, size_t* f)
{
  size_t s = 0;
  size_t i;
  size_t m = tlen;
  for (i = 0; i < slen; i++)
  {
    while ( (s > 0) && (src[i] != target[s]))
    {
      s = f[s];
    }
    if (src[i] == target[s]) s++;
    if (s == m) return (i-m+1);
  }
  return slen;
}

void
wxPdfDocument::ReplaceNbPagesAlias()
{
  size_t lenAsc = m_aliasNbPages.Length();
#if wxUSE_UNICODE
  const wxScopedCharBuffer wcb(m_aliasNbPages.ToAscii());
  const char* nbAsc = (const char*) wcb;
#else
  const char* nbAsc = m_aliasNbPages.c_str();
#endif
  size_t* fAsc = makeFail(nbAsc,lenAsc);

#if wxUSE_UNICODE
  wxMBConvUTF16BE conv;
  size_t lenUni = conv.FromWChar(NULL, 0, m_aliasNbPages.wc_str(), lenAsc);
  char* nbUni = new char[lenUni+3];
  lenUni = conv.FromWChar(nbUni, lenUni+3, m_aliasNbPages.wc_str(), lenAsc);
  size_t* fUni = makeFail(nbUni,lenUni);
#endif

  wxString pg = wxString::Format(wxS("%d"),m_page);
  size_t lenPgAsc = pg.Length();
#if wxUSE_UNICODE
  const wxScopedCharBuffer wpg(pg.ToAscii());
  const char* pgAsc = (const char*) wpg;
  size_t lenPgUni = conv.FromWChar(NULL, 0, pg.wc_str(), lenPgAsc);
  char* pgUni = new char[lenPgUni+3];
  lenPgUni = conv.FromWChar(pgUni, lenPgUni+3, pg.wc_str(), lenPgAsc);
#else
  const char* pgAsc = pg.c_str();
#endif

  int n;
  for (n = 1; n <= m_page; n++)
  {
    wxMemoryOutputStream* p = new wxMemoryOutputStream();
    wxMemoryInputStream inPage(*((*m_pages)[n]));
    size_t len = inPage.GetSize();
    char* buffer = new char[len];
    char* pBuf = buffer;
    inPage.Read(buffer,len);
    size_t pAsc = findString(buffer,len,nbAsc,lenAsc,fAsc);
#if wxUSE_UNICODE
    size_t pUni = findString(buffer,len,nbUni,lenUni,fUni);
    while (pAsc < len || pUni < len)
    {
      if (pAsc < len && pAsc < pUni)
      {
        if (pAsc > 0)
        {
          p->Write(pBuf,pAsc);
        }
        p->Write(pgAsc,lenPgAsc);
        pBuf += pAsc + lenAsc;
        len  -= (pAsc + lenAsc);
        pUni -= (pAsc + lenAsc);
        pAsc = findString(pBuf,len,nbAsc,lenAsc,fAsc);
      }
      else if (pUni < len && pUni < pAsc)
      {
        if (pUni > 0)
        {
          p->Write(pBuf,pUni);
        }
        p->Write(pgUni,lenPgUni);
        pBuf += pUni + lenUni;
        len  -= (pUni + lenUni);
        pAsc -= (pUni + lenUni);
        pUni = findString(pBuf,len,nbUni,lenUni,fUni);
      }
    }
#else
    while (pAsc < len)
    {
      if (pAsc > 0)
      {
        p->Write(pBuf,pAsc);
      }
      p->Write(pgAsc,lenPgAsc);
      pBuf += pAsc + lenAsc;
      len  -= (pAsc + lenAsc);
      pAsc = findString(pBuf,len,nbAsc,lenAsc,fAsc);
    }
#endif
    if (len > 0)
    {
      p->Write(pBuf,len);
    }
    delete [] buffer;
    delete (*m_pages)[n];
    (*m_pages)[n] = p;
  }

#if wxUSE_UNICODE
  delete [] pgUni;
  delete [] fUni;
  delete [] nbUni;
#endif
  delete [] fAsc;
}

void
wxPdfDocument::PutPages()
{
  double wPt, hPt;
  int nb = m_page;
  int n;
  int nbannot = 0;
  int firstTextAnnotation = m_n;

  //Text annotations
  for (n = 1; n <= nb; n++)
  {
    wxPdfAnnotationsMap::iterator pageAnnots = (*m_annotations).find(n);
    if (pageAnnots != (*m_annotations).end())
    {
      // Links
      wxArrayPtrVoid* pageAnnotsArray = pageAnnots->second;

      unsigned int pageAnnotsCount = (unsigned int) pageAnnotsArray->GetCount();
      unsigned int j;
      for (j = 0; j < pageAnnotsCount; j++)
      {
        wxPdfAnnotation* annotation = (wxPdfAnnotation*) (*pageAnnotsArray)[j];
        NewObj();
        double x = annotation->GetX();
        double y = annotation->GetY();
        Out("<</Type /Annot /Subtype /Text /Rect [", false);
        wxString rect = wxPdfUtility::Double2String(x,2) + wxString(wxS(" ")) +
                        wxPdfUtility::Double2String(y,2) + wxString(wxS(" ")) +
                        wxPdfUtility::Double2String(x,2) + wxString(wxS(" ")) +
                        wxPdfUtility::Double2String(y,2);
        OutAscii(rect,false);
        Out("] /Contents ", false);
        OutTextstring(annotation->GetText(), false);
        Out(">>");
        Out("endobj");
      }
    }
  }

  if (m_aliasNbPages.Length() > 0)
  {
    // Replace number of pages
    ReplaceNbPagesAlias();
  }

  if (m_defOrientation == wxPORTRAIT)
  {
    wPt = m_fwPt;
    hPt = m_fhPt;
  }
  else
  {
    wPt = m_fhPt;
    hPt = m_fwPt;
  }
  wxString filter = (m_compress) ? wxS("/Filter /FlateDecode ") : wxS("");

  m_firstPageId = m_n + 1;
  for (n = 1; n <= nb; n++)
  {
    // Page
    NewObj();
    Out("<</Type /Page");
    Out("/Parent 1 0 R");

    if ((*m_orientationChanges).find(n) != (*m_orientationChanges).end())
    {
      wxSize pageSize = (*m_pageSizes)[n];
      double pageWidth = pageSize.GetWidth() / 254. * 72.;
      double pageHeight = pageSize.GetHeight() / 254. * 72.;
      OutAscii(wxString(wxS("/MediaBox [0 0 ")) +
               wxPdfUtility::Double2String(pageWidth,3) + wxString(wxS(" ")) +
               wxPdfUtility::Double2String(pageHeight,3) + wxString(wxS("]")));
    }

    Out("/Resources 2 0 R");

    Out("/Annots [",false);
    wxPdfPageLinksMap::iterator pageLinks = (*m_pageLinks).find(n);
    if (pageLinks != (*m_pageLinks).end())
    {
      // Links
      wxArrayPtrVoid* pageLinkArray = pageLinks->second;
      unsigned int pageLinkCount = (unsigned int) pageLinkArray->GetCount();
      unsigned int j;
      for (j = 0; j < pageLinkCount; j++)
      {
        wxPdfPageLink* pl = (wxPdfPageLink*) (*pageLinkArray)[j];
        wxString rect = wxPdfUtility::Double2String(pl->GetX(),2) + wxString(wxS(" ")) +
                        wxPdfUtility::Double2String(pl->GetY(),2) + wxString(wxS(" ")) +
                        wxPdfUtility::Double2String(pl->GetX()+pl->GetWidth(),2) + wxString(wxS(" ")) +
                        wxPdfUtility::Double2String(pl->GetY()-pl->GetHeight(),2);
        Out("<</Type /Annot /Subtype /Link /Rect [",false);
        OutAscii(rect,false);
        Out("] /Border [0 0 0] ",false);
        if (!pl->IsLinkRef())
        {
          Out("/A <</S /URI /URI ",false);
          OutAsciiTextstring(pl->GetLinkURL(),false);
          Out(">>>>",false);
        }
        else
        {
          wxPdfLink* link = (*m_links)[pl->GetLinkRef()];
          double y = link->GetPosition()*m_k;
          if (m_yAxisOriginTop)
          {
            double h = hPt;
            if ((*m_orientationChanges).find(link->GetPage()) != (*m_orientationChanges).end())
            {
              wxSize pageSize = (*m_pageSizes)[link->GetPage()];
              h = pageSize.GetHeight() / 254. * 72.;
            }
            y = h - y;
          }
          OutAscii(wxString::Format(wxS("/Dest [%d 0 R /XYZ 0 "),m_firstPageId+2*(link->GetPage()-1)) +
                   wxPdfUtility::Double2String(y,2) +
                   wxString(wxS(" null]>>")),false);
        }
        delete pl;
        (*pageLinkArray)[j] = NULL;
      }
    }
    wxPdfAnnotationsMap::iterator pageAnnots = (*m_annotations).find(n);
    if (pageAnnots != (*m_annotations).end())
    {
      //Text annotations
      wxArrayPtrVoid* pageAnnotsArray = pageAnnots->second;
      unsigned int pageAnnotsCount = (unsigned int) pageAnnotsArray->GetCount();
      unsigned int j;
      for (j = 0; j < pageAnnotsCount; j++)
      {
        nbannot++;
        OutAscii(wxString::Format(wxS("%d 0 R "), firstTextAnnotation+nbannot), false);
        delete ((wxPdfAnnotation*) (*pageAnnotsArray)[j]);
        (*pageAnnotsArray)[j] = NULL;
      }
    }
    wxPdfFormAnnotsMap::iterator formAnnots = (*m_formAnnotations).find(n);
    if (formAnnots != (*m_formAnnotations).end())
    {
      // Form annotations
      wxArrayPtrVoid* formAnnotsArray = formAnnots->second;
      unsigned int formAnnotsCount = (unsigned int) formAnnotsArray->GetCount();
      unsigned int j;
      for (j = 0; j < formAnnotsCount; j++)
      {
        wxPdfIndirectObject* object = static_cast<wxPdfIndirectObject*>((*formAnnotsArray)[j]);
        OutAscii(wxString::Format(wxS("%d %d R "), object->GetObjectId(), object->GetGenerationId()), false);
//        delete ((wxPdfAnnotation*) (*formAnnotsArray)[j]);
//        (*formAnnotsArray)[j] = NULL;
      }
    }
    Out("]");
    // TODO: not sure whether writing the group dictionary is necessary
    if (!m_isPdfA1 && m_PDFVersion > wxS("1.3"))
    {
      Out("/Group <</Type /Group /S /Transparency /CS /DeviceRGB>>");
    }
    OutAscii(wxString::Format(wxS("/Contents %d 0 R>>"), m_n+1));
    Out("endobj");

    // Page content
    wxMemoryOutputStream mos;
    wxMemoryOutputStream* p = (*m_pages)[n];
    if (m_compress)
    {
      wxZlibOutputStream q(mos);
      wxMemoryInputStream tmp(*p);
      q.Write(tmp);
      p = &mos;
    }

    NewObj();
    OutAscii(wxString(wxS("<<")) + filter + wxString(wxS("/Length ")) +
             wxString::Format(wxS("%lu"), (unsigned long) CalculateStreamLength(p->TellO())) + wxString(wxS(">>")));
    PutStream(*p);
    Out("endobj");
  }
  // Pages root
  (*m_offsets)[0] = m_buffer->TellO();
  Out("1 0 obj");
  Out("<</Type /Pages");
  wxString kids = wxS("/Kids [");
  int i;
  for (i = 0; i < nb; i++)
  {
    kids += wxString::Format(wxS("%d"),(m_firstPageId+2*i)) + wxString(wxS(" 0 R "));
  }
  OutAscii(kids + wxString(wxS("]")));
  OutAscii(wxString(wxS("/Count ")) + wxString::Format(wxS("%d"),nb));
  OutAscii(wxString(wxS("/MediaBox [0 0 ")) +
           wxPdfUtility::Double2String(wPt,3) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(hPt,3) + wxString(wxS("]")));
  Out(">>");
  Out("endobj");
}

static const wxStringCharType* gs_bms[] = {
  wxS("/Normal"),     wxS("/Multiply"),   wxS("/Screen"),    wxS("/Overlay"),    wxS("/Darken"),
  wxS("/Lighten"),    wxS("/ColorDodge"), wxS("/ColorBurn"), wxS("/HardLight"),  wxS("/SoftLight"),
  wxS("/Difference"), wxS("/Exclusion"),  wxS("/Hue"),       wxS("/Saturation"), wxS("/Color"),
  wxS("Luminosity")
};

void
wxPdfDocument::PutExtGStates()
{
  wxPdfExtGStateMap::iterator extGState;
  for (extGState = m_extGStates->begin(); extGState != m_extGStates->end(); extGState++)
  {
    NewObj();
    extGState->second->SetObjIndex(m_n);
    Out("<</Type /ExtGState");
    OutAscii(wxString(wxS("/ca ")) + wxPdfUtility::Double2String(extGState->second->GetFillAlpha(), 3));
    OutAscii(wxString(wxS("/CA ")) + wxPdfUtility::Double2String(extGState->second->GetLineAlpha(), 3));
    OutAscii(wxString(wxS("/bm ")) + wxString(gs_bms[extGState->second->GetBlendMode()]));
    Out(">>");
    Out("endobj");
  }
}

void
wxPdfDocument::PutShaders()
{
  wxPdfGradientMap::iterator gradient;
  for (gradient = m_gradients->begin(); gradient != m_gradients->end(); gradient++)
  {
    wxPdfGradientType type = gradient->second->GetType();
    switch (type)
    {
      case wxPDF_GRADIENT_AXIAL:
      case wxPDF_GRADIENT_MIDAXIAL:
      case wxPDF_GRADIENT_RADIAL:
      {
        wxPdfColour colour1 = ((wxPdfAxialGradient*)(gradient->second))->GetColour1();
        wxPdfColour colour2 = ((wxPdfAxialGradient*)(gradient->second))->GetColour2();
        double intexp      = ((wxPdfAxialGradient*)(gradient->second))->GetIntExp();

        NewObj();
        Out("<<");
        Out("/FunctionType 2");
        Out("/Domain [0.0 1.0]");
        Out("/C0 [", false);
        OutAscii(colour1.GetColourValue(), false);
        Out("]");
        Out("/C1 [", false);
        OutAscii(colour2.GetColourValue(), false);
        Out("]");
        OutAscii(wxString(wxS("/N ")) + wxPdfUtility::Double2String(intexp,2));
        Out(">>");
        Out("endobj");
        int f1 = m_n;

        if (type == wxPDF_GRADIENT_MIDAXIAL)
        {
          double midpoint = ((wxPdfMidAxialGradient*)(gradient->second))->GetMidPoint();
          NewObj();
          Out("<<");
          Out("/FunctionType 3");
          Out("/Domain [0.0 1.0]");
          OutAscii(wxString::Format(wxS("/Functions [%d 0 R %d 0 R]"), f1, f1));
          OutAscii(wxString(wxS("/Bounds [")) + wxPdfUtility::Double2String(midpoint,3) + wxString(wxS("]")));
          Out("/Encode [0.0 1.0 1.0 0.0]");
          Out(">>");
          Out("endobj");
          f1 = m_n;
        }

        NewObj();
        Out("<<");
        OutAscii(wxString::Format(wxS("/ShadingType %d"), ((type == wxPDF_GRADIENT_RADIAL) ? 3 : 2)));
        switch (colour1.GetColourType())
        {
          case wxPDF_COLOURTYPE_GRAY:
            Out("/ColorSpace /DeviceGray");
            break;
          case wxPDF_COLOURTYPE_CMYK:
            Out("/ColorSpace /DeviceCMYK");
            break;
          case wxPDF_COLOURTYPE_RGB:
          default:
            Out("/ColorSpace /DeviceRGB");
            break;
        }
        if (type == wxPDF_GRADIENT_AXIAL ||
            type == wxPDF_GRADIENT_MIDAXIAL)
        {
          wxPdfAxialGradient* grad = (wxPdfAxialGradient*) (gradient->second);
          OutAscii(wxString(wxS("/Coords [")) +
                   wxPdfUtility::Double2String(grad->GetX1(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetY1(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetX2(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetY2(),3) + wxString(wxS("]")));
          OutAscii(wxString::Format(wxS("/Function %d 0 R"), f1));
          Out("/Extend [true true] ");
        }
        else
        {
          wxPdfRadialGradient* grad = (wxPdfRadialGradient*) (gradient->second);
          OutAscii(wxString(wxS("/Coords [")) +
                   wxPdfUtility::Double2String(grad->GetX1(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetY1(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetR1(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetX2(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetY2(),3) + wxString(wxS(" ")) +
                   wxPdfUtility::Double2String(grad->GetR2(),3) + wxString(wxS("]")));
          OutAscii(wxString::Format(wxS("/Function %d 0 R"), f1));
          Out("/Extend [true true] ");
        }
        Out(">>");
        Out("endobj");
        gradient->second->SetObjIndex(m_n);
        break;
      }
      case wxPDF_GRADIENT_COONS:
      {
        wxPdfCoonsPatchGradient* grad = (wxPdfCoonsPatchGradient*) (gradient->second);
        NewObj();
        Out("<<");
        Out("/ShadingType 6");
        switch (grad->GetColourType())
        {
          case wxPDF_COLOURTYPE_GRAY:
            Out("/ColorSpace /DeviceGray");
            break;
          case wxPDF_COLOURTYPE_CMYK:
            Out("/ColorSpace /DeviceCMYK");
            break;
          case wxPDF_COLOURTYPE_RGB:
          default:
            Out("/ColorSpace /DeviceRGB");
            break;
        }
        Out("/BitsPerCoordinate 16");
        Out("/BitsPerComponent 8");
        Out("/Decode[0 1 0 1 0 1 0 1 0 1]");
        Out("/BitsPerFlag 8");
        wxMemoryOutputStream* p = grad->GetBuffer();
        OutAscii(wxString::Format(wxS("/Length %lu"), (unsigned long) CalculateStreamLength(p->TellO())));
        Out(">>");
        PutStream(*p);
        Out("endobj");
        gradient->second->SetObjIndex(m_n);
      }
      default:
        break;
    }
  }
}

void
wxPdfDocument::PutFonts()
{
  int nf = m_n;

  int nb = (int) (*m_diffs).size();
  int i;
  for (i = 1; i <= nb; i++)
  {
    // Encodings
    NewObj();
    Out("<</Type /Encoding ", false);
    if ((*m_winansi)[i])
    {
      Out("/BaseEncoding /WinAnsiEncoding ", false);
    }
    Out("/Differences [", false);
    OutAscii(*(*m_diffs)[i], false);
    Out("]>>");
    Out("endobj");
  }

  wxString type;
  wxString name;
  wxPdfFontHashMap::iterator fontIter = m_fonts->begin();
  for (fontIter = m_fonts->begin(); fontIter != m_fonts->end(); fontIter++)
  {
    wxPdfFontDetails* font = fontIter->second;
    wxPdfFontExtended extFont = font->GetFont();
    if (extFont.IsEmbedded())
    {
      // Font data embedding
      type = font->GetType();
      NewObj();
      font->SetFileIndex(m_n);

      wxMemoryOutputStream p;
      size_t fontSize1 = font->WriteFontData(&p);

      size_t fontLen = CalculateStreamLength(p.TellO());
      OutAscii(wxString::Format(wxS("<</Length %lu"), (unsigned long) fontLen));
      // Note that font data is always compressed, so do _not_ check m_compress here
      Out("/Filter /FlateDecode");
      if (type == wxS("OpenTypeUnicode"))
      {
        Out("/Subtype /CIDFontType0C");
      }
      else
      {
        OutAscii(wxString::Format(wxS("/Length1 %lu"), (unsigned long) fontSize1));
        if (extFont.HasSize2())
        {
          OutAscii(wxString::Format(wxS("/Length2 %lu /Length3 0"), (unsigned long) extFont.GetSize2()));
        }
      }
      Out(">>");
      PutStream(p);
      Out("endobj");
    }
  }

  fontIter = m_fonts->begin();
  for (fontIter = m_fonts->begin(); fontIter != m_fonts->end(); fontIter++)
  {
    // Font objects
    wxPdfFontDetails* font = fontIter->second;
    wxPdfFontExtended extFont = font->GetFont();
    font->SetObjIndex(m_n+1);
    type = font->GetType();
    name = font->GetName();
    if (type == wxS("core"))
    {
      // Standard font
      NewObj();
      Out("<</Type /Font");
      OutAscii(wxString(wxS("/BaseFont /"))+name);
      Out("/Subtype /Type1");
      if (name != wxS("Symbol") && name != wxS("ZapfDingbats"))
      {
        Out("/Encoding /WinAnsiEncoding");
      }
      Out(">>");
      Out("endobj");

      if (m_isPdfA1)
      {
        wxLogError(wxString(wxS("wxPdfDocument::PutFonts: ")) +
                   name + wxString(wxS(" - ")) + wxString(_("core font not allowed; all fonts must be embedded in PDF/A-1.")));
      }
    }
    else if (type == wxS("Type1") || type == wxS("TrueType"))
    {
      // Additional Type1 or TrueType font
      bool hasToUnicodeMap = (type == wxS("Type1") && extFont.HasEncodingMap());
      NewObj();
      Out("<</Type /Font");
      OutAscii(wxString(wxS("/BaseFont /")) + name);
      OutAscii(wxString(wxS("/Subtype /")) + type);
      Out("/FirstChar 32 /LastChar 255");
      OutAscii(wxString::Format(wxS("/Widths %d  0 R"), m_n+1));
      OutAscii(wxString::Format(wxS("/FontDescriptor %d 0 R"), m_n+2));
      if (extFont.GetEncoding() != wxS(""))
      {
        if (extFont.HasDiffs())
        {
          OutAscii(wxString::Format(wxS("/Encoding %d 0 R"), (nf+font->GetDiffIndex())));
        }
        else
        {
          Out("/Encoding /WinAnsiEncoding");
        }
      }
      if (hasToUnicodeMap)
      {
        OutAscii(wxString::Format(wxS("/ToUnicode %d 0 R"), (m_n + 3)));
      }
      Out(">>");
      Out("endobj");

      // Widths
      NewObj();
      wxString s = font->GetWidthsAsString();
      OutAscii(s);
      Out("endobj");

      // Descriptor
      const wxPdfFontDescription& fd = font->GetDescription();
      NewObj();
      Out("<</Type /FontDescriptor");
      OutAscii(wxString(wxS("/FontName /")) + name);
      OutAscii(wxString::Format(wxS("/Ascent %d"), fd.GetAscent()));
      OutAscii(wxString::Format(wxS("/Descent %d"), fd.GetDescent()));
      OutAscii(wxString::Format(wxS("/CapHeight %d"), fd.GetCapHeight()));
      OutAscii(wxString::Format(wxS("/Flags %d"), fd.GetFlags()));
      OutAscii(wxString(wxS("/FontBBox")) + fd.GetFontBBox());
      OutAscii(wxString::Format(wxS("/ItalicAngle %d"), fd.GetItalicAngle()));
      OutAscii(wxString::Format(wxS("/StemV %d"), fd.GetStemV()));
      OutAscii(wxString::Format(wxS("/MissingWidth %d"), fd.GetMissingWidth()));
      if (extFont.IsEmbedded())
      {
        if (type == wxS("Type1"))
        {
          OutAscii(wxString::Format(wxS("/FontFile %d 0 R"), font->GetFileIndex()));
        }
        else
        {
          OutAscii(wxString::Format(wxS("/FontFile2 %d 0 R"), font->GetFileIndex()));
        }
      }
      Out(">>");
      Out("endobj");

      if (type == wxS("Type1") && extFont.HasEncodingMap())
      {
        // Embed ToUnicode Map
        // A specification of the mapping from CIDs to glyph indices
        NewObj();
        wxMemoryOutputStream p;
        /* size_t mapSize = */ font->WriteUnicodeMap(&p);
        size_t mapLen = CalculateStreamLength(p.TellO());
        OutAscii(wxString::Format(wxS("<</Length %lu"), (unsigned long) mapLen));
        Out("/Filter /FlateDecode");
        Out(">>");
        PutStream(p);
        Out("endobj");
      }
    }
    else if (type == wxS("TrueTypeUnicode") || type == wxS("OpenTypeUnicode"))
    {
      // Type0 Font
      // A composite font composed of other fonts, organized hierarchically
      NewObj();
      Out("<</Type /Font");
      Out("/Subtype /Type0");
      OutAscii(wxString(wxS("/BaseFont /")) + name);
      // The horizontal identity mapping for 2-byte CIDs; may be used with
      // CIDFonts using any Registry, Ordering, and Supplement values.
      Out("/Encoding /Identity-H");
      OutAscii(wxString::Format(wxS("/DescendantFonts [%d 0 R]"), (m_n + 1)));
      OutAscii(wxString::Format(wxS("/ToUnicode %d 0 R"), (m_n + 4)));
      Out(">>");
      Out("endobj");

      // CIDFontType
      NewObj();
      Out("<</Type /Font");
      if (type == wxS("TrueTypeUnicode"))
      {
        // A CIDFont whose glyph descriptions are based on TrueType font technology
        Out("/Subtype /CIDFontType2");
      }
      else
      {
        // A CIDFont whose glyph descriptions are based on OpenType font technology
        Out("/Subtype /CIDFontType0");
      }
      OutAscii(wxString(wxS("/BaseFont /")) + name);
      OutAscii(wxString::Format(wxS("/CIDSystemInfo %d 0 R"), (m_n + 1)));
      OutAscii(wxString::Format(wxS("/FontDescriptor %d 0 R"), (m_n + 2)));

      const wxPdfFontDescription& fd = font->GetDescription();
      if (fd.GetMissingWidth() > 0)
      {
        // The default width for glyphs in the CIDFont MissingWidth
        OutAscii(wxString::Format(wxS("/DW %d"), fd.GetMissingWidth()));
      }

      OutAscii(wxString(wxS("/W ")) + font->GetWidthsAsString()); // A description of the widths for the glyphs in the CIDFont

      if (type == wxS("TrueTypeUnicode"))
      {
        OutAscii(wxString::Format(wxS("/CIDToGIDMap %d 0 R"), (m_n + 4)));
      }

      Out(">>");
      Out("endobj");

      // CIDSystemInfo dictionary
      // A dictionary containing entries that define the character collectionof the CIDFont.
      NewObj();
      // A string identifying an issuer of character collections
      Out("<</Registry ", false);
      OutAsciiTextstring(wxString(wxS("Adobe")));
      // A string that uniquely names a character collection issued by a specific registry
      Out("/Ordering ", false);
      OutAsciiTextstring(wxString(wxS("Identity")));

      // The supplement number of the character collection.
      Out("/Supplement 0");
      Out(">>");
      Out("endobj");

      // Font descriptor
      // A font descriptor describing the CIDFonts default metrics other than its glyph widths
      NewObj();
      Out("<</Type /FontDescriptor");
      OutAscii(wxString(wxS("/FontName /")) + name);
      wxString s = wxEmptyString;
      OutAscii(wxString::Format(wxS("/Ascent %d"), fd.GetAscent()));
      OutAscii(wxString::Format(wxS("/Descent %d"), fd.GetDescent()));
      OutAscii(wxString::Format(wxS("/CapHeight %d"), fd.GetCapHeight()));
      OutAscii(wxString::Format(wxS("/Flags %d"), fd.GetFlags()));
      OutAscii(wxString(wxS("/FontBBox")) + fd.GetFontBBox());
      OutAscii(wxString::Format(wxS("/ItalicAngle %d"), fd.GetItalicAngle()));
      OutAscii(wxString::Format(wxS("/StemV %d"), fd.GetStemV()));
      OutAscii(wxString::Format(wxS("/MissingWidth %d"), fd.GetMissingWidth()));

      if (extFont.IsEmbedded())
      {
        if (type == wxS("TrueTypeUnicode"))
        {
          // A stream containing a TrueType font program
          OutAscii(wxString::Format(wxS("/FontFile2 %d 0 R"), font->GetFileIndex()));
          if (extFont.SubsetRequested())
          {
            OutAscii(wxString::Format(wxS("/CIDSet %d 0 R"), m_n + 3));
          }
        }
        else
        {
          // A stream containing a CFF font program
          OutAscii(wxString::Format(wxS("/FontFile3 %d 0 R"), font->GetFileIndex()));
          if (extFont.SubsetRequested())
          {
            OutAscii(wxString::Format(wxS("/CIDSet %d 0 R"), m_n + 2));
          }
        }
      }
      Out(">>");
      Out("endobj");

      // Embed ToUnicode CMap
      // A specification of the mapping from CIDs to Unicode values
      // Put it in a block of its own just for consistency with the code below,
      // even if this is done unconditionally.
      {
          NewObj();
          wxMemoryOutputStream mos;
          /* size_t mapSize = */ font->WriteUnicodeMap(&mos);
          size_t mapLen = CalculateStreamLength(mos.TellO());
          OutAscii(wxString::Format(wxS("<</Length %lu"), (unsigned long) mapLen));
          // Decompresses data encoded using the public-domain zlib/deflate compression
          // method, reproducing the original text or binary data
          Out("/Filter /FlateDecode");
          Out(">>");
          PutStream(mos);
          Out("endobj");
      }

      if (type == wxS("TrueTypeUnicode"))
      {
        // Embed CIDToGIDMap
        // A specification of the mapping from CIDs to glyph indices
        NewObj();
        wxMemoryOutputStream mos;
        /* size_t mapSize = */ font->WriteCIDToGIDMap(&mos);
        size_t mapLen = CalculateStreamLength(mos.TellO());
        OutAscii(wxString::Format(wxS("<</Length %lu"), (unsigned long)mapLen));
        // Decompresses data encoded using the public-domain zlib/deflate compression
        // method, reproducing the original text or binary data
        Out("/Filter /FlateDecode");
        Out(">>");
        PutStream(mos);
        Out("endobj");
      }
      if (extFont.IsEmbedded() && extFont.SubsetRequested())
      {
        // Embed CID set
        // A specification which CIDs are present in the subset
        NewObj();
        wxMemoryOutputStream mos;
        /* size_t mapSize = */ font->WriteCIDSet(&mos);
        size_t setLen = CalculateStreamLength(mos.TellO());
        OutAscii(wxString::Format(wxS("<</Length %lu"), (unsigned long)setLen));
        // Decompresses data encoded using the public-domain zlib/deflate compression
        // method, reproducing the original text or binary data
        Out("/Filter /FlateDecode");
        Out(">>");
        PutStream(mos);
        Out("endobj");
      }
    }
    else if (type == wxS("Type0"))
    {
      // Type0
      NewObj();
      Out("<</Type /Font");
      Out("/Subtype /Type0");
      OutAscii(wxString(wxS("/BaseFont /")) + name + wxString(wxS("-")) + extFont.GetCMap());
      OutAscii(wxString(wxS("/Encoding /")) + extFont.GetCMap());
      OutAscii(wxString::Format(wxS("/DescendantFonts [%d 0 R]"), (m_n+1)));
      Out(">>");
      Out("endobj");

      // CIDFont
      NewObj();
      Out("<</Type /Font");
      Out("/Subtype /CIDFontType0");
      OutAscii(wxString(wxS("/BaseFont /")) + name);
      Out("/CIDSystemInfo <</Registry ", false);
      OutAsciiTextstring(wxS("Adobe"), false);
      Out("/Ordering ", false);
      OutAsciiTextstring(extFont.GetOrdering(), false);
      OutAscii(wxString(wxS(" /Supplement ")) + extFont.GetSupplement() + wxString(wxS(">>")));
      OutAscii(wxString::Format(wxS("/FontDescriptor %d 0 R"), (m_n+1)));

      // Widths
      // A description of the widths for the glyphs in the CIDFont
      OutAscii(wxString(wxS("/W ")) + font->GetWidthsAsString());
      Out(">>");
      Out("endobj");

      // Font descriptor
      const wxPdfFontDescription& fd = font->GetDescription();
      NewObj();
      Out("<</Type /FontDescriptor");
      OutAscii(wxString(wxS("/FontName /")) + name);
      OutAscii(wxString::Format(wxS("/Ascent %d"), fd.GetAscent()));
      OutAscii(wxString::Format(wxS("/Descent %d"), fd.GetDescent()));
      OutAscii(wxString::Format(wxS("/CapHeight %d"), fd.GetCapHeight()));
      OutAscii(wxString::Format(wxS("/Flags %d"), fd.GetFlags()));
      OutAscii(wxString(wxS("/FontBBox")) + fd.GetFontBBox());
      OutAscii(wxString::Format(wxS("/ItalicAngle %d"), fd.GetItalicAngle()));
      OutAscii(wxString::Format(wxS("/StemV %d"), fd.GetStemV()));
      Out(">>");
      Out("endobj");
    }
  }
}

void
wxPdfDocument::PutImages()
{
  wxString filter = (m_compress) ? wxS("/Filter /FlateDecode ") : wxS("");
  int iter;
  for (iter = 0; iter < 2; iter++)
  {
    // We need two passes to resolve dependencies
    wxPdfImageHashMap::iterator image = m_images->begin();
    for (image = m_images->begin(); image != m_images->end(); image++)
    {
      // Image objects
      wxPdfImage* currentImage = image->second;

      if (currentImage->GetMaskImage() > 0)
      {
        // On first pass skip images depending on a mask
        if (iter == 0) continue;
      }
      else
      {
        // On second pass skip images already processed
        if (iter != 0) continue;
      }

      NewObj();
      currentImage->SetObjIndex(m_n);
      Out("<</Type /XObject");
      if (currentImage->IsFormObject())
      {
        Out("/Subtype /Form");
        OutAscii(wxString::Format(wxS("/BBox [%d %d %d %d]"),
                   currentImage->GetX(), currentImage->GetY(),
                   currentImage->GetWidth()+currentImage->GetX(),
                   currentImage->GetHeight() + currentImage->GetY()));
        if (m_compress)
        {
          Out("/Filter /FlateDecode");
        }
        size_t dataLen = currentImage->GetDataSize();
        wxMemoryOutputStream p;
        if (m_compress)
        {
          wxZlibOutputStream q(p);
          q.Write(currentImage->GetData(),currentImage->GetDataSize());
        }
        else
        {
          p.Write(currentImage->GetData(),currentImage->GetDataSize());
        }
        dataLen = CalculateStreamLength(p.TellO());
        OutAscii(wxString::Format(wxS("/Length %lu>>"), (unsigned long) dataLen));
        PutStream(p);

        Out("endobj");
      }
      else
      {
        Out("/Subtype /Image");
        OutAscii(wxString::Format(wxS("/Width %d"),currentImage->GetWidth()));
        OutAscii(wxString::Format(wxS("/Height %d"),currentImage->GetHeight()));

        int maskImage = currentImage->GetMaskImage();
        if (maskImage > 0)
        {
          int maskObjId = 0;
          wxPdfImageHashMap::iterator img = m_images->begin();
          while (maskObjId == 0 && img != m_images->end())
          {
            if (img->second->GetIndex() == maskImage)
            {
              maskObjId = img->second->GetObjIndex();
            }
            img++;
          }
          if (maskObjId > 0)
          {
            OutAscii(wxString::Format(wxS("/SMask %d 0 R"), maskObjId));
          }
        }

        if (currentImage->GetColourSpace() == wxS("Indexed"))
        {
          int palLen = currentImage->GetPaletteSize() / 3 - 1;
          OutAscii(wxString::Format(wxS("/ColorSpace [/Indexed /DeviceRGB %d %d 0 R]"),
                   palLen,(m_n+1)));
        }
        else
        {
          OutAscii(wxString(wxS("/ColorSpace /")) + currentImage->GetColourSpace());
          if (currentImage->GetColourSpace() == wxS("DeviceCMYK"))
          {
            Out("/Decode [1 0 1 0 1 0 1 0]");
          }
        }
        OutAscii(wxString::Format(wxS("/BitsPerComponent %d"),currentImage->GetBitsPerComponent()));
        wxString f = currentImage->GetF();
        if (f.Length() > 0)
        {
          OutAscii(wxString(wxS("/Filter /")) + f);
        }
        wxString parms = currentImage->GetParms();
        if (parms.Length() > 0)
        {
          OutAscii(parms);
        }
        int trnsSize = currentImage->GetTransparencySize();
        unsigned char* trnsData = (unsigned char*) currentImage->GetTransparency();
        if (trnsSize > 0)
        {
          wxString trns = wxS("");;
          int i;
          for (i = 0; i < trnsSize; i++)
          {
            int trnsValue = trnsData[i];
            trns += wxString::Format(wxS("%d %d "), trnsValue, trnsValue);
          }
          OutAscii(wxString(wxS("/Mask [")) + trns + wxString(wxS("]")));
        }

        OutAscii(wxString::Format(wxS("/Length %lu>>"), (unsigned long) CalculateStreamLength(currentImage->GetDataSize())));

        wxMemoryOutputStream mos;
        mos.Write(currentImage->GetData(),currentImage->GetDataSize());
        PutStream(mos);
        Out("endobj");

        // Palette
        if (currentImage->GetColourSpace() == wxS("Indexed"))
        {
          NewObj();
          unsigned int palLen = currentImage->GetPaletteSize();
          wxMemoryOutputStream mos2;
          if (m_compress)
          {
            wxZlibOutputStream q(mos2);
            q.Write(currentImage->GetPalette(),currentImage->GetPaletteSize());
          }
          else
          {
            mos2.Write(currentImage->GetPalette(),currentImage->GetPaletteSize());
          }
          palLen = (unsigned int) CalculateStreamLength(mos2.TellO());
          OutAscii(wxString(wxS("<<")) + filter + wxString::Format(wxS("/Length %d>>"), palLen));
          PutStream(mos2);
          Out("endobj");
        }
      }
    }
  }
}

void
wxPdfDocument::PutTemplates()
{
  wxString filter = (m_compress) ? wxS("/Filter /FlateDecode ") : wxS("");
  wxPdfTemplatesMap::iterator templateIter = m_templates->begin();
  for (templateIter = m_templates->begin(); templateIter != m_templates->end(); templateIter++)
  {
    // Image objects
    wxPdfTemplate* currentTemplate = templateIter->second;
    NewObj();
    currentTemplate->SetObjIndex(m_n);

    OutAscii(wxString(wxS("<<")) + filter + wxString(wxS("/Type /XObject")));
    Out("/Subtype /Form");
    Out("/FormType 1");

    OutAscii(wxString(wxS("/BBox [")) +
             wxPdfUtility::Double2String(currentTemplate->GetX()*m_k,2) + wxString(wxS(" ")) +
             wxPdfUtility::Double2String(currentTemplate->GetY()*m_k,2) + wxString(wxS(" ")) +
             wxPdfUtility::Double2String((currentTemplate->GetX()+currentTemplate->GetWidth())*m_k,2) + wxString(wxS(" ")) +
             wxPdfUtility::Double2String((currentTemplate->GetY()+currentTemplate->GetHeight())*m_k,2) + wxString(wxS("]")));

    Out("/Resources ");
    if (currentTemplate->GetResources() != NULL)
    {
      m_currentParser = currentTemplate->GetParser();
      WriteObjectValue(currentTemplate->GetResources());
    }
    else
    {
      Out("<</ProcSet [/PDF /Text /ImageB /ImageC /ImageI]");
      // Font references
      if (currentTemplate->m_fonts->size() > 0)
      {
        Out("/Font <<");
        wxPdfFontHashMap::iterator font = currentTemplate->m_fonts->begin();
        for (font = currentTemplate->m_fonts->begin(); font != currentTemplate->m_fonts->end(); font++)
        {
          OutAscii(wxString::Format(wxS("/F%d %d 0 R"), font->second->GetIndex(), font->second->GetObjIndex()));
        }
        Out(">>");
      }
      // Image and template references
      if (currentTemplate->m_images->size() > 0 ||
          currentTemplate->m_templates->size() > 0)
      {
        Out("/XObject <<");
        wxPdfImageHashMap::iterator image = currentTemplate->m_images->begin();
        for (image = currentTemplate->m_images->begin(); image != currentTemplate->m_images->end(); image++)
        {
          wxPdfImage* currentImage = image->second;
          OutAscii(wxString::Format(wxS("/I%d %d 0 R"), currentImage->GetIndex(), currentImage->GetObjIndex()));
        }
        for (wxPdfTemplatesMap::iterator templateIter2 = currentTemplate->m_templates->begin(); templateIter2 != currentTemplate->m_templates->end(); templateIter2++)
        {
          wxPdfTemplate* tpl = templateIter2->second;
          OutAscii(m_templatePrefix + wxString::Format(wxS("%d %d 0 R"), tpl->GetIndex(), tpl->GetObjIndex()));
        }
        Out(">>");
      }
      // References to extended graphics states
      if (currentTemplate->m_extGStates->size() > 0)
      {
        Out("/ExtGState <<");
        wxPdfExtGStateMap::iterator extGState = currentTemplate->m_extGStates->begin();
        for (extGState = currentTemplate->m_extGStates->begin(); extGState != currentTemplate->m_extGStates->end(); extGState++)
        {
          OutAscii(wxString::Format(wxS("/GS%ld %d 0 R"), extGState->first, extGState->second->GetObjIndex()));
        }
        Out(">>");
      }
      // References to patterns
      if (currentTemplate->m_patterns->size() > 0)
      {
        Out("/Pattern <<");
        wxPdfPatternMap::iterator pattern;
        for (pattern = currentTemplate->m_patterns->begin(); pattern != currentTemplate->m_patterns->end(); pattern++)
        {
          OutAscii(wxString::Format(wxS("/P%d %d 0 R"), pattern->second->GetIndex(), pattern->second->GetObjIndex()));
        }
        Out(">>");
      }
      Out(">>");
    }

    // Template data
    wxMemoryOutputStream mos, *p;
    if (m_compress)
    {
      p = &mos;
      wxZlibOutputStream q(mos);
      if (currentTemplate->m_buffer.GetLength() > 0)
      {
        wxMemoryInputStream tmp(currentTemplate->m_buffer);
        q.Write(tmp);
      }
    }
    else
    {
      p = &(currentTemplate->m_buffer);
    }

    OutAscii(wxString::Format(wxS("/Length %lu >>"), (unsigned long) CalculateStreamLength(p->TellO())));
    int nSave = m_n;
    m_n = currentTemplate->GetObjIndex();
    PutStream(*p);
    Out("endobj");
    m_n = nSave;
  }
}

void
wxPdfDocument::PutImportedObjects()
{
  wxPdfParserMap::iterator parser = m_parsers->begin();
  for (parser = m_parsers->begin(); parser != m_parsers->end(); parser++)
  {
    m_currentParser = parser->second;
    if (m_currentParser != NULL)
    {
      m_currentParser->SetUseRawStream(true);
      wxPdfObjectQueue* entry = m_currentParser->GetObjectQueue();
      while ((entry = entry->GetNext()) != NULL)
      {
        wxPdfObject* resolvedObject = m_currentParser->ResolveObject(entry->GetObject());
        resolvedObject->SetActualId(entry->GetActualObjectId());
        NewObj(entry->GetActualObjectId());
        WriteObjectValue(resolvedObject);
        Out("endobj");
        entry->SetObject(resolvedObject);
      }
    }
  }
}

void
wxPdfDocument::PutXObjectDict()
{
  wxPdfImageHashMap::iterator image = m_images->begin();
  for (image = m_images->begin(); image != m_images->end(); image++)
  {
    wxPdfImage* currentImage = image->second;
    OutAscii(wxString::Format(wxS("/I%d %d 0 R"), currentImage->GetIndex(), currentImage->GetObjIndex()));
  }
  wxPdfTemplatesMap::iterator templateIter = m_templates->begin();
  for (templateIter = m_templates->begin(); templateIter != m_templates->end(); templateIter++)
  {
    wxPdfTemplate* tpl = templateIter->second;
    OutAscii(m_templatePrefix + wxString::Format(wxS("%d %d 0 R"), tpl->GetIndex(), tpl->GetObjIndex()));
  }
}

void
wxPdfDocument::PutResourceDict()
{
  Out("/ProcSet [/PDF /Text /ImageB /ImageC /ImageI]");

  Out("/Font <<");
  wxPdfFontHashMap::iterator font = m_fonts->begin();

  for (font = m_fonts->begin(); font != m_fonts->end(); font++)
  {
    OutAscii(wxString::Format(wxS("/F%d %d 0 R"), font->second->GetIndex(), font->second->GetObjIndex()));
  }
  Out(">>");

  Out("/XObject <<");
  PutXObjectDict();
  Out(">>");

  if (!m_extGStates->empty())
  {
    Out("/ExtGState <<");
    wxPdfExtGStateMap::iterator extGState;
    for (extGState = m_extGStates->begin(); extGState != m_extGStates->end(); extGState++)
    {
      OutAscii(wxString::Format(wxS("/GS%ld %d 0 R"), extGState->first, extGState->second->GetObjIndex()));
    }
    Out(">>");
  }

  if (!m_gradients->empty())
  {
    Out("/Shading <<");
    wxPdfGradientMap::iterator gradient;
    for (gradient = m_gradients->begin(); gradient != m_gradients->end(); gradient++)
    {
      OutAscii(wxString::Format(wxS("/Sh%ld %d 0 R"), gradient->first, gradient->second->GetObjIndex()));
    }
    Out(">>");
  }

  if (!m_spotColours->empty())
  {
    Out("/ColorSpace <<");
    wxPdfSpotColourMap::iterator spotColour;
    for (spotColour = m_spotColours->begin(); spotColour != m_spotColours->end(); spotColour++)
    {
      OutAscii(wxString::Format(wxS("/CS%d %d 0 R"), spotColour->second->GetIndex(), spotColour->second->GetObjIndex()));
    }
    Out(">>");
  }

  if (!m_patterns->empty())
  {
    Out("/Pattern <<");
    wxPdfPatternMap::iterator pattern;
    for (pattern = m_patterns->begin(); pattern != m_patterns->end(); pattern++)
    {
      OutAscii(wxString::Format(wxS("/P%d %d 0 R"), pattern->second->GetIndex(), pattern->second->GetObjIndex()));
    }
    Out(">>");
  }

  if (!m_ocgs->empty())
  {
    Out("/Properties <<", false);
    wxPdfOcgMap::iterator ocgIter;
    for (ocgIter = m_ocgs->begin(); ocgIter != m_ocgs->end(); ++ocgIter)
    {
      wxPdfOcgType ocgType = ocgIter->second->GetType();
      if (ocgType == wxPDF_OCG_TYPE_LAYER || ocgType == wxPDF_OCG_TYPE_MEMBERSHIP)
      {
        wxPdfOcg* ocg = ocgIter->second;
        OutAscii(wxString::Format(wxS("/L%d %d 0 R "), ocg->GetIndex(), ocg->GetObjIndex()), false);
      }
    }
    Out(">>");
  }
}

void
wxPdfDocument::PutBookmarks()
{
  unsigned int nb = (unsigned int) m_outlines.GetCount();
  if (nb == 0)
  {
    return;
  }

  unsigned int i;
  int parent;
  wxArrayInt lru;
  lru.SetCount(m_maxOutlineLevel+1);
  int level = 0;
  for (i = 0; i < nb; i++)
  {
    wxPdfBookmark* bookmark = (wxPdfBookmark*) m_outlines[i];
    int currentLevel = bookmark->GetLevel();
    if (currentLevel > 0)
    {
      parent = lru[currentLevel-1];
      // Set parent and last pointers
      bookmark->SetParent(parent);
      wxPdfBookmark* parentBookmark = (wxPdfBookmark*) m_outlines[parent];
      parentBookmark->SetLast(i);
      if (currentLevel > level)
      {
        // Level increasing: set first pointer
        parentBookmark->SetFirst(i);
      }
    }
    else
    {
      bookmark->SetParent(nb);
    }
    if (currentLevel <= level && i > 0)
    {
      // Set prev and next pointers
      int prev = lru[currentLevel];
      wxPdfBookmark* prevBookmark = (wxPdfBookmark*) m_outlines[prev];
      prevBookmark->SetNext(i);
      bookmark->SetPrev(prev);
    }
    lru[currentLevel] = i;
    level = currentLevel;
  }

  // Outline items
  int n = m_n + 1;
  for (i = 0; i < nb; i++)
  {
    wxPdfBookmark* bookmark = (wxPdfBookmark*) m_outlines[i];
    NewObj();
    Out("<</Title ", false);
    OutTextstring(bookmark->GetText());
    OutAscii(wxString::Format(wxS("/Parent %d 0 R"), (n+bookmark->GetParent())));
    if (bookmark->GetPrev() >= 0)
    {
      OutAscii(wxString::Format(wxS("/Prev %d 0 R"), (n+bookmark->GetPrev())));
    }
    if (bookmark->GetNext() >= 0)
    {
      OutAscii(wxString::Format(wxS("/Next %d 0 R"), (n+bookmark->GetNext())));
    }
    if (bookmark->GetFirst() >= 0)
    {
      OutAscii(wxString::Format(wxS("/First %d 0 R"), (n+bookmark->GetFirst())));
    }
    if(bookmark->GetLast() >= 0)
    {
      OutAscii(wxString::Format(wxS("/Last %d 0 R"), (n+bookmark->GetLast())));
    }
    double y = bookmark->GetY();
    if (m_yAxisOriginTop)
    {
      y = m_h - y;
    }
    OutAscii(wxString::Format(wxS("/Dest [%d 0 R /XYZ 0 "), (m_firstPageId+2*(bookmark->GetPage()-1))) +
             wxPdfUtility::Double2String(y*m_k,2) + wxString(wxS(" null]")));
    Out("/Count 0>>");
    Out("endobj");
  }
  // Outline root
  NewObj();
  m_outlineRoot = m_n;
  OutAscii(wxString::Format(wxS("<</Type /Outlines /First %d 0 R"), n));
  OutAscii(wxString::Format(wxS("/Last %d 0 R>>"), (n+lru[0])));
  Out("endobj");
}

void
wxPdfDocument::PutFiles()
{
  int count = (int) (m_attachments->size());
  wxArrayString* attachment;
  wxString fileName;
  wxString attachName;
  wxString description;
  wxString nameTree;
  int j;
  for (j = 1; j <= count; ++j)
  {
    attachment = (*m_attachments)[j];
    fileName = (*attachment)[0];
    attachName = (*attachment)[1];
    description = (*attachment)[2];

    wxFileInputStream fileContent(fileName);
    if (fileContent.IsOk())
    {
      NewObj();
      nameTree += wxString::Format(wxS("(%04d) %d 0 R "), j, m_n);
//      $s .= $this->_textstring(sprintf('%03d',$i)).' '.$this->n.' 0 R ';

      Out("<<");
      Out("/Type /Filespec");
      Out("/F (", false);
      Out((const char*) attachName.mb_str(*wxConvFileName), false);
      Out(")");
#if wxUSE_UNICODE
      Out("/UF ", false);
      OutTextstring(attachName);
#endif
      Out("/EF <</F ", false);
      OutAscii(wxString::Format(wxS("%d 0 R>>"), m_n+1));
      if (!description.IsEmpty())
      {
        Out("/Desc ", false);
        OutTextstring(description);
      }
      Out(">>");
      Out("endobj");

      wxMemoryOutputStream mos;
      mos.Write(fileContent);
      size_t fileLen = CalculateStreamLength(mos.TellO());

      NewObj();
      Out("<<");
      Out("/Type /EmbeddedFile");
      OutAscii(wxString::Format(wxS("/Length %lu"), (unsigned long) fileLen));
      Out(">>");
      PutStream(mos);
      Out("endobj");
    }
  }
  NewObj();
  m_nAttachments = m_n;
  Out("<<");
  Out("/Names [", false);
  OutAscii(nameTree, false);
  Out("]");
  Out(">>");
  Out("endobj");
}

void
wxPdfDocument::PutEncryption()
{
  int revision = m_encryptor->GetRevision();
  Out("/Filter /Standard");
  switch (revision)
  {
    case 6:
      {
        Out("/V 5");
        Out("/R 6");
        Out("/CF <</StdCF <</CFM /AESV3 /Length 32 /AuthEvent /DocOpen>>>>");
        Out("/StrF /StdCF");
        Out("/StmF /StdCF");
        OutAscii(wxString::Format(wxS("/Length %d"), m_encryptor->GetKeyLength()));
      }
      break;
    case 5:
      {
        Out("/V 5");
        Out("/R 5");
        Out("/CF <</StdCF <</CFM /AESV3 /Length 32 /AuthEvent /DocOpen>>>>");
        Out("/StrF /StdCF");
        Out("/StmF /StdCF");
        OutAscii(wxString::Format(wxS("/Length %d"), m_encryptor->GetKeyLength()));
      }
      break;
    case 4:
      {
        Out("/V 4");
        Out("/R 4");
        Out("/CF <</StdCF <</CFM /AESV2 /Length 16 /AuthEvent /DocOpen>>>>");
        Out("/StrF /StdCF");
        Out("/StmF /StdCF");
        OutAscii(wxString::Format(wxS("/Length %d"), m_encryptor->GetKeyLength()));
      }
      break;
    case 3:
      {
        Out("/V 2");
        Out("/R 3");
        OutAscii(wxString::Format(wxS("/Length %d"), m_encryptor->GetKeyLength()));
      }
      break;
    case 2:
    default:
      {
        Out("/V 1");
        Out("/R 2");
      }
      break;
  }
  Out("/O ",false);
  OutHex(m_encryptor->GetOValue());
  Out("/U ",false);
  OutHex(m_encryptor->GetUValue());
  OutAscii(wxString::Format(wxS("/P %d"), m_encryptor->GetPValue()));
  if (revision > 4)
  {
    Out("/OE ", false);
    OutHex(m_encryptor->GetOEValue());
    Out("/UE ", false);
    OutHex(m_encryptor->GetUEValue());
    Out("/Perms ", false);
    OutHex(m_encryptor->GetPermsValue());
  }
}

void
wxPdfDocument::PutSpotColours()
{
  wxPdfSpotColourMap::iterator spotIter = (*m_spotColours).begin();
  for (spotIter = (*m_spotColours).begin(); spotIter != (*m_spotColours).end(); spotIter++)
  {
    wxPdfSpotColour* spotColour = spotIter->second;
    NewObj();
    wxString spotColourName = spotIter->first;
    spotColourName.Replace(wxS(" "),wxS("#20"));
    Out("[/Separation /", false);
    OutAscii(spotColourName);
    Out("/DeviceCMYK <<");
    Out("/Range [0 1 0 1 0 1 0 1] /C0 [0 0 0 0] ");
    OutAscii(wxS("/C1 [") +
             wxPdfUtility::Double2String(wxPdfUtility::ForceRange(spotColour->GetCyan(),    0., 100.)/100., 4) + wxS(" ") +
             wxPdfUtility::Double2String(wxPdfUtility::ForceRange(spotColour->GetMagenta(), 0., 100.)/100., 4) + wxS(" ") +
             wxPdfUtility::Double2String(wxPdfUtility::ForceRange(spotColour->GetYellow(),  0., 100.)/100., 4) + wxS(" ") +
             wxPdfUtility::Double2String(wxPdfUtility::ForceRange(spotColour->GetBlack(),   0., 100.)/100., 4) + wxS("] "));
    Out("/FunctionType 2 /Domain [0 1] /N 1>>]");
    Out("endobj");
    spotColour->SetObjIndex(m_n);
  }
}

void
wxPdfDocument::InitPatternIds()
{
  wxPdfPatternMap::iterator patternIter = m_patterns->begin();
  for (patternIter = m_patterns->begin(); patternIter != m_patterns->end(); patternIter++)
  {
    wxPdfPattern* pattern = patternIter->second;
    pattern->SetObjIndex(GetNewObjId());
  }
}

void
wxPdfDocument::PutPatterns()
{
  wxPdfPatternMap::iterator patternIter = m_patterns->begin();
  for (patternIter = m_patterns->begin(); patternIter != m_patterns->end(); patternIter++)
  {
    wxPdfPattern* pattern = patternIter->second;
    NewObj(pattern->GetObjIndex());
    Out("<<");
    Out("/Type /Pattern");
    Out("/PatternType 1");
    Out("/PaintType 1");
    Out("/TilingType 1");
    if (pattern->GetPatternStyle() == wxPDF_PATTERNSTYLE_IMAGE ||
        pattern->GetPatternStyle() == wxPDF_PATTERNSTYLE_TEMPLATE )
    {
      OutAscii(wxString(wxS("/BBox [0 0 ")) +
        wxPdfUtility::Double2String(pattern->GetWidth() * m_k, 4) + wxS(" ") +
        wxPdfUtility::Double2String(pattern->GetHeight() * m_k, 4) + wxS("]"));
      OutAscii(wxString(wxS("/XStep ")) +
        wxPdfUtility::Double2String(pattern->GetWidth() * m_k, 4));
      OutAscii(wxString(wxS("/YStep ")) +
        wxPdfUtility::Double2String(pattern->GetHeight() * m_k, 4));
      if (pattern->GetPatternStyle() == wxPDF_PATTERNSTYLE_IMAGE)
      {
        wxPdfImage* image = pattern->GetImage();
        OutAscii(wxString::Format(wxS("/Resources << /XObject << /I%d %d 0 R >> >>"), image->GetIndex(), image->GetObjIndex()));
        Out("/Matrix [ 1 0 0 1 0 0 ]");

        wxString sdata = wxString(wxS("q ")) +
          wxPdfUtility::Double2String(pattern->GetWidth() * m_k, 4) + wxS(" 0 0 ") +
          wxPdfUtility::Double2String(pattern->GetHeight() * m_k, 4) + wxS(" 0 0 cm ") +
          wxString::Format(wxS("/I%d Do Q"), image->GetIndex());
        wxMemoryOutputStream mos;
        mos.Write(sdata.ToAscii(), sdata.Length());
        OutAscii(wxString(wxS("/Length ")) + wxString::Format(wxS("%lu"), (unsigned long)CalculateStreamLength(mos.TellO())));
        Out(">>");
        PutStream(mos);
      }
      else
      {
        int templateId = pattern->GetTemplateId();
        wxPdfTemplate* tpl = (*m_templates)[templateId];
        OutAscii(wxString(wxS("/Resources << /XObject << ")) + m_templatePrefix + wxString::Format(wxS("%d %d 0 R >> >>"), tpl->GetIndex(), tpl->GetObjIndex()));
        Out("/Matrix [ 1 0 0 1 0 0 ]");
        double x, y, w, h;
        w = pattern->GetWidth();
        h = pattern->GetHeight();
        x = tpl->GetX();
        y = tpl->GetY();
        GetTemplateSize(templateId, w, h);

        double xScale = w / tpl->GetWidth();
        double yScale = h / tpl->GetHeight();
        double xTrans = (x - xScale * tpl->GetX()) * m_k;
        double yTrans = (y - yScale * tpl->GetY()) * m_k;
        wxString sdata = wxString(wxS("q ")) +
          wxPdfUtility::Double2String(xScale, 4) + wxString(wxS(" 0 0 ")) +
          wxPdfUtility::Double2String(yScale, 4) + wxString(wxS(" ")) +
          wxPdfUtility::Double2String(xTrans, 2) + wxString(wxS(" ")) +
          wxPdfUtility::Double2String(yTrans, 2) + wxString(wxS(" cm ")) +
          m_templatePrefix + wxString::Format(wxS("%d Do Q"), tpl->GetIndex());
        wxMemoryOutputStream mos;
        mos.Write(sdata.ToAscii(), sdata.Length());
        OutAscii(wxString(wxS("/Length ")) + wxString::Format(wxS("%lu"), (unsigned long)CalculateStreamLength(mos.TellO())));
        Out(">>");
        PutStream(mos);
      }
    }
    else
    {
      OutAscii(wxString(wxS("/BBox [0 0 10 10]")));
      OutAscii(wxString(wxS("/XStep 10")));
      OutAscii(wxString(wxS("/YStep 10")));
      OutAscii(wxString(wxS("/Resources << >>")));
      wxString patternData;
      double corrFactor = 1.0;
      switch (pattern->GetPatternStyle())
      {
        case wxPDF_PATTERNSTYLE_BDIAGONAL_HATCH:
          patternData = "0 0 m 10 10 l -1  9  m 1 11 l 9  -1  m 11 1 l";
          break;
        case wxPDF_PATTERNSTYLE_CROSSDIAG_HATCH:
          patternData = "0 0 m 10 10 l 0 10 m 10 0 l";
          break;
        case wxPDF_PATTERNSTYLE_FDIAGONAL_HATCH:
          patternData = "0 10 m 10 0 l -1 1 m 1 -1 l 9 11 m 11 9 l";
          break;
        case wxPDF_PATTERNSTYLE_CROSS_HATCH:
          patternData = "0 5 m 10 5 l 5 0 m 5 10 l";
          break;
        case wxPDF_PATTERNSTYLE_HORIZONTAL_HATCH:
          patternData = "0 5 m 10 5 l";
          break;
        case wxPDF_PATTERNSTYLE_VERTICAL_HATCH:
          patternData = "5 0 m 5 10 l";
          break;
        case wxPDF_PATTERNSTYLE_HERRINGBONE_HATCH:
          patternData = wxString(wxS("1.25 1.25 m 8.75 1.25 l 0 3.75 m 6.25 3.75 l 8.75 3.75 m 10 3.75 l")) +
                        wxS(" 0 6.25 m 3.75 6.25 l 6.25 6.25 m 10 6.25 l 0 8.75 m 1.25 8.75 l 3.75 8.75 m 10 8.75 l") +
                        wxS(" 1.25 0 m 1.25 3.75 l 1.25 6.25 m 1.25 10 l 3.75 0 m 3.75 1.25 l 3.75 3.75 m 3.75 10 l") +
                        wxS(" 6.25 1.25 m 6.25 8.75 l 8.75 0 m 8.75 6.25 l 8.75 8.75 m 8.75 10 l");
          corrFactor = 4;
          break;
        case wxPDF_PATTERNSTYLE_BASKETWEAVE_HATCH:
          patternData = wxString(wxS("0 1.25 m 10 1.25 l 0 6.25 m 10 6.25 l 3.75 0 m 3.75 10 l 8.75 0 m 8.75 10 l")) +
            wxS(" 0 3.75 m 3.75 3.75 l 8.75 3.75 m 10 3.75 l 3.75 8.75 m 8.75 8.75 l") +
            wxS(" 1.25 0 m 1.25 1.25 l 1.25 6.25 m 1.25 10 l 6.25 1.25 m 6.25 6.25 l");
          corrFactor = 4;
          break;
        case wxPDF_PATTERNSTYLE_BRICK_HATCH:
          patternData = "0 3 m 10 3 l 0 8 m 10 8 l 3 0 m 3 3 l 3 8 m 3 10 l 8 3 m 8 8 l";
          corrFactor = 2;
          break;

          // These pattern styles are not supported here, but still list them
          // to avoid -Wswitch (and similar) warnings.
        case wxPDF_PATTERNSTYLE_NONE:
        case wxPDF_PATTERNSTYLE_IMAGE:
        case wxPDF_PATTERNSTYLE_TEMPLATE:
          break;
      }
      OutAscii(wxString(wxS("/Matrix [")) +
        wxPdfUtility::Double2String((pattern->GetWidth() * m_k) / 10.0 * corrFactor, 4) + wxS(" 0 0 ") +
        wxPdfUtility::Double2String((pattern->GetHeight() * m_k) / 10.0 * corrFactor, 4) + wxS(" 0 0]"));
      wxString background;
      if (pattern->HasFillColour())
      { 
        background = wxPdfUtility::RGB2String(pattern->GetFillColour()) + wxS(" rg 0 0 10 10 re f ");
      }
      wxString sdata = wxString::Format(wxS("q ")) + background +
        wxPdfUtility::RGB2String(pattern->GetDrawColour()) + wxS(" RG 2 J 0.5 w ") +
        patternData + wxS(" S Q");
      wxMemoryOutputStream mos;
      mos.Write(sdata.ToAscii(), sdata.Length());
      OutAscii(wxString(wxS("/Length ")) + wxString::Format(wxS("%lu"), (unsigned long)CalculateStreamLength(mos.TellO())));
      Out(">>");
      PutStream(mos);
    }
    Out("endobj");
  }
}

void
wxPdfDocument::PutJavaScript()
{
  if (m_javascript.Length() > 0)
  {
    NewObj();
    m_nJS = m_n;
    Out("<<");
    Out("/Names [", false);
    OutAsciiTextstring(wxString(wxS("EmbeddedJS")), false);
    OutAscii(wxString::Format(wxS(" %d 0 R ]"), m_n+1));
    Out(">>");
    Out("endobj");
    NewObj();
    Out("<<");
    Out("/S /JavaScript");
    Out("/JS ", false);
    // TODO: Write Javascript object as stream
    OutTextstring(m_javascript);
    Out(">>");
    Out("endobj");
  }
}

#include "srgb2014icc.h"

void
wxPdfDocument::PutColourProfile()
{
  wxMemoryOutputStream mos((void*) sRGB2014_icc, sRGB2014_icc_size);
  size_t fileLen = CalculateStreamLength(sRGB2014_icc_size);

  NewObj();
  m_nColourProfile = m_n;
  Out("<<");
  OutAscii(wxString::Format(wxS("/Length %lu"), (unsigned long) fileLen));
  Out("/N 3");
  Out(">>");
  PutStream(mos);
  Out("endobj");
}

static wxXmlNode*
AddXmpDescription(const wxString& alias, const wxString& ns)
{
  wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, wxS("rdf:Description"));
  node->AddAttribute(wxS("rdf:about"), wxS(""));
  node->AddAttribute(wxString(wxS("xmlns:"))+alias, ns);
  return node;
}

static wxXmlNode*
AddXmpSimple(const wxString& tag, const wxString& value)
{
  wxXmlNode* tagNode = new wxXmlNode(wxXML_ELEMENT_NODE, tag);
  wxXmlNode* valNode = new wxXmlNode(wxXML_TEXT_NODE, wxS(""), value);
  tagNode->AddChild(valNode);
  return tagNode;
}

static wxXmlNode*
AddXmpSeq(const wxString& tag, const wxString& value)
{
  wxXmlNode* tagNode = new wxXmlNode(wxXML_ELEMENT_NODE, tag);
  wxXmlNode* seqNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxS("rdf:Seq"));
  wxXmlNode* liNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxS("rdf:li"));
  wxXmlNode* valNode = new wxXmlNode(wxXML_TEXT_NODE, wxS(""), value);
  liNode->AddChild(valNode);
  seqNode->AddChild(liNode);
  tagNode->AddChild(seqNode);
  return tagNode;
}

static wxXmlNode*
AddXmpAlt(const wxString& tag, const wxString& value)
{
  wxXmlNode* tagNode = new wxXmlNode(wxXML_ELEMENT_NODE, tag);
  wxXmlNode* altNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxS("rdf:Alt"));
  wxXmlNode* liNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxS("rdf:li"));
  wxXmlNode* valNode = new wxXmlNode(wxXML_TEXT_NODE, wxS(""), value);
  liNode->AddAttribute(wxS("xml:lang"), wxS("x-default"));
  liNode->AddChild(valNode);
  altNode->AddChild(liNode);
  tagNode->AddChild(altNode);
  return tagNode;
}

void
wxPdfDocument::PutMetaData()
{
  wxXmlDocument xmp;

  // RDF description for conformance with PDF/A-1b
  wxXmlNode* rootNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("rdf:RDF"));
  rootNode->AddAttribute(wxS("xmlns:rdf"), wxS("http://www.w3.org/1999/02/22-rdf-syntax-ns#"));

  wxXmlNode* pdfDesc = AddXmpDescription(wxS("pdf"), wxS("http://ns.adobe.com/pdf/1.3/"));
  pdfDesc->AddChild(AddXmpSimple(wxS("pdf:Producer"), wxString(wxPDF_PRODUCER)));
  if (m_keywords.Length() > 0)
  {
    pdfDesc->AddChild(AddXmpSimple(wxS("pdf:Keywords"), m_keywords));
  }
  rootNode->AddChild(pdfDesc);

  if (!m_creationDateSet)
  {
    SetCreationDate(wxDateTime::Now());
  }
  wxString creationDate = m_creationDate.FormatISOCombined() + wxString(wxS("Z"));

  wxXmlNode* xmpDesc = AddXmpDescription(wxS("xmp"), wxS("http://ns.adobe.com/xap/1.0/"));
  xmpDesc->AddChild(AddXmpSimple(wxS("xmp:CreateDate"), creationDate));
  if (m_creator.length() > 0)
  {
    xmpDesc->AddChild(AddXmpSimple(wxS("xmp:CreatorTool"), m_creator));
  }
  rootNode->AddChild(xmpDesc);

  if (m_author.length() > 0 || m_title.length() > 0 || m_subject.length() > 0)
  {
    wxXmlNode* dcDesc = AddXmpDescription(wxS("dc"), wxS("http://purl.org/dc/elements/1.1/"));
    if (m_author.length() > 0)
    {
      dcDesc->AddChild(AddXmpSeq(wxS("dc:creator"), m_author));
    }
    if (m_title.length() > 0)
    {
      dcDesc->AddChild(AddXmpAlt(wxS("dc:title"), m_title));
    }
    if (m_subject.length() > 0)
    {
      dcDesc->AddChild(AddXmpAlt(wxS("dc:description"), m_subject));
    }
    rootNode->AddChild(dcDesc);
  }

  wxXmlNode* aidDesc = AddXmpDescription(wxS("pdfaid"), wxS("http://www.aiim.org/pdfa/ns/id/"));
  aidDesc->AddChild(AddXmpSimple(wxS("pdfaid:part"), wxS("1")));
  aidDesc->AddChild(AddXmpSimple(wxS("pdfaid:conformance"), wxS("B")));
  rootNode->AddChild(aidDesc);

  xmp.SetRoot(rootNode);

  // Processing instruction prolog
  wxXmlNode* piNode1 = new wxXmlNode(wxXML_PI_NODE, wxS("xpacket"), wxS("begin=\"\" id=\"W5M0MpCehiHzreSzNTczkc9d\""));
  xmp.AppendToProlog(piNode1);

  // Processing instruction epilog
  wxXmlNode* piNode2 = new wxXmlNode(wxXML_PI_NODE, wxS("xpacket"), wxS("end=\"r\""));
  rootNode->SetNext(piNode2);

  wxMemoryOutputStream mos;
  xmp.Save(mos);
  size_t streamLen = CalculateStreamLength(mos.TellO());

  NewObj();
  m_nMetaData = m_n;
  Out("<<");
  Out("/Type /Metadata");
  Out("/Subtype /XML");
  OutAscii(wxString::Format(wxS("/Length %lu"), (unsigned long) streamLen));
  Out(">>");
  PutStream(mos);
  Out("endobj");
}

void
wxPdfDocument::PutResources()
{
  PutExtGStates();
  PutShaders();
  PutFonts();
  PutImages();
  InitPatternIds();
  PutTemplates();
  PutPatterns();
  PutImportedObjects();
  PutSpotColours();
  PutLayers();

  // Resource dictionary
  (*m_offsets)[2-1] = m_buffer->TellO();
  Out("2 0 obj");
  Out("<<");
  PutResourceDict();
  Out(">>");
  Out("endobj");

  PutBookmarks();
  PutJavaScript();
  PutFiles();

  // PDF/A-1 conformance
  if (m_isPdfA1)
  {
    PutColourProfile();
    PutMetaData();
  }

  if (m_encrypted)
  {
    NewObj();
    m_encObjId = m_n;
    Out("<<");
    PutEncryption();
    Out(">>");
    Out("endobj");
  }
}

wxString
wxPdfDocument::DoDecoration(double x, double y, const wxString& txt)
{
  // Decorate text
  int top  = m_currentFont->GetFont().GetBBoxTopPosition();
  int up   = m_currentFont->GetFont().GetUnderlinePosition();
  int ut   = m_currentFont->GetFont().GetUnderlineThickness();
  double w = GetStringWidth(txt) + m_ws * txt.Freq(wxS(' '));
  wxString decoration = wxS("");
  if (m_decoration & wxPDF_FONTSTYLE_UNDERLINE)
  {
    decoration = decoration + wxS(" ") +
      wxPdfUtility::Double2String(x * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String((y - up/1000.*m_fontSize) * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String(w * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String(ut/1000.*m_fontSizePt,2) + wxString(wxS(" re f"));
  }
  if (m_decoration & wxPDF_FONTSTYLE_OVERLINE)
  {
    up = (int) (top * 0.9);
    decoration = decoration + wxS(" ") +
      wxPdfUtility::Double2String(x * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String((y - up/1000.*m_fontSize) * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String(w * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String(ut/1000.*m_fontSizePt,2) + wxString(wxS(" re f"));
  }
  if (m_decoration & wxPDF_FONTSTYLE_STRIKEOUT)
  {
    up = (int) (top * 0.26);
    decoration = decoration + wxS(" ") +
      wxPdfUtility::Double2String(x * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String((y - up/1000.*m_fontSize) * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String(w * m_k,2) + wxString(wxS(" ")) +
      wxPdfUtility::Double2String(ut/1000.*m_fontSizePt,2) + wxString(wxS(" re f"));
  }
  return decoration;
}

void
wxPdfDocument::ShowGlyph(wxUint32 glyph)
{
  OutAscii(wxString(wxS("(")), false);

  wxString t = m_currentFont->ConvertGlyph(glyph);
  if (!t.IsEmpty())
  {
#if wxUSE_UNICODE
    wxMBConv* conv = m_currentFont->GetEncodingConv();
    size_t len = conv->FromWChar(NULL, 0, t.wc_str(), 1);
    char* mbstr = new char[len+3];
    len = conv->FromWChar(mbstr, len+3, t.wc_str(), 1);
#else
    size_t len = t.Length();;
    char* mbstr = new char[len+1];
    strcpy(mbstr,t.c_str());
#endif

    OutEscape(mbstr,len);
    delete [] mbstr;

    Out(") Tj");
  }
}

void
wxPdfDocument::ShowText(const wxString& txt)
{
  bool doSimple = !(m_kerning || m_wsApply);
  if (!doSimple)
  {
    wxArrayInt kerning;
    if (m_kerning)
    {
      kerning = m_currentFont->GetKerningWidthArray(txt);
    }
    if (m_wsApply)
    {
      size_t kPos = 0;
      size_t kLen = kerning.Count();
      size_t wsPos = 0;
      int wsAdjust = (int) (1000 * m_ws * m_k / GetFontSize());
      for (wxString::const_iterator it = txt.begin(); it != txt.end(); ++it)
      {
        if (*it == wxUniChar(' '))
        {
          for (; kPos < kLen && kerning[kPos] < (int) wsPos; kPos += 2);
          if (kPos < kLen)
          {
            kerning.Insert(kPos, wsPos);
            kerning.Insert(kPos+1, -wsAdjust);
          }
          else
          {
            kerning.Add(wsPos);
            kerning.Add(-wsAdjust);
          }
        }
        ++wsPos;
      }
    }

    if (!kerning.IsEmpty())
    {
      Out("[", false);
      size_t n = kerning.GetCount();
      size_t j, pos = 0, len;
      for (j = 0; j < n; j += 2)
      {
        len = kerning[j] - pos + 1;
        Out("(", false);
        TextEscape(txt.substr(pos, len), false);
        Out(") ", false);
        OutAscii(wxString::Format(wxS("%d "), kerning[j + 1]), false);
        pos = kerning[j] + 1;
      }
      Out("(", false);
      TextEscape(txt.substr(pos), false);
      Out(")] TJ ", false);
    }
    else
    {
      doSimple = true;
    }
  }
  if (doSimple)
  {
    OutAscii(wxString(wxS("(")), false);
    TextEscape(txt, false);
    Out(") Tj ", false);
  }
}

void
wxPdfDocument::TextEscape(const wxString& s, bool newline)
{
  if (m_currentFont != NULL)
  {
    wxString t = m_currentFont->ConvertCID2GID(s);
#if wxUSE_UNICODE
    size_t slen = t.length();
    wxMBConv* conv = m_currentFont->GetEncodingConv();
    size_t len = conv->FromWChar(NULL, 0, t.wc_str(), slen);
    char* mbstr = new char[len+3];
    len = conv->FromWChar(mbstr, len+3, t.wc_str(), slen);
    if (len == wxCONV_FAILED)
    {
      len = strlen(mbstr);
    }
#else
    size_t len = t.Length();;
    char* mbstr = new char[len+1];
    strcpy(mbstr,t.c_str());
#endif

    OutEscape(mbstr,len);
    if (newline)
    {
      Out("\n",false);
    }
    delete [] mbstr;
  }
  else
  {
    wxLogError(wxString(wxS("wxPdfDocument::TextEscape: ")) +
               wxString(_("No font selected.")));
  }
}

size_t
wxPdfDocument::CalculateStreamLength(size_t length)
{
  size_t realLength = length;
  if (m_encrypted)
  {
    realLength = m_encryptor->CalculateStreamLength(length);
  }
  return realLength;
}

size_t
wxPdfDocument::CalculateStreamOffset()
{
  size_t offset = 0;
  if (m_encrypted)
  {
    offset = m_encryptor->CalculateStreamOffset();
  }
  return offset;
}

void
wxPdfDocument::PutStream(wxMemoryOutputStream& s)
{
  Out("stream");
  if (s.GetLength() != 0)
  {
    if (m_encrypted)
    {
      wxMemoryInputStream instream(s);
      size_t len = instream.GetSize();
      size_t lenbuf = CalculateStreamLength(len);
      size_t ofs = CalculateStreamOffset();
      char* buffer = new char[lenbuf];
      instream.Read(&buffer[ofs],len);
      m_encryptor->Encrypt(m_n, 0, (unsigned char*) buffer, (unsigned int) len);
      Out(buffer, lenbuf);
      delete [] buffer;
    }
    else
    {
      wxMemoryInputStream tmp(s);
      if(m_state==2)
      {
        if (!m_inTemplate)
        {
          (*m_pages)[m_page]->Write(tmp);
          (*m_pages)[m_page]->Write("\n",1);
        }
        else
        {
          m_currentTemplate->m_buffer.Write(tmp);
          m_currentTemplate->m_buffer.Write("\n",1);
        }
      }
      else
      {
        m_buffer->Write(tmp);
        m_buffer->Write("\n",1);
      }
    }
  }
  Out("endstream");
}

void
wxPdfDocument::OutEscape(const char* s, size_t len)
{
  size_t j;
  for (j = 0; j < len; j++)
  {
    switch (s[j])
    {
      case '\b':
        Out("\\b",false);
        break;
      case '\f':
        Out("\\f",false);
        break;
      case '\n':
        Out("\\n",false);
        break;
      case '\r':
        Out("\\r",false);
        break;
      case '\t':
        Out("\\t",false);
        break;
      case '\\':
      case '(':
      case ')':
        Out("\\",false);
      default:
        Out(&s[j],1,false);
        break;
    }
  }
}

void
wxPdfDocument::OutEscape(const std::string& s)
{
  OutEscape(s.c_str(), s.length());
}

void
wxPdfDocument::OutHex(const char* s, size_t len)
{
  static char hexDigits[17] = "0123456789ABCDEF";
  size_t j;
  char hexDigit;

  Out("<", false);
  for (j = 0; j < len; ++j)
  {
    hexDigit = hexDigits[(s[j] >> 4) & 0x0f];
    Out(&hexDigit, 1, false);
    hexDigit = hexDigits[s[j] & 0x0f];
    Out(&hexDigit, 1, false);
  }
  Out(">");
}

void
wxPdfDocument::OutHex(const std::string& s)
{
  OutHex(s.c_str(), s.length());
}

void
wxPdfDocument::OutTextstring(const wxString& s, bool newline)
{
  // Format a text string
  size_t ofs = CalculateStreamOffset();
#if wxUSE_UNICODE
  size_t slen = s.length();
  wxMBConvUTF16BE conv;
  size_t len = conv.FromWChar(NULL, 0, s.wc_str(), slen);
  size_t lenbuf = CalculateStreamLength(len+2);
  char* mbstr = new char[lenbuf+3];
  mbstr[ofs+0] = '\xfe';
  mbstr[ofs+1] = '\xff';
  len = 2 + conv.FromWChar(&mbstr[ofs+2], len+3, s.wc_str(), slen);
#else
  size_t len = s.Length();;
  size_t lenbuf = CalculateStreamLength(len);
  char* mbstr = new char[lenbuf+1];
  strcpy(&mbstr[ofs], s.c_str());
#endif

  if (m_encrypted)
  {
    m_encryptor->Encrypt(m_n, 0, (unsigned char*) mbstr, (unsigned int) len);
  }
  Out("(",false);
  OutEscape(mbstr,lenbuf);
  Out(")",newline);
  delete [] mbstr;
}

void
wxPdfDocument::OutRawTextstring(const wxString& s, bool newline)
{
  // Format a text string
  size_t ofs = CalculateStreamOffset();
  size_t len = s.Length();;
  size_t lenbuf = CalculateStreamLength(len);
  char* mbstr = new char[lenbuf+1];
#if wxUSE_UNICODE
  size_t j;
  wxString::const_iterator sChar = s.begin();
  for (j = 0; j < len; j++)
  {
    mbstr[ofs+j] = (char) ((unsigned int) (*sChar) & 0xff);
    ++sChar;
  }
  mbstr[ofs+len] = 0;
#else
  strcpy(&mbstr[ofs],s.c_str());
#endif

  if (m_encrypted)
  {
    m_encryptor->Encrypt(m_n, 0, (unsigned char*) mbstr, (unsigned int) len);
  }
  Out("(",false);
  OutEscape(mbstr,lenbuf);
  Out(")",newline);
  delete [] mbstr;
}

void
wxPdfDocument::OutHexTextstring(const wxString& s, bool newline)
{
  static char hexDigits[17] = "0123456789ABCDEF";
  // Format a text string
  size_t j;
  size_t ofs = CalculateStreamOffset();
  size_t len = s.Length();;
  size_t lenbuf = CalculateStreamLength(len);
  char* mbstr = new char[lenbuf+1];
#if wxUSE_UNICODE
  wxString::const_iterator sChar = s.begin();
  for (j = 0; j < len; j++)
  {
    mbstr[ofs+j] = (char) ((unsigned int) (*sChar) & 0xff);
    ++sChar;
  }
  mbstr[ofs+len] = 0;
#else
  strcpy(&mbstr[ofs],s.c_str());
#endif

  if (m_encrypted)
  {
    m_encryptor->Encrypt(m_n, 0, (unsigned char*) mbstr, (unsigned int) len);
  }

  char hexDigit;
  Out("<",false);
  for (j = 0; j < lenbuf; ++j)
  {
    hexDigit = hexDigits[(mbstr[j] >> 4) & 0x0f];
    Out(&hexDigit, 1, false);
    hexDigit = hexDigits[mbstr[j] & 0x0f];
    Out(&hexDigit, 1, false);
  }
  Out(">",newline);
  delete [] mbstr;
}

void
wxPdfDocument::OutAsciiTextstring(const wxString& s, bool newline)
{
  // Format an ASCII text string
  size_t ofs = CalculateStreamOffset();
  size_t len = s.Length();;
  size_t lenbuf = CalculateStreamLength(len);
  char* mbstr = new char[lenbuf+1];
  strcpy(&mbstr[ofs],s.ToAscii());

  if (m_encrypted)
  {
    m_encryptor->Encrypt(m_n, 0, (unsigned char*) mbstr, (unsigned int) len);
  }
  Out("(",false);
  OutEscape(mbstr,lenbuf);
  Out(")",newline);
  delete [] mbstr;
}

void
wxPdfDocument::OutAscii(const wxString& s, bool newline)
{
  // Add a line of ASCII text to the document
  Out((const char*) s.ToAscii(),newline);
}

void
wxPdfDocument::Out(const char* s, bool newline)
{
  size_t len = strlen(s);
  Out(s,len,newline);
}

void
wxPdfDocument::Out(const char* s, size_t len, bool newline)
{
  if(m_state==2)
  {
    if (!m_inTemplate)
    {
      (*m_pages)[m_page]->Write(s,len);
      if (newline)
      {
        (*m_pages)[m_page]->Write("\n",1);
      }
    }
    else
    {
      m_currentTemplate->m_buffer.Write(s,len);
      if (newline)
      {
        m_currentTemplate->m_buffer.Write("\n",1);
      }
    }
  }
  else
  {
    m_buffer->Write(s,len);
    if (newline)
    {
      m_buffer->Write("\n",1);
    }
  }
}

void
wxPdfDocument::OutPoint(double x, double y)
{
  OutAscii(wxPdfUtility::Double2String(x * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(y * m_k,2) + wxString(wxS(" m")));
  m_x = x;
  m_y = y;
}

void
wxPdfDocument::OutPointRelative(double dx, double dy)
{
  m_x += dx;
  m_y += dy;
  OutAscii(wxPdfUtility::Double2String(m_x * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(m_y * m_k,2) + wxString(wxS(" m")));
}

void
wxPdfDocument::OutLine(double x, double y)
{
  // Draws a line from last draw point
  OutAscii(wxPdfUtility::Double2String(x * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(y * m_k,2) + wxString(wxS(" l")));
  m_x = x;
  m_y = y;
}

void
wxPdfDocument::OutLineRelative(double dx, double dy)
{
  m_x += dx;
  m_y += dy;
  // Draws a line from last draw point
  OutAscii(wxPdfUtility::Double2String(m_x * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(m_y * m_k,2) + wxString(wxS(" l")));
}

void
wxPdfDocument::OutCurve(double x1, double y1, double x2, double y2, double x3, double y3)
{
  // Draws a Bezier curve from last draw point
  OutAscii(wxPdfUtility::Double2String(x1 * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(y1 * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(x2 * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(y2 * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(x3 * m_k,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(y3 * m_k,2) + wxString(wxS(" c")));
  m_x = x3;
  m_y = y3;
}

void
wxPdfDocument::OutImage(wxPdfImage* currentImage,
                        double x, double y, double w, double h, const wxPdfLink& link)
{
  // Automatic width and height calculation if needed
  if (w <= 0 && h <= 0)
  {
    // Put image at 72 dpi, apply scale factor
    if (currentImage->IsFormObject())
    {
      w = currentImage->GetWidth() / (20 * m_imgscale * m_k);
      h = currentImage->GetHeight() / (20 * m_imgscale * m_k);
    }
    else
    {
      w = currentImage->GetWidth() / (m_imgscale * m_k);
      h = currentImage->GetHeight() / (m_imgscale * m_k);
    }
  }
  if (w <= 0)
  {
    w = (h * currentImage->GetWidth()) / currentImage->GetHeight();
  }
  if (h <= 0)
  {
    h = (w * currentImage->GetHeight()) / currentImage->GetWidth();
  }

  double sw, sh, sx, sy;
  if (currentImage->IsFormObject())
  {
    sw = w * m_k / currentImage->GetWidth();
    sh = -h * m_k / currentImage->GetHeight();
    sx = x * m_k - sw * currentImage->GetX();
    sy = y * m_k + sh * currentImage->GetY();
  }
  else
  {
    sw = w * m_k;
    sh = h * m_k;
    sx = x * m_k;
    sy = (y+h)*m_k;
  }
  if (m_yAxisOriginTop)
  {
    sh = -sh;
  }
  OutAscii(wxString(wxS("q ")) +
           wxPdfUtility::Double2String(sw,2) + wxString(wxS(" 0 0 ")) +
           wxPdfUtility::Double2String(sh,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(sx,2) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String(sy,2) +
           wxString::Format(wxS(" cm /I%d Do Q"),currentImage->GetIndex()));

  if (link.IsValid())
  {
    Link(x,y,w,h,link);
  }

  // set right-bottom corner coordinates
  m_img_rb_x = x + w;
  m_img_rb_y = y + h;

  //
  if (m_inTemplate)
  {
    (*(m_currentTemplate->m_images))[currentImage->GetName()] = currentImage;
  }
}

void
wxPdfDocument::Transform(double tm[6])
{
  OutAscii(wxPdfUtility::Double2String( tm[0],3) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String( tm[1],3) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String( tm[2],3) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String( tm[3],3) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String( tm[4],3) + wxString(wxS(" ")) +
           wxPdfUtility::Double2String( tm[5],3) + wxString(wxS(" cm")));
}

void
wxPdfDocument::SetFillGradient(double x, double y, double w, double h, int gradient)
{
  if (gradient > 0 && (size_t) gradient <= (*m_gradients).size())
  {
    ClippingRect(x, y, w, h, false);
    //set up transformation matrix for gradient
    double tm[6];
    tm[0] = w * m_k;
    tm[1] = 0;
    tm[2] = 0;
    if (m_yAxisOriginTop)
    {
      tm[3] = -h * m_k;
    }
    else
    {
      tm[3] = h * m_k;
    }
    tm[4] = x * m_k;
    tm[5] = (y+h) * m_k;
    Transform(tm);
    // paint the gradient
    OutAscii(wxString::Format(wxS("/Sh%d sh"), gradient));
    // restore previous Graphic State
    UnsetClipping();
  }
  else
  {
    wxLogError(wxString(wxS("wxPdfDocument::SetFillGradient: ")) +
               wxString(_("Gradient Id out of range.")));
  }
}

wxString
wxPdfDocument::MakeFontKey(const wxString& fontName, const wxString& fontEncoding)
{
  wxString fontKey;
  fontKey.reserve(fontName.Length() + fontEncoding.length() + 2);

  fontKey.Append(fontName.Lower());
  fontKey.Append(wxS('['));
  fontKey.Append(fontEncoding.Lower());
  fontKey.Append(wxS(']'));

  return fontKey;
}
