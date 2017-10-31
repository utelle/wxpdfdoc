///////////////////////////////////////////////////////////////////////////////
// Name:        pdfdc.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2010-11-28
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfdc.h Interface of the wxPdfDC class

#ifndef _PDF_DC_H_
#define _PDF_DC_H_

#include <wx/cmndata.h>
#include <wx/dc.h>

#include <stack>

#include "wx/pdfdocument.h"
#include "wx/pdffont.h"

/// Enumeration of map mode styles
enum wxPdfMapModeStyle
{
  wxPDF_MAPMODESTYLE_STANDARD = 1,
  wxPDF_MAPMODESTYLE_MSW,
  wxPDF_MAPMODESTYLE_GTK,
  wxPDF_MAPMODESTYLE_MAC,
  /// same font sizes and text position as with the wxPdfDocument API
  wxPDF_MAPMODESTYLE_PDF,
  /// same font sizes as with the wxPdfDocument API
  wxPDF_MAPMODESTYLE_PDFFONTSCALE
};

#if wxCHECK_VERSION(2,9,0)
// Interface of wxPdfDC for wxWidgets 2.9.x (and above)
#include "wx/pdfdc29.h"
#else
// Interface of wxPdfDC for wxWidgets 2.8.x
#include "wx/pdfdc28.h"
#endif

#endif
