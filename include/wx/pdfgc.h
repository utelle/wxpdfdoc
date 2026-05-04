///////////////////////////////////////////////////////////////////////////////
// Name:        pdfgc.h
// Purpose:     Interface of wxPdfGraphicsContext / wxPdfGraphicsRenderer
// Author:      Ulrich Telle, Blake Madden
// Created:     2012-11-25
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfgc.h Interface of the wxPdfGraphicsContext and wxPdfGraphicsRenderer classes

#ifndef _PDF_GC_H_
#define _PDF_GC_H_

#include <wx/defs.h>

#if wxUSE_GRAPHICS_CONTEXT

#include <wx/affinematrix2d.h>
#include <wx/cmndata.h>
#include <wx/graphics.h>
#include <wx/vector.h>

#include "wx/pdfdocdef.h"
#include "wx/pdfshape.h"

class WXDLLIMPEXP_FWD_PDFDOC wxPdfDocument;
class WXDLLIMPEXP_FWD_PDFDOC wxPdfDC;

/// wxGraphicsContext that renders into a wxPdfDocument.
///
/// Instances are normally obtained via
/// wxPdfGraphicsRenderer::GetPdfRenderer() or
/// wxPdfDC::GetGraphicsContext(). The rendering primitives mirror the
/// standard wxGraphicsContext API and are translated to wxPdfDocument
/// drawing calls.
///
/// @code
/// wxPdfDC dc(printData);
/// if (dc.StartDoc(_("Quarterly Report")))
/// {
///     dc.StartPage();
///     wxGraphicsContext* gc = dc.GetGraphicsContext();
///     if (gc)
///     {
///         // Draw title
///         gc->SetFont(*wxSWISS_FONT, *wxBLACK);
///         gc->DrawText(_("Quarterly Business Report"), 50, 50);
///
///         // Draw a squiggly Bézier underline
///         wxGraphicsPath path = gc->CreatePath();
///         path.MoveToPoint(50, 70);
///         path.AddCurveToPoint(100, 60, 150, 80, 200, 70);
///         path.AddCurveToPoint(250, 60, 300, 80, 350, 70);
///
///         gc->SetPen(wxGraphicsPenInfo(*wxBLUE).Width(2));
///         gc->StrokePath(path);
///     }
///     dc.EndPage();
///     dc.EndDoc();
/// }
/// @endcode
class WXDLLIMPEXP_PDFDOC wxPdfGraphicsContext : public wxGraphicsContext
{
public:
  /// Construct a context that owns a freshly-created wxPdfDocument
  /// configured from the given print data.
  /// @param renderer Pointer to the graphics renderer.
  /// @param data The print data to configure the document.
  wxPdfGraphicsContext(wxGraphicsRenderer* renderer, const wxPrintData& data);

  /// Construct a context that draws into an existing wxPdfDocument as a
  /// template region. The context does not own the document.
  /// @param renderer Pointer to the graphics renderer.
  /// @param pdfDocument Pointer to the existing PDF document.
  /// @param templateWidth The width of the template region.
  /// @param templateHeight The height of the template region.
  wxPdfGraphicsContext(wxGraphicsRenderer* renderer,
                       wxPdfDocument* pdfDocument,
                       double templateWidth,
                       double templateHeight);

  /// Construct an empty (measuring) context with no associated document.
  /// @param renderer Pointer to the graphics renderer.
  wxPdfGraphicsContext(wxGraphicsRenderer* renderer);

  /// Destructor.
  virtual ~wxPdfGraphicsContext();

  // wxGraphicsContext overrides --------------------------------------------

  /// Starts a new document.
  /// @param message The message to associate with the document (often unused in PDF context).
  /// @return true if successful, false otherwise.
  virtual bool StartDoc(const wxString& message) wxOVERRIDE;

  /// Ends the current document, saving it to the file specified in print data.
  virtual void EndDoc() wxOVERRIDE;

  /// Starts a new page in the document.
  /// @param width The width of the page.
  /// @param height The height of the page.
  virtual void StartPage(wxDouble width = 0, wxDouble height = 0) wxOVERRIDE;

  /// Ends the current page.
  virtual void EndPage() wxOVERRIDE {};

  /// Saves the current transformation matrix and clipping region.
  virtual void PushState() wxOVERRIDE;

  /// Restores the previously saved transformation matrix and clipping region.
  virtual void PopState() wxOVERRIDE;

  /// Clips the current graphics context to the given region.
  /// @param region The clipping region.
  virtual void Clip(const wxRegion& region) wxOVERRIDE;

  /// Clips the current graphics context to the given rectangle.
  /// @param x The top-left X coordinate of the clipping rectangle.
  /// @param y The top-left Y coordinate of the clipping rectangle.
  /// @param w The width of the clipping rectangle.
  /// @param h The height of the clipping rectangle.
  virtual void Clip(wxDouble x, wxDouble y, wxDouble w, wxDouble h) wxOVERRIDE;

  /// Clips the current graphics context to the given path.
  /// @param path The clipping path.
  virtual void Clip(const wxGraphicsPath& path);

  /// PDF-specific clip that takes a native shape.
  /// @param shape The shape to clip the drawing context to.
  void Clip(const wxPdfShape& shape);

  /// Resets the clipping region, restoring the entire context.
  virtual void ResetClip() wxOVERRIDE;

  /// Retrieves the bounding box of the current clipping region.
  /// @param[out] x Pointer to receive the top-left X coordinate.
  /// @param[out] y Pointer to receive the top-left Y coordinate.
  /// @param[out] w Pointer to receive the width of the box.
  /// @param[out] h Pointer to receive the height of the box.
  virtual void GetClipBox(wxDouble* x, wxDouble* y, wxDouble* w, wxDouble* h) wxOVERRIDE;

  /// Returns NULL, as the PDF backend has no native graphics context.
  /// @return Always returns NULL.
  virtual void* GetNativeContext() wxOVERRIDE;

  /// Sets the antialiasing mode.
  /// Note that PDF readers, not the producer, control antialiasing.
  /// This method accepts the request, but has no effect on the generated PDF.
  /// @param antialias The antialiasing mode.
  /// @return true if the mode is supported (default/none).
  virtual bool SetAntialiasMode(wxAntialiasMode antialias) wxOVERRIDE;

  /// Sets the interpolation quality (the process of estimating pixel values when resizing images).
  /// @param interpolation The interpolation quality.
  /// @return false, as this is not supported for PDF (PDF renders images at their native resolution).
  virtual bool SetInterpolationQuality(wxInterpolationQuality interpolation) wxOVERRIDE;

  /// Sets the composition mode.
  /// @param op The composition mode.
  /// @return true if supported (source/over blending), false otherwise.
  virtual bool SetCompositionMode(wxCompositionMode op) wxOVERRIDE;

  /// Gets the resolution of the context in DPI.
  /// @param[out] dpiX Pointer to receive the horizontal DPI.
  /// @param[out] dpiY Pointer to receive the vertical DPI.
  virtual void GetDPI(wxDouble* dpiX, wxDouble* dpiY) const wxOVERRIDE;

  /// Begins a new transparency layer.
  /// @param opacity The opacity factor (0.0 to 1.0).
  virtual void BeginLayer(wxDouble opacity) wxOVERRIDE;

  /// Ends the current transparency layer.
  virtual void EndLayer() wxOVERRIDE;

  /// Applies a translation to the transformation matrix.
  /// @param dx The translation in X.
  /// @param dy The translation in Y.
  virtual void Translate(wxDouble dx, wxDouble dy) wxOVERRIDE;

  /// Applies a scaling to the transformation matrix.
  /// @param xScale The scaling factor in X.
  /// @param yScale The scaling factor in Y.
  virtual void Scale(wxDouble xScale, wxDouble yScale) wxOVERRIDE;

  /// Applies a rotation to the transformation matrix.
  /// @param angle The rotation angle in radians.
  virtual void Rotate(wxDouble angle) wxOVERRIDE;

  /// Concatenates the given transformation matrix with the current one.
  /// @param matrix The transformation matrix to concatenate.
  virtual void ConcatTransform(const wxGraphicsMatrix& matrix) wxOVERRIDE;

  /// Sets the current transformation matrix.
  /// @param matrix The transformation matrix to set.
  virtual void SetTransform(const wxGraphicsMatrix& matrix) wxOVERRIDE;

  /// Returns the current transformation matrix.
  /// @return The current transformation matrix.
  virtual wxGraphicsMatrix GetTransform() const wxOVERRIDE;

  /// Sets the current pen.
  /// @param pen The graphics pen.
  virtual void SetPen(const wxGraphicsPen& pen) wxOVERRIDE;

  /// Sets the current brush.
  /// @param brush The graphics brush.
  virtual void SetBrush(const wxGraphicsBrush& brush) wxOVERRIDE;

  /// Sets the current font.
  /// @param font The graphics font.
  virtual void SetFont(const wxGraphicsFont& font) wxOVERRIDE;

  /// Strokes the given path with the current pen.
  /// @param p The path to stroke.
  virtual void StrokePath(const wxGraphicsPath& p) wxOVERRIDE;

  /// Fills the given path with the current brush.
  /// @param p The path to fill.
  /// @param fillStyle The polygon fill rule (@c wxWINDING_RULE or @c wxODDEVEN_RULE).
  virtual void FillPath(const wxGraphicsPath& p,
                        wxPolygonFillMode fillStyle = wxWINDING_RULE) wxOVERRIDE;

  /// Draws the given path (strokes and fills) with current pen and brush.
  /// @param path The path to draw.
  /// @param fillStyle The polygon fill rule.
  virtual void DrawPath(const wxGraphicsPath& path,
                        wxPolygonFillMode fillStyle = wxODDEVEN_RULE) wxOVERRIDE;

  /// Retrieves the extent of the given text.
  /// @param str The string to measure.
  /// @param[out] width Pointer to receive the width.
  /// @param[out] height Pointer to receive the height.
  /// @param[out] descent Pointer to receive the descent.
  /// @param[out] externalLeading Pointer to receive the external leading.
  virtual void GetTextExtent(const wxString& str,
                             wxDouble* width, wxDouble* height,
                             wxDouble* descent, wxDouble* externalLeading) const wxOVERRIDE;

  /// Not implemented for the PDF backend.
  /// @param text The text to measure.
  /// @param widths The array to receive the widths (cleared by this method).
  virtual void GetPartialTextExtents(const wxString& text,
                                     wxArrayDouble& widths) const wxOVERRIDE;

  /// Draws the given bitmap.
  /// @param bmp The graphics bitmap.
  /// @param x The top-left X coordinate.
  /// @param y The top-left Y coordinate.
  /// @param w The width.
  /// @param h The height.
  virtual void DrawBitmap(const wxGraphicsBitmap& bmp,
                          wxDouble x, wxDouble y,
                          wxDouble w, wxDouble h) wxOVERRIDE;

  /// Draws the given bitmap.
  /// @param bmp The bitmap.
  /// @param x The top-left X coordinate.
  /// @param y The top-left Y coordinate.
  /// @param w The width.
  /// @param h The height.
  virtual void DrawBitmap(const wxBitmap& bmp,
                          wxDouble x, wxDouble y,
                          wxDouble w, wxDouble h) wxOVERRIDE;

  /// Draws the given icon.
  /// @param icon The icon.
  /// @param x The top-left X coordinate.
  /// @param y The top-left Y coordinate.
  /// @param w The width.
  /// @param h The height.
  virtual void DrawIcon(const wxIcon& icon,
                        wxDouble x, wxDouble y,
                        wxDouble w, wxDouble h) wxOVERRIDE;

  /// Strokes a line.
  /// @param x1 The start X.
  /// @param y1 The start Y.
  /// @param x2 The end X.
  /// @param y2 The end Y.
  virtual void StrokeLine(wxDouble x1, wxDouble y1,
                          wxDouble x2, wxDouble y2) wxOVERRIDE;

  /// Strokes a series of connected lines.
  /// @param n The number of points.
  /// @param points Pointer to the array of points.
  virtual void StrokeLines(size_t n, const wxPoint2DDouble* points) wxOVERRIDE;

  /// Strokes a series of disjoint lines.
  /// @param n The number of lines.
  /// @param beginPoints Pointer to the start points.
  /// @param endPoints Pointer to the end points.
  virtual void StrokeLines(size_t n,
                           const wxPoint2DDouble* beginPoints,
                           const wxPoint2DDouble* endPoints) wxOVERRIDE;

  /// Draws a series of lines, filling the resulting polygon.
  /// @param n The number of points.
  /// @param points Pointer to the array of points.
  /// @param fillStyle The polygon fill rule.
  virtual void DrawLines(size_t n, const wxPoint2DDouble* points,
                         wxPolygonFillMode fillStyle = wxODDEVEN_RULE) wxOVERRIDE;

  /// Draws a rectangle.
  /// @param x The top-left X coordinate.
  /// @param y The top-left Y coordinate.
  /// @param w The width.
  /// @param h The height.
  virtual void DrawRectangle(wxDouble x, wxDouble y,
                             wxDouble w, wxDouble h) wxOVERRIDE;

  /// Draws an ellipse.
  /// @param x The top-left X coordinate.
  /// @param y The top-left Y coordinate.
  /// @param w The width.
  /// @param h The height.
  virtual void DrawEllipse(wxDouble x, wxDouble y,
                           wxDouble w, wxDouble h) wxOVERRIDE;

  /// Draws a rounded rectangle.
  /// @param x The top-left X coordinate.
  /// @param y The top-left Y coordinate.
  /// @param w The width.
  /// @param h The height.
  /// @param radius The corner radius.
  virtual void DrawRoundedRectangle(wxDouble x, wxDouble y,
                                    wxDouble w, wxDouble h,
                                    wxDouble radius) wxOVERRIDE;

  /// Returns true if coordinate offsetting is needed for pixel alignment.
  /// @return True if offsetting is required, false otherwise.
  virtual bool ShouldOffset() const wxOVERRIDE;

#ifdef __WXMSW__
  /// PDF has no native HDC; nothing to hand over.
  /// @return Always returns NULL.
  virtual WXHDC GetNativeHDC() wxOVERRIDE { return NULL; }

  /// Does nothing, as there is no native HDC to release.
  /// @param hdc The HDC to release (unused).
  virtual void ReleaseNativeHDC(WXHDC WXUNUSED(hdc)) wxOVERRIDE {}
#endif

  // PDF-specific accessors --------------------------------------------------
  
  /// Returns the underlying PDF document. May be NULL until StartDoc().
  /// @return A pointer to the underlying wxPdfDocument instance.
  wxPdfDocument* GetPdfDocument() { return m_pdfDocument; }

  // Track line / fill alpha so that pen and brush Apply()s can update one
  // side without clobbering the other. Both default to 1.0 (fully opaque).
  // Setting either pushes the combined value to wxPdfDocument::SetAlpha().
  
  /// Sets the line alpha transparency.
  /// @param a The alpha value (0.0 for fully transparent to 1.0 for fully opaque).
  void SetLineAlpha(double a);
  
  /// Sets the fill alpha transparency.
  /// @param a The alpha value (0.0 for fully transparent to 1.0 for fully opaque).
  void SetFillAlpha(double a);
  
  /// Gets the current line alpha transparency.
  /// @return The current line alpha value.
  double GetLineAlpha() const { return m_lineAlpha; }
  
  /// Gets the current fill alpha transparency.
  /// @return The current fill alpha value.
  double GetFillAlpha() const { return m_fillAlpha; }

protected:
  virtual void DoDrawText(const wxString& str, wxDouble x, wxDouble y) wxOVERRIDE;

private:
  void Init();
  void SetPrintData(const wxPrintData& data);

  /// Implementation detail used by font metric calculations.
  /// @param desc Pointer to the font description.
  /// @param pointSize The point size of the font.
  /// @param[out] height Pointer to receive the height.
  /// @param[out] ascent Pointer to receive the ascent.
  /// @param[out] descent Pointer to receive the descent.
  /// @param[out] extLeading Pointer to receive the external leading.
  void CalculateFontMetrics(class wxPdfFontDescription* desc, double pointSize,
                            double* height, double* ascent,
                            double* descent, double* extLeading) const;

  bool           m_templateMode;
  double         m_templateWidth;
  double         m_templateHeight;
  double         m_ppi;
  double         m_ppiPdfFont;
  wxPdfDocument* m_pdfDocument;
  int            m_imageCount;
  wxPrintData    m_printData;
  int            m_mappingModeStyle;

  // Active line / fill opacity (0..1). Pushed to wxPdfDocument::SetAlpha
  // whenever either side changes.
  double         m_lineAlpha;
  double         m_fillAlpha;

  // Track the number of q's emitted by Clip() calls within the current
  // PushState/PopState level, so PopState can balance them with Q's.
  int            m_clipCount;
  wxVector<int>  m_clipCountStack;

  // Shadow of the document's current transformation matrix. PDF cannot
  // read its own CTM back, so we keep one here for GetTransform() and
  // for PushState/PopState restore.
  wxAffineMatrix2D m_ctm;
  wxVector<wxAffineMatrix2D> m_ctmStack;

  // Stack of (lineAlpha, fillAlpha) pairs saved on BeginLayer.
  struct AlphaPair { double line; double fill; };
  wxVector<AlphaPair> m_layerAlphaStack;

  wxDECLARE_NO_COPY_CLASS(wxPdfGraphicsContext);
};

/// Renderer that produces wxPdfGraphicsContext instances.
///
/// Use GetPdfRenderer() to obtain the singleton instance and call
/// CreateContextFromPrintData() or CreateContext(wxPdfDC*) to obtain a
/// concrete context.
class WXDLLIMPEXP_PDFDOC wxPdfGraphicsRenderer : public wxGraphicsRenderer
{
public:
  /// Default constructor.
  wxPdfGraphicsRenderer() {}
  /// Destructor.
  virtual ~wxPdfGraphicsRenderer() {}

  /// Returns the process-wide PDF graphics renderer.
  /// @return A pointer to the singleton @c wxGraphicsRenderer.
  static wxGraphicsRenderer* GetPdfRenderer();

  // PDF-specific factories --------------------------------------------------

  /// Creates a context that owns a freshly-built PDF document configured
  /// from \a printData. Caller takes ownership of the returned context.
  /// @param printData The print data to configure the document.
  /// @return A pointer to the created @c wxGraphicsContext.
  wxGraphicsContext* CreateContextFromPrintData(const wxPrintData& printData);

  /// Creates a context that draws into the document already owned by
  /// \a dc. The returned context does not delete the underlying document
  /// when destroyed; the wxPdfDC retains ownership.
  /// @param dc The wxPdfDC to draw into.
  /// @return A pointer to the created @c wxGraphicsContext.
  wxGraphicsContext* CreateContext(wxPdfDC* dc);

  /// Creates a context that draws into the given existing PDF document as
  /// a template region of the given size. The context does not take
  /// ownership of the document.
  /// @param pdfDocument The PDF document to draw into.
  /// @param templateWidth The width of the template region.
  /// @param templateHeight The height of the template region.
  /// @return A pointer to the created @c wxGraphicsContext.
  wxGraphicsContext* CreateContextFromDocument(wxPdfDocument* pdfDocument,
                                               double templateWidth,
                                               double templateHeight);

  // wxGraphicsRenderer overrides -------------------------------------------

  /// Not supported for the PDF renderer.
  /// @param dc The window DC.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContext(const wxWindowDC& dc) wxOVERRIDE;

  /// Not supported for the PDF renderer.
  /// @param dc The memory DC.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContext(const wxMemoryDC& dc) wxOVERRIDE;

#if wxUSE_PRINTING_ARCHITECTURE
  /// Not supported for the PDF renderer.
  /// @param dc The printer DC.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContext(const wxPrinterDC& dc) wxOVERRIDE;
#endif

#ifdef __WXMSW__
#if wxUSE_ENH_METAFILE
  /// Not supported for the PDF renderer.
  /// @param dc The enhanced metafile DC.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContext(const wxEnhMetaFileDC& dc) wxOVERRIDE;
#endif
#endif

  /// Not supported for the PDF renderer.
  /// @param context The native context.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContextFromNativeContext(void* context) wxOVERRIDE;

  /// Not supported for the PDF renderer.
  /// @param window The native window.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContextFromNativeWindow(void* window) wxOVERRIDE;

#ifdef __WXMSW__
  /// Not supported for the PDF renderer.
  /// @param dc The native HDC.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContextFromNativeHDC(WXHDC dc) wxOVERRIDE;
#endif

  /// Not supported for the PDF renderer.
  /// @param window The window.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContext(wxWindow* window) wxOVERRIDE;

#if wxUSE_IMAGE
  /// Not supported for the PDF renderer.
  /// @param image The image.
  /// @return Always returns NULL.
  virtual wxGraphicsContext* CreateContextFromImage(wxImage& image) wxOVERRIDE;
#endif

  /// Creates a document-less context for measuring text and paths.
  /// @return A pointer to the created @c wxGraphicsContext.
  virtual wxGraphicsContext* CreateMeasuringContext() wxOVERRIDE;

  /// Creates a new graphics path.
  /// @return A new @c wxGraphicsPath.
  virtual wxGraphicsPath CreatePath() wxOVERRIDE;

  /// Creates a new graphics matrix.
  /// @param a The 11 component.
  /// @param b The 12 component.
  /// @param c The 21 component.
  /// @param d The 22 component.
  /// @param tx The X translation.
  /// @param ty The Y translation.
  /// @return A new wxGraphicsMatrix.
  virtual wxGraphicsMatrix CreateMatrix(wxDouble a = 1.0, wxDouble b = 0.0,
                                        wxDouble c = 0.0, wxDouble d = 1.0,
                                        wxDouble tx = 0.0, wxDouble ty = 0.0) wxOVERRIDE;

  /// Creates a graphics pen.
  /// @param info The pen info.
  /// @return A new @c wxGraphicsPen.
  virtual wxGraphicsPen CreatePen(const wxGraphicsPenInfo& info) wxOVERRIDE;

  /// Creates a graphics brush.
  /// @param brush The brush.
  /// @return A new @c wxGraphicsBrush.
  virtual wxGraphicsBrush CreateBrush(const wxBrush& brush) wxOVERRIDE;

  /// Creates a linear gradient brush.
  /// Note that the optional transformation matrix is currently ignored.
  /// @param x1 The start X.
  /// @param y1 The start Y.
  /// @param x2 The end X.
  /// @param y2 The end Y.
  /// @param stops The gradient stops.
  /// @param matrix The transformation matrix (ignored).
  /// @return A new @c wxGraphicsBrush.
  virtual wxGraphicsBrush
  CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                            wxDouble x2, wxDouble y2,
                            const wxGraphicsGradientStops& stops,
                            const wxGraphicsMatrix& matrix = wxNullGraphicsMatrix) wxOVERRIDE;

  /// Creates a radial gradient brush.
  /// Note that the optional transformation matrix is currently ignored.
  /// @param xo The start X.
  /// @param yo The start Y.
  /// @param xc The end X.
  /// @param yc The end Y.
  /// @param radius The radius.
  /// @param stops The gradient stops.
  /// @param matrix The transformation matrix (ignored).
  /// @return A new @c wxGraphicsBrush.
  virtual wxGraphicsBrush
  CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                            wxDouble xc, wxDouble yc,
                            wxDouble radius,
                            const wxGraphicsGradientStops& stops,
                            const wxGraphicsMatrix& matrix = wxNullGraphicsMatrix) wxOVERRIDE;

  /// Creates a graphics font.
  /// @param font The font.
  /// @param col The colour.
  /// @return A new @c wxGraphicsFont.
  virtual wxGraphicsFont CreateFont(const wxFont& font,
                                    const wxColour& col = *wxBLACK) wxOVERRIDE;

  /// Creates a graphics font.
  /// @param sizeInPixels The size in pixels.
  /// @param facename The font face name.
  /// @param flags The font flags.
  /// @param col The colour.
  /// @return A new @c wxGraphicsFont.
  virtual wxGraphicsFont CreateFont(double sizeInPixels,
                                    const wxString& facename,
                                    int flags = wxFONTFLAG_DEFAULT,
                                    const wxColour& col = *wxBLACK) wxOVERRIDE;

  /// Creates a graphics font at a specific DPI.
  /// Note that PDF user space is fixed at 72 DPI; the requested DPI is ignored.
  /// @param font The font.
  /// @param dpi The DPI.
  /// @param col The colour.
  /// @return A new @c wxGraphicsFont.
  virtual wxGraphicsFont CreateFontAtDPI(const wxFont& font,
                                         const wxRealPoint& dpi,
                                         const wxColour& col = *wxBLACK) wxOVERRIDE;

  /// Creates a graphics bitmap.
  /// @param bitmap The bitmap.
  /// @return A new @c wxGraphicsBitmap.
  virtual wxGraphicsBitmap CreateBitmap(const wxBitmap& bitmap) wxOVERRIDE;

#if wxUSE_IMAGE
  /// Creates a graphics bitmap from an image.
  /// @param image The image.
  /// @return A new @c wxGraphicsBitmap.
  virtual wxGraphicsBitmap CreateBitmapFromImage(const wxImage& image) wxOVERRIDE;

  /// Creates an image from a graphics bitmap.
  /// @param bmp The graphics bitmap.
  /// @return A new @c wxImage.
  virtual wxImage CreateImageFromBitmap(const wxGraphicsBitmap& bmp) wxOVERRIDE;
#endif

  /// Not implemented for the PDF renderer.
  /// @param bitmap The native bitmap.
  /// @return Always returns @c wxNullGraphicsBitmap.
  virtual wxGraphicsBitmap CreateBitmapFromNativeBitmap(void* bitmap) wxOVERRIDE;

  /// Not implemented for the PDF renderer.
  /// @param bitmap The graphics bitmap.
  /// @param x The top-left X.
  /// @param y The top-left Y.
  /// @param w The width.
  /// @param h The height.
  /// @return Always returns @c wxNullGraphicsBitmap.
  virtual wxGraphicsBitmap CreateSubBitmap(const wxGraphicsBitmap& bitmap,
                                           wxDouble x, wxDouble y,
                                           wxDouble w, wxDouble h) wxOVERRIDE;

  /// Returns "pdf" as the renderer name.
  /// @return The name of the renderer.
  virtual wxString GetName() const wxOVERRIDE;
  /// Retrieves the version of the renderer.
  /// @param[out] major Pointer to receive the major version.
  /// @param[out] minor Pointer to receive the minor version.
  /// @param[out] micro Pointer to receive the micro version.
  virtual void GetVersion(int* major, int* minor = NULL, int* micro = NULL) const wxOVERRIDE;

private:
  wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxPdfGraphicsRenderer);
};

#endif // wxUSE_GRAPHICS_CONTEXT

#endif // _PDF_GC_H_
