///////////////////////////////////////////////////////////////////////////////
// Name:        pdffont_ttf.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-11-20
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdftruetype.cpp Implementation of TrueType Font support classes

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
#include "wx/pdftruetype.h"

wxPdfTrueTypeSubset::wxPdfTrueTypeSubset(const wxString& fileName)
{
  m_fileName = fileName;
  m_tableDirectory = new wxPdfTableDirectory();
  m_includeCmap = false;
}

wxPdfTrueTypeSubset::~wxPdfTrueTypeSubset()
{
  delete [] m_newGlyfTable;
  delete [] m_newLocaTableStream;
  delete [] m_newLocaTable;
  delete [] m_locaTable;
  wxPdfTableDirectory::iterator entry = m_tableDirectory->begin();
  for (entry = m_tableDirectory->begin(); entry != m_tableDirectory->end(); entry++)
  {
    delete entry->second;
  }
  delete m_tableDirectory;
}

wxMemoryOutputStream*
wxPdfTrueTypeSubset::CreateSubset(wxInputStream* inFont, wxPdfSortedArrayInt* usedGlyphs, bool includeCmap)
{
  m_inFont = inFont;
  m_usedGlyphs = usedGlyphs;
  m_includeCmap = includeCmap;
  m_outFont = NULL;

  if (ReadTableDirectory())
  {
    if (ReadLocaTable())
    {
      if (CheckGlyphs())
      {
        CreateNewTables();
        WriteSubsetFont();
      }
    }
  }
  return m_outFont;
}

bool
wxPdfTrueTypeSubset::ReadTableDirectory()
{
  bool ok = true;
  m_inFont->SeekI(0);
  int id = ReadInt();
  if (id == 0x00010000)
  {
    int num_tables = ReadUShort();
    SkipBytes(6);
    int k;
    for (k = 0; k < num_tables; ++k)
    {
      wxString tag = ReadString(4);
      wxPdfTableDirectoryEntry* tableLocation = new wxPdfTableDirectoryEntry();
      tableLocation->m_checksum = ReadInt();
      tableLocation->m_offset = ReadInt();
      tableLocation->m_length = ReadInt();
      (*m_tableDirectory)[tag] = tableLocation;
    }
  }
  else
  {
    wxLogError(wxString(_T("wxPdfTrueTypeSubset::ReadTableDirectory: '")) + m_fileName + wxString(_T("' is not a TrueType Font file.")));
    ok = false;
  }
  return ok;
}
    
bool
wxPdfTrueTypeSubset::ReadLocaTable()
{
  bool ok = false;
  static int HEAD_LOCA_FORMAT_OFFSET = 51;

  wxPdfTableDirectoryEntry* tableLocation;
  wxPdfTableDirectory::iterator entry = m_tableDirectory->find(_T("head"));
  if (entry != m_tableDirectory->end())
  {
    tableLocation = entry->second;
    m_inFont->SeekI(tableLocation->m_offset + HEAD_LOCA_FORMAT_OFFSET);
    m_locaTableIsShort = (ReadUShort() == 0);
    entry = m_tableDirectory->find(_T("loca"));
    if (entry != m_tableDirectory->end())
    {
      tableLocation = entry->second;
      m_inFont->SeekI(tableLocation->m_offset);
      m_locaTableSize = (m_locaTableIsShort) ? tableLocation->m_length / 2 : tableLocation->m_length / 4;
      m_locaTable = new int[m_locaTableSize];
      size_t k;
      for (k = 0; k < m_locaTableSize; k++)
      {
        m_locaTable[k] = (m_locaTableIsShort) ? ReadUShort() * 2 : ReadInt();
      }
      ok = true;
    }
    else
    {
      wxLogError(wxString(_T("wxPdfTrueTypeSubset::ReadLocaTable: Table 'loca' does not exist in '")) + m_fileName + wxString(_T("'.")));
    }
  }
  else
  {
    wxLogError(wxString(_T("wxPdfTrueTypeSubset::ReadLocaTable: Table 'head' does not exist in '")) + m_fileName + wxString(_T("'.")));
  }
  return ok;
}
    
bool
wxPdfTrueTypeSubset::CheckGlyphs()
{
  bool ok = false;
  wxPdfTableDirectoryEntry* tableLocation;
  wxPdfTableDirectory::iterator entry = m_tableDirectory->find(_T("glyf"));
  if (entry != m_tableDirectory->end())
  {
    tableLocation = entry->second;
    int glyph0 = 0;
    if (m_usedGlyphs->Index(glyph0) == wxNOT_FOUND)
    {
      m_usedGlyphs->Add(glyph0);
    }
    m_glyfTableOffset = tableLocation->m_offset;
    size_t k;
    for (k = 0; k < m_usedGlyphs->GetCount(); k++)
    {
      FindGlyphComponents(m_usedGlyphs->Item(k));
    }
    ok = true;
  }
  else
  {
    wxLogError(wxString(_T("wxPdfTrueTypeSubset::CheckGlyphs: Table 'glyf' does not exist in '")) + m_fileName + wxString(_T("'.")));
  }
  return ok;
}

void
wxPdfTrueTypeSubset::FindGlyphComponents(int glyph)
{
  static int FLAG_HAS_WORD_ARGS       =   1;
  static int FLAG_HAS_SCALE           =   8;
  static int FLAG_HAS_MORE_COMPONENTS =  32;
  static int FLAG_HAS_SCALE_XY        =  64;
  static int FLAG_HAS_TWO_BY_TWO      = 128;

  int glyphOffset = m_locaTable[glyph];
  if (glyphOffset == m_locaTable[glyph + 1])
  {
    // glyph has no contour
    return;
  }
  m_inFont->SeekI(m_glyfTableOffset + glyphOffset);
  int numberContours = ReadShort();
  if (numberContours >= 0)
  {
    // glyph has contours (not glyph components)
    return;
  }
  SkipBytes(8);
  for (;;)
  {
    int flags = ReadUShort();
    int glyph = (int) ReadUShort();
    if (m_usedGlyphs->Index(glyph) == wxNOT_FOUND)
    {
      m_usedGlyphs->Add(glyph);
    }
    if ((flags & FLAG_HAS_MORE_COMPONENTS) == 0)
    {
      // glyph does not have more components
      return;
    }
    int skip = ((flags & FLAG_HAS_WORD_ARGS) != 0) ? 4 : 2;
    if ((flags & FLAG_HAS_SCALE) != 0)
    {
      skip += 2;
    }
    else if ((flags & FLAG_HAS_SCALE_XY) != 0)
    {
      skip += 4;
    }
    if ((flags & FLAG_HAS_TWO_BY_TWO) != 0)
    {
      skip += 8;
    }
    SkipBytes(skip);
  }
}

void
wxPdfTrueTypeSubset::CreateNewTables()
{
  size_t usedGlyphCount = m_usedGlyphs->GetCount();
  size_t k;
  m_newLocaTable = new int[m_locaTableSize];

  // Calculate new 'glyf' table size
  m_newGlyfTableSize = 0;
  for (k = 0; k < usedGlyphCount; k++)
  {
    int glyph = (*m_usedGlyphs)[k];
    m_newGlyfTableSize += m_locaTable[glyph + 1] - m_locaTable[glyph];
  }
  m_newGlyfTableRealSize = m_newGlyfTableSize;
  m_newGlyfTableSize =  (m_newGlyfTableSize + 3) & (~3);
  m_newGlyfTable = new char[m_newGlyfTableSize];

  // Initialize new 'glyf' table
  for (k = 0; k < m_newGlyfTableSize; k++)
  {
    m_newGlyfTable[k] = 0;
  }

  // Copy used glyphs to new 'glyf' table
  int newGlyphOffset = 0;
  size_t glyphIndex = 0;
  for (k = 0; k < m_locaTableSize; k++)
  {
    m_newLocaTable[k] = newGlyphOffset;
    if (glyphIndex < usedGlyphCount && (size_t)(*m_usedGlyphs)[glyphIndex] == k)
    {
      glyphIndex++;
      m_newLocaTable[k] = newGlyphOffset;
      int glyphOffset = m_locaTable[k];
      int glyphLength = m_locaTable[k + 1] - glyphOffset;
      if (glyphLength > 0)
      {
        m_inFont->SeekI(m_glyfTableOffset + glyphOffset);
        m_inFont->Read(&m_newGlyfTable[newGlyphOffset], glyphLength);
        newGlyphOffset += glyphLength;
      }
    }
  }

  // Convert new 'loca' table to byte stream
  m_locaTableRealSize = (m_locaTableIsShort) ? m_locaTableSize * 2 : m_locaTableSize * 4;
  m_newLocaTableStreamSize = (m_locaTableRealSize + 3) & (~3);
  m_newLocaTableStream = new char[m_newLocaTableStreamSize];
  for (k = 0; k < m_newLocaTableStreamSize; k++)
  {
    m_newLocaTableStream[k] = 0;
  }
  int offset = 0;
  for (k = 0; k < m_locaTableSize; k++)
  {
    if (m_locaTableIsShort)
    {
      WriteShortToBuffer(m_newLocaTable[k] / 2, &m_newLocaTableStream[offset]);
      offset += 2;
    }
    else
    {
      WriteIntToBuffer(m_newLocaTable[k], &m_newLocaTableStream[offset]);
      offset += 4;
    }
  }
}
    
void
wxPdfTrueTypeSubset::WriteSubsetFont()
{
  static wxChar* tableNamesDefault[] = {
    _T("cvt "), _T("fpgm"), _T("glyf"), _T("head"),
    _T("hhea"), _T("hmtx"), _T("loca"), _T("maxp"), _T("prep"),
    NULL
  };
  static wxChar* tableNamesCmap[] = {
    _T("cmap"), _T("cvt "), _T("fpgm"), _T("glyf"), _T("head"),
    _T("hhea"), _T("hmtx"), _T("loca"), _T("maxp"), _T("prep"),
    NULL
  };
  static wxChar* tableNamesExtra[] = {
    _T("OS/2"), _T("cmap"), _T("cvt "), _T("fpgm"), _T("glyf"), _T("head"),
    _T("hhea"), _T("hmtx"), _T("loca"), _T("maxp"), _T("name"), _T("prep"),
    NULL
  };
  wxPdfTableDirectoryEntry* tableLocation;
  wxPdfTableDirectory::iterator entry;
  int k;
  wxChar** tableNames = (m_includeCmap) ? tableNamesCmap : tableNamesDefault;
  int tableCount = 0;
  while (tableNames[tableCount] != NULL)
  {
    tableCount++;
  }

  int tablesUsed = 2;
  int tableLength = 0;
  for (k = 0; k < tableCount; k++)
  {
    wxString name = tableNames[k];
    if (name != _T("glyf") && name != _T("loca"))
    {
      entry = m_tableDirectory->find(name);
      if (entry != m_tableDirectory->end())
      {
        tableLocation = entry->second;
        ++tablesUsed;
      }
    }
  }

  // Write header and table directory
  int tableOffset = 16 * tablesUsed + 12;
  m_outFont = new wxMemoryOutputStream();
  WriteInt(0x00010000);
  WriteShort(tablesUsed);
//                               0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
  static int entrySelectors[] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4};
  int selector = entrySelectors[tablesUsed];
  WriteShort((1 << selector) * 16);
  WriteShort(selector);
  WriteShort((tablesUsed - (1 << selector)) * 16);
  for (k = 0; k < tableCount; ++k)
  {
    wxString name = tableNames[k];
    entry = m_tableDirectory->find(name);
    if (entry != m_tableDirectory->end())
    {
      tableLocation = entry->second;
      WriteString(name);
      if (name == _T("glyf"))
      {
        WriteInt(CalculateChecksum(m_newGlyfTable, m_newGlyfTableSize));
        tableLength = m_newGlyfTableRealSize;
      }
      else if (name == _T("loca"))
      {
        WriteInt(CalculateChecksum(m_newLocaTableStream, m_newLocaTableStreamSize));
        tableLength = m_locaTableRealSize;
      }
      else
      {
        WriteInt(tableLocation->m_checksum);
        tableLength = tableLocation->m_length;
      }
      WriteInt(tableOffset);
      WriteInt(tableLength);
      tableOffset += (tableLength + 3) & (~3);
    }
  }

  // Write new 'loca' and 'glyf' tables and copy all others
  for (k = 0; k < tableCount; k++)
  {
    wxString name = tableNames[k];
    entry = m_tableDirectory->find(name);
    if (entry != m_tableDirectory->end())
    {
      tableLocation = entry->second;
      if (name == _T("glyf"))
      {
        m_outFont->Write(m_newGlyfTable, m_newGlyfTableSize);
      }
      else if (name == _T("loca"))
      {
        m_outFont->Write(m_newLocaTableStream, m_newLocaTableStreamSize);
      }
      else
      {
        char buffer[1024];
        m_inFont->SeekI(tableLocation->m_offset);
        int tableLength = tableLocation->m_length;
        int bufferLength;
        while (tableLength > 0)
        {
          bufferLength = (tableLength > 1024) ? 1024 : tableLength;
          m_inFont->Read(buffer, bufferLength);
          m_outFont->Write(buffer, bufferLength);
          tableLength -= bufferLength;
        }
        int paddingLength = ((tableLocation->m_length + 3) & (~3)) - tableLocation->m_length;
        if (paddingLength > 0)
        {
          int pad;
          for (pad = 0; pad < paddingLength; pad++)
          {
            buffer[pad] = 0;
          }
          m_outFont->Write(buffer, paddingLength);
        }
      }
    }
  }
}
    
int
wxPdfTrueTypeSubset::CalculateChecksum(char* b, int length)
{
  int len = length / 4;
  int d0 = 0;
  int d1 = 0;
  int d2 = 0;
  int d3 = 0;
  int ptr = 0;
  for (int k = 0; k < len; ++k)
  {
    d3 += (int)b[ptr++] & 0xff;
    d2 += (int)b[ptr++] & 0xff;
    d1 += (int)b[ptr++] & 0xff;
    d0 += (int)b[ptr++] & 0xff;
  }
  return d0 + (d1 << 8) + (d2 << 16) + (d3 << 24);
}

void
wxPdfTrueTypeSubset::SkipBytes(int count)
{
  m_inFont->SeekI(count, wxFromCurrent);
}

int
wxPdfTrueTypeSubset::ReadInt()
{
  // Read a 4-byte integer from file (big endian)
  int i32;
  m_inFont->Read(&i32, 4);
  return wxINT32_SWAP_ON_LE(i32);
}

short
wxPdfTrueTypeSubset::ReadShort()
{
  // Read a 2-byte integer from file (big endian)
  short i16;
  m_inFont->Read(&i16, 2);
  return wxINT16_SWAP_ON_LE(i16);
}

unsigned short
wxPdfTrueTypeSubset::ReadUShort()
{
  // Read a unsigned 2-byte integer from file (big endian)
  unsigned short i16;
  m_inFont->Read(&i16, 2);
  return wxUINT16_SWAP_ON_LE(i16);
}

wxString
wxPdfTrueTypeSubset::ReadString(int length)
{
  wxString str = wxEmptyString;
  char* buffer = new char[length];
  m_inFont->Read(buffer, length);
  int j;
  for (j = 0; j < length; j++)
  {
    str.Append(buffer[j]);
  }
  delete [] buffer;
  return str;
}
    
void
wxPdfTrueTypeSubset::WriteShort(int n)
{
  char buffer[2];
  WriteShortToBuffer(n, buffer);
  m_outFont->Write(buffer, 2);
}

void
wxPdfTrueTypeSubset::WriteShortToBuffer(int n, char buffer[2])
{
  buffer[0] = (char)((n >> 8) & 0xff);
  buffer[1] = (char)((n     ) & 0xff);
}

void
wxPdfTrueTypeSubset::WriteInt(int n)
{
  char buffer[4];
  WriteIntToBuffer(n, buffer);
  m_outFont->Write(buffer, 4);
}

void
wxPdfTrueTypeSubset::WriteIntToBuffer(int n, char buffer[4])
{
  buffer[0] = (char)((n >> 24) & 0xff);
  buffer[1] = (char)((n >> 16) & 0xff);
  buffer[2] = (char)((n >>  8) & 0xff);
  buffer[3] = (char)((n      ) & 0xff);
}

void
wxPdfTrueTypeSubset::WriteString(const wxString& s)
{
  int len = s.Length();;
  char* buffer = new char[len];
  int j;
  for (j = 0; j < len; j++)
  {
    buffer[j] = s[j];
  }
  m_outFont->Write(buffer, len);
  delete [] buffer;
}
    
