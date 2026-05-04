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
  /// Default constructor
  wxPdfDC();

  /// Constructor initializing the DC with print data
  /**
  * \param printData Printer configuration data
  */
  wxPdfDC(const wxPrintData& printData);

  /// Constructor for creating a template in an existing PDF document
  /**
  * \param pdfDocument Associated PDF document
  * \param templateWidth Width of the template in user units
  * \param templateHeight Height of the template in user units
  */
  wxPdfDC(wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);

  /// Returns the associated PDF document
  /**
  * \return Pointer to the PDF document
  */
  wxPdfDocument* GetPdfDocument();

  /// Sets the resolution for the DC
  /**
  * \param ppi Resolution in pixels per inch
  */
  void SetResolution(int ppi);

  /// Returns the current resolution
  /**
  * \return Resolution in pixels per inch
  */
  int GetResolution() const;

  /// Sets the image type and quality for bitmap output
  /**
  * \param bitmapType The type of the bitmap (e.g., wxBITMAP_TYPE_JPEG)
  * \param quality Compression quality from 0 to 100
  */
  void SetImageType(wxBitmapType bitmapType, int quality = 75);

  /// Sets the map mode style
  /**
  * Map mode style determines how logical coordinates and font sizes are scaled to the PDF page,
  * allowing emulation of platform-specific behaviors or native PDF API scaling.
  * \param style The map mode style to apply
  */
  void SetMapModeStyle(wxPdfMapModeStyle style);

  /// Returns the current map mode style
  /**
  * \return The current map mode style
  */
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
  /// Constructor initializing with owner
  /**
  * \param owner Pointer to the owner wxPdfDC
  */
  wxPdfDCImpl(wxPdfDC *owner);

  /// Constructor initializing with owner and print data
  /**
  * \param owner Pointer to the owner wxPdfDC
  * \param data Printer configuration data
  */
  wxPdfDCImpl(wxPdfDC* owner, const wxPrintData& data);

  /// Constructor for creating a template
  /**
  * \param owner Pointer to the owner wxPdfDC
  * \param pdfDocument Associated PDF document
  * \param templateWidth Width of the template in user units
  * \param templateHeight Height of the template in user units
  */
  wxPdfDCImpl(wxPdfDC* owner, wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);

  /// Constructor initializing with a file
  /**
  * \param owner Pointer to the owner wxPdfDC
  * \param file Output file name
  * \param w Initial page width
  * \param h Initial page height
  */
  wxPdfDCImpl(wxPdfDC* owner, const wxString& file, int w = 300, int h = 200);

  /// Destructor
  virtual ~wxPdfDCImpl();

  /// Initializes the implementation
  void Init();

  /// Returns the associated PDF document
  /**
  * \return Pointer to the PDF document
  */
  wxPdfDocument* GetPdfDocument();

  /// Sets the print data
  /**
  * \param data Printer configuration data
  */
  void SetPrintData(const wxPrintData& data);

  /// Returns the print data
  /**
  * \return Reference to the print data
  */
  wxPrintData& GetPrintData() { return m_printData; }

  /// Sets the resolution
  /**
  * \param ppi Resolution in pixels per inch
  */
  void SetResolution(int ppi);

  /// Returns the resolution
  /**
  * \return Resolution in pixels per inch
  */
  virtual int GetResolution() const wxOVERRIDE;

  /// Sets the image type and quality
  /**
  * \param bitmapType The type of the bitmap
  * \param quality Compression quality from 0 to 100
  */
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

#if wxUSE_GRAPHICS_CONTEXT
  /// Returns a wxGraphicsContext that draws into the same PDF document
  /// as this DC. Lazily created on first call; the DC retains ownership
  /// of both the context and the underlying wxPdfDocument.
  virtual wxGraphicsContext* GetGraphicsContext() const wxOVERRIDE;
  virtual void SetGraphicsContext(wxGraphicsContext* ctx) wxOVERRIDE;
#endif // wxUSE_GRAPHICS_CONTEXT

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

  void SetupPen(bool force = false);
  void SetupBrush(bool force = false);
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
  wxPen            m_pdfPenSaved;
  wxBrush          m_pdfBrushSaved;

  bool m_jpegFormat;
  int  m_jpegQuality;

  static int    ms_imageCount;

#if wxUSE_GRAPHICS_CONTEXT
  // Lazily-allocated GC that draws into m_pdfDocument. Owned by this DC;
  // mutable so the const GetGraphicsContext() accessor can build it on
  // first use, matching the pattern in wxWindowsDCImpl etc.
  mutable wxGraphicsContext* m_graphicContext;
#endif

  wxDECLARE_DYNAMIC_CLASS(wxPdfDCImpl);
};

#endif
