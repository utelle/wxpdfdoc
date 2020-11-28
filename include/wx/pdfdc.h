///////////////////////////////////////////////////////////////////////////////
// Name:        pdfdc.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2010-11-28
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfdc.h Interface of the wxPdfDC class

#ifndef _PDF_DC_H_
#define _PDF_DC_H_

#include <wx/cmndata.h>
#include <wx/dc.h>

#include <stack>

#include "wx/pdfdocument.h"
#include "wx/pdffont.h"

/// Enumeration of map mode styles
enum wxPdfMapModeStyle
{
  wxPDF_MAPMODESTYLE_STANDARD = 1,
  wxPDF_MAPMODESTYLE_MSW,
  wxPDF_MAPMODESTYLE_GTK,
  wxPDF_MAPMODESTYLE_MAC,
  /// same font sizes and text position as with the wxPdfDocument API
  wxPDF_MAPMODESTYLE_PDF,
  /// same font sizes as with the wxPdfDocument API
  wxPDF_MAPMODESTYLE_PDFFONTSCALE
};

/// Class representing a PDF drawing context
class WXDLLIMPEXP_PDFDOC wxPdfDC : public wxDC
{
public:
  wxPdfDC();

  // Recommended constructor
  wxPdfDC(const wxPrintData& printData);

  wxPdfDC(wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);

  wxPdfDocument* GetPdfDocument();

  void SetResolution(int ppi);
  int GetResolution() const;

  void SetImageType(wxBitmapType bitmapType, int quality = 75);

  void SetMapModeStyle(wxPdfMapModeStyle style);
  wxPdfMapModeStyle GetMapModeStyle() const;

private:
  wxDECLARE_DYNAMIC_CLASS(wxPdfDC);
};

#if wxCHECK_VERSION(3,1,0)
#else
#define wxOVERRIDE
#endif

/// Class representing the PDF drawing context implementation
class WXDLLIMPEXP_PDFDOC wxPdfDCImpl: public wxDCImpl
{
public:
  wxPdfDCImpl(wxPdfDC *owner);
  wxPdfDCImpl(wxPdfDC* owner, const wxPrintData& data);
  wxPdfDCImpl(wxPdfDC* owner, wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);
  wxPdfDCImpl(wxPdfDC* owner, const wxString& file, int w = 300, int h = 200);
  virtual ~wxPdfDCImpl();

  void Init();

  wxPdfDocument* GetPdfDocument();
  void SetPrintData(const wxPrintData& data);
  wxPrintData& GetPrintData() { return m_printData; }

  void SetResolution(int ppi);
  int GetResolution() const;

  void SetImageType(wxBitmapType bitmapType, int quality = 75);

  // implement base class pure virtuals

  virtual void Clear() wxOVERRIDE;
  virtual bool StartDoc(const wxString& message) wxOVERRIDE;
  virtual void EndDoc() wxOVERRIDE;
  virtual void StartPage() wxOVERRIDE;
  virtual void EndPage() wxOVERRIDE;
  virtual void SetFont(const wxFont& font) wxOVERRIDE;
  virtual void SetPen(const wxPen& pen) wxOVERRIDE;
  virtual void SetBrush(const wxBrush& brush) wxOVERRIDE;
  virtual void SetBackground(const wxBrush& brush) wxOVERRIDE;
  virtual void SetBackgroundMode(int mode) wxOVERRIDE;
#if wxUSE_PALETTE 
  virtual void SetPalette(const wxPalette& palette) wxOVERRIDE;
#endif // wxUSE_PALETTE

  virtual void DestroyClippingRegion() wxOVERRIDE;

  virtual wxCoord GetCharHeight() const wxOVERRIDE;
  virtual wxCoord GetCharWidth() const wxOVERRIDE;

  virtual bool CanDrawBitmap() const wxOVERRIDE;
  virtual bool CanGetTextExtent() const wxOVERRIDE;
  virtual int GetDepth() const wxOVERRIDE;
  virtual wxSize GetPPI() const wxOVERRIDE;

  virtual void SetMapMode(wxMappingMode mode) wxOVERRIDE;
  virtual void SetUserScale(double x, double y) wxOVERRIDE;

  virtual void SetLogicalScale(double x, double y) wxOVERRIDE;
  virtual void SetLogicalOrigin(wxCoord x, wxCoord y) wxOVERRIDE;
  virtual void SetDeviceOrigin(wxCoord x, wxCoord y) wxOVERRIDE;
  virtual void SetAxisOrientation(bool xLeftRight, bool yBottomUp) wxOVERRIDE;

#if wxUSE_DC_TRANSFORM_MATRIX
  virtual bool CanUseTransformMatrix() const wxOVERRIDE;
  virtual bool SetTransformMatrix(const wxAffineMatrix2D& matrix) wxOVERRIDE;
  virtual wxAffineMatrix2D GetTransformMatrix() const wxOVERRIDE;
  virtual void ResetTransformMatrix() wxOVERRIDE;
#endif // wxUSE_DC_TRANSFORM_MATRIX

  virtual void SetLogicalFunction(wxRasterOperationMode function) wxOVERRIDE;

  virtual void SetTextForeground(const wxColour& colour) wxOVERRIDE;
  virtual void ComputeScaleAndOrigin() wxOVERRIDE;

#if 0
  // RTL related functions
  // ---------------------

  // get or change the layout direction (LTR or RTL) for this dc,
  // wxLayout_Default is returned if layout direction is not supported
  virtual wxLayoutDirection GetLayoutDirection() const
        { return wxLayout_Default; }
  virtual void SetLayoutDirection(wxLayoutDirection WXUNUSED(dir))
       { }
#endif

protected:
  // the true implementations
  virtual bool DoFloodFill(wxCoord x, wxCoord y, const wxColour& col,
                           wxFloodFillStyle style = wxFLOOD_SURFACE) wxOVERRIDE;

  virtual void DoGradientFillLinear(const wxRect& rect,
                                    const wxColour& initialColour,
                                    const wxColour& destColour,
                                    wxDirection nDirection = wxEAST) wxOVERRIDE;

  virtual void DoGradientFillConcentric(const wxRect& rect,
                                        const wxColour& initialColour,
                                        const wxColour& destColour,
                                        const wxPoint& circleCenter) wxOVERRIDE;

  virtual bool DoGetPixel(wxCoord x, wxCoord y, wxColour* col) const wxOVERRIDE;

  virtual void DoDrawPoint(wxCoord x, wxCoord y) wxOVERRIDE;

#if wxUSE_SPLINES
  virtual void DoDrawSpline(const wxPointList* points) wxOVERRIDE;
#endif

  virtual void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2) wxOVERRIDE;

  virtual void DoDrawArc(wxCoord x1, wxCoord y1,
                         wxCoord x2, wxCoord y2,
                         wxCoord xc, wxCoord yc) wxOVERRIDE;

  virtual void DoDrawCheckMark(wxCoord x, wxCoord y,
                               wxCoord width, wxCoord height) wxOVERRIDE;

  virtual void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                                 double sa, double ea) wxOVERRIDE;

  virtual void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height) wxOVERRIDE;
  virtual void DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                      wxCoord width, wxCoord height,
                                      double radius) wxOVERRIDE;
  virtual void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height) wxOVERRIDE;

  virtual void DoCrossHair(wxCoord x, wxCoord y) wxOVERRIDE;

  virtual void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y) wxOVERRIDE;
  virtual void DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y,
                            bool useMask = false) wxOVERRIDE;

  virtual void DoDrawText(const wxString& text, wxCoord x, wxCoord y) wxOVERRIDE;
  virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                                 double angle) wxOVERRIDE;

  virtual bool DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                      wxDC* source, wxCoord xsrc, wxCoord ysrc,
                      wxRasterOperationMode rop = wxCOPY, bool useMask = false,
                      wxCoord xsrcMask = -1, wxCoord ysrcMask = -1) wxOVERRIDE;

  virtual void DoGetSize(int* width, int* height) const wxOVERRIDE;
  virtual void DoGetSizeMM(int* width, int* height) const wxOVERRIDE;

  virtual void DoDrawLines(int n, const wxPoint points[],
                           wxCoord xoffset, wxCoord yoffset) wxOVERRIDE;
  virtual void DoDrawPolygon(int n, const wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset,
                             wxPolygonFillMode fillStyle = wxODDEVEN_RULE) wxOVERRIDE;
  virtual void DoDrawPolyPolygon(int n, const int count[], const wxPoint points[],
                                 wxCoord xoffset, wxCoord yoffset,
                                 wxPolygonFillMode fillStyle) wxOVERRIDE;

  virtual void DoSetClippingRegionAsRegion(const wxRegion& region);
  virtual void DoSetClippingRegion(wxCoord x, wxCoord y,
                                   wxCoord width, wxCoord height) wxOVERRIDE;
  virtual void DoSetDeviceClippingRegion(const wxRegion& region) wxOVERRIDE;

  virtual void DoGetTextExtent(const wxString& string,
                               wxCoord* x, wxCoord* y,
                               wxCoord* descent = NULL,
                               wxCoord* externalLeading = NULL,
                               const wxFont* theFont = NULL) const wxOVERRIDE;

  virtual bool DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const wxOVERRIDE;

public:
  int GetDrawingStyle();
  bool StretchBlt(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                  wxBitmap* bitmap);
  int IncreaseImageCounter();

  void SetMapModeStyle(wxPdfMapModeStyle style) { m_mappingModeStyle = style; }
  wxPdfMapModeStyle GetMapModeStyle() const { return m_mappingModeStyle; }

  double ScaleLogicalToPdfX(wxCoord x) const;
  double ScaleLogicalToPdfXRel(wxCoord x) const;
  double ScaleLogicalToPdfY(wxCoord y) const;
  double ScaleLogicalToPdfYRel(wxCoord y) const;
  double ScaleFontSizeToPdf(int pointSize) const;
  int ScalePdfToFontMetric(double metric) const;

private:
  int FindPdfFont(wxFont* font) const;

  bool MustSetCurrentPen(const wxPen& currentPen) const;
  bool MustSetCurrentBrush(const wxBrush& currentBrush) const;

  void SetupPen();
  void SetupBrush();
  void SetupAlpha();
  void SetupTextAlpha();
  void CalculateFontMetrics(wxPdfFontDescription* desc, int pointSize,
                            int* height, int* ascent, int* descent, int* extLeading) const;

  bool           m_templateMode;
  double         m_templateWidth;
  double         m_templateHeight;
  double         m_ppi;
  double         m_ppiPdfFont;
  wxPdfDocument* m_pdfDocument;
  wxPrintData    m_printData;
  wxPdfMapModeStyle m_mappingModeStyle;

  wxPdfColour   m_cachedPdfColour;
  wxUint32      m_cachedRGB;
  wxPen         m_pdfPen;
  wxBrush       m_pdfBrush;

  bool             m_inTransform;
  wxAffineMatrix2D m_matrix;

  bool m_jpegFormat;
  int  m_jpegQuality;

  static int    ms_imageCount;

  wxDECLARE_DYNAMIC_CLASS(wxPdfDCImpl);
};

#endif
