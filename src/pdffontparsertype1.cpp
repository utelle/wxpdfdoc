///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontparsertype1.cpp
// Purpose:     
// Author:      Ulrich Telle
// Created:     2008-07-26
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontparsertype1.cpp Implementation of Type1 font parsing support classes

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_UNICODE

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes
#include <wx/filename.h>
#include <wx/filesys.h>
#include "wx/tokenzr.h"
#include "wx/txtstrm.h"
#include <wx/zstream.h>

#include "wx/pdfcffindex.h"
#include "wx/pdfcffdecoder.h"
#include "wx/pdffontdata.h"
#include "wx/pdffontdatatype1.h"
#include "wx/pdffontparsertype1.h"
#include "wx/pdfencoding.h"
#include "wx/pdfutility.h"

// --- Type1 font parser

#define PFB_BLOCK_UNKNOWN 0x00
#define PFB_BLOCK_ASCII   0x01
#define PFB_BLOCK_BINARY  0x02
#define PFB_BLOCK_DONE    0x03
#define PFB_BLOCK_END     0x05
#define PFB_BLOCK_MARKER  0x80

wxPdfFontParserType1::wxPdfFontParserType1()
  : wxPdfFontParser()
{
  m_isPFB = false;
  m_privateFound = false;
  m_fontDirAfterPrivate = false;
  m_privateDict = NULL;
  m_lenIV = 4;
  m_charStringsIndex = new wxPdfCffIndexArray();
  m_subrsIndex = new wxPdfCffIndexArray();
  m_glyphWidthMap = NULL;
}

wxPdfFontParserType1::~wxPdfFontParserType1()
{
  if (m_privateDict != NULL)
  {
    delete m_privateDict;
  }
  delete m_charStringsIndex;
  delete m_subrsIndex;
  if (m_glyphWidthMap != NULL)
  {
    delete m_glyphWidthMap;
  }
}

bool
wxPdfFontParserType1::ParseFont(const wxString& fileName, wxInputStream* fontFile, wxInputStream* metricFile, bool onlyNames)
{
  bool ok = false;
  m_fileName = fileName;
  if (fontFile != NULL && metricFile != NULL)
  {
    ok = ReadPFX(fontFile, onlyNames);
    if (ok && !onlyNames)
    {
      ok = false;
      if (MetricIsAFM(metricFile))
      {
        ok = ReadAFM(*metricFile);
      }
      else if (MetricIsPFM(metricFile))
      {
        ok = ReadPFM(*metricFile);
      }
      if (ok)
      {
        // Initialize char width array
        // TODO: Handle different Type1 encodings
        wxArrayString glyphNames;
        if (!m_encoding.IsSameAs(wxS("ArrayEncoding")))
        {
          wxPdfEncoding encoding;
          if (m_encoding.IsSameAs(wxS("StandardEncoding")))
          {
            encoding.SetEncoding(wxS("standard"));
          }
          else if (m_encoding.IsSameAs(wxS("ISOLatin1Encoding")))
          {
            encoding.SetEncoding(wxS("isolatin1"));
          }
          else if (m_encoding.IsSameAs(wxS("ExpertEncoding")))
          {
            encoding.SetEncoding(wxS("winansi"));
          }
          m_fontData->SetEncoding(wxS("cp-1252"));
          glyphNames = encoding.GetGlyphNames();
        }
        else
        {
          m_fontData->SetEncoding(wxEmptyString);
          glyphNames = m_encodingVector;
        }
        wxPdfGlyphWidthMap* widthMap = new wxPdfGlyphWidthMap();
        wxPdfFontType1GlyphWidthMap::const_iterator glyphIter;
        wxString glyph;
        size_t n = glyphNames.GetCount();
        size_t j;
        for (j = 0; j < n; ++j)
        {
          glyph = glyphNames[j];
          glyphIter = m_glyphWidthMap->find(glyph);
          if (glyphIter != m_glyphWidthMap->end())
          {
            (*widthMap)[j] = (wxUint16) glyphIter->second;
          }
          else
          {
            (*widthMap)[j] = (wxUint16) m_missingWidth;
          }
        }
        m_fontData->SetGlyphWidthMap(widthMap);
        m_fontData->SetType1GlyphWidthMap(m_glyphWidthMap);
        m_fontData->CreateDefaultEncodingConv();
        m_glyphWidthMap = NULL;
      }
    }
  }
  else
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::ParseFont: ")) +
               wxString::Format(_("'%s' is not a valid Type1 Font file."), m_fileName.c_str()));
  }

  return ok;
}

wxPdfFontData*
wxPdfFontParserType1::IdentifyFont(const wxString& fontFileName, int fontIndex)
{
  wxFileSystem fs;
  bool ok = true;
  wxPdfFontData* fontData = NULL;
  wxUnusedVar(fontIndex);
  m_fileName = fontFileName;
  wxFileName fileNameFont(fontFileName);

  // Check for existance of metric file
  wxFSFile* metricFile = NULL;
  wxFileName fileNameMetric(fontFileName);
  fileNameMetric.SetExt(wxS("afm"));
  if (fileNameMetric.IsFileReadable())
  {
    metricFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileNameMetric));
  }
  else
  {
    fileNameMetric.SetExt(wxS("pfm"));
    if (fileNameMetric.IsFileReadable())
    {
      metricFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileNameMetric));
    }
  }
  if (metricFile != NULL)
  {
    wxInputStream* metricStream = metricFile->GetStream();
    wxMemoryInputStream* pfbStream = NULL;

    // Open font file
    wxFSFile* fontFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileNameFont));
    if (fontFile != NULL)
    {
      if (fileNameFont.GetExt().IsEmpty())
      {
        wxMemoryOutputStream* pfbOutputStream = ConvertMACtoPFB(fontFile->GetStream());
        if (pfbOutputStream != NULL)
        {
          pfbStream = new wxMemoryInputStream(*pfbOutputStream);
          delete pfbOutputStream;
          m_inFont = pfbStream;
        }
        else
        {
          m_inFont = NULL;
        }
      }
      else
      {
        m_inFont = fontFile->GetStream();
      }
      if (m_inFont != NULL)
      {
        m_inFont->SeekI(0);

        // Identify single font
        m_fontData = new wxPdfFontDataType1(pfbStream);

        ok = ParseFont(fontFileName, m_inFont, metricStream, true);
        if (ok)
        {
          fontData = m_fontData;
          fontData->SetFontFileName(m_fileName);
          fontData->SetFontIndex(fontIndex);
        }
        else
        {
          delete m_fontData;
          m_fontData = NULL;
          wxLogError(wxString(wxS("wxPdfFontParserType1::IdentifyFont: ")) +
                     wxString::Format(_("Reading of font directory failed for font file '%s'."), fontFileName.c_str()));
        }
      }
      delete fontFile;
    }
    else
    {
      wxLogError(wxString(wxS("wxPdfFontParserType1::IdentifyFont: ")) +
                 wxString::Format(_("Font file '%s' not accessible."), fontFileName.c_str()));
    }
    delete metricFile;
  }
  else
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::IdentifyFont: ")) +
               wxString::Format(_("Metric file of font file '%s' not accessible."), fontFileName.c_str()));
  }
  return fontData;
}

bool
wxPdfFontParserType1::LoadFontData(wxPdfFontData* fontData)
{
  wxFileSystem fs;
  bool ok = false;
  if (fontData->GetType().IsSameAs(wxS("Type1")))
  {
    m_fileName = fontData->GetFontFileName();
    m_fontData = (wxPdfFontDataType1*) fontData;
    wxFileName fileNameFont(m_fileName);

    // Check for existance of metric file
    wxFSFile* metricFile = NULL;
    wxFileName fileNameMetric(m_fileName);
    fileNameMetric.SetExt(wxS("afm"));
    if (fileNameMetric.IsFileReadable())
    {
      metricFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileNameMetric));
    }
    else
    {
      fileNameMetric.SetExt(wxS("pfm"));
      if (fileNameMetric.IsFileReadable())
      {
        metricFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileNameMetric));
      }
    }
    if (metricFile != NULL)
    {
      wxInputStream* metricStream = metricFile->GetStream();

      // Open font file
      wxFSFile* fontFile = fs.OpenFile(wxFileSystem::FileNameToURL(fileNameFont));
      if (fontFile != NULL)
      {
        m_inFont = fontFile->GetStream();
        m_inFont->SeekI(0);
        ok = ParseFont(m_fileName, m_inFont, metricStream, false);
        delete fontFile;
      }
      else
      {
        wxLogError(wxString(wxS("wxPdfFontParserType1::LoadFontData: ")) +
                   wxString::Format(_("Font file '%s' not accessible."), m_fileName.c_str()));
      }
      delete metricFile;
    }
    else
    {
      wxLogError(wxString(wxS("wxPdfFontParserType1::LoadFontData: ")) +
                 wxString::Format(_("Metric file of font file '%s' not accessible."), m_fileName.c_str()));
    }
  }
  else
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::LoadFontData: ")) +
               wxString::Format(_("Type1 font parser called for wrong font type '%s'."), fontData->GetType().c_str()));
  }
  return ok;
}

class GlyphListEntry
{
public:
  GlyphListEntry() {};
  ~GlyphListEntry() {};
  int m_gid;
  int m_uid;
};

WX_DEFINE_SORTED_ARRAY(GlyphListEntry*, GlyphList);

WX_DECLARE_HASH_MAP(long, wxString, wxIntegerHash, wxIntegerEqual, CTGMap);
WX_DECLARE_STRING_HASH_MAP(wxString, FixGlyphMap);

bool
wxPdfFontParserType1::MetricIsAFM(wxInputStream* stream)
{
  bool ok = false;
  size_t fileSize = stream->GetSize();
  if (fileSize > 16)
  {
    wxString keyword;
    char buffer[16];
    stream->SeekI(0);
    stream->Read(buffer, 16);
    keyword = wxString::From8BitData(buffer, 16);
    ok = keyword.IsSameAs(wxS("StartFontMetrics"));
    stream->SeekI(0);
  }
  return ok;
}

bool
wxPdfFontParserType1::MetricIsPFM(wxInputStream* stream)
{
  bool ok = false;
  size_t fileSize = stream->GetSize();
  if (fileSize > 147)
  {
    stream->SeekI(2);
    size_t len = ReadUIntLE(stream);
    stream->SeekI(117);
    unsigned short extlen = ReadUShortLE(stream);
    stream->SeekI(139);
    unsigned int fontname = ReadUIntLE(stream);
    // TODO: in iText the fontname position is checked for <= 512. Why?
    // There exist fonts where fontname > 512.
    ok = (fileSize == len) && (extlen == 30) && (fontname >= 75) /* && (fontname <= 512) */;
    stream->SeekI(0);
  }
  return ok;
}

bool
wxPdfFontParserType1::ReadAFM(wxInputStream& afmFile)
{
  bool ok = false;
  if (afmFile.IsOk())
  {
    // Initialize font description
    wxPdfFontDescription fd;
    fd.SetAscent(1000);
    fd.SetDescent(-200);
    fd.SetItalicAngle(0);
    fd.SetStemV(70);
    m_missingWidth = 600;
    fd.SetMissingWidth(600);
    fd.SetUnderlinePosition(-100);
    fd.SetUnderlineThickness(50);

    wxPdfKernPairMap* kpMap = NULL;
    wxString encodingScheme;

    bool hasCapHeight = false;
    bool hasXCapHeight = false;
    bool hasXHeight = false;
    bool hasFontBBox = false;
    bool hasStemV = false;
    bool hasMissingWidth = false;
    int flags = 0;

    wxTextInputStream text(afmFile);
    wxString line;
    wxString charcode, glyphname;
    wxString code, param, dummy, glyph;
    wxString token, tokenBoxHeight;
    long nParam;
    long cc, width, boxHeight, glyphNumber;
    bool hasGlyphNumbers = false;

    bool inHeader = true;
    bool inMetrics = false;
    bool inKerning = false;
    while (!afmFile.Eof())
    {
      line = text.ReadLine();
      line.Trim();

      wxStringTokenizer tkz(line, wxS(" "));
      int count = tkz.CountTokens();
      if (count < 2) continue;
      code  = tkz.GetNextToken(); // 0
      param = tkz.GetNextToken(); // 1
      if (inHeader)
      {
        if (code.IsSameAs(wxS("FontName")))
        {
          m_fontData->SetName(param);
        }
        else if (code.IsSameAs(wxS("FullName")))
        {
          wxArrayString fullNames;
          fullNames.Add(param);
          m_fontData->SetFullNames(fullNames);
        }
        else if (code.IsSameAs(wxS("FamilyName")))
        {
          m_fontData->SetFamily(param);
        }
        else if (code.IsSameAs(wxS("Weight")))
        {
          wxString weight = param.Lower();
          if (!hasStemV && (weight.IsSameAs(wxS("black")) || weight.IsSameAs(wxS("bold"))))
          {
            fd.SetStemV(120);
          }
        }
        else if (code.IsSameAs(wxS("ItalicAngle")))
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
        else if (code.IsSameAs(wxS("Ascender")))
        {
          long ascent;
          param.ToLong(&ascent);
          fd.SetAscent(ascent);
        }
        else if (code.IsSameAs(wxS("Descender")))
        {
          param.ToLong(&nParam);
          fd.SetDescent(nParam);
        }
        else if (code.IsSameAs(wxS("UnderlineThickness")))
        {
          param.ToLong(&nParam);
          fd.SetUnderlineThickness(nParam);
        }
        else if (code.IsSameAs(wxS("UnderlinePosition")))
        {
          param.ToLong(&nParam);
          fd.SetUnderlinePosition(nParam);
        }
        else if (code.IsSameAs(wxS("IsFixedPitch")))
        {
          if (param == wxS("true"))
          {
            flags += 1 << 0;
          }
        }
        else if (code.IsSameAs(wxS("FontBBox")))
        {
          hasFontBBox = true;
          wxString bbox2 = tkz.GetNextToken();
          wxString bbox3 = tkz.GetNextToken();
          wxString bbox4 = tkz.GetNextToken();
          wxString bBox = wxS("[") + param + wxS(" ") + bbox2 + wxS(" ") + bbox3 + wxS(" ") + bbox4 + wxS("]");
          fd.SetFontBBox(bBox);
        }
        else if (code.IsSameAs(wxS("CapHeight")))
        {
          hasCapHeight = true;
          long capHeight;
          param.ToLong(&capHeight);
          fd.SetCapHeight(capHeight);
        }
        else if (code.IsSameAs(wxS("XHeight")))
        {
          hasXHeight = true;
          long xHeight;
          param.ToLong(&xHeight);
          fd.SetXHeight(xHeight);
        }
        else if (code.IsSameAs(wxS("StdVW")))
        {
          hasStemV = true;
          long stemV;
          param.ToLong(&stemV);
          fd.SetStemV(stemV);
        }
        else if (code.IsSameAs(wxS("EncodingScheme")))
        {
          encodingScheme = param;
        }
        else if (code.IsSameAs(wxS("StartCharMetrics")))
        {
          inHeader = false;
          inMetrics = true;
        }
      }
      else if (inMetrics)
      {
        if (code.IsSameAs(wxS("C")))
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
            if (token.IsSameAs(wxS("WX"))) // Character width
            {
              param = tkz.GetNextToken(); // Width
              param.ToLong(&width);
              dummy = tkz.GetNextToken(); // Semicolon

              if (!hasMissingWidth && glyphname.IsSameAs(wxS(".notdef")))
              {
                hasMissingWidth = true;
                m_missingWidth = width;
                fd.SetMissingWidth(width);
              }
            }
            else if (token.IsSameAs(wxS("N"))) // Glyph name
            {
              glyphname = tkz.GetNextToken(); // Glyph name
              dummy = tkz.GetNextToken(); // Semicolon
            }
            else if (token.IsSameAs(wxS("G"))) // Glyph number
            {
              hasGlyphNumbers = true;
              param = tkz.GetNextToken(); // Number
              param.ToLong(&glyphNumber);
              dummy = tkz.GetNextToken(); // Semicolon
            }
            else if (token.IsSameAs(wxS("B"))) // Character bounding box
            {
              dummy = tkz.GetNextToken(); // x left
              dummy = tkz.GetNextToken(); // y bottom
              dummy = tkz.GetNextToken(); // x right
              tokenBoxHeight = tkz.GetNextToken(); // y top
              tokenBoxHeight.ToLong(&boxHeight);
              dummy = tkz.GetNextToken(); // Semicolon

              if (!tokenBoxHeight.IsEmpty())
              {
                if (!hasCapHeight && !hasXCapHeight && glyphname.IsSameAs(wxS("X")))
                {
                  hasXCapHeight = true;
                  fd.SetCapHeight(boxHeight);
                }
                if (!hasXHeight && glyphname.IsSameAs(wxS("x")))
                {
                  hasXHeight = true;
                  fd.SetXHeight(boxHeight);
                }
              }
            }
            else
            {
              while (tkz.HasMoreTokens() && tkz.GetNextToken() != wxS(";"))
              {
              }
            }
          }
        }
        else if (code.IsSameAs(wxS("EndCharMetrics")))
        {
          inMetrics = false;
          inKerning = true;
        }
      }
      else if (inKerning)
      {
        if (code.IsSameAs(wxS("KPX")))
        {
          if (kpMap == NULL)
          {
            kpMap = new wxPdfKernPairMap();
          }
          wxString t1 = param;
          wxString t2 = tkz.GetNextToken();
          param = tkz.GetNextToken(); // Number
          long delta;
          param.ToLong(&delta);

          wxUint32 u1, u2;
          bool g1 = wxPdfEncoding::GlyphName2Unicode(t1, u1);
          bool g2 = wxPdfEncoding::GlyphName2Unicode(t2, u2);
          if (g1 && g2)
          {
            wxPdfKernWidthMap* kwMap = NULL;
            wxPdfKernPairMap::iterator kp = (*kpMap).find(u1);
            if (kp == (*kpMap).end())
            {
              kwMap = new wxPdfKernWidthMap();
              (*kpMap)[u1] = kwMap;
            }
            else
            {
              kwMap = kp->second;
            }
            (*kwMap)[u2] = delta;
          }
        }
      }
    }

    if (kpMap != NULL)
    {
      m_fontData->SetKernPairMap(kpMap);
    }

#if 0
    type1Font->SetDiffs(diffs);
#endif

    if (m_encoding.IsSameAs(wxS("StandardEncoding"))   ||
        m_encoding.IsSameAs(wxS("ISOLatin1Encoding")))
    {
      // Non-symbolic font
      flags += 1 << 5;
    }
    else
    {
      // Symbolic font
      flags += 1 << 2;
    }
    fd.SetFlags(flags);

    if (!hasCapHeight && !hasXCapHeight)
    {
      fd.SetCapHeight(fd.GetAscent());
    }
    if (!hasFontBBox)
    {
      wxString fbb = wxString::Format(wxS("[0 %d 1000 %d]"), fd.GetDescent()-100, fd.GetAscent()+100);
      fd.SetFontBBox(fbb);
    }
    m_fontData->SetDescription(fd);
    ok = true;
  }
  return ok;
}

// --- ReadPFM
//
// Parts of the following method has been derived from the public domain
// pfm2afm.c, the original version of which is available from:
// ftp://tug.ctan.org/pub/tex-archive/fonts/utilities/
// 
/********************************************************************
 *                                                                  *
 *  Title:  pfm2afm - Convert Windows .pfm files to .afm files      *
 *                                                                  *
 *  Author: Ken Borgendale   10/9/91  Version 1.0                   *
 *                                                                  *
 *  Function:                                                       *
 *      Convert a Windows .pfm (Printer Font Metrics) file to a     *
 *      .afm (Adobe Font Metrics) file.  The purpose of this is     *
 *      to allow fonts put out for Windows to be used with OS/2.    *
 *                                                                  *
 *  Syntax:                                                         *
 *      pfm2afm  infile  [outfile] -a                               *
 *                                                                  *
 *  Copyright:                                                      *
 *      pfm2afm - Copyright (C) IBM Corp., 1991                     *
 *                                                                  *
 *      This code is released for public use as long as the         *
 *      copyright remains intact.  This code is provided asis       *
 *      without any warrenties, express or implied.                 *
 *                                                                  *
 *  Notes:                                                          *
 *      1. Much of the information in the original .afm file is     *
 *         lost when the .pfm file is created, and thus cannot be   *
 *         reconstructed by this utility.  This is especially true  *
 *         of data for characters not in the Windows character set. *
 *                                                                  *
 *      2. This module is coded to be compiled by the MSC 6.0.      *
 *         For other compilers, be careful of the packing of the    *
 *         PFM structure.                                           *
 *                                                                  *
 ********************************************************************
 ********************************************************************
 *  Modified:  Russell Lang <rjl@eng.monash.edu.au>                 *
 *             1994-01-06  Version 1.1                              *
 *  Compiles with EMX/GCC                                           *
 *  Changed to AFM 3.0                                              *
 *  Put PFM Copyright in Notice instead of Comment                  *
 *  Added ItalicAngle                                               *
 *  Added UnderlinePosition                                         *
 *  Added UnderlineThickness                                        *
 *                                                                  *
 *  Modified 1995-03-10  rjl (fixes from Norman Walsh)              *
 *  Dodge compiler bug when creating descender                      *
 ********************************************************************
*/

typedef struct
{
  unsigned short vers;
  unsigned long  len;             // Total length of .pfm file
  unsigned char  copyright[60];   // Copyright string
  unsigned short type;
  unsigned short points;
  unsigned short verres;
  unsigned short horres;
  unsigned short ascent;
  unsigned short intleading;
  unsigned short extleading;
  unsigned char  italic;
  unsigned char  uline;
  unsigned char  overs;
  unsigned short weight;
  unsigned char  charset;         // if 0 then windows, else nomap
  unsigned short pixwidth;        // Width for mono fonts
  unsigned short pixheight;
  unsigned char  kind;            // Lower bit off in mono
  unsigned short avgwidth;        // Mono if avg=max width
  unsigned short maxwidth;        // Use to compute bounding box
  unsigned char  firstchar;       // First char in table
  unsigned char  lastchar;        // Last char in table
  unsigned char  defchar;
  unsigned char  brkchar;
  unsigned short widthby;
  unsigned long  device;
  unsigned long  face;            // Face name
  unsigned long  bits;
  unsigned long  bitoff;
  unsigned short extlen;
  unsigned long  psext;           // PostScript extension
  unsigned long  chartab;         // Character width tables
  unsigned long  res1;
  unsigned long  kernpairs;       // Kerning pairs
  unsigned long  kerntrack;       // Track Kern table
  unsigned long  fontname;        // Font name
} wxPdfPfmHeader;

typedef struct
{
  unsigned short len;
  unsigned char  res1[12];
  unsigned short capheight;       // Cap height
  unsigned short xheight;         // X height
  unsigned short ascender;        // Ascender
  unsigned short descender;       // Descender (positive)
  short          slant;           // CW italic angle
  short          superscript;
  short          subscript;
  short          superscriptsize;
  short          subscriptsize;
  short          underlineoffset; // +ve down
  short          underlinewidth;  // width of underline
} wxPdfPfmExtension;

#if 0 // Not used
// Translate table from 1004 to psstd.  1004 is an extension of the Windows translate table used in PM.
static int Win2PSStd[] =
{
  0,   0,   0,   0, 197, 198, 199,   0, 202,   0,   205, 206, 207, 0,   0,   0,   // 00
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 10
  32,  33,  34,  35,  36,  37,  38, 169,  40,  41,  42,  43,  44,  45,  46,  47,  // 20
  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  // 30
  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  // 40
  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  // 50
  193, 97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, // 60
  112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, // 70
  0,   0,   184, 166, 185, 188, 178, 179, 195, 189, 0,   172, 234, 0,   0,   0,   // 80
  0,   96,  0,   170, 186, 183, 177, 208, 196, 0,   0,   173, 250, 0,   0,   0,   // 90
  0,   161, 162, 163, 168, 165, 0,   167, 200, 0,   227, 171, 0,   0,   0,   197, // A0
  0,   0,   0,   0,   194, 0,   182, 180, 203, 0,   235, 187, 0,   0,   0,   191, // B0
  0,   0,   0,   0,   0,   0,   225, 0,   0,   0,   0,   0,   0,   0,   0,   0,   // C0
  0,   0,   0,   0,   0,   0,   0,   0,   233, 0,   0,   0,   0,   0,   0,   251, // D0
  0,   0,   0,   0,   0,   0,   241, 0,   0,   0,   0,   0,   0,   0,   0,   0,   // E0
  0,   0,   0,   0,   0,   0,   0,   0,   249, 0,   0,   0,   0,   0,   0,   0    // F0
};
    
// Character class.  This is a minor attempt to overcome the problem that
// in the pfm file, all unused characters are given the width of space.

static int WinClass[] =
{
  0, 0, 0, 0, 2, 2, 2, 0, 2, 0, 2, 2, 2, 0, 0, 0,   // 00
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 10
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 20
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 30
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 40
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 50
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // 60
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,   // 70
  0, 0, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,   // 80
  0, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2,   // 90
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // a0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // b0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // c0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // d0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   // e0
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1    // f0
};
#endif

// Windows character names.  Give a name to the used locations for when the all flag is specified.
static wxString WinChars[] =
{
  wxS("W00"),              // 00
  wxS("W01"),              // 01
  wxS("W02"),              // 02
  wxS("W03"),              // 03
  wxS("macron"),           // 04
  wxS("breve"),            // 05
  wxS("dotaccent"),        // 06
  wxS("W07"),              // 07
  wxS("ring"),             // 08
  wxS("W09"),              // 09
  wxS("W0a"),              // 0a
  wxS("W0b"),              // 0b
  wxS("W0c"),              // 0c
  wxS("W0d"),              // 0d
  wxS("W0e"),              // 0e
  wxS("W0f"),              // 0f
  wxS("hungarumlaut"),     // 10
  wxS("ogonek"),           // 11
  wxS("caron"),            // 12
  wxS("W13"),              // 13
  wxS("W14"),              // 14
  wxS("W15"),              // 15
  wxS("W16"),              // 16
  wxS("W17"),              // 17
  wxS("W18"),              // 18
  wxS("W19"),              // 19
  wxS("W1a"),              // 1a
  wxS("W1b"),              // 1b
  wxS("W1c"),              // 1c
  wxS("W1d"),              // 1d
  wxS("W1e"),              // 1e
  wxS("W1f"),              // 1f
  wxS("space"),            // 20
  wxS("exclam"),           // 21
  wxS("quotedbl"),         // 22
  wxS("numbersign"),       // 23
  wxS("dollar"),           // 24
  wxS("percent"),          // 25
  wxS("ampersand"),        // 26
  wxS("quotesingle"),      // 27
  wxS("parenleft"),        // 28
  wxS("parenright"),       // 29
  wxS("asterisk"),         // 2A
  wxS("plus"),             // 2B
  wxS("comma"),            // 2C
  wxS("hyphen"),           // 2D
  wxS("period"),           // 2E
  wxS("slash"),            // 2F
  wxS("zero"),             // 30
  wxS("one"),              // 31
  wxS("two"),              // 32
  wxS("three"),            // 33
  wxS("four"),             // 34
  wxS("five"),             // 35
  wxS("six"),              // 36
  wxS("seven"),            // 37
  wxS("eight"),            // 38
  wxS("nine"),             // 39
  wxS("colon"),            // 3A
  wxS("semicolon"),        // 3B
  wxS("less"),             // 3C
  wxS("equal"),            // 3D
  wxS("greater"),          // 3E
  wxS("question"),         // 3F
  wxS("at"),               // 40
  wxS("A"),                // 41
  wxS("B"),                // 42
  wxS("C"),                // 43
  wxS("D"),                // 44
  wxS("E"),                // 45
  wxS("F"),                // 46
  wxS("G"),                // 47
  wxS("H"),                // 48
  wxS("I"),                // 49
  wxS("J"),                // 4A
  wxS("K"),                // 4B
  wxS("L"),                // 4C
  wxS("M"),                // 4D
  wxS("N"),                // 4E
  wxS("O"),                // 4F
  wxS("P"),                // 50
  wxS("Q"),                // 51
  wxS("R"),                // 52
  wxS("S"),                // 53
  wxS("T"),                // 54
  wxS("U"),                // 55
  wxS("V"),                // 56
  wxS("W"),                // 57
  wxS("X"),                // 58
  wxS("Y"),                // 59
  wxS("Z"),                // 5A
  wxS("bracketleft"),      // 5B
  wxS("backslash"),        // 5C
  wxS("bracketright"),     // 5D
  wxS("asciicircum"),      // 5E
  wxS("underscore"),       // 5F
  wxS("grave"),            // 60
  wxS("a"),                // 61
  wxS("b"),                // 62
  wxS("c"),                // 63
  wxS("d"),                // 64
  wxS("e"),                // 65
  wxS("f"),                // 66
  wxS("g"),                // 67
  wxS("h"),                // 68
  wxS("i"),                // 69
  wxS("j"),                // 6A
  wxS("k"),                // 6B
  wxS("l"),                // 6C
  wxS("m"),                // 6D
  wxS("n"),                // 6E
  wxS("o"),                // 6F
  wxS("p"),                // 70
  wxS("q"),                // 71
  wxS("r"),                // 72
  wxS("s"),                // 73
  wxS("t"),                // 74
  wxS("u"),                // 75
  wxS("v"),                // 76
  wxS("w"),                // 77
  wxS("x"),                // 78
  wxS("y"),                // 79
  wxS("z"),                // 7A
  wxS("braceleft"),        // 7B
  wxS("bar"),              // 7C
  wxS("braceright"),       // 7D
  wxS("asciitilde"),       // 7E
  wxS("W7f"),              // 7F
  wxS("euro"),             // 80
  wxS("W81"),              // 81
  wxS("quotesinglbase"),   // 82
  wxS("florin"),           // 83
  wxS("quotedblbase"),     // 84
  wxS("ellipsis"),         // 85
  wxS("dagger"),           // 86
  wxS("daggerdbl"),        // 87
  wxS("circumflex"),       // 88
  wxS("perthousand"),      // 89
  wxS("Scaron"),           // 8A
  wxS("guilsinglleft"),    // 8B
  wxS("OE"),               // 8C
  wxS("W8d"),              // 8D
  wxS("Zcaron"),           // 8E
  wxS("W8f"),              // 8F
  wxS("W90"),              // 90
  wxS("quoteleft"),        // 91
  wxS("quoteright"),       // 92
  wxS("quotedblleft"),     // 93
  wxS("quotedblright"),    // 94
  wxS("bullet"),           // 95
  wxS("endash"),           // 96
  wxS("emdash"),           // 97
  wxS("tilde"),            // 98
  wxS("trademark"),        // 99
  wxS("scaron"),           // 9A
  wxS("guilsinglright"),   // 9B
  wxS("oe"),               // 9C
  wxS("W9d"),              // 9D
  wxS("zcaron"),           // 9E
  wxS("Ydieresis"),        // 9F
  wxS("reqspace"),         // A0
  wxS("exclamdown"),       // A1
  wxS("cent"),             // A2
  wxS("sterling"),         // A3
  wxS("currency"),         // A4
  wxS("yen"),              // A5
  wxS("brokenbar"),        // A6
  wxS("section"),          // A7
  wxS("dieresis"),         // A8
  wxS("copyright"),        // A9
  wxS("ordfeminine"),      // AA
  wxS("guillemotleft"),    // AB
  wxS("logicalnot"),       // AC
  wxS("syllable"),         // AD
  wxS("registered"),       // AE
  wxS("macron"),           // AF
  wxS("degree"),           // B0
  wxS("plusminus"),        // B1
  wxS("twosuperior"),      // B2
  wxS("threesuperior"),    // B3
  wxS("acute"),            // B4
  wxS("mu"),               // B5
  wxS("paragraph"),        // B6
  wxS("periodcentered"),   // B7
  wxS("cedilla"),          // B8
  wxS("onesuperior"),      // B9
  wxS("ordmasculine"),     // BA
  wxS("guillemotright"),   // BB
  wxS("onequarter"),       // BC
  wxS("onehalf"),          // BD
  wxS("threequarters"),    // BE
  wxS("questiondown"),     // BF
  wxS("Agrave"),           // C0
  wxS("Aacute"),           // C1
  wxS("Acircumflex"),      // C2
  wxS("Atilde"),           // C3
  wxS("Adieresis"),        // C4
  wxS("Aring"),            // C5
  wxS("AE"),               // C6
  wxS("Ccedilla"),         // C7
  wxS("Egrave"),           // C8
  wxS("Eacute"),           // C9
  wxS("Ecircumflex"),      // CA
  wxS("Edieresis"),        // CB
  wxS("Igrave"),           // CC
  wxS("Iacute"),           // CD
  wxS("Icircumflex"),      // CE
  wxS("Idieresis"),        // CF
  wxS("Eth"),              // D0
  wxS("Ntilde"),           // D1
  wxS("Ograve"),           // D2
  wxS("Oacute"),           // D3
  wxS("Ocircumflex"),      // D4
  wxS("Otilde"),           // D5
  wxS("Odieresis"),        // D6
  wxS("multiply"),         // D7
  wxS("Oslash"),           // D8
  wxS("Ugrave"),           // D9
  wxS("Uacute"),           // DA
  wxS("Ucircumflex"),      // DB
  wxS("Udieresis"),        // DC
  wxS("Yacute"),           // DD
  wxS("Thorn"),            // DE
  wxS("germandbls"),       // DF
  wxS("agrave"),           // E0
  wxS("aacute"),           // E1
  wxS("acircumflex"),      // E2
  wxS("atilde"),           // E3
  wxS("adieresis"),        // E4
  wxS("aring"),            // E5
  wxS("ae"),               // E6
  wxS("ccedilla"),         // E7
  wxS("egrave"),           // E8
  wxS("eacute"),           // E9
  wxS("ecircumflex"),      // EA
  wxS("edieresis"),        // EB
  wxS("igrave"),           // EC
  wxS("iacute"),           // ED
  wxS("icircumflex"),      // EE
  wxS("idieresis"),        // EF
  wxS("eth"),              // F0
  wxS("ntilde"),           // F1
  wxS("ograve"),           // F2
  wxS("oacute"),           // F3
  wxS("ocircumflex"),      // F4
  wxS("otilde"),           // F5
  wxS("odieresis"),        // F6
  wxS("divide"),           // F7
  wxS("oslash"),           // F8
  wxS("ugrave"),           // F9
  wxS("uacute"),           // FA
  wxS("ucircumflex"),      // FB
  wxS("udieresis"),        // FC
  wxS("yacute"),           // FD
  wxS("thorn"),            // FE
  wxS("ydieresis")         // FF
};

static const wxUint16 gs_winchar_map[256] =
{
  0x0000, 0x0000, 0x0000, 0x0000, 0x00AF, 0x02D8, 0x02D9, 0x0000,
  0x02DA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x02DD, 0x02DB, 0x02C7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
  0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
  0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
  0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
  0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
  0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
  0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
  0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
  0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
  0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
  0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
  0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x0000,

  0x20AC, 0x0000, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
  0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x017D, 0x0000,
  0x0000, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
  0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x0000, 0x017E, 0x0178,
  0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
  0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
  0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
  0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
  0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
  0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
  0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
  0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
  0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
  0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
  0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
  0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
};

bool
wxPdfFontParserType1::ReadPFM(wxInputStream& pfmFile)
{
  bool ok = true;
  int len = pfmFile.GetSize();
  wxPdfPfmHeader hdr;
  wxPdfPfmExtension ext;

  wxPdfKernPairMap* kpMap = NULL;

  // Read Header
  hdr.vers       = ReadUShortLE(&pfmFile);
  hdr.len        = ReadUIntLE(&pfmFile);
  pfmFile.Read(&hdr.copyright, 60);
  hdr.type       = ReadUShortLE(&pfmFile);
  hdr.points     = ReadUShortLE(&pfmFile);
  hdr.verres     = ReadUShortLE(&pfmFile);
  hdr.horres     = ReadUShortLE(&pfmFile);
  hdr.ascent     = ReadUShortLE(&pfmFile);
  hdr.intleading = ReadUShortLE(&pfmFile);
  hdr.extleading = ReadUShortLE(&pfmFile);
  pfmFile.Read(&hdr.italic, 1);
  pfmFile.Read(&hdr.uline, 1);
  pfmFile.Read(&hdr.overs, 1);
  hdr.weight     = ReadUShortLE(&pfmFile);
  pfmFile.Read(&hdr.charset, 1);
  hdr.pixwidth   = ReadUShortLE(&pfmFile);
  hdr.pixheight  = ReadUShortLE(&pfmFile);
  pfmFile.Read(&hdr.kind, 1);
  hdr.avgwidth   = ReadUShortLE(&pfmFile);
  hdr.maxwidth   = ReadUShortLE(&pfmFile);
  pfmFile.Read(&hdr.firstchar, 1);
  pfmFile.Read(&hdr.lastchar, 1);
  pfmFile.Read(&hdr.defchar, 1);
  pfmFile.Read(&hdr.brkchar, 1);
  hdr.widthby    = ReadUShortLE(&pfmFile);
  hdr.device     = ReadUIntLE(&pfmFile);
  hdr.face       = ReadUIntLE(&pfmFile);
  hdr.bits       = ReadUIntLE(&pfmFile);
  hdr.bitoff     = ReadUIntLE(&pfmFile);
  hdr.extlen     = ReadUShortLE(&pfmFile);
  hdr.psext      = ReadUIntLE(&pfmFile);
  hdr.chartab    = ReadUIntLE(&pfmFile);
  hdr.res1       = ReadUIntLE(&pfmFile);
  hdr.kernpairs  = ReadUIntLE(&pfmFile);
  hdr.kerntrack  = ReadUIntLE(&pfmFile);
  hdr.fontname   = ReadUIntLE(&pfmFile);

  // Consistency check
  if (len != (int) hdr.len ||                   // Check length field matches file length
      hdr.extlen != 30 ||                       // Check length of PostScript extension
      hdr.fontname < 75 /* && hdr.fontname > 512 */)  // Font name specified
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::ReadPFM: ")) +
               wxString(_("Not a valid Type1 PFM file")));
    return false;
  }

  // The mono flag in the pfm actually indicates whether there is a
  // table of font widths, not if they are all the same.
  bool isMono = (!(hdr.kind & 1) || hdr.avgwidth == hdr.maxwidth );

  // Read extension
  pfmFile.SeekI(hdr.psext);
  ext.len             = ReadUShortLE(&pfmFile);
  pfmFile.Read(&ext.res1, 12);
  ext.capheight       = ReadUShortLE(&pfmFile);
  ext.xheight         = ReadUShortLE(&pfmFile);
  ext.ascender        = ReadUShortLE(&pfmFile);
  ext.descender       = ReadUShortLE(&pfmFile);
  ext.slant           = ReadUShortLE(&pfmFile);
  ext.superscript     = ReadUShortLE(&pfmFile);
  ext.subscript       = ReadUShortLE(&pfmFile);
  ext.superscriptsize = ReadUShortLE(&pfmFile);
  ext.subscriptsize   = ReadUShortLE(&pfmFile);
  ext.underlineoffset = ReadUShortLE(&pfmFile);
  ext.underlinewidth  = ReadUShortLE(&pfmFile);

  // Initialize font description
  wxPdfFontDescription fd;

  // Font name
  pfmFile.SeekI(hdr.fontname);
  wxString fontName = ReadString(pfmFile);
  m_fontData->SetName(fontName);
  wxString fontNameLower = fontName.Lower();

  // The .pfm is missing full name, so construct from font name by changing
  // the hyphen to a space.  This actually works in a lot of cases.
  wxString fullName = fontName;
  fullName.Replace(wxS("-"), wxS(" "));
  if (hdr.face != 0)
  {
    pfmFile.SeekI(hdr.face);
    ReadString(pfmFile);
  }

  wxString encodingScheme = (hdr.charset != 0) ? wxString(wxS("FontSpecific")) : wxString(wxS("AdobeStandardEncoding"));

  int stemV = (hdr.weight > 475 || 
               fontNameLower.Find(wxS("bold")) != wxNOT_FOUND ||
               fontNameLower.Find(wxS("black")) != wxNOT_FOUND)  ? 120 : 80;
  fd.SetStemV(stemV);

  int italicAngle = 0;
  if (hdr.italic != 0 || fontNameLower.Find(wxS("italic")) != wxNOT_FOUND)
  {
    italicAngle = int(ext.slant / 10);
    // -12.00 - this is a typical value
  }
  fd.SetItalicAngle(italicAngle);

  // The .pfm file does not conatin the font bounding box.
  // The font bounding box is reconstructed by guessing reasonable values.
  int bbox1 = (isMono) ? -20 : -100;
  int bbox2 = -(ext.descender+5);
  int bbox3 = hdr.maxwidth+10;
  int bbox4 = hdr.ascent+5;
  wxString bBox = wxString::Format(wxS("[%d %d %d %d]"), bbox1, bbox2, bbox3, bbox4);
  fd.SetFontBBox(bBox);

  fd.SetAscent(ext.ascender);
  fd.SetDescent(ext.descender);
  fd.SetCapHeight(ext.capheight);
  fd.SetXHeight(ext.xheight);
  fd.SetUnderlinePosition(ext.underlineoffset);
  fd.SetUnderlineThickness(ext.underlinewidth);
  fd.SetMissingWidth(600);

  int flags = 0;
  if (isMono)
  {
    flags += 1 << 0; // Fixed pitch
  }
  if (hdr.charset != 0)
  {
    flags += 1 << 2; // Symbolic font
  }
  else
  {
    flags += 1 << 5; // Non-symbolic font
  }
  if (italicAngle > 0)
  {
    flags += 1 << 6; // italic
  }
  fd.SetFlags(flags);

#if 0
  // Character table
  // currently ignored
  pfmFile.SeekI(hdr.chartab);
  int count = hdr.lastchar - hdr.firstchar + 1;
#endif

  m_fontData->SetDescription(fd);

  if (hdr.kernpairs != 0)
  {
    pfmFile.SeekI(hdr.kernpairs);
    int count = ReadUShortLE(&pfmFile);
    unsigned char code1, code2;
    short delta;
    int k;
    for (k = 0; k < count; ++k)
    {
      pfmFile.Read(&code1, 1);
      pfmFile.Read(&code2, 1);
      delta = ReadShortLE(&pfmFile);
      if (delta != 0)
      {
        wxUint32 u1 = (wxUint32) gs_winchar_map[code1];
        wxUint32 u2 = (wxUint32) gs_winchar_map[code2];
        if (u1 != 0 && u2 != 0)
        {
          if (kpMap == NULL)
          {
            kpMap = new wxPdfKernPairMap();
          }
          wxPdfKernWidthMap* kwMap = NULL;
          wxPdfKernPairMap::iterator kp = (*kpMap).find(u1);
          if (kp == (*kpMap).end())
          {
            kwMap = new wxPdfKernWidthMap();
            (*kpMap)[u1] = kwMap;
          }
          else
          {
            kwMap = kp->second;
          }
          (*kwMap)[u2] = delta;
        }
      }
    }
    if (kpMap != NULL)
    {
      m_fontData->SetKernPairMap(kpMap);
    }
  }

  return ok;
}

// -- Parse Postscript font (PFA or PFB plus AFM or PFM) ---

bool
wxPdfFontParserType1::ReadPFX(wxInputStream* pfxFile, bool onlyNames)
{
  int start;
  int length;
  bool ok = CheckType1Format(pfxFile, start, length);
  if (ok)
  {
    m_skipArray = true;
    ok = ParseDict(pfxFile, start, length, onlyNames);
    if (ok && !onlyNames)
    {
      start = (m_isPFB) ? start+length : 0;
      ok = GetPrivateDict(pfxFile, start);
      if (ok)
      {
        m_glyphWidthMap = new wxPdfFontType1GlyphWidthMap();
        m_skipArray = true;
        ok = ParseDict(m_privateDict, 0, (int) m_privateDict->GetLength(), false);
      }
    }
  }
  return ok;
}

bool
wxPdfFontParserType1::CheckType1Format(wxInputStream* stream, int& start, int& length)
{
  // Check for PFB or PFA file format
  int limit = (int) stream->GetSize();
  unsigned char blocktype;
  SeekI(0, stream);
  m_isPFB = ReadPfbTag(stream, blocktype, length);
  if (!m_isPFB)
  {
    // Assume PFA file format
    SeekI(0, stream);
    length = limit;
  }

  // Check header
  start = TellI(stream);
  wxString str = ReadString(14, stream);
  bool ok = str.IsSameAs(wxS("%!PS-AdobeFont"));
  if (!ok)
  {
    SeekI(start, stream);
    str = ReadString(10, stream);
    ok = str.IsSameAs(wxS("%!FontType"));
  }
  if (ok)
  {
    // Is stream size large enough?
    ok = (start+length <= limit);
  }
  stream->SeekI(start);
  return ok;
}

bool
wxPdfFontParserType1::GetPrivateDict(wxInputStream* stream, int start)
{
  bool ok = false;
  wxMemoryOutputStream privateDict;
  wxMemoryOutputStream* eexecStream = new wxMemoryOutputStream();
  stream->SeekI(start);
  if (m_isPFB)
  {
    // The private dictionary can be made of several segments
    // All binary segements are read in and concatenated
    unsigned char blocktype;
    int length;
    do
    {
      ok = ReadPfbTag(stream, blocktype, length);
      if (ok && blocktype == PFB_BLOCK_BINARY)
      {
        char* buf = new char[length];
        stream->Read(buf, length);
        eexecStream->Write(buf, length);
        delete [] buf;
      }
    }
    while (ok && blocktype == PFB_BLOCK_BINARY);
  }
  else
  {
    bool found = false;
    wxString token = wxEmptyString;
    int limit = (int) stream->GetSize();
    while(!found && stream->TellI() < limit)
    {
      token = GetToken(stream);
      if (token.IsSameAs(wxS("eexec")))
      {
        found = true;
      }
      else
      {
        SkipToNextToken(stream);
      }
    }
    if (found)
    {
      char ch = stream->GetC();
      if (ch == '\r' || ch == '\n')
      {
        if (ch == '\r' && stream->Peek() == '\n')
        {
          ch = stream->GetC();
        }
        int offset = stream->TellI();
        char prefix[4];
        stream->Read(prefix,4);
        if (IsHexDigit(prefix[0]) && IsHexDigit(prefix[1]) &&
            IsHexDigit(prefix[2]) && IsHexDigit(prefix[3]))
        {
          stream->SeekI(offset);
          DecodeHex(stream, eexecStream);
        }
        else
        {
          stream->SeekI(offset);
          eexecStream->Write(*stream);
        }
        ok = true;
      }
      else
      {
        ok = false; // error
      }
    }
  }
  if (ok && eexecStream->GetSize() > 0)
  {
    // decrypt the encoded binary private dictionary
    DecodeEExec(eexecStream, &privateDict, 55665U, 4);
    m_privateDict = new wxMemoryInputStream(privateDict);
    delete eexecStream;
#if 0
    wxFileOutputStream pfbPrivateDict(wxS("pfbprivdict.dat"));
    wxMemoryInputStream tmp(privateDict);
    pfbPrivateDict.Write(tmp);
    pfbPrivateDict.Close();
#endif
  }
  return ok;
}

bool
wxPdfFontParserType1::IsHexDigit(char digit)
{
  return ('0' <= digit && digit <= '9') || ('A' <= digit && digit <= 'F') || ('a' <= digit && digit <= 'f');
}

void
wxPdfFontParserType1::DecodeHex(wxInputStream* inStream, wxOutputStream* outStream)
{
  char ic, oc;
  char value = 0;
  int limit = (int) inStream->GetSize();
  bool ok = true;
  bool second = false;
  while (ok && inStream->TellI() < limit)
  {
    ic = inStream->GetC();
    if (ic == ' '  || ic == '\r' || ic == '\n' || 
        ic == '\t' || ic == '\f' || ic == '\0' )
      continue;
    if      ('0' <= ic && ic <= '9') value = ic - '0';
    else if ('A' <= ic && ic <= 'F') value = ic - 'A' + 10;
    else if ('a' <= ic && ic <= 'f') value = ic - 'a' + 10;
    else
    {
      // Invalid character encountered
      ok = false;
      break;
    }
    if (!second)
    {
      oc = (value & 0x0f) << 4;
    }
    else
    {
      oc |= (value & 0x0f);
      outStream->Write(&oc, 1);
    }
    second = !second;
  }
  if (ok && second)
  {
    outStream->Write(&oc, 1);
  }
}

void
wxPdfFontParserType1::DecodeEExec(wxMemoryOutputStream* eexecStream, wxOutputStream* outStream, unsigned short seed, int lenIV)
{
  wxMemoryInputStream inStream(*eexecStream);
  int limit = (int) inStream.GetSize();
  unsigned int s = seed;
  unsigned int val;
  char b;
  int j;
  for (j = 0; j < limit; j++)
  {
    val = (unsigned int) inStream.GetC();
    b   = (val ^ (s >> 8)) & 0xff;
    s         = ((val + s)*52845U + 22719) & 0xFFFFU;
    // ignore the four random bytes at the beginning
    if (j >= lenIV)
    {
      outStream->Write(&b, 1);
    }
  }
}

bool
wxPdfFontParserType1::ReadPfbTag(wxInputStream* stream, unsigned char& blocktype, int& blocksize)
{
  bool ok = false;
  blocktype = 0;
  blocksize = 0;
  unsigned char b1 = ReadByte(stream);
  unsigned char b2 = ReadByte(stream);
  if (b1 == PFB_BLOCK_MARKER && (b2 == PFB_BLOCK_ASCII || b2 == PFB_BLOCK_BINARY))
  {
    blocktype = b2;
    blocksize = (int) ReadUIntLE(stream);
    ok = true;
  }
  return ok;
}

void
wxPdfFontParserType1::CheckRestrictions(long fsType)
{
  bool rl = (fsType & 0x0002) != 0; // restricted license
  bool pp = (fsType & 0x0004) != 0; // preview and print embedding
  bool e  = (fsType & 0x0008) != 0; // editable embedding
  bool ns = (fsType & 0x0100) != 0; // no subsetting
  bool b  = (fsType & 0x0200) != 0; // bitmap embedding only
  m_embedAllowed = !((rl && !pp && !e) || b);
  m_subsetAllowed = !ns;
}

void
wxPdfFontParserType1::SkipComment(wxInputStream* stream)
{
  unsigned char ch = ReadByte(stream);
  while (!stream->Eof())
  {
    if (ch == '\r' || ch == '\n') break;
    ch = ReadByte(stream);
  }
}

void
wxPdfFontParserType1::SkipSpaces(wxInputStream* stream)
{
  char ch = (char) ReadByte(stream);
  while (!stream->Eof())
  {
    if (!(ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t' || ch == '\f' || ch == '\0'))
    {
      if (ch == '%')
      {
        // According to the PLRM, a comment is equal to a space.
        SkipComment(stream);
        ch = (char) ReadByte(stream);
      }
      else
      {
        stream->SeekI(-1, wxFromCurrent);
        break;
      }
    }
    else
    {
      ch = (char) ReadByte(stream);
    }
  }
}

void
wxPdfFontParserType1::SkipLiteralString(wxInputStream* stream)
{
  // first character must be `('
  int embed = 1;
  int j;
  char ch = (char) ReadByte(stream);
  while (!stream->Eof())
  {
    if (ch == '\\')
    {
      /* Red Book 3rd ed., section `Literal Text Strings', p. 29:     */
      /* A backslash can introduce three different types              */
      /* of escape sequences:                                         */
      /*   - a special escaped char like \r, \n, etc.                 */
      /*   - a one-, two-, or three-digit octal number                */
      /*   - none of the above in which case the backslash is ignored */

      ch = (char) ReadByte(stream);
      if (stream->Eof()) break;
      switch (ch)
      {
        /* skip `special' escape */
        case 'n':
        case 'r':
        case 't':
        case 'b':
        case 'f':
        case '\\':
        case '(':
        case ')':
          ch = (char) ReadByte(stream);
          break;

        default:
          /* skip octal escape or ignore backslash */
          for (j = 0; j < 3 && !stream->Eof(); ++j)
          {
            if (!('0' <= ch && ch <= '7')) break;
            ch = (char) ReadByte(stream);
          }
          break;
      }
    }
    else if (ch == '(')
    {
      embed++;
      ch = (char) ReadByte(stream);
    }
    else if (ch == ')')
    {
      embed--;
      if (embed == 0) break;
      ch = (char) ReadByte(stream);
    }
    else
    {
      ch = (char) ReadByte(stream);
    }
  }
}

wxString
wxPdfFontParserType1::GetLiteralString(wxInputStream* stream)
{
  wxString literalString;
  SkipSpaces(stream);
  // first character must be `('
  int embed = 0;
  int j;
  unsigned char ch = ReadByte(stream);
  while (!stream->Eof())
  {
    if (ch == '\\')
    {
      /* Red Book 3rd ed., section `Literal Text Strings', p. 29:     */
      /* A backslash can introduce three different types              */
      /* of escape sequences:                                         */
      /*   - a special escaped char like \r, \n, etc.                 */
      /*   - a one-, two-, or three-digit octal number                */
      /*   - none of the above in which case the backslash is ignored */

      ch = ReadByte(stream);
      if (stream->Eof()) break;
      switch (ch)
      {
        /* skip `special' escape */
        case 'n':
          literalString.Append(wxS("\n"));
          ch = ReadByte(stream);
          break;
        case 'r':
          literalString.Append(wxS("\r"));
          ch = ReadByte(stream);
          break;
        case 't':
          literalString.Append(wxS("\t"));
          ch = ReadByte(stream);
          break;
        case 'b':
          literalString.Append(wxS("\b"));
          ch = ReadByte(stream);
          break;
        case 'f':
          literalString.Append(wxS("\f"));
          ch = ReadByte(stream);
          break;
        case '\\':
        case '(':
        case ')':
#if wxCHECK_VERSION(2,9,0)
          literalString.Append(wxUniChar((unsigned int) ch));
#else
          literalString.Append(wxChar(ch));
#endif
          ch = ReadByte(stream);
          break;

        default:
          /* skip octal escape or ignore backslash */
          int value = 0;
          for (j = 0; j < 3 && !stream->Eof(); ++j)
          {
            if (!('0' <= ch && ch <= '7')) break;
            value = value * 8 + (ch - '0');
            ch = ReadByte(stream);
          }
#if wxCHECK_VERSION(2,9,0)
          literalString.Append(wxUniChar(value));
#else
          literalString.Append(wxChar(value));
#endif
          break;
      }
    }
    else if (ch == '(')
    {
      if (embed > 0)
      {
#if wxCHECK_VERSION(2,9,0)
        literalString.Append(wxUniChar((unsigned int) ch));
#else
        literalString.Append(wxChar(ch));
#endif
      }
      embed++;
      ch = ReadByte(stream);
    }
    else if (ch == ')')
    {
      embed--;
      if (embed == 0) break;
      ch = ReadByte(stream);
    }
    else
    {
#if wxCHECK_VERSION(2,9,0)
      literalString.Append(wxUniChar((unsigned int) ch));
#else
      literalString.Append(wxChar(ch));
#endif
      ch = ReadByte(stream);
    }
  }
  return literalString;
}

wxString
wxPdfFontParserType1::GetArray(wxInputStream* stream)
{
  wxString arrayString;
  SkipSpaces(stream);
  int embed = 0;
  unsigned char ch = ReadByte(stream);
  unsigned char delimBeg = ch;
  unsigned char delimEnd = (ch == '[') ? ']' : '}';
  while (!stream->Eof())
  {
    if (ch == delimBeg)
    {
      if (embed > 0)
      {
#if wxCHECK_VERSION(2,9,0)
        arrayString.Append(wxUniChar((unsigned int) ch));
#else
        arrayString.Append(wxChar(ch));
#endif
      }
      embed++;
      ch = ReadByte(stream);
    }
    else if (ch == delimEnd)
    {
      embed--;
      if (embed == 0) break;
#if wxCHECK_VERSION(2,9,0)
      arrayString.Append(wxUniChar((unsigned int) ch));
#else
      arrayString.Append(wxChar(ch));
#endif
      ch = ReadByte(stream);
    }
    else
    {
#if wxCHECK_VERSION(2,9,0)
      arrayString.Append(wxUniChar((unsigned int) ch));
#else
      arrayString.Append(wxChar(ch));
#endif
      ch = ReadByte(stream);
    }
  }
  return arrayString;
}

void
wxPdfFontParserType1::SkipString(wxInputStream* stream)
{
  // first character must be `<'
  char ch = (char) ReadByte(stream);
  while (!stream->Eof())
  {
    // All whitespace characters are ignored.
    SkipSpaces(stream);
    if (stream->Eof()) break;
    ch = (char) ReadByte(stream);
    if (!((ch >= '0' && ch <= '9') || (ch >='A' && ch <='F') || (ch >= 'a' && ch <= 'f')))
    {
      break;
    }
  }
  if (!stream->Eof() && ch != '>')
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::SkipString: ")) +
               wxString(_("skip_string: missing closing delimiter `>'")));
  }
}

void
wxPdfFontParserType1::SkipProcedure(wxInputStream* stream)
{
  /* first character must be the opening brace that */
  /* starts the procedure                           */

  /* NB: [ and ] need not match:                    */
  /* `/foo {[} def' is a valid PostScript fragment, */
  /* even within a Type1 font                       */
  bool endFound = false;
  int embed = 1;
  char ch = (char) ReadByte(stream);
  while (!endFound && !stream->Eof())
  {
    switch (ch)
    {
      case '{':
        ++embed;
        break;
      case '}':
        --embed;
        if (embed == 0)
        {
          endFound = true;
        }
        break;
      case '(':
        SkipLiteralString(stream);
        break;
      case '<':
        SkipString(stream);
        break;
      case '%':
        SkipComment(stream);
        break;
    }
    if (!endFound)
    {
      ch = (char) ReadByte(stream);
    }
  }
  if (!endFound)
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::SkipProcedure: ")) +
               wxString(_("Invalid file format")));
  }
}

void
wxPdfFontParserType1::SkipArray(wxInputStream* stream)
{
  /* first character must be the opening brace that */
  /* starts the procedure                           */

  /* NB: [ and ] need not match:                    */
  /* `/foo {[} def' is a valid PostScript fragment, */
  /* even within a Type1 font                       */
  bool endFound = false;
  int embed = 1;
  char ch = (char) ReadByte(stream);
  while (!endFound && !stream->Eof())
  {
    switch (ch)
    {
      case '[':
        ++embed;
        break;
      case ']':
        --embed;
        if (embed == 0)
        {
          endFound = true;
        }
        break;
      case '(':
        SkipLiteralString(stream);
        break;
      case '<':
        SkipString(stream);
        break;
      case '%':
        SkipComment(stream);
        break;
    }
    if (!endFound)
    {
      ch = (char) ReadByte(stream);
    }
  }
  if (!endFound)
  {
    wxLogError(wxString(wxS("wxPdfFontParserType1::SkipArray: ")) +
               wxString(_("Invalid file format")));
  }
}

void
wxPdfFontParserType1::SkipToNextToken(wxInputStream* stream)
{
  SkipSpaces(stream);
  if (!stream->Eof())
  {
    unsigned char ch = ReadByte(stream);

    if (ch == '[')
    {
      if (m_skipArray) SkipArray(stream);
    }
    else if (ch == ']')
    {
      //ch = (char) ReadByte(stream);
    }
    else if (ch == '{')
    {
      SkipProcedure(stream);
    }
    else if (ch == '(')
    {
      SkipLiteralString(stream);
    }
    else if (ch == '<')
    {
      ch = stream->Peek();
      if (!stream->Eof() && ch == '<' )
      {
        ch = ReadByte(stream);
      }
      else
      {
        SkipString(stream);
      }
    }
    else if (ch == '>')
    {
      ch = ReadByte(stream);
      if (stream->Eof() || ch != '>' )   /* >> */
      {
//      FT_ERROR(( "ps_parser_skip_PS_token: unexpected closing delimiter `>'\n" ));
        wxLogError(wxString(wxS("wxPdfFontParserType1::SkipToNextToken: ")) +
                   wxString(_("Invalid File Format")));
      }
    }
    else
    {
      stream->SeekI(-1, wxFromCurrent);
    }
  }
}

wxString
wxPdfFontParserType1::GetToken(wxInputStream* stream)
{
  wxString str = wxEmptyString;
  SkipSpaces(stream);
  unsigned char ch = ReadByte(stream);
  if (ch == '/')
  {
#if wxCHECK_VERSION(2,9,0)
    str.Append(wxUniChar((unsigned int) ch));
#else
    str.Append(wxChar(ch));
#endif
    ch = ReadByte(stream);
  }
  while (!stream->Eof())
  {
    if (ch == ' '  || ch == '\r' || ch == '\n' ||
        ch == '\t' || ch == '\f' || ch == '\0' ||
        ch == '/'  || ch == '('  || ch == ')'  ||
        ch == '<'  || ch == '>'  || ch == '['  ||
        ch == ']'  || ch == '{'  || ch == '}'  ||
        ch == '%')
    {
      if (str.IsEmpty() && (ch == '[' || ch == ']' /* || ch == '{' || ch == '}' */ ))
      {
#if wxCHECK_VERSION(2,9,0)
        str.Append(wxUniChar((unsigned int) ch));
#else
        str.Append(wxChar(ch));
#endif
      }
      else
      {
        stream->SeekI(-1, wxFromCurrent);
      }
      break;
    }
#if wxCHECK_VERSION(2,9,0)
    str.Append(wxUniChar((unsigned int) ch));
#else
    str.Append(wxChar(ch));
#endif
    ch = ReadByte(stream);
  }
  return str;
}

bool
wxPdfFontParserType1::ParseDict(wxInputStream* stream, int start, int length, bool onlyNames)
{
#if 0
  // Currently not used
  bool hasStemV = false;
  int flags = 0;
#endif
  bool ready = false;
  bool hasFontName = false;
  bool hasFontBBox = false;
  bool hasFullName = false;
  bool hasFamilyName = false;
  bool hasWeight = false;
  bool hasFSType = false;
  bool ok = true;
  bool haveInteger = false;
  long intValue = 0;
  int limit = start + length;
  stream->SeekI(start);
  while (!ready && stream->TellI() < limit)
  {
    wxString token = GetToken(stream);
    // Check for the keywords 'eexec' or 'closefile',
    // either of those terminates a section
    if (token.IsSameAs(wxS("eexec")) ||
        token.IsSameAs(wxS("closefile")))
    {
      break;
    }
    else if (token.IsSameAs(wxS("FontDirectory")))
    {
      // Check whether the keyword 'FontDictionary' occurred after a Private
      // dictionary (in that case only the charstrings are taken from the
      // base font of a synthetic font
      if (m_privateFound) m_fontDirAfterPrivate = true;
    }
    else if (!token.IsEmpty() && token.GetChar(0) >= wxS('0') && token.GetChar(0) <= wxS('9'))
    {
      // The token is an integer
      token.ToLong(&intValue);
      haveInteger = true;
    }
    else if ((token.StartsWith(wxS("RD")) || token.StartsWith(wxS("-|"))) && haveInteger )
    {
      // The keywords 'RD' or '-|' are not encountered directly in valid fonts,
      // since they are handled by one of the methods ParseSubrs and ParseCharStrings
      stream->SeekI(intValue+1, wxFromCurrent);
      haveInteger = false;
    }
    else if (!token.IsEmpty() && token.GetChar(0) == wxS('/'))
    {
      // Parse keyword value pairs
      wxString param;
      if (!m_privateFound)
      {
        if (token.IsSameAs(wxS("/FontMatrix")))
        {
          ParseFontMatrix(stream);
        }
        else if (token.IsSameAs(wxS("/Encoding")))
        {
          ParseEncoding(stream);
        }
        else if (token.IsSameAs(wxS("/Private")))
        {
          m_privateFound = true;
        }
        else if (token.IsSameAs(wxS("/FontName")))
        {
          hasFontName = true;
          param = GetToken(stream);
          m_fontData->SetName(param.substr(1));
        }
        else if (token.IsSameAs(wxS("/FullName")))
        {
          hasFullName = true;
          param = GetLiteralString(stream);
          wxArrayString fullNames;
          fullNames.Add(param);
          m_fontData->SetFullNames(fullNames);
        }
        else if (token.IsSameAs(wxS("/FamilyName")))
        {
          hasFamilyName = true;
          param = GetLiteralString(stream);
          m_fontData->SetFamily(param);
        }
        else if (token.IsSameAs(wxS("/Weight")))
        {
          hasWeight = true;
          param = GetLiteralString(stream);
          m_fontData->SetStyle(param);
        }

#if 0
  // For now most parameters are read from the metric file
        else if (token.IsSameAs(wxS("/ItalicAngle")))
        {
          param = GetToken(stream);
          double italic = wxPdfUtility::String2Double(param);
          int italicAngle = int(italic);
          m_fontDesc.SetItalicAngle(italicAngle);
          if (italicAngle > 0)
          {
            flags += 1 << 6;
          }
        }
        else if (token.IsSameAs(wxS("/isFixedPitch")))
        {
          param = GetToken(stream);
          if (param.IsSameAs(wxS("true")))
          {
            flags += 1 << 0;
          }
        }
        else if (token.IsSameAs(wxS("/UnderlinePosition")))
        {
          param = GetToken(stream);
          long nParam;
          param.ToLong(&nParam);
          m_fontDesc.SetUnderlinePosition(nParam);
        }
        else if (token.IsSameAs(wxS("/UnderlineThickness")))
        {
          param = GetToken(stream);
          long nParam;
          param.ToLong(&nParam);
          m_fontDesc.SetUnderlineThickness(nParam);
        }
#endif
        else if (token.IsSameAs(wxS("/FSType")))
        {
          hasFSType = true;
          param = GetToken(stream);
          long fsType = 0;
          param.ToLong(&fsType);
          CheckRestrictions(fsType);
          m_fontData->SetEmbedSupported(m_embedAllowed);
// TODO: Support subsetting for Type1
//          fontData->SetSubsetSupported(m_subsetAllowed);
// For now subsetting is not supported for Type1 fonts,
// so set the font data flag to 'false'
          m_fontData->SetSubsetSupported(false);
        }
        else if (token.IsSameAs(wxS("/FontBBox")))
        {
          if (!hasFontBBox)
          {
            param = GetArray(stream);
            if (param.Find(wxS('{')) == wxNOT_FOUND &&
                param.Find(wxS('[')) == wxNOT_FOUND)
            {
              hasFontBBox = true;
              m_fontDesc.SetFontBBox(wxString(wxS("["))+param+wxString(wxS("]")));
            }
          }
          else
          {
            SkipToNextToken(stream);
          }
        }
        else
        {
          SkipToNextToken(stream);
        }
      }
      else
      {
        // If the keyword 'FontDirectory' is encountered after the keyword
        // '/Private', the font file represents a synthetic font and all
        // keywords except '/CharStrings' are ignored 
        if (token.IsSameAs(wxS("/CharStrings")))
        {
          ParseCharStrings(stream);
        }
        else if (!m_fontDirAfterPrivate)
        {
          if (token.IsSameAs(wxS("/Subrs")))
          {
            ParseSubrs(stream);
          }
          else if (token.IsSameAs(wxS("/lenIV")))
          {
            param = GetToken(stream);
            long lenIV;
            param.ToLong(&lenIV);
            // maybe the keyword 'password' should be read. too
          }
          else
          {
            SkipToNextToken(stream);
          }
        }
        else
        {
          SkipToNextToken(stream);
        }
      }
      haveInteger = false;
    }
    else
    {
      SkipToNextToken(stream);
      haveInteger = false;
    }
    ready = onlyNames && hasFontName && hasFamilyName && hasFullName && hasWeight && hasFSType;
  }

  if (onlyNames)
  {
    ok = hasFontName;
  }
  return ok;
}

void
wxPdfFontParserType1::ParseFontMatrix(wxInputStream* stream)
{
  wxString matrix = GetArray(stream);
#if 0
  // If the font matrix is not [ 0.001 0 0 0.001 0 0]
  // font metrics need to be transformed accordingly
  int unitsPerEm;
  wxStringTokenizer tkz(matrix, wxS(" "));
  int count = tkz.CountTokens();
  if (count == 6)
  {
    double xx = wxPdfUtility::String2Double(tkz.GetNextToken());
    double yx = wxPdfUtility::String2Double(tkz.GetNextToken());
    double xy = wxPdfUtility::String2Double(tkz.GetNextToken());
    double yy = wxPdfUtility::String2Double(tkz.GetNextToken());
    double xo = wxPdfUtility::String2Double(tkz.GetNextToken());
    double yo = wxPdfUtility::String2Double(tkz.GetNextToken());
    unitsPerEm = (int) floor((1. / yy)+0.5);
  }
  else
  {
    unitPerEm = 1000;
  }
#endif
}

void
wxPdfFontParserType1::ParseEncoding(wxInputStream* stream)
{
  wxString token;
  long count, n, code;
  bool onlyImmediates = false;

  SkipSpaces(stream);
  char ch = stream->Peek();
  if (('0' <= ch && ch <= '9') || ch == '[')
  {
    // A number or `[' indicates that the encoding is an array
    // read the number of entries in the encoding; should be 256
    if (ch == '[')
    {
      count = 256;
      onlyImmediates = true;
      ch = stream->GetC(); // skip '['
    }
    else
    {
      token = GetToken(stream);
      token.ToLong(&count);
    }
    SkipSpaces(stream);
    // Allocate table with count entries
    m_encodingVector.Alloc(count);
    m_encodingVector.Insert(wxS(".notdef"), 0, count);

    // For each entry a record of the form 'charcode /charname' is read,
    // i.e. look for a number followed by an immediate name.
    // If the array is given in the form '/Encoding [ ... ]' read immediates only.

    n = 0;
    SkipSpaces(stream);
    while (true)
    {
      // Stop when next token is 'def' or ']'
      if (stream->Peek() == ']')
      {
        break;
      }
      token = GetToken(stream);
      if (token.IsSameAs(wxS("def")) || token.IsSameAs(wxS("]")))
      {
        break;
      }
      if ((wxS('0') <= token[0] && token[0] <= wxS('9')) || onlyImmediates)
      {
        if (onlyImmediates)
        {
          code = n;
        }
        else
        {
          token.ToLong(&code);
          token = GetToken(stream);
        }
        if (token[0] == wxS('/') && n < count)
        {
          m_encodingVector[code] = token;
          n++;
          SkipToNextToken(stream);
        }
      }
      else
      {
        SkipToNextToken(stream);
      }
    }
    m_encoding = wxS("ArrayEncoding");
    m_fontData->SetEncodingType(m_encoding);
    m_fontData->SetEncodingMap(m_encodingVector);
  }
  else
  {
    token = GetToken(stream);
    if (token.IsSameAs(wxS("StandardEncoding"))   ||
        token.IsSameAs(wxS("ExpertEncoding"))     ||
        token.IsSameAs(wxS("ISOLatin1Encoding")))
    {
      m_encoding = token;
      m_fontData->SetEncodingType(m_encoding);
    }
  }
}

void
wxPdfFontParserType1::ParseSubrs(wxInputStream* stream)
{
  // test for empty array
  wxString token = GetToken(stream);
  if (token[0] == wxS('['))
  {
    SkipToNextToken(stream);
    token = GetToken(stream);
    if (token[0] != wxS(']'))
    {
      wxLogError(wxString(wxS("wxPdfFontParserType1::ParseSubrs: ")) +
                 wxString(_("Invalid Type1 format")));
    }
    return;
  }
  long numSubrs, n, subrno;
  token.ToLong(&numSubrs);
  token = GetToken(stream); // 'array'

  // the format is simple: 'index' + binary data

  for (n = 0; n < numSubrs; n++)
  {
    token = GetToken(stream);
    /* If the next token isn't `dup', we are also done.  This */
    /* happens when there are `holes' in the Subrs array.     */
    if (!token.IsSameAs(wxS("dup")))
    {
      break;
    }

    token = GetToken(stream); // subr index
    if (token.ToLong(&subrno))
    token = GetToken(stream); // size
    long binarySize;
    token.ToLong(&binarySize);
    token = GetToken(stream);
    int binaryStart = stream->TellI() + 1;
    // Store glyph and charstring
    wxMemoryOutputStream subr;
    ReadBinary(*stream, binaryStart, binarySize, subr);

    if (m_lenIV >= 0)
    {
      if (binarySize < m_lenIV)
      {
        wxLogError(wxString(wxS("wxPdfFontParserType1::ParseSubrs: ")) +
                   wxString(_("Invalid Type1 file format")));
        break;
      }
      wxMemoryOutputStream subrDecoded;
      DecodeEExec(&subr, &subrDecoded, 4330, m_lenIV);
      //binarySize -= m_lenIV;
      // skip lenIV bytes
      m_subrsIndex->Add(wxPdfCffIndexElement(subrDecoded));
    }
    else
    {
      m_subrsIndex->Add(wxPdfCffIndexElement(subr));
    }

    stream->SeekI(binaryStart+binarySize);
    /* The binary string is followed by one token, e.g. `NP' */
    /* (bound to `noaccess put') or by two separate tokens:  */
    /* `noaccess' & `put'.  We position the parser right     */
    /* before the next `dup', if any.                        */
    token = GetToken(stream); // `NP' or `|' or `noaccess'
    if (token.IsSameAs(wxS("noaccess")))
    {
      // skip 'put'
      token = GetToken(stream);
    }
  }
}

void
wxPdfFontParserType1::ReadBinary(wxInputStream& inStream, int start, int size, wxOutputStream& outStream)
{
  char* buffer = new char[size];
  inStream.SeekI(start);
  inStream.Read(buffer, size);
  outStream.Write(buffer, size);
  delete [] buffer;
}

void
wxPdfFontParserType1::ParseCharStrings(wxInputStream* stream)
{
  long numGlyphs, n;
  wxString token;
  bool notdefFound = false;

  token = GetToken(stream);
  token.ToLong(&numGlyphs);
  if (numGlyphs == 0) return;

  wxPdfCffDecoder decoder;
  n = 0;
  for (;;)
  {
    // Format of charstrings dictionary entries: '/glyphname' + binary data
    SkipSpaces(stream);
    if (stream->Eof()) break;

    // All charstrings have been read when the keyword `def' or `end' is found
    token = GetToken(stream);
    if (token.IsSameAs(wxS("def")))
    {
      // Ignore 'def' if no charstring has been seen yet
      if (n > 0) break;
    }
    if (token.IsSameAs(wxS("end")))
    {
      break;
    }

    if (token[0] == wxS('/'))
    {
      wxString glyphName = token.substr(1);
      token = GetToken(stream);
      long binarySize;
      if (token.ToLong(&binarySize))
      {
        token = GetToken(stream);
        int binaryStart = stream->TellI() + 1;
        // Store glyph and charstring
        wxMemoryOutputStream charstring;
        ReadBinary(*stream, binaryStart, binarySize, charstring);

        if (m_lenIV >= 0)
        {
          if (binarySize < m_lenIV)
          {
            wxLogError(wxString(wxS("wxPdfFontParserType1::ParseCharStrings: ")) +
                       wxString(_("Invalid Type1 file format")));
            break;
          }
          wxMemoryOutputStream charstringDecoded;
          DecodeEExec(&charstring, &charstringDecoded, 4330, m_lenIV);
          // skip lenIV bytes
          // TODO: ?? binarySize -= m_lenIV;
          m_charStringsIndex->Add(wxPdfCffIndexElement(charstringDecoded));
        }
        else
        {
          m_charStringsIndex->Add(wxPdfCffIndexElement(charstring));
        }
        wxPdfCffIndexElement& element = (*m_charStringsIndex).Last();
        int width, bchar, achar;
        bool isComposite;
        bool ok = decoder.GetCharWidthAndComposite(element, width, isComposite, bchar, achar);
        if (ok)
        {
          (*m_glyphWidthMap)[glyphName] = (wxUint16) width;
        }
        if (glyphName.IsSameAs(wxS("/.notdef")))
        {
          notdefFound = true;
        }
        stream->SeekI(binaryStart+binarySize);
      }
      else
      {
        wxLogError(wxString(wxS("wxPdfFontParserType1::ParseCharStrings: ")) +
                   wxString(_("Invalid Type1 file format")));
        break;
      }
      n++;
    }
  }
}

void
wxPdfFontParserType1::ParsePrivate(wxInputStream* stream)
{
  wxUnusedVar(stream);
  m_privateFound = true;
}

// --- Convert Mac format to PFB format

// CRC table and routine copied from macutils-2.0b3

static unsigned short crctab[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

// Update a CRC check on the given buffer.

static int
crcbuf(int crc, unsigned int len, const char* buf)
{
  const unsigned char *ubuf = (const unsigned char *)buf;
  while (len--)
    crc = ((crc << 8) & 0xFF00) ^ crctab[((crc >> 8) & 0xFF) ^ *ubuf++];
  return crc;
}

#define APPLESINGLE_MAGIC 0x00051600
#define APPLEDOUBLE_MAGIC 0x00051607
#define POST_TYPE         0x504F5354

wxMemoryOutputStream*
wxPdfFontParserType1::ConvertMACtoPFB(wxInputStream* macFontStream)
{
  wxMemoryOutputStream* pfbStream = NULL;
  bool ok = true;
  m_inFont = macFontStream;

  // check magic number
  SeekI(0);
  int magic = ReadInt();

  int resourceOffset = -1;
  if (magic == APPLESINGLE_MAGIC || magic == APPLEDOUBLE_MAGIC)
  {
    // AppleSingle or AppleDouble file
    int i, n;
    SeekI(24);
    n = ReadShort();
    for (i = 0; i < n; ++i)
    {
      int type = ReadInt();
      if (type == 0)
      {
        // bad entry descriptor
        break;
      }
      if (type == 2)
      {
        // resource fork entry
        resourceOffset = ReadInt();
      }
      else
      {
        ReadInt();
      }
      ReadInt();
    }
    ok = (resourceOffset >= 0);
  }
  else if ((magic & 0xFF000000) == 0)
  {
    // MacBinary (I or II) file

    // Check "version" bytes at offsets 0 and 74
    SeekI(0);
    ok = (ReadByte() == 0);
    if (ok)
    {
      SeekI(74);
      ok = (ReadByte() == 0);
    }

    // Check file length
    if (ok)
    {
      SeekI(1);
      int i = ReadByte();
      ok = (i >= 0 && i <= 63);
    }
    if (ok)
    {
      SeekI(83);
      int i = ReadInt();
      int j = ReadInt();
      ok = (i >= 0 && j >= 0 && i < 0x800000 && j < 0x800000);
    }

    // Check CRC
    if (ok)
    {
      char buf[124];
      SeekI(0);
      m_inFont->Read(buf, 124);
      if (crcbuf(0, 124, buf) != ReadShort())
      {
        SeekI(82);
        ok = (ReadByte() == 0);
      }
    }

    // Calculate resource offset
    if (ok)
    {
      SeekI(83);
      int dataForkSize = ReadInt();
      ReadInt();

      // round data_fork_size up to multiple of 128
      if (dataForkSize % 128 != 0)
      {
        dataForkSize += 128 - dataForkSize % 128;
      }
      resourceOffset = 128 + dataForkSize;
    }
  }
  else
  {
    ok = false;
  }

  if (ok)
  {
    pfbStream = new wxMemoryOutputStream();
    int resourceDataOffset, resourceMapOffset, typeListOffset;

    // read offsets from resource fork header
    SeekI(resourceOffset);
    resourceDataOffset = resourceOffset + ReadInt();
    resourceMapOffset = resourceOffset + ReadInt();

    // read type list offset from resource map header
    SeekI(resourceMapOffset + 24);
    typeListOffset = resourceMapOffset + ReadShort();

    // read type list
    SeekI(typeListOffset);
    int numTypes = ReadShort() + 1;

    wxMemoryOutputStream* currentBlock = NULL;
    unsigned char lastBlockType = 0xFF;
    int numExtracted = 0;
    // find POST type
    while (numTypes--)
    {
      if (ReadInt() == POST_TYPE)
      {
        int nResource = 1 + ReadShort();
        int listOffset = typeListOffset + ReadShort();
        int posResource = 0;
        int idWanted = 501;
        bool secondTime = true;
        SeekI(listOffset);
        // Read resources sequentially, starting with ID 501
        // until "end" resource or no next resource is found
        while (posResource < nResource)
        {
          int offset = TellI();
          int id = ReadShort();
          if (id == idWanted)
          {
            ReadShort();
            ReadByte();
            numExtracted++;
            unsigned char b1 = ReadByte();
            unsigned char b2 = ReadByte();
            unsigned char b3 = ReadByte();
            int relOffset = (b1 << 16) | (b2 << 8) | b3;

            SeekI(resourceDataOffset + relOffset);
            int blockLen = ReadInt() - 2;   // subtract type field
            unsigned char blockType = ReadByte();
            ReadByte();

            if (blockType != lastBlockType)
            {
              unsigned char b;
              if (currentBlock != NULL)
              {
                wxMemoryInputStream block(*currentBlock);
                // Write block marker
                b = PFB_BLOCK_MARKER;
                pfbStream->Write(&b, 1);
                // Write block type
                pfbStream->Write(&lastBlockType, 1);
                // Write block size
                int blockSize = currentBlock->TellO();
                b = blockSize & 0xff;
                pfbStream->Write(&b, 1);
                b = (blockSize >> 8) & 0xff;
                pfbStream->Write(&b, 1);
                b = (blockSize >> 16) & 0xff;
                pfbStream->Write(&b, 1);
                b = (blockSize >> 24) & 0xff;
                pfbStream->Write(&b, 1);
                // Write block
                pfbStream->Write(block);
                delete currentBlock;
              }
              if (blockType != PFB_BLOCK_END)
              {
                currentBlock = new wxMemoryOutputStream();
              }
              else
              {
                b = PFB_BLOCK_MARKER;
                pfbStream->Write(&b, 1);
                b = PFB_BLOCK_DONE;
                pfbStream->Write(&b, 1);
                currentBlock = NULL;
              }
              lastBlockType = blockType;
            }
            if (blockType != PFB_BLOCK_END)
            {
              ReadBinary(*m_inFont, TellI(), blockLen, *currentBlock);
            }
            else
            {
              break;
            }

            secondTime = false;
            ++idWanted;
          }
          SeekI(offset + 12);
          posResource++;
          if (posResource >= nResource && !secondTime)
          {
            SeekI(listOffset);
            posResource = 0;
          }
        }
        break;
      }
      else
      {
        ReadShort();
        ReadShort();
      }
    }
    if (numExtracted == 0)
    {
    }
  }
  return pfbStream;
}

#endif // wxUSE_UNICODE
