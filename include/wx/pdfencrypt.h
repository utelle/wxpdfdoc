/*
** Name:        pdfencrypt.h
** Purpose:     Encryption support for PDF
** Author:      Ulrich Telle
** Created:     2005-08-16
** Copyright:   (c) 2005-2024 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file pdfencrypt.h Interface of the wxPdfFont class

#ifndef _PDF_ENCRYPT_H_
#define _PDF_ENCRYPT_H_

// wxWidgets headers
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

// Forward declaration of exported classes
class WXDLLIMPEXP_FWD_PDFDOC wxPdfRijndael;

class wxPdfRandomBytesGenerator;

/// Class representing PDF encryption methods. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfEncrypt
{
public:
  /// Constructor
  /**
  * \param revision revision of the encryption algorithm
  * \param keyLength length of the key for the encryption
  */
  wxPdfEncrypt(int revision = 2, int keyLength = 40);

  /// Default destructor
  virtual ~wxPdfEncrypt();

  /// Generate encryption key from user and owner passwords and protection key
  /**
  * \param userPassword password of the user
  * \param ownerPassword password of the owner
  * \param protection protection flags
  * \param documentId optional document identification
  */
  void GenerateEncryptionKey(const wxString& userPassword,
                             const wxString& ownerPassword,
                             int protection,
                             const wxString& documentId = wxEmptyString);

  /// Authenticate a document
  /**
  * \param documentID the identification of the document
  * \param password the password given by the user
  * \param uValue the U value from the document's encryption dictionary
  * \param oValue the O value from the document's encryption dictionary
  * \param ueValue the UE value from the document's encryption dictionary
  * \param oeValue the OE value from the document's encryption dictionary
  * \param permsValue the Perms value from the document's encryption dictionary
  * \param pValue the P value from the document's encryption dictionary
  * \param lengthValue the length value  from the document's encryption dictionary
  * \param rValue the R value from the document's encryption dictionary
  * \param vValue the V value from the document's encryption dictionary
  * \return TRUE if the document could be authenticated successfully, FALSE otherwise
  */
  bool Authenticate(const wxString& documentID, const wxString& password,
                    const wxString& uValue, const wxString& oValue,
                    const wxString& ueValue, const wxString& oeValue, const wxString& permsValue,
                    int pValue, int lengthValue, int rValue, int vValue);

  bool PasswordIsValid(const wxString& password) const;

  /// Get the U object value (user)
  const std::string& GetUValue() const { return m_uValue; }

  /// Get the UE object value (user)
  const std::string& GetUEValue() const { return m_ue; }

  /// Get the O object value (owner)
  const std::string& GetOValue() const { return m_oValue; }

  /// Get the OE object value (owner)
  const std::string& GetOEValue() const { return m_oe; }

  /// Get the P object value (protection)
  int GetPValue() const { return m_pValue; }

  /// Get the Perms object value (protection)
  const std::string& GetPermsValue() const { return m_permsValue; }

  /// Get the revision number of the encryption method
  int GetRevision() const { return m_rValue; }

  /// Get the key length of the encryption key in bits
  unsigned int GetKeyLength() const { return m_keyLength*8; }

  /// Encrypt a wxString
  /**
  * \param n number of the associated PDF object
  * \param g generation of the associated PDF object
  * \param str string to encrypt
  */
  void Encrypt(int n, int g, wxString& str);

  /// Decrypt a wxString
  /**
  * \param n number of the associated PDF object
  * \param g generation of the associated PDF object
  * \param str string to decrypt
  */
  void Decrypt(int n, int g, wxString& str);

  /// Encrypt a character string
  /**
  * \param n number of the associated PDF object
  * \param g generation of the associated PDF object
  * \param str string to encrypt
  * \param len length of the string to encrypt
  */
  void Encrypt(int n, int g, unsigned char* str, unsigned int len);

  /// Decrypt a character string
  /**
  * \param n number of the associated PDF object
  * \param g generation of the associated PDF object
  * \param str string to decrypt
  * \param len length of the string to decrypt
  */
  int Decrypt(int n, int g, unsigned char* str, unsigned int len);

  /// Calculate stream size
  /**
  * \param length length of the original stream
  * \return the length of the encrypted stream
  */
  size_t CalculateStreamLength(size_t length);

  /// Calculate stream offset
  /**
  * \return the offset of the stream
  */
  size_t CalculateStreamOffset();

  /// Get document id
  /**
  * \return the associated document identifier
  */
  wxString GetDocumentId() const { return m_documentId; }

  /// Create document id
  /**
  * \return the created document identifier
  */
  static wxString CreateDocumentId();

protected:
  /// Pad a password to 32 characters
  std::string PadPassword(const wxString& password);

  /// Compute owner key
  std::string ComputeOwnerKey(const std::string& userPad, const std::string& ownerPad,
                              unsigned int keylength, int revision, bool authenticate);

  /// Compute encryption key and user key
  std::string ComputeEncryptionKey(const wxString& documentID,
                                   const std::string& userPad, const std::string& ownerKey,
                                   int pValue, unsigned int keyLength, int revision);

  /// Check two keys for equality
  bool CheckKey(const std::string& key1, const std::string& key2);

  /// RC4 encryption
  void RC4(unsigned char* key, unsigned int keylen,
           const unsigned char* textin, unsigned int textlen,
           unsigned char* textout);

  /// AES-128 encryption
  void AESEncrypt(unsigned char* key, unsigned int keylen,
                  unsigned char* textin, unsigned int textlen,
                  unsigned char* textout);

  /// AES-128 decryption
  int AESDecrypt(unsigned char* key, unsigned int keylen,
                 unsigned char* textin, unsigned int textlen,
                 unsigned char* textout);

  /// AES-256 encryption
  void AESV3Encrypt(const unsigned char* key, unsigned int keylen,
                    const unsigned char* textin, unsigned int textlen,
                    unsigned char* textout);

  /// AES-256 decryption
  int AESV3Decrypt(const unsigned char* key, unsigned int keylen,
                   unsigned char* textin, unsigned int textlen,
                   unsigned char* textout);

  /// Calculate the binary MD5 message digest of the given data
  static void GetMD5Binary(const unsigned char* data, unsigned int length, unsigned char* digest);

  /// Generate initial vector
  static void GenerateInitialVector(unsigned char iv[16]);

private:
  /// Compute encryption parameters for revision below 5
  void ComputeEncryptionParameters(const wxString& userPassword, const wxString& ownerPassword);

  /// Compute encryption parameters for revision 5 and higher
  void ComputeEncryptionParametersV5(const wxString& userPassword, const wxString& ownerPassword);

  /// Prepare UTF-8 password
  std::string PreparePasswordV5(const std::string& password);

  /// Compute U and UE for revision 5 and higher
  void ComputeUandUEforV5(const std::string& userPassword, const std::string& encryptionKey);

  /// Compute O and OE for revision 5 and higher
  void ComputeOandOEforV5(const std::string& ownerPassword, const std::string& encryptionKey);

  /// Compute Perms for revision 5 and higher
  void ComputePermsV5(const std::string& encryptionKey);

  /// Compute hash for revision 5 and higher
  std::string HashV5(const std::string& password, const std::string& salt, const std::string& udata);

  /// Check for valid password for revision below 5
  bool CheckEncryptionParameters(const wxString& password);

  /// Check for valid password for revision 5 and higher
  bool CheckEncryptionParametersV5(const wxString& password);

  /// Check for valid owner password for revision 5 and higher
  bool CheckOwnerPasswordV5(const std::string& password);

  /// Check for valid user password for revision 5 and higher
  bool CheckUserPasswordV5(const std::string& userPassword);

  wxString       m_documentId;         ///< Document ID
  std::string    m_uValue;             ///< U entry in pdf document
  std::string    m_oValue;             ///< O entry in pdf document
  int            m_pValue;             ///< P entry in pdf document
  int            m_rValue;             ///< Revision
  int            m_vValue;             ///< Version
  unsigned char  m_encryptionKey[16];  ///< Encryption key
  unsigned int   m_keyLength;          ///< Length of encryption key
  unsigned char  m_rc4key[16];         ///< last RC4 key
  unsigned char  m_rc4last[256];       ///< last RC4 state table

  std::string m_u;
  std::string m_ue;
  std::string m_uValidSalt;
  std::string m_uKeySalt;
  std::string m_o;
  std::string m_oe;
  std::string m_oValidSalt;
  std::string m_oKeySalt;
  std::string m_permsValue;
  std::string m_fileEncryptionKey;
  bool m_encryptMetaData;

  wxPdfRijndael* m_aes;                ///< AES encryptor
  wxPdfRandomBytesGenerator* m_rbg;    ///< Random bytes generator
};

/// Class representing SHA2 hashing methods. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfSHA2
{
public:
};

#endif
