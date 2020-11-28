///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontmacosx.h
// Purpose:     Definition of Mac OS X specific font data handling
// Author:      Ulrich Telle
// Created:     2010-09-30
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontmacosx.h Definition of Mac OS X specific font data handling

#ifndef _PDF_FONT_MACOSX_H_
#define _PDF_FONT_MACOSX_H_

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

#if defined(__WXMAC__)

// Using CoreText fonts is now the default for wxWidgets 3.x
#include <wx/osx/private.h>
#define wxPDFMACOSX_HAS_CORE_TEXT 1
#define wxPDFMACOSX_HAS_ATSU_TEXT 0

#endif

#endif
