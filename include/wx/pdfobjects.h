///////////////////////////////////////////////////////////////////////////////
// Name:        pdfobjects.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2006-10-12
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfobjects.h Interfaces of the wxPdfObject classes

#ifndef _PDF_OBJECTS_H_
#define _PDF_OBJECTS_H_

// wxWidgets headers
#include <wx/dynarray.h>
#include <wx/mstream.h>
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

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
  /**
  * \param type The object type
  */
  wxPdfObject(int type);

  /// Destructor
  virtual ~wxPdfObject();

  /// Get the type of the object
  /**
  * \return The object type
  */
  int GetType() { return m_type; }

  /// Set object and generation number
  /**
  * \param objNum The object number
  * \param objGen The generation number
  */
  void SetObjNum(int objNum, int objGen = 0);

  /// Get object number
  /**
  * \return The object number
  */
  int GetNumber() { return m_objNum; }

  /// Get generation number
  /**
  * \return The generation number
  */
  int GetGeneration() { return m_objGen; }

  /// Set actual object id
  /**
  * \param actualId The actual object ID
  */
  void SetActualId(int actualId) { m_actualId = actualId; }

  // Get actual object id
  /**
  * \return The actual object ID
  */
  int GetActualId() { return m_actualId; }

  /// Flag this object as created through an indirect reference
  /**
  * \param indirect Flag whether the object is an indirect reference
  */
  void SetCreatedIndirect(bool indirect) { m_indirect = indirect; }

  /// Check whether this object was created through an indirect reference
  /**
  * \return @c true if the object was created through an indirect reference, @c false otherwise
  */
  bool IsCreatedIndirect() { return m_indirect; }

  /// Check whether this object can be in an object stream
  /**
  * \return @c true if the object can be in an object stream, @c false otherwise
  */
  bool CanBeInObjStm();

  /// Checks whether this is a wxPdfNull object.
  /**
  * \return @c true if this is a null object, @c false otherwise
  */
  bool IsNull() { return (m_type == OBJTYPE_NULL); }

  /// Checks whether this is a wxPdfBoolean object.
  /**
  * \return @c true if this is a boolean object, @c false otherwise
  */
  bool IsBoolean() { return (m_type == OBJTYPE_BOOLEAN); }

  /// Checks whether this is a wxPdfNumber object.
  /**
  * \return @c true if this is a number object, @c false otherwise
  */
  bool IsNumber() { return (m_type == OBJTYPE_NUMBER); }

  /// Checks whether this is a wxPdfString object.
  /**
  * \return @c true if this is a string object, @c false otherwise
  */
  bool IsString() { return (m_type == OBJTYPE_STRING); }

  /// Checks whether this is a wxPdfName object.
  /**
  * \return @c true if this is a name object, @c false otherwise
  */
  bool IsName() { return (m_type == OBJTYPE_NAME); }

  /// Checks whether this is a wxPdfArray object.
  /**
  * \return @c true if this is an array object, @c false otherwise
  */
  bool IsArray() { return (m_type == OBJTYPE_ARRAY); }

  /// Checks whether this is a wxPdfDictionary object.
  /**
  * \return @c true if this is a dictionary object, @c false otherwise
  */
  bool IsDictionary() { return (m_type == OBJTYPE_DICTIONARY); }

  /// Checks whether this PdfObject is of the type PdfStream.
  /**
  * \return @c true if this is a stream object, @c false otherwise
  */
  bool IsStream() { return (m_type == OBJTYPE_STREAM); }

  /// Checks if this is an indirect object.
  /**
  * \return @c true if this is an indirect object, @c false otherwise
  */
  bool IsIndirect() { return (m_type == OBJTYPE_INDIRECT); }

protected:
  int  m_type;     ///< Object type
  int  m_objNum;   ///< Object number
  int  m_objGen;   ///< Object generation
  int  m_actualId; ///< Actual object id
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
  /**
  * \param number The object number
  * \param generation The generation number
  */
  wxPdfIndirectReference(int number, int generation = 0);

  /// Destructor
  virtual ~wxPdfIndirectReference();
};

/// Class representing a literal object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfLiteral : public wxPdfObject
{
public:
  /// Constructor
  /**
  * \param type The object type
  * \param value The literal value
  */
  wxPdfLiteral(int type, const wxString& value);

  /// Destructor
  virtual ~wxPdfLiteral();

  /// Get string value of the literal
  /**
  * \return The literal value
  */
  wxString GetValue() { return m_value; };

private:
  wxString m_value; ///< Value of the literal
};

/// Class representing a boolean object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfBoolean : public wxPdfObject
{
public:
  /// Constructor
  /**
  * \param value The boolean value
  */
  wxPdfBoolean(bool value);

  /// Destructor
  virtual ~wxPdfBoolean();

  /// Get boolean value
  /**
  * \return The boolean value
  */
  bool GetValue() { return m_value; };

  /// Get boolean value as string
  /**
  * \return The boolean value as a string ("true" or "false")
  */
  wxString GetAsString();

private:
  bool m_value;  ///< Boolean value
};

/// Class representing a normal string object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfString : public wxPdfObject
{
public:
  /// Constructor
  /**
  * \param value The string value
  */
  wxPdfString(const wxString& value);

  /// Destructor
  virtual ~wxPdfString();

  /// Get value of the string
  /**
  * \return The string value
  */
  wxString GetValue() { return m_value; };

  /// Set hexadecimal string flag
  /**
  * \param isHexString Flag whether the string is hexadecimal
  */
  void SetIsHexString(bool isHexString) { m_isHexString = isHexString; }

  /// Check whether string is hexadecimal
  /**
  * \return @c true if the string is hexadecimal, @c false otherwise
  */
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
  /**
  * \param value The string representation of the numeric value
  */
  wxPdfNumber(const wxString& value);

  /// Constructor (value as integer)
  /**
  * \param value The integer value
  */
  wxPdfNumber(int value);

  /// Constructor (value as floating point)
  /**
  * \param value The floating point value
  */
  wxPdfNumber(double value);

  /// Destructor
  virtual ~wxPdfNumber();

  /// Get value as floating point
  /**
  * \return The numeric value as double
  */
  double GetValue() { return m_value; }

  /// Get value as integer
  /**
  * \return The numeric value as integer
  */
  int GetInt() { return (int) m_value; }

  /// Get value as string
  /**
  * \return The string representation of the numeric value
  */
  wxString GetAsString() { return m_string; }

  /// Check whether value is integer
  /**
  * \return @c true if the value is an integer, @c false otherwise
  */
  bool IsInt() { return m_isInt; }

private:
  double   m_value;   ///< Numeric value
  wxString m_string;  ///< String representation of numeric value
  bool     m_isInt;   ///< Flag whether value is integer
};

/// Class representing a name object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfName : public wxPdfObject
{
public:
  /// Default constructor
  wxPdfName();

  /// Constructor
  /**
  * \param name The name value
  */
  wxPdfName(const wxString& name);

  /// Destructor
  virtual ~wxPdfName();

  /// Get name
  /**
  * \return The name value
  */
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
  /**
  * \param obj The object to append
  */
  void Add(wxPdfObject* obj);

  /// Append an integer value to the array
  /**
  * \param value The integer value to append
  */
  void Add(int value);

  /// Append a floating point value to the array
  /**
  * \param value The floating point value to append
  */
  void Add(double value);

  /// Get the array element with the given index
  /**
  * \param index The element index
  * \return The object at the given index, or NULL if the index is out of range
  */
  wxPdfObject* Get(size_t index);

  /// Get the size of the array
  /**
  * \return The number of elements in the array
  */
  size_t GetSize() { return m_array.GetCount(); }

private:
  wxArrayPtrVoid m_array; ///< Array of objects
};

/// Hash map class for dictionaries. (For internal use only)
WX_DECLARE_STRING_HASH_MAP_WITH_DECL(wxPdfObject*, wxPdfDictionaryMap, class WXDLLIMPEXP_PDFDOC);

/// Class representing a dictionary object. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfDictionary : public wxPdfObject
{
public:
  /// Constructor
  wxPdfDictionary();

  /// Constructor
  /**
  * \param type The dictionary type
  */
  wxPdfDictionary(const wxString& type);

  /// Destructor
  virtual ~wxPdfDictionary();

  /// Add a (name,value) pair to the dictionary
  /**
  * \param key The key
  * \param value The value
  */
  void Put(wxPdfName* key, wxPdfObject* value);

  /// Add a (name,value) pair to the dictionary
  /**
  * \param key The key name
  * \param value The value
  */
  void Put(const wxString& key, wxPdfObject* value);

  /// Get the value identified by the given key
  /**
  * \param key The key name
  * \return The object associated with the key, or NULL if the key is not found
  */
  wxPdfObject* Get(const wxString& key);

  /// Get the dictionary map
  /**
  * \return The hash map containing the dictionary entries
  */
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
  /**
  * \param offset The offset of the stream data
  */
  wxPdfStream(off_t offset);

  /// Destructor
  virtual ~wxPdfStream();

  /// Get the offset of the stream data
  /**
  * \return The stream data offset
  */
  off_t GetOffset() { return m_offset; }

  /// Set the associated stream dictionary
  /**
  * \param dictionary The stream dictionary
  */
  void SetDictionary(wxPdfDictionary* dictionary) { m_dictionary = dictionary; }

  /// Get the associated stream dictionary
  /**
  * \return The stream dictionary
  */
  wxPdfDictionary* GetDictionary() { return m_dictionary; }

  /// Set the stream data buffer
  /**
  * \param buffer The memory output stream containing the data
  */
  void SetBuffer(wxMemoryOutputStream* buffer) { m_buffer = buffer; }

  /// Get the stream data buffer
  /**
  * \return The memory output stream containing the data
  */
  wxMemoryOutputStream* GetBuffer() { return m_buffer; }

  /// Get a value identified by the given key from the associated dictionary
  /**
  * \param key The key name
  * \return The object associated with the key in the stream dictionary
  */
  wxPdfObject* Get(const wxString& key);

  /// Set flag whether an object stream has already read the object index
  /**
  * \param hasObjOffsets Flag whether the object stream has object offsets
  */
  void SetHasObjOffsets(bool hasObjOffsets) { m_hasObjOffsets = hasObjOffsets; }

  /// Get flag whether the offsets of objects in an object stream are available
  /**
  * \return @c true if the offsets are available, @c false otherwise
  */
  bool HasObjOffsets() { return m_hasObjOffsets; }

  /// Get a pointer to the object offsets array
  /**
  * \return Pointer to the array of object offsets
  */
  wxArrayInt* GetObjOffsets() { return &m_objOffsets; }

  /// Get the offset of an object in the object stream
  /**
  * \param index The object index
  * \return The offset of the object, or -1 if the index is out of range
  */
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
  /**
  * \param originalId The original object number
  * \param actualObjectId The actual object number
  * \param object The associated object
  */
  wxPdfObjectQueue(int originalId = 0, int actualObjectId = 0, wxPdfObject* object = NULL);

  /// Destructor
  virtual ~wxPdfObjectQueue() {}

  /// Get original object id
  /**
  * \return The original object number
  */
  int GetOriginalObjectId() const { return m_originalObjectId; }

  /// Get actual object id
  /**
  * \return The actual object number
  */
  int GetActualObjectId() const { return m_actualObjectId; }

  /// Get associated object
  /**
  * \return The associated object
  */
  wxPdfObject* GetObject() const { return m_object; }

  /// Set associated object
  /**
  * \param object The associated object
  */
  void SetObject(wxPdfObject* object) { m_object = object; }

  /// Get next queue entry
  /**
  * \return The next queue entry, or NULL if this is the last entry
  */
  wxPdfObjectQueue* GetNext() const { return m_next; }

  /// Set next queue entry
  /**
  * \param next The next queue entry
  */
  void SetNext(wxPdfObjectQueue* next) { m_next = next; }

private:
  int               m_originalObjectId; ///< Original object number
  int               m_actualObjectId;   ///< Actual object number
  wxPdfObject*      m_object;           ///< Associated object
  wxPdfObjectQueue* m_next;             ///< Pointer to next queue entry
};

/// Hashmap class for object queue entries (For internal use only)
WX_DECLARE_HASH_MAP_WITH_DECL(long, wxPdfObjectQueue*, wxIntegerHash, wxIntegerEqual, wxPdfObjectMap, class WXDLLIMPEXP_PDFDOC);

/// Hashmap class for object queue entries (For internal use only)
WX_DECLARE_HASH_MAP_WITH_DECL(long, wxPdfObject*, wxIntegerHash, wxIntegerEqual, wxPdfObjStmMap, class WXDLLIMPEXP_PDFDOC);

#endif

