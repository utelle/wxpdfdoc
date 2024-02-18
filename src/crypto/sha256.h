/*
** Name:        sha256.h
** Purpose:     SHA-256 hashing
** Copyright:   (c) 2024-2024 Ulrich Telle
** SPDX-License-Identifier: MIT
*/

// SHA-256. Adapted from LibTomCrypt (Public Domain).

#pragma once
#ifndef WXPDFDOC_CRYPTO_SHA256_H_
#define WXPDFDOC_CRYPTO_SHA256_H_

#include <cstdint>

namespace wxpdfdoc {
namespace crypto {

struct sha256_state
{
    std::uint64_t length;
    std::uint32_t state[8];
    std::uint32_t curlen;
    unsigned char buf[64];
};

void sha_init(sha256_state& md);
void sha_process(sha256_state& md, const void* in, std::uint32_t inlen);
void sha_done(sha256_state& md, void* out);

} // namespace crypto
} // namespace wxpdfdoc

#endif // WXPDFDOC_CRYPTO_SHA256_H_
