///////////////////////////////////////////////////////////////////////////////
// Name:        makefont.cpp
// Purpose:     Utility for creating font metrics file for use with wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-09-05
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

#include "wx/cmdline.h"
#include "wx/log.h"
#include "wx/dynarray.h"
#include "wx/tokenzr.h"
#include "wx/mstream.h"
#include "wx/txtstrm.h"
#include "wx/wfstream.h"
#include "wx/xml/xml.h"
#include "wx/zstream.h"

#include "wx/pdfdoc.h"

class GlyphListEntry
{
public:
  GlyphListEntry() {};
  ~GlyphListEntry() {};
  int m_gid;
  int m_uid;
};

WX_DEFINE_SORTED_ARRAY(GlyphListEntry*, GlyphList);

static int
CompareGlyphListEntries(GlyphListEntry* item1, GlyphListEntry* item2)
{
  return item1->m_gid - item2->m_gid;
}

WX_DECLARE_HASH_MAP(long, wxString, wxIntegerHash, wxIntegerEqual, CTGMap);
WX_DECLARE_STRING_HASH_MAP(wxString, FixGlyphMap);

/// Fast string search (KMP method): initialization
static int*
makeFail(const char* target, int tlen)
{
  int t = 0;
  int s, m;
  m = tlen;
  int* f = new int[m+1];
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

/// Fast string search (KMP method)
static int
findString(const char* src, int slen, const char* target, int tlen, int* f)
{
  int s = 0;
  int i;
  int m = tlen;
  for (i = 0; i < slen; i++)
  {
    while ( (s > 0) && (src[i] != target[s]))
    {
      s = f[s];
    }
    if (src[i] == target[s]) s++;
    if (s == m) return (i-m+1);
  }
  return -1;
}

/// Read encoding map
bool
ReadMap(const wxString& enc, CTGMap& cc2gn)
{
  //Read a map file
  wxString mapFileName = enc.Lower() + _T(".map");
  wxFileInputStream mapFile(mapFileName);
  if (!mapFile.Ok())
  {
    wxLogMessage(_T("Error: Unable to read map file '") + mapFileName + _T("'."));
    return false;
  }

  wxTextInputStream text(mapFile);
  wxString charcode, unicode, glyphname;

  cc2gn.clear();
  while (!mapFile.Eof())
  {
    text >> charcode >> unicode >> glyphname;
    if (!mapFile.Eof() && charcode.Length() > 0)
    {
      if (charcode[0] == _T('!'))
      {
        int cc = wxHexToDec(charcode.Mid(1));
        cc2gn[cc] = glyphname;
      }
    }
  }

  int i;
  for (i = 0; i <= 255; i++)
  {
    CTGMap::iterator iter = cc2gn.find(i);
    if (iter == cc2gn.end())
    {
      cc2gn[i] = _T(".notdef");
    }
  }
  return true;
}

/// Read a 2-byte integer from file (big endian)
short
ReadShort(wxInputStream* stream)
{
  short i16;
  stream->Read(&i16, 2);
  return wxINT16_SWAP_ON_LE(i16);
}

/// Read a 4-byte integer from file (big endian)
int
ReadInt(wxInputStream* stream)
{
  int i32;
  stream->Read(&i32, 4);
  return wxINT32_SWAP_ON_LE(i32);
}

/// Check TrueType font file whether font license allows embedding
bool
CheckTTF(const wxString& fileName, int& cffOffset, int& cffLength)
{
  cffOffset = -1;
  cffLength = 0;
  if (fileName.Length() == 0)
  {
    return false;
  }
  wxFileInputStream ttfFile(fileName);
  if (!ttfFile.Ok())
  {
    // Can't open file
    wxLogMessage(_T("Error: Unable to read font file '") + fileName + _T("'."));
    return false;
  }
  // Extract number of tables
  ttfFile.SeekI(0, wxFromCurrent);
  int id = ReadInt(&ttfFile);
  if (id != 0x00010000 && id != 0x4f54544f)
  {
    wxLogError(_T("Error: File '") + fileName + _T("' is not a valid font file."));
    return false;
  }
  short nb = ReadShort(&ttfFile);
  ttfFile.SeekI(6, wxFromCurrent);
  // Seek OS/2 table
  bool found = false;
  int offset = 0;
  int i;
  for (i = 0; i < nb; i++)
  {
    char buffer[4];
    ttfFile.Read(buffer,4);
    if (strncmp(buffer,"OS/2",4) == 0)
    {
      found = true;
      ttfFile.SeekI(4, wxFromCurrent);
      offset = ReadInt(&ttfFile);
      ttfFile.SeekI(4, wxFromCurrent);
    }
    else if (strncmp(buffer,"CFF ",4) == 0)
    {
      ttfFile.SeekI(4, wxFromCurrent);
      cffOffset = ReadInt(&ttfFile);
      cffLength = ReadInt(&ttfFile);
    }
    else
    {
      ttfFile.SeekI(12, wxFromCurrent);
    }
  }
  if (!found)
  {
    return false;
  }
  ttfFile.SeekI(offset, wxFromStart);
  // Extract fsType flags
  ttfFile.SeekI(8, wxFromCurrent);
  short fsType = ReadShort(&ttfFile);
  bool rl = (fsType & 0x02) != 0;
  bool pp = (fsType & 0x04) != 0;
  bool e  = (fsType & 0x08) != 0;
  return !(rl && !pp && !e);
}

/// Create wxPdfDocument font metrics file
void
CreateFontMetricsFile(const wxString& xmlFileName, wxPdfFont& font, bool includeGlyphInfo)
{
  wxXmlDocument fontMetrics;
  const wxPdfFontDescription& fd = font.GetDesc();

  wxXmlNode* rootNode = new wxXmlNode(wxXML_ELEMENT_NODE, _T("wxpdfdoc-font-metrics"));
  rootNode->AddProperty(_T("type"), font.GetType() /*_T("TrueTypeUnicode")*/);
  wxXmlNode* node;
  wxXmlNode* textNode;

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("font-name"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), font.GetName());
  node->AddChild(textNode);
  rootNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("encoding"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), font.GetEncoding());
  node->AddChild(textNode);
  rootNode->AddChild(node);

  wxXmlNode* descNode = new wxXmlNode(wxXML_ELEMENT_NODE, _T("description"));

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("ascent"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetAscent()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("descent"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetDescent()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("cap-height"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetCapHeight()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("flags"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetFlags()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("font-bbox"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), fd.GetFontBBox());
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("italic-angle"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetItalicAngle()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("stem-v"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetStemV()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("missing-width"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetMissingWidth()));
  node->AddChild(textNode);
  descNode->AddChild(node);
  
  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("x-height"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetXHeight()));
  node->AddChild(textNode);
  descNode->AddChild(node);
  
  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("underline-position"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetUnderlinePosition()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("underline-thickness"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), wxString::Format(_T("%d"), fd.GetUnderlineThickness()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  rootNode->AddChild(descNode);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("diff"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, _T("text"), font.GetDiffs());
  node->AddChild(textNode);
  rootNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("file"));
  node->AddProperty(_T("name"), font.GetFontFile());
  wxString fontType = font.GetType();
  if (fontType == _T("TrueTypeUnicode") || fontType == _T("OpenTypeUnicode"))
  {
    // TrueTypeUnicode (name,ctg,originalsize)
    node->AddProperty(_T("ctg"), font.GetCtgFile());
    node->AddProperty(_T("originalsize"), wxString::Format(_T("%d"), font.GetSize1()));
  }
  else if (fontType == _T("TrueType"))
  {
    // Truetype (name, originalsize)
    node->AddProperty(_T("originalsize"), wxString::Format(_T("%d"), font.GetSize1()));
  }
  else if  (fontType == _T("Type1"))
  {
    // Type1 (name, size1, size2)
    node->AddProperty(_T("size1"), wxString::Format(_T("%d"), font.GetSize1()));
    if (font.HasSize2())
    {
      node->AddProperty(_T("size2"), wxString::Format(_T("%d"), font.GetSize2()));
    }
  }
  rootNode->AddChild(node);

  wxXmlNode* widthsNode = new wxXmlNode(wxXML_ELEMENT_NODE, _T("widths"));
  if (includeGlyphInfo)
  {
    widthsNode->AddProperty(_T("subsetting"), _T("enabled"));
  }
  wxPdfCharWidthMap* widths = (wxPdfCharWidthMap*) font.GetCharWidthMap();
  wxPdfCharWidthMap* glyphs = (wxPdfCharWidthMap*) font.GetGlyphNumberMap();
  wxPdfCharWidthMap::iterator iter = widths->begin();
  for (iter = widths->begin(); iter != widths->end(); iter++)
  {
    node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("char"));
    node->AddProperty(_T("id"), wxString::Format(_T("%d"), iter->first));
    if (includeGlyphInfo)
    {
      int glyph = (*glyphs)[iter->first];
      node->AddProperty(_T("gn"), wxString::Format(_T("%d"), glyph));
    }
    node->AddProperty(_T("width"), wxString::Format(_T("%d"), iter->second));
    widthsNode->AddChild(node);
  }
  rootNode->AddChild(widthsNode);
  fontMetrics.SetRoot(rootNode);
  fontMetrics.Save(xmlFileName);
}

/// Make wxPdfDocument font metrics file based on AFM file
bool
MakeFontAFM(const wxString& fontFileName, const wxString& afmFileName,
            const wxString& encoding = _T("cp1252"),
            const wxString& type = _T("TrueType"),
            const wxString& patch = wxEmptyString)
{
  bool valid = false;
  wxString fontType = type;
  wxFileName fileName(fontFileName);
  // Find font type
  if (fontFileName.Length() > 0)
  {
    wxString ext = fileName.GetExt();
    if (ext == _T("ttf"))
    {
      fontType = _T("TrueType");
    }
    else if (ext == _T("pfb"))
    {
      fontType = _T("Type1");
    }
    else
    {
      wxLogMessage(_T("Warning: Unrecognized font file extension (") + ext + _T("), ") + fontType + _T(" font assumed."));
    }
  }

  bool embeddingAllowed = false;
  if (fontFileName.Length() > 0)
  {
    int cffOffset, cffLength;
    embeddingAllowed = (fontType == _T("TrueType")) ? CheckTTF(fontFileName, cffOffset, cffLength) : true;
  }

  wxPdfFont* afmFont;
  if (fontType == _T("TrueType"))
  {
    afmFont = new wxPdfFontTrueType(0);
  }
  else
  {
    afmFont = new wxPdfFontType1(0);
  }
  // Initialize font description
  wxPdfFontDescription fd;
  fd.SetAscent(1000);
  fd.SetDescent(-200);
  fd.SetItalicAngle(0);
  fd.SetStemV(70);
  fd.SetMissingWidth(600);
  fd.SetUnderlinePosition(-100);
  fd.SetUnderlineThickness(50);
  
  bool hasCapHeight = false;
  bool hasXCapHeight = false;
  bool hasXHeight = false;
  bool hasFontBBox = false;
  bool hasStemV = false;
  bool hasMissingWidth = false;
  int flags = 0;

  CTGMap cc2gn;
  CTGMap cc2gnPatch;
  if (encoding.Length() > 0)
  {
    afmFont->SetEncoding(encoding);
    valid = ReadMap(encoding, cc2gn);
    if (!valid)
    {
      delete afmFont;
      return false;
    }
    if (patch.Length() > 0)
    {
      valid = ReadMap(patch, cc2gnPatch);
      if (valid)
      {
        CTGMap::iterator patchIter;
        for (patchIter = cc2gnPatch.begin(); patchIter != cc2gnPatch.end(); patchIter++)
        {
          cc2gn[patchIter->first] = patchIter->second;
        }
      }
      else
      {
        wxLogMessage(_T("Warning: Unable to read patch file '") + patch + _T("'."));
      }
    }
  }

  // Read a font metric file
  wxFileInputStream afmFile(afmFileName);
  if (!afmFile.Ok())
  {
    wxLogMessage(_T("Error: Unable to read AFM file '") + afmFileName + _T("'."));
    delete afmFont;
    return false;
  }
  wxTextInputStream text(afmFile);

  FixGlyphMap fix;

  fix[_T("Edot")] = _T("Edotaccent");
  fix[_T("edot")] = _T("edotaccent");
  fix[_T("Idot")] = _T("Idotaccent");
  fix[_T("Zdot")] = _T("Zdotaccent");
  fix[_T("zdot")] = _T("zdotaccent");
  fix[_T("Odblacute")] = _T("Ohungarumlaut");
  fix[_T("odblacute")] = _T("ohungarumlaut");
  fix[_T("Udblacute")] = _T("Uhungarumlaut");
  fix[_T("udblacute")] = _T("uhungarumlaut");
  fix[_T("Gcedilla")] = _T("Gcommaaccent");
  fix[_T("gcedilla")] = _T("gcommaaccent");
  fix[_T("Kcedilla")] = _T("Kcommaaccent");
  fix[_T("kcedilla")] = _T("kcommaaccent");
  fix[_T("Lcedilla")] = _T("Lcommaaccent");
  fix[_T("lcedilla")] = _T("lcommaaccent");
  fix[_T("Ncedilla")] = _T("Ncommaaccent");
  fix[_T("ncedilla")] = _T("ncommaaccent");
  fix[_T("Rcedilla")] = _T("Rcommaaccent");
  fix[_T("rcedilla")] = _T("rcommaaccent");
  fix[_T("Scedilla")] = _T("Scommaaccent");
  fix[_T("scedilla")] = _T("scommaaccent");
  fix[_T("Tcedilla")] = _T("Tcommaaccent");
  fix[_T("tcedilla")] = _T("tcommaaccent");
  fix[_T("Dslash")] = _T("Dcroat");
  fix[_T("dslash")] = _T("dcroat");
  fix[_T("Dmacron")] = _T("Dcroat");
  fix[_T("dmacron")] = _T("dcroat");
  fix[_T("combininggraveaccent")] = _T("gravecomb");
  fix[_T("combininghookabove")] = _T("hookabovecomb");
  fix[_T("combiningtildeaccent")] = _T("tildecomb");
  fix[_T("combiningacuteaccent")] = _T("acutecomb");
  fix[_T("combiningdotbelow")] = _T("dotbelowcomb");
  fix[_T("dongsign")] = _T("dong");

  // Read the AFM font metric file
  wxPdfCharWidthMap* widths = new wxPdfCharWidthMap();
  wxPdfCharWidthMap* glyphs = new wxPdfCharWidthMap();
  long cc;
  if (!cc2gn.empty())
  {
    for (cc = 0; cc <= 255; cc++)
    {
      (*widths)[cc] = -1;
      (*glyphs)[cc] = 0;
    }
  }

  wxString line;
  wxString charcode, glyphname;
  wxString code, param, dummy, glyph;
  wxString token, tokenBoxHeight;
  long nParam, incWidth = -1, incGlyph = 0;
  long width, boxHeight, glyphNumber;
  wxString weight;
  bool hasGlyphNumbers = false;
  while (!afmFile.Eof())
  {
    line = text.ReadLine();
    line.Trim();

    wxStringTokenizer tkz(line, wxT(" "));
    int count = tkz.CountTokens();
    if (count < 2) continue;
    code  = tkz.GetNextToken(); // 0
    param = tkz.GetNextToken(); // 1
    if (code == _T("C"))
    {
      width = -1;
      glyphNumber = 0;
      boxHeight = 0;
      tokenBoxHeight = wxEmptyString;
      // Character metrics
      param.ToLong(&cc);
      dummy = tkz.GetNextToken(); // 2
      while (tkz.HasMoreTokens())
      {
        token = tkz.GetNextToken();
        if (token == _T("WX")) // Character width
        {
          param = tkz.GetNextToken(); // Width
          param.ToLong(&width);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == _T("N")) // Glyph name
        {
          glyphname = tkz.GetNextToken(); // Glyph name
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == _T("G")) // Glyph number
        {
          hasGlyphNumbers = true;
          param = tkz.GetNextToken(); // Number
          param.ToLong(&glyphNumber);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == _T("B")) // Character bounding box
        {
          dummy = tkz.GetNextToken(); // x left
          dummy = tkz.GetNextToken(); // y bottom
          dummy = tkz.GetNextToken(); // x right
          tokenBoxHeight = tkz.GetNextToken(); // y top
          tokenBoxHeight.ToLong(&boxHeight);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else
        {
          while (tkz.HasMoreTokens() && tkz.GetNextToken() != _T(";"));
        }
      }

      if (glyphname.Right(4) == _T("20AC"))
      {
        glyphname = _T("Euro");
      }
      if (glyphname == _T("increment"))
      {
        incWidth = width;
        if (hasGlyphNumbers)
        {
          incGlyph = glyphNumber;
        }
      }
      if (fix.find(glyphname) != fix.end())
      {
        //Fix incorrect glyph name
        for (int i = 0; i <= 255; i++)
        {
          if (cc2gn[i] == fix[glyphname])
          {
            cc2gn[i] = glyphname;
          }
        }
      }
      if (cc2gn.empty())
      {
        // Symbolic font: use built-in encoding
        (*widths)[cc] = width;
        if (hasGlyphNumbers)
        {
          (*glyphs)[cc] = glyphNumber;
        }
        if (!hasCapHeight && !hasXCapHeight && cc == 'X' && tokenBoxHeight != wxEmptyString)
        {
          hasXCapHeight = true;
          fd.SetCapHeight(boxHeight);
        }
        if (!hasXHeight && cc == 'x' && tokenBoxHeight != wxEmptyString)
        {
          hasXHeight = true;
          fd.SetXHeight(boxHeight);
        }
      }
      else
      {
        // Search glyphname in cc2gn
        bool found = false;
        for (int i = 0; i <= 255; i++)
        {
          if (cc2gn[i] == glyphname)
          {
            found = true;
            (*widths)[i] = width;
            if (hasGlyphNumbers)
            {
              (*glyphs)[i] = glyphNumber;
            }
          }
        }
        if (!found)
        {
          wxLogMessage(_T("Warning: character '") + glyphname + _T("' is undefined."));
        }
        if (!hasCapHeight && !hasXCapHeight && glyphname == _T("X") && tokenBoxHeight != wxEmptyString)
        {
          hasXCapHeight = true;
          fd.SetCapHeight(boxHeight);
        }
        if (!hasXHeight && glyphname == _T("x") && tokenBoxHeight != wxEmptyString)
        {
          hasXHeight = true;
          fd.SetXHeight(boxHeight);
        }
      }
      if (!hasMissingWidth && glyphname == _T(".notdef"))
      {
        hasMissingWidth = true;
        fd.SetMissingWidth(width);
      }
    }
    else if (code == _T("FontName"))
    {
      afmFont->SetName(param);
    }
    else if (code == _T("Weight"))
    {
      wxString weight = param.Lower();
      if (!hasStemV && (weight == _T("black") || weight == _T("bold")))
      {
        fd.SetStemV(120);
      }
    }
    else if (code == _T("ItalicAngle"))
    {
      double italic;
      param.ToDouble(&italic);
      int italicAngle = int(italic);
      fd.SetItalicAngle(italicAngle);
      if (italicAngle > 0)
      {
        flags += 1 << 6;
      }
    }
    else if (code == _T("Ascender"))
    {
      long ascent;
      param.ToLong(&ascent);
      fd.SetAscent(ascent);
    }
    else if (code == _T("Descender"))
    {
      param.ToLong(&nParam);
      fd.SetDescent(nParam);
    }
    else if (code == _T("UnderlineThickness"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlineThickness(nParam);
    }
    else if (code == _T("UnderlinePosition"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlinePosition(nParam);
    }
    else if (code == _T("IsFixedPitch"))
    {
      if (param == _T("true"))
      {
        flags += 1 << 0;
      }
    }
    else if (code == _T("FontBBox"))
    {
      hasFontBBox = true;
      wxString bbox2 = tkz.GetNextToken();
      wxString bbox3 = tkz.GetNextToken();
      wxString bbox4 = tkz.GetNextToken();
      wxString bBox = _T("[") + param + _T(" ") + bbox2 + _T(" ") + bbox3 + _T(" ") + bbox4 + _T("]");
      fd.SetFontBBox(bBox);
    }
    else if (code == _T("CapHeight"))
    {
      hasCapHeight = true;
      long capHeight;
      param.ToLong(&capHeight);
      fd.SetCapHeight(capHeight);
    }
    else if (code == _T("StdVW"))
    {
      hasStemV = true;
      long stemV;
      param.ToLong(&stemV);
      fd.SetStemV(stemV);
    }
  }

  for (cc = 0; cc <= 255; cc++)
  {
    if ((*widths)[cc] == -1)
    {
      if (cc2gn[cc] == _T("Delta") && incWidth >= 0)
      {
        (*widths)[cc] = incWidth;
        if (hasGlyphNumbers)
        {
          (*glyphs)[cc] = incGlyph;
        }
      }
      else
      {
        (*widths)[cc] = fd.GetMissingWidth();
        if (hasGlyphNumbers)
        {
          (*glyphs)[cc] = 0;
        }
        wxLogMessage(_T("Warning: character '") + cc2gn[cc] + _T("' is missing."));
      }
    }
  }

  wxString diffs;
  if (encoding.Length() > 0)
  {
    // Build differences from reference encoding
    CTGMap cc2gnRef;
    valid = ReadMap(_T("cp1252"), cc2gnRef);
    diffs = _T("");
    int last = 0;
    for (int i = 32; i <= 255; i++)
    {
      if (cc2gn[i] != cc2gnRef[i])
      {
        if (i != last+1)
        {
          diffs += wxString::Format(_T("%d "), i);
        }
        last = i;
        diffs = diffs + _T("/") + cc2gn[i] + _T(" ");
      }
    }
  }
  afmFont->SetDiffs(diffs);  
  
  if (cc2gn.empty())
  {
    flags += 1 << 2;
  }
  else
  {
    flags += 1 << 5;
  }
  fd.SetFlags(flags);
  if (!hasCapHeight && !hasXCapHeight)
  {
    fd.SetCapHeight(fd.GetAscent());
  }
  if (!hasFontBBox)
  {
    wxString fbb = wxString::Format(_T("[0 %d 1000 %d]"), fd.GetDescent()-100, fd.GetAscent()+100);
    fd.SetFontBBox(fbb);
  }

  afmFont->SetCharWidthMap(widths);
  afmFont->SetGlyphNumberMap(glyphs);

  afmFont->SetDesc(fd);

  wxString baseName = fileName.GetName();
  if (embeddingAllowed)
  {
    if (fontType == _T("TrueType"))
    {
      wxFileInputStream ttfFile(fontFileName);
      if (!ttfFile.Ok())
      {
        wxLogMessage(_T("Error: Unable to read font file '") + fontFileName + _T("'."));
        delete afmFont;
        return false;
      }
      size_t len = ttfFile.GetLength();
      wxFileOutputStream outFontFile(baseName + _T(".z"));
      wxZlibOutputStream zOutFontFile(outFontFile);
      zOutFontFile.Write(ttfFile);
      zOutFontFile.Close();
      afmFont->SetFontFile(baseName + _T(".z"));
      afmFont->SetSize1(len);
      wxLogMessage(_T("Font file compressed (") + baseName + _T(".z)."));
    }
    else
    {
      wxFileInputStream pfbFile(fontFileName);
      if (!pfbFile.Ok())
      {
        wxLogMessage(_T("Error: Unable to read font file '") + fontFileName + _T("'."));
        delete afmFont;
        return false;
      }
      size_t len = pfbFile.GetLength();
      // Find first two sections and discard third one
      unsigned char* buffer = new unsigned char[len];
      unsigned char* buf1 = buffer;
      unsigned char* buf2;
      pfbFile.Read(buffer, len);
      unsigned char first = buffer[0];
      if (first == 128)
      {
        // Strip first binary header
        buf1 += 6;
        len -= 6;
      }
      int* f = makeFail("eexec",5);
      int size1 = findString((char*) buf1, len, "eexec", 5, f);
      delete [] f;

      int size2 = -1;
      if (size1 >= 0)
      {
        size1 += 6;
        unsigned char second = buf1[size1];
        buf2 = buf1 + size1;
        len -= size1;
        if (first == 128 && second == 128)
        {
          // Strip second binary header
          buf2 += 6;
          len -= 6;
        }
        f = makeFail("00000000",8);
        size2 = findString((char*) buf2, len, "00000000", 8, f);
        delete [] f;
        if (size2 >= 0)
        {
          wxFileOutputStream outFontFile(baseName + _T(".z"));
          wxZlibOutputStream zOutFontFile(outFontFile);
          zOutFontFile.Write(buf1, size1);
          zOutFontFile.Write(buf2, size2);
          afmFont->SetFontFile(baseName + _T(".z"));
          afmFont->SetSize1(size1);
          afmFont->SetSize2(size2);
        }
      }
      delete [] buffer;
      if (size1 < 0 || size2 < 0)
      {
        wxLogMessage(_T("Warning: Font file does not seem to be valid Type1, font embedding not possible."));
      }
    }
  }
  else
  {
    if (fontFileName.Length() > 0)
    {
      wxLogMessage(_T("Warning: Font license does not allow embedding."));
    }
    else
    {
      wxLogMessage(_T("Warning: Font file name missing, font embedding not possible."));
    }
  }

  // Create XML file
  CreateFontMetricsFile(baseName + _T(".xml"), *afmFont, hasGlyphNumbers);
  wxLogMessage(_T("Font definition file generated (") + baseName + _T(".xml)."));

  //delete widths;
  delete afmFont;
  return true;
}

#if wxUSE_UNICODE

void
WriteStreamBuffer(wxOutputStream& stream, const char* buffer)
{
  size_t buflen = strlen(buffer);
  stream.Write(buffer, buflen);
}

void
WriteToUnicode(GlyphList& glyphs, wxMemoryOutputStream& toUnicode)
{
  WriteStreamBuffer(toUnicode, "/CIDInit /ProcSet findresource begin\n");
  WriteStreamBuffer(toUnicode, "12 dict begin\n");
  WriteStreamBuffer(toUnicode, "begincmap\n");
  WriteStreamBuffer(toUnicode, "/CIDSystemInfo\n");
  WriteStreamBuffer(toUnicode, "<< /Registry (Adobe)\n");
  WriteStreamBuffer(toUnicode, "/Ordering (UCS)\n");
  WriteStreamBuffer(toUnicode, "/Supplement 0\n");
  WriteStreamBuffer(toUnicode, ">> def\n");
  WriteStreamBuffer(toUnicode, "/CMapName /Adobe-Identity-UCS def\n");
  WriteStreamBuffer(toUnicode, "/CMapType 2 def\n");
  WriteStreamBuffer(toUnicode, "1 begincodespacerange\n");
  WriteStreamBuffer(toUnicode, "<0000><FFFF>\n");
  WriteStreamBuffer(toUnicode, "endcodespacerange\n");
  int size = 0;
  size_t k;
  size_t numGlyphs = glyphs.GetCount();
  for (k = 0; k < numGlyphs; ++k)
  {
    if (size == 0)
    {
      if (k != 0)
      {
        WriteStreamBuffer(toUnicode, "endbfrange\n");
      }
      size = (numGlyphs-k > 100) ? 100 : numGlyphs - k;
      wxString sizeStr = wxString::Format(_T("%d"), size);
      WriteStreamBuffer(toUnicode, sizeStr.ToAscii());
      WriteStreamBuffer(toUnicode, " beginbfrange\n");
    }
    size--;
    GlyphListEntry* entry = glyphs[k];
    wxString fromTo = wxString::Format(_T("<%04x>"), entry->m_gid);
    wxString uniChr = wxString::Format(_T("<%04x>"), entry->m_uid);
    WriteStreamBuffer(toUnicode, fromTo.ToAscii());
    WriteStreamBuffer(toUnicode, fromTo.ToAscii());
    WriteStreamBuffer(toUnicode, uniChr.ToAscii());
    WriteStreamBuffer(toUnicode, "\n");
  }
  WriteStreamBuffer(toUnicode, "endbfrange\n");
  WriteStreamBuffer(toUnicode, "endcmap\n");
  WriteStreamBuffer(toUnicode, "CMapName currentdict /CMap defineresource pop\n");
  WriteStreamBuffer(toUnicode, "end end\n");
}

/// Make wxPdfDocument font metrics file based on UFM file
bool
MakeFontUFM(const wxString& fontFileName,
            const wxString& ufmFileName,
            const wxString& type = _T("TrueType"))
{
  bool cff = false;
  GlyphList glyphList(CompareGlyphListEntries);
  static size_t CC2GNSIZE = 131072; // 2*64kB
  wxFileName fileName(ufmFileName);
  bool embeddingAllowed = false;
  int cffOffset = -1;
  int cffLength = 0;
  if (fontFileName.Length() > 0)
  {
    embeddingAllowed = CheckTTF(fontFileName, cffOffset, cffLength);
    cff = cffOffset > 0;
  }
  else
  {
    cff = (type == _T("OpenType"));
  }

  wxPdfFont* ufmFont;
  if (cff)
  {
    ufmFont = new wxPdfFontOpenTypeUnicode(0);
  }
  else
  {
    ufmFont = new wxPdfFontTrueTypeUnicode(0);
  }

  // Initialize font description
  wxPdfFontDescription fd;
  fd.SetAscent(1000);
  fd.SetDescent(-200);
  fd.SetItalicAngle(0);
  fd.SetStemV(70);
  fd.SetMissingWidth(600);
  bool hasCapHeight = false;
  bool hasXCapHeight = false;
  bool hasXHeight = false;
  bool hasFontBBox = false;
  bool hasStemV = false;
  bool hasMissingWidth = false;
  int flags = 0;

  wxFileInputStream ufmFile(ufmFileName);
  if (!ufmFile.Ok())
  {
    wxLogMessage(_T("Error: Unable to read UFM file '") + ufmFileName + _T("'."));
    return false;
  }
  wxTextInputStream text(ufmFile);

  // Prepare empty CIDToGIDMap
  unsigned char* cc2gn = new unsigned char[131072];
  size_t j;
  for (j = 0; j < CC2GNSIZE; j++)
  {
    cc2gn[j] = '\0';
  }
  
  // Read the UFM font metric file
  wxPdfCharWidthMap* widths = new wxPdfCharWidthMap();
  wxPdfCharWidthMap* glyphs = new wxPdfCharWidthMap();

  wxString line;
  wxString charcode, glyphname;
  wxString code, param, dummy, glyph;
  wxString token, tokenBoxHeight;
  long nParam, width, glyphNumber, boxHeight;
  wxString weight;
  while (!ufmFile.Eof())
  {
    line = text.ReadLine();
    line.Trim();

    wxStringTokenizer tkz(line, wxT(" "));
    int count = tkz.CountTokens();
    if (count < 2) continue;
    code  = tkz.GetNextToken(); // 0
    param = tkz.GetNextToken(); // 1
    if (code == _T("U"))
    {
      long cc;
      param.ToLong(&cc); // Character code
      width = -1;
      glyphNumber = 0;
      boxHeight = 0;
      tokenBoxHeight = wxEmptyString;
      dummy = tkz.GetNextToken(); // Semicolon
      while (tkz.HasMoreTokens())
      {
        token = tkz.GetNextToken();
        if (token == _T("WX")) // Character width
        {
          param = tkz.GetNextToken(); // Width
          param.ToLong(&width);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == _T("N")) // Glyph name
        {
          glyphname = tkz.GetNextToken(); // Glyph name
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == _T("G")) // Glyph number
        {
          param = tkz.GetNextToken(); // Number
          param.ToLong(&glyphNumber);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == _T("B")) // Character bounding box
        {
          dummy = tkz.GetNextToken(); // x left
          dummy = tkz.GetNextToken(); // y bottom
          dummy = tkz.GetNextToken(); // x right
          tokenBoxHeight = tkz.GetNextToken(); // y top
          tokenBoxHeight.ToLong(&boxHeight);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else
        {
          while (tkz.HasMoreTokens() && tkz.GetNextToken() != _T(";"));
        }
      }
      if (cc != -1)
      {
        if (!hasCapHeight && !hasXCapHeight && cc == 'X' && tokenBoxHeight != wxEmptyString)
        {
          hasXCapHeight = true;
          fd.SetCapHeight(boxHeight);
        }
        if (!hasXHeight && cc == 'x' && tokenBoxHeight != wxEmptyString)
        {
          hasXHeight = true;
          fd.SetXHeight(boxHeight);
        }

        (*widths)[cc] = width;
        (*glyphs)[cc] = glyphNumber;
        // Set GID
        if (cc >= 0 && cc < 0xFFFF)
        {
          cc2gn[2*cc]   = (glyphNumber >> 8) & 0xFF;
          cc2gn[2*cc+1] =  glyphNumber       & 0xFF;
        }
        if (cff)
        {
          GlyphListEntry* glEntry = new GlyphListEntry();
          glEntry->m_gid = glyphNumber;
          glEntry->m_uid = cc;
          glyphList.Add(glEntry);
        }
      }
      if (!hasMissingWidth && glyphname == _T(".notdef"))
      {
        hasMissingWidth = true;
        fd.SetMissingWidth(width);
      }
    }
    else if (code == _T("FontName"))
    {
      ufmFont->SetName(param);
    }
    else if (code == _T("Weight"))
    {
      wxString weight = param.Lower();
      if (!hasStemV && (weight == _T("black") || weight == _T("bold")))
      {
        fd.SetStemV(120);
      }
    }
    else if (code == _T("ItalicAngle"))
    {
      double italic;
      param.ToDouble(&italic);
      int italicAngle = int(italic);
      fd.SetItalicAngle(italicAngle);
      if (italicAngle > 0)
      {
        flags += 1 << 6;
      }
    }
    else if (code == _T("Ascender"))
    {
      long ascent;
      param.ToLong(&ascent);
      fd.SetAscent(ascent);
    }
    else if (code == _T("Descender"))
    {
      param.ToLong(&nParam);
      fd.SetDescent(nParam);
    }
    else if (code == _T("UnderlineThickness"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlineThickness(nParam);
    }
    else if (code == _T("UnderlinePosition"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlinePosition(nParam);
    }
    else if (code == _T("IsFixedPitch"))
    {
      if (param == _T("true"))
      {
        flags += 1 << 0;
      }
    }
    else if (code == _T("FontBBox"))
    {
      hasFontBBox = true;
      wxString bbox2 = tkz.GetNextToken();
      wxString bbox3 = tkz.GetNextToken();
      wxString bbox4 = tkz.GetNextToken();
      wxString bBox = _T("[") + param + _T(" ") + bbox2 + _T(" ") + bbox3 + _T(" ") + bbox4 + _T("]");
      fd.SetFontBBox(bBox);
    }
    else if (code == _T("CapHeight"))
    {
      hasCapHeight = true;
      long capHeight;
      param.ToLong(&capHeight);
      fd.SetCapHeight(capHeight);
    }
    else if (code == _T("StdVW"))
    {
      hasStemV = true;
      long stemV;
      param.ToLong(&stemV);
      fd.SetStemV(stemV);
    }
  }
  flags += 1 << 5;
  fd.SetFlags(flags);
  if (!hasCapHeight && !hasXCapHeight)
  {
    fd.SetCapHeight(fd.GetAscent());
  }
  if (!hasFontBBox)
  {
    hasFontBBox = true;
    wxString fbb = wxString::Format(_T("[0 %d 1000 %d]"), fd.GetDescent()-100, fd.GetAscent()+100);
    fd.SetFontBBox(fbb);
  }

  ufmFont->SetCharWidthMap(widths);
  ufmFont->SetGlyphNumberMap(glyphs);

  ufmFont->SetDesc(fd);

  wxString baseName = fileName.GetName();
  if (embeddingAllowed)
  {
    wxFileInputStream fontFile(fontFileName);
    if (!fontFile.Ok())
    {
      wxLogMessage(_T("Error: Unable to read font file '") + fontFileName + _T("'."));
      delete [] cc2gn;
      return false;
    }
    size_t len = fontFile.GetLength();
    wxFileOutputStream outFontFile(baseName + _T(".z"));
    wxZlibOutputStream zOutFontFile(outFontFile);
    if (cff)
    {
      char* buffer = new char[cffLength];
      fontFile.SeekI(cffOffset);
      fontFile.Read(buffer, cffLength);
      zOutFontFile.Write(buffer, cffLength);
      delete buffer;
    }
    else
    {
      zOutFontFile.Write(fontFile);
    }
    zOutFontFile.Close();
    ufmFont->SetFontFile(baseName + _T(".z"));
    ufmFont->SetSize1(len);
    wxLogMessage(_T("Font file compressed (") + baseName + _T(".z)."));
  }
  else
  {
    if (fontFileName.Length() > 0)
    {
      wxLogMessage(_T("Warning: Font license does not allow embedding."));
    }
    else
    {
      wxLogMessage(_T("Warning: Font file name missing, font embedding not possible."));
    }
  }

  if (embeddingAllowed)
  {
    // Create CID to GID map file
    wxFileOutputStream outCtgFile(baseName + _T(".ctg.z"));
    wxZlibOutputStream zOutCtgFile(outCtgFile);
    if (cff)
    {
      wxMemoryOutputStream toUnicode;
      WriteToUnicode(glyphList, toUnicode);
      wxMemoryInputStream inUnicode(toUnicode);
      zOutCtgFile.Write(inUnicode);
    }
    else
    {
      zOutCtgFile.Write(cc2gn, CC2GNSIZE);
    }
    zOutCtgFile.Close();
    ufmFont->SetCtgFile(baseName + _T(".ctg.z"));
    wxLogMessage(_T("CIDToGIDMap created and compressed (") + baseName + _T(".ctg.z)"));
  }

  // Create XML file
  CreateFontMetricsFile(baseName + _T(".xml"), *ufmFont, cff);
  wxLogMessage(_T("Font definition file generated (") + baseName + _T(".xml)."));

  if (cff)
  {
    WX_CLEAR_ARRAY(glyphList);
  }
  //delete widths;
  delete [] cc2gn;
  delete ufmFont;

  return true;
}

#endif // wxUSE_UNICODE

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
  { wxCMD_LINE_OPTION, _T("a"), _T("afm"),   _T("Adobe Font Metric file (AFM)"),                    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#if wxUSE_UNICODE
  { wxCMD_LINE_OPTION, _T("u"), _T("ufm"),   _T("Unicode Font Metric file (UFM)"),                  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, _T("f"), _T("font"),  _T("font file (ttf, otf or pfb)"),                     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#else
  { wxCMD_LINE_OPTION, _T("f"), _T("font"),  _T("font file (ttf or pfb)"),                          wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#endif
  { wxCMD_LINE_OPTION, _T("e"), _T("enc"),   _T("Character encoding of the font"),                  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, _T("p"), _T("patch"), _T("Patch file"),                                      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, _T("t"), _T("type"),  _T("Font type (type = otf, ttf or t1, default: ttf)"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

  { wxCMD_LINE_NONE }
};

class MakeFont : public wxAppConsole
{
public:
  bool OnInit();
  int OnRun();
  int OnExit();
private:
  wxString m_version;
  bool     m_unicode;
  wxString m_afmFile;
  wxString m_ufmFile;
  wxString m_fontFile;
  wxString m_encoding;
  wxString m_patchFile;
  wxString m_fontType;
};

bool
MakeFont::OnInit()
{
  m_version = _T("1.2.0 (December 2006)");
  bool valid = false;
  //gets the parameters from cmd line
  wxCmdLineParser parser (cmdLineDesc, argc, argv);
  parser.SetLogo(_T("wxPdfDocument MakeFont Utility Version ") + m_version + _T("\n\nCreate wxPdfDocument font support files.\nPlease specify file extensions in ALL file name parameters.\n"));
  if (parser.Parse() == 0)
  {
    bool hasAfm   = parser.Found(_T("afm"),   &m_afmFile);
    bool hasUfm   = parser.Found(_T("ufm"),   &m_ufmFile);
    bool hasFont  = parser.Found(_T("font"),  &m_fontFile);
    bool hasEnc   = parser.Found(_T("enc"),   &m_encoding);
    bool hasPatch = parser.Found(_T("patch"), &m_patchFile);
    bool hasType  = parser.Found(_T("type"),  &m_fontType);

#if wxUSE_UNICODE
    m_unicode = hasUfm;
#else
    hasUfm = false;
    m_unicode = false;
#endif
    valid = (hasAfm && !hasUfm) || (!hasAfm && hasUfm);
    if (valid)
    {
      if (hasAfm)
      {
        m_fontType = (hasType && m_fontType == _T("t1")) ? _T("Type1") : _T("TrueType");
      }
      else if (hasUfm)
      {
        m_fontType = (hasType && m_fontType == _T("otf")) ? _T("OpenType") : _T("TrueType");
      }
    }
    else
    {
      parser.Usage();
    }
  }
  return valid;
}

int
MakeFont::OnExit()
{
  return 0;
}

int
MakeFont::OnRun()
{
  bool valid;
  wxSetWorkingDirectory(wxGetCwd() + _T("/../makefont"));
#if 0
  // Set the font path
  wxString fontPath = wxGetCwd() + _T("/../lib/fonts");
  wxSetEnv(_T("WXPDF_FONTPATH"), fontPath);
#endif

  wxLogMessage(_T("wxPdfDocument MakeFont Utility Version ") + m_version);
  wxLogMessage(_T("*** Starting to create font support files for ..."));
#if wxUSE_UNICODE
  if (m_unicode)
  {
    wxLogMessage(_T("  UFM file : ") + m_ufmFile);
    wxLogMessage(_T("  Font file: ") + m_fontFile);
    valid = MakeFontUFM(m_fontFile, m_ufmFile, m_fontType);
  }
  else
#endif
  {
    wxLogMessage(_T("  AFM file : ") + m_afmFile);
    wxLogMessage(_T("  Font file: ") + m_fontFile);
    wxLogMessage(_T("  Encoding : ") + m_encoding);
    wxLogMessage(_T("  Font type: ") + m_fontType);
    valid = MakeFontAFM(m_fontFile, m_afmFile, m_encoding, m_patchFile);
  }
  wxLogMessage(_T("*** wxPdfDocument MakeFont finished."));

  return 0;
}

IMPLEMENT_APP_CONSOLE(MakeFont)
