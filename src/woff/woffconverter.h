/*
** Name:        woffconverter.h
** Purpose:     Class for converting a WOFF font to TTF/OTF
** Author:      Ulrich Telle
** Created:     2024-11-06
** Copyright:   (c) 2005-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file woffconverter.h Interface of the WOFF converter class

#ifndef PDFDOC_WOFF_WOFFCONVERTER_H_
#define PDFDOC_WOFF_WOFFCONVERTER_H_

#include <wx/mstream.h>

class WoffConverter
{
public:
  static bool IsWoffFont(wxInputStream* fontStream);

  static wxMemoryInputStream* Convert(wxInputStream* fontStream);
};

#endif //
