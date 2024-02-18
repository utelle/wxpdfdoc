/*
** Name:        pdfencrypt.cpp
** Purpose:     Encryption support for PDF
** Author:      Ulrich Telle
** Created:     2005-08-17
** Copyright:   (c) 2005-2024 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file pdfencrypt.cpp Implementation of the wxPdfEncrypt class

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes
#include <wx/log.h>

#include "wx/pdfencrypt.h"
#include "wx/pdfrijndael.h"
#include "wx/pdfutility.h"
#include "wx/pdfproperties.h"

#include "crypto/sha256.h"
#include "crypto/sha384.h"
#include "crypto/sha384.h"
#include "crypto/random.h"
#include "crypto/saslprep.h"

using namespace wxpdfdoc::crypto;

#define MD5_HASHBYTES 16

class wxPdfRandomBytesGenerator : public RandomBytesGenerator
{
};

/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * This software was written by Alexander Peslyak in 2001.  No copyright is
 * claimed, and the software is hereby placed in the public domain.
 * In case this attempt to disclaim copyright and place the software in the
 * public domain is deemed null and void, then the software is
 * Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 * general public under the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 *
 * (This is a heavily cut-down "BSD license".)
 *
 * This differs from Colin Plumb's older public domain implementation in that
 * no exactly 32-bit integer data type is required (any 32-bit or wider
 * unsigned integer data type will do), there's no compile-time endianness
 * configuration, and the function prototypes match OpenSSL's.  No code from
 * Colin Plumb's implementation has been reused; this comment merely compares
 * the properties of the two independent implementations.
 *
 * The primary goals of this implementation are portability and ease of use.
 * It is meant to be fast, but not as fast as possible.  Some known
 * optimizations are not included to reduce source code size and avoid
 * compile-time configuration.
 */

#include <string.h>

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD5_u32plus;

typedef struct {
  MD5_u32plus lo, hi;
  MD5_u32plus a, b, c, d;
  unsigned char buffer[64];
  MD5_u32plus block[16];
} MD5_CTX;

static void MD5_Init(MD5_CTX *ctx);
static void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);
static void MD5_Final(unsigned char *result, MD5_CTX *ctx);

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)			(((x) ^ (y)) ^ (z))
#define H2(x, y, z)			((x) ^ ((y) ^ (z)))
#define I(x, y, z)			((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#define STEP(f, a, b, c, d, x, t, s) \
	(a) += f((b), (c), (d)) + (x) + (t); \
	(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
	(a) += (b);

/*
 * SET reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * doesn't work.
 */
#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#define SET(n) \
	(*(MD5_u32plus *)&ptr[(n) * 4])
#define GET(n) \
	SET(n)
#else
#define SET(n) \
	(ctx->block[(n)] = \
	(MD5_u32plus)ptr[(n) * 4] | \
	((MD5_u32plus)ptr[(n) * 4 + 1] << 8) | \
	((MD5_u32plus)ptr[(n) * 4 + 2] << 16) | \
	((MD5_u32plus)ptr[(n) * 4 + 3] << 24))
#define GET(n) \
	(ctx->block[(n)])
#endif

/*
 * This processes one or more 64-byte data blocks, but does NOT update
 * the bit counters.  There are no alignment requirements.
 */
static const void *body(MD5_CTX *ctx, const void *data, unsigned long size)
{
	const unsigned char *ptr;
	MD5_u32plus a, b, c, d;
	MD5_u32plus saved_a, saved_b, saved_c, saved_d;

	ptr = (const unsigned char *)data;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	do {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

/* Round 1 */
		STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
		STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
		STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
		STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
		STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
		STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
		STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
		STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
		STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
		STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
		STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
		STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
		STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
		STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
		STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
		STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

/* Round 2 */
		STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
		STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
		STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
		STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
		STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
		STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
		STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
		STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
		STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
		STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
		STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
		STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
		STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
		STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
		STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
		STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

/* Round 3 */
		STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
		STEP(H2, d, a, b, c, GET(8), 0x8771f681, 11)
		STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
		STEP(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
		STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
		STEP(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
		STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
		STEP(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
		STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
		STEP(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
		STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
		STEP(H2, b, c, d, a, GET(6), 0x04881d05, 23)
		STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
		STEP(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
		STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
		STEP(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

/* Round 4 */
		STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
		STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
		STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
		STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
		STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
		STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
		STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
		STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
		STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
		STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
		STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
		STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
		STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
		STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
		STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
		STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while (size -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}

void MD5_Init(MD5_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size)
{
	MD5_u32plus saved_lo;
	unsigned long used, available;

	saved_lo = ctx->lo;
	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += size >> 29;

	used = saved_lo & 0x3f;

	if (used) {
		available = 64 - used;

		if (size < available) {
			memcpy(&ctx->buffer[used], data, size);
			return;
		}

		memcpy(&ctx->buffer[used], data, available);
		data = (const unsigned char *)data + available;
		size -= available;
		body(ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		data = body(ctx, data, size & ~(unsigned long)0x3f);
		size &= 0x3f;
	}

	memcpy(ctx->buffer, data, size);
}

void MD5_Final(unsigned char *result, MD5_CTX *ctx)
{
	unsigned long used, available;

	used = ctx->lo & 0x3f;

	ctx->buffer[used++] = 0x80;

	available = 64 - used;

	if (available < 8) {
		memset(&ctx->buffer[used], 0, available);
		body(ctx, ctx->buffer, 64);
		used = 0;
		available = 64;
	}

	memset(&ctx->buffer[used], 0, available - 8);

	ctx->lo <<= 3;
	ctx->buffer[56] = ctx->lo;
	ctx->buffer[57] = ctx->lo >> 8;
	ctx->buffer[58] = ctx->lo >> 16;
	ctx->buffer[59] = ctx->lo >> 24;
	ctx->buffer[60] = ctx->hi;
	ctx->buffer[61] = ctx->hi >> 8;
	ctx->buffer[62] = ctx->hi >> 16;
	ctx->buffer[63] = ctx->hi >> 24;

	body(ctx, ctx->buffer, 64);

	result[0] = ctx->a;
	result[1] = ctx->a >> 8;
	result[2] = ctx->a >> 16;
	result[3] = ctx->a >> 24;
	result[4] = ctx->b;
	result[5] = ctx->b >> 8;
	result[6] = ctx->b >> 16;
	result[7] = ctx->b >> 24;
	result[8] = ctx->c;
	result[9] = ctx->c >> 8;
	result[10] = ctx->c >> 16;
	result[11] = ctx->c >> 24;
	result[12] = ctx->d;
	result[13] = ctx->d >> 8;
	result[14] = ctx->d >> 16;
	result[15] = ctx->d >> 24;

	memset(ctx, 0, sizeof(*ctx));
}


// ---------------------------
// wxPdfEncrypt implementation
// ---------------------------

static unsigned char padding[] =
  "\x28\xBF\x4E\x5E\x4E\x75\x8A\x41\x64\x00\x4E\x56\xFF\xFA\x01\x08\x2E\x2E\x00\xB6\xD0\x68\x3E\x80\x2F\x0C\xA9\xFE\x64\x53\x69\x7A";

wxPdfEncrypt::wxPdfEncrypt(int revision, int keyLength)
{
  m_rbg = new wxPdfRandomBytesGenerator();
  m_aes = (revision >= 4) ? new wxPdfRijndael() : nullptr;
  switch (revision)
  {
    case 6:
    case 5:
      m_rValue = revision;
      m_keyLength = 256 / 8;
      break;
    case 4:
      m_rValue = revision;
      m_keyLength = 128 / 8;
      break;
    case 3:
      keyLength = keyLength - keyLength % 8;
      keyLength = (keyLength >= 40) ? ((keyLength <= 128) ? keyLength : 128) : 40;
      m_rValue = revision;
      m_keyLength = keyLength / 8;
      break;
    case 2:
    default:
      m_rValue = revision;
      m_keyLength = 40 / 8;
      break;
  }

  int j;
  for (j = 0; j < 16; j++)
  {
    m_rc4key[j] = 0;
  }

  m_encryptMetaData = true;
}

wxPdfEncrypt::~wxPdfEncrypt()
{
  if (m_rValue >= 4)
  {
    delete m_aes;
  }
  delete m_rbg;
}

std::string
wxPdfEncrypt::PadPassword(const wxString& password)
{
  unsigned char pswd[32];
  unsigned int m = (unsigned int) password.Length();
  if (m > 32) m = 32;

  unsigned int j;
  unsigned int p = 0;
  wxString::const_iterator ch = password.begin();
  for (j = 0; j < m; j++)
  {
    pswd[p++] = (unsigned char) ((unsigned int) (*ch) & 0xff);
    ++ch;
  }
  for (j = 0; p < 32 && j < 32; j++)
  {
    pswd[p++] = padding[j];
  }
  return std::string(reinterpret_cast<char*>(pswd), 32);
}

void
wxPdfEncrypt::ComputeEncryptionParameters(const wxString& userPassword, const wxString& ownerPassword)
{
  // Pad passwords
  std::string userpswd = PadPassword(userPassword);
  std::string ownerpswd = PadPassword(ownerPassword);

  // Compute O value
  m_oValue = ComputeOwnerKey(userpswd, ownerpswd, m_keyLength * 8, m_rValue, false);

  m_uValue = ComputeEncryptionKey(m_documentId, userpswd,
                                  m_oValue, m_pValue, m_keyLength * 8, m_rValue);
}

std::string
wxPdfEncrypt::HashV5(const std::string& password, const std::string& salt, const std::string& udata)
{
  sha256_state hash;
  char result[64];
  sha_init(hash);
  sha_process(hash, password.c_str(), password.length());
  sha_process(hash, salt.c_str(), salt.length());
  sha_process(hash, udata.c_str(), udata.length());
  sha_done(hash, result);

  std::string K(result, 32);

  // AES-256 according to PDF 1.7 Adobe Extension Level 3
  // corresponding to V == 5 and R == 5
  if (m_rValue < 6)
    return K;

  // AES-256 according to PDF 2.0 (resp PDF 1.7 Adobe Extension Level 8)
  // Algorithm 2.B from ISO 32000-1 chapter 7: Computing a hash

  std::vector<unsigned char> K1andE(64 * (127 + 64 + 48), 0);
  unsigned char* dataK1 = K1andE.data();
  unsigned char* dataE = dataK1;

  int nRound = 0;
  bool done = false;
  while (!done)
  {
    ++nRound;
    std::string K1 = password + K + udata;
    size_t lenK1 = K1.length();
    size_t lenData = 64 * lenK1;
    unsigned char* data = dataK1;

    // Initialize K1
    for (int j = 0; j < 64; ++j)
    {
      memcpy(data, K1.c_str(), lenK1);
      data += lenK1;
    }

    m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Encrypt,
                reinterpret_cast<unsigned char*>(const_cast<char*>(K.substr(0,16).c_str())),
                wxPdfRijndael::Key16Bytes,
                reinterpret_cast<unsigned char*>(const_cast<char*>(K.substr(16, 16).c_str())));

    int len = m_aes->blockEncrypt(dataK1, lenData, dataE);

    // E16mod3 is equivalent to mod 3 of the first 16 bytes of E
    // taken as a (128-bit) big-endian number.
    int E16mod3 = 0;
    for (unsigned int i = 0; i < 16; ++i)
    {
      E16mod3 += static_cast<unsigned char>(dataE[i]);
    }
    E16mod3 %= 3;

    // Select SHA256, SHA384 or SHA512
    switch (E16mod3)
    {
      case 0:
        {
          sha256_state hash;
          sha_init(hash);
          sha_process(hash, dataE, lenData);
          sha_done(hash, result);
          K = std::string(result, 32);
        }
        break;
      case 1:
        {
          sha384_state hash;
          sha_init(hash);
          sha_process(hash, dataE, lenData);
          sha_done(hash, result);
          K = std::string(result, 48);
        }
        break;
      case 2:
        {
          sha512_state hash;
          sha_init(hash);
          sha_process(hash, dataE, lenData);
          sha_done(hash, result);
          K = std::string(result, 64);
        }
        break;
    }

    if (nRound >= 64)
    {
      unsigned int ch = dataE[lenData - 1];
      done = (ch <= static_cast<unsigned int>(nRound - 32));
    }
  }
  return K.substr(0, 32);
}

bool
wxPdfEncrypt::PasswordIsValid(const wxString& password) const
{
  if (m_rValue > 4)
  {
    std::string saslPassword;
    saslprep_rc rc = saslprep(password.utf8_string(), saslPassword);
    if (rc != SASLPREP_SUCCESS)
    {
      wxLogError(wxString(wxS("wxPdfEncrypt::PasswordIsValid: ")) + wxString(_("Invalid password for SASLprep.")));
    }
    return (rc == SASLPREP_SUCCESS);
  }

  return true;
}

std::string
wxPdfEncrypt::PreparePasswordV5(const std::string& password)
{
  std::string saslPassword;
  saslprep_rc rc = saslprep(password, saslPassword);
  if (rc != SASLPREP_SUCCESS)
  {
    wxLogError(wxString(wxS("wxPdfEncrypt::PreparePasswordV5: ")) + wxString(_("Invalid password for SASLprep.")));
  }

  return saslPassword.substr(0, std::min(static_cast<size_t>(127), saslPassword.length()));
}

void
wxPdfEncrypt::ComputeUandUEforV5(const std::string& userPassword, const std::string& encryptionKey)
{
  std::string password = PreparePasswordV5(userPassword);
  // Algorithm 8 from the PDF 2.0
  char k[16];
  m_rbg->GetRandomBytes(reinterpret_cast<unsigned char*>(k), sizeof(k));
  m_uValidSalt = std::string(k, 8);
  m_uKeySalt = std::string(k + 8, 8);
  m_u = HashV5(password, m_uValidSalt, "");
  m_uValue = m_u + m_uValidSalt + m_uKeySalt;
  std::string tempKey = HashV5(password, m_uKeySalt, "");

  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Encrypt,
              reinterpret_cast<unsigned char*>(const_cast<char*>(tempKey.c_str())),
              wxPdfRijndael::Key32Bytes,
              nullptr);
  unsigned char tempData[32];
  memcpy(tempData, encryptionKey.c_str(), 32);
  int len = m_aes->blockEncrypt(tempData, 32, tempData);
  m_ue = std::string(reinterpret_cast<char*>(tempData), 32);
}

void
wxPdfEncrypt::ComputeOandOEforV5(const std::string& ownerPassword, const std::string& encryptionKey)
{
  std::string password = PreparePasswordV5(ownerPassword);
  // Algorithm 9 from the PDF 2.0
  char k[16];
  m_rbg->GetRandomBytes(reinterpret_cast<unsigned char*>(k), sizeof(k));
  m_oValidSalt = std::string(k, 8);
  m_oKeySalt = std::string(k + 8, 8);
  m_o = HashV5(password, m_oValidSalt, m_uValue);
  m_oValue = m_o + m_oValidSalt + m_oKeySalt;
  std::string tempKey = HashV5(password, m_oKeySalt, m_uValue);

  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Encrypt,
              reinterpret_cast<unsigned char*>(const_cast<char*>(tempKey.c_str())),
              wxPdfRijndael::Key32Bytes,
              nullptr);
  unsigned char tempData[32];
  memcpy(tempData, encryptionKey.c_str(), 32);
  int len = m_aes->blockEncrypt(tempData, 32, tempData);
  m_oe = std::string(reinterpret_cast<char*>(tempData), 32);
}

void
wxPdfEncrypt::ComputePermsV5(const std::string& encryptionKey)
{
  // Algorithm 10 from the PDF 2.0
  unsigned char perms[16];

  // First 4 bytes = 32bits permissions
  perms[0] = ((unsigned)m_pValue >> 0) & 0xFF;
  perms[1] = ((unsigned)m_pValue >> 8) & 0xFF;
  perms[2] = ((unsigned)m_pValue >> 16) & 0xFF;
  perms[3] = ((unsigned)m_pValue >> 24) & 0xFF;

  // Placeholder for future versions that may need 64-bit permissions
  perms[4] = 0xFF;
  perms[5] = 0xFF;
  perms[6] = 0xFF;
  perms[7] = 0xFF;

  // if EncryptMetadata is false, this value should be set to 'F'
  perms[8] = m_encryptMetaData ? 'T' : 'F';
  // Next 3 bytes are mandatory
  perms[9] = 'a';
  perms[10] = 'd';
  perms[11] = 'b';

  // Next 4 bytes are ignored, fill with random data
  m_rbg->GetRandomBytes(&perms[12], 4);

  m_aes->init(wxPdfRijndael::ECB, wxPdfRijndael::Encrypt,
              reinterpret_cast<unsigned char*>(const_cast<char*>(encryptionKey.c_str())),
              wxPdfRijndael::Key32Bytes,
              nullptr);
  int len = m_aes->blockEncrypt(perms, sizeof(perms), perms);
  m_permsValue = std::string(reinterpret_cast<char*>(perms), sizeof(perms));
}

void
wxPdfEncrypt::ComputeEncryptionParametersV5(const wxString& userPassword, const wxString& ownerPassword)
{
  // Initialize file encryption key with random bytes
  std::vector<unsigned char> key(m_keyLength);
  unsigned char* dataK = key.data();
  m_rbg->GetRandomBytes(dataK, m_keyLength);
  m_fileEncryptionKey = std::string(reinterpret_cast<char*>(dataK), m_keyLength);

  ComputeUandUEforV5(userPassword.utf8_string(), m_fileEncryptionKey);
  ComputeOandOEforV5(ownerPassword.utf8_string(), m_fileEncryptionKey);
  ComputePermsV5(m_fileEncryptionKey);
}

void
wxPdfEncrypt::GenerateEncryptionKey(const wxString& userPassword,
                                    const wxString& ownerPassword,
                                    int protection,
                                    const wxString& documentId)
{
  // Compute P value
  m_pValue = protection;

  // Compute encryption key and U value
  if (documentId.IsEmpty())
  {
    m_documentId = CreateDocumentId();
  }
  else
  {
    m_documentId = documentId;
  }

  // Compute encryption parameters depending on algorithm revision
  if (m_rValue < 5)
    ComputeEncryptionParameters(userPassword, ownerPassword);
  else
    ComputeEncryptionParametersV5(userPassword, ownerPassword);
}

bool
wxPdfEncrypt::CheckEncryptionParameters(const wxString& password)
{
  // Pad password
  std::string pswd = PadPassword(password);

  // Check password:
  //   1) as user password,
  //   2) as owner password
  bool checkPermissions = true;
  std::string userKey = ComputeEncryptionKey(m_documentId, pswd, m_oValue, m_pValue, m_keyLength, m_rValue);
  bool ok = CheckKey(userKey, m_uValue);
  if (!ok)
  {
    std::string userpswd = ComputeOwnerKey(m_oValue, pswd, m_keyLength, m_rValue, true);
    userKey = ComputeEncryptionKey(m_documentId, userpswd, m_oValue, m_pValue, m_keyLength, m_rValue);
    ok = CheckKey(userKey, m_uValue);
    checkPermissions = false;
  }

  if (checkPermissions)
  {
    static int requiredPermissions = wxPDF_PERMISSION_COPY | wxPDF_PERMISSION_PRINT;
    ok = ok && ((m_pValue & requiredPermissions) == requiredPermissions);
    if (!ok)
    {
      wxLogError(wxString(wxS("wxPdfEncrypt::CheckEncryptionParameters: ")) +
                 wxString(_("Import of document not allowed due to missing permissions.")));
    }
  }

  return ok;
}

bool
wxPdfEncrypt::CheckUserPasswordV5(const std::string& password)
{
  std::string validSalt = m_uValue.substr(32, 8);
  std::string uValue = HashV5(password, validSalt, "");
  return uValue == m_uValue.substr(0, 32);
}

bool
wxPdfEncrypt::CheckOwnerPasswordV5(const std::string& password)
{
  // Algorithm 12 from PDF 2.0
  std::string validSalt = m_oValue.substr(32, 8);
  std::string oValue = HashV5(password, validSalt, m_uValue);
  return oValue == m_oValue.substr(0, 32);
}

bool
wxPdfEncrypt::CheckEncryptionParametersV5(const wxString& password)
{
  bool ok = false;
  bool checkPermissions = true;
  std::string pswd = PreparePasswordV5(password.utf8_string());

  // Algorithm 2A from the PDF 2.0
  std::string keySalt;
  std::string userData;
  std::string encryptedFileKey;
  if (CheckOwnerPasswordV5(pswd))
  {
    keySalt = m_oValue.substr(40, 8);
    userData = m_uValue.substr(0, 48);
    encryptedFileKey = m_oe.substr(0, 32);
    ok = true;
    checkPermissions = false;
  }
  else if (CheckUserPasswordV5(pswd))
  {
    keySalt = m_uValue.substr(40, 8);
    encryptedFileKey = m_ue.substr(0, 32);
    ok = true;
  }

  std::string tempKey = HashV5(pswd, keySalt, userData);
  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Decrypt,
              reinterpret_cast<unsigned char*>(const_cast<char*>(tempKey.c_str())),
              wxPdfRijndael::Key32Bytes,
              nullptr);
  unsigned char tempData[32];
  memcpy(tempData, encryptedFileKey.c_str(), 32);
  int len = m_aes->blockDecrypt(tempData, 32, tempData);
  std::string fileKey = std::string(reinterpret_cast<char*>(tempData), 32);

  // Decrypt Perms and check against expected value

  unsigned char permsExpected[12];

  // First 4 bytes = 32bits permissions
  permsExpected[0] = ((unsigned)m_pValue >> 0) & 0xFF;
  permsExpected[1] = ((unsigned)m_pValue >> 8) & 0xFF;
  permsExpected[2] = ((unsigned)m_pValue >> 16) & 0xFF;
  permsExpected[3] = ((unsigned)m_pValue >> 24) & 0xFF;

  // Placeholder for future versions that may need 64-bit permissions
  permsExpected[4] = 0xFF;
  permsExpected[5] = 0xFF;
  permsExpected[6] = 0xFF;
  permsExpected[7] = 0xFF;

  // if EncryptMetadata is false, this value should be set to 'F'
  permsExpected[8] = m_encryptMetaData ? 'T' : 'F';
  // Next 3 bytes are mandatory
  permsExpected[9] = 'a';
  permsExpected[10] = 'd';
  permsExpected[11] = 'b';

  m_aes->init(wxPdfRijndael::ECB, wxPdfRijndael::Decrypt,
    reinterpret_cast<unsigned char*>(const_cast<char*>(fileKey.c_str())),
    wxPdfRijndael::Key32Bytes,
    nullptr);
  unsigned char perms[16];
  memcpy(perms, m_permsValue.c_str(), 16);
  len = m_aes->blockDecrypt(perms, sizeof(perms), perms);
  bool permsValid = (memcmp(perms, permsExpected, 12) == 0);
  ok = ok && permsValid;

  if (checkPermissions)
  {
    static int requiredPermissions = wxPDF_PERMISSION_COPY | wxPDF_PERMISSION_PRINT;
    ok = ok && ((m_pValue & requiredPermissions) == requiredPermissions);
    if (!ok)
    {
      wxLogError(wxString(wxS("wxPdfEncrypt::CheckEncryptionParametersV5: ")) +
                 wxString(_("Import of document not allowed due to missing permissions.")));
    }
  }

  if (ok)
  {
    m_fileEncryptionKey = fileKey;
  }
  return ok;
}

bool
wxPdfEncrypt::Authenticate(const wxString& documentID, const wxString& password,
                           const wxString& uValue, const wxString& oValue,
                           const wxString& ueValue, const wxString& oeValue, const wxString& permsValue,
                           int pValue, int lengthValue, int rValue, int vValue)
{
  bool ok = false;
  m_documentId = documentID;
  m_uValue = uValue.To8BitData();
  m_oValue = oValue.To8BitData();
  m_ue = ueValue.To8BitData();
  m_oe = oeValue.To8BitData();
  m_permsValue = permsValue.To8BitData();

  m_pValue = pValue;
  m_keyLength = lengthValue / 8;
  m_rValue = rValue;
  m_vValue = vValue;

  if (m_rValue <= 4)
    ok = CheckEncryptionParameters(password);
  else
    ok = CheckEncryptionParametersV5(password);

  return ok;
}

std::string
wxPdfEncrypt::ComputeOwnerKey(const std::string& userPad, const std::string& ownerPad,
                              unsigned int keyLength, int revision, bool authenticate)
{
  unsigned char ownerKey[32];
  unsigned char mkey[MD5_HASHBYTES];
  unsigned char digest[MD5_HASHBYTES];
  unsigned int length = keyLength / 8;
  const unsigned char* pUserPad = reinterpret_cast<const unsigned char*>(userPad.c_str());
  const unsigned char* pOwnerPad = reinterpret_cast<const unsigned char*>(ownerPad.c_str());

  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, pOwnerPad, 32);
  MD5_Final(digest,&ctx);

  if (revision == 3 || revision == 4)
  {
    // only use for the input as many bit as the key consists of
    unsigned int k;
    for (k = 0; k < 50; ++k)
    {
      MD5_Init(&ctx);
      MD5_Update(&ctx, digest, length);
      MD5_Final(digest,&ctx);
    }
    memcpy(ownerKey, pUserPad, 32);
    unsigned int i;
    unsigned int j;
    for (i = 0; i < 20; ++i)
    {
      for (j = 0; j < length ; ++j)
      {
        if (authenticate)
        {
          mkey[j] = (digest[j] ^ (19-i));
        }
        else
        {
          mkey[j] = (digest[j] ^ i);
        }
      }
      RC4(mkey, length, ownerKey, 32, ownerKey);
    }
  }
  else
  {
    RC4(digest, 5, pUserPad, 32, ownerKey);
  }
  return std::string(reinterpret_cast<char*>(ownerKey), 32);
}

std::string
wxPdfEncrypt::ComputeEncryptionKey(const wxString& documentId,
                                   const std::string& userPad, const std::string& ownerKey,
                                   int pValue, unsigned int keyLength, int revision)
{
  unsigned int k;
  m_keyLength = keyLength / 8;
  const unsigned char* pUserPad = reinterpret_cast<const unsigned char*>(userPad.c_str());
  const unsigned char* pOwnerKey = reinterpret_cast<const unsigned char*>(ownerKey.c_str());
  unsigned char userKey[32];

  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, pUserPad, 32);
  MD5_Update(&ctx, pOwnerKey, 32);

  unsigned char ext[4];
  ext[0] = (unsigned char) ( pValue        & 0xff);
  ext[1] = (unsigned char) ((pValue >>  8) & 0xff);
  ext[2] = (unsigned char) ((pValue >> 16) & 0xff);
  ext[3] = (unsigned char) ((pValue >> 24) & 0xff);
  MD5_Update(&ctx, ext, 4);

  unsigned int docIdLength = (unsigned int) documentId.Length();
  unsigned char* docId = NULL;
  if (docIdLength > 0)
  {
    docId = new unsigned char[docIdLength];
    unsigned int j;
    wxString::const_iterator dChar = documentId.begin();
    for (j = 0; j < docIdLength; j++)
    {
      docId[j] = (unsigned char) ((unsigned int) (*dChar) & 0xff);
      ++dChar;
    }
    MD5_Update(&ctx, docId, docIdLength);
  }

  // Revision 4 or greater: If document metadata is not being encrypted,
  // pass 4 bytes with the value 0xFFFFFFFF to the MD5 hash function.
  if (revision >= 4 && !m_encryptMetaData)
  {
    MD5_Update(&ctx, "\xff\xff\xff\xff", 4);
  }

  unsigned char digest[MD5_HASHBYTES];
  MD5_Final(digest,&ctx);

  // only use the really needed bits as input for the hash
  if (revision == 3 || revision == 4)
  {
    for (k = 0; k < 50; ++k)
    {
      MD5_Init(&ctx);
      MD5_Update(&ctx, digest, m_keyLength);
      MD5_Final(digest, &ctx);
    }
  }

  memcpy(m_encryptionKey, digest, m_keyLength);

  // Setup user key
  if (revision == 3 || revision == 4)
  {
    MD5_Init(&ctx);
    MD5_Update(&ctx, padding, 32);
    if (docId != NULL)
    {
      MD5_Update(&ctx, docId, docIdLength);
    }
    MD5_Final(digest, &ctx);
    memcpy(userKey, digest, 16);
    for (k = 16; k < 32; ++k)
    {
      userKey[k] = 0;
    }
    for (k = 0; k < 20; k++)
    {
      unsigned int j;
      for (j = 0; j < m_keyLength; ++j)
      {
        digest[j] = (unsigned char)(m_encryptionKey[j] ^ k);
      }
      RC4(digest, m_keyLength, userKey, 16, userKey);
    }
  }
  else
  {
    RC4(m_encryptionKey, m_keyLength, padding, 32, userKey);
  }
  if (docId != NULL)
  {
    delete [] docId;
  }
  return std::string(reinterpret_cast<char*>(userKey), 32);
}

bool
wxPdfEncrypt::CheckKey(const std::string& key1, const std::string& key2)
{
  // Check whether the right password had been given
  bool ok = true;
  int k;
  int kmax = (m_rValue == 3) ? 16 : 32;
  for (k = 0; ok && k < kmax; k++)
  {
    ok = ok && (key1[k] == key2[k]);
  }
  return ok;
}

void
wxPdfEncrypt::Encrypt(int n, int g, wxString& str)
{
  unsigned int len = (unsigned int) str.Length();
  unsigned char* data = new unsigned char[len];
  unsigned int j;
  for (j = 0; j < len; j++)
  {
    data[j] = (unsigned char) str.GetChar(j);
  }
  Encrypt(n, g, data, len);
  for (j = 0; j < len; j++)
  {
    str.SetChar(j, data[j]);
  }
  delete [] data;
}

void
wxPdfEncrypt::Decrypt(int n, int g, wxString& str)
{
  const wxCharBuffer stdstr(str.To8BitData());
  unsigned int len = (unsigned int) stdstr.length();
  unsigned char* data = new unsigned char[len];
  memcpy(data, stdstr, len);
  int realLen = Decrypt(n, g, data, len);
  size_t offset = CalculateStreamOffset();
  str = wxString::From8BitData(reinterpret_cast<char*>(data + offset), realLen);
  delete[] data;
}

void
wxPdfEncrypt::Encrypt(int n, int g, unsigned char* str, unsigned int len)
{
  if (m_rValue <= 4)
  {
    unsigned char objkey[MD5_HASHBYTES];
    unsigned char nkey[MD5_HASHBYTES + 5 + 4];
    unsigned int nkeylen = m_keyLength + 5;
    unsigned int j;
    for (j = 0; j < m_keyLength; j++)
    {
      nkey[j] = m_encryptionKey[j];
    }
    nkey[m_keyLength + 0] = 0xff & n;
    nkey[m_keyLength + 1] = 0xff & (n >> 8);
    nkey[m_keyLength + 2] = 0xff & (n >> 16);
    nkey[m_keyLength + 3] = 0xff & g;
    nkey[m_keyLength + 4] = 0xff & (g >> 8);

    if (m_rValue == 4)
    {
      // AES encryption needs some 'salt'
      nkeylen += 4;
      nkey[m_keyLength + 5] = 0x73;
      nkey[m_keyLength + 6] = 0x41;
      nkey[m_keyLength + 7] = 0x6c;
      nkey[m_keyLength + 8] = 0x54;
    }

    GetMD5Binary(nkey, nkeylen, objkey);
    int keylen = (m_keyLength <= 11) ? m_keyLength + 5 : 16;
    switch (m_rValue)
    {
    case 4:
      AESEncrypt(objkey, keylen, str, len, str);
      break;
    case 3:
    case 2:
    default:
      RC4(objkey, keylen, str, len, str);
      break;
    }
  }
  else
  {
    // Revision 5 or 6
    const unsigned char* key = reinterpret_cast<const unsigned char*>(m_fileEncryptionKey.c_str());
    AESV3Encrypt(key, 32, str, len, str);
  }
}

int
wxPdfEncrypt::Decrypt(int n, int g, unsigned char* str, unsigned int len)
{
  int realLen = len;
  if (m_rValue <= 4)
  {
    unsigned char objkey[MD5_HASHBYTES];
    unsigned char nkey[MD5_HASHBYTES + 5 + 4];
    unsigned int nkeylen = m_keyLength + 5;
    unsigned int j;
    for (j = 0; j < m_keyLength; j++)
    {
      nkey[j] = m_encryptionKey[j];
    }
    nkey[m_keyLength + 0] = 0xff & n;
    nkey[m_keyLength + 1] = 0xff & (n >> 8);
    nkey[m_keyLength + 2] = 0xff & (n >> 16);
    nkey[m_keyLength + 3] = 0xff & g;
    nkey[m_keyLength + 4] = 0xff & (g >> 8);

    if (m_rValue == 4)
    {
      // AES encryption needs some 'salt'
      nkeylen += 4;
      nkey[m_keyLength + 5] = 0x73;
      nkey[m_keyLength + 6] = 0x41;
      nkey[m_keyLength + 7] = 0x6c;
      nkey[m_keyLength + 8] = 0x54;
    }

    GetMD5Binary(nkey, nkeylen, objkey);
    int keylen = (m_keyLength <= 11) ? m_keyLength + 5 : 16;
    switch (m_rValue)
    {
    case 4:
      realLen = AESDecrypt(objkey, keylen, str, len, str);
      break;
    case 3:
    case 2:
    default:
      RC4(objkey, keylen, str, len, str);
      break;
    }
  }
  else
  {
    // Revision 5 or 6
    const unsigned char* key = reinterpret_cast<const unsigned char*>(m_fileEncryptionKey.c_str());
    realLen = AESV3Decrypt(key, 32, str, len, str);
  }
  return realLen;
}

/**
* RC4 is the standard encryption algorithm used in PDF format
*/

void
wxPdfEncrypt::RC4(unsigned char* key, unsigned int keylen,
                  const unsigned char* textin, unsigned int textlen,
                  unsigned char* textout)
{
  unsigned int i;
  unsigned int j;
  int t;
  unsigned char rc4[256];

  if (memcmp(key,m_rc4key,keylen) != 0)
  {
    for (i = 0; i < 256; i++)
    {
      rc4[i] = i;
    }
    j = 0;
    for (i = 0; i < 256; i++)
    {
      t = rc4[i];
      j = (j + t + key[i % keylen]) % 256;
      rc4[i] = rc4[j];
      rc4[j] = t;
    }
    memcpy(m_rc4key,key,keylen);
    memcpy(m_rc4last,rc4,256);
  }
  else
  {
    memcpy(rc4,m_rc4last,256);
  }

  int a = 0;
  int b = 0;
  unsigned char k;
  for (i = 0; i < textlen; i++)
  {
    a = (a + 1) % 256;
    t = rc4[a];
    b = (b + t) % 256;
    rc4[a] = rc4[b];
    rc4[b] = t;
    k = rc4[(rc4[a] + rc4[b]) % 256];
    textout[i] = textin[i] ^ k;
  }
}

void
wxPdfEncrypt::GetMD5Binary(const unsigned char* data, unsigned int length, unsigned char* digest)
{
  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, data, length);
  MD5_Final(digest,&ctx);
}

void
wxPdfEncrypt::AESEncrypt(unsigned char* key, unsigned int keylen,
                  unsigned char* textin, unsigned int textlen,
                  unsigned char* textout)
{
  wxUnusedVar(keylen);
  GenerateInitialVector(textout);
  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Encrypt, key, wxPdfRijndael::Key16Bytes, textout);
  size_t offset = CalculateStreamOffset();
  int len = m_aes->padEncrypt(&textin[offset], textlen, &textout[offset]);

  // It is a good idea to check the error code
  if (len < 0)
  {
    wxLogError(wxString(wxS("wxPdfEncrypt::AES: ")) +
               wxString(_("Error on encrypting.")));
  }
}

int
wxPdfEncrypt::AESDecrypt(unsigned char* key, unsigned int keylen,
                         unsigned char* textin, unsigned int textlen,
                         unsigned char* textout)
{
  wxUnusedVar(keylen);
  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Decrypt, key, wxPdfRijndael::Key16Bytes, textin);
  size_t offset = CalculateStreamOffset();
  int len = m_aes->padDecrypt(&textin[offset], textlen-offset, &textout[offset]);

  // It is a good idea to check the error code
  if (len < 0)
  {
    wxLogError(wxString(wxS("wxPdfEncrypt::AES: ")) +
               wxString(_("Error on decrypting.")));
  }
  return len;
}

void
wxPdfEncrypt::AESV3Encrypt(const unsigned char* key, unsigned int keylen,
                    const unsigned char* textin, unsigned int textlen,
                    unsigned char* textout)
{
  wxUnusedVar(keylen);
  GenerateInitialVector(textout);
  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Encrypt, key, wxPdfRijndael::Key32Bytes, textout);
  size_t offset = CalculateStreamOffset();
  int len = m_aes->padEncrypt(&textin[offset], textlen, &textout[offset]);

  // It is a good idea to check the error code
  if (len < 0)
  {
    wxLogError(wxString(wxS("wxPdfEncrypt::AESV3: ")) +
               wxString(_("Error on encrypting.")));
  }
}

int
wxPdfEncrypt::AESV3Decrypt(const unsigned char* key, unsigned int keylen,
                           unsigned char* textin, unsigned int textlen,
                           unsigned char* textout)
{
  wxUnusedVar(keylen);
  m_aes->init(wxPdfRijndael::CBC, wxPdfRijndael::Decrypt, key, wxPdfRijndael::Key32Bytes, textin);
  size_t offset = CalculateStreamOffset();
  int len = m_aes->padDecrypt(&textin[offset], textlen-offset, &textout[offset]);

  // It is a good idea to check the error code
  if (len < 0)
  {
    wxLogError(wxString(wxS("wxPdfEncrypt::AESV3: ")) +
               wxString(_("Error on decrypting.")));
  }
  return len;
}

void
wxPdfEncrypt::GenerateInitialVector(unsigned char iv[16])
{
  wxString keyString = wxPdfUtility::GetUniqueId();
#if wxUSE_UNICODE
  const wxScopedCharBuffer cb(keyString.ToAscii());
  const char* key = (const char*) cb;
#else
  const char* key = (const char*) keyString.c_str();
#endif
  GetMD5Binary((const unsigned char*) key, (unsigned int) keyString.Length(), iv);
}

size_t
wxPdfEncrypt::CalculateStreamLength(size_t length)
{
  size_t realLength = length;
  if (m_rValue >= 4)
  {
//    realLength = (length % 0x7ffffff0) + 32;
    realLength = ((length + 15) & ~15) + 16;
    if (length % 16 == 0)
    {
      realLength += 16;
    }
  }
  return realLength;
}

size_t
wxPdfEncrypt::CalculateStreamOffset()
{
  size_t offset = 0;
  if (m_rValue >= 4)
  {
    offset = 16;
  }
  return offset;
}

wxString
wxPdfEncrypt::CreateDocumentId()
{
  wxString documentId;
  unsigned char id[16];
  GenerateInitialVector(id);
  int k;
  for (k = 0; k < 16; k++)
  {
    documentId.Append(wxUniChar((unsigned int) id[k]));
  }
  return documentId;
}
