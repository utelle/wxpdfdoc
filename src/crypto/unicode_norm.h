/*-------------------------------------------------------------------------
 *
 * unicode_norm.h
 *	  Routines for normalizing Unicode strings
 *
 * These definitions are used by both frontend and backend code.
 *
 * Copyright (c) 2017-2023, PostgreSQL Global Development Group
 *
 * src/include/common/unicode_norm.h
 *
 *-------------------------------------------------------------------------
 */

#pragma once
#ifndef WXPDFDOC_CRYPTO_UNICODE_NORM_H_
#define WXPDFDOC_CRYPTO_UNICODE_NORM_H_

namespace wxpdfdoc {
namespace crypto {

 /*
	* lengthof
	*		Number of elements in an array.
	*/
#define lengthof(array) (sizeof (array) / sizeof ((array)[0]))

typedef unsigned int pg_wchar;

typedef enum
{
	UNICODE_NFC = 0,
	UNICODE_NFD = 1,
	UNICODE_NFKC = 2,
	UNICODE_NFKD = 3,
} UnicodeNormalizationForm;

/* see UAX #15 */
typedef enum
{
	UNICODE_NORM_QC_NO = 0,
	UNICODE_NORM_QC_YES = 1,
	UNICODE_NORM_QC_MAYBE = -1,
} UnicodeNormalizationQC;

pg_wchar* unicode_normalize(UnicodeNormalizationForm form, const pg_wchar* input);

} // namespace crypto
} // namespace wxpdfdoc

#endif  // WXPDFDOC_CRYPTO_UNICODE_NORM_H_
