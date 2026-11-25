/*
** Name:        pdffontfallback.h
** Purpose:     Internal state of the font fallback (glyph substitution) feature
** Author:      wxPdfDocument contributors
** Created:     2026-11-23
** Copyright:   (c) 2026 wxPdfDocument contributors
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file pdffontfallback.h Internal state of the font fallback feature of wxPdfDocument
///
/// This header is private to the library; it is not installed.

#ifndef _PDF_FONT_FALLBACK_H_
#define _PDF_FONT_FALLBACK_H_

#include <map>
#include <vector>

#include <wx/string.h>

#include "wx/pdffont.h"
#include "wx/pdffontextended.h"

/// A piece of text that is rendered with a single font
/**
* wxPdfDocument splits a text into runs whenever font fallback is enabled and the
* current font lacks glyphs for some of the characters.
*/
struct wxPdfFontFallbackRun
{
  wxString m_text; ///< The characters of the run (complete surrogate pairs only)
  int      m_slot; ///< Index into wxPdfFontFallback::m_resolved, or -1 for the current font
};

typedef std::vector<wxPdfFontFallbackRun> wxPdfFontFallbackRuns;

/// Internal state of the font fallback feature of one wxPdfDocument instance
/**
* The state is kept out of the public header on purpose: it uses STL containers
* (which would trigger DLL-interface warnings in an exported class) and it is an
* implementation detail that may change without affecting the ABI of wxPdfDocument.
*/
class wxPdfFontFallback
{
public:
  wxPdfFontFallback() : m_useSystemFonts(false) {}

  /// Forget all cached lookup results (called whenever the configuration changes)
  void ClearCache()
  {
    m_cache.clear();
  }

  /// Check whether fallback is configured at all
  bool IsEnabled() const
  {
    return m_useSystemFonts || !m_fonts.empty();
  }

  /// Fonts explicitly added by the user via wxPdfDocument::AddFallbackFont(), in priority order
  std::vector<wxPdfFont> m_fonts;

  /// Flag whether the fonts installed on the system should be searched after m_fonts
  bool m_useSystemFonts;

  /// Concrete fonts that were selected for at least one character ("slots")
  /**
  * Text runs refer to these by index. The fonts are kept as wxPdfFontExtended
  * so that widths and glyph coverage can be queried without registering the
  * font in the document; registration happens only when text is actually output.
  */
  std::vector<wxPdfFontExtended> m_resolved;

  /// The user level font object belonging to each entry of m_resolved
  std::vector<wxPdfFont> m_resolvedFonts;

  /// Maps a font key (name + encoding) to its index in m_resolved
  std::map<wxString, int> m_slotByKey;

  /// Lookup cache: (style << 32 | code point) -> slot index, or -1 if no font has the glyph
  std::map<wxUint64, int> m_cache;
};

#endif
