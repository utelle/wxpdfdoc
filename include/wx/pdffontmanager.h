///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontmanager.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2008-08-10
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontmanager.h Interface of wxPdfFontManager class

#ifndef _PDF_FONT_MANAGER_H_
#define _PDF_FONT_MANAGER_H_

// wxWidgets headers
#include <wx/arrstr.h>
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdffont.h"

class WXDLLIMPEXP_FWD_PDFDOC wxPdfFontData;
class wxPdfFontManagerBase;

/// Class representing the font manager
class WXDLLIMPEXP_PDFDOC wxPdfFontManager
{
public:
  /// Get the font manager
  /**
  * \return a reference to the font manager
  */
  static wxPdfFontManager* GetFontManager();

  /// Add a path entry to the font search path list
  /**
  * \param path path to add to the search list
  */
  void AddSearchPath(const wxString& path);

  /// Add a list of path entries to the font search path list
  /**
  * \param pathArray a list of path entries to add to the search list
  */
  void AddSearchPath(const wxArrayString& pathArray);

  /// Set the default embedding mode
  /**
  * Usually it is recommended to enable embedding, since this guarantees that
  * no font substitution takes place on a target system not providing a required font.
  *
  * The default embedding mode is enabled by default, but the embedding mode
  * can be set for each font individually.
  *
  * \note For most fonts the embedding mode is predefined. For example the Adobe Standard
  * fonts and the Adobe CJK fonts are never embedded independently of the setting of the
  * default embedding mode, and TrueType/OpenType Unicode fonts need to be always embedded.
  *
  * \param embed flag whether to enable or disable embedding
  * \return the state of the default embedding mode prior to this call
  */
  bool SetDefaultEmbed(bool embed);

  /// Get the default embedding mode
  /**
  * \return the current state of the default embedding mode
  */
  bool GetDefaultEmbed();

  /// Set the default subsetting mode
  /**
  * Usually it is recommended to enable subsetting, since embedding only a font subset
  * reduces the size of the resulting PDF file significantly.
  *
  * The default subsetting mode is enabled by default, but the subsetting mode
  * can be set for each font individually.
  *
  * \param subset flag whether to enable or disable font subsetting
  * \return the state of the default subsetting mode prior to this call
  */
  bool SetDefaultSubset(bool subset);

  /// Get the default subsetting mode
  /**
  * \return the state of the default subsetting mode
  */
  bool GetDefaultSubset();

  /// Register a font
  /**
  * Register a font for later use in creating a PDF document.
  * \param fontFileName the name of the font file (if the name is not fully qualified,
  *                     it will be searched in font search path)
  * \param aliasName an alias name for the font family (default: no alias)
  * \param fontIndex the index of the font in a font collection (default: 0)
  * \return the registered font
  */
  wxPdfFont RegisterFont(const wxString& fontFileName, const wxString& aliasName = wxEmptyString, int fontIndex = 0);

  /// Register a font based on a wxFont object
  /**
  * Register a font based on a wxFont object for later use in creating a PDF document.
  * \param font the wxFont object to be registered
  * \param aliasName an alias name for the font family (default: no alias)
  * \return the registered font
  */
  wxPdfFont RegisterFont(const wxFont& font, const wxString& aliasName = wxEmptyString);

  /// Register a font based on a font data buffer
  /**
  * Register a font based on a font data buffer for later use in creating a PDF document.
  *
  * \note The registered font will not take ownership of the font data buffer.
  * That is, the buffer must be accessible during the life time of the font.
  *
  * \param fontData the font data buffer holding a TrueType or OpenType font
  * \param fontDataSize the size of the font data buffer
  * \param aliasName an alias name for the font family (default: no alias)
  * \return the registered font
  */
  wxPdfFont RegisterFont(const char* fontData, size_t fontDataSize, const wxString& aliasName = wxEmptyString);

  /// Register a font collection
  /**
  * Register all fonts from a font collection for later use in creating a PDF document.
  * \param fontCollectionFileName the name of the font collection file (if the name is
  *                     not fully qualified, it will be searched in font search path)
  * \return the number of registered fonts
  */
  int RegisterFontCollection(const wxString& fontCollectionFileName);

  /// Register a CJK font family
  /**
  * Register all fonts from a font collection for later use in creating a PDF document.
  * The following CJK font families are currently known to wxPdfDocument:
  * \li GB, GB-HW - Simplified Chinese (Adobe-Font STSongStd-Light-Acro)
  * \li BIG5, BIG5-HW - Traditional Chinese (Adobe-Font MSungStd-Light-Acro)
  * \li SJIS - Japanese (Adobe-Font KozMinPro-Regular-Acro)
  * \li UHC, UHC-HW - Korean (Adobe-Font HYSMyeongJoStd-Medium-Acro)
  * \li GOTHIC, PGOTHIC, UIGOTHIC - Japanese (Microsoft-Font MS-Gothic/MS-PGothic/MS-UIGothic)
  * \li MINCHO, PMINCHO - Japanese (Microsoft-Font MS-Mincho/MS-PMincho)
  *
  * \param family the name of the CJK font family
  * \return @c true if the CJK family could be registered successfully, @c false otherwise
  */
  bool RegisterFontCJK(const wxString& family);

  /// Register the fonts known to the operating system
  /*
  * This method tries to register all installed fonts. On Windows platforms the registry
  * is searched to identify installed fonts, on Linux platforms the fontconfig library
  * is used to identify installed fonts.
  *
  * \note This method is currently implemented only for wxMSW and wxGTK.
  * \return the number of registered fonts
  */
  int RegisterSystemFonts();

  /// Register all fonts located in a directory
  /**
  * \param directory the directory path to be searched for font files
  * \param recursive flag whether the search should include subdirectories or not (Default: false)
  * \return the number of registered fonts
  */
  int RegisterFontDirectory(const wxString& directory, bool recursive = false);

  /// Get a font by name and style
  /**
  * Based on the given name and style the list of registered fonts is searched for a matching font.
  * The name is checked whether it represents a font family. If the name is not a family name
  * then the name is checked whether it represents an alias for a font family. If the name is not
  * an alias either the name is checked whether it is a full font name.
  * \param fontName the name of the requested font
  * \param fontStyle the style of the requested font
  * \return the requested font
  * \note If no appropriate font is registered an invalid font object is returned.
  * It's the user's responsibility to check the font object before further using it.
  */
  wxPdfFont GetFont(const wxString& fontName, int fontStyle = wxPDF_FONTSTYLE_REGULAR) const;

  /// Get a font by name and style
  /**
  * Based on the given name and style the list of registered fonts is searched for a matching font.
  * The name is checked whether it represents a font family. If the name is not a family name
  * then the name is checked whether it represents an alias for a font family. If the name is not
  * an alias either the name is checked whether it is a full font name.
  * \param fontName the name of the requested font
  * \param fontStyle the style of the requested font (string representation)
  * \return the requested font
  * \note If no appropriate font is registered an invalid font object is returned.
  * It's the user's responsibility to check the font object before further using it.
  */
  wxPdfFont GetFont(const wxString& fontName, const wxString& fontStyle) const;

  /// Get the font corresponding to a wxFont object
  /**
  * This method is used by wxPdfDocument::SetFont(const wxFont&), wxPdfDC and
  * wxPdfGraphicsContext to map a wxFont to a PDF font.
  *
  * First a registered font matching the face name and style of the wxFont is searched.
  * If none is registered, the font is located via the operating system and registered
  * (see RegisterFont(const wxFont&)).
  *
  * If the face name denotes one of the 14 Adobe standard fonts or one of their aliases
  * (for example @c Arial, @c Helvetica, @c Times, @c Courier or @c Symbol), the font
  * installed on the system is preferred: a wxFont stands for the font the operating
  * system renders on screen, while the standard fonts support only the WinAnsi
  * character set (Symbol and ZapfDingbats only their own one) and are never embedded.
  * Only if no font can be located via the operating system the standard font is used.
  * The result is cached, so that the operating system is asked only once per face name
  * and style.
  *
  * \param font the wxFont object
  * \return the matching font, or an invalid font if no font could be found
  * \note It's the user's responsibility to check the font object before further using it.
  */
  wxPdfFont GetFont(const wxFont& font);

  /// Get a font by index
  /**
  * \param fontIndex the index of the requested font in the list of registered fonts
  * \return the requested font
  * \note If the index is out of range an invalid font object is returned.
  * It's the user's responsibility to check the font object before further using it.
  */
  wxPdfFont GetFont(size_t fontIndex) const;

  /// Find an installed font containing a glyph for a character
  /**
  * Searches the fonts installed on the system for a font that can show the given
  * character, preferably in the given style. This is the lookup used by
  * wxPdfDocument::SetSystemFontFallback(), but it can also be used directly,
  * for example to build an explicit fallback list with wxPdfDocument::AddFallbackFont().
  *
  * On wxGTK the fontconfig library is queried (the mechanism Pango uses for its own
  * font fallback). On other platforms a built-in list of well-known font families with
  * a large Unicode coverage is checked (e.g. Segoe UI and Cambria Math on Windows,
  * Lucida Grande and STIX Two Math on macOS, DejaVu Sans and Noto Sans everywhere).
  * A font found is registered with the font manager.
  *
  * Results, including negative ones, are cached, so repeated lookups are cheap.
  *
  * \param unicodeChar the Unicode code point of the character
  * \param fontStyle the preferred font style (only weight and italic are considered)
  * \return a font containing the character, or an invalid font if none was found
  * \note Only available in Unicode builds. The result depends on the fonts installed on the system.
  */
  wxPdfFont FindFallbackFont(wxUint32 unicodeChar, int fontStyle = wxPDF_FONTSTYLE_REGULAR);

  /// Get the number of registered fonts
  /**
  * \return the number of registered fonts
  */
  size_t GetFontCount() const;

  /// Initialize the font data of a font
  /**
  * On registering a font the associated font data are not completely loaded into memory
  * to save time and memory resources, but as soon as the font is actually used it must
  * be fully initialized. Usually this method is called automatically.
  * \param font the font to be initialized
  * \return @c true if the font could be initialized successfully, @c false otherwise
  */
  bool InitializeFontData(const wxPdfFont& font);

  /// Register a font encoding
  /**
  *
  * \param encoding the encoding to be registered
  * \return @c true if the encoding could be registered successfully
  */
  bool RegisterEncoding(const wxPdfEncoding& encoding);

  /// Get the encoding having the given name
  /**
  * \param encodingName name of the encoding
  * \return the encoding
  */
  const wxPdfEncoding* GetEncoding(const wxString& encodingName);

  /// Set the embeddable replacement for a font family
  /**
  * Some fonts can never be embedded into a PDF document: the 14 Adobe standard
  * fonts (Courier, Helvetica, Times, Symbol and ZapfDingbats), the Adobe CJK fonts,
  * and fonts whose licence does not allow embedding. PDF/A forbids fonts that are
  * not embedded, so in a PDF/A document (see wxPdfDocument::SetPdfA1Conformance())
  * such a font is replaced by an embeddable font family. This method defines which
  * family is used as replacement for a given font family.
  *
  * The replacement family is tried first. If it is not available, the built-in
  * list of metric-compatible free fonts is tried (see GetEmbeddableFont()).
  *
  * The replacement family should either be registered already (for example with
  * RegisterFont() or RegisterFontDirectory()) or be installed on the system, so
  * that it can be located via the operating system's font services.
  *
  * \note The fonts Symbol and ZapfDingbats use a font specific encoding. Text
  * written with them is reinterpreted using the encoding of the replacement font,
  * so they should only be replaced by fonts with the same built-in encoding.
  * Therefore no built-in replacements exist for these two fonts.
  *
  * \param fontFamily the font family (or family alias) to be replaced, i.e. @c Helvetica
  * \param replacementFamily the family of the embeddable replacement font, i.e. <tt>Liberation Sans</tt>.
  *        An empty string removes a previously defined replacement.
  * \see GetEmbeddableFontReplacement(), GetEmbeddableFont(), wxPdfDocument::SetPdfA1Conformance()
  */
  void SetEmbeddableFontReplacement(const wxString& fontFamily, const wxString& replacementFamily);

  /// Get the embeddable replacement defined for a font family
  /**
  * \param fontFamily the font family (or family alias)
  * \return the replacement family set by SetEmbeddableFontReplacement(), or an empty
  *         string if no replacement was defined for this family
  */
  wxString GetEmbeddableFontReplacement(const wxString& fontFamily) const;

  /// Get an embeddable version of a font
  /**
  * If the given font can be embedded, a copy of it with embedding enabled is returned.
  * Otherwise an embeddable replacement font with matching style is searched in the
  * following order:
  * \li the family defined by SetEmbeddableFontReplacement() for the font's family or alias;
  * \li for the Adobe standard fonts Helvetica, Times and Courier a built-in list of
  *     free fonts with compatible metrics (Liberation, Croscore, URW base 35, TeX Gyre,
  *     GNU FreeFont) followed by the common system fonts Arial, Times New Roman and
  *     Courier New.
  *
  * Each candidate family is first searched among the registered fonts. If it is not
  * registered yet, wxPdfDocument tries to locate the font via the operating system
  * (on wxMSW, wxGTK and wxOSX, see RegisterFont(const wxFont&)) and registers it.
  * A system font is only accepted if its family name equals the candidate family,
  * so that a platform specific fallback font is never used silently.
  *
  * \note Locating a system font creates a wxFont object, so like RegisterFont(const wxFont&)
  * this should be called from the main thread if the font is not registered yet.
  *
  * \param font the font to be made embeddable
  * \param fontStyle the requested font style (in addition to the style of the font itself)
  * \param useBuiltInReplacements flag whether the built-in replacement lists are used;
  *        if @c false, only the replacement defined by SetEmbeddableFontReplacement() is tried
  * \return the embeddable font, or an invalid font if neither the font itself
  *         nor any replacement can be embedded
  * \see SetEmbeddableFontReplacement(), wxPdfDocument::SetPdfA1Conformance()
  */
  wxPdfFont GetEmbeddableFont(const wxPdfFont& font, int fontStyle = wxPDF_FONTSTYLE_REGULAR,
                              bool useBuiltInReplacements = true);

  /// Get an embeddable font by family name and style
  /**
  * Like GetEmbeddableFont(const wxPdfFont&, int), but starts from a family name.
  * If no font of that family is registered, the replacements for the family name
  * are tried, and finally wxPdfDocument tries to locate the family itself via the
  * operating system.
  *
  * \param fontFamily the name of the requested font family
  * \param fontStyle the requested font style
  * \return the embeddable font, or an invalid font if no embeddable font could be found
  */
  wxPdfFont GetEmbeddableFont(const wxString& fontFamily, int fontStyle = wxPDF_FONTSTYLE_REGULAR);

private:
  /// Default constructor
  wxPdfFontManager();

  /// Default destructor
  ~wxPdfFontManager();

  /// Copy destructor
  wxPdfFontManager(const wxPdfFontManager&);

  /// Assignment operator
  wxPdfFontManager& operator=(const wxPdfFontManager&);

private:
  wxPdfFontManagerBase*    m_fontManagerBase; ///< Font manager implementation

  static wxPdfFontManager* ms_fontManager;    ///< Font manager singleton

  friend class wxPdfDocumentModule;
};

#endif
