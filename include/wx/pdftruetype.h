///////////////////////////////////////////////////////////////////////////////
// Name:        pdffont_ttf.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-11-20
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdftruetype.h Interface of the TrueType Font support classes

#ifndef _PDFTRUETYPE_H_
#define _PDFTRUETYPE_H_

// wxWidgets headers

#include <wx/string.h>
#include <wx/wfstream.h>

#include "wx/pdfdocdef.h"
#include "wx/pdffont.h"

/// Class representing a table directory entry for TrueType fonts (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfTableDirectoryEntry
{
public:
  int m_checksum;  ///< Table checksum
  int m_offset;    ///< Table offset
  int m_length;    ///< Table length
};

/// Hash map class for used/embedded fonts
WX_DECLARE_STRING_HASH_MAP(wxPdfTableDirectoryEntry*, wxPdfTableDirectory);

/// Class representing TrueType Font Subsets. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfTrueTypeSubset
{
public:
  /// Default constructor
  wxPdfTrueTypeSubset(const wxString& fileName);

  /// Default destructor
  virtual ~wxPdfTrueTypeSubset();

  /// Create subset of a font
  wxMemoryOutputStream* CreateSubset(wxInputStream* inFont,
                                     wxPdfSortedArrayInt* glyphsUsed,
                                     bool includeCmap = false);

protected:
  /// Read table directory
  bool ReadTableDirectory();

  /// Read 'loca' table
  bool ReadLocaTable();

  /// Check glyphs
  bool CheckGlyphs();
  void FindGlyphComponents(int glyph);
  void CreateNewTables();
  //void LocaTobytes();
  void WriteSubsetFont();
  int CalculateChecksum(char* b, int length);

  void SkipBytes(int count);
  int ReadInt();
  short ReadShort();
  unsigned short ReadUShort();
  wxString ReadString(int length);

  void WriteShort(int n);
  void WriteInt(int n);
  void WriteString(const wxString& s);
  void WriteShortToBuffer(int n, char buffer[2]);
  void WriteIntToBuffer(int n, char buffer[4]);

private:
  wxString              m_fileName; ///< File name of the font file
  wxInputStream*        m_inFont;   ///< Font file input stream
  wxMemoryOutputStream* m_outFont;  ///< Subset output stream

  wxPdfTableDirectory*  m_tableDirectory;
  wxPdfSortedArrayInt*  m_usedGlyphs;
  
  bool   m_includeCmap;

  bool   m_locaTableIsShort;
  int*   m_locaTable;
  size_t m_locaTableSize;
  int    m_locaTableRealSize;

  int*   m_newLocaTable;
  char*  m_newLocaTableStream;
  size_t m_newLocaTableStreamSize;

  int    m_glyfTableOffset;
  char*  m_newGlyfTable;
  size_t m_newGlyfTableSize;
  int    m_newGlyfTableRealSize;
};

#endif
