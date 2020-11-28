///////////////////////////////////////////////////////////////////////////////
// Name:        pdfcjkfontdata.h
// Purpose:     Definition of CJK font data structures
// Author:      Ulrich Telle
// Created:     2010-03-29
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfcjkfontdata.h Definition of CJK font data structures

#ifndef _PDF_CJK_FONT_DATA_H_
#define _PDF_CJK_FONT_DATA_H_

/// Structure describing core fonts (For internal use only)
typedef struct _wxPdfCjkFontDesc
{
  const wxStringCharType* family;             ///< font family
  const wxStringCharType* name;               ///< font name
  const wxStringCharType* encoding;           ///< font encoding
  const wxStringCharType* ordering;           ///< registry ordering
  const wxStringCharType* supplement;         ///< registry supplement
  const wxStringCharType* cmap;               ///< font cmap
  short*                  cwArray;            ///< array of character widths
  const wxStringCharType* bbox;               ///< bounding box
  int                     ascent;             ///< ascender
  int                     descent;            ///< descender
  int                     capHeight;          ///< height of capital characters
  int                     flags;              ///< font flags
  int                     italicAngle;        ///< italic angle
  int                     stemV;              ///< stemV value
  int                     missingWidth;       ///< width used for missing characters
  int                     xHeight;            ///< height of the character X
  int                     underlinePosition;  ///< position of the underline decoration
  int                     underlineThickness; ///< thickness of the underline decoration
} wxPdfCjkFontDesc;

#endif
