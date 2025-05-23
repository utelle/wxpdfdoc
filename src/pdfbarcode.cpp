///////////////////////////////////////////////////////////////////////////////
// Name:        pdfbarcode.cpp
// Purpose:     Implementation of wxPdfBarCodeCreator
// Author:      Ulrich Telle
// Created:     2005-09-12
// Copyright:   (c) 2005-2025 Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfbarcode.cpp Implementation of the barcode creator methods

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/pdfbarcode.h"
#include "wx/pdfdocument.h"

#include "wx/pdfbarcodezint.h"
#include <zint.h>

wxPdfBarCodeCreator::wxPdfBarCodeCreator(wxPdfDocument& document)
{
  m_document = &document;
}

wxPdfBarCodeCreator::~wxPdfBarCodeCreator()
{
}

bool
wxPdfBarCodeCreator::EAN13(double x, double y, const wxString& barcode, double h, double w)
{
  return Barcode(x, y, barcode, h, w, 13);
}

bool
wxPdfBarCodeCreator::UPC_A(double x, double y, const wxString& barcode, double h, double w)
{
  return Barcode(x, y, barcode, h, w, 12);
}

wxUniChar
wxPdfBarCodeCreator::GetCheckDigit(const wxString& barcode)
{
  //Compute the check digit
  int i, digit, r;
  int sum = 0;
  for ( i = 1; i <= 11; i += 2)
  {
    digit = barcode[i] - wxS('0');
    sum += 3 * digit;
  }
  for (i = 0; i <= 10; i += 2)
  {
    digit = barcode[i] - wxS('0');
    sum += digit;
  }
  r = sum % 10;
  if (r > 0)
  {
    r = 10 - r;
  }
  wxUniChar rChar = wxS('0') + r;
  return rChar;
}

bool
wxPdfBarCodeCreator::TestCheckDigit(const wxString& barcode)
{
  //Test validity of check digit
  int i, digit;
  int sum = 0;
  for (i = 1; i <= 11; i += 2)
  {
    digit = barcode[i] - wxS('0');
    sum += 3 * digit;
  }
  for (i = 0; i <= 10; i += 2)
  {
    digit = barcode[i] - wxS('0');
    sum += digit;
  }
  digit = barcode[12] - wxS('0');
  return (sum + digit) % 10 == 0;
}

// Code and parity constants for EAN13 and UPC_A
static wxString bc_codes[3][10] = {
  {
    wxS("0001101"),wxS("0011001"),wxS("0010011"),wxS("0111101"),wxS("0100011"),
    wxS("0110001"),wxS("0101111"),wxS("0111011"),wxS("0110111"),wxS("0001011")
  },
  {
    wxS("0100111"),wxS("0110011"),wxS("0011011"),wxS("0100001"),wxS("0011101"),
    wxS("0111001"),wxS("0000101"),wxS("0010001"),wxS("0001001"),wxS("0010111")
  },
  {
    wxS("1110010"),wxS("1100110"),wxS("1101100"),wxS("1000010"),wxS("1011100"),
    wxS("1001110"),wxS("1010000"),wxS("1000100"),wxS("1001000"),wxS("1110100")
  } };
static int bc_parities[10][6] = {
  { 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 1, 1 },
  { 0, 0, 1, 1, 0, 1 },
  { 0, 0, 1, 1, 1, 0 },
  { 0, 1, 0, 0, 1, 1 },
  { 0, 1, 1, 0, 0, 1 },
  { 0, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 1, 0, 1 },
  { 0, 1, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 1, 0 } };

bool
wxPdfBarCodeCreator::Barcode(double x, double y, const wxString& barcode, double h, double w, unsigned int len)
{
  //Padding
  int padlen = len - 1 - (int) barcode.Length();
  wxString locBarcode = barcode;
  locBarcode.Pad(padlen, wxS('0'), false); //str_pad($barcode,$len-1,'0',STR_PAD_LEFT);
  if (len == 12)
  {
    locBarcode = wxS("0") + locBarcode;
  }
  //Add or control the check digit
  if (locBarcode.Length() == 12)
  {
    locBarcode += wxString(GetCheckDigit(locBarcode));
  }
  else if (!TestCheckDigit(locBarcode))
  {
    //$this->Error('Incorrect check digit');
    return false;
  }
  //Convert digits to bars
  wxString code = wxS("101");
  int digit = locBarcode[0] - wxS('0');
  int* p = bc_parities[digit];
  unsigned int i;
  for (i = 1; i <= 6; i++)
  {
    digit = locBarcode[i] - wxS('0');
    code += bc_codes[p[i-1]][digit];
  }
  code += wxS("01010");
  for (i = 7; i <= 12; i++)
  {
    digit = locBarcode[i] - wxS('0');
    code += bc_codes[2][digit];
  }
  code += wxS("101");
  //Draw bars
  for (i = 0; i < code.Length(); i++)
  {
    if (code[i] == wxS('1'))
    {
      m_document->Rect(x + i * w, y, w, h, wxPDF_STYLE_FILL);
    }
  }
  //Print text under barcode
  m_document->SetFont(wxS("Helvetica"), wxS(""), 12);
  m_document->Text(x, y + h + 11 / m_document->GetScaleFactor(), locBarcode.Right(len));
  return true;
}

// Character set constant for Code39
static wxString code39_chars = wxS("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. $/+%*");

//Conversion tables for Code39
static wxString code39_narrowEncoding[] = {
  wxS("101001101101"), wxS("110100101011"), wxS("101100101011"),
  wxS("110110010101"), wxS("101001101011"), wxS("110100110101"),
  wxS("101100110101"), wxS("101001011011"), wxS("110100101101"),
  wxS("101100101101"), wxS("110101001011"), wxS("101101001011"),
  wxS("110110100101"), wxS("101011001011"), wxS("110101100101"),
  wxS("101101100101"), wxS("101010011011"), wxS("110101001101"),
  wxS("101101001101"), wxS("101011001101"), wxS("110101010011"),
  wxS("101101010011"), wxS("110110101001"), wxS("101011010011"),
  wxS("110101101001"), wxS("101101101001"), wxS("101010110011"),
  wxS("110101011001"), wxS("101101011001"), wxS("101011011001"),
  wxS("110010101011"), wxS("100110101011"), wxS("110011010101"),
  wxS("100101101011"), wxS("110010110101"), wxS("100110110101"),
  wxS("100101011011"), wxS("110010101101"), wxS("100110101101"),
  wxS("100100100101"), wxS("100100101001"), wxS("100101001001"),
  wxS("101001001001"), wxS("100101101101") };

static wxString code39_wideEncoding[] = {
  wxS("101000111011101"), wxS("111010001010111"), wxS("101110001010111"),
  wxS("111011100010101"), wxS("101000111010111"), wxS("111010001110101"),
  wxS("101110001110101"), wxS("101000101110111"), wxS("111010001011101"),
  wxS("101110001011101"), wxS("111010100010111"), wxS("101110100010111"),
  wxS("111011101000101"), wxS("101011100010111"), wxS("111010111000101"),
  wxS("101110111000101"), wxS("101010001110111"), wxS("111010100011101"),
  wxS("101110100011101"), wxS("101011100011101"), wxS("111010101000111"),
  wxS("101110101000111"), wxS("111011101010001"), wxS("101011101000111"),
  wxS("111010111010001"), wxS("101110111010001"), wxS("101010111000111"),
  wxS("111010101110001"), wxS("101110101110001"), wxS("101011101110001"),
  wxS("111000101010111"), wxS("100011101010111"), wxS("111000111010101"),
  wxS("100010111010111"), wxS("111000101110101"), wxS("100011101110101"),
  wxS("100010101110111"), wxS("111000101011101"), wxS("100011101011101"),
  wxS("100010001000101"), wxS("100010001010001"), wxS("100010100010001"),
  wxS("101000100010001"), wxS("100010111011101") };

bool
wxPdfBarCodeCreator::Code39(double x, double y, const wxString& code, bool ext, bool cks, double w, double h, bool wide)
{
  wxString locCode = code;
  //Display code
  m_document->SetFont(wxS("Helvetica"), wxS(""), 10);
  m_document->Text(x, y + h + 4, locCode);

  if (ext)
  {
    if (!locCode.IsAscii())
    {
      // code contains invalid character(s)
      return false;
    }
    //Extended encoding
    locCode = EncodeCode39Ext(locCode);
  }
  else
  {
    //Convert to upper case
    locCode.UpperCase();
    //Check validity
    size_t j;
    bool valid = true;
    for (j = 0; valid && j < locCode.Length(); j++)
    {
      valid = valid && locCode[j] != wxS('*') && code39_chars.Find(locCode[j]) >= 0;
    }
    if (!valid)
    {
      //$this->Error('Invalid barcode value: '.$code);
      return false;
    }
  }

  //Compute checksum
  if (cks)
  {
    locCode += ChecksumCode39(locCode);
  }

  //Add start and stop characters
  locCode = wxS("*") + locCode + wxS("*");

  wxString* encoding = wide ? code39_wideEncoding : code39_narrowEncoding;

  //Inter-character spacing
  wxString gap = (w > 0.29) ? wxS("00") : wxS("0");

  //Convert to bars
  wxString encode = wxS("");
  size_t i;
  for (i = 0; i< locCode.Length(); i++)
  {
    int pos = code39_chars.Find(locCode[i]);
    encode += encoding[pos] + gap;
  }

  //Draw bars
  DrawCode39(encode, x, y, w, h);
  return true;
}


wxUniChar
wxPdfBarCodeCreator::ChecksumCode39(const wxString& code)
{

  //Compute the modulo 43 checksum

  int sum = 0;
  size_t i;
  for (i = 0; i < code.Length(); i++)
  {
    sum += code39_chars.Find(code[i]);
  }
  int r = sum % 43;
  return code39_chars[r];
}

// Encoding table for Code39 Extended
static wxString code39_encode[] = {
  wxS("%U"), wxS("$A"), wxS("$B"), wxS("$C"),
  wxS("$D"), wxS("$E"), wxS("$F"), wxS("$G"),
  wxS("$H"), wxS("$I"), wxS("$J"), wxS("$K"),
  wxS("$L"), wxS("$M"), wxS("$N"), wxS("$O"),
  wxS("$P"), wxS("$Q"), wxS("$R"), wxS("$S"),
  wxS("$T"), wxS("$U"), wxS("$V"), wxS("$W"),
  wxS("$X"), wxS("$Y"), wxS("$Z"), wxS("%A"),
  wxS("%B"), wxS("%C"), wxS("%D"), wxS("%E"),
  wxS(" "),  wxS("/A"), wxS("/B"), wxS("/C"),
  wxS("/D"), wxS("/E"), wxS("/F"), wxS("/G"),
  wxS("/H"), wxS("/I"), wxS("/J"), wxS("/K"),
  wxS("/L"), wxS("-"),  wxS("."),  wxS("/O"),
  wxS("0"),  wxS("1"),  wxS("2"),  wxS("3"),
  wxS("4"),  wxS("5"),  wxS("6"),  wxS("7"),
  wxS("8"),  wxS("9"),  wxS("/Z"), wxS("%F"),
  wxS("%G"), wxS("%H"), wxS("%I"), wxS("%J"),
  wxS("%V"), wxS("A"),  wxS("B"),  wxS("C"),
  wxS("D"),  wxS("E"),  wxS("F"),  wxS("G"),
  wxS("H"),  wxS("I"),  wxS("J"),  wxS("K"),
  wxS("L"),  wxS("M"),  wxS("N"),  wxS("O"),
  wxS("P"),  wxS("Q"),  wxS("R"),  wxS("S"),
  wxS("T"),  wxS("U"),  wxS("V"),  wxS("W"),
  wxS("X"),  wxS("Y"),  wxS("Z"),  wxS("%K"),
  wxS("%L"), wxS("%M"), wxS("%N"), wxS("%O"),
  wxS("%W"), wxS("+A"), wxS("+B"), wxS("+C"),
  wxS("+D"), wxS("+E"), wxS("+F"), wxS("+G"),
  wxS("+H"), wxS("+I"), wxS("+J"), wxS("+K"),
  wxS("+L"), wxS("+M"), wxS("+N"), wxS("+O"),
  wxS("+P"), wxS("+Q"), wxS("+R"), wxS("+S"),
  wxS("+T"), wxS("+U"), wxS("+V"), wxS("+W"),
  wxS("+X"), wxS("+Y"), wxS("+Z"), wxS("%P"),
  wxS("%Q"), wxS("%R"), wxS("%S"), wxS("%T") };

wxString
wxPdfBarCodeCreator::EncodeCode39Ext(const wxString& code)
{

  //Encode characters in extended mode
  wxString codeExt = wxS("");
  size_t i;
  for (i = 0 ; i < code.Length(); i++)
  {
    codeExt += code39_encode[(long int) code[i]];
  }
  return codeExt;
}

void
wxPdfBarCodeCreator::DrawCode39(const wxString& code, double x, double y, double w, double h)
{
  //Draw bars
  size_t i;
  for (i = 0; i < code.Length(); i++)
  {
    if (code[i] == wxS('1'))
    {
      m_document->Rect(x + i * w, y, w, h, wxPDF_STYLE_FILL);
    }
  }
}

// Character and barcode constants for I25
static wxString i25_chars = wxS("0123456789AZ");
static wxString i25_barChar[] = {
  wxS("nnwwn"), wxS("wnnnw"), wxS("nwnnw"), wxS("wwnnn"), wxS("nnwnw"),
  wxS("wnwnn"), wxS("nwwnn"), wxS("nnnww"), wxS("wnnwn"), wxS("nwnwn"),
  wxS("nn"), wxS("wn") };

bool
wxPdfBarCodeCreator::I25(double xpos, double ypos, const wxString& code, double basewidth, double height)
{
  // wide/narrow codes for the digits
  wxString locCode = code;
  double wide = basewidth;
  double narrow = basewidth / 3 ;
  double lineWidth;

  if ((locCode.Length() > 0 && !wxIsdigit(locCode[0])) || !locCode.IsNumber())
  {
    return false;
  }

  // add leading zero if code-length is odd
  if (locCode.Length() % 2 != 0)
  {
    locCode = wxS("0") + locCode;
  }

  m_document->SetFont(wxS("Helvetica"), wxS(""), 10);
  m_document->Text(xpos, ypos + height + 4, locCode);
  m_document->SetFillColour(0);

  // add start and stop codes
  locCode = wxS("AA") + locCode + wxS("ZA");

  size_t i;
  for (i = 0; i < locCode.Length(); i += 2)
  {
    // choose next pair of digits
    int digitBar = i25_chars.Find(locCode[i]);
    int digitSpace = i25_chars.Find(locCode[i+1]);

    // create a wide/narrow-sequence (first digit=bars, second digit=spaces)
    wxString seq = wxS("");
    size_t j;
    for (j = 0; j < i25_barChar[digitBar].Length(); j++)
    {
      seq += wxString(i25_barChar[digitBar][j]) + wxString(i25_barChar[digitSpace][j]);
    }
    for (j = 0; j < seq.Length(); j++)
    {
      // set lineWidth depending on value
      lineWidth = (seq[j] == wxS('n')) ? narrow : wide;
      // draw every second value, because the second digit of the pair is represented by the spaces
      if (j % 2 == 0)
      {
        m_document->Rect(xpos, ypos, lineWidth, height, wxPDF_STYLE_FILL);
      }
      xpos += lineWidth;
    }
  }
  return true;
}

  // draws a bar code for the given zip code using pdf lines
  // triggers error if zip code is invalid
  // x,y specifies the lower left corner of the bar code
bool
wxPdfBarCodeCreator::PostNet(double x, double y, const wxString& zipcode)
{
  // Save nominal bar dimensions in user units
  // Full Bar Nominal Height = 0.125"
  double fullBarHeight = 9 / m_document->GetScaleFactor();
  // Half Bar Nominal Height = 0.050"
  double halfBarHeight = 3.6 / m_document->GetScaleFactor();
  // Full and Half Bar Nominal Width = 0.020"
  double barWidth = 1.44 / m_document->GetScaleFactor();
  // Bar Spacing = 0.050"
  double barSpacing = 3.6 / m_document->GetScaleFactor();

  double fiveBarSpacing = barSpacing * 5;

  // validate the zip code
  if (!ZipCodeValidate(zipcode))
  {
    return false;
  }

  // set the line width
  m_document->SetLineWidth(barWidth);

  // draw start frame bar
  m_document->Line(x, y, x, y - fullBarHeight);
  x += barSpacing;

  // draw digit bars
  size_t i;
  int digit;
  for (i = 0; i < zipcode.Length(); i++)
  {
    if (i != 5)
    {
      digit = zipcode[i] - wxS('0');
      ZipCodeDrawDigitBars(x, y, barSpacing, halfBarHeight, fullBarHeight, digit);
      x += fiveBarSpacing;
    }
  }

  // draw check sum digit
  digit = ZipCodeCheckSumDigit(zipcode);
  ZipCodeDrawDigitBars(x, y, barSpacing, halfBarHeight, fullBarHeight, digit);
  x += fiveBarSpacing;

  // draw end frame bar
  m_document->Line(x, y, x, y - fullBarHeight);
  return true;
}

// valid zip codes are of the form DDDDD or DDDDD-DDDD
// where D is a digit from 0 to 9, returns the validated zip code
bool
wxPdfBarCodeCreator::ZipCodeValidate(const wxString& zipcode)
{
  bool valid = true;
  if (zipcode.Length() == 5 || zipcode.Length() == 10)
  {
    // check that all characters are numeric
    size_t i;
    for (i = 0; valid && i < zipcode.Length(); i++ )
    {
      if ((i != 5 && !wxIsdigit(zipcode[i])) || (i == 5 && zipcode[5] != wxS('-')))
      {
        valid = false;
      }
    }
  }
  else
  {
    valid = false;
  }
  return valid;
}

// takes a validated zip code and
// calculates the checksum for POSTNET
int
wxPdfBarCodeCreator::ZipCodeCheckSumDigit(const wxString& zipcode)
{
  // calculate sum of digits
  size_t i;
  int sum = 0;
  for (i = 0; i < zipcode.Length(); i++)
  {
    if (i != 5)
    {
      sum += (zipcode[i] - wxS('0'));
    }
  }

  // return checksum digit
  int r = sum % 10;
  if (r > 0)
  {
    r = 10 - r;
  }
  return r;
}

// Bar definitions for Postnet
// 1 represents full-height bars and 0 represents half-height bars
static int postnet_barDefinitions[10][5] = {
  { 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1 },
  { 0, 0, 1, 0, 1 },
  { 0, 0, 1, 1, 0 },
  { 0, 1, 0, 0, 1 },
  { 0, 1, 0, 1, 0 },
  { 0, 1, 1, 0, 0 },
  { 1, 0, 0, 0, 1 },
  { 1, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 0 } };

// Takes a digit and draws the corresponding POSTNET bars.
void
wxPdfBarCodeCreator::ZipCodeDrawDigitBars(double x, double y, double barSpacing,
                                           double halfBarHeight, double fullBarHeight,
                                           int digit)
{
  // check for invalid digit
  if (digit >= 0 && digit <= 9)
  {
    // draw the five bars representing a digit
    int i;
    for (i = 0; i < 5; i++)
    {
      if (postnet_barDefinitions[digit][i] == 1)
      {
        m_document->Line(x, y, x, y - fullBarHeight);
      }
      else
      {
        m_document->Line(x, y, x, y - halfBarHeight);
      }
      x += barSpacing;
    }
  }
}

// --- Code128 implementation ---

// Code128 bar specification
static short code128_bars[108][6] = {
  { 2, 1, 2, 2, 2, 2 },  //  0 : [ ]
  { 2, 2, 2, 1, 2, 2 },  //  1 : [!]
  { 2, 2, 2, 2, 2, 1 },  //  2 : ["]
  { 1, 2, 1, 2, 2, 3 },  //  3 : [#]
  { 1, 2, 1, 3, 2, 2 },  //  4 : [$]
  { 1, 3, 1, 2, 2, 2 },  //  5 : [%]
  { 1, 2, 2, 2, 1, 3 },  //  6 : [&]
  { 1, 2, 2, 3, 1, 2 },  //  7 : [']
  { 1, 3, 2, 2, 1, 2 },  //  8 : [(]
  { 2, 2, 1, 2, 1, 3 },  //  9 : [)]

  { 2, 2, 1, 3, 1, 2 },  // 10 : [*]
  { 2, 3, 1, 2, 1, 2 },  // 11 : [+]
  { 1, 1, 2, 2, 3, 2 },  // 12 : [,]
  { 1, 2, 2, 1, 3, 2 },  // 13 : [-]
  { 1, 2, 2, 2, 3, 1 },  // 14 : [.]
  { 1, 1, 3, 2, 2, 2 },  // 15 : [/]
  { 1, 2, 3, 1, 2, 2 },  // 16 : [0]
  { 1, 2, 3, 2, 2, 1 },  // 17 : [1]
  { 2, 2, 3, 2, 1, 1 },  // 18 : [2]
  { 2, 2, 1, 1, 3, 2 },  // 19 : [3]

  { 2, 2, 1, 2, 3, 1 },  // 20 : [4]
  { 2, 1, 3, 2, 1, 2 },  // 21 : [5]
  { 2, 2, 3, 1, 1, 2 },  // 22 : [6]
  { 3, 1, 2, 1, 3, 1 },  // 23 : [7]
  { 3, 1, 1, 2, 2, 2 },  // 24 : [8]
  { 3, 2, 1, 1, 2, 2 },  // 25 : [9]
  { 3, 2, 1, 2, 2, 1 },  // 26 : [:]
  { 3, 1, 2, 2, 1, 2 },  // 27 : [;]
  { 3, 2, 2, 1, 1, 2 },  // 28 : [<]
  { 3, 2, 2, 2, 1, 1 },  // 29 : [=]

  { 2, 1, 2, 1, 2, 3 },  // 30 : [>]
  { 2, 1, 2, 3, 2, 1 },  // 31 : [?]
  { 2, 3, 2, 1, 2, 1 },  // 32 : [@]
  { 1, 1, 1, 3, 2, 3 },  // 33 : [A]
  { 1, 3, 1, 1, 2, 3 },  // 34 : [B]
  { 1, 3, 1, 3, 2, 1 },  // 35 : [C]
  { 1, 1, 2, 3, 1, 3 },  // 36 : [D]
  { 1, 3, 2, 1, 1, 3 },  // 37 : [E]
  { 1, 3, 2, 3, 1, 1 },  // 38 : [F]
  { 2, 1, 1, 3, 1, 3 },  // 39 : [G]

  { 2, 3, 1, 1, 1, 3 },  // 40 : [H]
  { 2, 3, 1, 3, 1, 1 },  // 41 : [I]
  { 1, 1, 2, 1, 3, 3 },  // 42 : [J]
  { 1, 1, 2, 3, 3, 1 },  // 43 : [K]
  { 1, 3, 2, 1, 3, 1 },  // 44 : [L]
  { 1, 1, 3, 1, 2, 3 },  // 45 : [M]
  { 1, 1, 3, 3, 2, 1 },  // 46 : [N]
  { 1, 3, 3, 1, 2, 1 },  // 47 : [O]
  { 3, 1, 3, 1, 2, 1 },  // 48 : [P]
  { 2, 1, 1, 3, 3, 1 },  // 49 : [Q]

  { 2, 3, 1, 1, 3, 1 },  // 50 : [R]
  { 2, 1, 3, 1, 1, 3 },  // 51 : [S]
  { 2, 1, 3, 3, 1, 1 },  // 52 : [T]
  { 2, 1, 3, 1, 3, 1 },  // 53 : [U]
  { 3, 1, 1, 1, 2, 3 },  // 54 : [V]
  { 3, 1, 1, 3, 2, 1 },  // 55 : [W]
  { 3, 3, 1, 1, 2, 1 },  // 56 : [X]
  { 3, 1, 2, 1, 1, 3 },  // 57 : [Y]
  { 3, 1, 2, 3, 1, 1 },  // 58 : [Z]
  { 3, 3, 2, 1, 1, 1 },  // 59 : [[]

  { 3, 1, 4, 1, 1, 1 },  // 60 : [\]
  { 2, 2, 1, 4, 1, 1 },  // 61 : []]
  { 4, 3, 1, 1, 1, 1 },  // 62 : [^]
  { 1, 1, 1, 2, 2, 4 },  // 63 : [_]
  { 1, 1, 1, 4, 2, 2 },  // 64 : [`]
  { 1, 2, 1, 1, 2, 4 },  // 65 : [a]
  { 1, 2, 1, 4, 2, 1 },  // 66 : [b]
  { 1, 4, 1, 1, 2, 2 },  // 67 : [c]
  { 1, 4, 1, 2, 2, 1 },  // 68 : [d]
  { 1, 1, 2, 2, 1, 4 },  // 69 : [e]

  { 1, 1, 2, 4, 1, 2 },  // 70 : [f]
  { 1, 2, 2, 1, 1, 4 },  // 71 : [g]
  { 1, 2, 2, 4, 1, 1 },  // 72 : [h]
  { 1, 4, 2, 1, 1, 2 },  // 73 : [i]
  { 1, 4, 2, 2, 1, 1 },  // 74 : [j]
  { 2, 4, 1, 2, 1, 1 },  // 75 : [k]
  { 2, 2, 1, 1, 1, 4 },  // 76 : [l]
  { 4, 1, 3, 1, 1, 1 },  // 77 : [m]
  { 2, 4, 1, 1, 1, 2 },  // 78 : [n]
  { 1, 3, 4, 1, 1, 1 },  // 79 : [o]

  { 1, 1, 1, 2, 4, 2 },  // 80 : [p]
  { 1, 2, 1, 1, 4, 2 },  // 81 : [q]
  { 1, 2, 1, 2, 4, 1 },  // 82 : [r]
  { 1, 1, 4, 2, 1, 2 },  // 83 : [s]
  { 1, 2, 4, 1, 1, 2 },  // 84 : [t]
  { 1, 2, 4, 2, 1, 1 },  // 85 : [u]
  { 4, 1, 1, 2, 1, 2 },  // 86 : [v]
  { 4, 2, 1, 1, 1, 2 },  // 87 : [w]
  { 4, 2, 1, 2, 1, 1 },  // 88 : [x]
  { 2, 1, 2, 1, 4, 1 },  // 89 : [y]

  { 2, 1, 4, 1, 2, 1 },  // 90 : [z]
  { 4, 1, 2, 1, 2, 1 },  // 91 : [{]
  { 1, 1, 1, 1, 4, 3 },  // 92 : [|]
  { 1, 1, 1, 3, 4, 1 },  // 93 : [}]
  { 1, 3, 1, 1, 4, 1 },  // 94 : [~]
  { 1, 1, 4, 1, 1, 3 },  // 95 : [DEL]
  { 1, 1, 4, 3, 1, 1 },  // 96 : [FNC3]
  { 4, 1, 1, 1, 1, 3 },  // 97 : [FNC2]
  { 4, 1, 1, 3, 1, 1 },  // 98 : [SHIFT]
  { 1, 1, 3, 1, 4, 1 },  // 99 : [Cswap]

  { 1, 1, 4, 1, 3, 1 },  //100 : [Bswap]
  { 3, 1, 1, 1, 4, 1 },  //101 : [Aswap]
  { 4, 1, 1, 1, 3, 1 },  //102 : [FNC1]
  { 2, 1, 1, 4, 1, 2 },  //103 : [Astart]
  { 2, 1, 1, 2, 1, 4 },  //104 : [Bstart]
  { 2, 1, 1, 2, 3, 2 },  //105 : [Cstart]
  { 2, 3, 3, 1, 1, 1 },  //106 : [STOP]
  { 2, 1, 0, 0, 0, 0 }   //107 : [END BAR]
};

// Code128 special codes
const wxStringCharType CODE128_FNC3_INDEX  =  96;
const wxStringCharType CODE128_FNC2_INDEX  =  97;
const wxStringCharType CODE128_SHIFT       =  98;
const wxStringCharType CODE128_CODE_TO_C   =  99;
const wxStringCharType CODE128_CODE_TO_B   = 100;
const wxStringCharType CODE128_CODE_TO_A   = 101;
const wxStringCharType CODE128_FNC1_INDEX  = 102;
const wxStringCharType CODE128_START_A     = 103;
const wxStringCharType CODE128_START_B     = 104;
const wxStringCharType CODE128_START_C     = 105;
const wxStringCharType CODE128_BARS_STOP   = 106;
const wxStringCharType CODE128_ENDBAR      = 107;

// Code128 internal functions

static bool Code128ValidChar(int ch)
{
  return (ch >= 0 && ch <= 127) || (ch >= CODE128_FNC1 && ch <= CODE128_FNC4);
}

static bool Code128ValidInCodeSetA(int ch)
{
  return (ch >= 0 && ch <= 95) || (ch >= CODE128_FNC1 && ch <= CODE128_FNC4);
}

static bool Code128ValidInCodeSetB(wxUniChar ch)
{
  return (ch >= 32 && ch <= 127) || (ch >= CODE128_FNC1 && ch <= CODE128_FNC4);
}

static bool Code128ValidInCodeSetC(wxUniChar ch)
{
  return (ch >= '0' && ch <= '9');
}

static void Code128AddCheck(wxString& barcode)
{
  size_t k = 1;
  wxString::const_iterator ch = barcode.begin();
  int chk = *ch;
  for (++ch; ch != barcode.end(); ++ch, ++k)
  {
    chk += (int)(*ch) * k;
  }
  chk = chk % 103;
  barcode += wxUniChar(chk);
  barcode += CODE128_BARS_STOP;
  barcode += CODE128_ENDBAR;
}

#if 0
static wxString Code128RemoveFNC1(const wxString& code)
{
  wxString buffer = wxEmptyString;
  size_t len = code.length();
  size_t k;
  for (k = 0; k < len; ++k)
  {
    wxUniChar c = code[k];
    if (c >= 32 && c <= 126)
    {
      buffer += c;
    }
  }
  return buffer;
}
#endif

static bool Code128IsNextDigits(const wxString& text, size_t textIndex, int numDigits)
{
  size_t len = text.length();
  while (textIndex < len && numDigits > 0)
  {
    if (text[textIndex] == CODE128_FNC1)
    {
      ++textIndex;
      continue;
    }
    int n = (numDigits > 2) ? 2 : numDigits;
    if (textIndex + n > len)
      return false;
    while (n-- > 0)
    {
      wxUniChar c = text[textIndex++];
      if (c < wxS('0') || c > wxS('9'))
        return false;
      --numDigits;
    }
  }
  return (numDigits == 0);
}

static wxString Code128PackDigits(const wxString& text, size_t& textIndex, int numDigits)
{
  wxString code = wxEmptyString;
  while (numDigits > 0)
  {
    if (text[textIndex] == CODE128_FNC1)
    {
      code += CODE128_FNC1_INDEX;
      ++textIndex;
      continue;
    }
    numDigits -= 2;
    int c1 = text[textIndex++] - wxS('0');
    int c2 = text[textIndex++] - wxS('0');
    code += wxUniChar(c1 * 10 + c2);
  }
  return code;
}

static wxString Code128MakeCode(const wxString& text, bool ucc)
{
  wxString out = wxEmptyString;
  size_t tLen = text.length();

  // if no text is given return a valid raw barcode
  if (tLen == 0)
  {
    out += CODE128_START_B;
    if (ucc)
    {
      out += CODE128_FNC1_INDEX;
    }
    return out;
  }

  // Check whether barcode text is valid
  wxString::const_iterator ch;
  for (ch = text.begin(); ch != text.end(); ++ch)
  {
    if (*ch > 127 && *ch != CODE128_FNC1)
    {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::Code128RawText: ")) +
                 wxString::Format(_("There are illegal characters for barcode 128 in '%s'."), text.c_str()));
      return out;
    }
  }

  wxUniChar c = text[0];
  wxUniChar currentCode = CODE128_START_B;
  size_t index = 0;
  if (Code128IsNextDigits(text, index, 2))
  {
    currentCode = CODE128_START_C;
    out += currentCode;
    if (ucc)
    {
      out += CODE128_FNC1_INDEX;
    }
    out += Code128PackDigits(text, index, 2);
  }
  else if (c < 32)
  {
    currentCode = CODE128_START_A;
    out += currentCode;
    if (ucc)
      out += CODE128_FNC1_INDEX;
    out += wxUniChar((int) c + 64);
    ++index;
  }
  else
  {
    out += currentCode;
    if (ucc)
    {
      out += CODE128_FNC1_INDEX;
    }
    if (c == CODE128_FNC1)
    {
      out += CODE128_FNC1_INDEX;
    }
    else
    {
      out += wxUniChar((int) c - 32);
    }
    ++index;
  }
  while (index < tLen)
  {
    switch (currentCode.GetValue())
    {
      case CODE128_START_A:
        {
          if (Code128IsNextDigits(text, index, 4))
          {
            currentCode = CODE128_START_C;
            out += CODE128_CODE_TO_C;
            out += Code128PackDigits(text, index, 4);
          }
          else
          {
            c = text[index++];
            switch (c.GetValue())
            {
              case CODE128_FNC1:
                out += CODE128_FNC1_INDEX;
                break;
              case CODE128_FNC2:
                out += CODE128_FNC2_INDEX;
                break;
              case CODE128_FNC3:
                out += CODE128_FNC3_INDEX;
                break;
              case CODE128_FNC4:
                out += CODE128_CODE_TO_A;
                break;
              default:
                if (c >= 96)
                {
                  if (index < tLen && text[index] >= 96)
                  {
                    currentCode = CODE128_START_B;
                    out += CODE128_CODE_TO_B;
                  }
                  else
                  {
                    out += CODE128_SHIFT;
                  }
                  out += wxUniChar((int) c - 32);
                }
                else if (c < 32)
                {
                  out += wxUniChar((int) c + 64);
                }
                else
                {
                 out += wxUniChar((int) c - 32);
                }
                break;
            }
          }
        }
        break;
      case CODE128_START_B:
        {
          if (Code128IsNextDigits(text, index, 4))
          {
            currentCode = CODE128_START_C;
            out += CODE128_CODE_TO_C;
            out += Code128PackDigits(text, index, 4);
          }
          else
          {
            c = text[index++];
            switch (c.GetValue())
            {
              case CODE128_FNC1:
                out += CODE128_FNC1_INDEX;
                break;
              case CODE128_FNC2:
                out += CODE128_FNC2_INDEX;
                break;
              case CODE128_FNC3:
                out += CODE128_FNC3_INDEX;
                break;
              case CODE128_FNC4:
                out += CODE128_CODE_TO_B;
                break;
              default:
                if (c < 32)
                {
                  if (index < tLen && text[index] < 32)
                  {
                    currentCode = CODE128_START_A;
                    out += CODE128_CODE_TO_A;
                    out += wxUniChar((int) c + 64);
                  }
                  else
                  {
                    out += CODE128_SHIFT;
                    out += wxUniChar((int) c + 64);
                  }
                }
                else
                {
                  out += wxUniChar((int) c - 32);
                }
                break;
            }
          }
        }
        break;
      case CODE128_START_C:
        {
          if (Code128IsNextDigits(text, index, 2))
          {
            out += Code128PackDigits(text, index, 2);
          }
          else
          {
            c = text[index++];
            if (c == CODE128_FNC1)
            {
              out += CODE128_FNC1_INDEX;
            }
            else if (c < 32)
            {
              currentCode = CODE128_START_A;
              out += CODE128_CODE_TO_A;
              out += wxUniChar((int) c + 64);
            }
            else
            {
              currentCode = CODE128_START_B;
              out += CODE128_CODE_TO_B;
              out += wxUniChar((int) c - 32);
            }
          }
        }
        break;
    }
  }
  return out;
}

static const struct code128_ailist_t
{
  int ai;
  int len;
}
code128_ailist[] = {
  { 0,    20 },
  { 1,    16 },
  { 2,    16 },
  { 10,   -1 },
  { 11,    9 },
  { 12,    8 },
  { 13,    8 },
  { 15,    8 },
  { 17,    8 },
  { 20,    4 },
  { 21,   -1 },
  { 22,   -1 },
  { 23,   -1 },
  { 30,   -1 },
  { 37,   -1 },
  { 240,  -1 },
  { 241,  -1 },
  { 250,  -1 },
  { 251,  -1 },
  { 252,  -1 },
  { 3900, -1 },
  { 3901, -1 },
  { 3902, -1 },
  { 3903, -1 },
  { 3904, -1 },
  { 3905, -1 },
  { 3906, -1 },
  { 3907, -1 },
  { 3908, -1 },
  { 3909, -1 },
  { 3910, -1 },
  { 3911, -1 },
  { 3912, -1 },
  { 3913, -1 },
  { 3914, -1 },
  { 3915, -1 },
  { 3916, -1 },
  { 3917, -1 },
  { 3918, -1 },
  { 3919, -1 },
  { 3920, -1 },
  { 3921, -1 },
  { 3922, -1 },
  { 3923, -1 },
  { 3924, -1 },
  { 3925, -1 },
  { 3926, -1 },
  { 3927, -1 },
  { 3928, -1 },
  { 3929, -1 },
  { 3930, -1 },
  { 3931, -1 },
  { 3932, -1 },
  { 3933, -1 },
  { 3934, -1 },
  { 3935, -1 },
  { 3936, -1 },
  { 3937, -1 },
  { 3938, -1 },
  { 3939, -1 },
  { 400,  -1 },
  { 401,  -1 },
  { 402,  20 },
  { 403,  -1 },
  { 410,  16 },
  { 411,  16 },
  { 412,  16 },
  { 413,  16 },
  { 414,  16 },
  { 415,  16 },
  { 420,  -1 },
  { 421,  -1 },
  { 422,   6 },
  { 423,  -1 },
  { 424,   6 },
  { 425,   6 },
  { 426,   6 },
  { 7001, 17 },
  { 7002, -1 },
  { 7030, -1 },
  { 7031, -1 },
  { 7032, -1 },
  { 7033, -1 },
  { 7034, -1 },
  { 7035, -1 },
  { 7036, -1 },
  { 7037, -1 },
  { 7038, -1 },
  { 7039, -1 },
  { 8001, 18 },
  { 8002, -1 },
  { 8003, -1 },
  { 8004, -1 },
  { 8005, 10 },
  { 8006, 22 },
  { 8007, -1 },
  { 8008, -1 },
  { 8018, 22 },
  { 8020, -1 },
  { 8100, 10 },
  { 8101, 14 },
  { 8102,  6 },
  { 90,   -1 },
  { 91,   -1 },
  { 92,   -1 },
  { 93,   -1 },
  { 94,   -1 },
  { 95,   -1 },
  { 96,   -1 },
  { 97,   -1 },
  { 98,   -1 },
  { 99,   -1 }
  };

static int Code128GetAILength(int ai)
{
  int len = 0;
  if (ai >= 3100 && ai < 3700)
  {
    len = 10;
  }
  else
  {
    size_t aiCount = WXSIZEOF(code128_ailist);
    if (ai >= code128_ailist[0].ai && ai <= code128_ailist[aiCount-1].ai)
    {
      size_t lo = 0;
      size_t hi = aiCount-1;
      size_t n;
      while (lo < hi)
      {
        n = (lo + hi) / 2;
        if (ai < code128_ailist[n].ai)
        {
          hi = n;
        }
        else if (ai > code128_ailist[n].ai)
        {
          lo = n;
        }
        else
        {
          len = code128_ailist[n].len;
          break;
        }
      }
    }
  }
  return len;
}

// Code128 public methods

bool
wxPdfBarCodeCreator::Code128A(double x, double y, const wxString& barcode, double h, double w)
{
  // Check whether barcode text is valid
  wxString::const_iterator ch;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    if (!Code128ValidInCodeSetA(*ch))
    {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::Code128A: ")) +
                 wxString::Format(_("There are illegal characters for Code128A in '%s'."), barcode.c_str()));
      return false;
    }
  }
  wxString bcode = CODE128_START_A;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    switch ((*ch).GetValue())
    {
      case CODE128_FNC1:
        bcode += CODE128_FNC1_INDEX;
        break;
      case CODE128_FNC2:
        bcode += CODE128_FNC2_INDEX;
        break;
      case CODE128_FNC3:
        bcode += CODE128_FNC3_INDEX;
        break;
      case CODE128_FNC4:
        bcode += CODE128_CODE_TO_A;
        break;
      default:
        if (*ch < 32)
          bcode += wxUniChar((int)(*ch) + 64);
        else
          bcode += wxUniChar((int)(*ch) - 32);
        break;
    }
  }
  Code128AddCheck(bcode);
  Code128Draw(x, y, bcode, h, w);
  return true;
}

bool
wxPdfBarCodeCreator::Code128B(double x, double y, const wxString& barcode, double h, double w)
{
  // Check whether barcode text is valid
  wxString::const_iterator ch;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    if (!Code128ValidInCodeSetB(*ch))
    {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::Code128B: ")) +
                 wxString::Format(_("There are illegal characters for Code128B in '%s'."), barcode.c_str()));
      return false;
    }
  }
  wxString bcode = CODE128_START_B;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    switch ((*ch).GetValue())
    {
      case CODE128_FNC1:
        bcode += CODE128_FNC1_INDEX;
        break;
      case CODE128_FNC2:
        bcode += CODE128_FNC2_INDEX;
        break;
      case CODE128_FNC3:
        bcode += CODE128_FNC3_INDEX;
        break;
      case CODE128_FNC4:
        bcode += CODE128_CODE_TO_B;
        break;
      default:
        bcode += wxUniChar((int)(*ch) - 32);
        break;
    }
  }
  Code128AddCheck(bcode);
  Code128Draw(x, y, bcode, h, w);
  return true;
}

bool
wxPdfBarCodeCreator::Code128C(double x, double y, const wxString& barcode, double h, double w)
{
  // Check whether barcode text is valid
  if (barcode.length() % 2 != 0)
  {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::Code128C: ")) +
                 wxString::Format(_("Invalid odd length for Code128C in '%s'."), barcode.c_str()));
      return false;
  }

  wxString::const_iterator ch;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    if (!Code128ValidInCodeSetC(*ch))
    {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::Code128C: ")) +
                 wxString::Format(_("There are illegal characters for Code128C in '%s'."), barcode.c_str()));
      return false;
    }
  }
  wxString bcode = CODE128_START_C;
  size_t index = 0;
  while (index < barcode.length())
  {
    bcode += Code128PackDigits(barcode, index, 2);
  }
  Code128AddCheck(bcode);
  Code128Draw(x, y, bcode, h, w);
  return true;
}

bool
wxPdfBarCodeCreator::Code128(double x, double y, const wxString& barcode, double h, double w)
{
  // Check whether barcode text is valid
  wxString::const_iterator ch;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    if (!Code128ValidChar(*ch))
    {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::Code128: ")) +
                 wxString::Format(_("There are illegal characters for Code128 in '%s'."), barcode.c_str()));
      return false;
    }
  }

  bool ucc = false;
  wxString bcode = Code128MakeCode(barcode, ucc);
  size_t len = bcode.length();
  if (len == 0) return false;

  Code128AddCheck(bcode);
  Code128Draw(x, y, bcode, h, w);
  return true;
}

bool
wxPdfBarCodeCreator::EAN128(double x, double y, const wxString& barcode, double h, double w)
{
  wxString uccCode = wxEmptyString;
  if (barcode[0] == wxS('('))
  {
    size_t idx = 0;
    while (idx != wxString::npos)
    {
      size_t end = barcode.find(wxS(')'), idx);
      if (end == wxString::npos)
      {
        wxLogError(wxString(wxS("wxPdfBarCodeCreator::EAN128: ")) +
                   wxString::Format(_("Badly formed UCC/EAN-128 string '%s'."), barcode.c_str()));
        return false;
      }
      wxString sai = barcode.SubString(idx+1, end-1);
      if (sai.length() < 2)
      {
        wxLogError(wxString(wxS("wxPdfBarCodeCreator::EAN128: ")) +
                   wxString::Format(_("AI too short (%s)."), sai.c_str()));
        return false;
      }
      int len = 0;
      long ai;
      if (sai.ToLong(&ai))
      {
        len = Code128GetAILength((int) ai);
      }
      if (len == 0)
      {
        wxLogError(wxString(wxS("wxPdfBarCodeCreator::EAN128: ")) +
                   wxString::Format(_("AI not found (%s)."), sai.c_str()));
        return false;
      }
      sai = wxString::Format(wxS("%ld"), ai);
      if (sai.length() == 1)
      {
        sai.Prepend(wxS("0"));
      }
      idx = barcode.find(wxS('('), end);
      size_t next = (idx == wxString::npos) ? barcode.length() : idx;
      uccCode += sai + barcode.SubString(end+1, next-1);
      if (len < 0)
      {
        if (idx != wxString::npos)
        {
          uccCode += CODE128_FNC1;
        }
      }
      else if (next - end - 1 + sai.length() != (size_t) len)
      {
        wxLogError(wxString(wxS("wxPdfBarCodeCreator::EAN128: ")) +
                   wxString::Format(_("Invalid AI length (%s)."), sai.c_str()));
        return false;
      }
    }
  }
  else
  {
    uccCode = barcode;
  }

  // Check whether barcode text is valid
  wxString::const_iterator ch;
  for (ch = uccCode.begin(); ch != uccCode.end(); ++ch)
  {
    if (!Code128ValidChar(*ch))
    {
      wxLogError(wxString(wxS("wxPdfBarCodeCreator::EAN128: ")) +
                 wxString::Format(_("There are illegal characters for EAN128 in '%s'."), barcode.c_str()));
      return false;
    }
  }

  bool ucc = true;
  wxString bcode = Code128MakeCode(uccCode, ucc);
  size_t len = bcode.length();
  if (len == 0) return false;

  Code128AddCheck(bcode);
  Code128Draw(x, y, bcode, h, w);
  return true;
}

void
wxPdfBarCodeCreator::Code128Draw(double x, double y, const wxString& barcode, double h, double w)
{
  //Draw bars
  double barWidth;
  double xPos = x;
  short* bars;
  size_t j;
  wxString::const_iterator ch;
  for (ch = barcode.begin(); ch != barcode.end(); ++ch)
  {
    bars = code128_bars[(long int) (*ch)];
    for (j = 0; j < 6 && bars[j] != 0; j = j+2)
    {
      barWidth = bars[j] * w;
      m_document->Rect(xPos, y, barWidth, h, wxPDF_STYLE_FILL);
      xPos += (bars[j]+bars[j+1]) * w;
    }
  }
}

bool
wxPdfBarCodeCreator::QRCode(double x, double y, const wxString& barcode, double scale, int version)
{
  wxPdfBarcodeZint bc;
  bc.SetSymbology(wxPdfBarcode::Symbology::SYM_QRCODE);
  bc.SetText(barcode);
  bc.SetShowText(false);
  if (version > 0) bc.SetOption2(version);
  if (bc.HasCompliantHeight(wxPdfBarcode::Symbology::SYM_QRCODE))
  {
    bc.SetCompliantHeight(true);
  }
  if (scale > 0)
  {
    bc.SetScale(scale);
  }
  bc.Render(*m_document, x, y);
  if (bc.GetError() > 0)
  {
    wxLogError(wxString(wxS("wxPdfBarCodeCreator::QRCode: Zint error=")) + bc.GetLastError());
    return false;
  }
  return true;
}

bool
wxPdfBarCodeCreator::QRCodeDotty(double x, double y, const wxString& barcode, double scale, int version)
{
  wxPdfBarcodeZint bc;
  bc.SetSymbology(wxPdfBarcode::Symbology::SYM_QRCODE);
  bc.SetText(barcode);
  bc.SetShowText(false);
  bc.SetDotty(true);
  if (version > 0) bc.SetOption2(version);
  if (bc.HasCompliantHeight(wxPdfBarcode::Symbology::SYM_QRCODE))
  {
    bc.SetCompliantHeight(true);
  }
  if (scale > 0)
  {
    bc.SetScale(scale);
  }
  bc.Render(*m_document, x, y);
  if (bc.GetError() > 0)
  {
    wxLogError(wxString(wxS("wxPdfBarCodeCreator::QRCodeDotty: Zint error=")) + bc.GetLastError());
    return false;
  }
  return true;
}

bool
wxPdfBarCodeCreator::DataMatrix(double x, double y, const wxString& barcode, double scale, int version, int option)
{
  wxPdfBarcodeZint bc;
  bc.SetSymbology(wxPdfBarcode::Symbology::SYM_DATAMATRIX);
  bc.SetText(barcode);
  bc.SetShowText(false);
  if (scale > 0)
  {
    bc.SetScale(scale);
  }
  if (version > 0) bc.SetOption2(version);
  if (option > 0) bc.SetOption3(option);
  bc.Render(*m_document, x, y);
  if (bc.GetError() > 0)
  {
    wxLogError(wxString(wxS("wxPdfBarCodeCreator::DataMatrix: Zint error=")) + bc.GetLastError());
    return false;
  }
  return true;
}

bool
wxPdfBarCodeCreator::MaxiCode(double x, double y, const wxString& primary, const wxString& secondary, double scale)
{
  wxPdfBarcodeZint bc;
  bc.SetSymbology(wxPdfBarcode::Symbology::SYM_MAXICODE);
  bc.SetPrimaryMessage(primary);
  bc.SetText(secondary);
  bc.SetShowText(false);
  if (scale > 0)
  {
    bc.SetScale(scale);
  }
  bc.Render(*m_document, x, y);
  if (bc.GetError() > 0)
  {
    wxLogError(wxString(wxS("wxPdfBarCodeCreator::MaxiCode: Zint error=")) + bc.GetLastError());
    return false;
  }
  return true;
}

bool
wxPdfBarCodeCreator::GenericBarcode(double x, double y, wxPdfBarcodeZint& barcode)
{
  barcode.Render(*m_document, x, y);
  if (barcode.GetError() > 0)
  {
    wxLogError(wxString(wxS("wxPdfBarCodeCreator::GenericBarcode: Zint error=")) + barcode.GetLastError());
    return false;
  }
  return true;
}
