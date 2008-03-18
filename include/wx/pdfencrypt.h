///////////////////////////////////////////////////////////////////////////////
// Name:        pdfencrypt.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-08-16
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfencrypt.h Interface of the wxPdfFont class

#ifndef _PDFENCRYPT_H_
#define _PDFENCRYPT_H_

// wxWidgets headers
#include <wx/string.h>
#include <wx/wfstream.h>

#include "wx/pdfdocdef.h"
#include "wx/pdfrijndael.h"

/// Class representing PDF encryption methods. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfEncrypt
{
public:
  /// Default constructor
  wxPdfEncrypt(int revision = 2, int keyLength = 40);

  /// Default destructor
  virtual ~wxPdfEncrypt();

  /// Generate encryption key from user and owner passwords and protection key
  void GenerateEncryptionKey(const wxString& userPassword,
                             const wxString& ownerPassword,
                             int protection);

  bool Authenticate(const wxString& documentID, const wxString& password,
                    const wxString& uValue, const wxString& oValue,
                    int pValue, int lengthValue, int rValue);

  /// Get the U object value (user)
  const unsigned char* GetUValue() const { return m_uValue; }

  /// Get the O object value (owner)
  const unsigned char* GetOValue() const { return m_oValue; }

  /// Get the P object value (protection)
  int GetPValue() const { return m_pValue; }

  /// Get the revision number of the encryption method
  int GetRevision() const { return m_rValue; }

  /// Get the key length of the encryption key in bits
  int GetKeyLength() const { return m_keyLength*8; }

  /// Encrypt a wxString
  void Encrypt(int n, int g, wxString& str);

  /// Encrypt a character string
  void Encrypt(int n, int g, unsigned char* str, int len);

  /// Calculate stream size
  int CalculateStreamLength(int length);

  /// Calculate stream offset
  int CalculateStreamOffset();

  /// Create document id
  wxString CreateDocumentId();

  /// Get document id
  wxString GetDocumentId() const { return m_documentId; }

protected:
  /// Pad a password to 32 characters
  void PadPassword(const wxString& password, unsigned char pswd[32]);

  /// Compute owner key
  void ComputeOwnerKey(unsigned char userPad[32], unsigned char ownerPad[32],
                       int keylength, int revision, bool authenticate,
                       unsigned char ownerKey[32]);

  /// Compute encryption key and user key
  void ComputeEncryptionKey(const wxString& documentID,
                            unsigned char userPad[32], unsigned char ownerKey[32],
                            int pValue, int keyLength, int revision,
                            unsigned char userKey[32]);

  /// Check two keys for equality
  bool CheckKey(unsigned char key1[32], unsigned char key2[32]);

  /// RC4 encryption
  void RC4(unsigned char* key, int keylen,
           unsigned char* textin, int textlen,
           unsigned char* textout);

  /// Calculate the binary MD5 message digest of the given data
  void GetMD5Binary(const unsigned char* data, int length, unsigned char* digest);

  /// AES encryption
  void AES(unsigned char* key, int WXUNUSED(keylen),
           unsigned char* textin, int textlen,
           unsigned char* textout);

  /// Generate initial vector
  void GenerateInitialVector(unsigned char iv[16]);

private:
  wxString       m_documentId;         ///< Document ID
  unsigned char  m_uValue[32];         ///< U entry in pdf document
  unsigned char  m_oValue[32];         ///< O entry in pdf document
  int            m_pValue;             ///< P entry in pdf document
  int            m_rValue;             ///< Revision
  unsigned char  m_encryptionKey[16];  ///< Encryption key
  int            m_keyLength;          ///< Length of encryption key
  unsigned char  m_rc4key[16];         ///< last RC4 key
  unsigned char  m_rc4last[256];       ///< last RC4 state table

  wxPdfRijndael* m_aes;                ///< AES encryptor
};

#endif
