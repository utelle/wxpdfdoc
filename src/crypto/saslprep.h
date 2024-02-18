/*
** Name:        saslprep.h
** Purpose:     SASLprep normalization, for SCRAM authentication
** Copyright:   (c) 2024-2024 Ulrich Telle
** SPDX-License-Identifier: PostgreSQL
*/

// SASLprep. Adapted from PostgreSQL.

/*-------------------------------------------------------------------------
 *
 * saslprep.h
 *    SASLprep normalization, for SCRAM authentication
 *
 * These definitions are used by both frontend and backend code.
 *
 * Copyright (c) 2017-2023, PostgreSQL Global Development Group
 *
 * src/include/common/saslprep.h
 *
 *-------------------------------------------------------------------------
 */

#pragma once
#ifndef WXPDFDOC_CRYPTO_SASLPREP_H_
#define WXPDFDOC_CRYPTO_SASLPREP_H_

#include <string>

namespace wxpdfdoc {
namespace crypto {

/*
 * Return codes for saslprep() function.
 */
typedef enum
{
  SASLPREP_SUCCESS      =  0,
  SASLPREP_OOM          = -1,  // out of memory (only in frontend)
  SASLPREP_INVALID_UTF8 = -2,  // input is not a valid UTF-8 string
  SASLPREP_PROHIBITED   = -3   // output would contain prohibited characters
} saslprep_rc;

saslprep_rc saslprep(const std::string& intxt, std::string& outtxt);

} // namespace crypto
} // namespace wxpdfdoc

#endif // WXPDFDOC_CRYPTO_SASLPREP_H_
