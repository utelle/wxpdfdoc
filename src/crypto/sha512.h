/*
** Name:        sha512.h
** Purpose:     SHA-512 hashing
** Copyright:   (c) 2024-2024 Ulrich Telle
** SPDX-License-Identifier: MIT
*/

// SHA-512. Adapted from LibTomCrypt (Public Domain).

#pragma once
#ifndef WXPDFDOC_CRYPTO_SHA512_H_
#define WXPDFDOC_CRYPTO_SHA512_H_

#include <cstdint>

namespace wxpdfdoc {
namespace crypto {

struct sha512_state
{
    std::uint64_t length;
    std::uint64_t state[8];
    std::uint32_t curlen;
    unsigned char buf[128];
};

void sha_init(sha512_state& md);
void sha_process(sha512_state& md, const void* in, std::uint32_t inlen);
void sha_done(sha512_state& md, void* out);

} // namespace crypto
} // namespace wxpdfdoc

#endif // WXPDFDOC_CRYPTO_SHA512_H_
