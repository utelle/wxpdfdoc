///////////////////////////////////////////////////////////////////////////////
// Name:        pdfparser.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2006-05-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfparser.h Interface of the wxPdfParser classes

#ifndef _PDF_PARSER_H_
#define _PDF_PARSER_H_

// wxWidgets headers
#include <wx/dynarray.h>
#include <wx/filesys.h>
#include <wx/mstream.h>
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraydouble.h"
#include "wx/pdfobjects.h"

class WXDLLIMPEXP_FWD_PDFDOC wxPdfEncrypt;
class WXDLLIMPEXP_FWD_PDFDOC wxPdfInfo;

/// Permissions required for import of a document
// Permission bit  3: Print
// Permission bit  5: Copy or extract text and graphics
// Permission bit 10: Extract text and graphics
// THIS MUST NOT BE CHANGED!
#define REQUIRED_PERMISSIONS 0x0214

/// Token types
#define TOKEN_COMMENT           1
#define TOKEN_BOOLEAN           2
#define TOKEN_NUMBER            3
#define TOKEN_STRING            4
#define TOKEN_NAME              5
#define TOKEN_START_ARRAY       6
#define TOKEN_END_ARRAY         7
#define TOKEN_START_DICTIONARY  8
#define TOKEN_END_DICTIONARY    9
#define TOKEN_REFERENCE        10
#define TOKEN_NULL             12
#define TOKEN_OTHER            13

/// Class representing a tokenizer for parsing PDF documents.
/**
* The tokenizer reads a PDF stream and breaks it into tokens like strings, names, numbers, etc.
* It also handles identifying the XRef (Cross-Reference) table, which contains the byte
* offsets of all indirect objects in the PDF file.
*/
class WXDLLIMPEXP_PDFDOC wxPdfTokenizer
{
public:
  /// Constructor
  /**
  * \param inputStream The stream to tokenize
  */
  wxPdfTokenizer(wxInputStream* inputStream);

  /// Destructor
  virtual ~wxPdfTokenizer();

  /// Set current offset position in stream
  /**
  * \param pos The offset to seek to
  * \return The new offset position
  */
  off_t Seek(off_t pos);

  /// Get current offset position in stream
  /**
  * \return The current offset position
  */
  off_t Tell();

  /// Go back one position in the stream
  /**
  * \param ch The character to put back
  */
  void BackOnePosition(int ch);

  /// Get length of stream
  /**
  * \return The total length of the stream
  */
  off_t GetLength();

  /// Read one byte from stream
  /**
  * \return The character read, or EOF
  */
  int ReadChar();

  /// Read size bytes from stream
  /**
  * \param size The number of bytes to read
  * \return A memory stream containing the read bytes
  */
  wxMemoryOutputStream* ReadBuffer(size_t size);

  /// Find the offset of the startxref tag
  /**
  * \return The byte offset of the cross-reference table
  */
  off_t GetStartXRef();

  /// Read a string
  /**
  * \param size The number of characters to read
  * \return The string read from the stream
  */
  wxString ReadString(int size);

  /// Check the header of the document stream
  /**
  * \return The PDF version string (e.g., "%PDF-1.4")
  */
  wxString CheckPdfHeader();

  /// Get the next token
  /**
  * \return @c true if a token was found, @c false otherwise
  */
  bool NextToken();

  /// Get the next valid token
  /**
  * Skips comments and whitespace to find the next meaningful token.
  */
  void NextValidToken();

  /// Get the type of the last token
  /**
  * \return The token type (e.g., @c TOKEN_STRING, @c TOKEN_NUMBER)
  */
  int GetTokenType() const;

  /// Get the token value as a string
  /**
  * \return The string value of the token
  */
  wxString GetStringValue() const;

  /// Get the token value as an integer
  /**
  * \return The integer value of the token
  */
  int GetIntValue() const;

  /// Check whether the token is a hexadecimal string
  /**
  * \return @c true if it is a hex string, @c false otherwise
  */
  bool IsHexString() const { return m_hexString; }

  /// Get object reference
  /**
  * \return The object reference number
  */
  int GetReference() const;

  /// Get object generation
  /**
  * \return The object generation number
  */
  int GetGeneration() const;

  /// Check byte whether it represents a white space character
  /**
  * \param ch The character to check
  * \return @c true if it is whitespace, @c false otherwise
  */
  static bool IsWhitespace(int ch);

  /// Check byte whether it is a delimiter
  /**
  * \param ch The character to check
  * \return @c true if it is a delimiter, @c false otherwise
  */
  static bool IsDelimiter(int ch);

  /// Check byte whether it is a delimiter or a whitespace character
  /**
  * \param ch The character to check
  * \return @c true if it is a delimiter or whitespace, @c false otherwise
  */
  static bool IsDelimiterOrWhitespace(int ch);

  /// Get hexadecimal character
  /**
  * \param v The integer value
  * \return The hex character value
  */
  static int GetHex(int v);

private:
  wxInputStream* m_inputStream; ///< Stream of document data
  int            m_type;        ///< Type of last token
  wxString       m_stringValue; ///< Value of last token
  int            m_reference;   ///< Reference number of object
  int            m_generation;  ///< Generation number of object
  bool           m_hexString;   ///< Flag for hexadecimal strings

};

/// Class representing an XRef entry (for internal use only)
/**
* This class stores information about an entry in the PDF cross-reference table,
* which is used to locate objects within the file or within object streams.
*/
class WXDLLIMPEXP_PDFDOC wxPdfXRefEntry
{
public:
  /// Constructor
  wxPdfXRefEntry();

  /// Destructor
  virtual ~wxPdfXRefEntry();

  int m_type;    ///< Type of XRef entry (0: free, 1: normal, 2: in stream)
  int m_ofs_idx; ///< Byte offset of object (type 1) or index in object stream (type 2)
  int m_gen_ref; ///< Generation number (type 1) or object number of the object stream (type 2)
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxPdfXRefEntry, wxPdfXRef, WXDLLIMPEXP_PDFDOC);

/// Class representing a PDF parser. (For internal use only)
/**
* The parser uses a tokenizer to read the PDF file, identifies all indirect objects
* via the cross-reference (XRef) table, and provides methods to access page-specific
* resources and content streams.
*/
class WXDLLIMPEXP_PDFDOC wxPdfParser
{
public:
  /// Constructor
  /**
  * \param filename The name of the PDF file to parse
  * \param password The password for encrypted documents
  */
  wxPdfParser(const wxString& filename,
              const wxString& password = wxEmptyString);

  /// Destructor
  virtual ~wxPdfParser();

  /// Check whether the PDF document to be parsed is valid
  /**
  * \return @c true if the document was parsed successfully, @c false otherwise
  */
  bool IsOk() const;

  /// Get PDF version of parsed document
  /**
  * \return The PDF version string
  */
  wxString GetPdfVersion() const { return m_pdfVersion; }

  /// Get number of pages in the parsed document
  /**
  * \return The number of pages
  */
  unsigned int GetPageCount() const;

  /// Get the document information dictionary
  /**
  * \param[out] info The info object to populate
  * \return @c true if successful, @c false otherwise
  */
  bool GetSourceInfo(wxPdfInfo& info);

  /// Get the queue of referenced objects
  /**
  * \return The object queue
  */
  wxPdfObjectQueue* GetObjectQueue() { return m_objectQueue; }

  /// Get the map of referenced objects
  /**
  * \return The object map
  */
  wxPdfObjectMap* GetObjectMap() { return m_objectMap; }

  /// Append a referenced object to the queue
  /**
  * \param originalObjectId The original ID in the source file
  * \param actualObjectId The new ID in the destination document
  * \param obj The PDF object
  */
  void AppendObject(int originalObjectId, int actualObjectId, wxPdfObject* obj);

  /// Get the resources of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The resources dictionary object
  */
  wxPdfObject* GetPageResources(unsigned int pageno);

  /// Get the content stream collection of a specific page
  /**
  * \param pageno The page number (1-based)
  * \param[out] contents Array to be filled with content stream objects
  */
  void GetContent(unsigned int pageno, wxArrayPtrVoid& contents);

  /// Get the media box of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The media box array
  */
  wxPdfArrayDouble* GetPageMediaBox(unsigned int pageno);

  /// Get the crop box of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The crop box array
  */
  wxPdfArrayDouble* GetPageCropBox(unsigned int pageno);

  /// Get the bleed box of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The bleed box array
  */
  wxPdfArrayDouble* GetPageBleedBox(unsigned int pageno);

  /// Get the trim box of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The trim box array
  */
  wxPdfArrayDouble* GetPageTrimBox(unsigned int pageno);

  /// Get the art box of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The art box array
  */
  wxPdfArrayDouble* GetPageArtBox(unsigned int pageno);

  /// Get the rotation of a specific page
  /**
  * \param pageno The page number (1-based)
  * \return The page rotation in degrees
  */
  int GetPageRotation (unsigned int pageno);

  /// Resolve an object
  /**
  * If the object is a reference, it is resolved to the actual object.
  * \param obj The object to resolve
  * \return The resolved PDF object
  */
  wxPdfObject* ResolveObject(wxPdfObject* obj);

  /// Set flag whether a stream should be decoded or not
  /**
  * \param useRawStream @c true to skip decoding, @c false to decode
  */
  void SetUseRawStream(bool useRawStream) { m_useRawStream = useRawStream; }

  /// Get flag whether a stream should be decoded or not
  /**
  * \return @c true if skip decoding, @c false if decode
  */
  bool GetUseRawStream() const { return m_useRawStream; }

protected:
  /// Get the resources of a specific page identified by a page object
  wxPdfObject* GetPageResources(wxPdfObject* page);

  /// Get the content stream collection of a specific page
  void GetPageContent(wxPdfObject* contentRef, wxArrayPtrVoid& contents);

  /// Get a page box
  wxPdfArrayDouble* GetPageBox(wxPdfDictionary* page, const wxString& boxIndex);

  /// Get a page rotation
  int GetPageRotation (wxPdfDictionary* page);

  /// Parse PDF document
  bool ParseDocument();

  /// Setup a decryptor
  bool SetupDecryptor();

  /// Parse the cross reference
  bool ParseXRef();

  /// Parse the page tree of the PDF document
  bool ParsePageTree(wxPdfDictionary* pages);

  /// Parse a cross reference section
  wxPdfDictionary* ParseXRefSection();

  /// Parse a cross reference stream
  bool ParseXRefStream(int ptr, bool setTrailer);

  /// Parse an object
  wxPdfObject* ParseObject();

  /// Parse a dictionary
  wxPdfDictionary* ParseDictionary();

  /// Parse an array
  wxPdfArray* ParseArray();

  /// Parse a specific object
  wxPdfObject* ParseSpecificObject(int idx);

  /// Parse a direct object
  wxPdfObject* ParseDirectObject(int k);

  /// Parse an object from an object stream
  wxPdfObject* ParseObjectStream(wxPdfStream* stream, int idx);

  /// Parse the content of a stream object
  void GetStreamBytes(wxPdfStream* stream);

  /// Parse the raw content of a stream object
  void GetStreamBytesRaw(wxPdfStream* stream);

  /// Decode a stream predictor
  wxMemoryOutputStream* DecodePredictor(wxMemoryOutputStream* in, wxPdfObject* dicPar);

  /// Decode a stream that has the FlateDecode filter.
  /**
   * \param osIn the input data
   * \return the decoded data
   */
  wxMemoryOutputStream* FlateDecode(wxMemoryOutputStream* osIn);

  /// Decode a stream that has the ASCIIHexDecode filter.
  /**
   * \param osIn the input data
   * \return the decoded data
   */
  wxMemoryOutputStream* ASCIIHexDecode(wxMemoryOutputStream* osIn);

  /// Decode a stream that has the ASCII85Decode filter.
  /**
   * \param osIn the input data
   * \return the decoded data
   */
  wxMemoryOutputStream* ASCII85Decode(wxMemoryOutputStream* osIn);

  /// Decode a stream that has the ASCII85Decode filter.
  /**
   * \param osIn the input data
   * \return the decoded data
   */
  wxMemoryOutputStream* LZWDecode(wxMemoryOutputStream* osIn);

  /// Get wxWidgets file system
  static wxFileSystem* GetFileSystem();

private:
  /// Reserve at least count cross reference entries
  void ReserveXRef(size_t count);

  bool              m_initialized;     ///< Flag whether parser is properly initialized
  int               m_fileSize;        ///< File size
  wxString          m_filename;        ///< File name of PDF document
  wxString          m_password;        ///<
  wxString          m_pdfVersion;      ///< Version of PDF document
  wxFSFile*         m_pdfFile;         ///< File system file object of PDF document
  wxPdfTokenizer*   m_tokens;          ///< Tokenizer
  wxPdfDictionary*  m_trailer;         ///< Trailer dictionary
  wxPdfDictionary*  m_root;            ///< Root object
  wxArrayPtrVoid    m_pages;           ///< Array of page objects
  unsigned int      m_currentPage;     ///< Number of current page
  bool              m_useRawStream;    ///< Flag whether to use raw stream data (without decoding)

  bool              m_encrypted;       ///< Flag whether the document is encrypted
  wxPdfEncrypt*     m_decryptor;       ///< decryptor instance
  wxPdfDictionary*  m_encryption;      ///< Encryption dictionary

  wxPdfObjectQueue* m_objectQueue;     ///< Queue of referenced objects
  wxPdfObjectQueue* m_objectQueueLast; ///< Pointer to last queue element
  wxPdfObjectMap*   m_objectMap;       ///< Map for object queue elements
  wxPdfObjStmMap*   m_objStmCache;     ///< Cache for object streams
  bool              m_cacheObjects;    ///< Flag whether object streams should be cached

  int               m_objNum;          ///< Number of current object
  int               m_objGen;          ///< Generation of current object

  wxPdfXRef         m_xref;            ///< Cross reference

  static wxFileSystem* ms_fileSystem; ///< wxWidgets file system
};

#define WXPDF_LZW_STRINGTABLE_SIZE 8192

/// Class representing an LZW decoder. (For internal use only)
/**
* Decodes streams that use the Lempel-Ziv-Welch (LZW) compression algorithm,
* commonly used in PDF and TIFF files.
*/
class WXDLLIMPEXP_PDFDOC wxPdfLzwDecoder
{
public:
  /// Constructor
  wxPdfLzwDecoder();

  /// Destructor
  virtual ~wxPdfLzwDecoder();

  /// Get next code from the encoded stream
  /**
  * \return The next LZW code
  */
  int GetNextCode();

  /// Decode a byte stream
  /**
  * \param dataIn The encoded input stream
  * \param dataOut The decoded output stream
  * \return @c true if successful, @c false otherwise
  */
  bool Decode(wxMemoryInputStream* dataIn, wxMemoryOutputStream* dataOut);

  /// Initialize the string table with initial 256 entries
  void InitializeStringTable();

  /// Write decoded string associated with code into output buffer
  /**
  * \param code The code to look up in the string table
  */
  void WriteString(int code);

  /// Add a new string to the string table
  /**
  * \param oldCode The previous code
  * \param newString The new character to append
  */
  void AddStringToTable(int oldCode, char newString);

private:
  wxMemoryInputStream*  m_dataIn;       ///< Encoded data stream
  wxMemoryOutputStream* m_dataOut;      ///< Decoded data stream
  size_t                m_dataSize;     ///< Length of encoded data stream
  wxArrayInt            m_stringTable[WXPDF_LZW_STRINGTABLE_SIZE]; ///< Table of decoded strings

  int                   m_tableIndex;   ///< Current string table index
  int                   m_bitsToGet;    ///< Number of bits to get from stream
  int                   m_bytePointer;  ///< Offset in encoded data stream
  int                   m_bitPointer;   ///< Bit offset
  int                   m_nextData;     ///< Next data item
  int                   m_nextBits;     ///< Next bits

  static int            ms_andTable[4]; ///< Static array of string table offsets
};

#endif
