/*
** Name:        pdfbarcodezint.h
** Purpose:     Barcode support for wxPdfDocument based on Zint
** Author:      Ulrich Telle
** Created:     2024-04-08
** Copyright:   (c) 2024-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

#ifndef PDF_BARCODE_ZINT_H
#define PDF_BARCODE_ZINT_H

#include <wx/object.h>
#include <wx/colour.h>
#include <wx/dc.h>

namespace wxPdfBarcode
{
  /// Symbologies as defined in the Zint library
  enum class Symbology : int
  {
    SYM_UNDEFINED       = 0, ///< Undefined symbology
    // Tbarcode 7 codes
    SYM_CODE11          =   1, ///< Code 11
    SYM_C25STANDARD     =   2, ///< 2 of 5 Standard (Matrix)
    SYM_C25MATRIX       =   2, ///< Legacy, same as SYM_C25STANDARD
    SYM_C25INTER        =   3, ///< 2 of 5 Interleaved
    SYM_C25IATA         =   4, ///< 2 of 5 IATA
    SYM_C25LOGIC        =   6, ///< 2 of 5 Data Logic
    SYM_C25IND          =   7, ///< 2 of 5 Industrial
    SYM_CODE39          =   8, ///< Code 39
    SYM_EXCODE39        =   9, ///< Extended Code 39
    SYM_EANX            =  13, ///< EAN (European Article Number)
    SYM_EANX_CHK        =  14, ///< EAN + Check Digit
    SYM_GS1_128         =  16, ///< GS1-128
    SYM_EAN128          =  16, ///< Legacy
    SYM_CODABAR         =  18, ///< Codabar
    SYM_CODE128         =  20, ///< Code 128
    SYM_DPLEIT          =  21, ///< Deutsche Post Leitcode
    SYM_DPIDENT         =  22, ///< Deutsche Post Identcode
    SYM_CODE16K         =  23, ///< Code 16k
    SYM_CODE49          =  24, ///< Code 49
    SYM_CODE93          =  25, ///< Code 93
    SYM_FLAT            =  28, ///< Flattermarken
    SYM_DBAR_OMN        =  29, ///< GS1 DataBar Omnidirectional
    SYM_RSS14           =  29, ///< Legacy, same as SYM_DBAR_OMN
    SYM_DBAR_LTD        =  30, ///< GS1 DataBar Limited
    SYM_RSS_LTD         =  30, ///< Legacy, same as SYM_DBAR_LTD
    SYM_DBAR_EXP        =  31, ///< GS1 DataBar Expanded
    SYM_RSS_EXP         =  31, ///< Legacy, same as SYM_DBAR_EXP
    SYM_TELEPEN         =  32, ///< Telepen Alpha
    SYM_UPCA            =  34, ///< UPC-A
    SYM_UPCA_CHK        =  35, ///< UPC-A + Check Digit
    SYM_UPCE            =  37, ///< UPC-E
    SYM_UPCE_CHK        =  38, ///< UPC-E + Check Digit
    SYM_POSTNET         =  40, ///< USPS (U.S. Postal Service) POSTNET
    SYM_MSI_PLESSEY     =  47, ///< MSI Plessey
    SYM_FIM             =  49, ///< Facing Identification Mark
    SYM_LOGMARS         =  50, ///< LOGMARS
    SYM_PHARMA          =  51, ///< Pharmacode One-Track
    SYM_PZN             =  52, ///< Pharmazentralnummer
    SYM_PHARMA_TWO      =  53, ///< Pharmacode Two-Track
    SYM_CEPNET          =  54, ///< Brazilian CEPNet Postal Code
    SYM_PDF417          =  55, ///< PDF417
    SYM_PDF417COMP      =  56, ///< Compact PDF417 (Truncated PDF417)
    SYM_PDF417TRUNC     =  56, ///< Legacy, same as SYM_PDF417COMP
    SYM_MAXICODE        =  57, ///< MaxiCode
    SYM_QRCODE          =  58, ///< QR Code
    SYM_CODE128AB       =  60, ///< Code 128 (Suppress Code Set C)
    SYM_CODE128B        =  60, ///< Legacy, same as SYM_CODE128AB
    SYM_AUSPOST         =  63, ///< Australia Post Standard Customer
    SYM_AUSREPLY        =  66, ///< Australia Post Reply Paid
    SYM_AUSROUTE        =  67, ///< Australia Post Routing
    SYM_AUSREDIRECT     =  68, ///< Australia Post Redirection
    SYM_ISBNX           =  69, ///< ISBN
    SYM_RM4SCC          =  70, ///< Royal Mail 4-State Customer Code
    SYM_DATAMATRIX      =  71, ///< Data Matrix (ECC200)
    SYM_EAN14           =  72, ///< EAN-14
    SYM_VIN             =  73, ///< Vehicle Identification Number
    SYM_CODABLOCKF      =  74, ///< Codablock-F
    SYM_NVE18           =  75, ///< NVE-18 (SSCC-18)
    SYM_JAPANPOST       =  76, ///< Japanese Postal Code
    SYM_KOREAPOST       =  77, ///< Korea Post
    SYM_DBAR_STK        =  79, ///< GS1 DataBar Stacked
    SYM_RSS14STACK      =  79, ///< Legacy, same as SYM_DBAR_STK
    SYM_DBAR_OMNSTK     =  80, ///< GS1 DataBar Stacked Omnidirectional
    SYM_RSS14STACK_OMNI =  80, ///< Legacy, same as SYM_DBAR_OMNSTK
    SYM_DBAR_EXPSTK     =  81, ///< GS1 DataBar Expanded Stacked
    SYM_RSS_EXPSTACK    =  81, ///< Legacy, same as SYM_DBAR_EXPSTK
    SYM_PLANET          =  82, ///< USPS PLANET
    SYM_MICROPDF417     =  84, ///< MicroPDF417
    SYM_USPS_IMAIL      =  85, ///< USPS Intelligent Mail (OneCode)
    SYM_ONECODE         =  85, ///< Legacy, same as SYM_USPS_IMAIL
    SYM_PLESSEY         =  86, ///< UK Plessey

    // Tbarcode 8 codes
    SYM_TELEPEN_NUM     =  87, ///< Telepen Numeric
    SYM_ITF14           =  89, ///< ITF-14
    SYM_KIX             =  90, ///< Dutch Post KIX Code
    SYM_AZTEC           =  92, ///< Aztec Code
    SYM_DAFT            =  93, ///< DAFT Code
    SYM_DPD             =  96, ///< DPD Code
    SYM_MICROQR         =  97, ///< Micro QR Code

    // Tbarcode 9 codes
    SYM_HIBC_128        =  98, ///< HIBC (Health Industry Barcode) Code 128
    SYM_HIBC_39         =  99, ///< HIBC Code 39
    SYM_HIBC_DM         = 102, ///< HIBC Data Matrix
    SYM_HIBC_QR         = 104, ///< HIBC QR Code
    SYM_HIBC_PDF        = 106, ///< HIBC PDF417
    SYM_HIBC_MICPDF     = 108, ///< HIBC MicroPDF417
    SYM_HIBC_BLOCKF     = 110, ///< HIBC Codablock-F
    SYM_HIBC_AZTEC      = 112, ///< HIBC Aztec Code

    // Tbarcode 10 codes
    SYM_DOTCODE         = 115, ///< DotCode
    SYM_HANXIN          = 116, ///< Han Xin (Chinese Sensible) Code

    // Tbarcode 11 codes
    SYM_MAILMARK_2D     = 119, ///< Royal Mail 2D Mailmark (CMDM) (Data Matrix)
    SYM_UPU_S10         = 120, ///< Universal Postal Union S10
    SYM_MAILMARK_4S     = 121, ///< Royal Mail 4-State Mailmark
    SYM_MAILMARK        = 121, ///< Legacy, same as SYM_MAILMARK_4S

    // Zint specific
    SYM_AZRUNE          = 128, ///< Aztec Runes
    SYM_CODE32          = 129, ///< Code 32
    SYM_EANX_CC         = 130, ///< EAN Composite
    SYM_GS1_128_CC      = 131, ///< GS1-128 Composite
    SYM_EAN128_CC       = 131, ///< Legacy, same as SYM_GS1_128_CC
    SYM_DBAR_OMN_CC     = 132, ///< GS1 DataBar Omnidirectional Composite
    SYM_RSS14_CC        = 132, ///< Legacy, same as SYM_DBAR_OMN_CC
    SYM_DBAR_LTD_CC     = 133, ///< GS1 DataBar Limited Composite
    SYM_RSS_LTD_CC      = 133, ///< Legacy, same as SYM_DBAR_LTD_CC
    SYM_DBAR_EXP_CC     = 134, ///< GS1 DataBar Expanded Composite
    SYM_RSS_EXP_CC      = 134, ///< Legacy, same as SYM_DBAR_EXP_CC
    SYM_UPCA_CC         = 135, ///< UPC-A Composite
    SYM_UPCE_CC         = 136, ///< UPC-E Composite
    SYM_DBAR_STK_CC     = 137, ///< GS1 DataBar Stacked Composite
    SYM_RSS14STACK_CC   = 137, ///< Legacy, same as SYM_DBAR_STK_CC
    SYM_DBAR_OMNSTK_CC  = 138, ///< GS1 DataBar Stacked Omnidirectional Composite
    SYM_RSS14_OMNI_CC   = 138, ///< Legacy, same as SYM_DBAR_OMNSTK_CC
    SYM_DBAR_EXPSTK_CC  = 139, ///< GS1 DataBar Expanded Stacked Composite
    SYM_RSS_EXPSTACK_CC = 139, ///< Legacy, same as SYM_DBAR_EXPSTK_CC
    SYM_CHANNEL         = 140, ///< Channel Code
    SYM_CODEONE         = 141, ///< Code One
    SYM_GRIDMATRIX      = 142, ///< Grid Matrix
    SYM_UPNQR           = 143, ///< UPNQR (Univerzalnega Plačilnega Naloga QR)
    SYM_ULTRA           = 144, ///< Ultracode
    SYM_RMQR            = 145, ///< Rectangular Micro QR Code (rMQR)
    SYM_BC412           = 146, ///< IBM BC412 (SEMI T1-95)
    SYM_DXFILMEDGE      = 147, ///< DX Film Edge Barcode on 35mm and APS films
    SYM_LAST            = 147  ///< Max barcode number marker, not barcode
  };

/// Zint Output options (`symbol->output_options`)
const int OUT_BIND_TOP                = 0x00001; ///< Boundary bar above the symbol only (not below), does not affect stacking (\note value was once used by the legacy (never-used) BARCODE_NO_ASCII)
const int OUT_BIND                    = 0x00002; ///< Boundary bars above & below the symbol and between stacked symbols
const int OUT_BOX                     = 0x00004; ///< Box around symbol
const int OUT_STDOUT                  = 0x00008; ///< Output to stdout
const int OUT_READER_INIT             = 0x00010; ///< Reader Initialisation (Programming)
const int OUT_SMALL_TEXT              = 0x00020; ///< Use smaller font
const int OUT_BOLD_TEXT               = 0x00040; ///< Use bold font
const int OUT_CMYK_COLOUR             = 0x00080; ///< CMYK colour space (Encapsulated PostScript and TIF)
const int OUT_DOTTY_MODE              = 0x00100; ///< Plot a matrix symbol using dots rather than squares
const int OUT_GS1_GS_SEPARATOR        = 0x00200; ///< Use GS instead of FNC1 as GS1 separator (Data Matrix)
const int OUT_BUFFER_INTERMEDIATE     = 0x00400; ///< Return ASCII values in bitmap buffer (OUT_BUFFER only)
const int OUT_QUIET_ZONES             = 0x00800; ///< Add compliant quiet zones (additional to any specified whitespace) (\note CODE16K, CODE49, CODABLOCKF, ITF14, EAN/UPC have default quiet zones)
const int OUT_NO_QUIET_ZONES          = 0x01000; ///< Disable quiet zones, notably those with defaults as listed above
const int OUT_COMPLIANT_HEIGHT        = 0x02000; ///< Warn if height not compliant, or use standard height (if any) as default
const int OUT_EANUPC_GUARD_WHITESPACE = 0x04000; ///< Add quiet zone indicators ("<"/">") to HRT whitespace (EAN/UPC)
const int OUT_EMBED_VECTOR_FONT       = 0x08000; ///< Embed font in vector output - currently only for SVG output
const int OUT_MEMORY_FILE             = 0x10000; ///< Write output to in-memory buffer `memfile` instead of to `outfile`

// Input data types (`symbol->input_mode`)
const int INP_DATA_MODE              = 0;       ///< Binary
const int INP_UNICODE_MODE           = 1;       ///< UTF-8
const int INP_GS1_MODE               = 2;       ///< GS1
// The following may be OR-ed with above
const int INP_ESCAPE_MODE            = 0x0008;  ///< Process escape sequences
const int INP_GS1PARENS_MODE         = 0x0010;  ///< Process parentheses as GS1 AI delimiters (instead of square brackets)
const int INP_GS1NOCHECK_MODE        = 0x0020;  ///< Do not check validity of GS1 data (except that printable ASCII only)
const int INP_HEIGHTPERROW_MODE      = 0x0040;  ///< Interpret `height` as per-row rather than as overall height
const int INP_FAST_MODE              = 0x0080;  ///< Use faster if less optimal encodation or other shortcuts if available (affects DATAMATRIX, MICROPDF417, PDF417, QRCODE & UPNQR only)
const int INP_EXTRA_ESCAPE_MODE      = 0x0100;  ///< Process special symbology-specific escape sequences as well as others (\note currently Code 128 only)

/// Data Matrix specific options (`symbol->option_3`)
const int OPT_DM_SQUARE             = 100;     ///< Only consider square versions on automatic symbol size selection
const int OPT_DM_DMRE               = 101;     ///< Consider DMRE versions on automatic symbol size selection
const int OPT_DM_ISO_144            = 128;     ///< Use ISO instead of "de facto" format for 144x144 (i.e. don't skew ECC)

/// QR, Han Xin, Grid Matrix specific options (`symbol->option_3`)
const int OPT_FULL_MULTIBYTE         = 200;     ///< Enable Kanji/Hanzi compression for Latin-1 & binary data */

/// Ultracode specific option (`symbol->option_3`)
const int OPT_ULTRA_COMPRESSION      = 128;     ///< Enable Ultracode compression (experimental)

/// Warning and error conditions (API return values)
enum class ReturnCode : int
{
  BC_WARN_HRT_TRUNCATED     = 1,   ///< Human Readable Text was truncated (max 199 bytes)
  BC_WARN_INVALID_OPTION    = 2,   ///< Invalid option given but overridden by Zint
  BC_WARN_USES_ECI          = 3,   ///< Automatic ECI inserted by Zint
  BC_WARN_NONCOMPLIANT      = 4,   ///< Symbol created not compliant with standards
  BC_ERROR                  = 5,   ///< Warn/error marker, not returned
  BC_ERROR_TOO_LONG         = 5,   ///< Input data wrong length
  BC_ERROR_INVALID_DATA     = 6,   ///< Input data incorrect
  BC_ERROR_INVALID_CHECK    = 7,   ///< Input check digit incorrect
  BC_ERROR_INVALID_OPTION   = 8,   ///< Incorrect option given
  BC_ERROR_ENCODING_PROBLEM = 9,   ///< Internal error (should not happen)
  BC_ERROR_FILE_ACCESS      = 10,  ///< Error opening output file
  BC_ERROR_MEMORY           = 11,  ///< Memory allocation (malloc) failure
  BC_ERROR_FILE_WRITE       = 12,  ///< Error writing to output file
  BC_ERROR_USES_ECI         = 13,  ///< Error counterpart of warning if WARN_FAIL_ALL set (see below)
  BC_ERROR_NONCOMPLIANT     = 14,  ///< Error counterpart of warning if WARN_FAIL_ALL set
  BC_ERROR_HRT_TRUNCATED    = 15   ///< Error counterpart of warning if WARN_FAIL_ALL set
};

/// Warning level (`symbol->warn_level`)
const int WARN_DEFAULT       =    0;  ///< Default behaviour
const int WARN_FAIL_ALL      =    2;  ///< Treat warning as error

/// Zint Capability flags (ZBarcode_Cap() `cap_flag`)
const int CAP_HRT              = 0x0001;  ///< Prints Human Readable Text?
const int CAP_STACKABLE        = 0x0002;  ///< Is stackable?
const int CAP_EANUPC           = 0x0004;  ///< Is EAN/UPC?
const int CAP_EXTENDABLE       = 0x0004;  ///< Legacy
const int CAP_COMPOSITE        = 0x0008;  ///< Can have composite data?
const int CAP_ECI              = 0x0010;  ///< Supports Extended Channel Interpretations?
const int CAP_GS1              = 0x0020;  ///< Supports GS1 data?
const int CAP_DOTTY            = 0x0040;  ///< Can be output as dots?
const int CAP_QUIET_ZONES      = 0x0080;  ///< Has default quiet zones?
const int CAP_FIXED_RATIO      = 0x0100;  ///< Has fixed width-to-height (aspect) ratio?
const int CAP_READER_INIT      = 0x0200;  ///< Supports Reader Initialisation?
const int CAP_FULL_MULTIBYTE   = 0x0400;  ///< Supports full-multibyte option?
const int CAP_MASK             = 0x0800;  ///< Is mask selectable?
const int CAP_STRUCTAPP        = 0x1000;  ///< Supports Structured Append?
const int CAP_COMPLIANT_HEIGHT = 0x2000;  ///< Has compliant height?

} // namespace wxPdfBarcode


// Forward declarations
struct zint_symbol;
struct zint_structapp;
class WXDLLIMPEXP_FWD_PDFDOC wxPdfDocument;

/// Class representing Zint segments.
/**
* wxString version of `struct zint_seg`
*/
class WXDLLIMPEXP_PDFDOC wxPdfZintSeg
{
public:
  wxString m_text; // `seg->source` and `seg->length`
  int m_eci; // `seg->eci`

  wxPdfZintSeg();
  wxPdfZintSeg(const wxString& text, const int ECIIndex = 0); // `ECIIndex` is comboBox index (not ECI value)
};

/// Class representing Zint barcode objects.
/**
* All supported barcodes are drawn directly in PDF.
*/
class WXDLLIMPEXP_PDFDOC wxPdfBarcodeZint : public wxObject
{
public:
  /// Constructor
  wxPdfBarcodeZint();

  /// Destructor
  ~wxPdfBarcodeZint();

  /// Get symbology in use
  /**
  * \return the barcode symbology in use
  */
  wxPdfBarcode::Symbology GetSymbology() const { return static_cast<wxPdfBarcode::Symbology>(m_symbol); }

  /// Set symbology to use
  /**
  * \param symbol the barcode symbology to use
  */
  void SetSymbology(wxPdfBarcode::Symbology symbol) { m_symbol = static_cast<int>(symbol); }

  /// Get the input data encoding. Default UNICODE_MODE
  /**
  * \return the input mode
  */
  int GetInputMode() const { return m_inputMode; }

  /// Set the input data encoding. Default UNICODE_MODE
  /**
  * \param inputMode the input mode
  */
  void SetInputMode(int inputMode) { m_inputMode = inputMode; }

  // Note text/eci and segs are mutally exclusive

  /// Get the input data
  /**
  * \return the input data (segment 0 text)
  */
  wxString GetText() const { return m_text; }

  /// Set the input data
  /**
  * \param text the barcode text
  * \note clears segments
  */
  void SetText(const wxString& text);

  /// Get the segments.
  /**
  * \return an array with the segments
  */
  std::vector<wxPdfZintSeg> GetSegments() const { return m_segs; }

  /// Set the segments.
  /**
  * \param segs the array of segments
  * \note clears text and sets eci
  */
  void SetSegments(const std::vector<wxPdfZintSeg>& segs);

  /// Get the primary message (Maxicode, Composite)
  /**
  * \return the text of the primary message
  */
  wxString GetPrimaryMessage() const { return m_primaryMessage; }

  /// Set the primary message (Maxicode, Composite)
  /**
  * \param primaryMessage the primary text
  */
  void SetPrimaryMessage(const wxString& primaryMessage) { m_primaryMessage = primaryMessage; }

  /// Get the symbol height in X-dimensions
  /**
  * \return the height in X-dimensions
  */
  double GetHeight() const { return m_height; }

  /// Set the symbol height in X-dimensions
  /**
  * \param height the height in X-dimensions
  */
  void SetHeight(double height) { m_height = height; }

  /// Get symbol-specific option 1
  /**
  * \return the value of option 1
  */
  int GetOption1() const { return m_option1; }

  /// Set symbol-specific option 1
  /**
  * \param option the value for option 1
  */
  void SetOption1(int option) { m_option1 = option; }

  /// Get symbol-specific option 2
  /**
  * \return the value of option 2
  */
  int GetOption2() const { return m_option2; }

  /// Set symbol-specific option 2
  /**
  * \param option the value for option 2
  */
  void SetOption2(int option) { m_option2 = option; }

  /// Get symbol-specific option 3
  /**
  * \return the value of option 3
  */
  int GetOption3() const { return m_option3; }

  /// Set symbol-specific option 3
  /**
  * \param option the value for option 3
  */
  void SetOption3(int option) { m_option3 = option; }

  /// Get scale factor when printing barcode, i.e. adjusts X-dimension
  /**
  * \return the scale factor
  */
  double GetScale() const { return m_scale; }

  /// Set scale factor when printing barcode, i.e. adjusts X-dimension
  /**
  * \param scale the scale factor
  */
  void SetScale(double scale) { m_scale = scale; }

  /// Get the resolution of output in dots per mm (BMP/EMF/PCX/PNG/TIF only)
  /**
  * \return the resoulution in dots per mm
  */
  double GetDpmm() const { return m_dpmm; }

  /// Set the resolution of output in dots per mm (BMP/EMF/PCX/PNG/TIF only)
  /**
  * \param dpmm the resoulution in dots per mm
  */
  void SetDpmm(double dpmm) { m_dpmm = dpmm; }

  /// Get dotty mode
  /**
  * \return TRUE if dotty mode is enabled, FALSE otherwise
  */
  bool GetDotty() const { return m_dotty; }

  /// Set dotty mode
  /**
  * \param dotty TRUE enables dotty mode, FALSE disables it
  */
  void SetDotty(bool dotty) { m_dotty = dotty; }

  /// Get size of dots used in dotty mode
  /**
  * \return the size of a dot in dotty mode
  */
  double GetDotSize() const { return m_dotSize; }

  /// Set size of dots used in dotty mode
  /**
  * \param dotSize the size of a dot in dotty mode
  */
  void SetDotSize(double dotSize) { m_dotSize = dotSize; }

  /// Get the height in X-dimensions that EAN/UPC guard bars descend
  /**
  * \return the height of the guard bar descend
  */
  double GetGuardDescent() const { return m_guardDescent; }

  /// Set the height in X-dimensions that EAN/UPC guard bars descend
  /**
  * \param guardDescent the height of the guard bar descend
  */
  void SetGuardDescent(double guardDescent) { m_guardDescent = guardDescent; }

  // Structured Append info
  int GetStructAppCount() const;
  int GetStructAppIndex() const;
  wxString GetStructAppID() const;
  void SetStructApp(const int count, const int index, const wxString& id);
  void ClearStructApp();

  /// Get the foreground colour (may be RGB(A) hex string or CMYK decimal "C,M,Y,K" percentage string)
  /**
  * \return the foreground colour as a string
  */
  wxString GetFgColourStr() const { return m_fgStr; }

  /// Set the foreground colour (may be RGB(A) hex string or CMYK decimal "C,M,Y,K" percentage string)
  /**
  * \param fgStr the foreground colour as a string
  */
  bool SetFgColourStr(const wxString& fgStr); // Returns false if not valid colour string

  /// Get the foreground colour as wxColour
  /**
  * \return the foreground colour
  */
  wxColour GetFgColour() const; // `symbol->fgcolour`

  /// Set the foreground colour as wxColour
  /**
  * \param fgColor the foreground colour
  */
  void SetFgColour(const wxColour& fgColor);

  /// Get the background colour (may be RGB(A) hex string or CMYK decimal "C,M,Y,K" percentage string)
  /**
  * \return the background colour as a string
  */
  wxString GetBgColourStr() const { return m_bgStr; }

  /// Set the background colour (may be RGB(A) hex string or CMYK decimal "C,M,Y,K" percentage string)
  /**
  * \param bgStr the background colour as a string
  */
  bool SetBgColourStr(const wxString& bgStr); // Returns false if not valid colour string

  /// Get the background colour as wxColour
  /**
  * \return the background colour
  */
  wxColour GetBgColour() const; // `symbol->bgcolour`

  /// Set the background colour as wxColour
  /**
  * \param bgColor the background colour
  */
  void SetBgColour(const wxColour& bgColor);

  /// Get flag whether CMYK colour space is used (Encapsulated PostScript and TIF)
  /**
  * \return TRUE if CMYK colour space is used, FALSE otherwise
  */
  bool GetCMYK() const { return m_cmyk; }

  /// Enable or disable the use of CMYK colour space (Encapsulated PostScript and TIF)
  /**
  * \param cmyk TRUE to enable CMYK colour space, FALSE to disable it
  */
  void SetCMYK(bool cmyk) { m_cmyk = cmyk; }

  /// Get the type of border above/below/around barcode
  /**
  * \return the border type
  */
  int GetBorderType() const { return m_borderType; }

  /// Set the type of border above/below/around barcode
  /**
  * \param borderTypeIndex the border type
  */
  void SetBorderType(int borderTypeIndex);

  /// Get the border width in X-dimensions
  /**
  * \return the border width
  */
  int GetBorderWidth() const { return m_borderWidth; }

  /// Set the border width in X-dimensions
  /**
  * \param borderWidth the border width
  */
  void SetBorderWidth(int borderWidth);

  /// Get the width in X-dimensions of whitespace to left & right of barcode
  /**
  * \return the width of whitespace
  */
  int GetWhitespace() const { return m_whitespace; }

  /// Set the width in X-dimensions of whitespace to left & right of barcode
  /**
  * \param whitespace the width of whitespace
  */
  void SetWhitespace(int whitespace) { m_whitespace = whitespace; }

  /// Get the height in X-dimensions of whitespace above & below the barcode
  /**
  * \return the height of whitespace
  */
  int GetVerticalWhitespace() const { return m_vwhitespace; }

  /// Get the height in X-dimensions of whitespace above & below the barcode
  /**
  * \param vWhitespace the height of whitespace
  */
  void SetVerticalWhitespace(int vWhitespace) { m_vwhitespace = vWhitespace; }

  /// Type of font to use i.e. normal, small, bold or (vector only) small bold
  /**
  * \return the type of font to use
  */
  int GetFontSetting() const { return m_fontSetting; }

  /// Set the type of font to use i.e. normal, small, bold or (vector only) small bold
  /**
  * \param fontSetting the type of font to use
  * \note Bold type is currently not supported
  */
  void SetFontSetting(int fontSetting);

  /// Get the text gap
  /**
  * \return the text gap
  */
  double GetTextGap() const { return m_textGap; }

  /// Set the text gap
  /**
  * \param textGap the text gap to use
  */
  void SetTextGap(double textGap) { m_textGap = textGap; }

  /// Show (true) or hide (false) Human Readable Text
  /**
  * \return TRUE if human readable text is enabled, FALSE otherwise
  */
  bool ShowText() const { return m_showHRT; }

  /// Enable (true) or disable (false) Human Readable Text
  /**
  * \param showText TRUE enables human readable text, FALSE disables it
  */
  void SetShowText(bool showText) { m_showHRT = showText; }

  /// Check whether GS (Group Separator) is used instead of FNC1 as GS1 separator (Data Matrix)
  /**
  * \return TRUE if group separator is used, FALSE otherwise
  */
  bool HasSeparatorGS() const { return m_gssep; }

  /// Use GS (Group Separator) instead of FNC1 as GS1 separator (Data Matrix)
  /**
  * \param gsSep TRUE enables the group separator, FALSE disables it
  */
  void SetSeparatorGS(bool gsSep) { m_gssep = gsSep; }

  /// Add compliant quiet zones (additional to any specified whitespace)
  /**
  * \return TRUE if compliant quiet zones are enabled, FALSE otherwise
  * \note CODE16K, CODE49, CODABLOCKF, ITF14, EAN/UPC have default quiet zones
  */
  bool HasQuietZones() const { return m_quietZones; }

  /// Enable or disable compliant quiet zones (additional to any specified whitespace)
  /**
  * \param quietZones TRUE enables compliant quiet zones, FALSE disables them
  * \note CODE16K, CODE49, CODABLOCKF, ITF14, EAN/UPC have default quiet zones
  */
  void SetQuietZones(bool quietZones) { m_quietZones = quietZones; }

  /// Disable quiet zones, notably those with defaults
  /**
  * \return TRUE if quiet zones are disabled, FALSE otherwise
  * \note CODE16K, CODE49, CODABLOCKF, ITF14, EAN/UPC have default quiet zones
  */
  bool HasNoQuietZones() const { return m_noQuietZones; }

  /// Enable or disable quiet zones, notably those with defaults
  /**
  * \param noQuietZones TRUE disables quiet zones, FALSE enables them
  * \note CODE16K, CODE49, CODABLOCKF, ITF14, EAN/UPC have default quiet zones
  */
  void SetNoQuietZones(bool noQuietZones) { m_noQuietZones = noQuietZones; }

  /// Get compliant height flag
  /**
  * \return TRUE if compliant height is used and checked, FALSE otherwise
  */
  bool GetCompliantHeight() const { return m_compliantHeight; }

  /// Set compliant height
  /**
  * \param compliantHeight TRUE enables compliant height, FALSE disables it
  * \note warning  if height not compliant and use standard height (if any) as default
  */
  void SetCompliantHeight(bool compliantHeight) { m_compliantHeight = compliantHeight; }

  /// Rotate barcode by angle (degrees 0, 90, 180 and 270)
  /**
  * \return the angle by which the barcode is rotated
  */
  int GetRotateAngle() const { return m_rotateAngle; }

  /// Rotate barcode by angle (degrees 0, 90, 180 and 270)
  /**
  * \param rotateAngle angle by which the barcode should be rotated
  * \note only degrees 0, 90, 180 and 270 are allowed
  */
  void SetRotateAngle(int rotateAngle); // Sets literal value

  /// Get Extended Channel Interpretation (segment 0 eci)
  /**
  * \return value of Extended Channel Interpretation
  */
  int GetECI() const { return m_eci; }

  // TODO
  /// Set Extended Channel Interpretation (segment 0 eci)
  /**
  * \param ECIIndex index of Extended Channel Interpretation
  */
  void SetECIFromIndex(int ECIIndex); // Sets from comboBox index

  /// Set Extended Channel Interpretation (segment 0 eci)
  /**
  * \param eci Extended Channel Interpretation
  */
  void SetECI(int eci); // Sets literal value

  /// Get flag whether parentheses are processed as GS1 AI delimiters (instead of square brackets)
  /**
  * \return TRUE if parentheses are processed as GS1 AI delimiters, FALSE otherwise
  */
  bool HasGS1ParensMode() const { return m_gs1parens; }

  /// Set flag whether parentheses are processed as GS1 AI delimiters (instead of square brackets)
  /**
  * \param gs1Parens TRUE if parentheses are processed as GS1 AI delimiters, FALSE otherwise
  */
  void SetGS1ParensMode(bool gs1Parens) { m_gs1parens = gs1Parens; }

  /// Get flag whether validity of GS1 data is not checked (except that printable ASCII only)
  /**
  * \return TRUE if validity of GS1 data is not checked, FALSE otherwise
  */
  bool GetGS1NoCheckMode() const { return m_gs1nocheck; }

  /// Set flag whether validity of GS1 data is not checked (except that printable ASCII only)
  /**
  * \param gs1NoCheck TRUE if validity of GS1 data is not checked, FALSE otherwise
  */
  void SetGS1NoCheckMode(bool gs1NoCheck) { m_gs1nocheck = gs1NoCheck; }

  /// Check whether Reader Initialisation (Programming) is used
  /**
  * \return TRUE if Reader Initialisation is used, FALSE otherwise
  */
  bool HasReaderInit() const { return m_readerInit; }

  /// Set Reader Initialisation (Programming)
  /**
  * \param readerInit TRUE if Reader Initialisation is enabled, FALSE otherwise
  */
  void SetReaderInit(bool readerInit) { m_readerInit = readerInit; }

  /// Check whether quiet zone indicators will be added
  /**
  * \return TRUE if quiet zone indicators will be added, FALSE otherwise
  */
  bool HasGuardWhitespace() const { return m_guardWhitespace; }

  /// Set flag whether to add quiet zone indicators ("<", ">") to HRT (EAN/UPC)
  /**
  * \param guardWhitespace TRUE if quiet zone indicators will be added, FALSE otherwise
  */
  void SetGuardWhitespace(bool guardWhitespace) { m_guardWhitespace = guardWhitespace; }

  /// Check whether the font will be embedded in vector output - currently only for SVG output of EAN/UPC
  /**
  * \return TRUE if the font will be embedded, FALSE otherwise
  */
  bool HasEmbedVectorFont() const { return m_embedVectorFont; }

  /// Set flag whether to embed the font in vector output - currently only for SVG output of EAN/UPC
  /**
  * \param embedVectorFont TRUE if the font will be embedded, FALSE otherwise
  */
  void SetEmbedVectorFont(bool embedVectorFont) { m_embedVectorFont = embedVectorFont; }

  /// Get the warning level
  /**
  * \return the warning level
  */
  int GetWarnLevel() const { return m_warnLevel; }

  /// Set the warning level
  /**
  * \param warnLevel the requested warning level
  */
  void SetWarnLevel(int warnLevel) { m_warnLevel = warnLevel; }

  /// Check the debugging flag
  /**
  * \return TRUE if debugging is enabled, FALSE otherwise
  */
  bool HasDebugFlag() const { return m_debug; }

  /// Debugging flags
  /**
  * \param debug TRUE if debugging should be enabled, FALSE otherwise
  */
  void SetDebugFlag(bool debug) { m_debug = debug; }

  /// Get the encoded width after successful rendering
  /**
  * \return encoded width after successful rendering
  * \note encoded width (no. of modules encoded)
  */
  int GetEncodedWidth() const { return m_encodedWidth; }

  /// Get the number of encoded rows after successful rendering
  /**
  * \return number of encoded rows after successful rendering
  */
  int GetEncodedRows() const { return m_encodedRows; }

  /// Get the encoded height after successful rendering
  /**
  * \return encoded height after successful rendering
  */
  double GetEncodedHeight() const; // Read-only, in X-dimensions

  /// Get the vector width after successful rendering
  /**
  * \return vector width after successful rendering
  */
  double GetVectorWidth() const { return m_vectorWidth; } // Read-only, scaled width

  /// Get the vector height after successful rendering
  /**
  * \return vector height after successful rendering
  */
  double GetVectorHeight() const { return m_vectorHeight; } // Read-only, scaled height

  // Legacy property getters/setters

  /// Set barcode width (option 1)
  /**
  * \param width the barcode width
  */
  void SetWidth(int width); // `symbol->option_1`

  /// Get barcode width (option 1)
  /**
  * \return the barcode width
  */
  int GetWidth() const;

  /// Set the security level (option 2)
  /**
  * \param securityLevel the requested security level
  */
  void SetSecurityLevel(int securityLevel); // `symbol->option_2`

  /// Get the security level (option 2)
  /**
  * \return the security level
  */
  int GetSecurityLevel() const;

  // Test capabilities - `ZBarcode_Cap()`

  /// Check whether the symbology has human readable text
  /**
  * \return TRUE if symbology has human readable text, FALSE otherwise
  */
  static bool HasHRT(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology is stackable
  /**
  * \return TRUE if symbology is stackable, FALSE otherwise
  */
  static bool IsStackable(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology is extendable
  /**
  * \return TRUE if symbology is extendable, FALSE otherwise
  */
  static bool IsExtendable(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology is composite
  /**
  * \return TRUE if symbology is composite, FALSE otherwise
  */
  static bool IsComposite(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology supports ECI
  /**
  * \return TRUE if symbology supports ECI, FALSE otherwise
  */
  static bool SupportsECI(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology supports GS1
  /**
  * \return TRUE if symbology supports GS1, FALSE otherwise
  */
  static bool SupportsGS1(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology is dotty
  /**
  * \return TRUE if symbology is dotty, FALSE otherwise
  */
  static bool IsDotty(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology has default quiet zones
  /**
  * \return TRUE if symbology has default quiet zones, FALSE otherwise
  */
  static bool HasDefaultQuietZones(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology has a fixed ratio
  /**
  * \return TRUE if symbology has a fixed ratio, FALSE otherwise
  */
  static bool HasFixedRatio(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology supports reader init
  /**
  * \return TRUE if symbology supports reader init, FALSE otherwise
  */
  static bool SupportsReaderInit(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology supports full multibyte
  /**
  * \return TRUE if symbology supports full multibyte, FALSE otherwise
  */
  static bool SupportsFullMultibyte(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology has a mask
  /**
  * \return TRUE if symbology has a mask, FALSE otherwise
  */
  static bool HasMask(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology supports structured apps
  /**
  * \return TRUE if symbology supports structured apps, FALSE otherwise
  */
  static bool SupportsStructApp(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology has a compliant height
  /**
  * \return TRUE if symbology has a compliant height, FALSE otherwise
  */
  static bool HasCompliantHeight(wxPdfBarcode::Symbology symbology);

  /// Check whether symbology takes GS1 AI-delimited data
  /**
  * \return TRUE if symbology takes GS1 AI-delimited data, FALSE otherwise
  */
  static bool TakesGS1AIData(wxPdfBarcode::Symbology symbology);

  // Error handling

  /// Get the error or warning code returned by Zint on `render()` or `save_to_file()`
  /**
  * \return the error or warning code
  */
  int GetError() const { return m_error; }

  /// Get the error message returned by Zint on `render()` or `save_to_file()`
  /**
  * \return the error message text
  */
  const wxString& GetLastError() const { return m_lastError; }

  /// Check whether errors occurred
  /**
  * \return TRUE if errors occurred, FALSE otherwise
  */
  bool HasErrors() const; // `symbol->errtxt`

  /// Encode and print barcode to file
  /**
  * \param filename name of file to which the barcode should be saved
  * \return TRUE if saving to file was successful, FALSE otherwise
  */
  bool SaveToFile(const wxString& filename); // `ZBarcode_Print()`

  /// Encode and render barcode to a PDF document.
  /**
  * \param pdfDoc PDF document instance to which the barcode should be rendered
  * \param x horizontal position on the current page of the PDF document
  * \param y vertical position on the current page of the PDF document
  * \note the position (x,y) denotes always the left upper corner of the rectangle
  * into which the resulting barcode is rendered
  */
  void Render(wxPdfDocument& pdfDoc, double x, double y);

  /// Get the default X-dimension of current barcode symbology
  /**
  * \return the default X-dimension
  */
  double GetDefaultXdim() const;

  /// Get the default X-dimension 
  /**
  * \param symbology symbology for which the X-dimension should be determined
  * \return the default X-dimension
  */
  static double GetDefaultXdim(wxPdfBarcode::Symbology symbology);

  /// Return the name of a symbology
  /**
  * \param symbology symbology for which the name should be returned
  * \return the name of the symbology
  */
  static wxString GetBarcodeName(const wxPdfBarcode::Symbology symbology); // `ZBarcode_BarcodeName()`

  /// Check whether PNG support is available
  /**
  * \return TRUE if PNG support is not available, FALSE otherwise
  */
  static bool NoPNG(); // `ZBarcode_NoPng()`

  /// Check validity of symbology
  /**
  * \return TRUE if symbology is valid, FALSE otherwise
  */
  static bool IsValidSymbology(int symbolId);

  /// Version of Zint library "libzint" linked to
  /**
  * \return the version number of the Zint library
  */
  static int GetVersion(); // `ZBarcode_Version()`

private:
  /// Reset the symbol structure for encoding using member fields
  bool ResetSymbol();

  /// `ZBarcode_Encode_and_Buffer_Vector()` or `ZBarcode_Encode_Segs_and_Buffer_Vector()`
  void Encode();

private:
  zint_symbol* m_zintSymbol;
  int m_symbol;
  int m_inputMode;
  wxString m_text;
  wxString m_primaryMessage;
  std::vector<wxPdfZintSeg> m_segs;
  double m_height;
  int m_option1;
  int m_option2;
  int m_option3;
  double m_dpmm;
  double m_scale;
  bool m_dotty;
  double m_dotSize;
  double m_guardDescent;
  double m_textGap;
  zint_structapp* m_structapp;
  wxString m_fgStr;
  wxString m_bgStr;
  bool m_cmyk;
  int m_borderType;
  int m_borderWidth;
  int m_whitespace;
  int m_vwhitespace;
  int m_fontSetting;
  bool m_showHRT;
  bool m_gssep;
  bool m_quietZones;
  bool m_noQuietZones;
  bool m_compliantHeight;
  int m_rotateAngle;
  int m_eci;
  bool m_gs1parens;
  bool m_gs1nocheck;
  bool m_readerInit;
  bool m_guardWhitespace;
  bool m_embedVectorFont;
  int m_warnLevel;
  bool m_debug;
  int m_encodedWidth;
  int m_encodedRows;
  double m_encodedHeight;
  double m_vectorWidth;
  double m_vectorHeight;
  wxString m_lastError;
  int m_error;
};

#endif // PDF_BARCODE_ZINT_H
