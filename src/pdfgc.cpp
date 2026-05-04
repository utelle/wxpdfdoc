///////////////////////////////////////////////////////////////////////////////
// Name:        pdfgc.cpp
// Purpose:     Implementation of wxPdfGraphicsContext
// Author:      Ulrich Telle, Blake Madden
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

#include "wx/private_graphics.h"
#include "wx/rawbmp.h"
#include "wx/vector.h"

#include <wx/affinematrix2d.h>
#include <wx/region.h>
#include <wx/font.h>
#include <wx/paper.h>

#include "wx/pdfgc.h"
#include "wx/pdfdoc_version.h"
#include "wx/pdffontmanager.h"
#include "wx/pdflinestyle.h"
#include "wx/pdfshape.h"

#include "wx/pdfdc.h"

#include <math.h>

//-----------------------------------------------------------------------------
// Private graphics-data subclasses
//-----------------------------------------------------------------------------

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

  // adds a quadratic Bezier curve from the current point, using a control point and an end point
  virtual void AddQuadCurveToPoint(wxDouble cx, wxDouble cy, wxDouble x, wxDouble y);

  // appends a rectangle as a new closed subpath
  virtual void AddRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h);

  // appends an ellipsis as a new closed subpath fitting the passed rectangle
  virtual void AddCircle(wxDouble x, wxDouble y, wxDouble r);

  //
  // These are convenience functions which - if not available natively will be assembled
  // using the primitives from above
  //

  // draws a an arc to two tangents connecting (current) to (x1,y1) and (x1,y1) to (x2,y2), also a straight line from (current) to (x1,y1)
  virtual void AddArcToPoint(wxDouble x1, wxDouble y1 , wxDouble x2, wxDouble y2, wxDouble r);

  // appends an ellipse
  virtual void AddEllipse(wxDouble x, wxDouble y, wxDouble w, wxDouble h);

  // appends a rounded rectangle
  virtual void AddRoundedRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h, wxDouble radius);

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
  // Update m_currentX/Y and the bounding box with a single point.
  void RecordPoint(double x, double y);
  // Update the bounding box only (used for control points).
  void RecordExtent(double x, double y);

  wxPdfShape m_path;

  // Tracked current point and bounding box. wxPdfShape doesn't expose these
  // so we maintain them ourselves; matches wxCairoPathData / wxGDIPlusPath.
  bool   m_hasCurrent;
  double m_currentX;
  double m_currentY;
  // Last MoveTo, used to restore the current point after CloseSubpath.
  double m_subpathStartX;
  double m_subpathStartY;

  bool   m_hasBox;
  double m_minX;
  double m_minY;
  double m_maxX;
  double m_maxY;
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
  virtual ~wxPdfGraphicsPenBrushData() {};

  virtual void Apply(wxPdfGraphicsContext* context);

protected:
  // Call this to use the given bitmap as stipple. Bitmap must be non-null
  // and valid.
  void InitStipple(const wxBitmap& bmp);

  // Call this to use the given hatch style. Hatch style must be valid.
  void InitHatch(wxHatchStyle hatchStyle);

  wxColour m_colour;

  class wxPdfGraphicsBitmapData* m_bmpdata;

private:
  wxHatchStyle m_hatchStyle;

  wxDECLARE_NO_COPY_CLASS(wxPdfGraphicsPenBrushData);
};

class wxPdfGraphicsPenData : public wxPdfGraphicsPenBrushData
{
public:
  wxPdfGraphicsPenData(wxGraphicsRenderer* renderer, const wxGraphicsPenInfo& info);
  ~wxPdfGraphicsPenData();

  void Init();

  virtual void Apply(wxPdfGraphicsContext* context);
  virtual wxDouble GetWidth() { return m_width; }

private :
  double m_width;

  wxPdfLineCap   m_cap;
  wxPdfLineJoin  m_join;

  int           m_count;
  const double* m_lengths;
  double*       m_userLengths;

  wxDECLARE_NO_COPY_CLASS(wxPdfGraphicsPenData);
};

class wxPdfGraphicsBrushData : public wxPdfGraphicsPenBrushData
{
public:
  enum GradientKind { GRAD_NONE, GRAD_LINEAR, GRAD_RADIAL };

  wxPdfGraphicsBrushData(wxGraphicsRenderer* renderer);
  wxPdfGraphicsBrushData(wxGraphicsRenderer* renderer, const wxBrush& brush);

  virtual void Apply(wxPdfGraphicsContext* context) wxOVERRIDE;

  void CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                                 wxDouble x2, wxDouble y2,
                                 const wxGraphicsGradientStops& stops);
  void CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                 wxDouble xc, wxDouble yc, wxDouble radius,
                                 const wxGraphicsGradientStops& stops);

  // Public accessors used by FillPath() to render gradients.
  GradientKind GetGradientKind() const { return m_gradientKind; }
  bool HasGradient() const { return m_gradientKind != GRAD_NONE; }
  double GetGradX1() const { return m_gradX1; }
  double GetGradY1() const { return m_gradY1; }
  double GetGradX2() const { return m_gradX2; }
  double GetGradY2() const { return m_gradY2; }
  double GetGradRadius() const { return m_gradRadius; }
  const wxGraphicsGradientStops& GetStops() const { return m_gradStops; }
  const wxColour& GetColour() const { return m_colour; }

protected:
  virtual void Init();

  // common part of Create{Linear,Radial}GradientBrush()
  void AddGradientStops(const wxGraphicsGradientStops& stops);

  GradientKind             m_gradientKind;
  double                   m_gradX1, m_gradY1, m_gradX2, m_gradY2;
  double                   m_gradRadius;
  wxGraphicsGradientStops  m_gradStops;
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

  virtual void* GetNativeBitmap() const wxOVERRIDE { return NULL; }
  // wxGraphicsBitmapData has no virtual GetSize; this is just a helper
  // for our internal use.
  wxSize GetSize() { return wxSize(m_image.GetWidth(), m_image.GetHeight()); }

#if wxUSE_IMAGE
  wxImage ConvertToImage() const { return m_image; }
#endif // wxUSE_IMAGE

  // Direct access for the context's DrawBitmap() implementation.
  const wxImage& GetImage() const { return m_image; }

private:
  wxImage m_image;
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

void
wxPdfGraphicsPenBrushData::InitStipple(const wxBitmap& WXUNUSED(bmp))
{
  // TODO: route stipple through wxPdfPattern. Currently a no-op so that
  // pens / brushes with stipple styles still construct successfully.
}

void
wxPdfGraphicsPenBrushData::InitHatch(wxHatchStyle hatchStyle)
{
  // We can't create pattern right now as as it's not implemented,
  // so just remember that we need to do it.
  m_hatchStyle = hatchStyle;
}

void
wxPdfGraphicsPenBrushData::Apply(wxPdfGraphicsContext* WXUNUSED(context))
{
  // Nothing to do at this layer — wxPdfGraphicsPenData::Apply and
  // wxPdfGraphicsBrushData::Apply own colour/alpha state because the
  // PDF document distinguishes draw colour from fill colour.
  // Hatch / stipple patterns are TODO (would route through wxPdfPattern).
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

wxPdfGraphicsPenData::wxPdfGraphicsPenData(wxGraphicsRenderer* renderer,
                                           const wxGraphicsPenInfo& info)
  : wxPdfGraphicsPenBrushData(renderer, info.GetColour(),
                              info.GetStyle() == wxPENSTYLE_TRANSPARENT)
{
  Init();
  m_width = info.GetWidth();
  if (m_width <= 0.0)
  {
    m_width = 0.1;
  }

  switch (info.GetCap())
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

  switch (info.GetJoin())
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

  switch (info.GetStyle())
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
        wxDash* wxdashes;
        m_count = info.GetDashes(&wxdashes);
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
      InitStipple(info.GetStipple());
      break;

    default :
      if (info.GetStyle() >= wxPENSTYLE_FIRST_HATCH &&
          info.GetStyle() <= wxPENSTYLE_LAST_HATCH)
      {
        InitHatch(static_cast<wxHatchStyle>(info.GetStyle()));
      }
      break;
  }
}

void
wxPdfGraphicsPenData::Apply(wxPdfGraphicsContext* context)
{
  wxPdfGraphicsPenBrushData::Apply(context);

  wxPdfDocument* doc = context->GetPdfDocument();
  if (!doc)
  {
    return;
  }

  // Build a wxPdfLineStyle that captures everything in one go:
  // colour, width, cap, join, dash array.
  wxPdfArrayDouble dash;
  for (int i = 0; i < m_count; ++i)
  {
    dash.Add(m_lengths[i]);
  }

  wxPdfColour pdfCol(m_colour.Red(), m_colour.Green(), m_colour.Blue());
  wxPdfLineStyle style(m_width, m_cap, m_join, dash, 0.0, pdfCol);
  doc->SetLineStyle(style);
  doc->SetDrawColour(m_colour.Red(), m_colour.Green(), m_colour.Blue());

  context->SetLineAlpha(m_colour.Alpha() / 255.0);
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
      InitStipple(*brush.GetStipple());
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
  // Stops are stored verbatim; the actual PDF shading is materialised by
  // Apply() once a document is available.
  m_gradStops = stops;
}

void
wxPdfGraphicsBrushData::CreateLinearGradientBrush(wxDouble x1, wxDouble y1,
                                                  wxDouble x2, wxDouble y2,
                                                  const wxGraphicsGradientStops& stops)
{
  m_gradientKind = GRAD_LINEAR;
  m_gradX1 = x1; m_gradY1 = y1;
  m_gradX2 = x2; m_gradY2 = y2;
  AddGradientStops(stops);
}

void
wxPdfGraphicsBrushData::CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                                  wxDouble xc, wxDouble yc,
                                                  wxDouble radius,
                                                  const wxGraphicsGradientStops& stops)
{
  m_gradientKind = GRAD_RADIAL;
  m_gradX1 = xo; m_gradY1 = yo;
  m_gradX2 = xc; m_gradY2 = yc;
  m_gradRadius = radius;
  AddGradientStops(stops);
}

void
wxPdfGraphicsBrushData::Apply(wxPdfGraphicsContext* context)
{
  wxPdfGraphicsPenBrushData::Apply(context);

  wxPdfDocument* doc = context->GetPdfDocument();
  if (!doc)
  {
    return;
  }

  // Solid colour path.
  if (m_gradientKind == GRAD_NONE)
  {
    doc->SetFillColour(m_colour.Red(), m_colour.Green(), m_colour.Blue());
    context->SetFillAlpha(m_colour.Alpha() / 255.0);
  }
  else
  {
    // Gradient is configured but the registration logic lives elsewhere.
    // For now fall back to the first stop colour so brushes still produce
    // visible output instead of nothing.
    if (m_gradStops.GetCount() > 0)
    {
      const wxColour c = m_gradStops.GetStartColour();
      doc->SetFillColour(c.Red(), c.Green(), c.Blue());
      context->SetFillAlpha(c.Alpha() / 255.0);
    }
  }
}

void
wxPdfGraphicsBrushData::Init()
{
  m_bmpdata = NULL;
  m_gradientKind = GRAD_NONE;
  m_gradX1 = m_gradY1 = m_gradX2 = m_gradY2 = 0.0;
  m_gradRadius = 0.0;
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
  const bool ok = m_regFont.IsValid() && pdfDocument != NULL;
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
  , m_hasCurrent(false)
  , m_currentX(0.0)
  , m_currentY(0.0)
  , m_subpathStartX(0.0)
  , m_subpathStartY(0.0)
  , m_hasBox(false)
  , m_minX(0.0)
  , m_minY(0.0)
  , m_maxX(0.0)
  , m_maxY(0.0)
{
  if (path)
  {
    // Replay the source path through our own setters so we recover the
    // current point and bounding box that wxPdfShape doesn't track.
    const unsigned int n = path->GetSegmentCount();
    int iterPoints = 0;
    double coords[8];
    for (unsigned int i = 0; i < n; ++i)
    {
      wxPdfSegmentType seg = path->GetSegment(i, iterPoints, coords);
      switch (seg)
      {
        case wxPDF_SEG_MOVETO:
          MoveToPoint(coords[0], coords[1]);
          iterPoints += 1;
          break;
        case wxPDF_SEG_LINETO:
          AddLineToPoint(coords[0], coords[1]);
          iterPoints += 1;
          break;
        case wxPDF_SEG_CURVETO:
          AddCurveToPoint(coords[0], coords[1],
                          coords[2], coords[3],
                          coords[4], coords[5]);
          iterPoints += 3;
          break;
        case wxPDF_SEG_CLOSE:
          CloseSubpath();
          iterPoints += 1;
          break;
        default:
          break;
      }
    }
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
wxPdfGraphicsPathData::UnGetNativePath(void* WXUNUSED(p)) const
{
}

void
wxPdfGraphicsPathData::RecordPoint(double x, double y)
{
  m_currentX = x;
  m_currentY = y;
  m_hasCurrent = true;
  RecordExtent(x, y);
}

void
wxPdfGraphicsPathData::RecordExtent(double x, double y)
{
  if (!m_hasBox)
  {
    m_minX = m_maxX = x;
    m_minY = m_maxY = y;
    m_hasBox = true;
  }
  else
  {
    if (x < m_minX) m_minX = x;
    if (x > m_maxX) m_maxX = x;
    if (y < m_minY) m_minY = y;
    if (y > m_maxY) m_maxY = y;
  }
}

//
// The Primitives
//

void
wxPdfGraphicsPathData::MoveToPoint(wxDouble x, wxDouble y)
{
  m_path.MoveTo(x, y);
  m_subpathStartX = x;
  m_subpathStartY = y;
  RecordPoint(x, y);
}

void
wxPdfGraphicsPathData::AddLineToPoint(wxDouble x, wxDouble y)
{
  m_path.LineTo(x, y);
  RecordPoint(x, y);
}

void
wxPdfGraphicsPathData::AddPath(const wxGraphicsPathData* path)
{
  // Walk the source segments and replay them on top of the current path.
  // wxPdfShape exposes GetSegment but not a direct AddPath, so this
  // synthesises one without modifying wxPdfShape.
  const wxPdfShape* src =
    static_cast<const wxPdfShape*>(path->GetNativePath());
  if (!src)
  {
    return;
  }

  const unsigned int n = src->GetSegmentCount();
  int iterPoints = 0;
  double coords[8];
  for (unsigned int i = 0; i < n; ++i)
  {
    wxPdfSegmentType seg = src->GetSegment(i, iterPoints, coords);
    switch (seg)
    {
      case wxPDF_SEG_MOVETO:
        MoveToPoint(coords[0], coords[1]);
        iterPoints += 1;
        break;
      case wxPDF_SEG_LINETO:
        AddLineToPoint(coords[0], coords[1]);
        iterPoints += 1;
        break;
      case wxPDF_SEG_CURVETO:
        AddCurveToPoint(coords[0], coords[1],
                        coords[2], coords[3],
                        coords[4], coords[5]);
        iterPoints += 3;
        break;
      case wxPDF_SEG_CLOSE:
        CloseSubpath();
        iterPoints += 1;
        break;
      default:
        break;
    }
  }

  path->UnGetNativePath(const_cast<wxPdfShape*>(src));
}

void
wxPdfGraphicsPathData::CloseSubpath()
{
  m_path.ClosePath();
  // PDF closepath returns the current point to the start of the subpath.
  m_currentX = m_subpathStartX;
  m_currentY = m_subpathStartY;
}

void
wxPdfGraphicsPathData::AddCurveToPoint(wxDouble cx1, wxDouble cy1, wxDouble cx2, wxDouble cy2, wxDouble x, wxDouble y)
{
  m_path.CurveTo(cx1, cy1, cx2, cy2, x, y);
  RecordExtent(cx1, cy1);
  RecordExtent(cx2, cy2);
  RecordPoint(x, y);
}

void
wxPdfGraphicsPathData::GetCurrentPoint(wxDouble* x, wxDouble* y) const
{
  if (x) *x = m_hasCurrent ? m_currentX : 0.0;
  if (y) *y = m_hasCurrent ? m_currentY : 0.0;
}

void
wxPdfGraphicsPathData::AddQuadCurveToPoint(wxDouble cx, wxDouble cy, wxDouble x, wxDouble y)
{
  // Quadratic Bezier (P0, P1, P2) converted to Cubic (P0, C1, C2, P3):
  // C1 = P0 + 2/3 * (P1 - P0)
  // C2 = P2 + 2/3 * (P1 - P2)
  double x0, y0;
  GetCurrentPoint(&x0, &y0);
  double cx1 = x0 + 2.0 / 3.0 * (cx - x0);
  double cy1 = y0 + 2.0 / 3.0 * (cy - y0);
  double cx2 = x + 2.0 / 3.0 * (cx - x);
  double cy2 = y + 2.0 / 3.0 * (cy - y);
  AddCurveToPoint(cx1, cy1, cx2, cy2, x, y);
}

void
wxPdfGraphicsPathData::AddRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  MoveToPoint(x, y);
  AddLineToPoint(x + w, y);
  AddLineToPoint(x + w, y + h);
  AddLineToPoint(x, y + h);
  CloseSubpath();
}

void
wxPdfGraphicsPathData::AddEllipse(wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  double rx = w / 2.0;
  double ry = h / 2.0;
  double cx = x + rx;
  double cy = y + ry;

  // Standard kappa for circle is 4/3 * (sqrt(2)-1) = approx 0.55228475
  const double kappa = 0.55228474983079339840;

  double ox = rx * kappa; // control point offset horizontal
  double oy = ry * kappa; // control point offset vertical

  MoveToPoint(cx + rx, cy);
  AddCurveToPoint(cx + rx, cy + oy, cx + ox, cy + ry, cx, cy + ry);
  AddCurveToPoint(cx - ox, cy + ry, cx - rx, cy + oy, cx - rx, cy);
  AddCurveToPoint(cx - rx, cy - oy, cx - ox, cy - ry, cx, cy - ry);
  AddCurveToPoint(cx + ox, cy - ry, cx + rx, cy - oy, cx + rx, cy);
  CloseSubpath();
}

void
wxPdfGraphicsPathData::AddRoundedRectangle(wxDouble x, wxDouble y, wxDouble w, wxDouble h, wxDouble radius)
{
  if (radius <= 0)
  {
    AddRectangle(x, y, w, h);
    return;
  }

  if (radius > w / 2) radius = w / 2;
  if (radius > h / 2) radius = h / 2;

  const double kappa = 0.55228474983079339840;
  double offset = radius * kappa;

  MoveToPoint(x + radius, y);
  AddLineToPoint(x + w - radius, y);
  AddCurveToPoint(x + w - radius + offset, y, x + w, y + radius - offset, x + w, y + radius);
  AddLineToPoint(x + w, y + h - radius);
  AddCurveToPoint(x + w, y + h - radius + offset, x + w - radius + offset, y + h, x + w - radius, y + h);
  AddLineToPoint(x + radius, y + h);
  AddCurveToPoint(x + radius - offset, y + h, x, y + h - radius + offset, x, y + h - radius);
  AddLineToPoint(x, y + radius);
  AddCurveToPoint(x, y + radius - offset, x + radius - offset, y, x + radius, y);
  CloseSubpath();
}

void
wxPdfGraphicsPathData::AddArcToPoint(wxDouble x1, wxDouble y1, wxDouble x2, wxDouble y2, wxDouble r)
{
  if (!m_hasCurrent)
  {
    MoveToPoint(x1, y1);
    return;
  }

  const double x0 = m_currentX;
  const double y0 = m_currentY;

  if (r <= 0)
  {
    AddLineToPoint(x1, y1);
    return;
  }

  double dx0 = x0 - x1;
  double dy0 = y0 - y1;
  double dx2 = x2 - x1;
  double dy2 = y2 - y1;

  double len0 = sqrt(dx0 * dx0 + dy0 * dy0);
  double len2 = sqrt(dx2 * dx2 + dy2 * dy2);

  if (len0 < 1e-10 || len2 < 1e-10)
  {
    AddLineToPoint(x1, y1);
    return;
  }

  dx0 /= len0; dy0 /= len0;
  dx2 /= len2; dy2 /= len2;

  double cosAlpha = dx0 * dx2 + dy0 * dy2;

  if (cosAlpha > 0.999999 || cosAlpha < -0.999999)
  {
    AddLineToPoint(x1, y1);
    return;
  }

  double alpha = acos(cosAlpha);
  double d = r / tan(alpha / 2.0);

  double bx = dx0 + dx2;
  double by = dy0 + dy2;
  double blen = sqrt(bx * bx + by * by);
  bx /= blen; by /= blen;

  double h = r / sin(alpha / 2.0);
  double cx = x1 + h * bx;
  double cy = y1 + h * by;

  double tx1 = x1 + d * dx0;
  double ty1 = y1 + d * dy0;
  double tx2 = x1 + d * dx2;
  double ty2 = y1 + d * dy2;

  double startAngle = atan2(ty1 - cy, tx1 - cx);
  double endAngle = atan2(ty2 - cy, tx2 - cx);

  bool clockwise = (dx0 * dy2 - dy0 * dx2) < 0;

  AddArc(cx, cy, r, startAngle, endAngle, clockwise);
}

void
wxPdfGraphicsPathData::AddCircle(wxDouble x, wxDouble y, wxDouble r)
{
  AddEllipse(x - r, y - r, 2 * r, 2 * r);
}

void
wxPdfGraphicsPathData::AddArc(wxDouble x, wxDouble y, wxDouble r, double startAngle, double endAngle, bool clockwise)
{
  double sweep = endAngle - startAngle;
  if (clockwise)
  {
    if (sweep <= 0) sweep += 2.0 * M_PI;
  }
  else
  {
    if (sweep >= 0) sweep -= 2.0 * M_PI;
  }

  // If there is already a current point, add a line to the start of the arc.
  // If not, move to the start of the arc.
  const double startX = x + r * cos(startAngle);
  const double startY = y + r * sin(startAngle);
  if (!m_hasCurrent)
  {
    MoveToPoint(startX, startY);
  }
  else
  {
    AddLineToPoint(startX, startY);
  }

  // Split into 90-degree segments
  int n = ceil(abs(sweep) / (M_PI / 2.0));
  if (n < 1) n = 1;
  double delta = sweep / n;
  double currentAngle = startAngle;

  for (int i = 0; i < n; ++i)
  {
    double nextAngle = currentAngle + delta;
    double alpha = sin(delta) * (sqrt(4.0 + 3.0 * pow(tan(delta / 2.0), 2)) - 1.0) / 3.0;

    // Control points for Y-down coordinates
    double cx1 = x + r * (cos(currentAngle) - alpha * sin(currentAngle));
    double cy1 = y + r * (sin(currentAngle) + alpha * cos(currentAngle));
    double cx2 = x + r * (cos(nextAngle) + alpha * sin(nextAngle));
    double cy2 = y + r * (sin(nextAngle) - alpha * cos(nextAngle));
    double ex = x + r * cos(nextAngle);
    double ey = y + r * sin(nextAngle);

    AddCurveToPoint(cx1, cy1, cx2, cy2, ex, ey);
    currentAngle = nextAngle;
  }
}

// transforms each point of this path by the matrix
void
wxPdfGraphicsPathData::Transform(const wxGraphicsMatrixData* matrix)
{
  // wxPdfShape has no in-place transform; rebuild it segment-by-segment.
  // The matrix is applied forward (the previous code inverted it, which
  // was a bug).
  const wxAffineMatrix2D& m =
    *static_cast<const wxAffineMatrix2D*>(matrix->GetNativeMatrix());

  wxPdfShape oldPath = m_path;
  wxPdfShape rebuilt;
  m_path = rebuilt;
  m_hasCurrent = false;
  m_hasBox = false;

  const unsigned int n = oldPath.GetSegmentCount();
  int iterPoints = 0;
  double coords[8];
  for (unsigned int i = 0; i < n; ++i)
  {
    wxPdfSegmentType seg = oldPath.GetSegment(i, iterPoints, coords);
    switch (seg)
    {
      case wxPDF_SEG_MOVETO:
      {
        wxPoint2DDouble p = m.TransformPoint(wxPoint2DDouble(coords[0], coords[1]));
        MoveToPoint(p.m_x, p.m_y);
        iterPoints += 1;
        break;
      }
      case wxPDF_SEG_LINETO:
      {
        wxPoint2DDouble p = m.TransformPoint(wxPoint2DDouble(coords[0], coords[1]));
        AddLineToPoint(p.m_x, p.m_y);
        iterPoints += 1;
        break;
      }
      case wxPDF_SEG_CURVETO:
      {
        wxPoint2DDouble c1 = m.TransformPoint(wxPoint2DDouble(coords[0], coords[1]));
        wxPoint2DDouble c2 = m.TransformPoint(wxPoint2DDouble(coords[2], coords[3]));
        wxPoint2DDouble e  = m.TransformPoint(wxPoint2DDouble(coords[4], coords[5]));
        AddCurveToPoint(c1.m_x, c1.m_y, c2.m_x, c2.m_y, e.m_x, e.m_y);
        iterPoints += 3;
        break;
      }
      case wxPDF_SEG_CLOSE:
        CloseSubpath();
        iterPoints += 1;
        break;
      default:
        break;
    }
  }
}

// gets the bounding box enclosing all points (possibly including control points)
void
wxPdfGraphicsPathData::GetBox(wxDouble* x, wxDouble* y, wxDouble* w, wxDouble* h) const
{
  if (!m_hasBox)
  {
    if (x) *x = 0;
    if (y) *y = 0;
    if (w) *w = 0;
    if (h) *h = 0;
    return;
  }
  if (x) *x = m_minX;
  if (y) *y = m_minY;
  if (w) *w = m_maxX - m_minX;
  if (h) *h = m_maxY - m_minY;
}

bool
wxPdfGraphicsPathData::Contains(wxDouble x, wxDouble y, wxPolygonFillMode fillStyle) const
{
  unsigned int segCount = m_path.GetSegmentCount();
  if (segCount == 0)
    return false;

  // Fast bounding box rejection
  wxDouble bx, by, bw, bh;
  GetBox(&bx, &by, &bw, &bh);
  if (x < bx || x > bx + bw || y < by || y > by + bh)
    return false;

  int windingNumber = 0;
  wxPoint2DDouble curPt(0, 0);
  wxPoint2DDouble startPt(0, 0);
  bool hasStartPt = false;
  bool hasCurPt = false;

  // Helper to process a single line segment for the winding number algorithm
  auto processEdge = [&](const wxPoint2DDouble& p1, const wxPoint2DDouble& p2)
  {
    if (p1.m_y <= y)
    {
      if (p2.m_y > y && (p2.m_x - p1.m_x) * (y - p1.m_y) - (x - p1.m_x) * (p2.m_y - p1.m_y) > 0)
        windingNumber++;
    }
    else
    {
      if (p2.m_y <= y && (p2.m_x - p1.m_x) * (y - p1.m_y) - (x - p1.m_x) * (p2.m_y - p1.m_y) < 0)
        windingNumber--;
    }
  };

  int iterType = 0;
  int iterPoints = 0;
  for (unsigned int i = 0; i < segCount; ++i)
  {
    double coords[8];
    wxPdfSegmentType type = m_path.GetSegment(iterType, iterPoints, coords);
    switch (type)
    {
      case wxPDF_SEG_MOVETO:
        if (hasCurPt && hasStartPt)
          processEdge(curPt, startPt);
        curPt = { coords[0], coords[1] };
        startPt = curPt;
        hasStartPt = true;
        hasCurPt = true;
        iterPoints += 2;
        break;

      case wxPDF_SEG_LINETO:
        if (hasCurPt)
        {
          processEdge(curPt, { coords[0], coords[1] });
          curPt = { coords[0], coords[1] };
        }
        iterPoints += 2;
        break;

      case wxPDF_SEG_CURVETO:
      {
        if (hasCurPt)
        {
          const wxPoint2DDouble p0 = curPt;
          const wxPoint2DDouble p1 = { coords[0], coords[1] };
          const wxPoint2DDouble p2 = { coords[2], coords[3] };
          const wxPoint2DDouble p3 = { coords[4], coords[5] };
          constexpr int steps = 10;
          for (int step = 1; step <= steps; ++step)
          {
            const double t = step / (double)steps;
            const double tInv = 1.0 - t;
            const wxPoint2DDouble next = {
              tInv * tInv * tInv * p0.m_x + 3 * tInv * tInv * t * p1.m_x + 3 * tInv * t * t * p2.m_x + t * t * t * p3.m_x,
              tInv * tInv * tInv * p0.m_y + 3 * tInv * tInv * t * p1.m_y + 3 * tInv * t * t * p2.m_y + t * t * t * p3.m_y
            };
            processEdge(curPt, next);
            curPt = next;
          }
        }
        iterPoints += 6;
        break;
      }

      case wxPDF_SEG_CLOSE:
        if (hasStartPt)
        {
          processEdge(curPt, startPt);
          curPt = startPt;
        }
        break;

      default:
        break;
    }
    iterType++;
  }

  if (fillStyle == wxODDEVEN_RULE)
    return (windingNumber % 2) != 0;
  return windingNumber != 0;
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

//-----------------------------------------------------------------------------
// wxPdfGraphicsBitmapData implementation
//-----------------------------------------------------------------------------

wxPdfGraphicsBitmapData::wxPdfGraphicsBitmapData(wxGraphicsRenderer* renderer,
                                                 const wxBitmap& bmp)
  : wxGraphicsBitmapData(renderer)
{
  // wxPdfDocument::Image takes a wxImage; convert once and store.
  if (bmp.IsOk())
  {
    m_image = bmp.ConvertToImage();
  }
}

#if wxUSE_IMAGE
wxPdfGraphicsBitmapData::wxPdfGraphicsBitmapData(wxGraphicsRenderer* renderer,
                                                 const wxImage& image)
  : wxGraphicsBitmapData(renderer)
  , m_image(image)
{
}
#endif // wxUSE_IMAGE

wxPdfGraphicsBitmapData::~wxPdfGraphicsBitmapData()
{
}

//-----------------------------------------------------------------------------
// wxPdfGraphicsContext implementation
//-----------------------------------------------------------------------------

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
  m_lineAlpha = 1.0;
  m_fillAlpha = 1.0;
  m_clipCount = 0;

  wxScreenDC screendc;
  m_ppiPdfFont = screendc.GetPPI().GetHeight();
  m_mappingModeStyle = wxPDF_MAPMODESTYLE_STANDARD;
 
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

void
wxPdfGraphicsContext::SetLineAlpha(double a)
{
  m_lineAlpha = a;
  if (m_pdfDocument)
  {
    m_pdfDocument->SetAlpha(m_lineAlpha, m_fillAlpha);
  }
}

void
wxPdfGraphicsContext::SetFillAlpha(double a)
{
  m_fillAlpha = a;
  if (m_pdfDocument)
  {
    m_pdfDocument->SetAlpha(m_lineAlpha, m_fillAlpha);
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
wxPdfGraphicsContext::ShouldOffset() const
{
  if (!m_enableOffset)
    return false;

  int penwidth = 0;
  if (!m_pen.IsNull())
  {
    penwidth = (int)((wxPdfGraphicsPenData*)m_pen.GetRefData())->GetWidth();
    if (penwidth == 0)
      penwidth = 1;
  }
  return (penwidth % 2) == 1;
}

bool
wxPdfGraphicsContext::StartDoc(const wxString& WXUNUSED(message))
{
  if (!m_templateMode && m_pdfDocument == NULL)
  {
    m_pdfDocument = new wxPdfDocument(m_printData.GetOrientation(), wxString(wxT("pt")), m_printData.GetPaperId());
    m_pdfDocument->Open();
    m_pdfDocument->SetAuthor(wxT("wxPdfGraphicsContext"));
    m_pdfDocument->SetTitle(wxT("wxPdfGraphicsContext"));

    wxGraphicsContext::SetBrush(*wxBLACK_BRUSH);
    wxGraphicsContext::SetPen(*wxBLACK_PEN);
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

void wxPdfGraphicsContext::PushState()
{
  if (!m_pdfDocument) return;
  m_ctmStack.push_back(m_ctm);
  m_clipCountStack.push_back(m_clipCount);
  m_clipCount = 0;
  // PDF "q" saves CTM, colour, line style, fill, and clip path together,
  // so callers don't need to re-set them manually after PopState.
  m_pdfDocument->StartTransform();
}

void wxPdfGraphicsContext::PopState()
{
  if (!m_pdfDocument) return;

  // Pop all clips added at this level.
  for (int i = 0; i < m_clipCount; ++i)
  {
    m_pdfDocument->StopTransform();
  }
  // Pop the PushState's q.
  m_pdfDocument->StopTransform();

  if (!m_ctmStack.empty())
  {
    m_ctm = m_ctmStack.back();
    m_ctmStack.pop_back();
  }
  else
  {
    m_ctm = wxAffineMatrix2D();
  }

  if (!m_clipCountStack.empty())
  {
    m_clipCount = m_clipCountStack.back();
    m_clipCountStack.pop_back();
  }
  else
  {
    m_clipCount = 0;
  }
}

void wxPdfGraphicsContext::Clip(const wxRegion& region)
{
  if (!m_pdfDocument) return;

  // Build a single wxPdfShape with one closed rectangular subpath per
  // region rectangle, then submit it as a clipping path. This preserves
  // non-rectangular regions exactly instead of
  // approximating with axis-aligned ClippingRect calls.
  wxPdfShape shape;
  bool any = false;
  wxRegionIterator ri(region);
  while (ri)
  {
    const double x = ri.GetX();
    const double y = ri.GetY();
    const double w = ri.GetW();
    const double h = ri.GetH();
    shape.MoveTo(x, y);
    shape.LineTo(x + w, y);
    shape.LineTo(x + w, y + h);
    shape.LineTo(x, y + h);
    shape.ClosePath();
    any = true;
    ++ri;
  }
  if (any)
  {
    m_pdfDocument->ClippingPath(shape);
    m_clipCount++;
  }
}

void wxPdfGraphicsContext::Clip(wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
  if (!m_pdfDocument) return;
  m_pdfDocument->ClippingRect(x, y, w, h);
  m_clipCount++;
}

void wxPdfGraphicsContext::Clip(const wxGraphicsPath& path)
{
  if (!m_pdfDocument || path.IsNull()) return;
  const wxPdfShape& shape = ((wxPdfGraphicsPathData*)path.GetRefData())->GetPdfShape();
  m_pdfDocument->ClippingPath(shape);
  m_clipCount++;
}

void wxPdfGraphicsContext::Clip(const wxPdfShape& shape)
{
  if (!m_pdfDocument) return;
  m_pdfDocument->ClippingPath(shape);
  m_clipCount++;
}

void wxPdfGraphicsContext::ResetClip()
{
  if (!m_pdfDocument) return;
  m_pdfDocument->UnsetClipping();
  // UnsetClipping typically clears the entire stack of q/Q clips
  // in wxPdfDocument, so we reset our count.
  m_clipCount = 0;
}

void wxPdfGraphicsContext::GetClipBox(wxDouble* x, wxDouble* y,
                                      wxDouble* w, wxDouble* h)
{
  // We don't track an explicit clip rect; PDF maintains it internally
  // and doesn't surface it. Report the whole page as the clipping
  // bounds, which is a safe upper bound for callers that use this to
  // size scratch buffers or similar.
  if (x) *x = 0;
  if (y) *y = 0;
  if (w) *w = m_pdfDocument ? m_pdfDocument->GetPageWidth()  : 0;
  if (h) *h = m_pdfDocument ? m_pdfDocument->GetPageHeight() : 0;
}

void*
wxPdfGraphicsContext::GetNativeContext()
{
  return NULL;
}

bool
wxPdfGraphicsContext::SetAntialiasMode(wxAntialiasMode antialias)
{
  // PDF readers, not the producer, control AA. We accept the request
  // (recording it on the context state) but produce identical output
  // either way.
  if (m_antialias == antialias) return true;
  if (antialias != wxANTIALIAS_DEFAULT && antialias != wxANTIALIAS_NONE)
  {
    return false;
  }
  m_antialias = antialias;
  return true;
}

bool wxPdfGraphicsContext::SetInterpolationQuality(wxInterpolationQuality WXUNUSED(interpolation))
{
  // PDF has no analog. Reject so callers can fall back if they care.
  return false;
}

bool wxPdfGraphicsContext::SetCompositionMode(wxCompositionMode op)
{
  // PDF lacks Porter-Duff compositing; only the OVER / SOURCE flavours map
  // cleanly to its normal blend mode. Everything else is rejected so
  // callers can detect non-support.
  if (m_composition == op) return true;

  switch (op)
  {
    case wxCOMPOSITION_OVER:
    case wxCOMPOSITION_SOURCE:
      m_composition = op;
      if (m_pdfDocument)
      {
        m_pdfDocument->SetAlpha(m_lineAlpha, m_fillAlpha,
                                wxPDF_BLENDMODE_NORMAL);
      }
      return true;

    default:
      return false;
  }
}

void
wxPdfGraphicsContext::GetDPI(wxDouble* dpiX, wxDouble* dpiY) const
{
  // PDF user space is fixed at 72 dpi unless rescaled.
  *dpiX = 72.0;
  *dpiY = 72.0;
}

void wxPdfGraphicsContext::BeginLayer(wxDouble opacity)
{
  // PDF doesn't have a perfect equivalent of cairo's transparency groups
  // without emitting Form XObjects + soft masks. The pragmatic emulation
  // (matching what GDI+ does) is to multiply the current line / fill
  // alphas by the layer opacity for the duration of the layer; everything
  // drawn inside it picks up the lowered opacity.
  AlphaPair saved = { m_lineAlpha, m_fillAlpha };
  m_layerAlphaStack.push_back(saved);
  SetLineAlpha(m_lineAlpha * opacity);
  SetFillAlpha(m_fillAlpha * opacity);
}

void wxPdfGraphicsContext::EndLayer()
{
  if (m_layerAlphaStack.empty()) return;
  AlphaPair saved = m_layerAlphaStack.back();
  m_layerAlphaStack.pop_back();
  SetLineAlpha(saved.line);
  SetFillAlpha(saved.fill);
}

void wxPdfGraphicsContext::Translate(wxDouble dx, wxDouble dy)
{
  if (!m_pdfDocument) return;
  m_pdfDocument->Transform(1, 0, 0, 1, dx, dy);
  m_ctm.Translate(dx, dy);
}

void wxPdfGraphicsContext::Scale(wxDouble xScale, wxDouble yScale)
{
  if (!m_pdfDocument) return;
  m_pdfDocument->Transform(xScale, 0, 0, yScale, 0, 0);
  m_ctm.Scale(xScale, yScale);
}

void wxPdfGraphicsContext::Rotate(wxDouble angle)
{
  if (!m_pdfDocument) return;
  const double c = cos(angle);
  const double s = sin(angle);
  m_pdfDocument->Transform(c, s, -s, c, 0, 0);
  m_ctm.Rotate(angle);
}

void wxPdfGraphicsContext::ConcatTransform(const wxGraphicsMatrix& matrix)
{
  if (!m_pdfDocument) return;
  const wxAffineMatrix2D& m =
    *static_cast<const wxAffineMatrix2D*>(matrix.GetNativeMatrix());

  wxMatrix2D mat2D;
  wxPoint2DDouble tr;
  m.Get(&mat2D, &tr);
  m_pdfDocument->Transform(mat2D.m_11, mat2D.m_12,
                           mat2D.m_21, mat2D.m_22,
                           tr.m_x, tr.m_y);
  m_ctm.Concat(m);
}

void wxPdfGraphicsContext::SetTransform(const wxGraphicsMatrix& matrix)
{
  if (!m_pdfDocument) return;

  const wxAffineMatrix2D& m =
    *static_cast<const wxAffineMatrix2D*>(matrix.GetNativeMatrix());

  // Instead of Q q (which pops clipping and other state), 
  // apply the difference between current CTM and the new one.
  wxAffineMatrix2D diff = m_ctm;
  diff.Invert();
  diff.Concat(m); // diff = inv(C) * M

  wxMatrix2D mat2D;
  wxPoint2DDouble tr;
  diff.Get(&mat2D, &tr);
  m_pdfDocument->Transform(mat2D.m_11, mat2D.m_12,
                           mat2D.m_21, mat2D.m_22,
                           tr.m_x, tr.m_y);
  m_ctm = m;
}

wxGraphicsMatrix
wxPdfGraphicsContext::GetTransform() const
{
  wxGraphicsMatrix matrix = CreateMatrix();
  wxMatrix2D mat2D;
  wxPoint2DDouble tr;
  m_ctm.Get(&mat2D, &tr);
  matrix.Set(mat2D.m_11, mat2D.m_12,
             mat2D.m_21, mat2D.m_22,
             tr.m_x, tr.m_y);
  return matrix;
}

void
wxPdfGraphicsContext::SetPen(const wxGraphicsPen& pen)
{
  m_pen = pen;
  if (!m_pen.IsNull())
  {
    ((wxPdfGraphicsPenData*) m_pen.GetRefData())->Apply(this);
  }
}

void
wxPdfGraphicsContext::SetBrush(const wxGraphicsBrush& brush)
{
  m_brush = brush;
  if (!m_brush.IsNull())
  {
    ((wxPdfGraphicsBrushData*) m_brush.GetRefData())->Apply(this);
  }
}

void
wxPdfGraphicsContext::SetFont(const wxGraphicsFont& font)
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::SetFont - no valid PDF document"));
  m_font = font;
  if (!m_font.IsNull())
  {
    ((wxPdfGraphicsFontData*) m_font.GetRefData())->Apply(this);
  }
}

void
wxPdfGraphicsContext::StrokePath(const wxGraphicsPath& path)
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::StrokePath - no valid PDF document"));
  if (!m_pen.IsNull())
  {
    // Re-apply the pen so PushState/PopState can't leave us out of sync
    // with the document's draw state.
    ((wxPdfGraphicsPenData*) m_pen.GetRefData())->Apply(this);
    const wxPdfShape& shape = ((wxPdfGraphicsPathData*) path.GetRefData())->GetPdfShape();
    m_pdfDocument->Shape(shape, wxPDF_STYLE_DRAW);
  }
}

// Helper: fill a path with a multi-stop gradient brush.
//
// PDF's wxPdfDocument::AxialGradient/RadialGradient only accept two
// colours, and the engine doesn't yet expose a Type 3 stitching function.
// For axial gradients with N >= 2 stops we paint N-1 separately-clipped
// AxialGradients in series. The trick: rotate user space so the gradient
// axis runs along x in [0,1], clip to the actual path, then for each
// stop interval [t_i, t_{i+1}] call SetFillGradient with rect
// (t_i, -big, t_{i+1}-t_i, 2*big). SetFillGradient internally clips to
// that rect, so each slice paints only its own band; the seams line up
// because adjacent gradients share an endpoint colour.
//
// For radial gradients with > 2 stops we currently fall back to the
// first/last stop pair (PDF radial slicing requires annular clipping
// which the engine doesn't expose).
static void
DoFillPathGradient(wxPdfGraphicsContext* context,
                   wxPdfGraphicsBrushData* brush,
                   const wxPdfShape& shape,
                   wxPolygonFillMode fillStyle)
{
  wxPdfDocument* doc = context->GetPdfDocument();
  if (!doc) return;

  const wxGraphicsGradientStops& stops = brush->GetStops();
  const size_t numStops = stops.GetCount();
  if (numStops < 2)
  {
    // Degenerate: treat as solid fill of the start colour.
    int saveRule = doc->GetFillingRule();
    doc->SetFillingRule(fillStyle);
    doc->Shape(shape, wxPDF_STYLE_FILL);
    doc->SetFillingRule(saveRule);
    return;
  }

  if (brush->GetGradientKind() == wxPdfGraphicsBrushData::GRAD_LINEAR)
  {
    const double gx1 = brush->GetGradX1();
    const double gy1 = brush->GetGradY1();
    const double gx2 = brush->GetGradX2();
    const double gy2 = brush->GetGradY2();
    const double dx = gx2 - gx1;
    const double dy = gy2 - gy1;
    const double len = sqrt(dx * dx + dy * dy);
    if (len <= 0.0)
    {
      // Degenerate axis: solid fill of the first stop.
      const wxColour c = stops.GetStartColour();
      doc->SetFillColour(c.Red(), c.Green(), c.Blue());
      int saveRule = doc->GetFillingRule();
      doc->SetFillingRule(fillStyle);
      doc->Shape(shape, wxPDF_STYLE_FILL);
      doc->SetFillingRule(saveRule);
      return;
    }

    context->PushState();

    // Clip to the actual path so the gradient slices are bounded by it.
    context->Clip(shape);

    // Rotate / scale user space so the gradient axis runs from (0,0) to
    // (1,0). We need: translate by (gx1,gy1), then rotate by theta,
    // then scale by len. Composed CTM in column-major:
    //   M = T(gx1,gy1) * R(theta) * S(len)
    // where theta = atan2(dy, dx). PDF's Transform takes a, b, c, d, tx, ty
    // such that the matrix is [[a c tx][b d ty]].
    // This is equivalent to mapping (1,0) to (dx,dy) and (0,1) to (-dy,dx).
    doc->Transform(dx, dy, -dy, dx, gx1, gy1);

    // Big perpendicular extent. 10000 user units is chosen as a safe, 
    // effectively infinite value compared to any realistic page dimensions.
    // Since we have already clipped the entire context to the actual path shape,
    // this large rectangle merely ensures the gradient slice covers the 
    // full height of the geometry without needing to calculate the specific
    // bounding box of the slice.
    const double bigY = 10000.0;

    for (size_t i = 0; i + 1 < numStops; ++i)
    {
      const wxGraphicsGradientStop s0 = stops.Item(i);
      const wxGraphicsGradientStop s1 = stops.Item(i + 1);
      const double t0 = s0.GetPosition();
      const double t1 = s1.GetPosition();
      if (t1 <= t0) continue;
      const wxColour c0 = s0.GetColour();
      const wxColour c1 = s1.GetColour();

      // Slice rect in transformed coords: x = [t0, t1], y = [-bigY, bigY].
      // SetFillGradient sets a clip rect and maps the gradient (defined in
      // 0..1 normalised coords) onto it. We register a left-to-right axial
      // gradient and target the slice rect.
      int gradId = doc->AxialGradient(
        wxPdfColour(c0.Red(), c0.Green(), c0.Blue()),
        wxPdfColour(c1.Red(), c1.Green(), c1.Blue()),
        0.0, 0.5, 1.0, 0.5, 1.0);
      if (gradId > 0)
      {
        // Set alpha for this slice. This provides a stepped approximation 
        // for axial alpha gradients.
        doc->SetAlpha(1.0, c0.Alpha() / 255.0);
        doc->SetFillGradient(t0, -bigY, t1 - t0, 2 * bigY, gradId);
      }
    }

    context->PopState();
    return;
  }

  // Radial. PDF supports two-stop radial natively. For >2 stops we'd need
  // annular clipping, which wxPdfDocument doesn't currently expose; fall
  // back to the first / last stop pair so we at least produce a smooth
  // radial gradient.
  const wxColour c0 = stops.GetStartColour();
  const wxColour c1 = stops.GetEndColour();
  const double xo = brush->GetGradX1();
  const double yo = brush->GetGradY1();
  const double xc = brush->GetGradX2();
  const double yc = brush->GetGradY2();
  const double r  = brush->GetGradRadius();

  // Use the path's bbox as the SetFillGradient rect. The radial gradient
  // is registered in 0..1 normalised coords mapped onto that rect.
  double bx, by, bw, bh;
  // We use the global bbox of the shape via a temporary path-data trick:
  // pull it from the source GraphicsPath via the brush's caller. Easier:
  // recompute from the shape's segments here.
  bx = by = 0.0; bw = bh = 1.0;
  {
    const unsigned int n = shape.GetSegmentCount();
    int iter = 0;
    double pts[8];
    bool first = true;
    double minX = 0, minY = 0, maxX = 0, maxY = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
      wxPdfSegmentType seg = shape.GetSegment(i, iter, pts);
      int npts = 0;
      switch (seg)
      {
        case wxPDF_SEG_MOVETO:
        case wxPDF_SEG_LINETO:
        case wxPDF_SEG_CLOSE:
          npts = 1; break;
        case wxPDF_SEG_CURVETO:
          npts = 3; break;
        default: break;
      }
      for (int k = 0; k < npts; ++k)
      {
        const double x = pts[2 * k];
        const double y = pts[2 * k + 1];
        if (first) { minX = maxX = x; minY = maxY = y; first = false; }
        else
        {
          if (x < minX) minX = x;
          if (x > maxX) maxX = x;
          if (y < minY) minY = y;
          if (y > maxY) maxY = y;
        }
      }
      iter += npts;
    }
    if (!first)
    {
      bx = minX; by = minY; bw = maxX - minX; bh = maxY - minY;
    }
  }

  if (bw <= 0.0 || bh <= 0.0) return;

  // Normalise centres / radius into the bbox 0..1 range.
  const double nxo = (xo - bx) / bw;
  const double nyo = (yo - by) / bh;
  const double nxc = (xc - bx) / bw;
  const double nyc = (yc - by) / bh;
  const double nr  = r / wxMax(bw, bh);

  context->PushState();
  context->Clip(shape);
  int gradId = doc->RadialGradient(
    wxPdfColour(c0.Red(), c0.Green(), c0.Blue()),
    wxPdfColour(c1.Red(), c1.Green(), c1.Blue()),
    nxo, nyo, 0.0, nxc, nyc, nr, 1.0);
  if (gradId > 0)
  {
    if (c0.Alpha() != c1.Alpha())
    {
      const int steps = 16;
      for (int i = 0; i < steps; ++i)
      {
        double t0 = (double)i / steps;
        double t1 = (double)(i + 1) / steps;

        double alpha = (c0.Alpha() + (c1.Alpha() - c0.Alpha()) * t0) / 255.0;

        context->PushState();

        wxPdfShape annulus;
        auto AddEllipseToShape = [](wxPdfShape& s, double ex, double ey, double erx, double ery) {
          const double kappa = 0.55228474983079339840;
          double eox = erx * kappa;
          double eoy = ery * kappa;
          s.MoveTo(ex + erx, ey);
          s.CurveTo(ex + erx, ey + eoy, ex + eox, ey + ery, ex, ey + ery);
          s.CurveTo(ex - eox, ey + ery, ex - erx, ey + eoy, ex - erx, ey);
          s.CurveTo(ex - erx, ey - eoy, ex - eox, ey - ery, ex, ey - ery);
          s.CurveTo(ex + eox, ey - ery, ex + erx, ey - eoy, ex + erx, ey);
          s.ClosePath();
        };

        // Radii at t0 and t1. nr is normalized radius (0..0.5 typical) 
        // relative to max(bw,bh). SetFillGradient maps 0..1 to bw, bh.
        double rx1 = t1 * nr * bw;
        double ry1 = t1 * nr * bh;
        double rx0 = t0 * nr * bw;
        double ry0 = t0 * nr * bh;

        AddEllipseToShape(annulus, xo, yo, rx1, ry1);
        if (t0 > 0)
          AddEllipseToShape(annulus, xo, yo, rx0, ry0);

        // Use context->Clip(shape) which uses W* when we set doc filling rule.
        // The context tracks the clip's 'q' so PopState() balances it correctly.
        int saveRule = doc->GetFillingRule();
        doc->SetFillingRule(wxODDEVEN_RULE);
        context->Clip(annulus);
        doc->SetFillingRule(saveRule);

        doc->SetAlpha(1.0, alpha);
        doc->SetFillGradient(bx, by, bw, bh, gradId);
        
        context->PopState();
      }
    }
    else
    {
      // For radial gradients with uniform alpha, we use the start color's 
      // alpha as a constant alpha for the entire gradient.
      doc->SetAlpha(1.0, c0.Alpha() / 255.0);
      doc->SetFillGradient(bx, by, bw, bh, gradId);
    }
  }
  else
  {
    // Fallback: solid fill if gradient registration failed.
    int saveRule = doc->GetFillingRule();
    doc->SetFillingRule(fillStyle);
    doc->Shape(shape, wxPDF_STYLE_FILL);
    doc->SetFillingRule(saveRule);
  }
  context->PopState();
}

void wxPdfGraphicsContext::FillPath(const wxGraphicsPath& path, wxPolygonFillMode fillStyle)
{
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::FillPath - no valid PDF document"));

  if (m_brush.IsNull()) return;

  wxPdfGraphicsBrushData* brush =
    static_cast<wxPdfGraphicsBrushData*>(m_brush.GetRefData());
  brush->Apply(this);

  const wxPdfShape& shape =
    ((wxPdfGraphicsPathData*) path.GetRefData())->GetPdfShape();

  if (brush->HasGradient())
  {
    DoFillPathGradient(this, brush, shape, fillStyle);
    return;
  }

  int saveFillingRule = m_pdfDocument->GetFillingRule();
  m_pdfDocument->SetFillingRule(fillStyle);
  m_pdfDocument->Shape(shape, wxPDF_STYLE_FILL);
  m_pdfDocument->SetFillingRule(saveFillingRule);
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
  wxLogDebug(wxT("wxPdfGraphicsContext::GetPartialTextExtents - not implemented"));
  widths.Clear();
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
  wxCHECK_RET(m_pdfDocument, wxT("wxPdfGraphicsContext::DrawBitmap - no valid PDF document"));
  if (bmp.IsNull()) return;

  wxPdfGraphicsBitmapData* data =
    static_cast<wxPdfGraphicsBitmapData*>(bmp.GetRefData());
  if (!data) return;
  const wxImage& image = data->GetImage();
  if (!image.IsOk()) return;

  // Each embedded image needs a unique name; m_imageCount provides the
  // monotonic suffix the same way wxPdfDCImpl::DoDrawBitmap does.
  const wxString imgName = wxString::Format(wxT("pdfgcimg%d"), ++m_imageCount);
  m_pdfDocument->Image(imgName, image, x, y, w, h, wxPdfLink(-1), 0);
}

void
wxPdfGraphicsContext::DrawIcon(const wxIcon& icon, wxDouble x, wxDouble y, wxDouble w, wxDouble h)
{
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
// wxPdfGraphicsRenderer implementation
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wxPdfGraphicsRenderer, wxGraphicsRenderer);

wxGraphicsRenderer*
wxPdfGraphicsRenderer::GetPdfRenderer()
{
  static wxPdfGraphicsRenderer s_renderer;
  return &s_renderer;
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromPrintData(const wxPrintData& printData)
{
  return new wxPdfGraphicsContext(this, printData);
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(wxPdfDC* dc)
{
  if (!dc)
  {
    return NULL;
  }
  wxPdfDocument* pdfDocument = dc->GetPdfDocument();
  if (!pdfDocument)
  {
    return NULL;
  }
  // The DC owns the document; reuse the template-mode constructor so the
  // GC does not delete it on destruction.
  int width = 0;
  int height = 0;
  dc->GetSize(&width, &height);
  return new wxPdfGraphicsContext(this, pdfDocument,
                                  static_cast<double>(width),
                                  static_cast<double>(height));
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromDocument(wxPdfDocument* pdfDocument,
                                                 double templateWidth,
                                                 double templateHeight)
{
  return new wxPdfGraphicsContext(this, pdfDocument, templateWidth, templateHeight);
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxWindowDC& WXUNUSED(dc))
{
  return NULL;
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxMemoryDC& WXUNUSED(dc))
{
  return NULL;
}

#if wxUSE_PRINTING_ARCHITECTURE
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxPrinterDC& WXUNUSED(dc))
{
  return NULL;
}
#endif

#ifdef __WXMSW__
#if wxUSE_ENH_METAFILE
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContext(const wxEnhMetaFileDC& WXUNUSED(dc))
{
  return NULL;
}
#endif
#endif

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromNativeContext(void* WXUNUSED(context))
{
  return NULL;
}

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromNativeWindow(void* WXUNUSED(window))
{
  return NULL;
}

#ifdef __WXMSW__
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromNativeHDC(WXHDC WXUNUSED(dc))
{
  return NULL;
}
#endif

wxGraphicsContext * wxPdfGraphicsRenderer::CreateContext(wxWindow* WXUNUSED(window))
{
  return NULL;
}

#if wxUSE_IMAGE
wxGraphicsContext*
wxPdfGraphicsRenderer::CreateContextFromImage(wxImage& WXUNUSED(image))
{
  return NULL;
}
#endif // wxUSE_IMAGE

wxGraphicsContext*
wxPdfGraphicsRenderer::CreateMeasuringContext()
{
  // Document-less context. Only GetTextExtent and font / path / matrix
  // operations that don't touch a wxPdfDocument are expected to work,
  // matching wxCairoRenderer::CreateMeasuringContext() semantics.
  return new wxPdfGraphicsContext(this);
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
wxPdfGraphicsRenderer::CreatePen(const wxGraphicsPenInfo& info)
{
  if (info.GetStyle() == wxPENSTYLE_TRANSPARENT)
  {
    return wxNullGraphicsPen;
  }
  wxGraphicsPen p;
  p.SetRefData(new wxPdfGraphicsPenData(this, info));
  return p;
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
                                                 const wxGraphicsGradientStops& stops,
                                                 const wxGraphicsMatrix& WXUNUSED(matrix))
{
  // We currently ignore the optional gradient matrix; the gradient axis
  // is treated as already in user space.
  wxGraphicsBrush p;
  wxPdfGraphicsBrushData* d = new wxPdfGraphicsBrushData(this);
  d->CreateLinearGradientBrush(x1, y1, x2, y2, stops);
  p.SetRefData(d);
  return p;
}

wxGraphicsBrush
wxPdfGraphicsRenderer::CreateRadialGradientBrush(wxDouble xo, wxDouble yo,
                                                 wxDouble xc, wxDouble yc, wxDouble r,
                                                 const wxGraphicsGradientStops& stops,
                                                 const wxGraphicsMatrix& WXUNUSED(matrix))
{
  // Optional gradient matrix is ignored; the gradient is treated as
  // already in user space.
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

wxGraphicsFont
wxPdfGraphicsRenderer::CreateFontAtDPI(const wxFont& font,
                                       const wxRealPoint& WXUNUSED(dpi),
                                       const wxColour& col)
{
  // PDF user space is fixed at 72 dpi, so the requested DPI doesn't
  // affect anything in the produced document. Forward to the regular
  // wxFont-based factory.
  return CreateFont(font, col);
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
wxPdfGraphicsRenderer::CreateBitmapFromNativeBitmap(void* WXUNUSED(bitmap))
{
  wxFAIL_MSG("wxPdfGraphicsRenderer::CreateBitmapFromNativeBitmap is not implemented.");
  return wxNullGraphicsBitmap;
}

wxGraphicsBitmap
wxPdfGraphicsRenderer::CreateSubBitmap(const wxGraphicsBitmap& WXUNUSED(bitmap),
                                       wxDouble WXUNUSED(x), wxDouble WXUNUSED(y),
                                       wxDouble WXUNUSED(w), wxDouble WXUNUSED(h))
{
  wxFAIL_MSG("wxPdfGraphicsRenderer::CreateSubBitmap is not implemented.");
  return wxNullGraphicsBitmap;
}

wxString
wxPdfGraphicsRenderer::GetName() const
{
  return wxS("pdf");
}

void
wxPdfGraphicsRenderer::GetVersion(int* major, int* minor, int* micro) const
{
  // Track the wxpdfdoc release.
  if (major) *major = PDFDOC_MAJOR_VERSION;
  if (minor) *minor = PDFDOC_MINOR_VERSION;
  if (micro) *micro = PDFDOC_RELEASE_NUMBER;
}


#endif // wxUSE_GRAPHICS_CONTEXT
