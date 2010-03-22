///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontmanager.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2008-08-10
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontmanager.cpp Implementation of the wxPdfFontManager class

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes
#include <wx/dir.h>
#include <wx/dynarray.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/filesys.h>
#include <wx/thread.h>
#include <wx/xml/xml.h>

#include "wx/pdffontmanager.h"
#include "wx/pdffontdata.h"
#include "wx/pdffontdatacore.h"
#include "wx/pdffontdataopentype.h"
#include "wx/pdffontdatatruetype.h"
#include "wx/pdffontdatatype0.h"
#include "wx/pdffontdatatype1.h"
#include "wx/pdffontparsertruetype.h"
#include "wx/pdffontparsertype1.h"

#if defined(__WXMSW__)
  #include <wx/msw/registry.h>
//  #include "wx/msw/private.h"
#elif defined(__WXGTK20__)
// TODO: Would testing for __WXGTK__ be sufficient?
  #include <fontconfig/fontconfig.h>
#elif defined(__WXMAC__)
#else
#endif

#include "wxmemdbg.h"

// Include core font data
#include "pdfcorefontdata.inc"

// wxPdfFontManager is a singleton.
// Critical sections are used to make access to it thread safe if necessary.
#if wxUSE_THREADS
static wxCriticalSection gs_csFontManager;
static wxCriticalSection gs_csFontData;
#endif

// To make reference counting and encoding conversion thread safe
// some methods of the wxFontData class use the critical section
// associated with the font manager.

wxMBConv* wxPdfFontData::ms_winEncoding = NULL;

wxMBConv*
wxPdfFontData::GetWinEncodingConv()
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontData);
#endif
  if (ms_winEncoding == NULL)
  {
    static wxCSConv winEncoding(wxFONTENCODING_CP1252);
    ms_winEncoding = &winEncoding;
  }
  return ms_winEncoding;
}

int
wxPdfFontData::IncrementRefCount()
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontData);
#endif
  return ++m_refCount;
}

int
wxPdfFontData::DecrementRefCount()
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontData);
#endif
  return --m_refCount;
}

class wxPdfFontListEntry
{
public:
  wxPdfFontListEntry(wxPdfFontData* fontData) : m_fontData(fontData)
  {
    if (m_fontData != NULL)
    {
      m_fontData->IncrementRefCount();
    }
  }

  ~wxPdfFontListEntry()
  {
    if (m_fontData != NULL && m_fontData->DecrementRefCount() == 0)
    {
      delete m_fontData;
    }
  }

  wxPdfFontListEntry(const wxPdfFontListEntry& p) : m_fontData(p.m_fontData)
  { 
    if (m_fontData != NULL)
    {
      m_fontData->IncrementRefCount();
    }
  }

  wxPdfFontListEntry& operator=(const wxPdfFontListEntry& p)
  { // DO NOT CHANGE THE ORDER OF THESE STATEMENTS!
    // (This order properly handles self-assignment)
    // (This order also properly handles recursion)
    wxPdfFontData* const prevFontData = m_fontData;
    m_fontData = p.m_fontData;
    if (m_fontData != NULL)
    {
      m_fontData->IncrementRefCount();
    }
    if (prevFontData != NULL && prevFontData->DecrementRefCount() == 0)
    {
      delete prevFontData;
    }
    return *this;
  }

  wxPdfFontData* GetFontData() const
  {
    return m_fontData;
  }

  wxPdfFontData* operator->()
  {
    return m_fontData;
  }

  wxPdfFontData& operator*()
  {
    return *m_fontData;
  }

private:
  wxPdfFontData* m_fontData;
};

WX_DEFINE_ARRAY_PTR(wxPdfFontListEntry*, wxPdfFontList);

/// Hashmap class for mapping font families
WX_DECLARE_STRING_HASH_MAP(wxArrayInt, wxPdfFontFamilyMap);

/// Hashmap class for mapping font names to font list entries
WX_DECLARE_STRING_HASH_MAP(int, wxPdfFontNameMap);

/// Hashmap class for mapping alias names to family names
WX_DECLARE_STRING_HASH_MAP(wxString, wxPdfFontAliasMap);

class wxPdfFontManagerBase
{
public:
  /// Default constructor
  wxPdfFontManagerBase();
  
  /// Default destructor
  ~wxPdfFontManagerBase();

  void AddSearchPath(const wxString& path);

  void AddSearchPath(const wxArrayString& pathArray);

  bool SetDefaultEmbed(bool embed);

  bool GetDefaultEmbed();
  
  bool SetDefaultSubset(bool subset);

  bool GetDefaultSubset();
  
  wxPdfFont RegisterFont(const wxString& fontFileName, const wxString& aliasName = wxEmptyString, int fontIndex = 0);

#if wxUSE_UNICODE
  wxPdfFont RegisterFont(const wxFont& font, const wxString& aliasName = wxEmptyString);

  int RegisterFontCollection(const wxString& fontCollectionFileName);
#endif

  bool RegisterFontCJK(const wxString& family);

#if wxUSE_UNICODE
  int RegisterSystemFonts();

  int RegisterFontDirectory(const wxString& directory, bool recursive = false);
#endif

  wxPdfFont GetFont(const wxString& fontName, int fontStyle = wxPDF_FONTSTYLE_REGULAR) const;

  wxPdfFont GetFont(const wxString& fontName, const wxString& fontStyle) const;

  wxPdfFont GetFont(size_t fontIndex) const;

  size_t GetFontCount() const;

  bool InitializeFontData(const wxPdfFont& font);

  bool FindFile(const wxString& fileName, wxString& fullFileName) const;

  static wxString ConvertStyleToString(int fontStyle);

private:
  void InitializeCoreFonts();

  bool RegisterFontCJK(const wxString& fontFileName, const wxString& fontStyle, const wxString& alias);

  wxPdfFontData* LoadFontFromXML(const wxString& fontFileName);

  bool IsRegistered(wxPdfFontData* fontData);

  bool AddFont(wxPdfFontData* fontData);

  bool AddFont(wxPdfFontData* fontData, wxPdfFont& font);

  wxPathList         m_searchPaths;

  wxPdfFontNameMap   m_fontNameMap;
  wxPdfFontFamilyMap m_fontFamilyMap;
  wxPdfFontAliasMap  m_fontAliasMap;
  wxPdfFontList      m_fontList;

  bool               m_defaultEmbed;
  bool               m_defaultSubset;
};

#include "wxmemdbg.h"

wxPdfFontManagerBase::wxPdfFontManagerBase()
{
  m_defaultEmbed = true;
  m_defaultSubset = true;
  {
    // Since InitializeCoreFonts uses locking, too, it is necessary
    // to create a new context, thus locking only the access of the
    // search path member.
#if wxUSE_THREADS
    wxCriticalSectionLocker locker(gs_csFontManager);
#endif
    m_searchPaths.Add(wxT("fonts"));
    m_searchPaths.AddEnvList(wxT("WXPDF_FONTPATH"));
  }
  InitializeCoreFonts();
}
  
wxPdfFontManagerBase::~wxPdfFontManagerBase()
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  m_fontNameMap.clear();
  m_fontFamilyMap.clear();
  m_fontAliasMap.clear();
  size_t n = m_fontList.GetCount();
  size_t j;
  for (j = 0; j < n; ++j)
  {
    delete m_fontList[j];
  }
  m_fontList.clear();
}

void
wxPdfFontManagerBase::AddSearchPath(const wxString& path)
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  m_searchPaths.Add(path);
}

void
wxPdfFontManagerBase::AddSearchPath(const wxArrayString& pathArray)
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  m_searchPaths.Add(pathArray);
}

bool
wxPdfFontManagerBase::FindFile(const wxString& fileName, wxString& fullFileName) const
{
  bool ok = false;
  wxFileName myFileName(fileName);
  fullFileName = wxEmptyString;
  if (myFileName.IsOk())
  {
    if (myFileName.IsRelative())
    {
      // Check whether the file is relative to the current working directory
      if (!(myFileName.MakeAbsolute() && myFileName.FileExists()))
      {
        // File not found, search in given search paths
#if wxUSE_THREADS
        wxCriticalSectionLocker locker(gs_csFontManager);
#endif
        wxString foundFileName = m_searchPaths.FindAbsoluteValidPath(fileName);
        if (!foundFileName.IsEmpty())
        {
          myFileName.Assign(foundFileName);
        }
      }
    }
    if (myFileName.FileExists() && myFileName.IsFileReadable())
    {
      // File exists and is accessible
      fullFileName = myFileName.GetFullPath();
      ok = true;
    }
    else
    {
      wxLogDebug(wxString(wxT("wxPdfFontManagerBase::FindFile: ")) +
                 wxString::Format(_("File '%s' does not exist."), fileName.c_str()));
    }
  }
  else
  {
    wxLogDebug(wxString(wxT("wxPdfFontManagerBase::FindFile: ")) +
               wxString::Format(_("File name '%s' is invalid."), fileName.c_str()));
  }
  return ok;
}

bool
wxPdfFontManagerBase::SetDefaultEmbed(bool embed)
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  bool previous = m_defaultEmbed;
  m_defaultEmbed = embed;
  return previous;
}
  
bool
wxPdfFontManagerBase::GetDefaultEmbed()
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  return m_defaultEmbed;
}
  
bool
wxPdfFontManagerBase::SetDefaultSubset(bool subset)
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  bool previous = m_defaultSubset;
  m_defaultSubset = subset;
  return previous;
}
  
bool
wxPdfFontManagerBase::GetDefaultSubset()
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  return m_defaultSubset;
}
  
wxPdfFont
wxPdfFontManagerBase::RegisterFont(const wxString& fontFileName, const wxString& aliasName, int fontIndex)
{
#if !wxUSE_UNICODE
  wxUnusedVar(fontIndex);
#endif
  wxPdfFont font;
  wxString fullFontFileName;
  if (FindFile(fontFileName, fullFontFileName))
  {
    wxFileName fileName(fullFontFileName);
    wxString ext = fileName.GetExt().Lower();
    if (ext.IsSameAs(wxT("ttf")) || ext.IsSameAs(wxT("otf")) || ext.IsSameAs(wxT("ttc")))
    {
#if wxUSE_UNICODE
      // TrueType font, OpenType font, or TrueType collection
      wxPdfFontParserTrueType fontParser;
      wxPdfFontData* fontData = fontParser.IdentifyFont(fileName.GetFullPath(), fontIndex);
      if (fontData != NULL)
      {
        fontData->SetAlias(aliasName);
        if (!AddFont(fontData, font))
        {
          delete fontData;
          wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                       wxString::Format(_("Font file '%s' already registered."), fontFileName.c_str()));
        }
      }
#else
      wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                 wxString::Format(_("Format of font file '%s' not supported."), fontFileName.c_str()));
#endif
    }
    else if (/* ext.IsSameAs(wxT("pfa")) || */ ext.IsSameAs(wxT("pfb")))
    {
      // TODO: allow Type1 fonts in PFA format (this requires encoding the binary section)
#if wxUSE_UNICODE
      // Type1 font
      wxPdfFontParserType1 fontParser;
      wxPdfFontData* fontData = fontParser.IdentifyFont(fileName.GetFullPath(), fontIndex);
      if (fontData != NULL)
      {
        fontData->SetAlias(aliasName);
        if (!AddFont(fontData, font))
        {
          delete fontData;
          wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                       wxString::Format(_("Font file '%s' already registered."), fontFileName.c_str()));
        }
      }
#else
      wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                 wxString::Format(_("Format of font file '%s' not supported."), fontFileName.c_str()));
#endif
    }
    else if (ext.IsSameAs(wxT("xml")))
    {
      // wxPdfDocument font description file
      wxPdfFontData* fontData = LoadFontFromXML(fullFontFileName);
      if (fontData != NULL)
      {
        fontData->SetAlias(aliasName);
        if (!AddFont(fontData, font))
        {
          delete fontData;
          wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                       wxString::Format(_("Font file '%s' already registered."), fontFileName.c_str()));
        }
      }
    }
    else
    {
      wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                 wxString::Format(_("Format of font file '%s' not supported."), fontFileName.c_str()));
    }
  }
  else
  {
    wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
               wxString::Format(_("Font file '%s' does not exist or is not readable."), fontFileName.c_str()));
  }
  return font;
}

#if wxUSE_UNICODE
wxPdfFont
wxPdfFontManagerBase::RegisterFont(const wxFont& font, const wxString& aliasName)
{
  wxPdfFont regFont;
#if defined(__WXMSW__)
  wxPdfFontParserTrueType fontParser;
  wxPdfFontData* fontData = fontParser.IdentifyFont(font);
  if (fontData != NULL)
  {
    fontData->SetAlias(aliasName);
    if (!AddFont(fontData, regFont))
    {
      wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                   wxString::Format(_("wxFont '%s' already registered."), font.GetFaceName().c_str()));
    }
  }
#elif defined(__WXGTK20__)
// TODO: Would testing for __WXGTK__ be sufficient?
#if 0
  // TODO: Do we need to load the fontconfig library?
  FcConfig* fc = FcInitLoadConfigAndFonts();
  if (fc == NULL)
  {
    wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
               wxString(_("FontConfig init failed.")));
    return 0;
  }
#endif

  wxString fontFileName = wxEmptyString;
  int fontFileIndex = 0;
  int slant = -1;
  int weight = -1;
  int width = -1;
  wxString fontDesc = font.GetNativeFontInfoUserDesc();
  wxString faceName = font.GetFaceName();
  wxCharBuffer faceNameBuffer = faceName.ToUTF8();
  const char* fontFamily = faceNameBuffer;
  
  // Check font slant
  if (fontDesc.Find(wxT("Oblique")) != wxNOT_FOUND) 
    slant = FC_SLANT_OBLIQUE;
  else if (fontDesc.Find(wxT("Italic")) != wxNOT_FOUND) 
    slant = FC_SLANT_ITALIC;
  else
    slant = FC_SLANT_ROMAN;
  
  // Check font weight
  if (fontDesc.Find(wxT("Book")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_BOOK;
  else if (fontDesc.Find(wxT("Medium")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_MEDIUM;
#ifdef FC_WEIGHT_ULTRALIGHT
  else if (fontDesc.Find(wxT("Ultra-Light")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_ULTRALIGHT;
#endif   
  else if (fontDesc.Find(wxT("Light")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_LIGHT;
  else if (fontDesc.Find(wxT("Semi-Bold")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_DEMIBOLD;
#ifdef FC_WEIGHT_ULTRABOLD
  else if (fontDesc.Find(wxT("Ultra-Bold")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_ULTRABOLD;
#endif   
  else if (fontDesc.Find(wxT("Bold")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_BOLD;
  else if (fontDesc.Find(wxT("Heavy")) != wxNOT_FOUND) 
    weight = FC_WEIGHT_BLACK;
  else
    weight = FC_WEIGHT_NORMAL;

  // Check font width
  if (fontDesc.Find(wxT("Ultra-Condensed")) != wxNOT_FOUND) 
    width = FC_WIDTH_ULTRACONDENSED;
  else if (fontDesc.Find(wxT("Extra-Condensed")) != wxNOT_FOUND) 
    width = FC_WIDTH_EXTRACONDENSED;
  else if (fontDesc.Find(wxT("Semi-Condensed")) != wxNOT_FOUND) 
    width = FC_WIDTH_SEMICONDENSED;
  else if (fontDesc.Find(wxT("Condensed")) != wxNOT_FOUND) 
    width = FC_WIDTH_CONDENSED;
  else if (fontDesc.Find(wxT("Ultra-Expanded")) != wxNOT_FOUND) 
    width = FC_WIDTH_ULTRAEXPANDED;
  else if (fontDesc.Find(wxT("Extra-Expanded")) != wxNOT_FOUND) 
    width = FC_WIDTH_EXTRAEXPANDED;
  else if (fontDesc.Find(wxT("Semi-Expanded")) != wxNOT_FOUND) 
    width = FC_WIDTH_SEMIEXPANDED;
  else if (fontDesc.Find(wxT("Expanded")) != wxNOT_FOUND) 
    width = FC_WIDTH_EXPANDED;
  else
    width = FC_WIDTH_NORMAL;

  FcResult res;
  FcPattern* matchPattern;
  matchPattern = FcPatternBuild(NULL,
                                FC_FAMILY, FcTypeString, (FcChar8*) fontFamily,
                                NULL);
  if (slant != -1) 
    FcPatternAddInteger(matchPattern, FC_SLANT, slant);
  if (weight != -1) 
    FcPatternAddInteger(matchPattern, FC_WEIGHT, weight);
  if (width != -1) 
    FcPatternAddInteger(matchPattern, FC_WIDTH, width);

  FcConfigSubstitute (NULL, matchPattern, FcMatchPattern);
  FcDefaultSubstitute (matchPattern);

  FcPattern* resultPattern = FcFontMatch (NULL, matchPattern, &res);
  if (resultPattern)
  {
    FcChar8* fileName;
    int id = 0;
    if (FcPatternGetString (resultPattern, FC_FILE, 0, &fileName) == FcResultMatch)
    {
      fontFileName = wxString::FromUTF8((char*) fileName);
    }
    else
    {
    }
    if (FcPatternGetInteger (resultPattern, FC_INDEX, 0, &id) == FcResultMatch)
    {
      fontFileIndex = id;
    }
    FcPatternDestroy (resultPattern);
  }
  FcPatternDestroy (matchPattern);

  if (!fontFileName.IsEmpty())
  {
    regFont = RegisterFont(fontFileName, aliasName, fontFileIndex);
  }
  else
  {
    wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
                 wxString::Format(_("Font file name not found for wxFont '%s'."), fontDesc.c_str()));
  }
#elif defined(__WXMAC__)
  wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
             wxString(_("Method 'RegisterFont' for wxFont instances is not yet available for platform WXMAC.")));
#else
  wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFont: ")) +
             wxString(_("Method 'RegisterFont' for wxFont instances is not available for your platform.")));
#endif
  return regFont;
}

int
wxPdfFontManagerBase::RegisterFontCollection(const wxString& fontCollectionFileName)
{
  int count = 0;
  wxString fullFontCollectionFileName;
  if (FindFile(fontCollectionFileName, fullFontCollectionFileName))
  {
    wxFileName fileName(fullFontCollectionFileName);
    if (fileName.IsOk() && fileName.GetExt().Lower().IsSameAs(wxT("ttc")))
    {
      wxPdfFontParserTrueType fontParser;
      int fontCount = fontParser.GetCollectionFontCount(fullFontCollectionFileName);
      int j;
      for (j = 0; j < fontCount; ++j)
      {
        wxPdfFont registeredFont = RegisterFont(fileName.GetFullPath(), wxEmptyString, j);
        if (registeredFont.IsValid())
        {
          ++count;
        }
      }
    }
    else
    {
      wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFontCollection: ")) +
                   wxString::Format(_("Font collection file '%s' has not the file extension '.ttc'."), fontCollectionFileName.c_str()));
    }
  }
  else
  {
    wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFontCollection: ")) +
               wxString::Format(_("Font collection file '%s' does not exist or is not readable."), fontCollectionFileName.c_str()));
  }
  return count;
}
#endif

bool
wxPdfFontManagerBase::RegisterFontCJK(const wxString& family)
{
  bool ok = false;
  wxString fontFileName = family.Lower() + wxString(wxT(".xml"));
  wxString fullFontFileName;
  if (FindFile(fontFileName, fullFontFileName))
  {
    ok = RegisterFontCJK(fullFontFileName, wxT(""), family);
    if (ok)
    {
      // Add all available styles (bold, italic and bold-italic)
      // For all styles the same font metric file is used.
      RegisterFontCJK(fullFontFileName, wxT(",Bold"), family);
      RegisterFontCJK(fullFontFileName, wxT(",Italic"), family);
      RegisterFontCJK(fullFontFileName, wxT(",BoldItalic"), family);
    }
  }
  else
  {
    wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterFontCJK: ")) +
               wxString::Format(_("CJK Font file '%s' for CJK family '%s' does not exist or is not readable."), fontFileName.c_str(), family.c_str()));
  }
  return ok;
}

#if wxUSE_UNICODE
int
wxPdfFontManagerBase::RegisterSystemFonts()
{
  int count = 0;
#if defined(__WXMSW__)
  wxString strFont;
  if (wxGetOsVersion() == wxOS_WINDOWS_NT)
  {
    strFont = wxT("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
  }
  else
  {
    strFont = wxT("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
  }

  wxRegKey* fontRegKey = new wxRegKey(strFont);
  fontRegKey->Open(wxRegKey::Read);
  // Retrieve the number of SubKeys and enumerate them
  size_t nSubKeys = 0, nMaxKeyLen = 0;
  size_t nValues = 0, nMaxValueLen = 0;
  fontRegKey->GetKeyInfo(&nSubKeys, &nMaxKeyLen, &nValues, &nMaxValueLen);
  wxString fontDirectory = wxGetOSDirectory();
  if (!wxEndsWithPathSeparator(fontDirectory))
  {
    fontDirectory += wxT("\\");
  }
  fontDirectory += wxT("Fonts\\");
  wxString strValueName;
  wxString strValueData;
  wxString fontFileName;
  long nIndex;
  fontRegKey->GetFirstValue(strValueName, nIndex);
  size_t i;
  for (i = 0; i < nValues; ++i)
  {
    fontRegKey->QueryValue(strValueName, fontFileName);
    if (fontFileName.Find(wxT('\\')) == wxNOT_FOUND)
    {
      fontFileName.Prepend(fontDirectory);
    }
    if (fontFileName.Lower().EndsWith(wxT(".ttc")))
    {
      count += RegisterFontCollection(fontFileName);
    }
    else
    {
      wxPdfFont registeredFont = RegisterFont(fontFileName);
      if (registeredFont.IsValid())
      {
        ++count;
      }
    }
    fontRegKey->GetNextValue(strValueName, nIndex);
  }
  fontRegKey->Close();
  delete fontRegKey;
#elif defined(__WXGTK20__)
// TODO: Would testing for __WXGTK__ be sufficient?
#if 0
  // TODO: Do we need to load the fontconfig library?
  FcConfig* fc = FcInitLoadConfigAndFonts();
  if (fc == NULL)
  {
    wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterSystemFonts: ")) +
               wxString(_("FontConfig init failed.")));
    return 0;
  }
#endif
  FcPattern* pat = FcPatternBuild(NULL,
	                                FC_OUTLINE, FcTypeBool, 1,
	                                FC_SCALABLE, FcTypeBool, 1,
                                  NULL);
  FcObjectSet* os = FcObjectSetBuild (FC_FAMILY, 
                                      FC_STYLE, 
                                      FC_FULLNAME, 
                                      FC_FILE, 
                                      FC_INDEX,
                                      NULL);
  // TODO: Add FC_OUTLINE, FC_WEIGHT, FC_SLANT ?
  FcFontSet* fs = FcFontList(0, pat, os);
  FcObjectSetDestroy(os);
  FcPatternDestroy(pat);
  if (fs != NULL)
  {
    int j;
    for (j = 0; j < fs->nfont; ++j)
    {
      FcChar8* file;
      if (FcPatternGetString(fs->fonts[j], FC_FILE, 0, &file) == FcResultMatch)
      {
        int fontFileIndex = 0;
        int id = 0;
        if (FcPatternGetInteger (fs->fonts[j], FC_INDEX, 0, &id) == FcResultMatch)
        {
          fontFileIndex = id;
        }
        wxString fontFileName = wxString::FromUTF8((char*) file);
        wxPdfFont registeredFont = RegisterFont(fontFileName, wxEmptyString, fontFileIndex);
        if (registeredFont.IsValid())
        {
          ++count;
        }
      }
    }
    FcFontSetDestroy(fs);
  }
#elif defined(__WXMAC__)
  wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterSystemFonts: ")) +
             wxString(_("Method is not yet implemented for platform WXMAC.")));
#else
  wxLogError(wxString(wxT("wxPdfFontManagerBase::RegisterSystemFonts: ")) +
             wxString(_("Method is not available for your platform.")));
#endif
  return count;
}

class wxPdfFontDirTraverser : public wxDirTraverser
{
public:
  wxPdfFontDirTraverser(wxPdfFontManagerBase* fontManager)
    : m_fontManager(fontManager), m_count(0)
  {
  }

  virtual wxDirTraverseResult OnFile(const wxString& fileName)
  {
    wxFileName fontFileName(fileName);
    wxString ext = fontFileName.GetExt().Lower();
    if (ext.IsSameAs(wxT("ttf")) || ext.IsSameAs(wxT("otf")) ||
        /* ext.IsSameAs(wxT("pfa")) || */ ext.IsSameAs(wxT("pfb"))) 
    {
      wxPdfFont registeredFont = m_fontManager->RegisterFont(fontFileName.GetFullPath());
      if (registeredFont.IsValid())
      {
        ++m_count;
      }
    }
    else if (ext.IsSameAs(wxT("ttc")))
    {
      m_count += m_fontManager->RegisterFontCollection(fontFileName.GetFullPath());
    }
    return wxDIR_CONTINUE;
  }

  virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
  {
    return wxDIR_CONTINUE;
  }

  int GetCount() const
  {
    return m_count;
  }

private:
  wxPdfFontManagerBase* m_fontManager;
  int                   m_count;
};

int
wxPdfFontManagerBase::RegisterFontDirectory(const wxString& directory, bool recursive)
{
  int count = 0;
  if (wxDir::Exists(directory))
  {
    wxDir fontDir(directory);
    if (fontDir.IsOpened())
    {
      wxPdfFontDirTraverser fontDirTraverser(this);
      int flags = (recursive) ? wxDIR_FILES | wxDIR_DIRS : wxDIR_FILES;
      fontDir.Traverse(fontDirTraverser, wxEmptyString, flags);
      count = fontDirTraverser.GetCount();
    }
    else
    {
      wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFontDirectory: ")) +
                   wxString::Format(_("Directory '%s' could not be opened."),directory.c_str()));
    }
  }
  else
  {
    wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFontDirectory: ")) +
                 wxString::Format(_("Directory '%s' does not exist."),directory.c_str()));
  }
  return count;
}
#endif

wxPdfFont
wxPdfFontManagerBase::GetFont(const wxString& fontName, int fontStyle) const
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  wxString lcFontName = fontName.Lower();
  int searchStyle = (fontStyle & ~wxPDF_FONTSTYLE_DECORATION_MASK) & wxPDF_FONTSTYLE_MASK;
  wxPdfFontData* fontData = NULL;

  // Check whether font name equals font family
  wxPdfFontFamilyMap::const_iterator familyIter = m_fontFamilyMap.find(lcFontName);
  if (familyIter == m_fontFamilyMap.end())
  {
    // 1. Check family alias if given name was not a family name
    wxPdfFontAliasMap::const_iterator aliasIter = m_fontAliasMap.find(lcFontName);
    if (aliasIter != m_fontAliasMap.end())
    {
      familyIter = m_fontFamilyMap.find(aliasIter->second);
    }
  }

  if (familyIter != m_fontFamilyMap.end())
  {
    // 2. Check whether the family contains a font with the requested style
    size_t n = familyIter->second.GetCount();
    size_t j;
    for (j = 0; j < n && fontData == NULL; ++j)
    {
      fontData = m_fontList[familyIter->second[j]]->GetFontData();
      if (fontData->GetStyle() != searchStyle)
      {
        fontData = NULL;
      }
    }
  }

  if (fontData == NULL)
  {
    // 3. Check whether a font is registered under the given name
    wxPdfFontNameMap::const_iterator fontIter = m_fontNameMap.find(lcFontName);
    if (fontIter != m_fontNameMap.end())
    {
      fontData = m_fontList[fontIter->second]->GetFontData();
    }
    else
    {
      wxString style = ConvertStyleToString(searchStyle);
      wxLogDebug(wxString(wxT("wxPdfFontManagerBase::GetFont: ")) +
                 wxString::Format(_("Font '%s' with style '%s' not found."), fontName.c_str(), style.c_str()));
    }
  }
  wxPdfFont font(fontData, fontStyle);
  font.SetEmbed(m_defaultEmbed);
  font.SetSubset(m_defaultSubset);
  return font;
}

wxPdfFont
wxPdfFontManagerBase::GetFont(const wxString& fontName, const wxString& fontStyle) const
{
  int style = wxPDF_FONTSTYLE_REGULAR;
  wxString localStyle = fontStyle.Lower();
  if (localStyle.Len() > 2)
  {
    if (localStyle.Find(wxT("bold")) != wxNOT_FOUND)
    {
      style |= wxPDF_FONTSTYLE_BOLD;
    }
    if (localStyle.Find(wxT("italic")) != wxNOT_FOUND || localStyle.Find(wxT("oblique")) != wxNOT_FOUND)
    {
      style |= wxPDF_FONTSTYLE_ITALIC;
    }
  }
  else
  {
    if (localStyle.Find(wxT("b")) != wxNOT_FOUND)
    {
      style |= wxPDF_FONTSTYLE_BOLD;
    }
    if (localStyle.Find(wxT("i")) != wxNOT_FOUND)
    {
      style |= wxPDF_FONTSTYLE_ITALIC;
    }
  }
  return GetFont(fontName, style);
}

wxPdfFont
wxPdfFontManagerBase::GetFont(size_t fontIndex) const
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  wxPdfFont font;
  if (fontIndex < m_fontList.GetCount())
  {
    font = wxPdfFont(m_fontList[fontIndex]->GetFontData());
  }
  return font;
}

size_t
wxPdfFontManagerBase::GetFontCount() const
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  return m_fontList.GetCount();
}

bool
wxPdfFontManagerBase::InitializeFontData(const wxPdfFont& font)
{
  bool ok = false;
  if (font.m_fontData != NULL)
  {
    ok = font.m_fontData->IsInitialized(); 
    if (!ok)
    {
#if wxUSE_THREADS
      wxCriticalSectionLocker locker(gs_csFontManager);
#endif
      ok = font.m_fontData->Initialize();
    }
  }
  return ok;
}

wxString
wxPdfFontManagerBase::ConvertStyleToString(int fontStyle)
{
  wxString style = wxEmptyString;
  if ((fontStyle & wxPDF_FONTSTYLE_BOLDITALIC) == wxPDF_FONTSTYLE_BOLDITALIC)
  {
    style = wxString(_("BoldItalic"));
  }
  else if (fontStyle & wxPDF_FONTSTYLE_BOLD)
  {
    style = wxString(_("Bold"));
  }
  else if (fontStyle & wxPDF_FONTSTYLE_ITALIC)
  {
    style = wxString(_("Italic"));
  }
  else
  {
    style = wxString(_("Regular"));
  }
  return style;
}

// --- wxPdfFontManagerBase (private)

void
wxPdfFontManagerBase::InitializeCoreFonts()
{
  wxPdfFontDataCore* coreFontData;
  int j;
  for (j = 0; gs_coreFontTable[j].name != wxEmptyString; j++)
  {
    const wxPdfCoreFontDesc& coreFontDesc = gs_coreFontTable[j];
    coreFontData = new wxPdfFontDataCore(coreFontDesc.family, coreFontDesc.alias, coreFontDesc.name, 
                                         coreFontDesc.cwArray, coreFontDesc.kpArray,
                                         wxPdfFontDescription(coreFontDesc.ascent, coreFontDesc.descent,
                                                              coreFontDesc.capHeight, coreFontDesc.flags,
                                                              coreFontDesc.bbox, coreFontDesc.italicAngle,
                                                              coreFontDesc.stemV, coreFontDesc.missingWidth,
                                                              coreFontDesc.xHeight, coreFontDesc.underlinePosition,
                                                              coreFontDesc.underlineThickness));
    AddFont(coreFontData);
  }
}

bool
wxPdfFontManagerBase::RegisterFontCJK(const wxString& fontFileName, const wxString& fontStyle, const wxString& alias)
{
  bool ok = false;
  wxPdfFontData* fontData;
  fontData = LoadFontFromXML(fontFileName);
  if (fontData != NULL)
  {
    wxString fontName = fontData->GetName();
    fontName += fontStyle;
    fontData->SetName(fontName);
    fontData->SetFamily(alias);
    fontData->SetAlias(alias);
    fontData->SetStyleFromName();
    ok = AddFont(fontData);
    if (!ok)
    {
      wxLogWarning(wxString(wxT("wxPdfFontManagerBase::RegisterFontCJK: ")) +
                   wxString::Format(_("CJK font '%s' already registered."), fontName.c_str()));
    }
  }
  return ok;
}

wxPdfFontData*
wxPdfFontManagerBase::LoadFontFromXML(const wxString& fontFileName)
{
  wxPdfFontData* fontData = NULL;
  wxFileName fileName(fontFileName);
  wxFileSystem fs;

  // Open font metrics XML file
  wxFSFile* xmlFontMetrics = fs.OpenFile(fileName.GetFullPath());
  if (xmlFontMetrics != NULL)
  {
    // Load the XML file
    wxXmlDocument fontMetrics;
    bool loaded = fontMetrics.Load(*xmlFontMetrics->GetStream());
    delete xmlFontMetrics;
    if (loaded)
    {
      if (fontMetrics.IsOk() && fontMetrics.GetRoot()->GetName().IsSameAs(wxT("wxpdfdoc-font-metrics")))
      {
        wxString fontType;
        wxXmlNode* root = fontMetrics.GetRoot();
#if wxCHECK_VERSION(2,9,0)
        if (root->GetAttribute(wxT("type"), &fontType))
#else
        if (root->GetPropVal(wxT("type"), &fontType))
#endif
        {
          if (fontType.IsSameAs(wxT("TrueType")))
          {
            fontData = new wxPdfFontDataTrueType();
          }
          else if (fontType.IsSameAs(wxT("Type1")))
          {
            fontData = new wxPdfFontDataType1();
          }
#if wxUSE_UNICODE
          else if (fontType.IsSameAs(wxT("TrueTypeUnicode")))
          {
            fontData = new wxPdfFontDataTrueTypeUnicode();
          }
          else if (fontType.IsSameAs(wxT("OpenTypeUnicode")))
          {
            fontData = new wxPdfFontDataOpenTypeUnicode();
          }
          else if (fontType.IsSameAs(wxT("Type0")))
          {
            fontData = new wxPdfFontDataType0();
          }
#endif
          else
          {
            // Unknown font type
            wxLogError(wxString(wxT("wxPdfFontManagerBase::LoadFontFromXML: ")) +
                       wxString::Format(_("Unknown font type '%s' in font file '%s'."), fontType.c_str(), fontFileName.c_str()));
          }
          if (fontData != NULL)
          {
            fontData->SetFilePath(fileName.GetPath());
            if (!fontData->LoadFontMetrics(root))
            {
              wxLogError(wxString(wxT("wxPdfFontManagerBase::LoadFontFromXML: ")) +
                         wxString::Format(_("Loading of font metrics failed for font file '%s'."), fontFileName.c_str()));
              delete fontData;
              fontData = NULL;
            }
          }
        }
        else
        {
          // Font type not specified
          wxLogError(wxString(wxT("wxPdfFontManagerBase::LoadFontFromXML: ")) +
                     wxString::Format(_("Font type not specified for font '%s'."), fontFileName.c_str()));
        }
      }
      else
      {
        // Not a font metrics file
        wxLogError(wxString(wxT("wxPdfFontManagerBase::LoadFontFromXML: ")) +
                   wxString::Format(_("Font metrics file '%s' invalid."), fontFileName.c_str()));
      }
    }
    else
    {
      // Font metrics file loading failed
      wxLogError(wxString(wxT("wxPdfFontManagerBase::LoadFontFromXML: ")) +
                 wxString::Format(_("Loading of font metrics file '%s' failed."), fontFileName.c_str()));
    }
  }
  else
  {
    // Font metrics XML file not found
    wxLogError(wxString(wxT("wxPdfFontManagerBase::LoadFontFromXML: ")) +
               wxString::Format(_("Font metrics file '%s' not found."), fontFileName.c_str()));
  }
  return fontData;
}

bool
wxPdfFontManagerBase::IsRegistered(wxPdfFontData* fontData)
{
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  wxString fontName = fontData->GetName();
  wxPdfFontNameMap::const_iterator font = m_fontNameMap.find(fontName.Lower());
  return (font != m_fontNameMap.end());
}

bool
wxPdfFontManagerBase::AddFont(wxPdfFontData* fontData)
{
  wxPdfFont font;
  return AddFont(fontData, font);
}

bool
wxPdfFontManagerBase::AddFont(wxPdfFontData* fontData, wxPdfFont& font)
{
  bool ok = false;
#if wxUSE_THREADS
  wxCriticalSectionLocker locker(gs_csFontManager);
#endif
  wxString fontName = fontData->GetName().Lower();
  wxString family = fontData->GetFamily().Lower();
  wxString alias = fontData->GetAlias().Lower();

  wxPdfFontNameMap::const_iterator fontIter = m_fontNameMap.find(fontName.Lower());
  if (fontIter == m_fontNameMap.end())
  {
    // Font not yet registered
    wxArrayString fullNames = fontData->GetFullNames();
    size_t pos = m_fontList.GetCount();
    wxPdfFontListEntry* fontEntry = new wxPdfFontListEntry(fontData);
    m_fontList.Add(fontEntry);
    font = wxPdfFont(fontData);
    ok = true;

    // Register Postscript font name
    m_fontNameMap[fontName] = pos;

    // Register all full font names
    size_t j;
    for (j = 0; j < fullNames.GetCount(); ++j)
    {
      m_fontNameMap[fullNames[j].Lower()] = pos;
    }

    // Register font in family
    if (!family.IsEmpty())
    {
      m_fontFamilyMap[family].Add(pos);
    }
    else if (!alias.IsEmpty())
    {
      m_fontFamilyMap[alias].Add(pos);
    }
  }
  else
  {
    font = wxPdfFont(m_fontList[fontIter->second]->GetFontData());
  }

  // Register family alias
  if (!alias.IsEmpty() && !alias.IsSameAs(family))
  {
    // Check whether the alias is already assigned and - if so - to the same family
    wxPdfFontAliasMap::const_iterator aliasIter = m_fontAliasMap.find(alias);
    if (aliasIter != m_fontAliasMap.end())
    {
      if (!aliasIter->second.IsSameAs(family))
      {
        wxLogError(wxString(wxT("wxPdfFontManagerBase::AddFont: ")) +
                   wxString::Format(_("Family alias '%s' for family '%s' already assigned to family '%s'."), 
                   alias.c_str(), family.c_str(), aliasIter->second.c_str()));
      }
    }
    else
    {
      // alias not previously assigned, remember assignment
      m_fontAliasMap[alias] = family;
    }
  }
  return ok;
}

// --- wxPdfFontManager

wxPdfFontManager* wxPdfFontManager::ms_fontManager = NULL;

wxPdfFontManager::wxPdfFontManager()
{
  m_fontManagerBase = new wxPdfFontManagerBase();
}
  
wxPdfFontManager::~wxPdfFontManager()
{
  delete m_fontManagerBase;
}

wxPdfFontManager*
wxPdfFontManager::GetFontManager()
{
  return ms_fontManager;
}

  
void
wxPdfFontManager::AddSearchPath(const wxString& path)
{
  m_fontManagerBase->AddSearchPath(path);
}

void
wxPdfFontManager::AddSearchPath(const wxArrayString& pathArray)
{
  m_fontManagerBase->AddSearchPath(pathArray);
}

bool
wxPdfFontManager::SetDefaultEmbed(bool embed)
{
  return m_fontManagerBase->SetDefaultEmbed(embed);
}
  
bool
wxPdfFontManager::GetDefaultEmbed()
{
  return m_fontManagerBase->GetDefaultEmbed();
}
  
bool
wxPdfFontManager::SetDefaultSubset(bool subset)
{
  return m_fontManagerBase->SetDefaultSubset(subset);
}
  
bool
wxPdfFontManager::GetDefaultSubset()
{
  return m_fontManagerBase->GetDefaultSubset();
}
  
wxPdfFont
wxPdfFontManager::RegisterFont(const wxString& fontFileName, const wxString& aliasName, int fontIndex)
{
  return m_fontManagerBase->RegisterFont(fontFileName, aliasName, fontIndex);
}

wxPdfFont
wxPdfFontManager::RegisterFont(const wxFont& font, const wxString& aliasName)
{
#if wxUSE_UNICODE
  return m_fontManagerBase->RegisterFont(font, aliasName);
#else
  wxUnusedVar(font);
  wxUnusedVar(aliasName);
  return wxPdfFont();
#endif
}

int
wxPdfFontManager::RegisterFontCollection(const wxString& fontCollectionFileName)
{
#if wxUSE_UNICODE
  return m_fontManagerBase->RegisterFontCollection(fontCollectionFileName);
#else
  wxUnusedVar(fontCollectionFileName);
  return 0;
#endif
}

bool
wxPdfFontManager::RegisterFontCJK(const wxString& family)
{
  return m_fontManagerBase->RegisterFontCJK(family);
}

int
wxPdfFontManager::RegisterSystemFonts()
{
#if wxUSE_UNICODE
  return m_fontManagerBase->RegisterSystemFonts();
#else
  return 0;
#endif
}

int
wxPdfFontManager::RegisterFontDirectory(const wxString& directory, bool recursive)
{
#if wxUSE_UNICODE
  return m_fontManagerBase->RegisterFontDirectory(directory, recursive);
#else
  wxUnusedVar(directory);
  wxUnusedVar(recursive);
  return 0;
#endif
}

wxPdfFont
wxPdfFontManager::GetFont(const wxString& fontName, int fontStyle) const
{
  return m_fontManagerBase->GetFont(fontName, fontStyle);
}

wxPdfFont
wxPdfFontManager::GetFont(const wxString& fontName, const wxString& fontStyle) const
{
  return m_fontManagerBase->GetFont(fontName, fontStyle);
}

wxPdfFont
wxPdfFontManager::GetFont(size_t fontIndex) const
{
  return m_fontManagerBase->GetFont(fontIndex);
}

size_t
wxPdfFontManager::GetFontCount() const
{
  return m_fontManagerBase->GetFontCount();
}

bool
wxPdfFontManager::InitializeFontData(const wxPdfFont& font)
{
  bool ok = false;
  if (font.IsValid())
  {
    ok = m_fontManagerBase->InitializeFontData(font);
  }
  return ok;
}

// A module to allow initialization/cleanup of wxPdfDocument
// singletons without calling these functions from app.cpp.

class WXDLLIMPEXP_PDFDOC wxPdfDocumentModule : public wxModule
{
DECLARE_DYNAMIC_CLASS(wxPdfDocumentModule)
public:
  wxPdfDocumentModule() {}
  bool OnInit();
  void OnExit();
};

IMPLEMENT_DYNAMIC_CLASS(wxPdfDocumentModule, wxModule)

/*
 * Initialization/cleanup module
 */

bool wxPdfDocumentModule::OnInit()
{
  wxPdfFontManager::ms_fontManager = new wxPdfFontManager();
  return true;
}

void wxPdfDocumentModule::OnExit()
{
  delete wxPdfFontManager::ms_fontManager;
  wxPdfFontManager::ms_fontManager = NULL;
}
