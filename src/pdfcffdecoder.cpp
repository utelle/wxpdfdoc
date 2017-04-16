///////////////////////////////////////////////////////////////////////////////
// Name:        pdfcffdecoder.cpp
// Purpose:     
// Author:      Ulrich Telle
// Created:     2008-08-01
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfcffdecoder.cpp Implementation of Type1 and Type2 CFF decoder class

/*
 * This Class decodes a Type1 or Type2 CFF string. The code is based on code and ideas from the iText project.
 */

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes

#include "wx/pdfarraytypes.h"
#include "wx/pdfcffindex.h"
#include "wx/pdfcffdecoder.h"

// --- Implementation of CFF Decoder for charstring types 1 and 2

// CFF Dict Operators
// If the high byte is 0 the command is encoded with a single byte.

const int BASEFONTNAME_OP = 0x0c16;
const int CHARSET_OP      = 0x000f;
const int CHARSTRINGS_OP  = 0x0011;
const int CIDCOUNT_OP     = 0x0c22;
const int COPYRIGHT_OP    = 0x0c00;
const int ENCODING_OP     = 0x0010;
const int FAMILYNAME_OP   = 0x0003;
const int FDARRAY_OP      = 0x0c24;
const int FDSELECT_OP     = 0x0c25;
const int FONTBBOX_OP     = 0x0005;
const int FONTNAME_OP     = 0x0c26;
const int FULLNAME_OP     = 0x0002;
const int LOCAL_SUB_OP    = 0x0013;
const int NOTICE_OP       = 0x0001;
const int POSTSCRIPT_OP   = 0x0c15;
const int PRIVATE_OP      = 0x0012;
const int ROS_OP          = 0x0c1e;
const int UNIQUEID_OP     = 0x000d;
const int VERSION_OP      = 0x0000;
const int WEIGHT_OP       = 0x0004;
const int XUID_OP         = 0x000e;

const int NUM_STD_STRINGS = 391;

const char SUBR_RETURN_OP = 11;

#if 0
static const wxChar* gs_standardStrings[] = {
    // Generated from Appendix A of the CFF specification. Size should be 391.
    wxS(".notdef"),             wxS("space"),              wxS("exclam"),           wxS("quotedbl"),       wxS("numbersign"),
    wxS("dollar"),              wxS("percent"),            wxS("ampersand"),        wxS("quoteright"),     wxS("parenleft"),
    wxS("parenright"),          wxS("asterisk"),           wxS("plus"),             wxS("comma"),          wxS("hyphen"), 
    wxS("period"),              wxS("slash"),              wxS("zero"),             wxS("one"),            wxS("two"),
    wxS("three"),               wxS("four"),               wxS("five"),             wxS("six"),            wxS("seven"),
    wxS("eight"),               wxS("nine"),               wxS("colon"),            wxS("semicolon"),      wxS("less"),
    wxS("equal"),               wxS("greater"),            wxS("question"),         wxS("at"),             wxS("A"),
    wxS("B"),                   wxS("C"),                  wxS("D"),                wxS("E"),              wxS("F"),
    wxS("G"),                   wxS("H"),                  wxS("I"),                wxS("J"),              wxS("K"),
    wxS("L"),                   wxS("M"),                  wxS("N"),                wxS("O"),              wxS("P"),
    wxS("Q"),                   wxS("R"),                  wxS("S"),                wxS("T"),              wxS("U"),
    wxS("V"),                   wxS("W"),                  wxS("X"),                wxS("Y"),              wxS("Z"),
    wxS("bracketleft"),         wxS("backslash"),          wxS("bracketright"),     wxS("asciicircum"),    wxS("underscore"),
    wxS("quoteleft"),           wxS("a"),                  wxS("b"),                wxS("c"),              wxS("d"),
    wxS("e"),                   wxS("f"),                  wxS("g"),                wxS("h"),              wxS("i"),
    wxS("j"),                   wxS("k"),                  wxS("l"),                wxS("m"),              wxS("n"),
    wxS("o"),                   wxS("p"),                  wxS("q"),                wxS("r"),              wxS("s"),
    wxS("t"),                   wxS("u"),                  wxS("v"),                wxS("w"),              wxS("x"),
    wxS("y"),                   wxS("z"),                  wxS("braceleft"),        wxS("bar"),            wxS("braceright"),
    wxS("asciitilde"),          wxS("exclamdown"),         wxS("cent"),             wxS("sterling"),       wxS("fraction"),
    wxS("yen"),                 wxS("florin"),             wxS("section"),          wxS("currency"),       wxS("quotesingle"),
    wxS("quotedblleft"),        wxS("guillemotleft"),      wxS("guilsinglleft"),    wxS("guilsinglright"), wxS("fi"),
    wxS("fl"),                  wxS("endash"),             wxS("dagger"),           wxS("daggerdbl"),      wxS("periodcentered"),
    wxS("paragraph"),           wxS("bullet"),             wxS("quotesinglbase"),   wxS("quotedblbase"),   wxS("quotedblright"),
    wxS("guillemotright"),      wxS("ellipsis"),           wxS("perthousand"),      wxS("questiondown"),   wxS("grave"),
    wxS("acute"),               wxS("circumflex"),         wxS("tilde"),            wxS("macron"),         wxS("breve"),
    wxS("dotaccent"),           wxS("dieresis"),           wxS("ring"),             wxS("cedilla"),        wxS("hungarumlaut"),
    wxS("ogonek"),              wxS("caron"),              wxS("emdash"),           wxS("AE"),             wxS("ordfeminine"),
    wxS("Lslash"),              wxS("Oslash"),             wxS("OE"),               wxS("ordmasculine"),   wxS("ae"),
    wxS("dotlessi"),            wxS("lslash"),             wxS("oslash"),           wxS("oe"),             wxS("germandbls"),
    wxS("onesuperior"),         wxS("logicalnot"),         wxS("mu"),               wxS("trademark"),      wxS("Eth"),
    wxS("onehalf"),             wxS("plusminus"),          wxS("Thorn"),            wxS("onequarter"),     wxS("divide"),
    wxS("brokenbar"),           wxS("degree"),             wxS("thorn"),            wxS("threequarters"),  wxS("twosuperior"),
    wxS("registered"),          wxS("minus"),              wxS("eth"),              wxS("multiply"),       wxS("threesuperior"),
    wxS("copyright"),           wxS("Aacute"),             wxS("Acircumflex"),      wxS("Adieresis"),      wxS("Agrave"),
    wxS("Aring"),               wxS("Atilde"),             wxS("Ccedilla"),         wxS("Eacute"),         wxS("Ecircumflex"),
    wxS("Edieresis"),           wxS("Egrave"),             wxS("Iacute"),           wxS("Icircumflex"),    wxS("Idieresis"),
    wxS("Igrave"),              wxS("Ntilde"),             wxS("Oacute"),           wxS("Ocircumflex"),    wxS("Odieresis"),
    wxS("Ograve"),              wxS("Otilde"),             wxS("Scaron"),           wxS("Uacute"),         wxS("Ucircumflex"),
    wxS("Udieresis"),           wxS("Ugrave"),             wxS("Yacute"),           wxS("Ydieresis"),      wxS("Zcaron"),
    wxS("aacute"),              wxS("acircumflex"),        wxS("adieresis"),        wxS("agrave"),         wxS("aring"),
    wxS("atilde"),              wxS("ccedilla"),           wxS("eacute"),           wxS("ecircumflex"),    wxS("edieresis"),
    wxS("egrave"),              wxS("iacute"),             wxS("icircumflex"),      wxS("idieresis"),      wxS("igrave"),
    wxS("ntilde"),              wxS("oacute"),             wxS("ocircumflex"),      wxS("odieresis"),      wxS("ograve"),
    wxS("otilde"),              wxS("scaron"),             wxS("uacute"),           wxS("ucircumflex"),    wxS("udieresis"),
    wxS("ugrave"),              wxS("yacute"),             wxS("ydieresis"),        wxS("zcaron"),         wxS("exclamsmall"),
    wxS("Hungarumlautsmall"),   wxS("dollaroldstyle"),     wxS("dollarsuperior"),   wxS("ampersandsmall"), wxS("Acutesmall"),
    wxS("parenleftsuperior"),   wxS("parenrightsuperior"), wxS("twodotenleader"),   wxS("onedotenleader"), wxS("zerooldstyle"),
    wxS("oneoldstyle"),         wxS("twooldstyle"),        wxS("threeoldstyle"),    wxS("fouroldstyle"),   wxS("fiveoldstyle"),
    wxS("sixoldstyle"),         wxS("sevenoldstyle"),      wxS("eightoldstyle"),    wxS("nineoldstyle"),   wxS("commasuperior"),
    wxS("threequartersemdash"), wxS("periodsuperior"),     wxS("questionsmall"),    wxS("asuperior"),      wxS("bsuperior"),
    wxS("centsuperior"),        wxS("dsuperior"),          wxS("esuperior"),        wxS("isuperior"),      wxS("lsuperior"),
    wxS("msuperior"),           wxS("nsuperior"),          wxS("osuperior"),        wxS("rsuperior"),      wxS("ssuperior"),
    wxS("tsuperior"),           wxS("ff"),                 wxS("ffi"),              wxS("ffl"),            wxS("parenleftinferior"),
    wxS("parenrightinferior"),  wxS("Circumflexsmall"),    wxS("hyphensuperior"),   wxS("Gravesmall"),     wxS("Asmall"),
    wxS("Bsmall"),              wxS("Csmall"),             wxS("Dsmall"),           wxS("Esmall"),         wxS("Fsmall"),
    wxS("Gsmall"),              wxS("Hsmall"),             wxS("Ismall"),           wxS("Jsmall"),         wxS("Ksmall"),
    wxS("Lsmall"),              wxS("Msmall"),             wxS("Nsmall"),           wxS("Osmall"),         wxS("Psmall"),
    wxS("Qsmall"),              wxS("Rsmall"),             wxS("Ssmall"),           wxS("Tsmall"),         wxS("Usmall"),
    wxS("Vsmall"),              wxS("Wsmall"),             wxS("Xsmall"),           wxS("Ysmall"),         wxS("Zsmall"),
    wxS("colonmonetary"),       wxS("onefitted"),          wxS("rupiah"),           wxS("Tildesmall"),     wxS("exclamdownsmall"),
    wxS("centoldstyle"),        wxS("Lslashsmall"),        wxS("Scaronsmall"),      wxS("Zcaronsmall"),    wxS("Dieresissmall"),
    wxS("Brevesmall"),          wxS("Caronsmall"),         wxS("Dotaccentsmall"),   wxS("Macronsmall"),    wxS("figuredash"),
    wxS("hypheninferior"),      wxS("Ogoneksmall"),        wxS("Ringsmall"),        wxS("Cedillasmall"),   wxS("questiondownsmall"),
    wxS("oneeighth"),           wxS("threeeighths"),       wxS("fiveeighths"),      wxS("seveneighths"),   wxS("onethird"),
    wxS("twothirds"),           wxS("zerosuperior"),       wxS("foursuperior"),     wxS("fivesuperior"),   wxS("sixsuperior"),
    wxS("sevensuperior"),       wxS("eightsuperior"),      wxS("ninesuperior"),     wxS("zeroinferior"),   wxS("oneinferior"),
    wxS("twoinferior"),         wxS("threeinferior"),      wxS("fourinferior"),     wxS("fiveinferior"),   wxS("sixinferior"),
    wxS("seveninferior"),       wxS("eightinferior"),      wxS("nineinferior"),     wxS("centinferior"),   wxS("dollarinferior"),
    wxS("periodinferior"),      wxS("commainferior"),      wxS("Agravesmall"),      wxS("Aacutesmall"),    wxS("Acircumflexsmall"),
    wxS("Atildesmall"),         wxS("Adieresissmall"),     wxS("Aringsmall"),       wxS("AEsmall"),        wxS("Ccedillasmall"),
    wxS("Egravesmall"),         wxS("Eacutesmall"),        wxS("Ecircumflexsmall"), wxS("Edieresissmall"), wxS("Igravesmall"),
    wxS("Iacutesmall"),         wxS("Icircumflexsmall"),   wxS("Idieresissmall"),   wxS("Ethsmall"),       wxS("Ntildesmall"),
    wxS("Ogravesmall"),         wxS("Oacutesmall"),        wxS("Ocircumflexsmall"), wxS("Otildesmall"),    wxS("Odieresissmall"),
    wxS("OEsmall"),             wxS("Oslashsmall"),        wxS("Ugravesmall"),      wxS("Uacutesmall"),    wxS("Ucircumflexsmall"),
    wxS("Udieresissmall"),      wxS("Yacutesmall"),        wxS("Thornsmall"),       wxS("Ydieresissmall"), wxS("001.000"),
    wxS("001.001"),             wxS("001.002"),            wxS("001.003"),          wxS("Black"),          wxS("Bold"),
    wxS("Book"),                wxS("Light"),              wxS("Medium"),           wxS("Regular"),        wxS("Roman"),
    wxS("Semibold")
  };
static int gs_standardStringsCount = sizeof(gs_standardStrings) / sizeof(wxChar*);
#endif

// The Strings in this array represent Type1/Type2 operator names
static const wxChar* gs_subrsFunctions[] = {
    wxS("RESERVED_0"),  wxS("hstem"),       wxS("RESERVED_2"),  wxS("vstem"),          wxS("vmoveto"),
    wxS("rlineto"),     wxS("hlineto"),     wxS("vlineto"),     wxS("rrcurveto"),      wxS("RESERVED_9"),
    wxS("callsubr"),    wxS("return"),      wxS("escape"),      wxS("hsbw"),/*RES_13*/ wxS("endchar"),
    wxS("RESERVED_15"), wxS("RESERVED_16"), wxS("RESERVED_17"), wxS("hstemhm"),        wxS("hintmask"),
    wxS("cntrmask"),    wxS("rmoveto"),     wxS("hmoveto"),     wxS("vstemhm"),        wxS("rcurveline"),
    wxS("rlinecurve"),  wxS("vvcurveto"),   wxS("hhcurveto"),   wxS("shortint"),       wxS("callgsubr"),
    wxS("vhcurveto"),   wxS("hvcurveto")
  };
#if 0
static int gs_subrsFunctionsCount = sizeof(gs_subrsFunctions) / sizeof(wxChar*);
#endif

// The Strings in this array represent Type1/Type2 escape operator names
static const wxChar* gs_subrsEscapeFuncs[] = {
    wxS("RESERVED_0"),  wxS("RESERVED_1"),    wxS("RESERVED_2"),   wxS("and"),           wxS("or"), 
    wxS("not"),         wxS("seac"),/*RES_6*/ wxS("sbw"),/*RES_7*/ wxS("RESERVED_8"),    wxS("abs"),
    wxS("add"),         wxS("sub"),           wxS("div"),          wxS("RESERVED_13"),   wxS("neg"),
    wxS("eq"),          wxS("RESERVED_16"),   wxS("RESERVED_17"),  wxS("drop"),          wxS("RESERVED_19"),
    wxS("put"),         wxS("get"),           wxS("ifelse"),       wxS("random"),        wxS("mul"),
    wxS("RESERVED_25"), wxS("sqrt"),          wxS("dup"),          wxS("exch"),          wxS("index"), 
    wxS("roll"),        wxS("RESERVED_31"),   wxS("RESERVED_32"),  wxS("RESERVED_33"),   wxS("hflex"),
    wxS("flex"),        wxS("hflex1"),        wxS("flex1"),        wxS("RESERVED_REST")
  };
static int gs_subrsEscapeFuncsCount = sizeof(gs_subrsEscapeFuncs) / sizeof(wxChar*);

#if 0
static wxChar* gs_operatorNames[] = {
    wxS("version"),           wxS("Notice"),             wxS("FullName"),      wxS("FamilyName"),     wxS("Weight"),
    wxS("FontBBox"),          wxS("BlueValues"),         wxS("OtherBlues"),    wxS("FamilyBlues"),    wxS("FamilyOtherBlues"),
    wxS("StdHW"),             wxS("StdVW"),              wxS("UNKNOWN_12"),    wxS("UniqueID"),       wxS("XUID"),
    wxS("charset"),           wxS("Encoding"),           wxS("CharStrings"),   wxS("Private"),        wxS("Subrs"),
    wxS("defaultWidthX"),     wxS("nominalWidthX"),      wxS("UNKNOWN_22"),    wxS("UNKNOWN_23"),     wxS("UNKNOWN_24"),
    wxS("UNKNOWN_25"),        wxS("UNKNOWN_26"),         wxS("UNKNOWN_27"),    wxS("UNKNOWN_28"),     wxS("UNKNOWN_29"),
    wxS("UNKNOWN_30"),        wxS("UNKNOWN_31"),         wxS("Copyright"),     wxS("isFixedPitch"),   wxS("ItalicAngle"),
    wxS("UnderlinePosition"), wxS("UnderlineThickness"), wxS("PaintType"),     wxS("CharstringType"), wxS("FontMatrix"),
    wxS("StrokeWidth"),       wxS("BlueScale"),          wxS("BlueShift"),     wxS("BlueFuzz"),       wxS("StemSnapH"),
    wxS("StemSnapV"),         wxS("ForceBold"),          wxS("UNKNOWN_12_15"), wxS("UNKNOWN_12_16"),  wxS("LanguageGroup"),
    wxS("ExpansionFactor"),   wxS("initialRandomSeed"),  wxS("SyntheticBase"), wxS("PostScript"),     wxS("BaseFontName"),
    wxS("BaseFontBlend"),     wxS("UNKNOWN_12_24"),      wxS("UNKNOWN_12_25"), wxS("UNKNOWN_12_26"),  wxS("UNKNOWN_12_27"),
    wxS("UNKNOWN_12_28"),     wxS("UNKNOWN_12_29"),      wxS("ROS"),           wxS("CIDFontVersion"), wxS("CIDFontRevision"),
    wxS("CIDFontType"),       wxS("CIDCount"),           wxS("UIDBase"),       wxS("FDArray"),        wxS("FDSelect"),
    wxS("FontName")
  };
static int gs_operatorNamesCount = sizeof(gs_operatorNames) / sizeof(wxChar*);
#endif

class wxPdfCffFontObject
{
public:
  wxPdfCffFontObject() {}
  int      m_type;
  int      m_intValue;
  wxString m_strValue;
};

wxPdfCffDecoder::wxPdfCffDecoder()
{
  m_charstringType = 1;
  
  m_globalSubrIndex  = NULL;
  m_hGlobalSubrsUsed = NULL;
  m_lGlobalSubrsUsed = NULL;

  m_args = new wxPdfCffFontObject[48];
  m_argCount = 0;
}

wxPdfCffDecoder::wxPdfCffDecoder(wxPdfCffIndexArray* globalSubrIndex,
                                 wxPdfSortedArrayInt* hGlobalSubrsUsed,
                                 wxArrayInt* lGlobalSubrsUsed)
{
  m_charstringType = 2;
  
  m_globalSubrIndex  = globalSubrIndex;
  m_hGlobalSubrsUsed = hGlobalSubrsUsed;
  m_lGlobalSubrsUsed = lGlobalSubrsUsed;

  m_args = new wxPdfCffFontObject[48];
  m_argCount = 0;
}

wxPdfCffDecoder::~wxPdfCffDecoder()
{
  delete [] m_args;
}

// --- Read original CFF stream

unsigned char
wxPdfCffDecoder::ReadByte(wxInputStream* stream)
{
  unsigned char card8;
  stream->Read(&card8, 1);
  return card8;
}

short
wxPdfCffDecoder::ReadShort(wxInputStream* stream)
{
  // Read a 2-byte integer from file (big endian)
  short i16;
  stream->Read(&i16, 2);
  return wxINT16_SWAP_ON_LE(i16);
}

int
wxPdfCffDecoder::ReadInt(wxInputStream* stream)
{
  // Read a 4-byte integer from file (big endian)
  int i32;
  stream->Read(&i32, 4);
  return wxINT32_SWAP_ON_LE(i32);
}

// -- Subset global and local subroutines

int
wxPdfCffDecoder::CalcBias(int nSubrs)
{
  int bias;
  // If type == 1 then bias = 0, else calc according to the count
  if (m_charstringType == 1)
  {
    bias = 0;
  }
  else if (nSubrs < 1240)
  {
    bias = 107;
  }
  else if (nSubrs < 33900)
  {
    bias = 1131;
  }
  else
  {
    bias = 32768;
  }
  return bias;
}

bool
wxPdfCffDecoder::GetCharWidthAndComposite(wxPdfCffIndexElement& charstring, int& width, bool& isComposite, int& bchar, int& achar)
{
  bool ok = false;
  width = -1;
  isComposite = false;
  bchar = -1;
  achar = -1;

  wxInputStream* stream = charstring.GetBuffer();
  int begin = charstring.GetOffset();
  int end   = begin + charstring.GetLength();

  // Clear the stack
  EmptyStack();
  m_numHints = 0;

  stream->SeekI(begin);
  ReadCommand(stream);
  wxPdfCffFontObject* element = NULL;
  int numArgs = m_argCount;
  HandleStack();
  if (m_key == wxS("hsbw"))
  {
    if (numArgs == 2)
    {
      ok = true;
      element = &m_args[1]; // 2nd argument is width
      width = element->m_intValue;
    }
  }
  else if (m_key == wxS("sbw"))
  {
    if (numArgs == 4)
    {
      ok = true;
      element = &m_args[2]; // 3rd argument is width
      width = element->m_intValue;
    }
  }
  if (ok && (stream->TellI() < end))
  {
    ReadCommand(stream);
    numArgs = m_argCount;
    // Check the modification needed on the Argument Stack according to key;
    HandleStack();
    if (m_key == wxS("seac"))
    {
      if (numArgs == 5)
      {
        isComposite = true;
        // third argument
        element = &m_args[3];
        bchar = element->m_intValue;
        element = &m_args[4];
        achar = element->m_intValue;
      }
    }
  }
  return ok;
}

void
wxPdfCffDecoder::ReadASubr(wxInputStream* stream, int begin, int end,
                           int globalBias, int localBias, 
                           wxPdfSortedArrayInt& hSubrsUsed, wxArrayInt& lSubrsUsed,
                           wxPdfCffIndexArray& localSubrIndex)
{
  int beginSubr, endSubr;
#if 0
  wxLogDebug(wxS("ReadAsubr %d %d %d %d"), begin, end, globalBias, localBias);
#endif
  // Clear the stack for the subrs
  EmptyStack();
  m_numHints = 0;
  // Goto begining of the subr
  stream->SeekI(begin);
  while (stream->TellI() < end)
  {
    // Read the next command
    ReadCommand(stream);
    int pos = stream->TellI();
    wxPdfCffFontObject* topElement = NULL;
    if (m_argCount > 0)
    {
      topElement = &m_args[m_argCount-1];
    }
    int numArgs = m_argCount;
    // Check the modification needed on the Argument Stack according to key;
    HandleStack();
    // a call to a Lsubr
    if (m_key == wxS("callsubr")) 
    {
      // Verify that arguments are passed 
      if (numArgs > 0)
      {
        // Calc the index of the Subrs
        int subr = topElement->m_intValue + localBias;
        // If the subr isn't in the HashMap -> Put in
        if (hSubrsUsed.Index(subr) == wxNOT_FOUND)
        {
#if 0
          wxLogDebug(wxS("Add hSubr: %s %d"), m_key.c_str(), subr);
#endif
          hSubrsUsed.Add(subr);
          lSubrsUsed.Add(subr);
        }
        wxPdfCffIndexElement& localSubr = localSubrIndex[subr];
        beginSubr = localSubr.GetOffset();
        endSubr = beginSubr + localSubr.GetLength();
        CalcHints(localSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }            
    }
    // a call to a Gsubr
    else if (m_key == wxS("callgsubr"))
    {
      // Verify that arguments are passed 
      if (numArgs > 0)
      {
        // Calc the index of the Subrs
        int subr = topElement->m_intValue + globalBias;
        // If the subr isn't in the HashMap -> Put in
        if (m_hGlobalSubrsUsed->Index(subr) == wxNOT_FOUND)
        {
#if 0
          wxLogDebug(wxS("Add hGSubr: %s %d"), m_key.c_str(), subr);
#endif
          m_hGlobalSubrsUsed->Add(subr);
          m_lGlobalSubrsUsed->Add(subr);
        }
        wxPdfCffIndexElement& globalSubr = (*m_globalSubrIndex)[subr];
        beginSubr = globalSubr.GetOffset();
        endSubr = beginSubr + globalSubr.GetLength();
        CalcHints(globalSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }
    }
    // A call to "stem"
    else if (m_key == wxS("hstem") || m_key == wxS("vstem") || m_key == wxS("hstemhm") || m_key == wxS("vstemhm"))
    {
      // Increment the NumOfHints by the number couples of of arguments
      m_numHints += numArgs / 2;
    }
          // A call to "mask"
    else if (m_key == wxS("hintmask") || m_key == wxS("cntrmask"))
    {
      // Compute the size of the mask
      int sizeOfMask = m_numHints / 8;
      if (m_numHints % 8 != 0 || sizeOfMask == 0)
      {
        sizeOfMask++;
      }
      // Continue the pointer in SizeOfMask steps
      int i;
      for (i = 0; i < sizeOfMask; i++)
      {
        ReadByte(stream);
      }
    }
  }
#if 0
  wxLogDebug(wxS("ReadASubr end"));
#endif
}

void
wxPdfCffDecoder::HandleStack()
{
  // Findout what the operator does to the stack
  int stackHandle = StackOpp();
  if (stackHandle < 2)
  {
    // The operators that enlarge the stack by one
    if (stackHandle == 1)
    {
      PushStack();
    }
    // The operators that pop the stack
    else
    {
      // Abs value for the for loop
      stackHandle *= -1;
      int i;
      for (i = 0; i < stackHandle; i++)
      {
        PopStack();
      }
    }
  }
  // All other flush the stack
  else
  {
    EmptyStack();
  }
}
  
int
wxPdfCffDecoder::StackOpp()
{
  int op;
  if (m_key == wxS("ifelse"))
  {
    op = -3;
  }
  else if (m_key == wxS("roll") || m_key == wxS("put"))
  {
    op = -2;
  }
  else if (m_key == wxS("callsubr") || m_key == wxS("callgsubr") || m_key == wxS("add")  ||
           m_key == wxS("sub")      || m_key == wxS("div")       || m_key == wxS("mul")  ||
           m_key == wxS("drop")     || m_key == wxS("and")       || m_key == wxS("or")   ||
           m_key == wxS("eq"))
  {
    op = -1;
  }
  else if (m_key == wxS("abs")  || m_key == wxS("neg")   || m_key == wxS("sqrt") ||
           m_key == wxS("exch") || m_key == wxS("index") || m_key == wxS("get")  ||
           m_key == wxS("not")  || m_key == wxS("return"))
  {
    op = 0;
  }
  else if (m_key == wxS("random") || m_key == wxS("dup"))
  {
    op = 1;
  }
  else
  {
    op = 2;
  }
  return op;
}
  
void
wxPdfCffDecoder::EmptyStack()
{
  m_argCount = 0;    
}
  
void
wxPdfCffDecoder::PopStack()
{
  if (m_argCount > 0)
  {
    m_argCount--;
  }
}
  
void
wxPdfCffDecoder::PushStack()
{
  m_argCount++;
}
  
void
wxPdfCffDecoder::ReadCommand(wxInputStream* stream)
{
  m_key = wxEmptyString;
  bool gotKey = false;
  // Until a key is found
  while (!gotKey)
  {
    // Read the first Char
    unsigned char b0 = ReadByte(stream);
    // decode according to the type1/type2 format
    if (b0 == 28) // the two next bytes represent a short int;
    {
      int first = ReadByte(stream) & 0xff;
      int second = ReadByte(stream) & 0xff;
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = first << 8 | second;
      m_argCount++;
      continue;
    }
    if (b0 >= 32 && b0 <= 246) // The byte read is the byte;
    {
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = (int) (b0 - 139);
      m_argCount++;
      continue;
    }
    if (b0 >= 247 && b0 <= 250) // The byte read and the next byte constetute a short int
    {
      unsigned char b1 = ReadByte(stream);
      short item = (short) ((b0-247)*256 + b1 + 108);
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = item;
      m_argCount++;
      continue;
    }
    if (b0 >= 251 && b0 <= 254)// Same as above except negative
    {
      unsigned char b1 = ReadByte(stream);
      short item = (short) (-(b0-251)*256-b1-108);
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = item;
      m_argCount++;
      continue;
    }
    if (b0 == 255)// The next for bytes represent a double.
    {
      int item = ReadInt(stream);
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = item;
      m_argCount++;
      continue;
    }
    if (b0 <= 31 && b0 != 28) // An operator was found.. Set Key.
    {
      gotKey = true;
      // 12 is an escape command therefor the next byte is a part
      // of this command
      if (b0 == 12)
      {
        unsigned char b1 = ReadByte(stream);
        if (b1 > gs_subrsEscapeFuncsCount-1)
        {
          b1 = gs_subrsEscapeFuncsCount-1;
        }
        m_key = gs_subrsEscapeFuncs[b1];
      }
      else
      {
        m_key = gs_subrsFunctions[b0];
      }
      continue;
    }
  }
}
  
int
wxPdfCffDecoder::CalcHints(wxInputStream* stream, int begin, int end, int globalBias, int localBias, wxPdfCffIndexArray& localSubrIndex)
{
  int beginSubr, endSubr;
  // Goto begining of the subr
  stream->SeekI(begin);
  while (stream->TellI() < end)
  {
    // Read the next command
    ReadCommand(stream);
    int pos = stream->TellI();
    wxPdfCffFontObject* topElement = NULL;
    if (m_argCount > 0)
    {
      topElement = &m_args[m_argCount-1];
    }
    int numArgs = m_argCount;
    //Check the modification needed on the Argument Stack according to key;
    HandleStack();
    // a call to a Lsubr
    if (m_key == wxS("callsubr")) 
    {
      if (numArgs > 0)
      {
        int subr = topElement->m_intValue + localBias;
        wxPdfCffIndexElement& localSubr = localSubrIndex[subr];
        beginSubr = localSubr.GetOffset();
        endSubr = beginSubr + localSubr.GetLength();
        CalcHints(localSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }
    }
    // a call to a Gsubr
    else if (m_key == wxS("callgsubr"))
    {
      if (numArgs > 0)
      {
        int subr = topElement->m_intValue + globalBias;
        wxPdfCffIndexElement& globalSubr = (*m_globalSubrIndex)[subr];
        beginSubr = globalSubr.GetOffset();
        endSubr = beginSubr + globalSubr.GetLength();
        CalcHints(globalSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }
    }
    // A call to "stem"
    else if (m_key == wxS("hstem") || m_key == wxS("vstem") || m_key == wxS("hstemhm") || m_key == wxS("vstemhm"))
    {
      // Increment the NumOfHints by the number couples of of arguments
      m_numHints += numArgs / 2;
    }
    // A call to "mask"
    else if (m_key == wxS("hintmask") || m_key == wxS("cntrmask"))
    {
      // Compute the size of the mask
      int sizeOfMask = m_numHints / 8;
      if (m_numHints % 8 != 0 || sizeOfMask == 0)
      {
        sizeOfMask++;
      }
      // Continue the pointer in SizeOfMask steps
      int i;
      for (i = 0; i < sizeOfMask; i++)
      {
        ReadByte(stream);
      }
    }
  }
  return m_numHints;
}
