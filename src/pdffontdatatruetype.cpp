///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatatruetype.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2008-08-07
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatatruetype.cpp Implementation of wxPdfFontDataTrueType and wxPdfFontDataTrueTypeUnicode classes

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes
#include <wx/filename.h>
#include <wx/filesys.h>
#include <wx/zstream.h>

#include "wx/pdfarraytypes.h"
#include "wx/pdffontdatatruetype.h"
#include "wx/pdffontparser.h"
#include "wx/pdffontsubsettruetype.h"
#include "wx/pdffontvolt.h"

#include "woff/woffconverter.h"
#include "woff/woff2converter.h"

// TrueType font

wxPdfFontDataTrueType::wxPdfFontDataTrueType()
  : wxPdfFontData()
{
  m_type  = wxS("TrueType");
  m_conv = nullptr;

  m_embedRequired = false;
  m_embedSupported = true;
  m_subsetSupported = true;
}

wxPdfFontDataTrueType::~wxPdfFontDataTrueType()
{
  if (m_conv)
  {
    delete m_conv;
  }
}

#if wxUSE_UNICODE
void
wxPdfFontDataTrueType::CreateDefaultEncodingConv()
{
  // Unicode build needs charset conversion
  if (!m_conv)
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
}
#endif

bool
wxPdfFontDataTrueType::LoadFontMetrics(wxXmlNode* root)
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
    if (child->GetName() == wxS("font-name"))
    {
      m_name = GetNodeContent(child);
      m_style = FindStyleFromName(m_name);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxS("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxS("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxS("diff"))
    {
      m_diffs = GetNodeContent(child);
    }
    else if (child->GetName() == wxS("file"))
    {
      value = child->GetAttribute(wxS("name"), wxS(""));
      if (value.Length() > 0)
      {
        m_file = value;
        value = child->GetAttribute(wxS("originalsize"), wxS(""));
        if (value.Length() > 0 && value.ToLong(&number))
        {
          bFile = true;
          m_size1 = number;
        }
        else
        {
          bFile = false;
          m_file = wxS("");
        }
      }
    }
    else if (child->GetName() == wxS("widths"))
    {
      wxString subsetting = child->GetAttribute(wxS("subsetting"), wxS("disabled"));
      m_subsetSupported = (subsetting == wxS("enabled"));
      bWidth = true;
      m_cw = new wxPdfGlyphWidthMap();
      if (m_subsetSupported)
      {
        m_gn = new wxPdfChar2GlyphMap();
      }
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strGn, strWidth;
        long charId, charWidth, glyph;
        if (charNode->GetName() == wxS("char"))
        {
          strId = charNode->GetAttribute(wxS("id"), wxS(""));
          if (m_subsetSupported)
          {
            strGn = charNode->GetAttribute(wxS("gn"), wxS(""));
          }
          strWidth = charNode->GetAttribute(wxS("width"), wxS(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
            if (m_subsetSupported)
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
  CreateDefaultEncodingConv();
#endif

  m_initialized = (bName && bDesc && bFile && bSize && bWidth);
  if (m_initialized)
  {
    wxFileName fileName(m_file);
    m_initialized = fileName.MakeAbsolute(m_path) && fileName.FileExists() && fileName.IsFileReadable();
  }
  return m_initialized;
}

double
wxPdfFontDataTrueType::GetStringWidth(const wxString& s, const wxPdfEncoding* encoding, bool withKerning, double charSpacing) const
{
  wxUnusedVar(encoding);
  // Get width of a string in the current font
  int glyphCount = 0;
  double w = 0;
#if wxUSE_UNICODE
  wxString t = ConvertToValid(s);
  const wxScopedCharBuffer wcb(t.mb_str(*m_conv));
  const char* str = (const char*) wcb;
#else
  const char* str = s.c_str();
#endif

  wxPdfGlyphWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    charIter = m_cw->find((unsigned char) str[i]);
    if (charIter != m_cw->end())
    {
      w += charIter->second;
    }
    else
    {
      w += m_desc.GetMissingWidth();
    }
    ++glyphCount;
  }
  if (withKerning)
  {
    int kerningWidth = GetKerningWidth(s);
    if (kerningWidth != 0)
    {
      w += (double) kerningWidth;
    }
  }
  if (charSpacing > 0)
  {
    w += (glyphCount * charSpacing * 1000);
  }
  return w / 1000;
}

bool
wxPdfFontDataTrueType::CanShow(const wxString& s, const wxPdfEncoding* encoding) const
{
  wxUnusedVar(encoding);
  bool canShow = true;
#if wxUSE_UNICODE
  wxMBConv* conv = GetEncodingConv();
  size_t len = conv->FromWChar(nullptr, 0, s.wc_str(), s.length());
  canShow = (len != wxCONV_FAILED);
#endif
  return canShow;
}

wxString
wxPdfFontDataTrueType::ConvertCID2GID(const wxString& s,
                                      const wxPdfEncoding* encoding,
                                      wxPdfSortedArrayInt* usedGlyphs,
                                      wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(encoding);
  // Currently no cid to gid conversion is done
  // only the list of used glyphs is updated
  wxUnusedVar(subsetGlyphs);
  if (usedGlyphs)
  {
    // Convert string according to the font encoding
#if wxUSE_UNICODE
    size_t slen = s.length();
    wxString t = ConvertToValid(s);
    wxMBConv* conv = GetEncodingConv();
    size_t len = conv->FromWChar(nullptr, 0, t.wc_str(), slen);
    char* mbstr = new char[len+3];
    len = conv->FromWChar(mbstr, len+3, t.wc_str(), slen);
#else
    size_t len = s.Length();;
    char* mbstr = new char[len+1];
    strcpy(mbstr,s.c_str());
#endif
    // Update used glyph list
    wxPdfChar2GlyphMap::const_iterator glyphIter;
    size_t i;
    for (i = 0; i < len; i++)
    {
      int ch = (int) mbstr[i];
      glyphIter = m_gn->find(ch);
      if (glyphIter != m_gn->end())
      {
        if (usedGlyphs->Index(glyphIter->second) == wxNOT_FOUND)
        {
          usedGlyphs->Add(glyphIter->second);
        }
      }
    }
    delete [] mbstr;
  }
  return s;
}

wxString
wxPdfFontDataTrueType::GetWidthsAsString(bool subset, wxPdfSortedArrayInt* usedGlyphs, wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(subset);
  wxUnusedVar(usedGlyphs);
  wxUnusedVar(subsetGlyphs);
  wxString s = wxString(wxS("["));
  int i;
  for (i = 32; i <= 255; i++)
  {
    s += wxString::Format(wxS("%u "), (*m_cw)[i]);
  }
  s += wxString(wxS("]"));
  return s;
}

size_t
wxPdfFontDataTrueType::WriteFontData(wxOutputStream* fontData, wxPdfSortedArrayInt* usedGlyphs, wxPdfChar2GlyphMap* subsetGlyphs)
{
  wxUnusedVar(subsetGlyphs);
  wxFSFile* fontFile = nullptr;
  wxFileName fileName;
  wxInputStream* fontStream = nullptr;
  bool deleteFontStream = false;
  size_t fontSize1 = 0;
  bool compressed = false;

  if (m_fontBuffer && m_fontBufferSize > 0)
  {
    deleteFontStream = true;
    fontStream = new wxMemoryInputStream(m_fontBuffer, m_fontBufferSize);
  }
  else
  {
    if (m_fontFileName.IsEmpty())
    {
      // Font data preprocessed by MakeFont
      compressed = m_file.Lower().Right(2) == wxS(".z");
      fileName = m_file;
      fileName.MakeAbsolute(m_path);
    }
    else
    {
      fileName = m_fontFileName;
    }

    // Open font file
    wxFileSystem fs;
    fontFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileName));
    if (fontFile)
    {
      bool isWoff = false;
      wxMemoryInputStream* woffStream = nullptr;
      fontStream = fontFile->GetStream();
      if (fileName.GetExt().Lower().IsSameAs(wxS("woff")))
      {
        isWoff = true;
        woffStream = Woff2Converter::Convert(fontStream);
      }
      else if (fileName.GetExt().Lower().IsSameAs(wxS("woff2")))
      {
        isWoff = true;
        woffStream = Woff2Converter::Convert(fontStream);
        fontStream = woffStream;
        deleteFontStream = true;
      }
      if (isWoff)
      {
        fontStream = woffStream;
        if (woffStream)
        {
          deleteFontStream = true;
        }
        else
        {
          wxLogError(wxString(wxS("wxPdfFontDataTrueType::WriteFontData: ")) +
                     wxString::Format(_("Font file '%s' is not a valid WOFF/WOFF2 file."), fileName.GetFullPath().c_str()));
        }
      }
    }
    else
    {
      // usually this should not happen since file accessability was already checked
      wxLogError(wxString(wxS("wxPdfFontDataTrueType::WriteFontData: ")) +
                 wxString::Format(_("Font file '%s' not found."), fileName.GetFullPath().c_str()));
    }
  }

  if (fontStream)
  {
    if (usedGlyphs)
    {
      if (compressed)
      {
        // Uncompress font file
        wxZlibInputStream zCompressed(*fontStream);
        wxMemoryOutputStream zUncompressed;
        zUncompressed.Write(zCompressed);
        zUncompressed.Close();
        if (deleteFontStream)
        {
          delete fontStream;
        }
        fontStream = new wxMemoryInputStream(zUncompressed);
      }

      // Assemble subset
      wxPdfFontSubsetTrueType subset(fileName.IsOk() ? fileName.GetFullPath()
                                                     : wxString::Format(_("Font buffer size '%lu'"), (unsigned long) m_fontBufferSize),
                                     m_fontIndex);
      wxMemoryOutputStream* subsetStream = subset.CreateSubset(fontStream, usedGlyphs, true);
      if (compressed || deleteFontStream)
      {
        delete fontStream;
      }

      // Write font subset data
      wxZlibOutputStream zFontData(*fontData);
      wxMemoryInputStream tmp(*subsetStream);
      fontSize1 = tmp.GetSize();
      zFontData.Write(tmp);
      zFontData.Close();
      delete subsetStream;
    }
    else
    {
      if (!compressed)
      {
        fontSize1 = fontStream->GetSize();
        wxZlibOutputStream zFontData(*fontData);
        zFontData.Write(*fontStream);
        zFontData.Close();
      }
      else
      {
        fontSize1 = GetSize1();
        fontData->Write(*fontStream);
      }
    }
  }

  if (fontFile)
  {
    delete fontFile;
  }

  return fontSize1;
}

#if wxUSE_UNICODE

// TrueTypeUnicode font

wxPdfFontDataTrueTypeUnicode::wxPdfFontDataTrueTypeUnicode()
  : wxPdfFontData()
{
  m_type = wxS("TrueTypeUnicode");
  m_gw   = nullptr;
  m_conv = nullptr;
  m_volt = nullptr;

  m_embedRequired = true;
  m_embedSupported = true;
  m_subsetSupported = true;
}

wxPdfFontDataTrueTypeUnicode::~wxPdfFontDataTrueTypeUnicode()
{
  // delete m_cw;
  if (m_conv)
  {
    delete m_conv;
  }
  if (m_gw)
  {
    delete m_gw;
  }
  if (m_volt)
  {
    delete m_volt;
  }
}

void
wxPdfFontDataTrueTypeUnicode::CreateDefaultEncodingConv()
{
  if (!m_conv)
  {
    m_conv = new wxMBConvUTF16BE();
  }
}


bool
wxPdfFontDataTrueTypeUnicode::LoadFontMetrics(wxXmlNode* root)
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
    if (child->GetName() == wxS("font-name"))
    {
      m_name = GetNodeContent(child);
      m_style = FindStyleFromName(m_name);
      bName = m_name.Length() > 0;
    }
    else if (child->GetName() == wxS("encoding"))
    {
      m_enc = GetNodeContent(child);
    }
    else if (child->GetName() == wxS("description"))
    {
      bDesc = GetFontDescription(child, m_desc);
    }
    else if (child->GetName() == wxS("diff"))
    {
      m_diffs = GetNodeContent(child);
    }
    else if (child->GetName() == wxS("file"))
    {
      value = child->GetAttribute(wxS("ctg"), wxS(""));
      if (value.Length() > 0)
      {
        bFile = true;
        m_ctg = value;
        value = child->GetAttribute(wxS("name"), wxS(""));
        if (value.Length() > 0)
        {
          m_file = value;
          value = child->GetAttribute(wxS("originalsize"), wxS(""));
          if (value.Length() > 0 && value.ToLong(&number))
          {
            bFile = true;
            m_size1 = number;
          }
          else
          {
            bFile = false;
            m_file = wxS("");
          }
        }
      }
      else
      {
        bFile = false;
        m_file = wxS("");
        m_ctg = wxS("");
      }
    }
    else if (child->GetName() == wxS("widths"))
    {
      bWidth = true;
      m_cw = new wxPdfGlyphWidthMap();
      const wxXmlNode *charNode = child->GetChildren();
      while (charNode)
      {
        wxString strId, strWidth;
        long charId, charWidth;
        if (charNode->GetName() == wxS("char"))
        {
          strId = charNode->GetAttribute(wxS("id"), wxS(""));
          strWidth = charNode->GetAttribute(wxS("width"), wxS(""));
          if (strId.Length() > 0 && strId.ToLong(&charId) &&
              strWidth.Length() > 0 && strWidth.ToLong(&charWidth))
          {
            (*m_cw)[charId] = charWidth;
          }
        }
        charNode = charNode->GetNext();
      }
    }
    else if (child->GetName() == wxS("volt"))
    {
      m_volt = new wxPdfVolt();
      m_volt->LoadVoltData(child);
    }
    child = child->GetNext();
  }
  CreateDefaultEncodingConv();

  m_initialized = (bName && bDesc && bFile && bSize && bWidth);
  if (m_initialized)
  {
    wxFileName fileName(m_file);
    m_initialized = fileName.MakeAbsolute(m_path) && fileName.FileExists() && fileName.IsFileReadable();
    if (m_initialized)
    {
      fileName.Assign(m_ctg);
      m_initialized = fileName.MakeAbsolute(m_path) && fileName.FileExists() && fileName.IsFileReadable();
    }
  }
  if (m_initialized && !m_gn)
  {
    // We now always need a cid to gid mapping whether subsetting is enabled or not
    // So we read the CTG file produced by MakeFont and create the map
    bool compressed = m_ctg.Lower().Right(2) == wxS(".z");
    wxFileName fileName(m_ctg);
    fileName.MakeAbsolute(m_path);
    wxFileSystem fs;
    wxFSFile* ctgFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileName));
    wxInputStream* ctgStream = nullptr;
    if (ctgFile)
    {
      ctgStream = ctgFile->GetStream();
    }
    else
    {
      m_initialized = false;
      // usually this should not happen since file accessability was already checked
      wxLogError(wxString(wxS("wxPdfFontDataTrueTypeUnicode::LoadFontMetrics: ")) +
                 wxString::Format(_("CTG file '%s' not found."), fileName.GetFullPath().c_str()));
    }
    if (ctgStream)
    {
      size_t ctgLen;
      unsigned char* cc2gn = nullptr;
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

      // Create the cid to gid mapping
      m_gn = new wxPdfChar2GlyphMap();
      size_t charId;
      for (charId = 0; charId < 0xFFFF; ++charId)
      {
        int glyph = (cc2gn[2*charId] << 8) + cc2gn[2*charId+1];
        if (glyph != 0)
        {
          (*m_gn)[charId] = glyph;
        }
      }
      delete [] cc2gn;
    }
  }
  return m_initialized;
}

bool
wxPdfFontDataTrueTypeUnicode::Initialize()
{
  bool ok = true;
  if (!IsInitialized())
  {
    wxPdfFontParserTrueType fontParser;
    ok = fontParser.LoadFontData(this);
    m_initialized = ok;
  }
  return ok;
}

wxString
wxPdfFontDataTrueTypeUnicode::ApplyVoltData(const wxString& s) const
{
  return (m_volt) ? m_volt->ProcessRules(s) : s;
}

void
wxPdfFontDataTrueTypeUnicode::SetGlyphWidths(const wxPdfArrayUint16& glyphWidths)
{
  if (!m_gw)
  {
    m_gw = new wxPdfArrayUint16;
  }
  *m_gw = glyphWidths;
}

double
wxPdfFontDataTrueTypeUnicode::GetStringWidth(const wxString& s, const wxPdfEncoding* encoding, bool withKerning, double charSpacing) const
{
  wxUnusedVar(encoding);
  // Get width of a string in the current font
  int glyphCount = 0;
  double w = 0;

  wxPdfGlyphWidthMap::iterator charIter;
  size_t i;
  for (i = 0; i < s.Length(); i++)
  {
    charIter = m_cw->find(s[i]);
    if (charIter != m_cw->end())
    {
      w += charIter->second;
    }
    else
    {
      w += m_desc.GetMissingWidth();
    }
    ++glyphCount;
  }
  if (withKerning)
  {
    int kerningWidth = GetKerningWidth(s);
    if (kerningWidth != 0)
    {
      w += (double) kerningWidth;
    }
  }
  if (charSpacing > 0)
  {
    w += (glyphCount * charSpacing * 1000);
  }
  return w / 1000;
}

bool
wxPdfFontDataTrueTypeUnicode::CanShow(const wxString& s, const wxPdfEncoding* encoding) const
{
  wxUnusedVar(encoding);
  bool canShow = true;
  wxString::const_iterator ch;
  for (ch = s.begin(); canShow && ch != s.end(); ++ch)
  {
    canShow = (m_gn->find(*ch) != m_gn->end());
  }
  return canShow;
}

wxString
wxPdfFontDataTrueTypeUnicode::ConvertCID2GID(const wxString& s,
                                             const wxPdfEncoding* encoding,
                                             wxPdfSortedArrayInt* usedGlyphs,
                                             wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(encoding);
  wxUnusedVar(subsetGlyphs);
  bool doSubsetting = usedGlyphs;
  wxString t;
  wxPdfChar2GlyphMap::const_iterator charIter;
  wxUint32 glyph;
  wxString::const_iterator ch;
  for (ch = s.begin(); ch != s.end(); ++ch)
  {
    // Handle surrogates
    if ((*ch < 0xd800) || (*ch > 0xdfff))
    {
      charIter = m_gn->find(*ch);
    }
    else
    {
      wxUint32 c1 = *ch;
      ++ch;
      if (ch != s.end())
      {
        wxUint32 c2 = *ch;
        if ((c2 < 0xdc00) || (c2 > 0xdfff))
        {
          charIter = m_gn->end();
          --ch;
        }
        else
        {
          wxUint32 cc = ((c1 - 0xd7c0) << 10) + (c2 - 0xdc00);
          charIter = m_gn->find(cc);
        }
      }
      else
      {
        --ch;
        charIter = m_gn->find(*ch);
      }
    }
    if (charIter != m_gn->end())
    {
      glyph = charIter->second;
      if (doSubsetting)
      {
        if (usedGlyphs->Index(glyph) == wxNOT_FOUND)
        {
          usedGlyphs->Add(glyph);
        }
      }
      t.Append(wxUniChar(glyph));
    }
    else
    {
      t.Append(wxUniChar(0));
    }
  }
  return t;
}

wxString
wxPdfFontDataTrueTypeUnicode::ConvertGlyph(wxUint32 glyph,
                                           const wxPdfEncoding* encoding,
                                           wxPdfSortedArrayInt* usedGlyphs,
                                           wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(encoding);
  wxUnusedVar(subsetGlyphs);
  wxString t = wxEmptyString;
  if (m_gw && glyph < m_gw->size())
  {
    bool doSubsetting = usedGlyphs;
    if (doSubsetting)
    {
      if (usedGlyphs->Index(glyph) == wxNOT_FOUND)
      {
        usedGlyphs->Add(glyph);
      }
    }
    t.Append(wxUniChar(glyph));
  }
  else
  {
    t.Append(wxUniChar(0));
  }
  return t;
}

wxString
wxPdfFontDataTrueTypeUnicode::GetWidthsAsString(bool subset, wxPdfSortedArrayInt* usedGlyphs, wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(subsetGlyphs);
  wxString s = wxString(wxS("["));
  wxUint32 glyph;
  wxPdfChar2GlyphMap::const_iterator glyphIter;
  wxPdfGlyphWidthMap::iterator charIter;
  for (charIter = m_cw->begin(); charIter != m_cw->end(); charIter++)
  {
    glyphIter = m_gn->find(charIter->first);
    if (glyphIter != m_gn->end())
    {
      glyph = glyphIter->second;
    }
    else
    {
      glyph = 0;
    }
    if (glyph != 0 && (!subset || !usedGlyphs ||
                       (subset && SubsetSupported() && (usedGlyphs->Index(glyph) != wxNOT_FOUND))))
    {
      // define a specific width for each individual CID
      s += wxString::Format(wxS("%u [%u] "), glyph, charIter->second);
    }
  }
  s += wxString(wxS("]"));
  return s;
}

size_t
wxPdfFontDataTrueTypeUnicode::WriteFontData(wxOutputStream* fontData, wxPdfSortedArrayInt* usedGlyphs, wxPdfChar2GlyphMap* subsetGlyphs)
{
  bool isMacCoreText = false;
  bool deleteFontStream = false;
  wxUnusedVar(subsetGlyphs);
  size_t fontSize1 = 0;
  wxFSFile* fontFile = nullptr;
  wxInputStream* fontStream = nullptr;
  bool compressed = false;
  wxString fontFullPath = wxEmptyString;
  wxFileName fileName;

  if (m_fontBuffer && m_fontBufferSize > 0)
  {
    deleteFontStream = true;
    fontStream = new wxMemoryInputStream(m_fontBuffer, m_fontBufferSize);
    fontFullPath = wxString::Format(_("Font buffer size '%lu'"), (unsigned long)m_fontBufferSize);
  }
  else
  {
    if (m_fontFileName.IsEmpty())
    {
#if defined(__WXMSW__)
      if (m_file.IsEmpty() && m_font.IsOk())
      {
        fontStream = wxPdfFontParserTrueType::LoadTrueTypeFontStream(m_font);
        deleteFontStream = true;
      }
      else
#elif defined(__WXMAC__)
#if wxPDFMACOSX_HAS_CORE_TEXT
      if (m_file.IsEmpty() && m_font.IsOk())
      {
        fontStream = new wxMemoryInputStream("dummy", 5);
        deleteFontStream = true;
        isMacCoreText = true;
      }
      else
#endif
#endif
      {
        // Font data preprocessed by MakeFont
        compressed = m_file.Lower().Right(2) == wxS(".z");
        fileName = m_file;
        fileName.MakeAbsolute(m_path);
      }
    }
    else
    {
      fileName = m_fontFileName;
    }

    if (fileName.IsOk())
    {
      // Open font file
      wxFileSystem fs;
      fontFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileName));
      if (fontFile)
      {
        bool isWoff = false;
        wxMemoryInputStream* woffStream = nullptr;
        fontStream = fontFile->GetStream();
        deleteFontStream = false;
        fontFullPath = fileName.GetFullPath();
        if (fileName.GetExt().Lower().IsSameAs(wxS("woff")))
        {
          isWoff = true;
          woffStream = Woff2Converter::Convert(fontStream);
        }
        else if (fileName.GetExt().Lower().IsSameAs(wxS("woff2")))
        {
          isWoff = true;
          woffStream = Woff2Converter::Convert(fontStream);
        }
        if (isWoff)
        {
          fontStream = woffStream;
          if (woffStream)
          {
            deleteFontStream = true;
          }
          else
          {
            wxLogError(wxString(wxS("wxPdfFontDataTrueTypeUnicode::WriteFontData: ")) +
                       wxString::Format(_("Font file '%s' is not a valid WOFF/WOFF2 file."), fileName.GetFullPath().c_str()));
          }
        }
      }
      else
      {
        // usually this should not happen since file accessability was already checked
        wxLogError(wxString(wxS("wxPdfFontDataTrueTypeUnicode::WriteFontData: ")) +
                   wxString::Format(_("Font file '%s' not found."), fileName.GetFullPath().c_str()));
      }
    }
  }

  if (fontStream)
  {
    if (usedGlyphs)
    {
      if (compressed)
      {
        // Uncompress font file
        wxZlibInputStream zCompressed(*fontStream);
        wxMemoryOutputStream zUncompressed;
        zUncompressed.Write(zCompressed);
        zUncompressed.Close();
        if (deleteFontStream)
        {
          delete fontStream;
        }
        fontStream = new wxMemoryInputStream(zUncompressed);
        deleteFontStream = true;
      }

      // Assemble subset
      wxPdfFontSubsetTrueType subset(fontFullPath, 0, isMacCoreText);
#if defined(__WXMAC__)
#if wxPDFMACOSX_HAS_CORE_TEXT
      if (m_font.IsOk())
      {
        subset.SetCTFontRef(m_font);
      }
#endif
#endif
      wxMemoryOutputStream* subsetStream = subset.CreateSubset(fontStream, usedGlyphs, false);
      if (deleteFontStream && fontStream)
      {
        delete fontStream;
      }

      // Write font subset data
      wxZlibOutputStream zFontData(*fontData);
      wxMemoryInputStream tmp(*subsetStream);
      fontSize1 = tmp.GetSize();
      zFontData.Write(tmp);
      zFontData.Close();
      delete subsetStream;
    }
    else
    {
      if (!compressed)
      {
        fontSize1 = fontStream->GetSize();
        wxZlibOutputStream zFontData(*fontData);
        zFontData.Write(*fontStream);
        zFontData.Close();
      }
      else
      {
        fontSize1 = GetSize1();
        fontData->Write(*fontStream);
      }
    }
  }

  if (fontFile)
  {
    delete fontFile;
  }

  return fontSize1;
}

size_t
wxPdfFontDataTrueTypeUnicode::WriteUnicodeMap(wxOutputStream* mapData,
                                              const wxPdfEncoding* encoding,
                                              wxPdfSortedArrayInt* usedGlyphs,
                                              wxPdfChar2GlyphMap* subsetGlyphs)
{
  wxUnusedVar(encoding);
  wxUnusedVar(subsetGlyphs);
  wxPdfGlyphList glyphList(wxPdfFontData::CompareGlyphListEntries);
  wxPdfChar2GlyphMap::const_iterator charIter = m_gn->begin();
  for (charIter = m_gn->begin(); charIter != m_gn->end(); ++charIter)
  {
    if (usedGlyphs)
    {
      if (usedGlyphs->Index(charIter->second) != wxNOT_FOUND)
      {
        wxPdfGlyphListEntry* glEntry = new wxPdfGlyphListEntry();
        glEntry->m_gid = charIter->second;
        glEntry->m_uid = charIter->first;
        glyphList.Add(glEntry);
      }
    }
    else
    {
      wxPdfGlyphListEntry* glEntry = new wxPdfGlyphListEntry();
      glEntry->m_gid = charIter->second;
      glEntry->m_uid = charIter->first;
      glyphList.Add(glEntry);
    }
  }
  wxMemoryOutputStream toUnicode;
  WriteToUnicode(glyphList, toUnicode);
  wxMemoryInputStream inUnicode(toUnicode);
  wxZlibOutputStream zUnicodeMap(*mapData);
  zUnicodeMap.Write(inUnicode);
  zUnicodeMap.Close();

  WX_CLEAR_ARRAY(glyphList);

  return 0;
}

size_t
wxPdfFontDataTrueTypeUnicode::WriteCIDToGIDMap(wxOutputStream* mapData,
                                               const wxPdfEncoding* encoding,
                                               wxPdfSortedArrayInt* usedGlyphs,
                                               wxPdfChar2GlyphMap* subsetGlyphs)
{
  wxUnusedVar(encoding);
  wxUnusedVar(subsetGlyphs);

  // Prepare empty CIDToGIDMap
  static size_t CC2GNSIZE = 131072; // 2*64kB
  unsigned char* cc2gn = new unsigned char[CC2GNSIZE];
  size_t j;
  for (j = 0; j < CC2GNSIZE; j++)
  {
    cc2gn[j] = '\0';
  }

  wxPdfChar2GlyphMap::const_iterator c2gMapIter;
  for (c2gMapIter = m_gn->begin(); c2gMapIter != m_gn->end(); ++c2gMapIter)
  {
    wxUint32 gid = c2gMapIter->second;
    bool setMap = (usedGlyphs) ? (usedGlyphs->Index(gid) != wxNOT_FOUND) : true;
    // Set GID
    // Note: One would expect that CID is used to index the mapping array.
    // However, wxPdfDocument already replaces CIDs by GIDs on adding text strings
    // to PDF content. Therefore gid is used as the array index.
    if (setMap && gid < 0xFFFF)
    {
      cc2gn[2 * gid] = (gid >> 8) & 0xFF;
      cc2gn[2 * gid + 1] = gid & 0xFF;
    }
  }

  wxZlibOutputStream zCIDToGIDMap(*mapData);
  zCIDToGIDMap.Write(cc2gn, CC2GNSIZE);
  zCIDToGIDMap.Close();

  delete [] cc2gn;

  return 0;
}

size_t
wxPdfFontDataTrueTypeUnicode::WriteCIDSet(wxOutputStream* setData,
                                          const wxPdfEncoding* encoding,
                                          wxPdfSortedArrayInt* usedGlyphs,
                                          wxPdfChar2GlyphMap* subsetGlyphs)
{
  wxUnusedVar(encoding);
  wxUnusedVar(subsetGlyphs);

  size_t gCount = m_gn->size();
  size_t gExtra = (gCount % 8) ? 1 : 0;
  size_t gBytes = gCount / 8 + gExtra;
  unsigned char* cidSet = new unsigned char[8192];
  size_t j;
  for (j = 0; j < gBytes; j++)
  {
    cidSet[j] = 0x00;
  }
  cidSet[0] = 0x80;

  wxPdfChar2GlyphMap::const_iterator c2gMapIter;
  for (c2gMapIter = m_gn->begin(); c2gMapIter != m_gn->end(); ++c2gMapIter)
  {
    wxUint32 gid = c2gMapIter->second;
    bool setMap = (usedGlyphs) ? (usedGlyphs->Index(gid) != wxNOT_FOUND) : true;
    // Set GID
    // Note: One would expect that CID is used to index the mapping array.
    // However, wxPdfDocument already replaces CIDs by GIDs on adding text strings
    // to PDF content. Therefore gid is used as the array index.
    if (setMap)
    {
      size_t bytePos = gid / 8;
      size_t bitPos = gid % 8;
      unsigned char mask = (0x80 >> bitPos);
      cidSet[bytePos] |= mask;
    }
  }

  wxZlibOutputStream zCIDSet(*setData);
  zCIDSet.Write(cidSet, 8192);
  zCIDSet.Close();

  delete[] cidSet;

  return 0;
}

#endif // wxUSE_UNICODE
