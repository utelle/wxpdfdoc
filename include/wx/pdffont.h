///////////////////////////////////////////////////////////////////////////////
// Name:        pdffont.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-08-04
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffont.h Interface of the wxPdfFont class

#ifndef _PDFFONT_H_
#define _PDFFONT_H_

#include <wx/dynarray.h>
#include <wx/hashmap.h>
#include <wx/strconv.h>
#include <wx/xml/xml.h>

#include "wx/pdfdocdef.h"

/// Sorted integer array
//WX_DEFINE_SORTED_USER_EXPORTED_ARRAY_INT(int, wxPdfSortedArrayInt, WXDLLIMPEXP_PDFDOC);
WX_DEFINE_SORTED_ARRAY_INT(int, wxPdfSortedArrayInt);

/// Class representing a font description. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDescription
{
public:
  /// Default constructor
  wxPdfFontDescription();
  
  /// Constructor
  wxPdfFontDescription(int ascent, int descent, int capHeight, int flags,
                       const wxString &fontBBox, int italicAngle, int stemV,
                       int missingWidth, int xHeight,
                       int underlinePosition, int underlineThickness);

  /// Default destructor
  ~wxPdfFontDescription();

  /// Set ascender
  void SetAscent(int ascent) { m_ascent = ascent; };

  /// Get ascender
  int  GetAscent() const { return m_ascent; };
  
  /// Set descender
  void SetDescent(int descent) { m_descent = descent;};

  /// Get descender
  int  GetDescent() const { return m_descent;};
  
  /// Set CapHeight
  void SetCapHeight(int capHeight) { m_capHeight = capHeight; };

  /// Get CapHeight
  int  GetCapHeight() const { return m_capHeight; };
  
  /// Set font flags
  void SetFlags(int flags) { m_flags = flags; };

  /// Get font flags
  int  GetFlags() const { return m_flags; };
  
  /// Set font bounding box
  void SetFontBBox(const wxString& fontBBox) { m_fontBBox = fontBBox; };

  /// Get font bounding box
  wxString GetFontBBox() const { return m_fontBBox; };
  
  /// Set italic angle
  void SetItalicAngle(int italicAngle) { m_italicAngle = italicAngle; };

  /// Get italic angle
  int  GetItalicAngle() const { return m_italicAngle; };
  
  /// Set StemV
  void SetStemV(int stemV) { m_stemV = stemV; };

  /// Get StemV
  int  GetStemV() const { return m_stemV; };

  /// Set missing character width
  void SetMissingWidth(int missingWidth) { m_missingWidth = missingWidth; };

  /// Get missing character width
  int  GetMissingWidth() const { return m_missingWidth; };
  
  /// Set xHeight
  void SetXHeight(int xHeight) { m_xHeight = xHeight; };

  /// Get xHeight
  int  GetXHeight() const { return m_xHeight; };
  
  /// Set underline position
  void SetUnderlinePosition(int underlinePosition) { m_underlinePosition = underlinePosition; };

  /// Get underline position
  int  GetUnderlinePosition() const { return m_underlinePosition; };
  
  /// Set underline thickness
  void SetUnderlineThickness(int underlineThickness) { m_underlineThickness = underlineThickness; };

  /// Get underline thickness
  int  GetUnderlineThickness() const { return m_underlineThickness; };

private:
  int      m_ascent;                  ///< Ascender
  int      m_descent;                 ///< Descender
  int      m_capHeight;               ///< CapHeight
  int      m_flags;                   ///< Font flags
  wxString m_fontBBox;                ///< Font bounding box
  int      m_italicAngle;             ///< Angle for italics
  int      m_stemV;                   ///< StemV
  int      m_missingWidth;            ///< Missing character width
  int      m_xHeight;                 ///< xHeight
  int      m_underlinePosition;       ///< Underline position
  int      m_underlineThickness;      ///< Underline thickness
};

WX_DECLARE_HASH_MAP(long, short, wxIntegerHash, wxIntegerEqual, wxPdfCharWidthMap);

/// Base class for all fonts and for representing the core fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFont
{
public:
  /// Font constructor
  wxPdfFont(int index, const wxString& name = wxEmptyString, short* cwArray = NULL,
            const wxPdfFontDescription& desc = wxPdfFontDescription());

  /// Default destructor
  virtual ~wxPdfFont();

  /// Set font index
  void SetIndex(int index) { m_index = index; }

  /// Get font index
  int  GetIndex() { return m_index; }

  /// Set font object index
  void SetObjIndex(int n) { m_n = n; }

  /// Get font object index
  int  GetObjIndex() { return m_n; }

  /// Set font file index
  void SetFileIndex(int fn) { m_fn = fn; }

  /// Get font file index
  int  GetFileIndex() { return m_fn; }

  /// Set encoding differences index
  void SetDiffIndex(int ndiff) { m_ndiff = ndiff; }

  /// Get encoding differences index
  int  GetDiffIndex() { return m_ndiff; }

  /// Set font type
  void SetType(const wxString& type) { m_type = type; }

  /// Get font type
  wxString GetType() { return m_type; }

  /// Set font name
  void SetName(const wxString& name) { m_name = name; }

  /// Get font name
  wxString GetName();

  /// Set underline position
  void SetUnderlinePosition(int up) { m_desc.SetUnderlinePosition(up); }

  /// Get underline position
  int  GetUnderlinePosition() { return m_desc.GetUnderlinePosition(); }

  /// Set underline thickness
  void SetUnderlineThickness(int ut) { m_desc.SetUnderlineThickness(ut); }

  /// Get underline thickness
  int  GetUnderlineThickness() { return m_desc.GetUnderlineThickness(); }

  /// Get bounding box top position
  int GetBBoxTopPosition();

  /// Set encoding
  void SetEncoding(const wxString& enc) { m_enc = enc; }

  /// Get encoding
  wxString GetEncoding() { return m_enc; }

  /// Check whether the font has differences to WinAnsi encoding
  bool HasDiffs() { return m_diffs.Length() > 0; }

  /// Set encoding differences
  void SetDiffs(const wxString& diffs) { m_diffs = diffs; }

  /// Get encoding differences
  wxString GetDiffs() { return m_diffs; }

  /// Set path of font files
  void SetFilePath(const wxString& path) { m_path = path; }

  /// Get path of font files
  wxString GetFilePath() { return m_path; }

  /// Check whether the font has an associated font file
  bool HasFile() { return m_file.Length() > 0; }

  /// Set the name of the Character-to-Glyph mapping file
  void SetFontFile(const wxString& file) { m_file = file; }

  /// Get the name of the associated font file
  wxString GetFontFile() { return m_file; }

  /// Set the name of the Character-to-Glyph mapping file
  void SetCtgFile(const wxString& ctg) { m_ctg = ctg; }

  /// Get the name of the Character-to-Glyph mapping file
  wxString GetCtgFile() { return m_ctg; }

  /// Get font file size 1
  void SetSize1(int size1) { m_size1 = size1; }

  /// Get font file size 1
  int  GetSize1() { return m_size1; }

  /// Check whether the file has a size 2
  bool HasSize2() { return m_size2 > 0; }

  /// Get font file size 2
  void SetSize2(int size2) { m_size2 = size2; }

  /// Get font file size 2 (Type 1 only)
  int  GetSize2() { return m_size2; }

  /// Get the font's CMap (Type 0 only)
  wxString GetCMap() { return m_cmap; }

  /// Get font ordering
  wxString GetOrdering() { return m_ordering; }

  /// Get font supplement /Type 0 only)
  wxString GetSupplement() { return m_supplement; }

  /// Set char width map
  void SetCharWidthMap(wxPdfCharWidthMap* cw) { m_cw = cw; }

  /// Get char width map
  const wxPdfCharWidthMap* GetCharWidthMap() { return m_cw; }

  /// Set glyph number map
  void SetGlyphNumberMap(wxPdfCharWidthMap* gn) { m_gn = gn; }

  /// Get glyph number map
  const wxPdfCharWidthMap* GetGlyphNumberMap() { return m_gn; }

  /// Create a subset name prefix
  wxString CreateSubsetPrefix() const;

  /// Set subset flag if font subsetting is supported
  void SetSubset(bool subset);

  /// Get the character width array as string
  virtual wxString GetWidthsAsString();
  
  /// Get the width of a string
  virtual double GetStringWidth(const wxString& s);

  /// Update the list of used characters
  virtual void UpdateUsedChars(const wxString& s);

  /// Check whether the font supports subsetting
  virtual bool SupportsSubset() { return false; }

  /// Create font subset
  virtual int CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset);

  /// Set the font description
  virtual void SetDesc(const wxPdfFontDescription& desc) { m_desc = desc; }

  /// Get the font description
  virtual const wxPdfFontDescription& GetDesc() const { return m_desc; }

  /// Load the font metrics XML file
  virtual bool LoadFontMetrics(wxXmlNode* WXUNUSED(root)) { return false; };

  /// Convert character codes to glyph numbers
  virtual wxString ConvertCID2GID(const wxString& s);

#if wxUSE_UNICODE
  /// Get the associated encoding converter
  virtual wxMBConv* GetEncodingConv();
#endif

  /// Get the default WinAnsi encoding converter
  static wxMBConv* GetWinEncodingConv();

protected:
  int                  m_index; ///< Index number of this font
  int                  m_n;     ///< Font object index
  int                  m_fn;    ///< Font file index
  int                  m_ndiff; ///< Index of encoding differences object

  wxString             m_type;  ///< Font type
  wxString             m_name;  ///< Font name
  
  wxPdfCharWidthMap*   m_cw;    ///< Array with character widths
  wxPdfCharWidthMap*   m_gn;    ///< Array with glyph numbers

  wxPdfFontDescription m_desc;  ///< Font description

  wxString             m_enc;   ///< Encoding
  wxString             m_diffs; ///< Encoding differences

  wxString             m_path;  ///< Path of font files
  wxString             m_file;  ///< Filename of font program
  wxString             m_ctg;   ///< Filename of char to glyph mapping
  int                  m_size1; ///< TrueType file size or Type1 file size 1
  int                  m_size2; ///< Type1 file size 2

  wxString             m_cmap;          ///< CMap of a CID font
  wxString             m_ordering;      ///< Ordering of a CID font 
  wxString             m_supplement;    ///< Supplement of a CID font

  wxPdfSortedArrayInt* m_usedChars;     ///< Array of used characters
  bool                 m_subset;        ///< Flag whether the font uses subsetting
  bool                 m_subsetSupport; ///< Flag whether the font supports subsetting

  static wxMBConv*     ms_winEncoding;  ///< WinAnsi converter
};

/// Class representing TrueType fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontTrueType : public wxPdfFont
{
public:
  ///< Constructor
  wxPdfFontTrueType(int index);

  /// Default destructor
  virtual ~wxPdfFontTrueType();

  /// Get the width of a string
  virtual double GetStringWidth(const wxString& s);

  /// Load the font metrics XML file
  virtual bool LoadFontMetrics(wxXmlNode* root);

  /// Update the list of used characters
  virtual void UpdateUsedChars(const wxString& s);

  /// Check whether the font supports subsetting
  virtual bool SupportsSubset() { return m_subsetSupport; }

  /// Create a font subset
  virtual int CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset);

#if wxUSE_UNICODE
  /// Get the associated encoding converter
  virtual wxMBConv* GetEncodingConv() { return m_conv; }
#endif

protected:
  wxMBConv* m_conv;   ///< Assocated encoding converter
};

#if wxUSE_UNICODE

/// Class representing Unicode TrueType fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontTrueTypeUnicode : public wxPdfFont
{
public:
  ///< Constructor
  wxPdfFontTrueTypeUnicode(int index);

  /// Default destructor
  virtual ~wxPdfFontTrueTypeUnicode();

  /// Get the character width array as string
  virtual wxString GetWidthsAsString();

  /// Get the width of a string
  virtual double GetStringWidth(const wxString& s);

  /// Load the font metrics XML file
  virtual bool LoadFontMetrics(wxXmlNode* root);

  /// Check whether the font supports subsetting
  virtual bool SupportsSubset() { return true; }

  /// Create font subset
  virtual int CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset);

  /// Get the associated encoding converter
  virtual wxMBConv* GetEncodingConv() { return m_conv; }

protected:
  wxMBConv* m_conv;   ///< Assocated encoding converter
};

/// Class representing Unicode OpenType fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontOpenTypeUnicode : public wxPdfFont
{
public:
  ///< Constructor
  wxPdfFontOpenTypeUnicode(int index);

  /// Default destructor
  virtual ~wxPdfFontOpenTypeUnicode();

  /// Get the character width array as string
  virtual wxString GetWidthsAsString();

  /// Get the width of a string
  virtual double GetStringWidth(const wxString& s);

  /// Convert character codes to glyph numbers
  virtual wxString ConvertCID2GID(const wxString& s);

  /// Load the font metrics XML file
  virtual bool LoadFontMetrics(wxXmlNode* root);

  /// Check whether the font supports subsetting
  virtual bool SupportsSubset() { return false; }

  /// Create font subset
  virtual int CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset);

  /// Get the associated encoding converter
  virtual wxMBConv* GetEncodingConv() { return m_conv; }

protected:
  wxMBConv* m_conv;   ///< Assocated encoding converter
};

#endif // wxUSE_UNICODE

/// Class representing Type 1 fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontType1 : public wxPdfFont
{
public:
  ///< Constructor
  wxPdfFontType1(int index);

  /// Default destructor
  virtual ~wxPdfFontType1();

  /// Get the width of a string
  virtual double GetStringWidth(const wxString& s);

  /// Load the font metrics XML file
  virtual bool LoadFontMetrics(wxXmlNode* root);

  /// Create font subset
  virtual int CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset);

#if wxUSE_UNICODE
  /// Get the associated encoding converter
  virtual wxMBConv* GetEncodingConv() { return m_conv; }
#endif

protected:
  wxMBConv* m_conv;   ///< Assocated encoding converter
};

#if wxUSE_UNICODE

/// Class representing Type 0 fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontType0 : public wxPdfFont
{
public:
  ///< Constructor
  wxPdfFontType0(int index);

  /// Default destructor
  virtual ~wxPdfFontType0();

  /// Get the character width array as string
  virtual wxString GetWidthsAsString();

  /// Get the width of a string
  virtual double GetStringWidth(const wxString& s);

  /// Load the font metrics XML file
  virtual bool LoadFontMetrics(wxXmlNode* root);

  /// Create font subset
  virtual int CreateSubset(wxInputStream* fontFile, wxOutputStream* fontSubset);

  /// Get the associated encoding converter
  virtual wxMBConv* GetEncodingConv() { return m_conv; }

protected:
  bool HasHalfWidthRange() { return m_hwRange; }
  wxChar HalfWidthRangeFirst() { return m_hwFirst; }
  wxChar HalfWidthRangeLast() { return m_hwLast; }

  bool      m_hwRange;  ///< Flag whether the font has a half width range
  wxChar    m_hwFirst;  ///< CID of the first half width character
  wxChar    m_hwLast;   ///< CID of the last half width character

  wxMBConv* m_conv;     ///< Assocated encoding converter
};

#endif // wxUSE_UNICODE

#endif

