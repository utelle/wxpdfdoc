/*
** Name:        sha384.h
** Purpose:     SHA-384 hashing
** Copyright:   (c) 2024-2024 Ulrich Telle
** SPDX-License-Identifier: MIT
*/

// SHA-384. Adapted from LibTomCrypt (Public Domain).

#pragma once
#ifndef WXPDFDOC_CRYPTO_SHA384_H_
#define WXPDFDOC_CRYPTO_SHA384_H_

// SHA-384 is a truncated SHA-512 with different input vector.
#include "sha512.h"

namespace wxpdfdoc {
namespace crypto {

struct sha384_state
{
    sha512_state md;
};

void sha_init(sha384_state& md);
void sha_process(sha384_state& md, const void* in, std::uint32_t inlen);
void sha_done(sha384_state& md, void* out);

} // namespace crypto
} // namespace wxpdfdoc

#endif // WXPDFDOC_CRYPTO_SHA384_H_
