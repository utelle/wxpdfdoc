///////////////////////////////////////////////////////////////////////////////
// Name:        pdfobjects.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-10-12
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfobjects.h Interfaces of the wxPdfObject classes

#ifndef _PDFOBJECTS_H_
#define _PDFOBJECTS_H_

// wxWidgets headers
#include <wx/dynarray.h>

#include "wx/pdfdocdef.h"
#include "wx/pdfdoc.h"

#define OBJTYPE_NULL         1
#define OBJTYPE_BOOLEAN      2
#define OBJTYPE_NUMBER       3
#define OBJTYPE_STRING       4
#define OBJTYPE_NAME         5
#define OBJTYPE_ARRAY        6
#define OBJTYPE_DICTIONARY   7
#define OBJTYPE_STREAM       8
#define OBJTYPE_INDIRECT     9

/// Class representing a base PDF object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfObject
{
public:
  /// Constructor
  wxPdfObject(int type);

  /// Destructor
  virtual ~wxPdfObject();

  /// Get the type of the object
  int GetType() { return m_type; }

  /// Set object and generation number
  void SetObjNum(int objNum, int objGen = 0);

  /// Get object number
  int GetNumber() { return m_objNum; }

  /// Get generation number
  int GetGeneration() { return m_objGen; }

  /// Flag this object as created through a indirect reference
  void SetIndirect(bool indirect) { m_indirect = indirect; }

  /// Check whether this object was created through a indirect reference
  bool IsIndirect() { return m_indirect; }

protected:
  int  m_type;     ///< Object type
  int  m_objNum;   ///< Object number
  int  m_objGen;   ///< Object generation
  bool m_indirect; ///< Flag whether created through indirect reference
};

/// Class representing a null object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfNull : public wxPdfObject
{
public:
  /// Constructor
  wxPdfNull();

  /// Destructor
  virtual ~wxPdfNull();
};

/// Class representing an indirect reference object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfIndirectReference : public wxPdfObject
{
public:
  /// Constructor
  wxPdfIndirectReference(int number, int generation = 0);

  /// Destructor
  virtual ~wxPdfIndirectReference();
};

/// Class representing a literal object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfLiteral : public wxPdfObject
{
public:
  /// Constructor
  wxPdfLiteral(int type, const wxString& value);

  /// Destructor
  virtual ~wxPdfLiteral();

  /// Get string value of the literal
  wxString GetValue() { return m_value; };

private:
  wxString m_value; ///< Value of the literal
};

/// Class representing a boolean object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfBoolean : public wxPdfObject
{
public:
  /// Constructor
  wxPdfBoolean(bool value);

  /// Constructor
  virtual ~wxPdfBoolean();

  /// Get boolean value
  bool GetValue() { return m_value; };

  /// Get boolean value as string
  wxString GetAsString();

private:
  bool m_value;  ///< Boolean value
};

/// Class representing a normal string object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfString : public wxPdfObject
{
public:
  /// Constructor
  wxPdfString(const wxString& value);

  /// Destructor
  virtual ~wxPdfString();

  /// Get value of the string
  wxString GetValue() { return m_value; };

  /// Set hexadecimal string flag
  void SetIsHexString(bool isHexString) { m_isHexString = isHexString; }

  /// Check whether string is hexadecimal
  bool IsHexString() const { return m_isHexString; }

private:
  wxString m_value; ///< Value of the string
  bool m_isHexString; ///< Flag whether string is a hexadecimal string
};

/// Class representing a numeric object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfNumber : public wxPdfObject
{
public:
  /// Constructor (value as string)
  wxPdfNumber(const wxString& value);

  /// Constructor (value as integer)
  wxPdfNumber(int value);

  /// Constructor (value as floating point)
  wxPdfNumber(double value);

  /// Destructor
  virtual ~wxPdfNumber();

  /// Get value as floating point
  double GetValue() { return m_value; }

  /// Get value as integer
  int GetInt() { return (int) m_value; }

  /// Get value as string
  wxString GetAsString() { return m_string; }

private:
  double   m_value;   ///< Numeric value
  wxString m_string;  ///< String representation of numeric value
};

/// Class representing a name object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfName : public wxPdfObject
{
public:
  /// Default constructor
  wxPdfName();

  /// Constructor
  wxPdfName(const wxString& name);

  /// Destructor
  virtual ~wxPdfName();

  /// Get name
  wxString GetName() { return m_name; };

private:
  wxString m_name; ///< Name value
};

/// Class representing an array object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfArray : public wxPdfObject
{
public:
  /// Constructor
  wxPdfArray();

  /// Destructor
  virtual ~wxPdfArray();

  /// Append an object to the array
  void Add(wxPdfObject* obj);

  /// Append an integer value to the array
  void Add(int value);

  /// Append a floating point value to the array
  void Add(double value);

  /// Get the array element with the given index
  wxPdfObject* Get(size_t index);

  /// Get the size of the array
  size_t GetSize() { return m_array.GetCount(); }

private:
  wxArrayPtrVoid m_array; ///< Array of objects
};

/// Hash map class for dictionaries. (For internal use only)
WX_DECLARE_STRING_HASH_MAP(wxPdfObject*, wxPdfDictionaryMap);

/// Class representing a dictionary object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfDictionary : public wxPdfObject
{
public:
  /// Constructor
  wxPdfDictionary();

  /// Destructor
  virtual ~wxPdfDictionary();

  /// Add a (name,value) pair to the dictionary
  void Put(wxPdfName* key, wxPdfObject* value);

  /// Get the value identified by the given key
  wxPdfObject* Get(const wxString& key);

  /// Get the dictionary map
  wxPdfDictionaryMap* GetHashMap() { return m_hashMap; }

private:
  wxPdfDictionaryMap* m_hashMap; ///< Dictionary map
};

/// Class representing a stream object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfStream : public wxPdfObject
{
public:
  /// Default constructor
  wxPdfStream();

  /// Constructor
  wxPdfStream(off_t offset);

  /// Destructor
  virtual ~wxPdfStream();

  /// Get the offset of the stream data
  off_t GetOffset() { return m_offset; }

  /// Set the associated stream dictionary
  void SetDictionary(wxPdfDictionary* dictionary) { m_dictionary = dictionary; }

  /// Get the associated stream dictionary
  wxPdfDictionary* GetDictionary() { return m_dictionary; }

  /// Set the stream data buffer
  void SetBuffer(wxMemoryOutputStream* buffer) { m_buffer = buffer; }

  /// Get the stream data buffer
  wxMemoryOutputStream* GetBuffer() { return m_buffer; }

  /// Get a value identified by the given key from the associated dictionary
  wxPdfObject* Get(const wxString& key);

  /// Set flag whether an object stream has already read the object index
  void SetHasObjOffsets(bool hasObjOffsets) { m_hasObjOffsets = hasObjOffsets; }

  /// Get flag whether the offsets of objects in an object stream are available
  bool HasObjOffsets() { return m_hasObjOffsets; }
  
  /// Get a pointer to the object offsets array
  wxArrayInt* GetObjOffsets() { return &m_objOffsets; }

  /// Get the offset of an object in the object stream
  int GetObjOffset(int index) const;

private:
  off_t                 m_offset;        ///< Offset of the stream data
  wxPdfDictionary*      m_dictionary;    ///< Associated stream dictionary
  wxMemoryOutputStream* m_buffer;        ///< Stream data buffer
  bool                  m_hasObjOffsets; ///< Flag whether the stream is an object stream
  wxArrayInt            m_objOffsets;    ///< Object offsets in object stream
};

/// Class representing a queue of objects. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfObjectQueue
{
public:
  /// Constructor
  wxPdfObjectQueue(int originalId = 0, int actualObjectId = 0, wxPdfObject* object = NULL);

  /// Destructor
  virtual ~wxPdfObjectQueue() {}

  /// Get original object id
  int GetOriginalObjectId() const { return m_originalObjectId; }

  /// Get actual object id
  int GetActualObjectId() const { return m_actualObjectId; }

  /// Get associated object
  wxPdfObject* GetObject() const { return m_object; }

  /// Set associated object
  void SetObject(wxPdfObject* object) { m_object = object; }

  /// Get next queue entry
  wxPdfObjectQueue* GetNext() const { return m_next; }

  /// Set next queue entry
  void SetNext(wxPdfObjectQueue* next) { m_next = next; }

private:
  int               m_originalObjectId; ///< Original object number
  int               m_actualObjectId;   ///< Actual object number
  wxPdfObject*      m_object;           ///< Associated object
  wxPdfObjectQueue* m_next;             ///< Pointer to next queue entry
};

/// Hashmap class for object queue entries (For internal use only)
WX_DECLARE_HASH_MAP(long, wxPdfObjectQueue*, wxIntegerHash, wxIntegerEqual, wxPdfObjectMap);

/// Hashmap class for object queue entries (For internal use only)
WX_DECLARE_HASH_MAP(long, wxPdfObject*, wxIntegerHash, wxIntegerEqual, wxPdfObjStmMap);

#endif

