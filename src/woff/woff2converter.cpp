/*
** Name:        woff2converter.cpp
** Purpose:     Implementation of WOFF2 converter class
** Author:      Ulrich Telle
** Created:     2024-11-06
** Copyright:   (c) 2005-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file woff2converter.cpp Implementation of the WOFF2 converter class

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "woff2converter.h"

#include <woff2/decode.h>

#include <cstdint>
#include <vector>

namespace
{
  static uint32_t woff2Signature = 0x774f4632ul;

  static uint32_t BytesToUInt(const std::vector<uint8_t>& b, int start)
  {
    return (b[start] & 0xFFul) << 24
         | (b[start + 1] & 0xFFul) << 16
         | (b[start + 2] & 0xFFul) << 8
         | (b[start + 3] & 0xFFul);
  }
}

/* static */
bool Woff2Converter::IsWoff2Font(wxInputStream* fontStream)
{
  wxFileOffset streamLen = fontStream->SeekI(0, wxFromEnd);
  if (streamLen < 4)
  {
    return false;
  }

  std::vector<uint8_t> woff2Bytes(4);
  fontStream->SeekI(0, wxFromStart);
  fontStream->Read(woff2Bytes.data(), 4);
  uint32_t signature = BytesToUInt(woff2Bytes, 0);

  return signature == woff2Signature;
}

/* static */
wxMemoryInputStream* Woff2Converter::Convert(wxInputStream* fontStream)
{
  wxFileOffset streamLen = fontStream->SeekI(0, wxFromEnd);
  std::vector<uint8_t> fontData(streamLen);
  fontStream->SeekI(0, wxFromStart);
  fontStream->ReadAll(fontData.data(), fontData.size());

  uint32_t signature = BytesToUInt(fontData, 0);
  if (signature != woff2Signature)
  {
    return nullptr;
  }

  wxMemoryInputStream* finalStream = nullptr;
  size_t finalSize = woff2::ComputeWOFF2FinalSize(fontData.data(), fontData.size());
  std::vector<uint8_t> finalData(finalSize);
  bool ok = woff2::ConvertWOFF2ToTTF(finalData.data(), finalData.size(), fontData.data(), fontData.size());
  if (ok)
  {
    wxMemoryOutputStream otfStream;
    otfStream.Write(finalData.data(), finalData.size());
    finalStream = new wxMemoryInputStream(otfStream);
  }

  return finalStream;
}
