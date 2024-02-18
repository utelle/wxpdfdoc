/*
** Name:        random.h
** Purpose:     Random bytes generator based on ChaCha20 cipher stream
** Copyright:   (c) 2024-2024 Ulrich Telle
** SPDX-License-Identifier: MIT
**
** The code was taken from the public domain implementation
** of the sqleet project (https://github.com/resilar/sqleet)
*/

#pragma once
#ifndef WXPDFDOC_CRYPTO_RANDOM_H_
#define WXPDFDOC_CRYPTO_RANDOM_H_

#include <cstdint>

namespace wxpdfdoc {
namespace crypto {

/// Class representing a cryptographically secure random number generator
class RandomBytesGenerator
{
public:
  /// Constructor for text annotation
  RandomBytesGenerator() = default;

  /// Copy constructor
  RandomBytesGenerator(const RandomBytesGenerator&) = delete;
  RandomBytesGenerator& operator=(const RandomBytesGenerator&) = delete;

  /// Move constructors
  RandomBytesGenerator(RandomBytesGenerator&&) = delete;
  RandomBytesGenerator& operator=(RandomBytesGenerator&&) = delete;

  /// Destructor
  virtual ~RandomBytesGenerator() = default;

  /// Get the X offset of the annotation
  void GetRandomBytes(uint8_t* out, size_t n);

private:
  uint8_t key[32] = { 0 };
  uint8_t nonce[12] = { 0 };
  uint8_t buffer[64] = { 0 };
  uint32_t counter = 0;
  size_t available = 0;
};


} // namespace crypto
} // namespace wxpdfdoc

#endif // WXPDFDOC_CRYPTO_RANDOM_H_
