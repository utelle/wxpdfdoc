///////////////////////////////////////////////////////////////////////////////
// Name:        pdfarraydouble.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-05-25
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfarraydouble.h Definition of array of type double

#ifndef _PDF_ARRAY_DOUBLE_H_
#define _PDF_ARRAY_DOUBLE_H_

// wxWidgets headers
#include <wx/dynarray.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

// Just map wxPdfArrayDouble to wxArrayDouble, for wxWidgets version 2.7.0 or later
#define wxPdfArrayDouble wxArrayDouble

#endif
