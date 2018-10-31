///////////////////////////////////////////////////////////////////////////////
// Name:        pdfarraytypes.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2008-08-08
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfarraytypes.h Definition of array types

#ifndef _PDF_ARRAY_TYPES_H_
#define _PDF_ARRAY_TYPES_H_

// wxWidgets headers
#include <wx/dynarray.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

/// Unsorted array types
WX_DEFINE_ARRAY_SHORT(wxUint16, wxPdfArrayUint16);
WX_DEFINE_ARRAY_LONG(wxUint32, wxPdfArrayUint32);

/// Sorted array types
WX_DEFINE_SORTED_ARRAY_INT(int, wxPdfSortedArrayInt);
#endif
