/*
** Name:        wxzint.cpp
** Purpose:     Barcode support for wxPdfDocument
** Author:      Ulrich Telle
** Created:     2024-04-08
** Copyright:   (c) 2024-2025 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1900 // MSVC 2015
#pragma warning(disable: 4996) // function or variable may be unsafe
#endif
#endif

#include "wx/pdfdocument.h"
#include "wx/pdffontmanager.h"
#include "wx/pdfbarcodezint.h"
#include <zint.h>

#include <wx/regex.h>

#include <math.h>
#include <stdio.h>

#include "normal_ttf.h" // Arimo
#include "upcean_ttf.h" // OCR-B subset (digits, "<", ">")

static const int maxSegs = 256;
static const int maxCLISegs = 10; // CLI restricted to 10 segments (including main data)

// Matches RGB(A) hex string or CMYK decimal "C,M,Y,K" percentage string
static const wxRegEx colorRE("^([0-9A-Fa-f]{6}([0-9A-Fa-f]{2})?)|(((100|[0-9]{0,2}),){3}(100|[0-9]{0,2}))$");

static const wxString normalFontFamily = "Arimo"; // Sans-serif metrically compatible with Arial
static const wxString upceanFontFamily = "OCRB"; // Monospace OCR-B
static const wxString fontFamilyError = "Arimo";
static const int fontSizeError = 14; // Point size

static int normalFontID = -2; // Use -2 as `addApplicationFontFromData()` returns -1 on error

// Load Arimo from static array
static int loadNormalFont()
{
  wxPdfFontManager::GetFontManager()->RegisterFont((const char*) normal_ttf, sizeof(normal_ttf), "Arimo");
  normalFontID = 1;
  return normalFontID;
}

static int upceanFontID = -2; // Use -2 as `addApplicationFontFromData()` returns -1 on error

// Load OCR-B EAN/UPC subset from static array
static int loadUpceanFont()
{
  wxPdfFontManager::GetFontManager()->RegisterFont((const char*)upcean_ttf, sizeof(upcean_ttf), "OCRB");
  normalFontID = 2;
  return upceanFontID;
}

// Helper to convert wxColour to RGB(A) hex string
static wxString wxColourToStr(const wxColour &color)
{
  if (color.Alpha() == 0xFF)
  {
    return wxString::Format("%02X%02X%02X", color.Red(), color.Green(), color.Blue());
  }
  return wxString::Format("%02X%02X%02X%02X", color.Red(), color.Green(), color.Blue(), color.Alpha());
}

// Helper to convert RGB(A) hex string or CMYK decimal "C,M,Y,K" percentage string) to wxColour
static wxColour strToWxColour(const wxString &text)
{
  int r, g, b, a;
  if (text.Contains(','))
  {
    int comma1 = text.find(',');
    int comma2 = text.find(',', comma1 + 1);
    int comma3 = text.find(',', comma2 + 1);
    int val = 0;
    text.Mid(comma3 + 1).ToInt(&val);
    int black = 100 - val;
    text.Mid(0, comma1).ToInt(&val);
    val = 100 - val;
    r = (int) roundf((0xFF * val * black) / 10000.0f);
    text.Mid(comma1 + 1, comma2 - comma1 - 1).ToInt(&val);
    val = 100 - val;
    g = (int) roundf((0xFF * val * black) / 10000.0f);
    text.Mid(comma2 + 1, comma3 - comma2 - 1).ToInt(&val);
    val = 100 - val;
    b = (int) roundf((0xFF * val * black) / 10000.0f);
    a = 0xFF;
  }
  else
  {
    text.Mid(0, 2).ToInt(&r, 16);
    text.Mid(2, 2).ToInt(&g, 16);
    text.Mid(4, 2).ToInt(&b, 16);
    if (text.length() == 8)
      text.Mid(6, 2).ToInt(&a, 16);
    else
      a = 0xFF;
  }
  return wxColour(r, g, b, a);
}

// Helper to convert ECI combo index to ECI value
static int ECIIndexToECI(const int ECIIndex)
{
  int ret;
  if (ECIIndex >= 1 && ECIIndex <= 11)
  {
    ret = ECIIndex + 2;
  }
  else if (ECIIndex >= 12 && ECIIndex <= 15)
  {
    ret = ECIIndex + 3;
  }
  else if (ECIIndex >= 16 && ECIIndex <= 31)
  {
    ret = ECIIndex + 4;
  }
  else if (ECIIndex == 32)
  {
    ret = 170; // ISO 646 Invariant
  }
  else if (ECIIndex == 33)
  {
    ret = 899; // 8-bit binary data
  }
  else
  {
    ret = 0;
  }
  return ret;
}

// Helper to convert `m_segs` to `struct zint_seg[]`
static int ConvertSegments(const std::vector<wxPdfZintSeg>& m_segs, struct zint_seg segs[], std::vector<wxCharBuffer>& bstrs)
{
  bstrs.reserve(m_segs.size());
  int i;
  for (i = 0; i < (int)m_segs.size() && i < maxSegs && !m_segs[i].m_text.IsEmpty(); i++)
  {
    segs[i].eci = m_segs[i].m_eci;
    bstrs.push_back(m_segs[i].m_text.utf8_str());
    segs[i].source = (unsigned char*)bstrs.back().data();
    segs[i].length = bstrs.back().length();
  }
  return i;
}


// Segment constructors
wxPdfZintSeg::wxPdfZintSeg() : m_eci(0)
{
}

wxPdfZintSeg::wxPdfZintSeg(const wxString& text, const int ECIIndex) : m_text(text), m_eci(ECIIndexToECI(ECIIndex))
{
}

wxPdfBarcodeZint::wxPdfBarcodeZint()
  : m_zintSymbol(nullptr), m_symbol(BARCODE_CODE128), m_inputMode(UNICODE_MODE),
    m_height(0.0f),
    m_option1(-1), m_option2(0), m_option3(0),
    m_dpmm(0.0f),
    m_scale(1.0f),
    m_dotty(false), m_dotSize(4.0f / 5.0f),
    m_guardDescent(5.0f),
    m_textGap(0.0f),
    m_fgStr("000000"), m_bgStr("FFFFFF"), m_cmyk(false),
    m_borderType(0), m_borderWidth(0),
    m_whitespace(0), m_vwhitespace(0),
    m_fontSetting(0),
    m_showHRT(true),
    m_gssep(false),
    m_quietZones(false), m_noQuietZones(false),
    m_compliantHeight(false),
    m_rotateAngle(0),
    m_eci(0),
    m_gs1parens(false), m_gs1nocheck(false),
    m_readerInit(false),
    m_guardWhitespace(false),
    m_embedVectorFont(false),
    m_warnLevel(WARN_DEFAULT), m_debug(false),
    m_encodedWidth(0), m_encodedRows(0), m_encodedHeight(0.0f),
    m_vectorWidth(0.0f), m_vectorHeight(0.0f),
    m_error(0)
{
  m_structapp = new zint_structapp();
  memset(m_structapp, 0, sizeof(zint_structapp));
}

wxPdfBarcodeZint::~wxPdfBarcodeZint()
{
  if (m_zintSymbol)
  {
    ZBarcode_Delete(m_zintSymbol);
  }
  delete m_structapp;
}

bool wxPdfBarcodeZint::ResetSymbol()
{
  m_error = 0;
  m_lastError.clear();

  if (m_zintSymbol)
  {
    ZBarcode_Clear(m_zintSymbol);
  }
  else if (!(m_zintSymbol = ZBarcode_Create()))
  {
    m_error = ZINT_ERROR_MEMORY;
    m_lastError = "Insufficient memory for Zint structure";
    return false;
  }

  m_zintSymbol->symbology = m_symbol;
  m_zintSymbol->height = m_height;
  m_zintSymbol->scale = m_scale;
  m_zintSymbol->whitespace_width = m_whitespace;
  m_zintSymbol->whitespace_height = m_vwhitespace;
  m_zintSymbol->border_width = m_borderWidth;
  m_zintSymbol->output_options = m_borderType | m_fontSetting;
  if (m_dotty)
  {
    m_zintSymbol->output_options |= BARCODE_DOTTY_MODE;
  }
  if (m_cmyk)
  {
    m_zintSymbol->output_options |= CMYK_COLOUR;
  }
  if (m_gssep)
  {
    m_zintSymbol->output_options |= GS1_GS_SEPARATOR;
  }
  if (m_quietZones)
  {
    m_zintSymbol->output_options |= BARCODE_QUIET_ZONES;
  }
  if (m_noQuietZones)
  {
    m_zintSymbol->output_options |= BARCODE_NO_QUIET_ZONES;
  }
  if (m_compliantHeight)
  {
    m_zintSymbol->output_options |= COMPLIANT_HEIGHT;
  }
  if (m_readerInit)
  {
    m_zintSymbol->output_options |= READER_INIT;
  }
  if (m_guardWhitespace)
  {
    m_zintSymbol->output_options |= EANUPC_GUARD_WHITESPACE;
  }
  if (m_embedVectorFont)
  {
    m_zintSymbol->output_options |= EMBED_VECTOR_FONT;
  }
  strcpy(m_zintSymbol->fgcolour, m_fgStr.Left(15).c_str());
  strcpy(m_zintSymbol->bgcolour, m_bgStr.Left(15).c_str());
  strcpy(m_zintSymbol->primary, m_primaryMessage.Left(127).c_str());
  m_zintSymbol->option_1 = m_option1;
  m_zintSymbol->option_2 = m_option2;
  m_zintSymbol->option_3 = m_option3;
  m_zintSymbol->show_hrt = m_showHRT ? 1 : 0;
  m_zintSymbol->input_mode = m_inputMode;
  if (m_gs1parens)
  {
    m_zintSymbol->input_mode |= GS1PARENS_MODE;
  }
  if (m_gs1nocheck)
  {
    m_zintSymbol->input_mode |= GS1NOCHECK_MODE;
  }
  m_zintSymbol->eci = m_eci;
  m_zintSymbol->dpmm = m_dpmm;
  m_zintSymbol->dot_size = m_dotSize;
  m_zintSymbol->guard_descent = m_guardDescent;
  m_zintSymbol->text_gap = m_textGap;
  m_zintSymbol->structapp = *m_structapp;
  m_zintSymbol->warn_level = m_warnLevel;
  m_zintSymbol->debug = m_debug ? ZINT_DEBUG_PRINT : 0;

  return true;
}

void wxPdfBarcodeZint::Encode()
{
  if (ResetSymbol())
  {
    if (m_segs.empty())
    {
      wxScopedCharBuffer bstr(m_text.utf8_str());
      // Note do our own rotation
      m_error = ZBarcode_Encode_and_Buffer_Vector(m_zintSymbol, (unsigned char *) bstr.data(), bstr.length(), 0);
    }
    else
    {
      struct zint_seg segs[maxSegs];
      std::vector<wxCharBuffer> bstrs;
      int seg_count = ConvertSegments(m_segs, segs, bstrs);
      // Note do our own rotation
      m_error = ZBarcode_Encode_Segs_and_Buffer_Vector(m_zintSymbol, segs, seg_count, 0);
    }
    m_lastError = m_zintSymbol->errtxt;
  }

  if (m_error < ZINT_ERROR)
  {
    m_borderType = m_zintSymbol->output_options & (BARCODE_BIND | BARCODE_BOX | BARCODE_BIND_TOP);
    m_height = m_zintSymbol->height;
    m_borderWidth = m_zintSymbol->border_width;
    m_whitespace = m_zintSymbol->whitespace_width;
    m_vwhitespace = m_zintSymbol->whitespace_height;
    m_encodedWidth = m_zintSymbol->width;
    m_encodedRows = m_zintSymbol->rows;
    m_encodedHeight = m_zintSymbol->height;
    m_vectorWidth = m_zintSymbol->vector->width;
    m_vectorHeight = m_zintSymbol->vector->height;
  }
  else
  {
    m_encodedWidth = m_encodedRows = 0;
    m_encodedHeight = m_vectorWidth = m_vectorHeight = 0.0f;
  }
}

// Set input data. Note: clears segs
void wxPdfBarcodeZint::SetText(const wxString& text)
{
  m_text = text;
  m_segs.clear();
}

// Set segments. Note: clears text and sets eci
void wxPdfBarcodeZint::SetSegments(const std::vector<wxPdfZintSeg>& segs)
{
  m_segs = segs;
  m_text.clear();
  if (m_segs.size())
  {
    // Make sure `symbol->eci` synced
    m_eci = m_segs[0].m_eci;
  }
}

int wxPdfBarcodeZint::GetStructAppCount() const
{
  return m_structapp->count;
}

int wxPdfBarcodeZint::GetStructAppIndex() const
{
  return m_structapp->index;
}

wxString wxPdfBarcodeZint::GetStructAppID() const
{
  return m_structapp->id;
}

// Structured Append info
void wxPdfBarcodeZint::SetStructApp(const int count, const int index, const wxString& id)
{
  if (count)
  {
    m_structapp->count = count;
    m_structapp->index = index;
    memset(m_structapp->id, 0, sizeof(m_structapp->id));
    if (!id.IsEmpty())
    {
      wxScopedCharBuffer idArr(id.utf8_str());
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
      strncpy(m_structapp->id, idArr.data(), sizeof(m_structapp->id));
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
    }
  }
  else
  {
    ClearStructApp();
  }
}

void wxPdfBarcodeZint::ClearStructApp()
{
  memset(&m_structapp, 0, sizeof(m_structapp));
}

bool wxPdfBarcodeZint::SetFgColourStr(const wxString& fgStr)
{
  if (colorRE.Matches(fgStr))
  {
    m_fgStr = fgStr;
    return true;
  }
  return false;
}

// Foreground colour as wxColour
wxColour wxPdfBarcodeZint::GetFgColour() const
{
  return strToWxColour(m_fgStr);
}

void wxPdfBarcodeZint::SetFgColour(const wxColour& fgColor)
{
  m_fgStr = wxColourToStr(fgColor);
}

bool wxPdfBarcodeZint::SetBgColourStr(const wxString& bgStr)
{
  if (colorRE.Matches(bgStr))
  {
    m_bgStr = bgStr;
    return true;
  }
  return false;
}

// Background colour as wxColour
wxColour wxPdfBarcodeZint::GetBgColour() const
{
  return strToWxColour(m_bgStr);
}

void wxPdfBarcodeZint::SetBgColour(const wxColour& bgColor)
{
  m_bgStr = wxColourToStr(bgColor);
}

void wxPdfBarcodeZint::SetBorderType(int borderTypeIndex)
{
  if (borderTypeIndex == 1)
  {
    m_borderType = BARCODE_BIND;
  }
  else if (borderTypeIndex == 2)
  {
    m_borderType = BARCODE_BOX;
  }
  else if (borderTypeIndex == 3)
  {
    m_borderType = BARCODE_BIND_TOP;
  }
  else
  {
    m_borderType = 0;
  }
}

void wxPdfBarcodeZint::SetBorderWidth(int borderWidth)
{
  m_borderWidth = (borderWidth < 0 || borderWidth > 16) ? 0 : borderWidth;
}

void wxPdfBarcodeZint::SetFontSetting(int fontSetting)
{ // Sets literal value
  if ((fontSetting & (BOLD_TEXT | SMALL_TEXT)) == fontSetting)
  {
    m_fontSetting = fontSetting;
  }
  else
  {
    m_fontSetting = 0;
  }
}

void wxPdfBarcodeZint::SetRotateAngle(int rotateAngle)
{ // Sets literal value
  if (rotateAngle == 90)
  {
    m_rotateAngle = 90;
  }
  else if (rotateAngle == 180)
  {
    m_rotateAngle = 180;
  }
  else if (rotateAngle == 270)
  {
    m_rotateAngle = 270;
  }
  else
  {
    m_rotateAngle = 0;
  }
}

void wxPdfBarcodeZint::SetECIFromIndex(int ECIIndex)
{ // Sets from comboBox index
  m_eci = ECIIndexToECI(ECIIndex);
}

void wxPdfBarcodeZint::SetECI(int eci)
{ // Sets literal value
  if (eci < 3 || (eci > 35 && eci != 170 && eci != 899) || eci == 14 || eci == 19)
  {
    m_eci = 0;
  }
  else
  {
    m_eci = eci;
  }
}

double wxPdfBarcodeZint::GetEncodedHeight() const
{
  // Read-only, in X-dimensions
  if (m_symbol == BARCODE_MAXICODE)
  {
    // Maxicode encoded height is meaningless, so return fixed value
    return 33 * 0.866f; // √3 / 2
  }
  return m_encodedHeight;
}

// Legacy property getters/setters
void wxPdfBarcodeZint::SetWidth(int width)
{
  SetOption1(width);
}

int wxPdfBarcodeZint::GetWidth() const
{
  return m_option1;
}
  
void wxPdfBarcodeZint::SetSecurityLevel(int securityLevel)
{
  SetOption2(securityLevel);
}
  
int wxPdfBarcodeZint::GetSecurityLevel() const
{
  return m_option2;
}

// Test capabilities - `ZBarcode_Cap()`
bool wxPdfBarcodeZint::HasHRT(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_HRT);
}

bool wxPdfBarcodeZint::IsStackable(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_STACKABLE);
}

bool wxPdfBarcodeZint::IsExtendable(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_EXTENDABLE);
}

bool wxPdfBarcodeZint::IsComposite(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_COMPOSITE);
}

bool wxPdfBarcodeZint::SupportsECI(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_ECI);
}

bool wxPdfBarcodeZint::SupportsGS1(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_GS1);
}

bool wxPdfBarcodeZint::IsDotty(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_DOTTY);
}

bool wxPdfBarcodeZint::HasDefaultQuietZones(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_QUIET_ZONES);
}

bool wxPdfBarcodeZint::HasFixedRatio(wxPdfBarcode::Symbology  symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_FIXED_RATIO);
}

bool wxPdfBarcodeZint::SupportsReaderInit(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_READER_INIT);
}

bool wxPdfBarcodeZint::SupportsFullMultibyte(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_FULL_MULTIBYTE);
}

bool wxPdfBarcodeZint::HasMask(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_MASK);
}

bool wxPdfBarcodeZint::SupportsStructApp(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_STRUCTAPP);
}

bool wxPdfBarcodeZint::HasCompliantHeight(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Cap(static_cast<int>(symbology), ZINT_CAP_COMPLIANT_HEIGHT);
}

// Whether takes GS1 AI-delimited data
bool wxPdfBarcodeZint::TakesGS1AIData(wxPdfBarcode::Symbology symbology)
{
  int symbol = static_cast<int>(symbology);
  switch (symbol)
  {
    case BARCODE_GS1_128:
    case BARCODE_DBAR_EXP:
    case BARCODE_DBAR_EXPSTK:
      return true;
    default:
      return symbol >= BARCODE_EANX_CC && symbol <= BARCODE_DBAR_EXPSTK_CC;
  }
}

// Whether `lastError()` set
bool wxPdfBarcodeZint::HasErrors() const
{
  return m_lastError.length();
}

bool wxPdfBarcodeZint::SaveToFile(const wxString& filename)
{
  if (ResetSymbol())
  {
    strncpy(m_zintSymbol->outfile, filename.utf8_str(), 255);
    m_zintSymbol->outfile[255] = 0;
    if (m_segs.empty())
    {
      wxScopedCharBuffer bstr(m_text.utf8_str());
      m_error = ZBarcode_Encode_and_Print(m_zintSymbol, (unsigned char *) bstr.data(), bstr.length(), m_rotateAngle);
    }
    else
    {
      struct zint_seg segs[maxSegs];
      std::vector<wxCharBuffer> bstrs;
      int seg_count = ConvertSegments(m_segs, segs, bstrs);
      m_error = ZBarcode_Encode_Segs_and_Print(m_zintSymbol, segs, seg_count, m_rotateAngle);
    }
  }
  if (m_error >= ZINT_ERROR)
  {
    m_lastError = m_zintSymbol->errtxt;
    m_encodedWidth = m_encodedRows = 0;
    m_encodedHeight = m_vectorWidth = m_vectorHeight = 0.0f;
    return false;
  }
  return true;
}

// Convert `zint_vector_rect->colour` to wxColour
static wxColour ColourToWxColour(int colour)
{
  switch (colour)
  {
    case 1: // Cyan
      return *wxCYAN;
    case 2: // Blue
      return *wxBLUE;
    case 3: // Magenta
      return wxColour("magenta");
    case 4: // Red
      return *wxRED;
    case 5: // Yellow
      return *wxYELLOW;
    case 6: // Green
      return *wxGREEN;
     case 8: // White
       return *wxWHITE;
     default:
       return *wxBLACK;
  }
}

void wxPdfBarcodeZint::Render(wxPdfDocument& pdfDoc, double x, double y)
{
  struct zint_vector_rect* rect;
  struct zint_vector_hexagon* hex;
  struct zint_vector_circle* circle;
  struct zint_vector_string* string;
  wxColour fgColor = strToWxColour(m_fgStr);
  wxColour bgColor = strToWxColour(m_bgStr);
  double xDimMM = GetDefaultXdim(static_cast<wxPdfBarcode::Symbology>(m_symbol));
  double xDim = xDimMM / 25.4 * 72. / pdfDoc.GetScaleFactor();

  Encode();

  if (m_error > 0) return;

  double xScale = xDim;
  double yScale = xDim;

  double gwidth = m_zintSymbol->vector->width;
  double gheight = m_zintSymbol->vector->height;

  pdfDoc.StartTransform();
  pdfDoc.SetXY(x, y);
  switch (m_rotateAngle)
  {
    case 270:
      pdfDoc.Translate(x, y + gwidth * xScale);
      pdfDoc.Rotate(-m_rotateAngle, 0, 0);
      break;
    case 180:
      pdfDoc.Translate(x + gwidth * xScale, y + gheight * xScale);
      pdfDoc.Rotate(-m_rotateAngle, 0, 0);
      break;
    case 90:
      pdfDoc.Translate(x + gheight*xScale, y);
      pdfDoc.Rotate(-m_rotateAngle, 0, 0);
      break;
    default:
      pdfDoc.Translate(x, y);
      break;
  }

  if (bgColor.GetAlpha() != 0)
  {
    pdfDoc.SetFillColour(bgColor);
    pdfDoc.Rect(0, 0, xScale*gwidth, yScale*gheight, wxPDF_STYLE_FILL);
  }

  // Plot rectangles
  wxColour lastColor;
  rect = m_zintSymbol->vector->rectangles;
  if (rect)
  {
    while (rect)
    {
      if (rect->colour == -1)
      {
        if (fgColor != lastColor)
        {
          pdfDoc.SetFillColour(fgColor);
          lastColor = fgColor;
        }
      }
      else
      {
        wxColour color = ColourToWxColour(rect->colour);
        if (color != lastColor)
        {
          pdfDoc.SetFillColour(color);
          lastColor = color;
        }
      }
      pdfDoc.Rect(xScale * rect->x, yScale * rect->y, xScale * rect->width, yScale * rect->height, wxPDF_STYLE_FILL);
      rect = rect->next;
    }
  }

  // Plot hexagons
  hex = m_zintSymbol->vector->hexagons;
  if (hex)
  {
    double previousDiameter = 0.0;
    double radius = 0.0;
    double halfRadius = 0.0;
    double halfSqrt3Radius = 0.0;
    pdfDoc.SetFillColour(fgColor);
    while (hex)
    {
      if (previousDiameter != hex->diameter)
      {
        previousDiameter = hex->diameter;
        radius = 0.5 * previousDiameter;
        halfRadius = 0.25 * previousDiameter;
        halfSqrt3Radius = 0.43301270189221932338 * previousDiameter;
      }

      wxPdfArrayDouble px;
      wxPdfArrayDouble py;
      px.Add(xScale * (hex->x));                   py.Add(yScale * (hex->y + radius));
      px.Add(xScale * (hex->x + halfSqrt3Radius)); py.Add(yScale * (hex->y + halfRadius));
      px.Add(xScale * (hex->x + halfSqrt3Radius)); py.Add(yScale * (hex->y - halfRadius));
      px.Add(xScale * (hex->x));                   py.Add(yScale * (hex->y - radius));
      px.Add(xScale * (hex->x - halfSqrt3Radius)); py.Add(yScale * (hex->y - halfRadius));
      px.Add(xScale * (hex->x - halfSqrt3Radius)); py.Add(yScale * (hex->y + halfRadius));
      px.Add(xScale * (hex->x));                   py.Add(yScale * (hex->y + radius));
      pdfDoc.Polygon(px, py, wxPDF_STYLE_FILL);

      hex = hex->next;
    }
  }

  // Plot dots (circles)
  lastColor = wxColour();
  circle = m_zintSymbol->vector->circles;
  if (circle)
  {
    while (circle)
    {
      if (circle->colour)
      { // Set means use background colour (legacy, no longer used)
        if (bgColor != lastColor)
        {
          pdfDoc.SetDrawColour(bgColor);
          pdfDoc.SetFillColour(bgColor);
          lastColor = bgColor;
        }
      }
      else
      {
        if (fgColor != lastColor)
        {
          pdfDoc.SetDrawColour(fgColor);
          pdfDoc.SetFillColour(fgColor);
          lastColor = fgColor;
        }
      }
      if (circle->width > 0)
      {
        pdfDoc.SetLineWidth(xScale * circle->width);
        pdfDoc.Circle(xScale * circle->x, yScale * circle->y, xScale * 0.5 * circle->diameter);
      }
      else
      {
        pdfDoc.Marker(xScale * circle->x, yScale * circle->y, wxPDF_MARKER_CIRCLE, xScale * circle->diameter);
      }
      circle = circle->next;
    }
  }

  // Plot text
  string = m_zintSymbol->vector->strings;
  if (string)
  {
    if (normalFontID == -2)
    {
      // First time?
      loadNormalFont();
    }
    if (upceanFontID == -2)
    {
      // First time?
      loadUpceanFont();
    }
    pdfDoc.SetTextColour(fgColor);
#if 0
    // Bold text is currently not supported
    bool bold = (m_zintSymbol->output_options & BOLD_TEXT) && !IsExtendable();
#endif
    wxString fontName = IsExtendable(static_cast<wxPdfBarcode::Symbology>(m_symbol)) ? upceanFontFamily : normalFontFamily;
    while (string)
    {
      pdfDoc.SetFont(fontName, 0, 72./25.4*xDim * string->fsize);

      wxString content = wxString::FromUTF8((const char*)string->text);
      // string->y is baseline of font
      double tx = string->x;
      double ty = string->y;
      double width = pdfDoc.GetStringWidth(content);
      if (string->halign == 2)
      {
        tx = tx - width / xScale; // Right align
      }
      else if (string->halign != 1)
      {
        tx = tx - width * 0.5 / xScale;
      }
      pdfDoc.Text(xScale * tx, yScale * ty, content);
      string = string->next;
    }
  }

  pdfDoc.StopTransform();
}

// Returns the default X-dimension (`ZBarcode_Default_Xdim()`).
// If `symbology` non-zero then used instead of `symbol()`
double wxPdfBarcodeZint::GetDefaultXdim() const
{
  return ZBarcode_Default_Xdim(m_symbol);
}

double wxPdfBarcodeZint::GetDefaultXdim(wxPdfBarcode::Symbology symbology)
{
  return ZBarcode_Default_Xdim(static_cast<int>(symbology));
}

// Return the BARCODE_XXX name of `symbology`
wxString wxPdfBarcodeZint::GetBarcodeName(const wxPdfBarcode::Symbology symbology)
{
  char buf[32];
  if (ZBarcode_BarcodeName(static_cast<int>(symbology), buf) == 0)
  {
    return wxString(buf);
  }
  return wxString();
}

// Whether Zint library "libzint" built with PNG support or not
bool wxPdfBarcodeZint::NoPNG()
{
  return ZBarcode_NoPng() == 1;
}

bool wxPdfBarcodeZint::IsValidSymbology(int symbolId)
{
  return ZBarcode_ValidID(symbolId) != 0;
}

// Version of Zint library "libzint" linked to
int wxPdfBarcodeZint::GetVersion()
{
  return ZBarcode_Version();
}
