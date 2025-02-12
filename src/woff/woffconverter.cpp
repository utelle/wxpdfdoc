/*
** Name:        woffconverter.cpp
** Purpose:     Implementation of WOFF converter class
** Author:      Ulrich Telle
** Created:     2024-11-06
** Copyright:   (c) 2005-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file woffconverter.cpp Implementation of the WOFF converter class

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/zstream.h>

#include "woffconverter.h"

#include <cstdint>
#include <vector>

namespace
{
  static uint32_t woffSignature = 0x774F4646ul;

  static uint32_t BytesToUInt(const std::vector<uint8_t>& b, int start)
  {
    return (b[start]     & 0xFFul) << 24
         | (b[start + 1] & 0xFFul) << 16
         | (b[start + 2] & 0xFFul) << 8
         | (b[start + 3] & 0xFFul);
  }

  static uint16_t BytesToUShort(const std::vector<uint8_t>& b, int start)
  {
    return (b[start]   & 0xFF) << 8
         | (b[start+1] & 0xFF);
  }

  static void ArrayCopy(const uint8_t* srcArray, int srcOffset, uint8_t* destArray, int destOffset, int count)
  {
    for (int j = 0; j < count; ++j)
    {
      destArray[destOffset + j] = srcArray[srcOffset + j];
    }
  }
}

struct TableDirectory
{
  uint8_t tag[4];
  uint32_t offset;
  uint32_t compLength;
  uint8_t origLength[4];
  uint32_t origLengthVal;
  uint8_t origChecksum[4];
  int outOffset;
};

/* static */
bool WoffConverter::IsWoffFont(wxInputStream* fontStream)
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

  return signature == woffSignature;
}

/* static */
wxMemoryInputStream* WoffConverter::Convert(wxInputStream* fontStream)
{
  wxFileOffset streamLen = fontStream->SeekI(0, wxFromEnd);
  std::vector<uint8_t> fontData(streamLen);
  fontStream->SeekI(0, wxFromStart);
  fontStream->ReadAll(fontData.data(), fontData.size());

  int srcPos = 0;
  int destPos = 0;

  // signature
  if (BytesToUInt(fontData, srcPos) != woffSignature)
  {
    return nullptr;
  }
  srcPos += 4;

  std::vector<uint8_t> flavor(4);
  ArrayCopy(fontData.data(), srcPos, flavor.data(), 0, 4);
  srcPos += 4;

  // length
  if (BytesToUInt(fontData, srcPos) != fontData.size())
  {
    return nullptr;
  }
  srcPos += 4;

  std::vector<uint8_t> numTables(2);
  ArrayCopy(fontData.data(), srcPos, numTables.data(), 0, 2);
  srcPos += 2;

  // reserved
  if (BytesToUShort(fontData, srcPos) != 0)
  {
    return nullptr;
  }
  srcPos += 2;

  uint32_t totalSfntSize = BytesToUInt(fontData, srcPos);
  srcPos += 4;

  // majorVersion
  srcPos += 2;
  // minorVersion
  srcPos += 2;
  // metaOffset
  srcPos += 4;
  // metaLength
  srcPos += 4;
  // metaOrigLength
  srcPos += 4;
  // privOffset
  srcPos += 4;
  // privLength
  srcPos += 4;

  std::vector<uint8_t> otfBytes(totalSfntSize);
  ArrayCopy(flavor.data(), 0, otfBytes.data(), destPos, flavor.size());
  destPos += 4;
  ArrayCopy(numTables.data(), 0, otfBytes.data(), destPos, numTables.size());
  destPos += 2;

  int entrySelector = -1;
  int searchRange = -1;
  int numTablesVal = BytesToUShort(numTables, 0);
  for (int i = 0; i < 17; ++i)
  {
    int powerOfTwo = 1 << i;
    if (powerOfTwo > numTablesVal)
    {
      entrySelector = i;
      searchRange = powerOfTwo * 16;
      break;
    }
  }
  if (entrySelector < 0)
  {
    return nullptr;
  }
  otfBytes[destPos] = static_cast<uint8_t>((searchRange >> 8) & 0xFF);
  otfBytes[destPos + 1] = static_cast<uint8_t>(searchRange);
  destPos += 2;
  otfBytes[destPos] = static_cast<uint8_t>((entrySelector >> 8) & 0xFF);
  otfBytes[destPos + 1] = static_cast<uint8_t>(entrySelector);
  destPos += 2;
  int rangeShift = numTablesVal * 16 - searchRange;
  otfBytes[destPos] = static_cast<uint8_t>((rangeShift >> 8) & 0xFF);
  otfBytes[destPos + 1] = static_cast<uint8_t>(rangeShift);
  destPos += 2;

  int outTableOffset = destPos;
  std::vector<TableDirectory> tdList(numTablesVal);
  for (int i = 0; i < numTablesVal; ++i)
  {
    TableDirectory& td = tdList[i];
    ArrayCopy(fontData.data(), srcPos, td.tag, 0, 4);
    srcPos += 4;
    td.offset = BytesToUInt(fontData, srcPos);
    srcPos += 4;

    if (td.offset % 4 != 0)
    {
      return nullptr;
    }

    td.compLength = BytesToUInt(fontData, srcPos);
    srcPos += 4;
    ArrayCopy(fontData.data(), srcPos, td.origLength, 0, 4);
    td.origLengthVal = BytesToUInt(fontData, srcPos);
    srcPos += 4;
    ArrayCopy(fontData.data(), srcPos, td.origChecksum, 0, 4);
    srcPos += 4;

    outTableOffset += 4 * 4;
  }

  for (TableDirectory& td : tdList)
  {
    ArrayCopy(td.tag, 0, otfBytes.data(), destPos, 4);
    destPos += 4;

    ArrayCopy(td.origChecksum, 0, otfBytes.data(), destPos, 4);
    destPos += 4;

    otfBytes[destPos] = static_cast<uint8_t>(outTableOffset >> 24);
    otfBytes[destPos + 1] = static_cast<uint8_t>(outTableOffset >> 16);
    otfBytes[destPos + 2] = static_cast<uint8_t>(outTableOffset >> 8);
    otfBytes[destPos + 3] = static_cast<uint8_t>(outTableOffset);
    destPos += 4;

    ArrayCopy(td.origLength, 0, otfBytes.data(), destPos, 4);
    destPos += 4;

    td.outOffset = outTableOffset;

    outTableOffset += (int)td.origLengthVal;
    if (outTableOffset % 4 != 0)
    {
      outTableOffset += 4 - outTableOffset % 4;
    }
  }

  if (outTableOffset != totalSfntSize)
  {
    return nullptr;
  }

  for (TableDirectory& td : tdList)
  {
    std::vector<uint8_t> compressedData(td.compLength);
    std::vector<uint8_t> uncompressedData;
    ArrayCopy(fontData.data(), td.offset, compressedData.data(), 0, td.compLength);
    int expectedUncompressedLen = (int)td.origLengthVal;
    if (td.compLength > td.origLengthVal)
    {
      return nullptr;
    }
    if (td.compLength != td.origLengthVal)
    {
      wxMemoryOutputStream compressedStream;
      compressedStream.Write(compressedData.data(), compressedData.size());
      wxMemoryInputStream in(compressedStream);
      wxZlibInputStream zin(in);
      wxMemoryOutputStream tblOut;
      tblOut.Write(zin);
      tblOut.Close();
      wxMemoryInputStream tblIn(tblOut);

      wxFileOffset streamLen = tblIn.SeekI(0, wxFromEnd);
      std::vector<uint8_t> uncompressedData(expectedUncompressedLen);
      tblIn.SeekI(0, wxFromStart);
      tblIn.ReadAll(uncompressedData.data(), uncompressedData.size());
      ArrayCopy(uncompressedData.data(), 0, otfBytes.data(), td.outOffset, expectedUncompressedLen);
    }
    else
    {
      ArrayCopy(compressedData.data(), 0, otfBytes.data(), td.outOffset, expectedUncompressedLen);
    }
  }

  wxMemoryOutputStream otfStream;
  otfStream.Write(otfBytes.data(), otfBytes.size());
  wxMemoryInputStream* finalStream = new wxMemoryInputStream(otfStream);
  return finalStream;
}
