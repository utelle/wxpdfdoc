/*
** Name:        woff2converter.h
** Purpose:     Class for converting a WOFF2 font to TTF/OTF
** Author:      Ulrich Telle
** Created:     2024-11-06
** Copyright:   (c) 2005-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file woff2converter.h Interface of the WOFF2 converter class

#ifndef PDFDOC_WOFF_WOFF2CONVERTER_H_
#define PDFDOC_WOFF_WOFF2CONVERTER_H_

#include <wx/mstream.h>

class Woff2Converter
{
public:
  static bool IsWoff2Font(wxInputStream* fontStream);

  static wxMemoryInputStream* Convert(wxInputStream* fontStream);
};

#endif //
