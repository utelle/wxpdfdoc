///////////////////////////////////////////////////////////////////////////////
// Name:        pdfgc.cpp
// Purpose:     Implementation of wxPdfGraphicsContext
// Author:      Ulrich Telle
// Modified by:
// Created:     2012-11-25
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfgc.cpp Implementation of the wxPdfGraphicsContext

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if wxUSE_GRAPHICS_CONTEXT

#include "wx/graphics.h"

#ifndef WX_PRECOMP
    #include "wx/bitmap.h"
    #include "wx/icon.h"
    #include "wx/dcclient.h"
    #include "wx/dcmemory.h"
    #include "wx/dcprint.h"
    #include "wx/window.h"
#endif

#include "wx/private/graphics.h"
#include "wx/rawbmp.h"
#include "wx/vector.h"

#include <wx/affinematrix2d.h>
#include <wx/region.h>
#include <wx/font.h>
#include <wx/paper.h>

#include "wx/pdfgc.h" 
#include "wx/pdffontmanager.h"
#include "wx/pdflinestyle.h"
#include "wx/pdfshape.h"

#include <math.h>

using namespace std;

//-----------------------------------------------------------------------------
// wxGraphicsPath implementation
//-----------------------------------------------------------------------------

// TODO remove this dependency (gdiplus needs the macros)
// TODO really needed for wxPdfGraphicsContext?

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class WXDLLIMPEXP_FWD_PDFDOC wxPdfGraphicsContext;

class wxPdfGraphicsPathData : public wxGraphicsPathData
{
public :
  wxPdfGraphicsPathData(wxGraphicsRenderer* renderer, const wxPdfShape* path = NULL);
  ~wxPdfGraphicsPathData();

  virtual wxGraphicsObjectRefData* Clone() const;

  //
  // These are the path primitives from which everything else can be constructed
  //

  // begins a new subpath at (x,y)
  virtual void MoveToPoint(wxDouble x, wxDouble y);

  // adds a straight line from the current point to (x,y)
  virtual void AddLineToPoint(wxDouble x, wxDouble y);

  // adds a cubic Bezier curve from the current point, using two control points and an end point
  virtual void AddCurveToPoint(wxDouble cx1, wxDouble cy1, wxDouble cx2, wxDouble cy2, wxDouble x, wxDouble y);

  // adds another path
  virtual void AddPath(const wxGraphicsPathData* path);

  // closes the current sub-path
  virtual void CloseSubpath();

  // gets the last point of the current path, (0,0) if not yet set
  virtual void GetCurrentPoint(wxDouble* x, wxDouble* y) const;

  // adds an arc of a circle centering at (x,y) with radius (r) from startAngle to endAngle
  virtual void AddArc(wxDouble x, wxDouble y, wxDouble r, wxDouble startAngle, wxDouble endAngle, bool clockwise);

  //
  // These are convenience functions which - if not available natively will be assembled
  // using the primitives from above
  //

  /*

  // adds a quadratic Bezier curve from the current point, using a control point and an end point
  virtual void AddQuadCurveToPoint(wxDouble cx, wxDouble cy, wxDouble x, wxDouble y);

  // appends a rectangle as a new closed subpath
  virtual void AddRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h);
    
  // appends an ellipsis as a new closed subpath fitting the passed rectangle
  virtual void AddCircle(wxDouble x, wxDouble y, wxDouble r);

  // draws a an arc to two tangents connecting (current) to (x1,y1) and (x1,y1) to (x2,y2), also a straight line from (current) to (x1,y1)
  virtual void AddArcToPoint(wxDouble x1, wxDouble y1 , wxDouble x2, wxDouble y2, wxDouble r);

  // appends an ellipse
  virtual void AddEllipse(wxDouble x, wxDouble y, wxDouble w, wxDouble h);

  // appends a rounded rectangle
  virtual void AddRoundedRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h, wxDouble radius);

  */

  // returns the native path
  virtual void* GetNativePath() const;

  // give the native path returned by GetNativePath() back (there might be some deallocations necessary)
  virtual void UnGetNativePath(void* p) const;

  // transforms each point of this path by the matrix
  virtual void Transform(const wxGraphicsMatrixData* matrix);

  // gets the bounding box enclosing all points (possibly including control points)
  virtual void GetBox(wxDouble* x, wxDouble* y, wxDouble* w, wxDouble* h) const;

  virtual bool Contains(wxDouble x, wxDouble y, wxPolygonFillMode fillStyle = wxWINDING_RULE) const;

  const wxPdfShape& GetPdfShape() const { return m_path; }

private:
  wxPdfShape m_path;
};

class WXDLLIMPEXP_PDFDOC wxPdfGraphicsMatrixData : public wxGraphicsMatrixData
{
public:
  wxPdfGraphicsMatrixData(wxGraphicsRenderer* renderer, const wxAffineMatrix2D* matrix = NULL);
  virtual ~wxPdfGraphicsMatrixData();

  virtual wxGraphicsObjectRefData *Clone() const;

  // concatenates the matrix
  virtual void Concat(const wxGraphicsMatrixData* t);

  // sets the matrix to the respective values
  virtual void Set(wxDouble a = 1.0, wxDouble b = 0.0, wxDouble c = 0.0, wxDouble d = 1.0, wxDouble tx = 0.0, wxDouble ty = 0.0);

  // gets the component valuess of the matrix
  virtual void Get(wxDouble* a = NULL, wxDouble* b = NULL,  wxDouble* c = NULL, wxDouble* d = NULL, wxDouble* tx = NULL, wxDouble* ty = NULL) const;

  // makes this the inverse matrix
  virtual void Invert();

  // returns true if the elements of the transformation matrix are equal ?
  virtual bool IsEqual(const wxGraphicsMatrixData* t) const;

  // return true if this is the identity matrix
  virtual bool IsIdentity() const;

  //
  // transformation
  //

  // add the translation to this matrix
  virtual void Translate(wxDouble dx, wxDouble dy);

  // add the scale to this matrix
  virtual void Scale(wxDouble xScale, wxDouble yScale);

  // add the rotation to this matrix (radians)
  virtual void Rotate( wxDouble angle );

  //
  // apply the transforms
  //

  // applies that matrix to the point
  virtual void TransformPoint(wxDouble* x, wxDouble* y) const;

  // applies the matrix except for translations
  virtual void TransformDistance(wxDouble* dx, wxDouble* dy) const;

  // returns the native representation
  virtual void* GetNativeMatrix() const;
private:
  wxAffineMatrix2D m_matrix;
};

// Common base class for pens and brushes.
class wxPdfGraphicsPenBrushData : public wxGraphicsObjectRefData
{
public:
  wxPdfGraphicsPenBrushData(wxGraphicsRenderer* renderer,
                            const wxColour& col,
                            bool isTransparent);
  virtual ~wxPdfGraphicsPenBrushData();

  virtual void Apply(wxPdfGraphicsContext* context);

protected:
  // Call this to use the given bitmap as stipple. Bitmap must be non-null
  // and valid.
  void InitStipple(wxBitmap* bmp);

  // Call this to use the given hatch style. Hatch style must be valid.
  void InitHatch(wxHatchStyle hatchStyle);

  wxColour m_colour;

  class wxPdfGraphicsBitmapData* m_bmpdata;

private:
  // Called once to allocate m_pattern if needed.
  void InitHatchPattern();

  wxHatchStyle m_hatchStyle;

  wxDECLARE_NO_COPY_CLASS(wxPdfGraphicsPenBrushData);
};

class wxPdfGraphicsPenData : public wxPdfGraphicsPenBrushData
{
public:
  wxPdfGraphicsPenData(wxGraphicsRenderer* renderer, const wxPen&pen);
  ~wxPdfGraphicsPenData();

  void Init();

  virtual void Apply(wxPdfGraphicsContext* context);
  virtual wxDouble GetWidth() { return m_width; }

private :
  double m_width;

  wxPdfLineStyle m_cap;
  wxPdfLineJoin  m_join;

  int           m_count;
  const double* m_lengths;
  double*       m_userLengths;

  wxDECLARE_NO_COPY_CLASS(wxPdfGraphicsPenData);
};

class wxPdfGraphicsBrushData : public wxPdfGraphicsPenBrushData
{
public:
  wxPdfGraphicsBrushData(wxGraphicsRenderer* renderer);
  wxPdfGraphicsBrushData(wxGraphicsRenderer* renderer, const wxBrush& brush);

  void CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                                 wxDouble x2, wxDouble y2,
                                 const wxGraphicsGradientStops& stops);
  void CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                 wxDouble xc, wxDouble yc, wxDouble radius,
                                 const wxGraphicsGradientStops& stops);

protected:
  virtual void Init();

  // common part of Create{Linear,Radial}GradientBrush()
  void AddGradientStops(const wxGraphicsGradientStops& stops);
};

class wxPdfGraphicsFontData : public wxGraphicsObjectRefData
{
public:
  wxPdfGraphicsFontData(wxGraphicsRenderer* renderer, const wxFont& font, const wxColour& col);
  wxPdfGraphicsFontData(wxGraphicsRenderer* renderer,
                        double sizeInPixels,
                        const wxString& facename,
                        int flags,
                        const wxColour& col);
  ~wxPdfGraphicsFontData();

  virtual bool Apply(wxPdfGraphicsContext* context);

  wxPdfFont GetFont() const { return m_regFont; }
  double GetSize() const { return m_size; }
  wxColour GetColour() const { return m_colour; }
  int GetStyles() const { return m_styles; }

private :
  double    m_size;
  wxColour  m_colour;
  int       m_styles;

  wxPdfFont m_regFont;
  wxFont    m_font;
};

class wxPdfGraphicsBitmapData : public wxGraphicsBitmapData
{
public:
  wxPdfGraphicsBitmapData(wxGraphicsRenderer* renderer, const wxBitmap& bmp);
#if wxUSE_IMAGE
  wxPdfGraphicsBitmapData(wxGraphicsRenderer* renderer, const wxImage& image);
#endif // wxUSE_IMAGE
  ~wxPdfGraphicsBitmapData();

  virtual void* GetNativeBitmap() const { return NULL /*m_surface*/; }
  virtual wxSize GetSize() { return wxSize(m_width, m_height); }

#if wxUSE_IMAGE
  wxImage ConvertToImage() const;
#endif // wxUSE_IMAGE

private :
#if 0
  // Allocate m_buffer for the bitmap of the given size in the given format.
  //
  // Returns the stride used for the buffer.
  int InitBuffer(int width, int height, cairo_format_t format);

  // Really create the surface using the buffer (which was supposed to be
  // filled since InitBuffer() call).
  void InitSurface(cairo_format_t format, int stride);

  cairo_surface_t* m_surface;
  cairo_pattern_t* m_pattern;
#endif
  int m_width;
  int m_height;
  unsigned char* m_buffer;
};

class WXDLLIMPEXP_PDFDOC wxPdfGraphicsContext : public wxGraphicsContext
{
public:
  wxPdfGraphicsContext(wxGraphicsRenderer* renderer, const wxPrintData& data);
  wxPdfGraphicsContext(wxGraphicsRenderer* renderer, wxPdfDocument* pdfDocument, double templateWidth, double templateHeight);
  wxPdfGraphicsContext(wxGraphicsRenderer* renderer);

  virtual ~wxPdfGraphicsContext();

  // begin a new document (relevant only for printing / pdf etc) if there is a progress dialog, message will be shown
  virtual bool StartDoc(const wxString& message);

  // done with that document (relevant only for printing / pdf etc)
  virtual void EndDoc();

  // opens a new page  (relevant only for printing / pdf etc) with the given size in points
  // (if both are null the default page size will be used)
  virtual void StartPage(wxDouble width = 0, wxDouble height = 0);

  // ends the current page  (relevant only for printing / pdf etc)
  virtual void EndPage();

  virtual void PushState();
  virtual void PopState();

  virtual void Clip(const wxRegion &region);

  // clips drawings to the rect
  virtual void Clip(wxDouble x, wxDouble y, wxDouble w, wxDouble h);

  // resets the clipping to original extent
  virtual void ResetClip();

  virtual void* GetNativeContext();

  virtual bool SetAntialiasMode(wxAntialiasMode antialias);

  virtual bool SetInterpolationQuality(wxInterpolationQuality interpolation);

  virtual bool SetCompositionMode(wxCompositionMode op);

  // returns the resolution of the graphics context in device points per inch
  // TODO: required?
  virtual void GetDPI( wxDouble* dpiX, wxDouble* dpiY);
    
  virtual void BeginLayer(wxDouble opacity);

  virtual void EndLayer();

  virtual void Translate(wxDouble dx , wxDouble dy);
  virtual void Scale(wxDouble xScale , wxDouble yScale);
  virtual void Rotate(wxDouble angle);

  // concatenates this transform with the current transform of this context
  virtual void ConcatTransform(const wxGraphicsMatrix& matrix);

  // sets the transform of this context
  virtual void SetTransform(const wxGraphicsMatrix& matrix);

  // gets the matrix of this context
  virtual wxGraphicsMatrix GetTransform() const;

  // sets the pen
  virtual void SetPen( const wxGraphicsPen& pen );

  // sets the brush for filling
  virtual void SetBrush( const wxGraphicsBrush& brush );

  // sets the font
  virtual void SetFont( const wxGraphicsFont& font );

  virtual void StrokePath(const wxGraphicsPath& p);
  virtual void FillPath(const wxGraphicsPath& p, wxPolygonFillMode fillStyle = wxWINDING_RULE);

  // draws a path by first filling and then stroking
  virtual void DrawPath(const wxGraphicsPath& path, wxPolygonFillMode fillStyle = wxODDEVEN_RULE);

  virtual void GetTextExtent(const wxString& str, wxDouble* width, wxDouble* height,
                             wxDouble* descent, wxDouble*externalLeading) const;
  virtual void GetPartialTextExtents(const wxString& text, wxArrayDouble& widths) const;

  virtual void DrawBitmap(const wxGraphicsBitmap& bmp, wxDouble x, wxDouble y, wxDouble w, wxDouble h);
  virtual void DrawBitmap(const wxBitmap& bmp, wxDouble x, wxDouble y, wxDouble w, wxDouble h);
  virtual void DrawIcon(const wxIcon& icon, wxDouble x, wxDouble y, wxDouble w, wxDouble h);

//TODO Begin
  // strokes a single line
  virtual void StrokeLine(wxDouble x1, wxDouble y1, wxDouble x2, wxDouble y2);

  // stroke lines connecting each of the points
  virtual void StrokeLines(size_t n, const wxPoint2DDouble* points);

  // stroke disconnected lines from begin to end points
  virtual void StrokeLines(size_t n, const wxPoint2DDouble* beginPoints, const wxPoint2DDouble* endPoints);

  // draws a polygon
  virtual void DrawLines(size_t n, const wxPoint2DDouble* points, wxPolygonFillMode fillStyle = wxODDEVEN_RULE);

  // draws a rectangle
  virtual void DrawRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h);

  // draws an ellipse
  virtual void DrawEllipse(wxDouble x, wxDouble y, wxDouble w, wxDouble h);

  // draws a rounded rectangle
  virtual void DrawRoundedRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h, wxDouble radius);
//TODO End

  virtual bool ShouldOffset() const
  {
    if (!m_enableOffset)
      return false;
        
    int penwidth = 0 ;
    if (!m_pen.IsNull())
    {
      penwidth = (int)((wxPdfGraphicsPenData*)m_pen.GetRefData())->GetWidth();
      if ( penwidth == 0 )
        penwidth = 1;
    }
    return ( penwidth % 2 ) == 1;
  }

  wxPdfDocument* GetPdfDocument() { return m_pdfDocument; }

protected:
    virtual void DoDrawText( const wxString &str, wxDouble x, wxDouble y );

#if 0
  virtual void DoDrawRotatedText(const wxString& str, wxDouble x, wxDouble y,
                                 wxDouble angle);
  virtual void DoDrawFilledText(const wxString& str, wxDouble x, wxDouble y,
                                const wxGraphicsBrush& backgroundBrush);
  virtual void DoDrawRotatedFilledText(const wxString& str,
                                       wxDouble x, wxDouble y,
                                       wxDouble angle,
                                       const wxGraphicsBrush& backgroundBrush);
#endif

  void Init();
  void SetPrintData(const wxPrintData& data);

  void CalculateFontMetrics(wxPdfFontDescription* desc, double pointSize,
                            double* height, double* ascent, 
                            double* descent, double* extLeading) const;

private:
  bool           m_templateMode;
  double         m_templateWidth;
  double         m_templateHeight;
  double         m_ppi;
  double         m_ppiPdfFont;
  wxPdfDocument* m_pdfDocument;
  int            m_imageCount;
  wxPrintData    m_printData;
  wxPdfMapModeStyle m_mappingModeStyle;

  wxVector<float> m_layerOpacities;

  wxDECLARE_NO_COPY_CLASS(wxPdfGraphicsContext);
};

// ----------------------------------------------------------------------------
// wxPdfGraphicsPenBrushData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsPenBrushData::wxPdfGraphicsPenBrushData(wxGraphicsRenderer* renderer,
                                                     const wxColour& col,
                                                     bool isTransparent)
  : wxGraphicsObjectRefData(renderer)
{
  m_hatchStyle = wxHATCHSTYLE_INVALID;
//  m_pattern = NULL;
  m_bmpdata = NULL;

  if (isTransparent)
  {
    m_colour = wxTransparentColour;
  }
  else // non-transparent
  {
    m_colour = col;
  }
}

wxPdfGraphicsPenBrushData::~wxPdfGraphicsPenBrushData()
{
#if 0
  if (m_bmpdata)
  {
    // Deleting the bitmap data also deletes the pattern referenced by
    // m_pattern, so set it to NULL to avoid deleting it twice.
    delete m_bmpdata;
    m_pattern = NULL;
  }
  if (m_pattern)
  {
    cairo_pattern_destroy(m_pattern);
  }
#endif
}

void
wxPdfGraphicsPenBrushData::InitHatchPattern()
{
#if 0
  cairo_surface_t* const surface = 
    cairo_surface_create_similar(cairo_get_target(ctext),                             CAIRO_CONTENT_COLOR_ALPHA, 10, 10);

  cairo_t* const cr = cairo_create(surface);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
  cairo_set_line_width(cr, 1);
  cairo_set_line_join(cr,CAIRO_LINE_JOIN_MITER);

  switch (m_hatchStyle)
  {
    case wxHATCHSTYLE_CROSS:
      cairo_move_to(cr, 5, 0);
      cairo_line_to(cr, 5, 10);
      cairo_move_to(cr, 0, 5);
      cairo_line_to(cr, 10, 5);
      break;

    case wxHATCHSTYLE_BDIAGONAL:
      cairo_move_to(cr, 0, 10);
      cairo_line_to(cr, 10, 0);
      break;

    case wxHATCHSTYLE_FDIAGONAL:
      cairo_move_to(cr, 0, 0);
      cairo_line_to(cr, 10, 10);
      break;

    case wxHATCHSTYLE_CROSSDIAG:
      cairo_move_to(cr, 0, 0);
      cairo_line_to(cr, 10, 10);
      cairo_move_to(cr, 10, 0);
      cairo_line_to(cr, 0, 10);
      break;

    case wxHATCHSTYLE_HORIZONTAL:
      cairo_move_to(cr, 0, 5);
      cairo_line_to(cr, 10, 5);
      break;

    case wxHATCHSTYLE_VERTICAL:
      cairo_move_to(cr, 5, 0);
      cairo_line_to(cr, 5, 10);
      break;

    default:
      wxFAIL_MSG(wxS("Invalid hatch pattern style."));
  }

  cairo_set_source_rgba(cr, m_red, m_green, m_blue, m_alpha);
  cairo_stroke(cr);

  cairo_destroy(cr);

  m_pattern = cairo_pattern_create_for_surface(surface);
  cairo_surface_destroy(surface);
  cairo_pattern_set_extend(m_pattern, CAIRO_EXTEND_REPEAT);
#endif
}

void
wxPdfGraphicsPenBrushData::InitStipple(wxBitmap* bmp)
{
#if 0
  wxCHECK_RET(bmp && bmp->IsOk(), wxS("Invalid stippled bitmap"));
  m_bmpdata = new wxPdfGraphicsBitmapData(GetRenderer(), *bmp);
  m_pattern = m_bmpdata->GetCairoPattern();
  cairo_pattern_set_extend(m_pattern, CAIRO_EXTEND_REPEAT);
#endif
}

void
wxPdfGraphicsPenBrushData::InitHatch(wxHatchStyle hatchStyle)
{
  // We can't create m_pattern right now as we don't have the Cairo context
  // needed for it, so just remember that we need to do it.
  m_hatchStyle = hatchStyle;
}

void
wxPdfGraphicsPenBrushData::Apply(wxPdfGraphicsContext* context)
{
#if 0
  cairo_t* const ctext = (cairo_t*) context->GetNativeContext();

  if (m_hatchStyle != wxHATCHSTYLE_INVALID && !m_pattern)
  {
    InitHatchPattern(ctext);
  }

  if (m_pattern)
  {
    cairo_set_source(ctext, m_pattern);
  }
  else
  {
    cairo_set_source_rgba(ctext, m_red, m_green, m_blue, m_alpha);
  }
#endif
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsPenData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsPenData::~wxPdfGraphicsPenData()
{
  delete[] m_userLengths;
}

void
wxPdfGraphicsPenData::Init()
{
  m_lengths = NULL;
  m_userLengths = NULL;
  m_width = 0;
  m_count = 0;
}

wxPdfGraphicsPenData::wxPdfGraphicsPenData(wxGraphicsRenderer* renderer, const wxPen &pen )
  : wxPdfGraphicsPenBrushData(renderer, pen.GetColour(), pen.IsTransparent())
{
  Init();
  m_width = pen.GetWidth();
  if (m_width <= 0.0)
  {
    m_width = 0.1;
  }

  switch (pen.GetCap())
  {
    case wxCAP_ROUND:
      m_cap = wxPDF_LINECAP_ROUND;
      break;

    case wxCAP_PROJECTING:
      m_cap = wxPDF_LINECAP_SQUARE;
      break;

    case wxCAP_BUTT:
      m_cap = wxPDF_LINECAP_BUTT;
      break;

    default:
      m_cap = wxPDF_LINECAP_BUTT;
      break;
  }

  switch (pen.GetJoin())
  {
    case wxJOIN_BEVEL:
      m_join = wxPDF_LINEJOIN_BEVEL;
      break;

    case wxJOIN_MITER:
      m_join = wxPDF_LINEJOIN_MITER;
      break;

    case wxJOIN_ROUND:
      m_join = wxPDF_LINEJOIN_ROUND;
      break;

    default:
      m_join = wxPDF_LINEJOIN_MITER;
      break;
  }

  const double dashUnit = m_width < 1.0 ? 1.0 : m_width;
  const double dotted[] =
  {
    dashUnit , dashUnit + 2.0
  };
  static const double short_dashed[] =
  {
    9.0 , 6.0
  };
  static const double dashed[] =
  {
    19.0 , 9.0
  };
  static const double dotted_dashed[] =
  {
    9.0 , 6.0 , 3.0 , 3.0
  };

  switch (pen.GetStyle())
  {
    case wxPENSTYLE_SOLID:
      break;

    case wxPENSTYLE_DOT:
      m_count = WXSIZEOF(dotted);
      m_userLengths = new double[ m_count ] ;
      memcpy( m_userLengths, dotted, sizeof(dotted) );
      m_lengths = m_userLengths;
      break;

    case wxPENSTYLE_LONG_DASH:
      m_lengths = dashed;
      m_count = WXSIZEOF(dashed);
      break;

    case wxPENSTYLE_SHORT_DASH:
      m_lengths = short_dashed;
      m_count = WXSIZEOF(short_dashed);
      break;

    case wxPENSTYLE_DOT_DASH:
      m_lengths = dotted_dashed;
      m_count = WXSIZEOF(dotted_dashed);
      break;

    case wxPENSTYLE_USER_DASH:
      {
        wxDash* wxdashes ;
        m_count = pen.GetDashes(&wxdashes) ;
        if ((wxdashes != NULL) && (m_count > 0))
        {
          m_userLengths = new double[m_count];
          for (int i = 0 ; i < m_count ; ++i)
          {
            m_userLengths[i] = wxdashes[i] * dashUnit;

            if (i % 2 == 1 && m_userLengths[i] < dashUnit + 2.0)
            {
              m_userLengths[i] = dashUnit + 2.0 ;
            }
            else if (i % 2 == 0 && m_userLengths[i] < dashUnit)
              m_userLengths[i] = dashUnit;
            }
          }
          m_lengths = m_userLengths;
      }
      break;

    case wxPENSTYLE_STIPPLE :
    case wxPENSTYLE_STIPPLE_MASK :
    case wxPENSTYLE_STIPPLE_MASK_OPAQUE :
      InitStipple(pen.GetStipple());
      break;

    default :
      if (pen.GetStyle() >= wxPENSTYLE_FIRST_HATCH &&
          pen.GetStyle() <= wxPENSTYLE_LAST_HATCH)
      {
        InitHatch(static_cast<wxHatchStyle>(pen.GetStyle()));
      }
      break;
  }
}

void
wxPdfGraphicsPenData::Apply(wxPdfGraphicsContext* context)
{
#if 0
  wxPdfGraphicsPenBrushData::Apply(context);

  cairo_t * ctext = (cairo_t*) context->GetNativeContext();
  cairo_set_line_width(ctext,m_width);
  cairo_set_line_cap(ctext,m_cap);
  cairo_set_line_join(ctext,m_join);
  cairo_set_dash(ctext,(double*)m_lengths,m_count,0.0);
#endif
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsBrushData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsBrushData::wxPdfGraphicsBrushData( wxGraphicsRenderer* renderer )
  : wxPdfGraphicsPenBrushData(renderer, wxColour(), true /* transparent */)
{
  Init();
}

wxPdfGraphicsBrushData::wxPdfGraphicsBrushData(wxGraphicsRenderer* renderer,
                                               const wxBrush &brush)
  : wxPdfGraphicsPenBrushData(renderer, brush.GetColour(), brush.IsTransparent())
{
  Init();

  switch ( brush.GetStyle() )
  {
    case wxBRUSHSTYLE_STIPPLE:
    case wxBRUSHSTYLE_STIPPLE_MASK:
    case wxBRUSHSTYLE_STIPPLE_MASK_OPAQUE:
      InitStipple(brush.GetStipple());
      break;

    default:
      if (brush.IsHatch())
      {
        InitHatch(static_cast<wxHatchStyle>(brush.GetStyle()));
      }
      break;
  }
}

void
wxPdfGraphicsBrushData::AddGradientStops(const wxGraphicsGradientStops& stops)
{
#if 0
  // loop over all the stops, they include the beginning and ending ones
  const unsigned numStops = stops.GetCount();
  for (unsigned n = 0; n < numStops; n++)
  {
    const wxGraphicsGradientStop stop = stops.Item(n);

    const wxColour col = stop.GetColour();

    cairo_pattern_add_color_stop_rgba(m_pattern,
                                      stop.GetPosition(),
                                      col.Red() / 255.0,
                                      col.Green() / 255.0,
                                      col.Blue() / 255.0,
                                      col.Alpha() / 255.0);
  }

  wxASSERT_MSG(cairo_pattern_status(m_pattern) == CAIRO_STATUS_SUCCESS,
               wxT("Couldn't create cairo pattern"));
#endif
}

void
wxPdfGraphicsBrushData::CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                                                  wxDouble x2, wxDouble y2,
                                                  const wxGraphicsGradientStops& stops)
{
#if 0
  m_pattern = cairo_pattern_create_linear(x1,y1,x2,y2);

  AddGradientStops(stops);
#endif
}

void
wxPdfGraphicsBrushData::CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                                  wxDouble xc, wxDouble yc,
                                                  wxDouble radius,
                                                  const wxGraphicsGradientStops& stops)
{
#if 0
  m_pattern = cairo_pattern_create_radial(xo,yo,0.0,xc,yc,radius);

  AddGradientStops(stops);
#endif
}

void
wxPdfGraphicsBrushData::Init()
{
//  m_pattern = NULL;
  m_bmpdata = NULL;
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsFontData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsFontData::wxPdfGraphicsFontData(wxGraphicsRenderer* renderer, const wxFont& font,
                                             const wxColour& col )
  : wxGraphicsObjectRefData(renderer)
{
  m_colour = col;

  m_size = font.GetPointSize();

  m_font = font;
  m_styles = wxPDF_FONTSTYLE_REGULAR;
  if (font.GetWeight() == wxFONTWEIGHT_BOLD)
  {
    m_styles |= wxPDF_FONTSTYLE_BOLD;
  }
  if (font.GetStyle() == wxFONTSTYLE_ITALIC)
  {
    m_styles |= wxPDF_FONTSTYLE_ITALIC;
  }
  if (font.GetUnderlined())
  {
    m_styles |= wxPDF_FONTSTYLE_UNDERLINE;
  }
  if (font.GetStrikethrough())
  {
    m_styles |= wxPDF_FONTSTYLE_STRIKEOUT;
  }

  m_regFont = wxPdfFontManager::GetFontManager()->GetFont(font.GetFaceName(), m_styles);
  if (!m_regFont.IsValid())
  {
    m_regFont = wxPdfFontManager::GetFontManager()->RegisterFont(font, font.GetFaceName());
  }
}

wxPdfGraphicsFontData::wxPdfGraphicsFontData(wxGraphicsRenderer* renderer,
                                             double sizeInPixels,
                                             const wxString& facename,
                                             int flags,
                                             const wxColour& col)
  : wxGraphicsObjectRefData(renderer)
{
  m_colour = col;

  // Default resolution for PDF is 72 DPI meaning that the sizes in points
  // and pixels are identical, so we can just pass the size in pixels directly.
  m_size = sizeInPixels;

  // Let wxWidgets determine a suitable font 
  wxFont font(wxRound(sizeInPixels), 
 	            wxFONTFAMILY_DEFAULT, 
 	            flags & wxFONTFLAG_ITALIC ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, 
 	            flags & wxFONTFLAG_BOLD   ? wxFONTWEIGHT_BOLD  : wxFONTWEIGHT_NORMAL, 
 	            (flags & wxFONTFLAG_UNDERLINED) != 0, 
 	            facename); 

  m_font = font;
  int styles = wxPDF_FONTSTYLE_REGULAR;
  if (font.GetWeight() == wxFONTWEIGHT_BOLD)
  {
    styles |= wxPDF_FONTSTYLE_BOLD;
  }
  if (font.GetStyle() == wxFONTSTYLE_ITALIC)
  {
    styles |= wxPDF_FONTSTYLE_ITALIC;
  }
  if (font.GetUnderlined())
  {
    styles |= wxPDF_FONTSTYLE_UNDERLINE;
  }
  if (font.GetStrikethrough())
  {
    styles |= wxPDF_FONTSTYLE_STRIKEOUT;
  }

  m_regFont = wxPdfFontManager::GetFontManager()->GetFont(font.GetFaceName(), styles);
  if (!m_regFont.IsValid())
  {
    m_regFont = wxPdfFontManager::GetFontManager()->RegisterFont(font, font.GetFaceName());
  }
}

wxPdfGraphicsFontData::~wxPdfGraphicsFontData()
{
}

bool
wxPdfGraphicsFontData::Apply(wxPdfGraphicsContext* context)
{
  wxPdfDocument* pdfDocument = context->GetPdfDocument();
  bool ok = m_regFont.IsValid() && pdfDocument != NULL;
  if (ok)
  {
    pdfDocument->SetTextColour(m_colour.Red(), m_colour.Green(), m_colour.Blue());
    pdfDocument->SetFont(m_regFont, m_styles, m_size);
  }
  return ok;
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsPathData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsPathData::wxPdfGraphicsPathData(wxGraphicsRenderer* renderer, const wxPdfShape* path)
  : wxGraphicsPathData(renderer)
{
  if (path)
  {
    m_path = *path;
  }
}

wxPdfGraphicsPathData::~wxPdfGraphicsPathData()
{
}

wxGraphicsObjectRefData*
wxPdfGraphicsPathData::Clone() const
{
  return new wxPdfGraphicsPathData(GetRenderer(), &m_path);
}

void*
wxPdfGraphicsPathData::GetNativePath() const
{
  return (void*) (&m_path);
}

void
wxPdfGraphicsPathData::UnGetNativePath(void* p) const
{
}

//
// The Primitives
//

void
wxPdfGraphicsPathData::MoveToPoint(wxDouble x , wxDouble y)
{
  m_path.MoveTo(x, y);
}

void
wxPdfGraphicsPathData::AddLineToPoint(wxDouble x, wxDouble y)
{
  m_path.LineTo(x, y);
}

void
wxPdfGraphicsPathData::AddPath(const wxGraphicsPathData* path)
{
  wxPdfShape* p = (wxPdfShape*) path->GetNativePath();
  m_path.AddPath(*p);
  UnGetNativePath(p);
}

void
wxPdfGraphicsPathData::CloseSubpath()
{
  m_path.ClosePath();
}

void
wxPdfGraphicsPathData::AddCurveToPoint(wxDouble cx1, wxDouble cy1, wxDouble cx2, wxDouble cy2, wxDouble x, wxDouble y)
{
  m_path.CurveTo(cx1, cy1, cx2, cy2, x, y);
}

// gets the last point of the current path, (0,0) if not yet set
void
wxPdfGraphicsPathData::GetCurrentPoint(wxDouble* x, wxDouble* y) const
{
  double dx,dy;
  m_path.GetCurrentPoint(&dx,&dy);
  if (x) *x = dx;
  if (y) *y = dy;
}

void
wxPdfGraphicsPathData::AddArc(wxDouble x, wxDouble y, wxDouble r, double startAngle, double endAngle, bool clockwise)
{
  static double pi = 4. * atan(1.);
  static double pi2 = 0.5 * pi;
  double xc = x;
  double yc = y;
  double astart = startAngle;
  double afinish = endAngle;
  double origin = 0;
  double d;
#if 0  
  if (clockwise)
  {
    d = afinish;
    afinish = origin - astart;
    astart = origin - d;
  }
  else
  {
    afinish += origin;
    astart += origin;
  }
  astart = fmod(astart, 360.) + 360;
  afinish = fmod(afinish, 360.) + 360;
  if (astart > afinish)
  {
    afinish += 360;
  }
  afinish = afinish / 180. * pi;
  astart = astart / 180. * pi;
#endif  
  d = afinish - astart;
  if (d == 0)
  {
    d = 2 * pi;
  }
  
  double myArc;
  if (sin(d/2) != 0.0)
  {
    myArc = 4./3. * (1.-cos(d/2))/sin(d/2) * r;
  }
  else
  {
    myArc = 0.0;
  }

  // add the arc
  if (d < pi2)
  {
    m_path.CurveTo(xc+r*cos(astart)+myArc*cos(pi2+astart),
                   yc-r*sin(astart)-myArc*sin(pi2+astart),
                   xc+r*cos(afinish)+myArc*cos(afinish-pi2),
                   yc-r*sin(afinish)-myArc*sin(afinish-pi2),
                   xc+r*cos(afinish),
                   yc-r*sin(afinish));
  }
  else
  {
    afinish = astart + d/4;
    myArc = 4./3. * (1.-cos(d/8))/sin(d/8) * r;
    m_path.CurveTo(xc+r*cos(astart)+myArc*cos(pi2+astart),
                   yc-r*sin(astart)-myArc*sin(pi2+astart),
                   xc+r*cos(afinish)+myArc*cos(afinish-pi2),
                   yc-r*sin(afinish)-myArc*sin(afinish-pi2),
                   xc+r*cos(afinish),
                   yc-r*sin(afinish));
    astart = afinish;
    afinish = astart + d/4;
    m_path.CurveTo(xc+r*cos(astart)+myArc*cos(pi2+astart),
                   yc-r*sin(astart)-myArc*sin(pi2+astart),
                   xc+r*cos(afinish)+myArc*cos(afinish-pi2),
                   yc-r*sin(afinish)-myArc*sin(afinish-pi2),
                   xc+r*cos(afinish),
                   yc-r*sin(afinish));
    astart = afinish;
    afinish = astart + d/4;
    m_path.CurveTo(xc+r*cos(astart)+myArc*cos(pi2+astart),
                   yc-r*sin(astart)-myArc*sin(pi2+astart),
                   xc+r*cos(afinish)+myArc*cos(afinish-pi2),
                   yc-r*sin(afinish)-myArc*sin(afinish-pi2),
                   xc+r*cos(afinish),
                   yc-r*sin(afinish));
    astart = afinish;
    afinish = astart + d/4;
    m_path.CurveTo(xc+r*cos(astart)+myArc*cos(pi2+astart),
                   yc-r*sin(astart)-myArc*sin(pi2+astart),
                   xc+r*cos(afinish)+myArc*cos(afinish-pi2),
                   yc-r*sin(afinish)-myArc*sin(afinish-pi2),
                   xc+r*cos(afinish),
                   yc-r*sin(afinish));
  }
}

// transforms each point of this path by the matrix
void
wxPdfGraphicsPathData::Transform(const wxGraphicsMatrixData* matrix)
{
  // as we don't have a true path object, we have to apply the inverse
  // matrix to the context
  wxAffineMatrix2D m = *((wxAffineMatrix2D*) matrix->GetNativeMatrix());
  m.Invert();
  m_path.Transform(m);
}

// gets the bounding box enclosing all points (possibly including control points)
void
wxPdfGraphicsPathData::GetBox(wxDouble* x, wxDouble* y, wxDouble* w, wxDouble* h) const
{
  double x1,y1,x2,y2;
  m_path.GetBox(&x1, &y1, &x2, &y2);
  if (x2 < x1)
  {
    *x = x2;
    *w = x1-x2;
  }
  else
  {
    *x = x1;
    *w = x2-x1;
  }

  if (y2 < y1)
  {
    *y = y2;
    *h = y1-y2;
  }
  else
  {
    *y = y1;
    *h = y2-y1;
  }
}

bool
wxPdfGraphicsPathData::Contains(wxDouble x, wxDouble y, wxPolygonFillMode fillStyle) const
{
  // TODO: return reasonable value if possible
  return false;
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsMatrixData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsMatrixData::wxPdfGraphicsMatrixData(wxGraphicsRenderer* renderer, const wxAffineMatrix2D* matrix)
  : wxGraphicsMatrixData(renderer)
{
  if (matrix)
  {
    m_matrix = *matrix;
  }
}

wxPdfGraphicsMatrixData::~wxPdfGraphicsMatrixData()
{
    // nothing to do
}

wxGraphicsObjectRefData*
wxPdfGraphicsMatrixData::Clone() const
{
  return new wxPdfGraphicsMatrixData(GetRenderer(),&m_matrix);
}

// concatenates the matrix
void
wxPdfGraphicsMatrixData::Concat(const wxGraphicsMatrixData* t)
{
  m_matrix.Concat(*((wxAffineMatrix2D*) t->GetNativeMatrix()));
}

// sets the matrix to the respective values
void
wxPdfGraphicsMatrixData::Set(wxDouble a, wxDouble b, wxDouble c, wxDouble d, wxDouble tx, wxDouble ty)
{
  wxMatrix2D mat2D(a, b, c, d);
  wxPoint2DDouble tr(tx, ty);
  m_matrix.Set(mat2D, tr);
}

// gets the component valuess of the matrix
void
wxPdfGraphicsMatrixData::Get(wxDouble* a, wxDouble* b,  wxDouble* c, wxDouble* d, wxDouble* tx, wxDouble* ty) const
{
  wxMatrix2D mat2D;
  wxPoint2DDouble tr;
  m_matrix.Get(&mat2D, &tr);
  if (a)  *a  = mat2D.m_11;
  if (b)  *b  = mat2D.m_12;
  if (c)  *c  = mat2D.m_21;
  if (d)  *d  = mat2D.m_22;
  if (tx) *tx = tr.m_x;
  if (ty) *ty = tr.m_y;
}

// makes this the inverse matrix
void
wxPdfGraphicsMatrixData::Invert()
{
  m_matrix.Invert();
}

// returns true if the elements of the transformation matrix are equal ?
bool
wxPdfGraphicsMatrixData::IsEqual(const wxGraphicsMatrixData* t) const
{
  return m_matrix.IsEqual(*((wxAffineMatrix2D*) t->GetNativeMatrix()));
}

// return true if this is the identity matrix
bool
wxPdfGraphicsMatrixData::IsIdentity() const
{
  return m_matrix.IsIdentity();
}

//
// transformation
//

// add the translation to this matrix
void
wxPdfGraphicsMatrixData::Translate(wxDouble dx, wxDouble dy)
{
  m_matrix.Translate(dx, dy);
}

// add the scale to this matrix
void
wxPdfGraphicsMatrixData::Scale(wxDouble xScale, wxDouble yScale)
{
  m_matrix.Scale(xScale, yScale);
}

// add the rotation to this matrix (radians)
void
wxPdfGraphicsMatrixData::Rotate(wxDouble angle)
{
  m_matrix.Rotate(angle);
}

//
// apply the transforms
//

// applies that matrix to the point
void
wxPdfGraphicsMatrixData::TransformPoint(wxDouble* x, wxDouble* y) const
{
  wxPoint2DDouble tr(*x, *y);
  tr = m_matrix.TransformPoint(tr);
  *x = tr.m_x;
  *y = tr.m_y;
}

// applies the matrix except for translations
void
wxPdfGraphicsMatrixData::TransformDistance(wxDouble* dx, wxDouble* dy) const
{
  wxPoint2DDouble tr(*dx, *dy);
  tr = m_matrix.TransformDistance(tr);
  *dx = tr.m_x;
  *dy = tr.m_y;
}

// returns the native representation
void*
wxPdfGraphicsMatrixData::GetNativeMatrix() const
{
  return (void*) &m_matrix;
}

// ----------------------------------------------------------------------------
// wxPdfGraphicsBitmap implementation
// ----------------------------------------------------------------------------
#if 0
void wxPdfGraphicsBitmapData::InitSurface(cairo_format_t format, int stride)
{

    m_surface = cairo_image_surface_create_for_data(
                            m_buffer, format, m_width, m_height, stride);
    m_pattern = cairo_pattern_create_for_surface(m_surface);
}
#endif

wxPdfGraphicsBitmapData::wxPdfGraphicsBitmapData( wxGraphicsRenderer* renderer, const wxBitmap& bmp ) : wxGraphicsBitmapData( renderer )
{
#if 0
    wxCHECK_RET( bmp.IsOk(), wxT("Invalid bitmap in wxPdfGraphicsContext::DrawBitmap"));

#ifdef wxHAS_RAW_BITMAP
    // Create a surface object and copy the bitmap pixel data to it.  if the
    // image has alpha (or a mask represented as alpha) then we'll use a
    // different format and iterator than if it doesn't...
    cairo_format_t bufferFormat = bmp.GetDepth() == 32
#if defined(__WXGTK__) && !defined(__WXGTK3__)
                                            || bmp.GetMask()
#endif
                                        ? CAIRO_FORMAT_ARGB32
                                        : CAIRO_FORMAT_RGB24;

    int stride = InitBuffer(bmp.GetWidth(), bmp.GetHeight(), bufferFormat);

    wxBitmap bmpSource = bmp;  // we need a non-const instance
    wxUint32* data = (wxUint32*)m_buffer;

    if ( bufferFormat == CAIRO_FORMAT_ARGB32 )
    {
        // use the bitmap's alpha
        wxAlphaPixelData
            pixData(bmpSource, wxPoint(0, 0), wxSize(m_width, m_height));
        wxCHECK_RET( pixData, wxT("Failed to gain raw access to bitmap data."));

        wxAlphaPixelData::Iterator p(pixData);
        for (int y=0; y<m_height; y++)
        {
            wxAlphaPixelData::Iterator rowStart = p;
            wxUint32* const rowStartDst = data;
            for (int x=0; x<m_width; x++)
            {
                // Each pixel in CAIRO_FORMAT_ARGB32 is a 32-bit quantity,
                // with alpha in the upper 8 bits, then red, then green, then
                // blue. The 32-bit quantities are stored native-endian.
                // Pre-multiplied alpha is used.
                unsigned char alpha = p.Alpha();
                if (alpha == 0)
                    *data = 0;
                else
                    *data = ( alpha                      << 24
                              | (p.Red() * alpha/255)    << 16
                              | (p.Green() * alpha/255)  <<  8
                              | (p.Blue() * alpha/255) );
                ++data;
                ++p;
            }

            data = rowStartDst + stride / 4;
            p = rowStart;
            p.OffsetY(pixData, 1);
        }
    }
    else  // no alpha
    {
        wxNativePixelData
            pixData(bmpSource, wxPoint(0, 0), wxSize(m_width, m_height));
        wxCHECK_RET( pixData, wxT("Failed to gain raw access to bitmap data."));

        wxNativePixelData::Iterator p(pixData);
        for (int y=0; y<m_height; y++)
        {
            wxNativePixelData::Iterator rowStart = p;
            wxUint32* const rowStartDst = data;
            for (int x=0; x<m_width; x++)
            {
                // Each pixel in CAIRO_FORMAT_RGB24 is a 32-bit quantity, with
                // the upper 8 bits unused. Red, Green, and Blue are stored in
                // the remaining 24 bits in that order.  The 32-bit quantities
                // are stored native-endian.
                *data = ( p.Red() << 16 | p.Green() << 8 | p.Blue() );
                ++data;
                ++p;
            }

            data = rowStartDst + stride / 4;
            p = rowStart;
            p.OffsetY(pixData, 1);
        }
    }
#if defined(__WXMSW__) || defined(__WXGTK3__)
    // if there is a mask, set the alpha bytes in the target buffer to 
    // fully transparent or fully opaque
    if (bmpSource.GetMask())
    {
        wxBitmap bmpMask = bmpSource.GetMaskBitmap();
        bufferFormat = CAIRO_FORMAT_ARGB32;
        data = (wxUint32*)m_buffer;
        wxNativePixelData
            pixData(bmpMask, wxPoint(0, 0), wxSize(m_width, m_height));
        wxCHECK_RET( pixData, wxT("Failed to gain raw access to mask data."));

        wxNativePixelData::Iterator p(pixData);
        for (int y=0; y<m_height; y++)
        {
            wxNativePixelData::Iterator rowStart = p;
            wxUint32* const rowStartDst = data;
            for (int x=0; x<m_width; x++)
            {
                if (p.Red()+p.Green()+p.Blue() == 0)
                    *data = 0;
                else
                    *data = (wxALPHA_OPAQUE << 24) | (*data & 0x00FFFFFF);
                ++data;
                ++p;
            }

            data = rowStartDst + stride / 4;
            p = rowStart;
            p.OffsetY(pixData, 1);
        }
    }
#endif

    InitSurface(bufferFormat, stride);
#endif // wxHAS_RAW_BITMAP
#endif
}

#if wxUSE_IMAGE

// Helper functions for dealing with alpha pre-multiplication.
namespace
{

inline unsigned char Premultiply(unsigned char alpha, unsigned char data)
{
    return alpha ? (data * alpha)/0xff : data;
}

inline unsigned char Unpremultiply(unsigned char alpha, unsigned char data)
{
    return alpha ? (data * 0xff)/alpha : data;
}

} // anonymous namespace

wxPdfGraphicsBitmapData::wxPdfGraphicsBitmapData(wxGraphicsRenderer* renderer,
                                     const wxImage& image)
    : wxGraphicsBitmapData(renderer)
{
#if 0
    const cairo_format_t bufferFormat = image.HasAlpha()
                                            ? CAIRO_FORMAT_ARGB32
                                            : CAIRO_FORMAT_RGB24;

    int stride = InitBuffer(image.GetWidth(), image.GetHeight(), bufferFormat);

    // Copy wxImage data into the buffer. Notice that we work with wxUint32
    // values and not bytes becase Cairo always works with buffers in native
    // endianness.
    wxUint32* dst = reinterpret_cast<wxUint32*>(m_buffer);
    const unsigned char* src = image.GetData();

    if ( bufferFormat == CAIRO_FORMAT_ARGB32 )
    {
        const unsigned char* alpha = image.GetAlpha();

        for ( int y = 0; y < m_height; y++ )
        {
            wxUint32* const rowStartDst = dst;

            for ( int x = 0; x < m_width; x++ )
            {
                const unsigned char a = *alpha++;

                *dst++ = a                      << 24 |
                         Premultiply(a, src[0]) << 16 |
                         Premultiply(a, src[1]) <<  8 |
                         Premultiply(a, src[2]);
                src += 3;
            }

            dst = rowStartDst + stride / 4;
        }
    }
    else // RGB
    {
        for ( int y = 0; y < m_height; y++ )
        {
            wxUint32* const rowStartDst = dst;

            for ( int x = 0; x < m_width; x++ )
            {
                *dst++ = src[0] << 16 |
                         src[1] <<  8 |
                         src[2];
                src += 3;
            }

            dst = rowStartDst + stride / 4;
        }
    }

    InitSurface(bufferFormat, stride);
#endif
}

wxImage wxPdfGraphicsBitmapData::ConvertToImage() const
{
  wxImage image(m_width, m_height, false /* don't clear */);
#if 0

    // Get the surface type and format.
    wxCHECK_MSG( cairo_surface_get_type(m_surface) == CAIRO_SURFACE_TYPE_IMAGE,
                 wxNullImage,
                 wxS("Can't convert non-image surface to image.") );

    switch ( cairo_image_surface_get_format(m_surface) )
    {
        case CAIRO_FORMAT_ARGB32:
            image.SetAlpha();
            break;

        case CAIRO_FORMAT_RGB24:
            // Nothing to do, we don't use alpha by default.
            break;

        case CAIRO_FORMAT_A8:
        case CAIRO_FORMAT_A1:
            wxFAIL_MSG(wxS("Unsupported Cairo image surface type."));
            return wxNullImage;

        default:
            wxFAIL_MSG(wxS("Unknown Cairo image surface type."));
            return wxNullImage;
    }

    // Prepare for copying data.
    const wxUint32* src = (wxUint32*)cairo_image_surface_get_data(m_surface);
    wxCHECK_MSG( src, wxNullImage, wxS("Failed to get Cairo surface data.") );

    int stride = cairo_image_surface_get_stride(m_surface);
    wxCHECK_MSG( stride > 0, wxNullImage,
                 wxS("Failed to get Cairo surface stride.") );

    // As we work with wxUint32 pointers and not char ones, we need to adjust
    // the stride accordingly. This should be lossless as the stride must be a
    // multiple of pixel size.
    wxASSERT_MSG( !(stride % sizeof(wxUint32)), wxS("Unexpected stride.") );
    stride /= sizeof(wxUint32);

    unsigned char* dst = image.GetData();
    unsigned char *alpha = image.GetAlpha();
    if ( alpha )
    {
        // We need to also copy alpha and undo the pre-multiplication as Cairo
        // stores pre-multiplied values in this format while wxImage does not.
        for ( int y = 0; y < m_height; y++ )
        {
            const wxUint32* const rowStart = src;
            for ( int x = 0; x < m_width; x++ )
            {
                const wxUint32 argb = *src++;

                *alpha++ = (argb & 0xff000000) >> 24;

                // Copy the RGB data undoing the pre-multiplication.
                *dst++ = Unpremultiply(*alpha, (argb & 0x00ff0000) >> 16);
                *dst++ = Unpremultiply(*alpha, (argb & 0x0000ff00) >>  8);
                *dst++ = Unpremultiply(*alpha, (argb & 0x000000ff));
            }

            src = rowStart + stride;
        }
    }
    else // RGB
    {
        // Things are pretty simple in this case, just copy RGB bytes.
        for ( int y = 0; y < m_height; y++ )
        {
            const wxUint32* const rowStart = src;
            for ( int x = 0; x < m_width; x++ )
            {
                const wxUint32 argb = *src++;

                *dst++ = (argb & 0x00ff0000) >> 16;
                *dst++ = (argb & 0x0000ff00) >>  8;
                *dst++ = (argb & 0x000000ff);
            }

            src = rowStart + stride;
        }
    }

#endif
  return image;
}

#endif // wxUSE_IMAGE

wxPdfGraphicsBitmapData::~wxPdfGraphicsBitmapData()
{
#if 0
    if (m_pattern)
        cairo_pattern_destroy(m_pattern);

    if (m_surface)
        cairo_surface_destroy(m_surface);

    delete [] m_buffer;
#endif
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsContext implementation
//-----------------------------------------------------------------------------

#if 0
class wxPdfGraphicsOffsetHelper
{
public :
    wxPdfGraphicsOffsetHelper( cairo_t* ctx , bool offset )
    {
        m_ctx = ctx;
        m_offset = offset;
        if ( m_offset )
             cairo_translate( m_ctx, 0.5, 0.5 );
    }
    ~wxPdfGraphicsOffsetHelper( )
    {
        if ( m_offset )
            cairo_translate( m_ctx, -0.5, -0.5 );
    }
public :
    cairo_t* m_ctx;
    bool m_offset;
} ;
#endif

wxPdfGraphicsContext::wxPdfGraphicsContext(wxGraphicsRenderer* renderer, const wxPrintData& data)
  : wxGraphicsContext(renderer)
{
  Init();
  SetPrintData(data);
}

wxPdfGraphicsContext::wxPdfGraphicsContext(wxGraphicsRenderer* renderer, wxPdfDocument* pdfDocument, double templateWidth, double templateHeight)
  : wxGraphicsContext(renderer)
{
  Init();
  m_pdfDocument = pdfDocument;
  m_templateMode = true;
  m_templateWidth = templateWidth;
  m_templateHeight = templateHeight;
}

void
wxPdfGraphicsContext::Init()
{
  m_templateMode = false;
  m_ppi = 72;
  m_pdfDocument = NULL;
  m_imageCount = 0;

  wxScreenDC screendc;
  m_ppiPdfFont = screendc.GetPPI().GetHeight();
  m_mappingModeStyle = wxPDF_MAPMODESTYLE_STANDARD;

//  SetBackgroundMode(wxSOLID);
  
  m_printData.SetOrientation(wxPORTRAIT);
  m_printData.SetPaperId(wxPAPER_A4);
  m_printData.SetFilename(wxT("default.pdf"));
}

void
wxPdfGraphicsContext::SetPrintData(const wxPrintData& data)
{
  m_printData = data;
  wxPaperSize id = m_printData.GetPaperId();
  wxPrintPaperType* paper = wxThePrintPaperDatabase->FindPaperType(id);
  if (!paper)
  {
    m_printData.SetPaperId(wxPAPER_A4);
  }
}

wxPdfGraphicsContext::wxPdfGraphicsContext(wxGraphicsRenderer* renderer)
  : wxGraphicsContext(renderer)
{
  Init();
}

wxPdfGraphicsContext::~wxPdfGraphicsContext()
{
  if (m_pdfDocument != NULL)
  {
    if (!m_templateMode)
    {
      delete m_pdfDocument;
    }
  }
}

bool
wxPdfGraphicsContext::StartDoc(const wxString& message)
{
  wxUnusedVar(message);
  if (!m_templateMode && m_pdfDocument == NULL)
  {
    m_pdfDocument = new wxPdfDocument(m_printData.GetOrientation(), wxString(wxT("pt")), m_printData.GetPaperId());
    m_pdfDocument->Open();
    //m_pdfDocument->SetCompression(false);
    m_pdfDocument->SetAuthor(wxT("wxPdfGraphicsContext"));
    m_pdfDocument->SetTitle(wxT("wxPdfGraphicsContext"));

    // TODO Begin
    wxGraphicsContext::SetBrush(*wxBLACK_BRUSH);
    wxGraphicsContext::SetPen(*wxBLACK_PEN);
//    SetBackground(*wxWHITE_BRUSH);
//    SetTextForeground(*wxBLACK);
//    SetDeviceOrigin(0, 0);
    // TODO End
  }
  return true;
}

void
wxPdfGraphicsContext::EndDoc()
{
  if (!m_templateMode && m_pdfDocument != NULL)
  {
    m_pdfDocument->SaveAsFile(m_printData.GetFilename());
    delete m_pdfDocument;
    m_pdfDocument = NULL;
  }
}

void
wxPdfGraphicsContext::StartPage(wxDouble width, wxDouble height)
{
  if (!m_templateMode && m_pdfDocument != NULL)
  {
    // Begin a new page
    // Library needs it this way (always landscape) to size the page correctly
    m_pdfDocument->AddPage(m_printData.GetOrientation());
    wxPdfLineStyle style = m_pdfDocument->GetLineStyle();
    style.SetWidth(1.0);
    style.SetColour(wxPdfColour(0, 0, 0));
    style.SetLineCap(wxPDF_LINECAP_ROUND);
    style.SetLineJoin(wxPDF_LINEJOIN_MITER);
    m_pdfDocument->SetLineStyle(style);
  }
}

void
wxPdfGraphicsContext::EndPage()
{
#if 0
  if (m_ok)
  {
    if (m_clipping)
    {
      DestroyClippingRegion();
    }
  }
#endif
}

void wxPdfGraphicsContext::PushState()
{
#if 0
    cairo_save(m_context);
#endif
}

void wxPdfGraphicsContext::PopState()
{
#if 0
    cairo_restore(m_context);
#endif
}

void wxPdfGraphicsContext::Clip(const wxRegion& region)
{
#if 0
    // Create a path with all the rectangles in the region
    wxGraphicsPath path = GetRenderer()->CreatePath();
    wxRegionIterator ri(region);
    while (ri)
    {
        path.AddRectangle(ri.GetX(), ri.GetY(), ri.GetW(), ri.GetH());
        ++ri;
    }

    // Put it in the context
    cairo_path_t* cp = (cairo_path_t*) path.GetNativePath() ;
    cairo_append_path(m_context, cp);

    // clip to that path
    cairo_clip(m_context);
    path.UnGetNativePath(cp);
#endif
}

void wxPdfGraphicsContext::Clip( wxDouble x, wxDouble y, wxDouble w, wxDouble h )
{
#if 0
    // Create a path with this rectangle
    wxGraphicsPath path = GetRenderer()->CreatePath();
    path.AddRectangle(x,y,w,h);

    // Put it in the context
    cairo_path_t* cp = (cairo_path_t*) path.GetNativePath() ;
    cairo_append_path(m_context, cp);

    // clip to that path
    cairo_clip(m_context);
    path.UnGetNativePath(cp);
#endif
}

void wxPdfGraphicsContext::ResetClip()
{
#if 0
    cairo_reset_clip(m_context);
#endif
}

void*
wxPdfGraphicsContext::GetNativeContext()
{
  return NULL;
}

bool
wxPdfGraphicsContext::SetAntialiasMode(wxAntialiasMode antialias)
{
  if (m_antialias == antialias)
    return true;

  m_antialias = antialias;
#if 0
    cairo_antialias_t antialiasMode;
    switch (antialias)
    {
        case wxANTIALIAS_DEFAULT:
            antialiasMode = CAIRO_ANTIALIAS_DEFAULT;
            break;
        case wxANTIALIAS_NONE:
            antialiasMode = CAIRO_ANTIALIAS_NONE;
            break;
        default:
            return false;
    }
    cairo_set_antialias(m_context, antialiasMode);
#endif
  return true;
}

bool wxPdfGraphicsContext::SetInterpolationQuality(wxInterpolationQuality WXUNUSED(interpolation))
{
    // placeholder
    return false;
}

bool wxPdfGraphicsContext::SetCompositionMode(wxCompositionMode op)
{
  // TODO: SetAlpha ???
#if 0
    if ( m_composition == op )
        return true;

    m_composition = op;
    cairo_operator_t cop;
    switch (op)
    {
        case wxCOMPOSITION_CLEAR:
            cop = CAIRO_OPERATOR_CLEAR;
            break;
        case wxCOMPOSITION_SOURCE:
            cop = CAIRO_OPERATOR_SOURCE;
            break;
        case wxCOMPOSITION_OVER:
            cop = CAIRO_OPERATOR_OVER;
            break;
        case wxCOMPOSITION_IN:
            cop = CAIRO_OPERATOR_IN;
            break;
        case wxCOMPOSITION_OUT:
            cop = CAIRO_OPERATOR_OUT;
            break;
        case wxCOMPOSITION_ATOP:
            cop = CAIRO_OPERATOR_ATOP;
            break;
        case wxCOMPOSITION_DEST:
            cop = CAIRO_OPERATOR_DEST;
            break;
        case wxCOMPOSITION_DEST_OVER:
            cop = CAIRO_OPERATOR_DEST_OVER;
            break;
        case wxCOMPOSITION_DEST_IN:
            cop = CAIRO_OPERATOR_DEST_IN;
            break;
        case wxCOMPOSITION_DEST_OUT:
            cop = CAIRO_OPERATOR_DEST_OUT;
            break;
        case wxCOMPOSITION_DEST_ATOP:
            cop = CAIRO_OPERATOR_DEST_ATOP;
            break;
        case wxCOMPOSITION_XOR:
            cop = CAIRO_OPERATOR_XOR;
            break;
        case wxCOMPOSITION_ADD:
            cop = CAIRO_OPERATOR_ADD;
            break;
        default:
            return false;
    }
    cairo_set_operator(m_context, cop);
#endif
  return true;
}

void
wxPdfGraphicsContext::GetDPI(wxDouble* dpiX, wxDouble* dpiY)
{
  // TODO: required?
  *dpiX = 72.0;
  *dpiY = 72.0;
}

void wxPdfGraphicsContext::BeginLayer(wxDouble opacity)
{
#if 0
    m_layerOpacities.push_back(opacity);
    cairo_push_group(m_context);
#endif
}

void wxPdfGraphicsContext::EndLayer()
{
#if 0
    float opacity = m_layerOpacities.back();
    m_layerOpacities.pop_back();
    cairo_pop_group_to_source(m_context);
    cairo_paint_with_alpha(m_context,opacity);
#endif
}

void wxPdfGraphicsContext::Translate( wxDouble dx , wxDouble dy )
{
#if 0
    cairo_translate(m_context,dx,dy);
#endif
}

void wxPdfGraphicsContext::Scale( wxDouble xScale , wxDouble yScale )
{
#if 0
    cairo_scale(m_context,xScale,yScale);
#endif
}

void wxPdfGraphicsContext::Rotate( wxDouble angle )
{
#if 0
    cairo_rotate(m_context,angle);
#endif
}

// concatenates this transform with the current transform of this context
void wxPdfGraphicsContext::ConcatTransform( const wxGraphicsMatrix& matrix )
{
#if 0
    cairo_transform(m_context,(const cairo_matrix_t *) matrix.GetNativeMatrix());
#endif
}

// sets the transform of this context
void wxPdfGraphicsContext::SetTransform( const wxGraphicsMatrix& matrix )
{
#if 0
    cairo_set_matrix(m_context,(const cairo_matrix_t*) matrix.GetNativeMatrix());
#endif
}

// gets the matrix of this context
wxGraphicsMatrix
wxPdfGraphicsContext::GetTransform() const
{
  wxGraphicsMatrix matrix = CreateMatrix();
#if 0
    cairo_get_matrix(m_context,(cairo_matrix_t*) matrix.GetNativeMatrix());
#endif
  return matrix;
}

void
wxPdfGraphicsContext::SetPen(const wxGraphicsPen& pen)
{
  if (!pen.IsNull())
  {
    m_pen = pen;
    ((wxPdfGraphicsPenData*) m_pen.GetRefData())->Apply(this);
  }
}

void
wxPdfGraphicsContext::SetBrush(const wxGraphicsBrush& brush)
{
  if (!brush.IsNull())
  {
    m_brush = brush;
    ((wxPdfGraphicsBrushData*) m_brush.GetRefData())->Apply(this);
  }
}

void
wxPdfGraphicsContext::SetFont(const wxGraphicsFont& font)
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::SetFont - no valid PDF document"));
  if (!font.IsNull())
  {
    m_font = font;
    ((wxPdfGraphicsFontData*) m_font.GetRefData())->Apply(this);
  }
}

void
wxPdfGraphicsContext::StrokePath(const wxGraphicsPath& path)
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::StrokePath - no valid PDF document"));
  if (!m_pen.IsNull())
  {
    const wxPdfShape& shape = ((wxPdfGraphicsPathData*) path.GetRefData())->GetPdfShape();
    m_pdfDocument->Shape(shape, wxPDF_STYLE_DRAW);
  }
}

void wxPdfGraphicsContext::FillPath( const wxGraphicsPath& path , wxPolygonFillMode fillStyle )
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::FillPath - no valid PDF document"));

  if (!m_brush.IsNull())
  {
    const wxPdfShape& shape = ((wxPdfGraphicsPathData*) path.GetRefData())->GetPdfShape();
    m_pdfDocument->Shape(shape, wxPDF_STYLE_FILL);
  }
}

void
wxPdfGraphicsContext::DrawPath(const wxGraphicsPath& path, wxPolygonFillMode fillStyle)
{
  FillPath(path, fillStyle);
  StrokePath(path);
}

void
wxPdfGraphicsContext::GetTextExtent(const wxString& str, 
                                    wxDouble* width, wxDouble* height,
                                    wxDouble* descent, wxDouble* externalLeading) const
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::GetTextExtent - no valid PDF document"));
  wxCHECK_RET(!m_font.IsNull(), wxT("wxPdfGraphicsContext::GetTextExtent - no valid font set") );

  if (width)           *width = 0;
  if (height)          *height = 0;
  if (descent)         *descent = 0;
  if (externalLeading) *externalLeading = 0;

  if (str.empty()) return;

  wxPdfGraphicsFontData* fontData = (wxPdfGraphicsFontData*) m_font.GetRefData();
  if (fontData)
  {
    wxPdfFontDescription desc = m_pdfDocument->GetFontDescription();
    double myAscent, myDescent, myHeight, myExtLeading;
    CalculateFontMetrics(&desc, fontData->GetSize(), &myHeight, &myAscent, &myDescent, &myExtLeading);

    if (width)
    {
      *width = m_pdfDocument->GetStringWidth(str);
    }
    if (height)
    {
      *height = myHeight;
    }
    if (descent)
    {
      *descent = abs(myDescent);
    }
    if( externalLeading )
    {
      *externalLeading = myExtLeading;
    }
  }
}

void wxPdfGraphicsContext::GetPartialTextExtents(const wxString& text, wxArrayDouble& widths) const
{
#if 0
    widths.Empty();
    wxCHECK_RET( !m_font.IsNull(), wxT("wxPdfGraphicsContext::GetPartialTextExtents - no valid font set") );
#if __WXGTK__
    const wxCharBuffer data = text.utf8_str();
    int w = 0;
    if (data.length())
    {
        PangoLayout* layout = pango_cairo_create_layout(m_context);
        const wxFont& font = static_cast<wxPdfGraphicsFontData*>(m_font.GetRefData())->GetFont();
        pango_layout_set_font_description(layout, font.GetNativeFontInfo()->description);
        pango_layout_set_text(layout, data, data.length());
        PangoLayoutIter* iter = pango_layout_get_iter(layout);
        PangoRectangle rect;
        do {
            pango_layout_iter_get_cluster_extents(iter, NULL, &rect);
            w += rect.width;
            widths.Add(PANGO_PIXELS(w));
        } while (pango_layout_iter_next_cluster(iter));
        pango_layout_iter_free(iter);
        g_object_unref(layout);
    }
    size_t i = widths.GetCount();
    const size_t len = text.length();
    while (i++ < len)
        widths.Add(PANGO_PIXELS(w));
#else
    // TODO
#endif
#endif
}

void
wxPdfGraphicsContext::DrawBitmap(const wxBitmap& bmp, wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  wxGraphicsBitmap bitmap = GetRenderer()->CreateBitmap(bmp);
  DrawBitmap(bitmap, x, y, w, h);
}

void
wxPdfGraphicsContext::DrawBitmap(const wxGraphicsBitmap& bmp, wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
#if 0
    PushState();

    // In case we're scaling the image by using a width and height different
    // than the bitmap's size create a pattern transformation on the surface and
    // draw the transformed pattern.
    wxPdfGraphicsBitmapData* data = static_cast<wxPdfGraphicsBitmapData*>(bmp.GetRefData());
    cairo_pattern_t* pattern = data->GetCairoPattern();
    wxSize size = data->GetSize();

    wxDouble scaleX = w / size.GetWidth();
    wxDouble scaleY = h / size.GetHeight();

    // prepare to draw the image
    cairo_translate(m_context, x, y);
    cairo_scale(m_context, scaleX, scaleY);
    cairo_set_source(m_context, pattern);
    // use the original size here since the context is scaled already...
    cairo_rectangle(m_context, 0, 0, size.GetWidth(), size.GetHeight());
    // fill the rectangle using the pattern
    cairo_fill(m_context);

    PopState();
#endif
}

void
wxPdfGraphicsContext::DrawIcon(const wxIcon& icon, wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  // An icon is a bitmap on wxGTK, so do this the easy way.  When we want to
  // start using the Cairo backend on other platforms then we may need to
  // fiddle with this...
  DrawBitmap(icon, x, y, w, h);
}

void
wxPdfGraphicsContext::StrokeLine(wxDouble x1, wxDouble y1, wxDouble x2, wxDouble y2)
{
  wxGraphicsPath path = CreatePath();
  path.MoveToPoint(x1, y1);
  path.AddLineToPoint( x2, y2 );
  StrokePath( path );
}

void
wxPdfGraphicsContext::StrokeLines( size_t n, const wxPoint2DDouble *points)
{
  wxASSERT(n > 1);
  wxGraphicsPath path = CreatePath();
  path.MoveToPoint(points[0].m_x, points[0].m_y);
  for ( size_t i = 1; i < n; ++i)
    path.AddLineToPoint( points[i].m_x, points[i].m_y );
  StrokePath( path );
}

void
wxPdfGraphicsContext::StrokeLines( size_t n, const wxPoint2DDouble *beginPoints, const wxPoint2DDouble *endPoints)
{
  wxASSERT(n > 0);
  wxGraphicsPath path = CreatePath();
  for ( size_t i = 0; i < n; ++i)
  {
    path.MoveToPoint(beginPoints[i].m_x, beginPoints[i].m_y);
    path.AddLineToPoint( endPoints[i].m_x, endPoints[i].m_y );
  }
  StrokePath( path );
}

void
wxPdfGraphicsContext::DrawLines( size_t n, const wxPoint2DDouble *points, wxPolygonFillMode fillStyle)
{
  wxASSERT(n > 1);
  wxGraphicsPath path = CreatePath();
  path.MoveToPoint(points[0].m_x, points[0].m_y);
  for ( size_t i = 1; i < n; ++i)
    path.AddLineToPoint( points[i].m_x, points[i].m_y );
  DrawPath( path , fillStyle);
}

void
wxPdfGraphicsContext::DrawRectangle( wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  wxGraphicsPath path = CreatePath();
  path.AddRectangle( x , y , w , h );
  DrawPath( path );
}

void wxPdfGraphicsContext::DrawEllipse( wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  wxGraphicsPath path = CreatePath();
  path.AddEllipse(x,y,w,h);
  DrawPath(path);
}

void wxPdfGraphicsContext::DrawRoundedRectangle( wxDouble x, wxDouble y, wxDouble w, wxDouble h, wxDouble radius)
{
  wxGraphicsPath path = CreatePath();
  path.AddRoundedRectangle(x,y,w,h,radius);
  DrawPath(path);
}

void
wxPdfGraphicsContext::DoDrawText(const wxString& str, wxDouble x, wxDouble y)
{
  wxCHECK_RET( !m_font.IsNull(), wxT("wxPdfGraphicsContext::DrawText - no valid font set") );

  if (!str.empty())
  {
    //double pointSize = static_cast<wxPdfGraphicsFontData*>(m_font.GetRefData())->GetSize();
    double pointSize = m_pdfDocument->GetFontSize();
    wxPdfFontDescription desc = m_pdfDocument->GetFontDescription();
    double height, descent;
    CalculateFontMetrics(&desc, pointSize, &height, NULL, &descent, NULL);
    if (m_mappingModeStyle != wxPDF_MAPMODESTYLE_PDF)
    {
      y += (height - abs(descent));
    }
    m_pdfDocument->Text(x, y, str);
  }
}

void
wxPdfGraphicsContext::CalculateFontMetrics(wxPdfFontDescription* desc, double pointSize,
                                           double* height, double* ascent, 
                                           double* descent, double* extLeading) const
{
  double em_height, em_ascent, em_descent, em_externalLeading;
  int hheaAscender, hheaDescender, hheaLineGap;

  double size = pointSize;
#if 0
  if ((m_mappingModeStyle == wxPDF_MAPMODESTYLE_PDF) && (m_mappingMode != wxMM_TEXT))
  {
    size = pointSize;
  }
  else
  {
    size = pointSize * (m_ppiPdfFont / 72.0);
  }
#endif
  int os2sTypoAscender, os2sTypoDescender, os2sTypoLineGap, os2usWinAscent, os2usWinDescent;

  desc->GetOpenTypeMetrics(&hheaAscender, &hheaDescender, &hheaLineGap,
                           &os2sTypoAscender, &os2sTypoDescender, &os2sTypoLineGap,
                           &os2usWinAscent, &os2usWinDescent);

  if (hheaAscender)
  {
    em_ascent  = os2usWinAscent;
    em_descent = os2usWinDescent;
    em_externalLeading = (hheaLineGap - ((em_ascent + em_descent) - (hheaAscender - hheaDescender)));
    em_height = em_ascent + em_descent;
  }
  else
  {
    // magic numbers 1120 and 25 give reasonable defaults
    // for core fonts etc.
    // This may need adjustment for CJK fonts ??
    em_height  = 1120;
    em_descent = desc->GetDescent();
    em_ascent  = em_height + em_descent;
    em_externalLeading = 25;
  }
    
  if (ascent)
  {
    *ascent = em_ascent * size / 1000.0;
  }
  if (descent)
  {
    *descent = em_descent * size / 1000.0;
  }
  if (height)
  {
    *height = em_height * size / 1000.0;
  }
  if (extLeading)
  {
    *extLeading = em_externalLeading * size / 1000.0;
  }
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsRenderer declaration
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_PDFDOC wxPdfGraphicsRenderer : public wxGraphicsRenderer
{
public :
  wxPdfGraphicsRenderer() {}

  virtual ~wxPdfGraphicsRenderer() {}

  // Context
  virtual wxGraphicsContext* CreateContext(const wxWindowDC& dc);
  virtual wxGraphicsContext* CreateContext(const wxMemoryDC& dc);
#if wxUSE_PRINTING_ARCHITECTURE
  virtual wxGraphicsContext* CreateContext(const wxPrinterDC& dc);
#endif
#ifdef __WXMSW__
#if wxUSE_ENH_METAFILE
  virtual wxGraphicsContext* CreateContext(const wxEnhMetaFileDC& dc);
#endif
#endif

  virtual wxGraphicsContext* CreateContextFromNativeContext(void* context);

  virtual wxGraphicsContext* CreateContextFromNativeWindow(void* window);

  virtual wxGraphicsContext* CreateContext(wxWindow* window);

#if wxUSE_IMAGE
  virtual wxGraphicsContext* CreateContextFromImage(wxImage& image);
#endif // wxUSE_IMAGE

  virtual wxGraphicsContext* CreateMeasuringContext();

  // Path
  virtual wxGraphicsPath CreatePath();

  // Matrix
  virtual wxGraphicsMatrix CreateMatrix(wxDouble a = 1.0, wxDouble b = 0.0, 
                                        wxDouble c = 0.0, wxDouble d = 1.0,
                                        wxDouble tx = 0.0, wxDouble ty = 0.0);

  virtual wxGraphicsPen CreatePen(const wxPen& pen);

  virtual wxGraphicsBrush CreateBrush(const wxBrush& brush);

  virtual wxGraphicsBrush CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                                                    wxDouble x2, wxDouble y2,
                                                    const wxGraphicsGradientStops& stops);

   virtual wxGraphicsBrush CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                                     wxDouble xc, wxDouble yc,
                                                     wxDouble radius,
                                                     const wxGraphicsGradientStops& stops);

  // sets the font
  virtual wxGraphicsFont CreateFont(const wxFont& font, const wxColour& col = *wxBLACK);
  virtual wxGraphicsFont CreateFont(double sizeInPixels,
                                    const wxString& facename,
                                    int flags = wxFONTFLAG_DEFAULT,
                                    const wxColour& col = *wxBLACK);

  // create a native bitmap representation
  virtual wxGraphicsBitmap CreateBitmap(const wxBitmap& bitmap);
#if wxUSE_IMAGE
  virtual wxGraphicsBitmap CreateBitmapFromImage(const wxImage& image);
  virtual wxImage CreateImageFromBitmap(const wxGraphicsBitmap& bmp);
#endif // wxUSE_IMAGE

  // create a graphics bitmap from a native bitmap
  virtual wxGraphicsBitmap CreateBitmapFromNativeBitmap(void* bitmap);

  // create a subimage from a native image representation
  virtual wxGraphicsBitmap CreateSubBitmap(const wxGraphicsBitmap &bitmap,
                                           wxDouble x, wxDouble y, 
                                           wxDouble w, wxDouble h);

protected :

private :

  DECLARE_DYNAMIC_CLASS_NO_COPY(wxPdfGraphicsRenderer)
};

//-----------------------------------------------------------------------------
// wxPdfGraphicsRenderer implementation
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPdfGraphicsRenderer,wxGraphicsRenderer)

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxWindowDC& dc)
{
  wxUnusedVar(dc);
  return NULL;
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxMemoryDC& dc)
{
  wxUnusedVar(dc);
  return NULL;
}

#if wxUSE_PRINTING_ARCHITECTURE
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxPrinterDC& dc)
{
  wxUnusedVar(dc);
  return NULL;
}
#endif

#ifdef __WXMSW__
#if wxUSE_ENH_METAFILE
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxEnhMetaFileDC& dc)
{
  wxUnusedVar(dc);
  return NULL;
}
#endif
#endif

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromNativeContext(void* context)
{
  wxUnusedVar(context);
  return NULL;
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromNativeWindow(void* window)
{
  wxUnusedVar(window);
  return NULL;
}

wxGraphicsContext * wxPdfGraphicsRenderer::CreateContext(wxWindow* window)
{
  wxUnusedVar(window);
  return NULL;
}

#if wxUSE_IMAGE
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromImage(wxImage& image)
{
  wxUnusedVar(image);
  return NULL;
}
#endif // wxUSE_IMAGE

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateMeasuringContext()
{
  return NULL;
}

// Path

wxGraphicsPath
wxPdfGraphicsRenderer::CreatePath()
{
  wxGraphicsPath path;
  path.SetRefData(new wxPdfGraphicsPathData(this));
  return path;
}


// Matrix

wxGraphicsMatrix
wxPdfGraphicsRenderer::CreateMatrix(wxDouble a, wxDouble b, wxDouble c, wxDouble d,
                                    wxDouble tx, wxDouble ty)
{
  wxGraphicsMatrix m;
  wxPdfGraphicsMatrixData* data = new wxPdfGraphicsMatrixData(this);
  data->Set(a, b, c, d, tx, ty);
  m.SetRefData(data);
  return m;
}

wxGraphicsPen
wxPdfGraphicsRenderer::CreatePen(const wxPen& pen)
{
  if (!pen.IsOk() || pen.GetStyle() == wxPENSTYLE_TRANSPARENT)
  {
    return wxNullGraphicsPen;
  }
  else
  {
    wxGraphicsPen p;
    p.SetRefData(new wxPdfGraphicsPenData(this, pen));
    return p;
  }
}

wxGraphicsBrush
wxPdfGraphicsRenderer::CreateBrush(const wxBrush& brush)
{
  if (!brush.IsOk() || brush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT)
  {
    return wxNullGraphicsBrush;
  }
  else
  {
    wxGraphicsBrush p;
    p.SetRefData(new wxPdfGraphicsBrushData(this, brush));
    return p;
  }
}

wxGraphicsBrush
wxPdfGraphicsRenderer::CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                                                 wxDouble x2, wxDouble y2,
                                                 const wxGraphicsGradientStops& stops)
{
  wxGraphicsBrush p;
  wxPdfGraphicsBrushData* d = new wxPdfGraphicsBrushData(this);
  d->CreateLinearGradientBrush(x1, y1, x2, y2, stops);
  p.SetRefData(d);
  return p;
}

wxGraphicsBrush
wxPdfGraphicsRenderer::CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                                 wxDouble xc, wxDouble yc, wxDouble r,
                                                 const wxGraphicsGradientStops& stops)
{
  wxGraphicsBrush p;
  wxPdfGraphicsBrushData* d = new wxPdfGraphicsBrushData(this);
  d->CreateRadialGradientBrush(xo, yo, xc, yc, r, stops);
  p.SetRefData(d);
  return p;
}

wxGraphicsFont
wxPdfGraphicsRenderer::CreateFont(const wxFont& font, const wxColour& col)
{
  if (font.IsOk())
  {
    wxGraphicsFont p;
    p.SetRefData(new wxPdfGraphicsFontData(this, font, col));
    return p;
  }
  else
  {
    return wxNullGraphicsFont;
  }
}

wxGraphicsFont
wxPdfGraphicsRenderer::CreateFont(double sizeInPixels,
                                  const wxString& facename,
                                  int flags,
                                  const wxColour& col)
{
  wxGraphicsFont font;
  font.SetRefData(new wxPdfGraphicsFontData(this, sizeInPixels, facename, flags, col));
  return font;
}

wxGraphicsBitmap
wxPdfGraphicsRenderer::CreateBitmap(const wxBitmap& bmp)
{
  if (bmp.IsOk())
  {
    wxGraphicsBitmap p;
    p.SetRefData(new wxPdfGraphicsBitmapData(this, bmp));
    return p;
  }
  else
  {
    return wxNullGraphicsBitmap;
  }
}

#if wxUSE_IMAGE

wxGraphicsBitmap
wxPdfGraphicsRenderer::CreateBitmapFromImage(const wxImage& image)
{
  wxGraphicsBitmap bmp;
  if (image.IsOk())
  {
    bmp.SetRefData(new wxPdfGraphicsBitmapData(this, image));
  }
  return bmp;
}

wxImage
wxPdfGraphicsRenderer::CreateImageFromBitmap(const wxGraphicsBitmap& bmp)
{
  const wxPdfGraphicsBitmapData* const data = 
    static_cast<wxPdfGraphicsBitmapData*>(bmp.GetGraphicsData());

  return data ? data->ConvertToImage() : wxNullImage;
}

#endif // wxUSE_IMAGE

wxGraphicsBitmap
wxPdfGraphicsRenderer::CreateBitmapFromNativeBitmap(void* bitmap)
{
  wxUnusedVar(bitmap);
  wxFAIL_MSG("wxPdfGraphicsRenderer::CreateBitmapFromNativeBitmap is not implemented.");
  return wxNullGraphicsBitmap;
}

wxGraphicsBitmap
wxPdfGraphicsRenderer::CreateSubBitmap(const wxGraphicsBitmap& bitmap,
                                       wxDouble x, wxDouble y,
                                       wxDouble w, wxDouble h)
{
  wxUnusedVar(bitmap);
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxUnusedVar(w);
  wxUnusedVar(h);
  wxFAIL_MSG("wxPdfGraphicsRenderer::CreateSubBitmap is not implemented.");
  return wxNullGraphicsBitmap;
}


#if 0 // wxPdfDC

//-------------------------------------------------------------------------------
// wxPostScriptDC
//-------------------------------------------------------------------------------


IMPLEMENT_DYNAMIC_CLASS(wxPdfDC, wxDC)

wxPdfDC::wxPdfDC()
  : wxDC(new wxPdfDCImpl(this))
{
}

wxPdfDC::wxPdfDC(const wxPrintData& printData)
  : wxDC(new wxPdfDCImpl(this, printData))
{
}

wxPdfDC::wxPdfDC(wxPdfDocument* pdfDocument, double templateWidth, double templateHeight)
  : wxDC(new wxPdfDCImpl(this, pdfDocument, templateWidth, templateHeight))
{
}

wxPdfDocument*
wxPdfDC::GetPdfDocument()
{
  return ((wxPdfDCImpl*) m_pimpl)->GetPdfDocument();
}

void
wxPdfDC::SetResolution(int ppi)
{
  ((wxPdfDCImpl*) m_pimpl)->SetResolution(ppi);
}

int
wxPdfDC::GetResolution() const
{
  return ((wxPdfDCImpl*) m_pimpl)->GetResolution();
}

void
wxPdfDC::SetMapModeStyle(wxPdfMapModeStyle style)
{
  ((wxPdfDCImpl*) m_pimpl)->SetMapModeStyle(style);
}

wxPdfMapModeStyle
wxPdfDC::GetMapModeStyle() const
{
  return ((wxPdfDCImpl*) m_pimpl)->GetMapModeStyle();
}

static double
angleByCoords(wxCoord xa, wxCoord ya, wxCoord xc, wxCoord yc)
{
  static double pi = 4. * atan(1.0);
  double diffX = xa - xc, diffY = -(ya - yc), ret = 0;
  if (diffX == 0) // singularity
  {
    ret = diffY > 0 ? 90 : -90;
  }
  else if (diffX >= 0) // quadrants 1, 4
  {
    ret = (atan(diffY / diffX) * 180.0 / pi);
  }
  else // quadrants 2, 3
  {
    ret = 180 + (atan(diffY / diffX) * 180.0 / pi);
  }
  return ret;
}

/*
 * PDF device context
 *
 */

IMPLEMENT_ABSTRACT_CLASS(wxPdfDCImpl, wxDCImpl)

wxPdfDCImpl::wxPdfDCImpl(wxPdfDC* owner)
  : wxDCImpl(owner)
{
  Init();
  m_ok = true;
}

wxPdfDCImpl::wxPdfDCImpl(wxPdfDC* owner, const wxPrintData& data)
  : wxDCImpl(owner)
{
  Init();
  SetPrintData(data);
  m_ok = true;
}

wxPdfDCImpl::wxPdfDCImpl(wxPdfDC* owner, const wxString& file, int w, int h)
  : wxDCImpl(owner)
{
  Init();
  m_printData.SetFilename(file);
  m_ok = true;
}

wxPdfDCImpl::wxPdfDCImpl(wxPdfDC* owner, wxPdfDocument* pdfDocument, double templateWidth, double templateHeight)
  : wxDCImpl(owner)
{
  Init();
  m_templateMode = true;
  m_templateWidth = templateWidth;
  m_templateHeight = templateHeight;
  m_pdfDocument = pdfDocument;
}

wxPdfDCImpl::~wxPdfDCImpl()
{
  if (m_pdfDocument != NULL)
  {
    if (!m_templateMode)
    {
      delete m_pdfDocument;
    }
  }
}

void
wxPdfDCImpl::Init()
{
  m_templateMode = false;
  m_ppi = 72;
  m_pdfDocument = NULL;
  m_imageCount = 0;

  wxScreenDC screendc;
  m_ppiPdfFont = screendc.GetPPI().GetHeight();
  m_mappingModeStyle = wxPDF_MAPMODESTYLE_STANDARD;

  SetBackgroundMode(wxSOLID);
  
  m_printData.SetOrientation(wxPORTRAIT);
  m_printData.SetPaperId(wxPAPER_A4);
  m_printData.SetFilename(wxT("default.pdf"));
}

wxPdfDocument*
wxPdfDCImpl::GetPdfDocument()
{
  return m_pdfDocument;
}

void
wxPdfDCImpl::SetPrintData(const wxPrintData& data)
{
  m_printData = data;
  wxPaperSize id = m_printData.GetPaperId();
  wxPrintPaperType* paper = wxThePrintPaperDatabase->FindPaperType(id);
  if (!paper)
  {
    m_printData.SetPaperId(wxPAPER_A4);
  }
}

void
wxPdfDCImpl::SetResolution(int ppi)
{
  m_ppi = ppi;
}

int
wxPdfDCImpl::GetResolution() const
{
  return (int) m_ppi;
}

void
wxPdfDCImpl::Clear()
{
  // Not yet implemented
}

bool
wxPdfDCImpl::StartDoc(const wxString& message)
{
  wxCHECK_MSG(m_ok, false, wxT("Invalid PDF DC"));
  wxUnusedVar(message);
  if (!m_templateMode && m_pdfDocument == NULL)
  {
    m_pdfDocument = new wxPdfDocument(m_printData.GetOrientation(), wxString(wxT("pt")), m_printData.GetPaperId());
    m_pdfDocument->Open();
    //m_pdfDocument->SetCompression(false);
    m_pdfDocument->SetAuthor(wxT("wxPdfDC"));
    m_pdfDocument->SetTitle(wxT("wxPdfDC"));

    SetBrush(*wxBLACK_BRUSH);
    SetPen(*wxBLACK_PEN);
    SetBackground(*wxWHITE_BRUSH);
    SetTextForeground(*wxBLACK);
    SetDeviceOrigin(0, 0);
  }
  return true;
}

void
wxPdfDCImpl::EndDoc()
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (!m_templateMode)
  {
    m_pdfDocument->SaveAsFile(m_printData.GetFilename());
    delete m_pdfDocument;
    m_pdfDocument = NULL;
  }
}

void
wxPdfDCImpl::StartPage()
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (!m_templateMode)
  {
    // Begin a new page
    // Library needs it this way (always landscape) to size the page correctly
    m_pdfDocument->AddPage(m_printData.GetOrientation());
    wxPdfLineStyle style = m_pdfDocument->GetLineStyle();
    style.SetWidth(1.0);
    style.SetColour(wxPdfColour(0, 0, 0));
    style.SetLineCap(wxPDF_LINECAP_ROUND);
    style.SetLineJoin(wxPDF_LINEJOIN_MITER);
    m_pdfDocument->SetLineStyle(style);
  }
}

void
wxPdfDCImpl::EndPage()
{
  if (m_ok)
  {
    if (m_clipping)
    {
      DestroyClippingRegion();
    }
  }
}

void
wxPdfDCImpl::SetFont(const wxFont& font)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  m_font = font;
  if (!font.IsOk())
  {
    return;
  }
  int styles = wxPDF_FONTSTYLE_REGULAR;
  if (font.GetWeight() == wxFONTWEIGHT_BOLD)
  {
    styles |= wxPDF_FONTSTYLE_BOLD;
  }
  if (font.GetStyle() == wxFONTSTYLE_ITALIC)
  {
    styles |= wxPDF_FONTSTYLE_ITALIC;
  }
  if (font.GetUnderlined())
  {
    styles |= wxPDF_FONTSTYLE_UNDERLINE;
  }

  wxPdfFont regFont = wxPdfFontManager::GetFontManager()->GetFont(font.GetFaceName(), styles);
  bool ok = regFont.IsValid();
  if (!ok)
  {
    regFont = wxPdfFontManager::GetFontManager()->RegisterFont(font, font.GetFaceName());
    ok = regFont.IsValid();
  }

  if (ok)
  {
    ok = m_pdfDocument->SetFont(regFont, styles, ScaleFontSizeToPdf(font.GetPointSize()));    
  }
}

void
wxPdfDCImpl::SetPen(const wxPen& pen)
{
  if (pen.Ok())
  {
    m_pen = pen;
  }
}

void
wxPdfDCImpl::SetBrush(const wxBrush& brush)
{
  if (brush.Ok())
  {
    m_brush = brush;
  }
}

void
wxPdfDCImpl::SetBackground(const wxBrush& brush)
{
  if (brush.Ok())
  {
    m_backgroundBrush = brush;
  }
}

void
wxPdfDCImpl::SetBackgroundMode(int mode)
{
  // TODO: check implementation
  m_backgroundMode = (mode == wxTRANSPARENT) ? wxTRANSPARENT : wxSOLID;
}

void
wxPdfDCImpl::SetPalette(const wxPalette& palette)
{
  // Not yet implemented
  wxUnusedVar(palette);
}

void
wxPdfDCImpl::SetTextForeground(const wxColour& colour)
{
  if (colour.IsOk())
  {
    m_textForegroundColour = colour;
  }
}

void
wxPdfDCImpl::DestroyClippingRegion()
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (m_clipping)
  {
    m_pdfDocument->UnsetClipping();
    { 
      wxPen x(GetPen()); SetPen(x);
    }
    { 
      wxBrush x(GetBrush()); SetBrush(x);
    }
    {
      wxFont x(GetFont()); m_pdfDocument->SetFont(x);
    }
  }
  ResetClipping();
}

wxCoord
wxPdfDCImpl::GetCharHeight() const
{
  // default for 12 point font
  int height = 18;
  int width;
  if (m_font.Ok())
  {
    DoGetTextExtent("x", &width, &height);
  }
  return height;
}

wxCoord
wxPdfDCImpl::GetCharWidth() const
{
  int height;
  int width = 8;
  if (m_font.Ok())
  {
    DoGetTextExtent("x", &width, &height);
  }
  return width;
}

bool
wxPdfDCImpl::CanDrawBitmap() const
{
  return true;
}

bool
wxPdfDCImpl::CanGetTextExtent() const
{
  return true;
}

int
wxPdfDCImpl::GetDepth() const
{
  // TODO: check value
  return 32;
}

wxSize
wxPdfDCImpl::GetPPI() const
{
  int ppi = (int) m_ppi;
  return wxSize(ppi,ppi);
}

void 
wxPdfDCImpl::ComputeScaleAndOrigin()
{
  m_scaleX = m_logicalScaleX * m_userScaleX;
  m_scaleY = m_logicalScaleY * m_userScaleY;
}

void
wxPdfDCImpl::SetMapMode(wxMappingMode mode)
{
  m_mappingMode = mode;
  switch (mode)
  {
    case wxMM_TWIPS:
      SetLogicalScale(m_ppi / 1440.0, m_ppi / 1440.0);
      break;
    case wxMM_POINTS:
      SetLogicalScale(m_ppi / 72.0, m_ppi / 72.0);
      break;
    case wxMM_METRIC:
      SetLogicalScale(m_ppi / 25.4, m_ppi / 25.4);
      break;
    case wxMM_LOMETRIC:
      SetLogicalScale(m_ppi / 254.0, m_ppi / 254.0);
      break;
    default:
    case wxMM_TEXT:
      SetLogicalScale(1.0, 1.0);
      break;
  }
}

void
wxPdfDCImpl::SetUserScale(double x, double y)
{
  m_userScaleX = x;
  m_userScaleY = y;
  ComputeScaleAndOrigin();
}

void
wxPdfDCImpl::SetLogicalScale(double x, double y)
{
  m_logicalScaleX = x;
  m_logicalScaleY = y;
  ComputeScaleAndOrigin();
}

void
wxPdfDCImpl::SetLogicalOrigin(wxCoord x, wxCoord y)
{
  m_logicalOriginX = x * m_signX;
  m_logicalOriginY = y * m_signY;
  ComputeScaleAndOrigin();
}

void
wxPdfDCImpl::SetDeviceOrigin(wxCoord x, wxCoord y)
{
  m_deviceOriginX = x;
  m_deviceOriginY = y;
  ComputeScaleAndOrigin();
}

void
wxPdfDCImpl::SetAxisOrientation(bool xLeftRight, bool yBottomUp)
{
  m_signX = (xLeftRight ?  1 : -1);
  m_signY = (yBottomUp  ? -1 :  1);
  ComputeScaleAndOrigin();
}

void
wxPdfDCImpl::SetLogicalFunction(wxRasterOperationMode function)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  // TODO: check implementation
  m_logicalFunction = function;
  switch(function)
  {
    case wxAND:
      m_pdfDocument->SetAlpha(0.5, 0.5);
      break;
    case wxCOPY:
    default:
      m_pdfDocument->SetAlpha(1.0, 1.0);
      break;
  }
}

// the true implementations

bool
wxPdfDCImpl::DoFloodFill(wxCoord x, wxCoord y, const wxColour& col, wxFloodFillStyle style)
{
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxUnusedVar(col);
  wxUnusedVar(style);
  wxFAIL_MSG(wxString(wxT("wxPdfDCImpl::FloodFill: "))+_("Not implemented."));
  return false;
}

void
wxPdfDCImpl::DoGradientFillLinear(const wxRect& rect,
                              const wxColour& initialColour,
                              const wxColour& destColour,
                              wxDirection nDirection)
{
  // TODO: native implementation
  wxDCImpl::DoGradientFillLinear(rect, initialColour, destColour, nDirection);
}

void
wxPdfDCImpl::DoGradientFillConcentric(const wxRect& rect,
                                  const wxColour& initialColour,
                                  const wxColour& destColour,
                                  const wxPoint& circleCenter)
{
  // TODO: native implementation
  wxDCImpl::DoGradientFillConcentric(rect, initialColour, destColour, circleCenter);
}

bool
wxPdfDCImpl::DoGetPixel(wxCoord x, wxCoord y, wxColour* col) const
{
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxUnusedVar(col);
  wxFAIL_MSG(wxString(wxT("wxPdfDCImpl::DoGetPixel: "))+_("Not implemented."));
  return false;
}

void
wxPdfDCImpl::DoDrawPoint(wxCoord x, wxCoord y)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupPen();
  double xx = ScaleLogicalToPdfX(x);
  double yy = ScaleLogicalToPdfY(y);
  m_pdfDocument->SetFillColour(m_pdfDocument->GetDrawColour());
  m_pdfDocument->Rect(xx-0.5, yy-0.5, xx+0.5, yy+0.5);
  CalcBoundingBox(x, y);
}

#if wxUSE_SPLINES
void
wxPdfDCImpl::DoDrawSpline(wxList* points)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetPen( m_pen );
  wxASSERT_MSG( points, wxT("NULL pointer to spline points?") );
  const size_t n_points = points->GetCount();
  wxASSERT_MSG( n_points > 2 , wxT("incomplete list of spline points?") );
#if 0
  wxPoint* p;
  wxPdfArrayDouble xp, yp;
  wxList::compatibility_iterator node = points->GetFirst();
  while (node)
  {
    p = (wxPoint *)node->GetData();
    xp.Add(ScaleLogicalToPdfX(p->x));
    yp.Add(ScaleLogicalToPdfY(p->y));
    node = node->GetNext();
  }
  m_pdfDocument->BezierSpline(xp, yp, wxPDF_STYLE_DRAW);
#endif

  // Code taken from wxDC MSW implementation
  // quadratic b-spline to cubic bezier spline conversion
  //
  // quadratic spline with control points P0,P1,P2
  // P(s) = P0*(1-s)^2 + P1*2*(1-s)*s + P2*s^2
  //
  // bezier spline with control points B0,B1,B2,B3
  // B(s) = B0*(1-s)^3 + B1*3*(1-s)^2*s + B2*3*(1-s)*s^2 + B3*s^3
  //
  // control points of bezier spline calculated from b-spline
  // B0 = P0
  // B1 = (2*P1 + P0)/3
  // B2 = (2*P1 + P2)/3
  // B3 = P2

  double x1, y1, x2, y2, cx1, cy1, cx4, cy4;
  double bx1, by1, bx2, by2, bx3, by3;

  wxList::compatibility_iterator node = points->GetFirst();
  wxPoint* p = (wxPoint*) node->GetData();

  x1 = ScaleLogicalToPdfX(p->x);
  y1 = ScaleLogicalToPdfY(p->y);
  m_pdfDocument->MoveTo(x1, y1);

  node = node->GetNext();
  p = (wxPoint*) node->GetData();

  bx1 = x2 = ScaleLogicalToPdfX(p->x);
  by1 = y2 = ScaleLogicalToPdfY(p->y);
  cx1 = ( x1 + x2 ) / 2;
  cy1 = ( y1 + y2 ) / 2;
  bx3 = bx2 = cx1;
  by3 = by2 = cy1;
  m_pdfDocument->CurveTo(bx1, by1, bx2, by2, bx3, by3);

#if !wxUSE_STL
  while ((node = node->GetNext()) != NULL)
#else
  while ((node = node->GetNext()))
#endif // !wxUSE_STL
  {
    p = (wxPoint*) node->GetData();
    x1 = x2;
    y1 = y2;
    x2 = ScaleLogicalToPdfX(p->x);
    y2 = ScaleLogicalToPdfY(p->y);
    cx4 = (x1 + x2) / 2;
    cy4 = (y1 + y2) / 2;
    // B0 is B3 of previous segment
    // B1:
    bx1 = (x1*2+cx1)/3;
    by1 = (y1*2+cy1)/3;
    // B2:
    bx2 = (x1*2+cx4)/3;
    by2 = (y1*2+cy4)/3;
    // B3:
    bx3 = cx4;
    by3 = cy4;
    cx1 = cx4;
    cy1 = cy4;
    m_pdfDocument->CurveTo(bx1, by1, bx2, by2, bx3, by3);
  }

  bx1 = bx3;
  by1 = by3;
  bx3 = bx2 = x2;
  by3 = by2 = y2;
  m_pdfDocument->CurveTo(bx1, by1, bx2, by2, bx3, by3);
  m_pdfDocument->EndPath(wxPDF_STYLE_DRAW);
}
#endif

void
wxPdfDCImpl::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if  (m_pen.GetStyle() != wxTRANSPARENT)
  {
    SetupBrush();
    SetupPen();
    m_pdfDocument->Line(ScaleLogicalToPdfX(x1), ScaleLogicalToPdfY(y1), 
                        ScaleLogicalToPdfX(x2), ScaleLogicalToPdfY(y2));
    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
  }
}

void
wxPdfDCImpl::DoDrawArc(wxCoord x1, wxCoord y1,
                       wxCoord x2, wxCoord y2,
                       wxCoord xc, wxCoord yc)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupBrush();
  SetupPen();
  const wxBrush& curBrush = GetBrush();
  const wxPen& curPen = GetPen();
  bool doFill = (curBrush != wxNullBrush) && curBrush.GetStyle() != wxTRANSPARENT;
  bool doDraw = (curPen != wxNullPen) && curPen.GetStyle() != wxTRANSPARENT;
  if (doDraw || doFill)
  {
    double xx1 = x1;
    double yy1 = y1;
    double xx2 = x2;
    double yy2 = y2;
    double xxc = xc;
    double yyc = yc;
    double start = angleByCoords(xx1, yy1, xxc, yyc);
    double end   = angleByCoords(xx2, yy2, xxc, yyc);
    xx1 = ScaleLogicalToPdfX(xx1);
    yy1 = ScaleLogicalToPdfY(yy1);
    xx2 = ScaleLogicalToPdfX(xx2);
    yy2 = ScaleLogicalToPdfY(yy2);
    xxc = ScaleLogicalToPdfX(xxc);
    yyc = ScaleLogicalToPdfY(yyc);
    double rx = xx1 - xxc;
    double ry = yy1 - yyc;
    double r = sqrt(rx * rx + ry * ry);
    int style = wxPDF_STYLE_FILLDRAW;
    if (!(doDraw && doFill))
    {
      style = (doFill) ? wxPDF_STYLE_FILL : wxPDF_STYLE_DRAW;
    }
    m_pdfDocument->Ellipse(xxc, yyc, r, 0, 0, start, end, style, 8, false);
    wxCoord radius = (wxCoord) sqrt( (double)((x1-xc)*(x1-xc)+(y1-yc)*(y1-yc)) );
    CalcBoundingBox(xc-radius, yc-radius);
    CalcBoundingBox(xc+radius, yc+radius);
  }
}

void
wxPdfDCImpl::DoDrawCheckMark(wxCoord x, wxCoord y,
                             wxCoord width, wxCoord height)
{
  // TODO: native implementation?
  wxDCImpl::DoDrawCheckMark(x, y, width, height);
}

void
wxPdfDCImpl::DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord width, wxCoord height,
                               double sa, double ea)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (sa >= 360 || sa <= -360)
  {
    sa -= int(sa/360)*360;
  }
  if (ea >= 360 || ea <=- 360)
  {
    ea -= int(ea/360)*360;
  }
  if (sa < 0)
  {
    sa += 360;
  }
  if (ea < 0)
  {
    ea += 360;
  }
  if (wxIsSameDouble(sa, ea))
  {
    DoDrawEllipse(x, y, width, height);
  }
  else
  {
    SetupBrush();
    SetupPen();
    const wxBrush& curBrush = GetBrush();
    const wxPen& curPen = GetPen();
    bool doFill = (curBrush != wxNullBrush) && curBrush.GetStyle() != wxTRANSPARENT;
    bool doDraw = (curPen != wxNullPen) && curPen.GetStyle() != wxTRANSPARENT;
    if (doDraw || doFill)
    {
      m_pdfDocument->SetLineWidth(ScaleLogicalToPdfXRel(1)); // pen width != 1 sometimes fools readers when closing paths
      int style = wxPDF_STYLE_FILL | wxPDF_STYLE_DRAWCLOSE;
      if (!(doDraw && doFill))
      {
        style = (doFill) ? wxPDF_STYLE_FILL : wxPDF_STYLE_DRAWCLOSE;
      }
      m_pdfDocument->Ellipse(ScaleLogicalToPdfX(x + 0.5 * width), 
                             ScaleLogicalToPdfY(y + 0.5 * height),
                             ScaleLogicalToPdfXRel(0.5 * width), 
                             ScaleLogicalToPdfYRel(0.5 * height), 
                             0, sa, ea, style, 8, true);
      CalcBoundingBox(x, y);
      CalcBoundingBox(x+width, y+height);
    }
  }
}

void
wxPdfDCImpl::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupBrush();
  SetupPen();
  m_pdfDocument->Rect(ScaleLogicalToPdfX(x), ScaleLogicalToPdfY(y),
                      ScaleLogicalToPdfXRel(width), ScaleLogicalToPdfYRel(height), 
                      GetDrawingStyle()); 
  CalcBoundingBox(x, y);
  CalcBoundingBox(x+width, y+height);
}

void
wxPdfDCImpl::DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                    wxCoord width, wxCoord height,
                                    double radius)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (radius < 0.0)
  {
    // Now, a negative radius is interpreted to mean
    // 'the proportion of the smallest X or Y dimension'
    double smallest = width < height ? width : height;
    radius =  (-radius * smallest);
  }
  SetupBrush();
  SetupPen();
  m_pdfDocument->RoundedRect(ScaleLogicalToPdfX(x), ScaleLogicalToPdfY(y), 
                             ScaleLogicalToPdfXRel(width), ScaleLogicalToPdfYRel(height), 
                             ScaleLogicalToPdfXRel(radius), wxPDF_CORNER_ALL, GetDrawingStyle());  
  CalcBoundingBox(x, y);
  CalcBoundingBox(x+width, y+height);
}

void
wxPdfDCImpl::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupBrush();
  SetupPen();
  m_pdfDocument->Ellipse(ScaleLogicalToPdfX(x + width / 2.0), 
                         ScaleLogicalToPdfY(y + height / 2.0), 
                         ScaleLogicalToPdfXRel(width / 2.0), 
                         ScaleLogicalToPdfYRel(height / 2.0), 
                         0, 0, 360, GetDrawingStyle()); 
  CalcBoundingBox(x-width, y-height);
  CalcBoundingBox(x+width, y+height);
}

void
wxPdfDCImpl::DoCrossHair(wxCoord x, wxCoord y)
{
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxFAIL_MSG(wxString(wxT("wxPdfDCImpl::DoCrossHair: "))+_("Not implemented."));
}

void
wxPdfDCImpl::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
  DoDrawBitmap(icon, x, y, true);
}

void
wxPdfDCImpl::DoDrawBitmap(const wxBitmap& bitmap, wxCoord x, wxCoord y, bool useMask)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  wxCHECK_RET( IsOk(), wxT("wxPdfDCImpl::DoDrawBitmap - invalid DC") );
  wxCHECK_RET( bitmap.Ok(), wxT("wxPdfDCImpl::DoDrawBitmap - invalid bitmap") );

  if (!bitmap.Ok()) return;

  int idMask = 0;
  wxImage image = bitmap.ConvertToImage();
  if (!image.Ok()) return;

  if (!useMask)
  {
    image.SetMask(false);
  }
  wxCoord w = image.GetWidth();
  wxCoord h = image.GetHeight();

  wxCoord ww = ScaleLogicalToPdfXRel(w);
  wxCoord hh = ScaleLogicalToPdfYRel(h);

  wxCoord xx = ScaleLogicalToPdfX(x);
  wxCoord yy = ScaleLogicalToPdfY(y);

  wxString imgName = wxString::Format(wxT("pdfdcimg%d"), ++m_imageCount);

  if (bitmap.GetDepth() == 1)
  {
    wxPen savePen = m_pen;
    wxBrush saveBrush = m_brush;
    SetPen(*wxTRANSPARENT_PEN);
    SetBrush(wxBrush(m_textBackgroundColour, wxSOLID));
    DoDrawRectangle(xx, yy, ww, hh);        
    SetBrush(wxBrush(m_textForegroundColour, wxSOLID));
    m_pdfDocument->Image(imgName, image, xx, yy, ww, hh, wxPdfLink(-1), idMask);
    SetBrush(saveBrush);
    SetPen(savePen);
  }
  else
  {
    m_pdfDocument->Image(imgName, image, xx, yy, ww, hh, wxPdfLink(-1), idMask);
  }
}

void
wxPdfDCImpl::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
  DoDrawRotatedText(text, x, y, 0.0);
}

void
wxPdfDCImpl::DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));

  wxFont* fontToUse = &m_font;
  if (!fontToUse->IsOk())
  {
    return;
  }
  wxFont old = m_font; 

  wxPdfFontDescription desc = m_pdfDocument->GetFontDescription();
  int height, descent;
  CalculateFontMetrics(&desc, fontToUse->GetPointSize(), &height, NULL, &descent, NULL);
  if (m_mappingModeStyle != wxPDF_MAPMODESTYLE_PDF)
  {
    y += (height - abs(descent));
  }

  m_pdfDocument->SetTextColour(m_textForegroundColour.Red(), m_textForegroundColour.Green(), m_textForegroundColour.Blue());
  m_pdfDocument->SetFontSize(ScaleFontSizeToPdf(fontToUse->GetPointSize()));
  m_pdfDocument->RotatedText(ScaleLogicalToPdfX(x), ScaleLogicalToPdfY(y), text, angle);
  SetFont(old);
}

bool
wxPdfDCImpl::DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                    wxDC* source, wxCoord xsrc, wxCoord ysrc,
                    wxRasterOperationMode rop /*= wxCOPY*/, bool useMask /*= false*/, 
                    wxCoord xsrcMask /*= -1*/, wxCoord ysrcMask /*= -1*/)
{
//  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  wxCHECK_MSG( IsOk(), false, wxT("wxPdfDCImpl::DoBlit - invalid DC") );
  wxCHECK_MSG( source->IsOk(), false, wxT("wxPdfDCImpl::DoBlit - invalid source DC") );

  wxUnusedVar(useMask);
  wxUnusedVar(xsrcMask);
  wxUnusedVar(ysrcMask);

  // blit into a bitmap
  wxBitmap bitmap((int)width, (int)height);
  wxMemoryDC memDC;
  memDC.SelectObject(bitmap);
  memDC.Blit(0, 0, width, height, source, xsrc, ysrc, rop);
  memDC.SelectObject(wxNullBitmap);

  // draw bitmap. scaling and positioning is done there
  DoDrawBitmap( bitmap, xdest, ydest );

  return true;
}

void
wxPdfDCImpl::DoGetSize(int* width, int* height) const
{
  int w;
  int h;
  if (m_templateMode)
  {
    w = wxRound(m_templateWidth * m_pdfDocument->GetScaleFactor());
    h = wxRound(m_templateHeight * m_pdfDocument->GetScaleFactor());
  }
  else
  {
    wxPaperSize id = m_printData.GetPaperId();
    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(id);
    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(wxPAPER_A4);

    w = 595;
    h = 842;
    if (paper)
    {
      w = paper->GetSizeDeviceUnits().x;
      h = paper->GetSizeDeviceUnits().y;
    }

    if (m_printData.GetOrientation() == wxLANDSCAPE)
    {
      int tmp = w;
      w = h;
      h = tmp;
    }
  }

  if (width)
  {
    *width = wxRound( w * m_ppi / 72.0 );
  }

  if (height)
  {
    *height = wxRound( h * m_ppi / 72.0 );
  }
}

void
wxPdfDCImpl::DoGetSizeMM(int* width, int* height) const
{
  int w;
  int h;
  if (m_templateMode)
  {
    w = wxRound(m_templateWidth * m_pdfDocument->GetScaleFactor() * 25.4/72.0);
    h = wxRound(m_templateHeight * m_pdfDocument->GetScaleFactor() * 25.4/72.0);
  }
  else
  {
    wxPaperSize id = m_printData.GetPaperId();
    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(id);
    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(wxPAPER_A4);

    w = 210;
    h = 297;
    if (paper)
    {
      w = paper->GetWidth() / 10;
      h = paper->GetHeight() / 10;
    }

    if (m_printData.GetOrientation() == wxLANDSCAPE)
    {
      int tmp = w;
      w = h;
      h = tmp;
    }
  }
  if (width)
  {
    *width = w;
  }
  if (height)
  {
    *height = h;
  }
}

void
wxPdfDCImpl::DoDrawLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupPen();
  int i;
  for (i = 0; i < n; ++i)
  {
    wxPoint& point = points[i];
    double xx = ScaleLogicalToPdfX(xoffset + point.x);
    double yy = ScaleLogicalToPdfY(yoffset + point.y);
    CalcBoundingBox(point.x+xoffset, point.y+yoffset);
    if (i == 0)
    {
      m_pdfDocument->MoveTo(xx, yy);
    }
    else
    {
      m_pdfDocument->LineTo(xx, yy);
    }
  }
  m_pdfDocument->EndPath(wxPDF_STYLE_DRAW);
}

void
wxPdfDCImpl::DoDrawPolygon(int n, wxPoint points[],
                           wxCoord xoffset, wxCoord yoffset,
                           wxPolygonFillMode fillStyle /* = wxODDEVEN_RULE*/)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupBrush();
  SetupPen();
  wxPdfArrayDouble xp;
  wxPdfArrayDouble yp;
  int i;
  for (i = 0; i < n; ++i)
  {
    wxPoint& point = points[i];
    xp.Add(ScaleLogicalToPdfX(xoffset + point.x));
    yp.Add(ScaleLogicalToPdfY(yoffset + point.y));
    CalcBoundingBox(point.x + xoffset, point.y + yoffset);
  }
  int saveFillingRule = m_pdfDocument->GetFillingRule();
  m_pdfDocument->SetFillingRule(fillStyle);
  int style = GetDrawingStyle();
  m_pdfDocument->Polygon(xp, yp, style);
  m_pdfDocument->SetFillingRule(saveFillingRule);
}

void
wxPdfDCImpl::DoDrawPolyPolygon(int n, int count[], wxPoint points[],
                               wxCoord xoffset, wxCoord yoffset,
                               int fillStyle)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (n > 0)
  {
    SetupBrush();
    SetupPen();
    int style = GetDrawingStyle();
    int saveFillingRule = m_pdfDocument->GetFillingRule();
    m_pdfDocument->SetFillingRule(fillStyle);

    int ofs = 0;
    int i, j;
    for (j = 0; j < n; ofs += count[j++])
    {
      wxPdfArrayDouble xp;
      wxPdfArrayDouble yp;
      for (i = 0; i < count[j]; ++i)
      {
        wxPoint& point = points[ofs+i];
        xp.Add(ScaleLogicalToPdfX(xoffset + point.x));
        yp.Add(ScaleLogicalToPdfY(yoffset + point.y));
        CalcBoundingBox(point.x + xoffset, point.y + yoffset);
      }
      m_pdfDocument->Polygon(xp, yp, style);
    }
    m_pdfDocument->SetFillingRule(saveFillingRule);
  }
}

void
wxPdfDCImpl::DoSetClippingRegionAsRegion(const wxRegion& region)
{
  wxCoord x, y, w, h;
  region.GetBox(x, y, w, h);
  DoSetClippingRegion(x, y, w, h);
}

void
wxPdfDCImpl::DoSetClippingRegion(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (m_clipping)
  {
    DestroyClippingRegion();
  }

  m_clipX1 = (int) x;
  m_clipY1 = (int) y;
  m_clipX2 = (int) (x + width);
  m_clipY2 = (int) (y + height);

  // Use the current path as a clipping region
  m_pdfDocument->ClippingRect(ScaleLogicalToPdfX(x), 
                              ScaleLogicalToPdfY(y), 
                              ScaleLogicalToPdfXRel(width), 
                              ScaleLogicalToPdfYRel(height));
  m_clipping = true;
}

void
wxPdfDCImpl::DoSetDeviceClippingRegion(const wxRegion& region)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  wxCoord x, y, w, h;
  region.GetBox(x, y, w, h);
  DoSetClippingRegion(DeviceToLogicalX(x), DeviceToLogicalY(y), DeviceToLogicalXRel(w), DeviceToLogicalYRel(h));
}

void
wxPdfDCImpl::DoGetTextExtent(const wxString& text,
                         wxCoord* x, wxCoord* y,
                         wxCoord* descent,
                         wxCoord* externalLeading,
                         const wxFont* theFont) const
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));

  const wxFont* fontToUse = theFont;
  if (!fontToUse)
  {
    fontToUse = const_cast<wxFont*>(&m_font);
  }

  if (fontToUse)
  {
    wxFont old = m_font;
    
    const_cast<wxPdfDCImpl*>(this)->SetFont(*fontToUse);
    wxPdfFontDescription desc = const_cast<wxPdfDCImpl*>(this)->m_pdfDocument->GetFontDescription();
    int myAscent, myDescent, myHeight, myExtLeading;
    CalculateFontMetrics(&desc, fontToUse->GetPointSize(), &myHeight, &myAscent, &myDescent, &myExtLeading);

    if (descent)
    {
      *descent = abs(myDescent);
    }
    if( externalLeading )
    {
      *externalLeading = myExtLeading;
    }
    *x = ScalePdfToFontMetric((double)const_cast<wxPdfDCImpl*>(this)->m_pdfDocument->GetStringWidth(text));
    *y = myHeight;
    const_cast<wxPdfDCImpl*>(this)->SetFont(old);
  }
  else
  {
    *x = *y = 0;
    if (descent)
    {
      *descent = 0;
    }
    if (externalLeading)
    {
      *externalLeading = 0;
    }
  }
}

bool
wxPdfDCImpl::DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const
{
  wxCHECK_MSG( m_pdfDocument, false, wxT("wxPdfDCImpl::DoGetPartialTextExtents - invalid DC") );
    
  /// very slow - but correct ??
    
  const size_t len = text.length();
  if (len == 0)
  {
    return true;
  }
    
  widths.Empty();
  widths.Add(0, len);
  int w, h;
    
  wxString buffer;
  buffer.Alloc(len);

  for ( size_t i = 0; i < len; ++i )
  {
    buffer.Append(text.Mid( i, 1));
    DoGetTextExtent(buffer, &w, &h);
    widths[i] = w;
  }

  buffer.Clear();
  return true;
}

void
wxPdfDCImpl::SetupPen()
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  // pen
  const wxPen& curPen = GetPen();
  if (curPen != wxNullPen)
  {
    wxPdfLineStyle style = m_pdfDocument->GetLineStyle();
    wxPdfArrayDouble dash;
    style.SetColour(wxColour(curPen.GetColour().Red(),
                             curPen.GetColour().Green(),
                             curPen.GetColour().Blue()));
    if (curPen.GetWidth())
    {
      style.SetWidth(ScaleLogicalToPdfXRel(curPen.GetWidth()));
    }
    switch (curPen.GetStyle())
    {
      case wxDOT:
        dash.Add(1);
        dash.Add(1);
        style.SetDash(dash);
        break;
      case wxLONG_DASH:
        dash.Add(4);
        dash.Add(4);
        style.SetDash(dash);
        break;
      case wxSHORT_DASH:
        dash.Add(2);
        dash.Add(2);
        style.SetDash(dash);
        break;
      case wxDOT_DASH:
        {
          dash.Add(1);
          dash.Add(1);
          dash.Add(4);
          dash.Add(1);
          style.SetDash(dash);
        }
        break;
      case wxSOLID:
      default:
        style.SetDash(dash);
        break;
    }
    m_pdfDocument->SetLineStyle(style);
  }
  else
  {
    m_pdfDocument->SetDrawColour(0, 0, 0);
    m_pdfDocument->SetLineWidth(ScaleLogicalToPdfXRel(1.0));
  }
}

void
wxPdfDCImpl::SetupBrush()
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  // brush
  const wxBrush& curBrush = GetBrush();
  if (curBrush != wxNullBrush)
  {
    m_pdfDocument->SetFillColour(curBrush.GetColour().Red(), curBrush.GetColour().Green(), curBrush.GetColour().Blue());
  }
  else
  {
    m_pdfDocument->SetFillColour(0, 0, 0);
  }
}

// Get the current drawing style based on the current brush and pen
int
wxPdfDCImpl::GetDrawingStyle()
{
  int style = wxPDF_STYLE_NOOP;
  const wxBrush &curBrush = GetBrush();
  bool do_brush = (curBrush != wxNullBrush) && curBrush.GetStyle() != wxTRANSPARENT;
  const wxPen &curPen = GetPen();
  bool do_pen = (curPen != wxNullPen) && curPen.GetWidth() && curPen.GetStyle() != wxTRANSPARENT;
  if (do_brush && do_pen)
  {
    style = wxPDF_STYLE_FILLDRAW;
  }
  else if (do_pen)
  {
    style = wxPDF_STYLE_DRAW;
  }
  else if (do_brush)
  {
    style = wxPDF_STYLE_FILL;  
  }
  return style;
}

double
wxPdfDCImpl::ScaleLogicalToPdfX(wxCoord x) const
{
  double docScale = 72.0 / (m_ppi * m_pdfDocument->GetScaleFactor());
  return docScale * (((double)((x - m_logicalOriginX) * m_signX) * m_scaleX) +
         m_deviceOriginX + m_deviceLocalOriginX);
}

double
wxPdfDCImpl::ScaleLogicalToPdfXRel(wxCoord x) const
{
  double docScale = 72.0 / (m_ppi * m_pdfDocument->GetScaleFactor());
  return (double)(x) * m_scaleX * docScale;
}

double
wxPdfDCImpl::ScaleLogicalToPdfY(wxCoord y) const
{
  double docScale = 72.0 / (m_ppi * m_pdfDocument->GetScaleFactor());
  return docScale * (((double)((y - m_logicalOriginY) * m_signY) * m_scaleY) + 
         m_deviceOriginY + m_deviceLocalOriginY);
}
 
double
wxPdfDCImpl::ScaleLogicalToPdfYRel(wxCoord y) const
{
  double docScale = 72.0 / (m_ppi * m_pdfDocument->GetScaleFactor());
  return (double)(y) * m_scaleY * docScale;
}

double
wxPdfDCImpl::ScaleFontSizeToPdf(int pointSize) const
{
  double fontScale;
  double rval;
  switch (m_mappingModeStyle)
  {
    case wxPDF_MAPMODESTYLE_MSW:
      // as implemented in wxMSW
      fontScale = (m_ppiPdfFont / m_ppi);
      rval = (double) pointSize * fontScale * m_scaleY;
      break;
    case wxPDF_MAPMODESTYLE_GTK:
      // as implemented in wxGTK / wxMAC / wxOSX
      fontScale = (m_ppiPdfFont / m_ppi);
      rval = (double) pointSize * fontScale * m_userScaleY;
      break;
    case wxPDF_MAPMODESTYLE_MAC:
      // as implemented in wxGTK / wxMAC / wxOSX
      fontScale = (m_ppiPdfFont / m_ppi);
      rval = (double) pointSize * fontScale * m_userScaleY;
      break;
    case wxPDF_MAPMODESTYLE_PDF:
      // an implementation where a font size of 12 gets a 12 point font if the
      // mapping mode is wxMM_POINTS and suitable scaled for other modes
      fontScale = (m_mappingMode == wxMM_TEXT) ? (m_ppiPdfFont / m_ppi) : (72.0 / m_ppi);
      rval = (double) pointSize * fontScale * m_scaleY;
      break;
    default:
      // standard and fall through
#if defined( __WXMSW__)
      fontScale = (m_ppiPdfFont / m_ppi);
      rval = (double) pointSize * fontScale * m_scaleY;
#else
      fontScale = (m_ppiPdfFont / m_ppi);
      rval = (double) pointSize * fontScale * m_userScaleY;
#endif
      break;
  }
  return rval;
}

int
wxPdfDCImpl::ScalePdfToFontMetric( double metric ) const
{
  double fontScale = (72.0 / m_ppi) / (double)m_pdfDocument->GetScaleFactor();
  return wxRound(((metric * m_signY) / m_scaleY ) / fontScale);
}

#endif // 0 (wxPdfDC implementations

#endif // wxUSE_GRAPHICS_CONTEXT
