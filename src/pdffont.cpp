///////////////////////////////////////////////////////////////////////////////
// Name:        pdffont.cpp
// Purpose:     Implementation of wxPdfFont classes
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-08-04
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffont.cpp Implementation of the wxPdfFont class

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/tokenzr.h>
#include <wx/zstream.h>

#include "wx/pdfdoc.h"
#include "wx/pdffont.h"
#include "wx/pdftruetype.h"

static int
CompareInts(int n1, int n2)
{
    return n1 - n2;
}

static wxString
GetNodeContent(const wxXmlNode *node)
{
  const wxXmlNode *n = node;
  if (n == NULL) return wxEmptyString;
   n = n->GetChildren();

  while (n)
  {
    if (n->GetType() == wxXML_TEXT_NODE ||
        n->GetType() == wxXML_CDATA_SECTION_NODE)
      return n->GetContent();
    n = n->GetNext();
  }

  return wxEmptyString;
}

static bool
GetFontDescription(const wxXmlNode *node, wxPdfFontDescription& fontDescription)
{
  bool bAscent             = false,
       bDescent            = false,
       bCapheight          = false,
       bFlags              = false,
       bFontbbox           = false,
       bItalicangle        = false,
       bStemv              = false,
       bMissingwidth       = false,
       bXHeight            = false,
       bUnderlinePosition  = false,
       bUnderlineThickness = false;
  wxString value;
  long number;
  wxXmlNode* child = node->GetChildren();
  while (child)
  {
    // parse the children
    if (child->GetName() == wxT("ascent"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bAscent = true;
        fontDescription.SetAscent(number);
      }
    }
    else if (child->GetName() == wxT("descent"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bDescent = true;
        fontDescription.SetDescent(number);
      }
    }
    else if (child->GetName() == wxT("cap-height"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bCapheight = true;
        fontDescription.SetCapHeight(number);
      }
    }
    else if (child->GetName() == wxT("flags"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bFlags = true;
        fontDescription.SetFlags(number);
      }
    }
    else if (child->GetName() == wxT("font-bbox"))
    {
      value = GetNodeContent(child);
      if (value.Length() > 0 && value[0] == _T('[') && value.Last() == _T(']'))
      {
        bFontbbox = true;
        fontDescription.SetFontBBox(value);
      }
    }
    else if (child->GetName() == wxT("italic-angle"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bItalicangle = true;
        fontDescription.SetItalicAngle(number);
      }
    }
    else if (child->GetName() == wxT("stem-v"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bStemv = true;
        fontDescription.SetStemV(number);
      }
    }
    else if (child->GetName() == wxT("missing-width"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bMissingwidth = true;
        fontDescription.SetMissingWidth(number);
      }
    }
    else if (child->GetName() == wxT("x-height"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bXHeight = true;
        fontDescription.SetXHeight(number);
      }
    }
    else if (child->GetName() == wxT("underline-position"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bUnderlinePosition = true;
        fontDescription.SetUnderlinePosition(number);
      }
    }
    else if (child->GetName() == wxT("underline-thickness"))
    {
      value = GetNodeContent(child);
      if (value.ToLong(&number))
      {
        bUnderlineThickness = true;
        fontDescription.SetUnderlineThickness(number);
      }
    }
    child = child->GetNext();
  }
  return (bAscent && bDescent && bCapheight && bFlags && bFontbbox &&
          bItalicangle && bStemv && bMissingwidth && bXHeight &&
          bUnderlinePosition && bUnderlineThickness);
}

wxPdfFontDescription::wxPdfFontDescription()
  : m_ascent(0), m_descent(0), m_capHeight(0), m_flags(0),
    m_fontBBox(wxEmptyString), m_italicAngle(0), m_stemV(0),
    m_missingWidth(0), m_xHeight(0),
    m_underlinePosition(-100), m_underlineThickness(50)
{
}

wxPdfFontDescription::wxPdfFontDescription (int ascent, int descent, int capHeight, int flags,
                                            const wxString& fontBBox, int italicAngle, int stemV,
                                            int missingWidth, int xHeight,
                                            int underlinePosition, int underlineThickness)
  : m_ascent(ascent), m_descent(descent), m_capHeight(capHeight), m_flags(flags),
    m_fontBBox(fontBBox), m_italicAngle(italicAngle), m_stemV(stemV),
    m_missingWidth(missingWidth), m_xHeight(xHeight),
    m_underlinePosition(underlinePosition), m_underlineThickness(underlineThickness)
{
}

#if 0
wxPdfFontDescription::wxPdfFontDescription(const wxPdfFontDescription& desc)
{
  m_ascent             = desc.m_ascent;
  m_descent            = desc.m_descent;
  m_capHeight          = desc.m_capHeight;
  m_flags              = desc.m_flags;
  m_fontBBox           = desc.m_fontBBox;
  m_italicAngle        = desc.m_italicAngle;
  m_stemV              = desc.m_stemV;
  m_missingWidth       = desc.m_missingWidth;
  m_xHeight            = desc.m_xHeight;
  m_underlinePosition  = desc.m_underlinePosition;
  m_underlineThickness = desc.m_underlineThickness;
}
#endif

wxPdfFontDescription::~wxPdfFontDescription()
{
}

wxMBConv* wxPdfFont::ms_winEncoding = NULL;

wxMBConv*
wxPdfFont::GetWinEncodingConv()
{
  if (ms_winEncoding == NULL)
  {
    static wxCSConv winEncoding(wxFONTENCODING_CP1252);
    ms_winEncoding = &winEncoding;
  }
  return ms_winEncoding;
}

// ----------------------------------------------------------------------------
// wxPdfFont: class 
// ----------------------------------------------------------------------------

wxPdfFont::wxPdfFont(int index, const wxString& name, short* cwArray,
                     const wxPdfFontDescription& desc)
{
  m_index = index;
  m_name  = name;
  m_type  = _T("core");
  m_desc  = desc;

  if (cwArray != NULL)
  {
    m_cw = new wxPdfCharWidthMap();
    int j;
    for (j = 0; j <256; j++)
    {
      (*m_cw)[j] = cwArray[j];
    }
  }
  else
  {
    m_cw = NULL;
  }
  m_gn = NULL;

  m_enc   = wxEmptyString;
  m_diffs = wxEmptyString;
  m_file  = wxEmptyString;
  m_ctg   = wxEmptyString;
  m_size1 = -1;
  m_size2 = -1;

  m_usedChars = new wxPdfSortedArrayInt(CompareInts);
  m_subset = false;
  m_subsetSupport = false;
}

wxPdfFont::~wxPdfFont()
{
  if (m_gn != NULL)
  {
    delete m_gn;
  }
  delete m_cw;
  delete m_usedChars;
}

wxString
wxPdfFont::GetName()
{
  wxString name = m_name;
  if (m_subset && SupportsSubset())
  {
    name.Prepend(CreateSubsetPrefix());
  }
  return name;
}

wxString
wxPdfFont::GetWidthsAsString()
{
  wxString s = wxString(_T("["));
  int i;
  for (i = 32; i <= 255; i++)
  {
    s += wxString::Format(_T("%d "), (*m_cw)[i]);
  }
  s += wxString(_T("]"));
  return s;
}

#if wxUSE_UNICODE
wxMBConv*
wxPdfFont::GetEncodingConv()
{
  wxMBConv* conv;
  if (m_type == _T("core") && (m_name == _T("") || m_name == _T("")))
  {
    conv = &wxConvISO8859_1;
  }
  else
  {
    conv = GetWinEncodingConv();
  }
  return conv;
}
#endif

double
wxPdfFont::GetStringWidth(const wxString& s)
{
  double w = 0;
  // Get width of a string in the current font
#if wxUSE_UNICODE
  wxCharBuffer wcb(s.mb_str(*GetEncodingConv()));
  const char* str = (const char*) wcb;
#else
  const char* str = s.c_str();
#endif

  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    w += (*m_cw)[(unsigned char) str[i]];
  }
  return w / 1000;
}

wxString
wxPdfFont::ConvertCID2GID(const wxString& s)
{
  return s;
}

void
wxPdfFont::UpdateUsedChars(const wxString& s)
{
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    int ch = (int) s[i];
    if (m_usedChars->Index(ch) == wxNOT_FOUND)
    {
      m_usedChars->Add(ch);
    }
  }
}

int
wxPdfFont::CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset)
{
  // Font subsetting is not supported for base fonts
  // Just copy the font file stream to the font subset stream
  fontSubset->Write(*fontFile);
  return GetSize1();
}

int
wxPdfFont::GetBBoxTopPosition()
{
  long top = 1000;
  wxString bBox = m_desc.GetFontBBox();
  wxStringTokenizer tkz(bBox, _T(" []"), wxTOKEN_STRTOK);
  if (tkz.CountTokens() >= 4)
  {
    tkz.GetNextToken();
    tkz.GetNextToken();
    tkz.GetNextToken();
    wxString topToken = tkz.GetNextToken();
    topToken.ToLong(&top);
  }
  return top;
}

wxString
wxPdfFont::CreateSubsetPrefix() const
{
  wxString prefix = _T("WXP");
  int k;
  int code = m_index;
  for (k = 0; k < 3; k++)
  {
    prefix += wxChar(_T('A' + (code % 26)));
    code /= 26;
  }
  prefix += _T("+");
  return prefix;
}

void
wxPdfFont::SetSubset(bool subset)
{
  if (SupportsSubset())
  {
    m_subset = subset;
  }
}

// ----------------------------------------------------------------------------
// wxPdfFontTrueType: class 
// ----------------------------------------------------------------------------

wxPdfFontTrueType::wxPdfFontTrueType(int index)
  : wxPdfFont(index)
{
  m_type  = _T("TrueType");
  m_conv = NULL;
}

wxPdfFontTrueType::~wxPdfFontTrueType()
{
  if (m_conv != NULL)
  {
    delete m_conv;
  }
}

bool
wxPdfFontTrueType::LoadFontMetrics(wxXmlNode* root)
{
  bool bName  = false,
       bDesc  = false,
       bFile  = true,
       bSize  = true,
       bWidth = false;
  wxString value;
  long number;
  wxXmlNode *child = root->GetChildren();
  while (child)
  {
    // parse the children
    if (child->GetName() == wxT("font-name"))
    {
      m_name = GetNodeContent(child);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxT("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxT("diff"))
    {
      m_diffs = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("file"))
    {
      value = child->GetPropVal(_T("name"), _T(""));
      if (value.Length() > 0)
      {
        m_file = value;
        value = child->GetPropVal(_T("originalsize"), _T(""));
        if (value.Length() > 0 && value.ToLong(&number))
        {
          bFile = true;
          m_size1 = number;
        }
        else
        {
          bFile = false;
          m_file = _T("");
        }
      }
    }
    else if (child->GetName() == wxT("widths"))
    {
      wxString subsetting = child->GetPropVal(_T("subsetting"), _T("disabled"));
      m_subsetSupport = (subsetting == _T("enabled"));
      bWidth = true;
      m_cw = new wxPdfCharWidthMap();
      if (m_subsetSupport)
      {
        m_gn = new wxPdfCharWidthMap();
      }
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strGn, strWidth;
        long charId, charWidth, glyph;
        if (charNode->GetName() == wxT("char"))
        {
          strId = charNode->GetPropVal(_T("id"), _T(""));
          if (m_subsetSupport)
          {
            strGn = charNode->GetPropVal(_T("gn"), _T(""));
          }
          strWidth = charNode->GetPropVal(_T("width"), _T(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
            if (m_subsetSupport)
            {
              if (strGn.Length() > 0 && strGn.ToLong(&glyph))
              {
                (*m_gn)[charId] = glyph;
              }
              else
              {
                (*m_gn)[charId] = 0;
              }
            }
          }
        }
        charNode = charNode->GetNext();
      }
    }
    child = child->GetNext();
  }

#if wxUSE_UNICODE
  // Unicode build needs charset conversion
  if (m_conv == NULL)
  {
    if (m_enc.Length() > 0)
    {
      m_conv = new wxCSConv(m_enc);
    }
    else
    {
      m_conv = new wxCSConv(wxFONTENCODING_ISO8859_1);
    }
  }
#endif

  return (bName && bDesc && bFile && bSize && bWidth);
}

double
wxPdfFontTrueType::GetStringWidth(const wxString& s)
{
  // Get width of a string in the current font
  double w = 0;
#if wxUSE_UNICODE
  wxCharBuffer wcb(s.mb_str(*m_conv));
  const char* str = (const char*) wcb;
#else
  const char* str = s.c_str();
#endif

  wxPdfCharWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    wxPdfCharWidthMap::iterator charIter = (*m_cw).find((unsigned char) str[i]);
    if (charIter != (*m_cw).end())
    {
      w += charIter->second;
    }
    else
    {
      w += m_desc.GetMissingWidth();
    }
  }
  return w / 1000;
}

void
wxPdfFontTrueType::UpdateUsedChars(const wxString& s)
{
#if wxUSE_UNICODE
  wxMBConv* conv = GetEncodingConv();
  int len = conv->WC2MB(NULL, s, 0);
  char* mbstr = new char[len+3];
  len = conv->WC2MB(mbstr, s, len+3);
#else
  int len = s.Length();;
  char* mbstr = new char[len+1];
  strcpy(mbstr,s.c_str());
#endif
  int i;
  for (i = 0; i < len; i++)
  {
    int ch = (int) mbstr[i];
    if (m_usedChars->Index(ch) == wxNOT_FOUND)
    {
      m_usedChars->Add(ch);
    }
  }
  delete mbstr;
}

int
wxPdfFontTrueType::CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset)
{
  int fontSize1 = GetSize1();
  // 1) Translate used characters to used glyphs
  size_t usedCharCount = m_usedChars->GetCount();
  wxPdfSortedArrayInt usedGlyphs(CompareInts);
  size_t k;
  for (k = 0; k < usedCharCount; k++)
  {
    int charIndex = (*m_usedChars)[k];
    int glyph = (*m_gn)[charIndex];
    usedGlyphs.Add(glyph);
  }
  
  // 2) Uncompress fontFile
  wxZlibInputStream zin2(*fontFile);
  wxMemoryOutputStream zout2;
  zout2.Write(zin2);
  zout2.Close();
  wxMemoryInputStream uncompressedFont(zout2);

  // 3) Assemble subset
  wxPdfTrueTypeSubset subset(GetFontFile());
  wxMemoryOutputStream* subsetStream = subset.CreateSubset(&uncompressedFont, &usedGlyphs, true);
  wxZlibOutputStream q(*fontSubset);
  wxMemoryInputStream tmp(*subsetStream);
  fontSize1 = tmp.GetSize();
  q.Write(tmp);
  q.Close();
  delete subsetStream;

  return fontSize1;
}

#if wxUSE_UNICODE

// ----------------------------------------------------------------------------
// wxPdfFontTrueTypeUnicode: class 
// ----------------------------------------------------------------------------

wxPdfFontTrueTypeUnicode::wxPdfFontTrueTypeUnicode(int index)
  : wxPdfFont(index)
{
  m_type  = _T("TrueTypeUnicode");
  m_conv = NULL;
}

wxPdfFontTrueTypeUnicode::~wxPdfFontTrueTypeUnicode()
{
  // delete m_cw;
  if (m_conv != NULL)
  {
    delete m_conv;
  }
}

bool
wxPdfFontTrueTypeUnicode::LoadFontMetrics(wxXmlNode* root)
{
  bool bName  = false,
       bDesc  = false,
       bFile  = true,
       bSize  = true,
       bWidth = false;
  wxString value;
  long number;
  wxXmlNode *child = root->GetChildren();
  while (child)
  {
    // parse the children
    if (child->GetName() == wxT("font-name"))
    {
      m_name = GetNodeContent(child);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxT("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxT("diff"))
    {
      m_diffs = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("file"))
    {
      value = child->GetPropVal(_T("ctg"), _T(""));
      if (value.Length() > 0)
      {
        bFile = true;
        m_ctg = value;
        value = child->GetPropVal(_T("name"), _T(""));
        if (value.Length() > 0)
        {
          m_file = value;
          value = child->GetPropVal(_T("originalsize"), _T(""));
          if (value.Length() > 0 && value.ToLong(&number))
          {
            bFile = true;
            m_size1 = number;
          }
          else
          {
            bFile = false;
            m_file = _T("");
          }
        }
      }
      else
      {
        bFile = false;
        m_file = _T("");
        m_ctg = _T("");
      }
    }
    else if (child->GetName() == wxT("widths"))
    {
      bWidth = true;
      m_cw = new wxPdfCharWidthMap();
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strWidth;
        long charId, charWidth;
        if (charNode->GetName() == wxT("char"))
        {
          strId = charNode->GetPropVal(_T("id"), _T(""));
          strWidth = charNode->GetPropVal(_T("width"), _T(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
          }
        }
        charNode = charNode->GetNext();
      }
    }
    child = child->GetNext();
  }
  if (m_conv == NULL)
  {
    m_conv = new wxMBConvUTF16BE();
  }
  return (bName && bDesc && bFile && bSize && bWidth);
}

wxString
wxPdfFontTrueTypeUnicode::GetWidthsAsString()
{
  wxString s = wxString(_T("["));
  wxPdfCharWidthMap::iterator charIter;
  for (charIter = (*m_cw).begin(); charIter != (*m_cw).end(); charIter++)
  {
    if (!m_subset || (m_subset && SupportsSubset() && (m_usedChars->Index(charIter->first) != wxNOT_FOUND)))
    {
      // define a specific width for each individual CID
      s += wxString::Format(_T("%d [%d] "), charIter->first, charIter->second);
    }
  }
  s += wxString(_T("]"));
  return s;
}

double
wxPdfFontTrueTypeUnicode::GetStringWidth(const wxString& s)
{
  // Get width of a string in the current font
  double w = 0;

  wxPdfCharWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    charIter = (*m_cw).find(s[i]);
    if (charIter != (*m_cw).end())
    {
      w += charIter->second;
    }
    else
    {
      w += m_desc.GetMissingWidth();
    }
  }
  return w / 1000;
}

int
wxPdfFontTrueTypeUnicode::CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset)
{
  int fontSize1 = GetSize1();
  // 1) Read and uncompress CTG file
  wxString strCtgFileName = GetCtgFile();
  wxFileName ctgFileName(strCtgFileName);
  ctgFileName.MakeAbsolute(GetFilePath());
  wxFileSystem fs;
  wxFSFile* ctgFile = fs.OpenFile(ctgFileName.GetFullPath());
  if (ctgFile)
  {
    wxInputStream* ctgStream = ctgFile->GetStream();
    // check file extension
    int ctgLen = 0;
    unsigned char* cc2gn = NULL;
    bool compressed = strCtgFileName.Right(2) == _T(".z");
    if (compressed)
    {
      wxZlibInputStream zin(*ctgStream);
      wxMemoryOutputStream zout;
      zout.Write(zin);
      zout.Close();
      wxMemoryInputStream cid2gidStream(zout);
      ctgLen = cid2gidStream.GetSize();
      cc2gn = new unsigned char[ctgLen];
      cid2gidStream.Read(cc2gn, ctgLen);
    }
    else
    {
      ctgLen = ctgStream->GetSize();
      cc2gn = new unsigned char[ctgLen];
      ctgStream->Read(cc2gn, ctgLen);
    }
    delete ctgFile;
    // 2) Translate used characters to used glyphs
    if (cc2gn != NULL)
    {
      size_t usedCharCount = m_usedChars->GetCount();
      wxPdfSortedArrayInt usedGlyphs(CompareInts);
      size_t k;
      for (k = 0; k < usedCharCount; k++)
      {
        int charIndex = 2 * (*m_usedChars)[k];
        int glyph = (cc2gn[charIndex] << 8) + cc2gn[charIndex+1];
        usedGlyphs.Add(glyph);
      }
      // 3) Uncompress fontFile
      wxZlibInputStream zin2(*fontFile);
      wxMemoryOutputStream zout2;
      zout2.Write(zin2);
      zout2.Close();
      wxMemoryInputStream uncompressedFont(zout2);
      // 4) Assemble subset

      wxPdfTrueTypeSubset subset(GetFontFile());
      wxMemoryOutputStream* subsetStream = subset.CreateSubset(&uncompressedFont, &usedGlyphs, false);
      wxZlibOutputStream q(*fontSubset);
      wxMemoryInputStream tmp(*subsetStream);
      fontSize1 = tmp.GetSize();
      q.Write(tmp);
      q.Close();
      delete subsetStream;
      delete [] cc2gn;
    }
    else
    {
      // Error reading CTG file
    }
  }
  else
  {
    wxLogError (wxString(_T("wxPdfTrueTypeUnicode::CreateSubset: CTG file '"))+strCtgFileName+wxString(_T("' not found.")));
    // CTG file not found, just copy the full font file
    fontSubset->Write(*fontFile);
  }
  return fontSize1;
}

// ----------------------------------------------------------------------------
// wxPdfFontOpenTypeUnicode: class 
// ----------------------------------------------------------------------------

wxPdfFontOpenTypeUnicode::wxPdfFontOpenTypeUnicode(int index)
  : wxPdfFont(index)
{
  m_type  = _T("OpenTypeUnicode");
  m_conv = NULL;
}

wxPdfFontOpenTypeUnicode::~wxPdfFontOpenTypeUnicode()
{
  // delete m_cw;
  if (m_conv != NULL)
  {
    delete m_conv;
  }
}

bool
wxPdfFontOpenTypeUnicode::LoadFontMetrics(wxXmlNode* root)
{
  bool bName  = false,
       bDesc  = false,
       bFile  = true,
       bSize  = true,
       bWidth = false;
  wxString value;
  long number;
  wxXmlNode *child = root->GetChildren();
  while (child)
  {
    // parse the children
    if (child->GetName() == wxT("font-name"))
    {
      m_name = GetNodeContent(child);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxT("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxT("diff"))
    {
      m_diffs = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("file"))
    {
      value = child->GetPropVal(_T("ctg"), _T(""));
      if (value.Length() > 0)
      {
        bFile = true;
        m_ctg = value;
        value = child->GetPropVal(_T("name"), _T(""));
        if (value.Length() > 0)
        {
          m_file = value;
          value = child->GetPropVal(_T("originalsize"), _T(""));
          if (value.Length() > 0 && value.ToLong(&number))
          {
            bFile = true;
            m_size1 = number;
          }
          else
          {
            bFile = false;
            m_file = _T("");
          }
        }
      }
      else
      {
        bFile = false;
        m_file = _T("");
        m_ctg = _T("");
      }
    }
    else if (child->GetName() == wxT("widths"))
    {
      wxString subsetting = child->GetPropVal(_T("subsetting"), _T("disabled"));
      m_subsetSupport = (subsetting == _T("enabled"));
      bWidth = true;
      m_cw = new wxPdfCharWidthMap();
      if (m_subsetSupport)
      {
        m_gn = new wxPdfCharWidthMap();
      }
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strGn, strWidth;
        long charId, glyph, charWidth;
        if (charNode->GetName() == wxT("char"))
        {
          strId = charNode->GetPropVal(_T("id"), _T(""));
          if (m_subsetSupport)
          {
            strGn = charNode->GetPropVal(_T("gn"), _T(""));
          }
          strWidth = charNode->GetPropVal(_T("width"), _T(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
            if (m_subsetSupport)
            {
              if (strGn.Length() > 0 && strGn.ToLong(&glyph))
              {
                (*m_gn)[charId] = glyph;
              }
              else
              {
                (*m_gn)[charId] = 0;
              }
            }
          }
        }
        charNode = charNode->GetNext();
      }
    }
    child = child->GetNext();
  }
  if (m_conv == NULL)
  {
    m_conv = new wxMBConvUTF16BE();
  }
  return (bName && bDesc && bFile && bSize && bWidth);
}

wxString
wxPdfFontOpenTypeUnicode::GetWidthsAsString()
{
  wxString s = wxString(_T("["));
  wxPdfCharWidthMap::iterator charIter;
  for (charIter = (*m_cw).begin(); charIter != (*m_cw).end(); charIter++)
  {
    if (!m_subset || (m_subset && SupportsSubset() && (m_usedChars->Index(charIter->first) != wxNOT_FOUND)))
    {
      int glyph = (*m_gn)[charIter->first];
      // define a specific width for each individual CID
      s += wxString::Format(_T("%d [%d] "), glyph, charIter->second);
    }
  }
  s += wxString(_T("]"));
  return s;
}

double
wxPdfFontOpenTypeUnicode::GetStringWidth(const wxString& s)
{
  // Get width of a string in the current font
  double w = 0;

  wxPdfCharWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    charIter = (*m_cw).find(s[i]);
    if (charIter != (*m_cw).end())
    {
      w += charIter->second;
    }
    else
    {
      w += m_desc.GetMissingWidth();
    }
  }
  return w / 1000;
}

wxString
wxPdfFontOpenTypeUnicode::ConvertCID2GID(const wxString& s)
{
  wxPdfCharWidthMap::iterator charIter;
  wxString t;
  size_t n = s.Length();
  size_t i;
  for (i = 0; i < n; i++)
  {
    charIter = (*m_gn).find(s[i]);
    if (charIter != (*m_gn).end())
    {
      t.Append(charIter->second);
    }
    else
    {
      t.Append(wxChar(0));
    }
  }
  return t;
}

int
wxPdfFontOpenTypeUnicode::CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset)
{
  // Font subsetting is currently not supported for OpenType Unicode fonts
  // Just copy the font file stream to the font subset stream
  fontSubset->Write(*fontFile);
  return GetSize1();
}

#endif // wxUSE_UNICODE

// ----------------------------------------------------------------------------
// wxPdfFontType1: class 
// ----------------------------------------------------------------------------

wxPdfFontType1::wxPdfFontType1(int index)
  : wxPdfFont(index)
{
  m_type  = _T("Type1");
  m_conv = NULL;
}

wxPdfFontType1::~wxPdfFontType1()
{
  if (m_conv != NULL)
  {
    delete m_conv;
  }
}

bool
wxPdfFontType1::LoadFontMetrics(wxXmlNode* root)
{
  bool bName  = false,
       bDesc  = false,
       bFile  = true,
       bSize  = true,
       bWidth = false;
  wxString value;
  long number;
  wxXmlNode *child = root->GetChildren();
  while (child)
  {
    // parse the children
    if (child->GetName() == wxT("font-name"))
    {
      m_name = GetNodeContent(child);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxT("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxT("diff"))
    {
      m_diffs = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("file"))
    {
      value = child->GetPropVal(_T("name"), _T(""));
      if (value.Length() > 0)
      {
        m_file = value;
        value = child->GetPropVal(_T("size1"), _T(""));
        if (value.Length() > 0 && value.ToLong(&number))
        {
          bFile = true;
          m_size1 = number;
          value = child->GetPropVal(_T("size2"), _T(""));
          if (value.Length() > 0 && value.ToLong(&number))
          {
            m_size2 = number;
          }
        }
        else
        {
          bFile = false;
          m_file = _T("");
        }
      }
    }
    else if (child->GetName() == wxT("widths"))
    {
      bWidth = true;
      m_cw = new wxPdfCharWidthMap();
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strWidth;
        long charId, charWidth;
        if (charNode->GetName() == wxT("char"))
        {
          strId = charNode->GetPropVal(_T("id"), _T(""));
          strWidth = charNode->GetPropVal(_T("width"), _T(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
          }
        }
        charNode = charNode->GetNext();
      }
    }
    child = child->GetNext();
  }

#if wxUSE_UNICODE
  // Unicode build needs charset conversion
  if (m_conv == NULL)
  {
    if (m_enc.Length() > 0)
    {
      m_conv = new wxCSConv(m_enc);
    }
    else
    {
      m_conv = new wxCSConv(wxFONTENCODING_ISO8859_1);
    }
  }
#endif

  return (bName && bDesc && bFile && bSize && bWidth);
}

double
wxPdfFontType1::GetStringWidth(const wxString& s)
{
  // Get width of a string in the current font
  double w = 0;
#if wxUSE_UNICODE
  wxCharBuffer wcb(s.mb_str(*m_conv));
  const char* str = (const char*) wcb;
#else
  const char* str = s.c_str();
#endif

  wxPdfCharWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    wxPdfCharWidthMap::iterator charIter = (*m_cw).find((unsigned char) str[i]);
    if (charIter != (*m_cw).end())
    {
      w += charIter->second;
    }
    else
    {
      w += m_desc.GetMissingWidth();
    }
  }
  return w / 1000;
}

int
wxPdfFontType1::CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset)
{
  // Font subsetting is currently not supported for Type1 fonts
  // Just copy the font file stream to the font subset stream
  fontSubset->Write(*fontFile);
  return GetSize1();
}

#if wxUSE_UNICODE

// ----------------------------------------------------------------------------
// wxPdfFontType0: class 
// ----------------------------------------------------------------------------

wxPdfFontType0::wxPdfFontType0(int index)
  : wxPdfFont(index)
{
  m_type = _T("Type0");
  m_conv = NULL;
  m_hwRange = false;
}

wxPdfFontType0::~wxPdfFontType0()
{
  if (m_conv != NULL)
  {
    delete m_conv;
  }
}

bool
wxPdfFontType0::LoadFontMetrics(wxXmlNode* root)
{
  bool bName     = false,
       bDesc     = false,
       bRegistry = false,
       bCmap     = false,
       bWidth    = false;
  wxString value;
  wxXmlNode *child = root->GetChildren();
  while (child)
  {
    // parse the children
    if (child->GetName() == wxT("font-name"))
    {
      m_name = GetNodeContent(child);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxT("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxT("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxT("cmap"))
    {
      m_cmap = wxEmptyString;
      value = GetNodeContent(child);
      if (value.Length() > 0)
      {
        bCmap = true;
        m_cmap = value;
      }
    }
    else if (child->GetName() == wxT("registry"))
    {
      m_ordering = wxEmptyString;
      m_supplement = wxEmptyString;
      value = child->GetPropVal(_T("ordering"), _T(""));
      if (value.Length() > 0)
      {
        m_ordering = value;
        value = child->GetPropVal(_T("supplement"), _T(""));
        if (value.Length() > 0)
        {
          bRegistry = true;
          m_supplement = value;
        }
        else
        {
          bRegistry = false;
        }
      }
    }
    else if (child->GetName() == wxT("widths"))
    {
      bWidth = true;
      m_cw = new wxPdfCharWidthMap();
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strWidth;
        long charId, charWidth;
        if (charNode->GetName() == wxT("char"))
        {
          strId = charNode->GetPropVal(_T("id"), _T(""));
          strWidth = charNode->GetPropVal(_T("width"), _T(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
          }
        }
        charNode = charNode->GetNext();
      }
    }
    child = child->GetNext();
  }
  if (m_conv == NULL)
  {
    if (m_enc.Length() > 0)
    {
      m_conv = new wxCSConv(m_enc);
    }
    else
    {
      m_conv = new wxCSConv(wxFONTENCODING_ISO8859_1);
    }
  }
  if (m_ordering == _T("Japan1"))
  {
    m_hwRange = true;
    m_hwFirst = 0xff61;
    m_hwLast  = 0xff9f;
  }
  return (bName && bDesc && bRegistry && bCmap && bWidth);
}

wxString
wxPdfFontType0::GetWidthsAsString()
{
  wxString s = wxString(_T("[1 ["));
  int i;
  for (i = 32; i <= 126; i++)
  {
    s += wxString::Format(_T("%d "), (*m_cw)[i]);
  }
  s += wxString(_T("]"));
  if (HasHalfWidthRange())
  {
    s += wxString(_T(" 231 325 500 631 [500] 326 389 500"));
  }
  s += wxString(_T("]"));
  return s;
}

double
wxPdfFontType0::GetStringWidth(const wxString& s)
{
  // Get width of a string in the current font
  double w = 0;
  wxPdfCharWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    wxChar c = s[i];
    if (c >= 0 && c < 128)
    {
      wxPdfCharWidthMap::iterator charIter = (*m_cw).find(c);
      if (charIter != (*m_cw).end())
      {
        w += charIter->second;
      }
      else
      {
        // TODO: ???
        // w += m_desc.GetMissingWidth();
      }
    }
    else
    {
      if (HasHalfWidthRange() &&
          c >= HalfWidthRangeFirst() &&
          c <= HalfWidthRangeLast())
      {
        w += 500;
      }
      else
      {
        w += 1000;
      }
    }
  }
  return w / 1000;
}

int
wxPdfFontType0::CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset)
{
  // Font subsetting is currently not supported for Type0 fonts
  // Just copy the font file stream to the font subset stream
  fontSubset->Write(*fontFile);
  return GetSize1();
}

#endif // wxUSE_UNICODE
