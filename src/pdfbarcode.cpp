///////////////////////////////////////////////////////////////////////////////
// Name:        pdfbarcode.cpp
// Purpose:     Implementation of wxPdfBarCodeCreator
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-09-12
// Copyright:   (c) Ulrich Telle
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

#include "wx/pdfdoc.h"
#include "wx/pdfbarcode.h"

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

wxChar
wxPdfBarCodeCreator::GetCheckDigit(const wxString& barcode)
{
  //Compute the check digit
  int i, digit, r;
  int sum = 0;
  for ( i = 1; i <= 11; i += 2)
  {
    digit = barcode[i] - _T('0');
    sum += 3 * digit;
  }
  for (i = 0; i <= 10; i += 2)
  {
    digit = barcode[i] - _T('0');
    sum += digit;
  }
  r = sum % 10;
  if (r > 0)
  {
    r = 10 - r;
  }
  wxChar rChar = _T('0') + r;
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
    digit = barcode[i] - _T('0');
    sum += 3 * digit;
  }
  for (i = 0; i <= 10; i += 2)
  {
    digit = barcode[i] - _T('0');
    sum += digit;
  }
  digit = barcode[12] - _T('0');
  return (sum + digit) % 10 == 0;
}

bool
wxPdfBarCodeCreator::Barcode(double x, double y, const wxString& barcode, double h, double w, int len)
{
  //Padding
  int padlen = len - 1 - barcode.Length();
  wxString locBarcode = barcode;
  locBarcode.Pad(padlen, _T('0'), false); //str_pad($barcode,$len-1,'0',STR_PAD_LEFT);
  if (len == 12)
  {
    locBarcode = _T("0") + locBarcode;
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
  static wxString codes[3][10] = { 
    {
      _T("0001101"),_T("0011001"),_T("0010011"),_T("0111101"),_T("0100011"),
      _T("0110001"),_T("0101111"),_T("0111011"),_T("0110111"),_T("0001011")
    },
    {
      _T("0100111"),_T("0110011"),_T("0011011"),_T("0100001"),_T("0011101"),
      _T("0111001"),_T("0000101"),_T("0010001"),_T("0001001"),_T("0010111")
    },
    {
      _T("1110010"),_T("1100110"),_T("1101100"),_T("1000010"),_T("1011100"),
      _T("1001110"),_T("1010000"),_T("1000100"),_T("1001000"),_T("1110100")
    } };
  static int parities[10][6] = {
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
  wxString code = _T("101");
  int digit = locBarcode[0] - _T('0');
  int* p = parities[digit];
  size_t i;
  for (i = 1; i <= 6; i++)
  {
    digit = locBarcode[i] - _T('0');
    code += codes[p[i-1]][digit];
  }
  code += _T("01010");
  for (i = 7; i <= 12; i++)
  {
    digit = locBarcode[i] - _T('0');
    code += codes[2][digit];
  }
  code += _T("101");
  //Draw bars
  for (i = 0; i < code.Length(); i++)
  {
    if (code[i] == _T('1'))
    {
      m_document->Rect(x + i * w, y, w, h, wxPDF_STYLE_FILL);
    }
  }
  //Print text under barcode
  m_document->SetFont(_T("Arial"), _T(""), 12);
  m_document->Text(x, y + h + 11 / m_document->GetScaleFactor(), locBarcode.Right(len));
  return true;
}

bool
wxPdfBarCodeCreator::Code39(double x, double y, const wxString& code, bool ext, bool cks, double w, double h, bool wide)
{
  static wxString chars = _T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. *$/+%");
  wxString locCode = code;
  //Display code
  m_document->SetFont(_T("Arial"), _T(""), 10);
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
      valid = valid && locCode[j] != _T('*') && chars.Find(locCode[j]) >= 0;
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
  locCode = _T("*") + locCode + _T("*");

  //Conversion tables
  static wxString narrowEncoding[] = {
    _T("101001101101"), _T("110100101011"), _T("101100101011"),
    _T("110110010101"), _T("101001101011"), _T("110100110101"),
    _T("101100110101"), _T("101001011011"), _T("110100101101"),
    _T("101100101101"), _T("110101001011"), _T("101101001011"),
    _T("110110100101"), _T("101011001011"), _T("110101100101"),
    _T("101101100101"), _T("101010011011"), _T("110101001101"),
    _T("101101001101"), _T("101011001101"), _T("110101010011"),
    _T("101101010011"), _T("110110101001"), _T("101011010011"),
    _T("110101101001"), _T("101101101001"), _T("101010110011"),
    _T("110101011001"), _T("101101011001"), _T("101011011001"),
    _T("110010101011"), _T("100110101011"), _T("110011010101"),
    _T("100101101011"), _T("110010110101"), _T("100110110101"),
    _T("100101011011"), _T("110010101101"), _T("100110101101"),
    _T("100101101101"), _T("100100100101"), _T("100100101001"),
    _T("100101001001"), _T("101001001001") };

  static wxString wideEncoding[] = {
    _T("101000111011101"), _T("111010001010111"), _T("101110001010111"),
    _T("111011100010101"), _T("101000111010111"), _T("111010001110101"),
    _T("101110001110101"), _T("101000101110111"), _T("111010001011101"),
    _T("101110001011101"), _T("111010100010111"), _T("101110100010111"),
    _T("111011101000101"), _T("101011100010111"), _T("111010111000101"),
    _T("101110111000101"), _T("101010001110111"), _T("111010100011101"),
    _T("101110100011101"), _T("101011100011101"), _T("111010101000111"),
    _T("101110101000111"), _T("111011101010001"), _T("101011101000111"),
    _T("111010111010001"), _T("101110111010001"), _T("101010111000111"),
    _T("111010101110001"), _T("101110101110001"), _T("101011101110001"),
    _T("111000101010111"), _T("100011101010111"), _T("111000111010101"),
    _T("100010111010111"), _T("111000101110101"), _T("100011101110101"),
    _T("100010101110111"), _T("111000101011101"), _T("100011101011101"),
    _T("100010111011101"), _T("100010001000101"), _T("100010001010001"),
    _T("100010100010001"), _T("101000100010001") };

  wxString* encoding = wide ? wideEncoding : narrowEncoding;

  //Inter-character spacing
  wxString gap = (w > 0.29) ? _T("00") : _T("0");

  //Convert to bars
  wxString encode = _T("");
  size_t i;
  for (i = 0; i< locCode.Length(); i++)
  {
    int pos = chars.Find(locCode[i]);
    encode += encoding[pos] + gap;
  }

  //Draw bars
  DrawCode39(encode, x, y, w, h);
  return true;
}


wxChar
wxPdfBarCodeCreator::ChecksumCode39(const wxString& code)
{

  //Compute the modulo 43 checksum

  static wxString chars = _T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. $/+%");
  int sum = 0;
  size_t i;
  for (i = 0; i < code.Length(); i++)
  {
    sum += chars.Find(code[i]);
  }
  int r = sum % 43;
  return chars[r];
}

wxString
wxPdfBarCodeCreator::EncodeCode39Ext(const wxString& code)
{

  //Encode characters in extended mode
  static wxString encode[] = {
    _T("%U"), _T("$A"), _T("$B"), _T("$C"),
    _T("$D"), _T("$E"), _T("$F"), _T("$G"),
    _T("$H"), _T("$I"), _T("$J"), _T("$K"),
    _T("$L"), _T("$M"), _T("$N"), _T("$O"),
    _T("$P"), _T("$Q"), _T("$R"), _T("$S"),
    _T("$T"), _T("$U"), _T("$V"), _T("$W"),
    _T("$X"), _T("$Y"), _T("$Z"), _T("%A"),
    _T("%B"), _T("%C"), _T("%D"), _T("%E"),
    _T(" "),  _T("/A"), _T("/B"), _T("/C"),
    _T("/D"), _T("/E"), _T("/F"), _T("/G"),
    _T("/H"), _T("/I"), _T("/J"), _T("/K"),
    _T("/L"), _T("-"),  _T("."),  _T("/O"),
    _T("0"),  _T("1"),  _T("2"),  _T("3"),
    _T("4"),  _T("5"),  _T("6"),  _T("7"),
    _T("8"),  _T("9"),  _T("/Z"), _T("%F"),
    _T("%G"), _T("%H"), _T("%I"), _T("%J"),
    _T("%V"), _T("A"),  _T("B"),  _T("C"),
    _T("D"),  _T("E"),  _T("F"),  _T("G"),
    _T("H"),  _T("I"),  _T("J"),  _T("K"),
    _T("L"),  _T("M"),  _T("N"),  _T("O"),
    _T("P"),  _T("Q"),  _T("R"),  _T("S"),
    _T("T"),  _T("U"),  _T("V"),  _T("W"),
    _T("X"),  _T("Y"),  _T("Z"),  _T("%K"),
    _T("%L"), _T("%M"), _T("%N"), _T("%O"),
    _T("%W"), _T("+A"), _T("+B"), _T("+C"),
    _T("+D"), _T("+E"), _T("+F"), _T("+G"),
    _T("+H"), _T("+I"), _T("+J"), _T("+K"),
    _T("+L"), _T("+M"), _T("+N"), _T("+O"),
    _T("+P"), _T("+Q"), _T("+R"), _T("+S"),
    _T("+T"), _T("+U"), _T("+V"), _T("+W"),
    _T("+X"), _T("+Y"), _T("+Z"), _T("%P"),
    _T("%Q"), _T("%R"), _T("%S"), _T("%T") };

  wxString codeExt = _T("");
  size_t i;
  for (i = 0 ; i < code.Length(); i++)
  {
    codeExt += encode[code[i]];
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
    if (code[i] == _T('1'))
    {
      m_document->Rect(x + i * w, y, w, h, wxPDF_STYLE_FILL);
    }
  }
}

bool
wxPdfBarCodeCreator::I25(double xpos, double ypos, const wxString& code, double basewidth, double height)
{
  // wide/narrow codes for the digits
  static wxString chars = _T("0123456789AZ");
  static wxString barChar[] = {
    _T("nnwwn"), _T("wnnnw"), _T("nwnnw"), _T("wwnnn"), _T("nnwnw"), 
    _T("wnwnn"), _T("nwwnn"), _T("nnnww"), _T("wnnwn"), _T("nwnwn"),
    _T("nn"), _T("wn") };
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
    locCode = _T("0") + locCode;
  }

  m_document->SetFont(_T("Arial"), _T(""), 10);
  m_document->Text(xpos, ypos + height + 4, locCode);
  m_document->SetFillColor(0);

  // add start and stop codes
  locCode = _T("AA") + locCode + _T("ZA");

  size_t i;
  for (i = 0; i < locCode.Length(); i += 2)
  {
    // choose next pair of digits
    int digitBar = chars.Find(locCode[i]);
    int digitSpace = chars.Find(locCode[i+1]);

    // create a wide/narrow-sequence (first digit=bars, second digit=spaces)
    wxString seq = _T("");
    size_t j;
    for (j = 0; j < barChar[digitBar].Length(); j++)
    {
      seq += wxString(barChar[digitBar][j]) + wxString(barChar[digitSpace][j]);
    }
    for (j = 0; j < seq.Length(); j++)
    {
      // set lineWidth depending on value
      lineWidth = (seq[j] == _T('n')) ? narrow : wide;
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
      digit = zipcode[i] - _T('0');
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
      if ((i != 5 && !wxIsdigit(zipcode[i])) || (i == 5 && zipcode[5] != _T('-')))
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
      sum += (zipcode[i] - _T('0'));
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

// Takes a digit and draws the corresponding POSTNET bars.
void
wxPdfBarCodeCreator::ZipCodeDrawDigitBars(double x, double y, double barSpacing,
                                           double halfBarHeight, double fullBarHeight,
                                           int digit)
{
  // 1 represents full-height bars and 0 represents half-height bars
  static int barDefinitions[10][5] = {
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
      
  // check for invalid digit
  if (digit >= 0 && digit <= 9)
  {
    // draw the five bars representing a digit
    int i;
    for (i = 0; i < 5; i++)
    {
      if (barDefinitions[digit][i] == 1)
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

