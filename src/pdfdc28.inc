///////////////////////////////////////////////////////////////////////////////
// Name:        pdfdc28.inc
// Purpose:     Implementation of wxPdfDC for wxWidgets 2.8.x
// Author:      Ulrich Telle
// Modified by:
// Created:     2010-11-28
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfdc28.inc Implementation of the wxPdfDC for wxWidgets 2.8.x

#include <wx/region.h>
#include <wx/font.h>
#include <wx/paper.h>
#include "wx/pdfdc.h" 
#include "wx/pdffontmanager.h"

#include <math.h>

static double
angleByCoords(wxCoord xa, wxCoord ya, wxCoord xc, wxCoord yc)
{
  static double pi = 4. * atan(1.0);
  double diffX = xa - xc, diffY = -(ya - yc), ret = 0;
  if (diffX == 0) // singularity
  {
    ret = diffY > 0 ? 90 : 270;
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

IMPLEMENT_DYNAMIC_CLASS(wxPdfDC, wxDC)

wxPdfDC::wxPdfDC()
{
  Init();
  m_ok = true;
}

wxPdfDC::wxPdfDC(const wxPrintData& data)
{
  Init();
  SetPrintData(data);
  m_ok = true;
}

wxPdfDC::wxPdfDC(wxPdfDocument* pdfDocument, double templateWidth, double templateHeight)
{
  Init();
  m_templateMode = true;
  m_templateWidth = templateWidth;
  m_templateHeight = templateHeight;
  m_pdfDocument = pdfDocument;
}

wxPdfDC::~wxPdfDC()
{
  if (m_pdfDocument != NULL)
  {
    EndDoc();
  }
}

void
wxPdfDC::Init()
{
  m_templateMode = false;
  m_ppi = 72;
  m_bkgMode = wxSOLID;

  m_isInteractive = false; // Is GetPixel possible ?
  m_logicalOriginX = 0;
  m_logicalOriginY = 0;

  m_deviceOriginX = 0;
  m_deviceOriginY = 0;

  m_logicalScaleX = 1.0;
  m_logicalScaleY = 1.0;

  m_scaleX = 1.0;
  m_scaleY = 1.0;

  m_userScaleX = 1.0;
  m_userScaleY = 1.0;

  m_mappingMode = wxMM_TEXT;

  m_pdfDocument = NULL;
  m_imageCount = 0;

  SetBackgroundMode(wxSOLID);
  
  m_printData.SetOrientation(wxPORTRAIT);
  m_printData.SetPaperId(wxPAPER_A4);
  m_printData.SetFilename(wxT("default.pdf"));
}

wxPdfDocument*
wxPdfDC::GetPdfDocument()
{
  return m_pdfDocument;
}

void
wxPdfDC::SetPrintData(const wxPrintData& data)
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
wxPdfDC::SetResolution(int ppi)
{
  m_ppi = ppi;
}

int
wxPdfDC::GetResolution() const
{
  return (int) m_ppi;
}

void
wxPdfDC::Clear()
{
  // Not yet implemented
}

bool
wxPdfDC::StartDoc( const wxString& message )
{
  wxCHECK_MSG(m_ok, false, wxT("Invalid PDF DC"));
  wxUnusedVar(message);
  if (!m_templateMode && m_pdfDocument == NULL)
  {
    m_pdfDocument = new wxPdfDocument(m_printData.GetOrientation(), wxString(wxT("pt")), m_printData.GetPaperId());
    m_pdfDocument->Open();
    m_pdfDocument->SetCompression(false);
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
wxPdfDC::EndDoc()
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
wxPdfDC::StartPage()
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if (!m_templateMode)
  {
    // Begin a new page
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
wxPdfDC::EndPage()
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
wxPdfDC::SetFont(const wxFont& font)
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
    // TODO: point size adjustment is not always reliable
#if wxCHECK_VERSION(2,8,10)
    double size = (double) font.GetPointSize() * GetFontPointSizeAdjustment(72.0) * 4./3.;
#else
    // GetFontPointSizeAdjustment is not available in wxWidgets versions < 2.8.10
    // TODO: apply a correction nevertheless?
    double size = (double) font.GetPointSize() * 4./3.;
#endif    
    ok = m_pdfDocument->SetFont(regFont, styles, ScaleToPdf(LogicalToDeviceXRel(size) * m_pdfDocument->GetScaleFactor()));
  }
}

void
wxPdfDC::SetPen(const wxPen& pen)
{
  m_pen = pen;
}

void
wxPdfDC::SetBrush(const wxBrush& brush)
{
  m_brush = brush;
}

void
wxPdfDC::SetBackground(const wxBrush& brush)
{
  // Not yet implemented
  wxUnusedVar(brush);
}

void
wxPdfDC::SetBackgroundMode(int mode)
{
  // TODO: check implementation
  m_bkgMode = (mode == wxTRANSPARENT) ? wxTRANSPARENT : wxSOLID; // N.B.: per ora non usato
}

void
wxPdfDC::SetPalette(const wxPalette& palette)
{
  // Not yet implemented
  wxUnusedVar(palette);
}

void
wxPdfDC::DestroyClippingRegion()
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
wxPdfDC::GetCharHeight() const
{
  wxCoord height = 12;
  if (m_font.Ok())
  {
    height = m_font.GetPointSize();
  }
  return height;
}

wxCoord
wxPdfDC::GetCharWidth() const
{
  // Chris Breeze: reasonable approximation using wxMODERN/Courier
  return (wxCoord) (GetCharHeight() * 72.0 / 120.0);
}

bool
wxPdfDC::CanDrawBitmap() const
{
  return true;
}

bool
wxPdfDC::CanGetTextExtent() const
{
  return true;
}

int
wxPdfDC::GetDepth() const
{
  // TODO: check value
  return 32;
}

wxSize
wxPdfDC::GetPPI() const
{
  int ppi = (int) m_ppi;
  return wxSize(ppi,ppi);
}

void
wxPdfDC::SetMapMode(int mode)
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
wxPdfDC::SetUserScale(double x, double y)
{
  m_userScaleX = x;
  m_userScaleY = y;
  ComputeScaleAndOrigin();
}

void
wxPdfDC::SetLogicalScale(double x, double y)
{
  m_logicalScaleX = x;
  m_logicalScaleY = y;
  ComputeScaleAndOrigin();
}

void
wxPdfDC::SetLogicalOrigin(wxCoord x, wxCoord y)
{
  m_logicalOriginX = x * m_signX;
  m_logicalOriginY = y * m_signY;
  ComputeScaleAndOrigin();
}

void
wxPdfDC::SetDeviceOrigin(wxCoord x, wxCoord y)
{
  m_deviceOriginX = x;
  m_deviceOriginY = y;
  ComputeScaleAndOrigin();
}

void
wxPdfDC::SetAxisOrientation(bool xLeftRight, bool yBottomUp)
{
  m_signX = (xLeftRight ?  1 : -1);
  m_signY = (yBottomUp  ? -1 :  1);
  ComputeScaleAndOrigin();
}

void
wxPdfDC::SetLogicalFunction(int function)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  // TODO: check implementation
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
wxPdfDC::DoFloodFill(wxCoord x, wxCoord y, const wxColour& col, int style)
{
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxUnusedVar(col);
  wxUnusedVar(style);
  wxFAIL_MSG(wxString(wxT("wxPdfDC::FloodFill: "))+_("Not implemented."));
  return false;
}

void
wxPdfDC::DoGradientFillLinear(const wxRect& rect,
                              const wxColour& initialColour,
                              const wxColour& destColour,
                              wxDirection nDirection)
{
  // Not yet implemented
  wxUnusedVar(rect);
  wxUnusedVar(initialColour);
  wxUnusedVar(destColour);
  wxUnusedVar(nDirection);
}

void
wxPdfDC::DoGradientFillConcentric(const wxRect& rect,
                                  const wxColour& initialColour,
                                  const wxColour& destColour,
                                  const wxPoint& circleCenter)
{
  // Not yet implemented
  wxUnusedVar(rect);
  wxUnusedVar(initialColour);
  wxUnusedVar(destColour);
  wxUnusedVar(circleCenter);
}

bool
wxPdfDC::DoGetPixel(wxCoord x, wxCoord y, wxColour* col) const
{
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxUnusedVar(col);
  wxFAIL_MSG(wxString(wxT("wxPdfDC::DoGetPixel: "))+_("Not implemented."));
  return false;
}

void
wxPdfDC::DoDrawPoint(wxCoord x, wxCoord y)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupPen();
  double xx = ScaleToPdf(LogicalToDeviceX(x));
  double yy = ScaleToPdf(LogicalToDeviceY(y));
  m_pdfDocument->SetFillColour(m_pdfDocument->GetDrawColour());
  m_pdfDocument->Rect(xx-0.5, yy-0.5, xx+0.5, yy+0.5);
  CalcBoundingBox(x, y);
}

#if wxUSE_SPLINES
void
wxPdfDC::DoDrawSpline(wxList* points)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetPen( m_pen );
  wxASSERT_MSG( points, wxT("NULL pointer to spline points?") );
  const size_t n_points = points->GetCount();
  wxASSERT_MSG( n_points > 2 , wxT("incomplete list of spline points?") );

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

  x1 = ScaleToPdf(LogicalToDeviceX(p->x));
  y1 = ScaleToPdf(LogicalToDeviceY(p->y));
  m_pdfDocument->MoveTo(x1, y1);

  node = node->GetNext();
  p = (wxPoint*) node->GetData();

  bx1 = x2 = ScaleToPdf(LogicalToDeviceX(p->x));
  by1 = y2 = ScaleToPdf(LogicalToDeviceY(p->y));
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
    x2 = ScaleToPdf(LogicalToDeviceX(p->x));
    y2 = ScaleToPdf(LogicalToDeviceY(p->y));
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
wxPdfDC::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  if  (m_pen.GetStyle() != wxTRANSPARENT)
  {
    SetupBrush();
    SetupPen();
    m_pdfDocument->Line(ScaleToPdf(LogicalToDeviceX(x1)), ScaleToPdf(LogicalToDeviceY(y1)), 
                        ScaleToPdf(LogicalToDeviceX(x2)), ScaleToPdf(LogicalToDeviceY(y2)));
    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
  }
}

void
wxPdfDC::DoDrawArc(wxCoord x1, wxCoord y1,
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
    xx1 = ScaleToPdf(LogicalToDeviceX(xx1));
    yy1 = ScaleToPdf(LogicalToDeviceY(yy1));
    xx2 = ScaleToPdf(LogicalToDeviceX(xx2));
    yy2 = ScaleToPdf(LogicalToDeviceY(yy2));
    xxc = ScaleToPdf(LogicalToDeviceX(xxc));
    yyc = ScaleToPdf(LogicalToDeviceY(yyc));
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
wxPdfDC::DoDrawCheckMark(wxCoord x, wxCoord y,
                         wxCoord width, wxCoord height)
{
  // Not yet implemented
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxUnusedVar(width);
  wxUnusedVar(height);
}

void
wxPdfDC::DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord width, wxCoord height,
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
    DrawEllipse(x, y, width, height);
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
      m_pdfDocument->SetLineWidth(ScaleToPdf(LogicalToDeviceXRel(1))); // pen width != 1 sometimes fools readers when closing paths
      int style = wxPDF_STYLE_FILL | wxPDF_STYLE_DRAWCLOSE;
      if (!(doDraw && doFill))
      {
        style = (doFill) ? wxPDF_STYLE_FILL : wxPDF_STYLE_DRAWCLOSE;
      }
      m_pdfDocument->Ellipse(ScaleToPdf(LogicalToDeviceX(x + 0.5 * width)), 
                             ScaleToPdf(LogicalToDeviceY(y + 0.5 * height)),
                             ScaleToPdf(LogicalToDeviceXRel(0.5 * width)), 
                             ScaleToPdf(LogicalToDeviceYRel(0.5 * height)), 
                             0, sa, ea, style, 8, true);
      CalcBoundingBox(x, y);
      CalcBoundingBox(x+width, y+height);
    }
  }
}

void
wxPdfDC::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupBrush();
  SetupPen();
  m_pdfDocument->Rect(ScaleToPdf(LogicalToDeviceX(x)), ScaleToPdf(LogicalToDeviceY(y)),
                      ScaleToPdf(LogicalToDeviceXRel(width)), ScaleToPdf(LogicalToDeviceYRel(height)), 
                      GetDrawingStyle()); 
  CalcBoundingBox(x, y);
  CalcBoundingBox(x+width, y+height);
}

void
wxPdfDC::DoDrawRoundedRectangle(wxCoord x, wxCoord y,
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
  m_pdfDocument->RoundedRect(ScaleToPdf(LogicalToDeviceX(x)), ScaleToPdf(LogicalToDeviceY(y)), 
                             ScaleToPdf(LogicalToDeviceXRel(width)), ScaleToPdf(LogicalToDeviceYRel(height)), 
                             ScaleToPdf(LogicalToDeviceXRel(radius)), wxPDF_CORNER_ALL, GetDrawingStyle());  
  CalcBoundingBox(x, y);
  CalcBoundingBox(x+width, y+height);
}

void
wxPdfDC::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupBrush();
  SetupPen();
  m_pdfDocument->Ellipse(ScaleToPdf(LogicalToDeviceX(x + width / 2.0)), ScaleToPdf(LogicalToDeviceY(y + height / 2.0)), 
                         ScaleToPdf(LogicalToDeviceXRel(width / 2.0)), ScaleToPdf(LogicalToDeviceYRel(height / 2.0)), 
                         0, 0, 360, GetDrawingStyle()); 
  CalcBoundingBox(x-width, y-height);
  CalcBoundingBox(x+width, y+height);
}

void
wxPdfDC::DoCrossHair(wxCoord x, wxCoord y)
{
  wxUnusedVar(x);
  wxUnusedVar(y);
  wxFAIL_MSG(wxString(wxT("wxPdfDC::DoCrossHair: "))+_("Not implemented."));
}

void
wxPdfDC::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
  DrawBitmap(icon, x, y, true);
}

void
wxPdfDC::DoDrawBitmap(const wxBitmap& bitmap, wxCoord x, wxCoord y,
                      bool useMask)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  wxCHECK_RET( IsOk(), wxT("wxPdfDC::DoDrawBitmap - invalid DC") );
  wxCHECK_RET( bitmap.Ok(), wxT("wxPdfDC::DoDrawBitmap - invalid bitmap") );

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

  wxCoord ww = ScaleToPdf(LogicalToDeviceXRel(w));
  wxCoord hh = ScaleToPdf(LogicalToDeviceYRel(h));

  wxCoord xx = ScaleToPdf(LogicalToDeviceX(x));
  wxCoord yy = ScaleToPdf(LogicalToDeviceY(y));

  wxString imgName = wxString::Format(wxT("pdfdcimg%d"), ++m_imageCount);

  if (bitmap.GetDepth() == 1)
  {
    wxPen savePen = m_pen;
    wxBrush saveBrush = m_brush;
    SetPen(*wxTRANSPARENT_PEN);
    SetBrush(wxBrush(m_textBackgroundColour, wxSOLID));
    DrawRectangle(xx, yy, ww, hh);        
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
wxPdfDC::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
  DoDrawRotatedText(text, x, y, 0.0);
}

void
wxPdfDC::DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                           double angle)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  wxFont* fontToUse = &m_font;
  if (!fontToUse->IsOk())
  {
    return;
  }
#ifdef wx_msw
  y += m_pdfDocument->GetFontSize();
#else
  y += 0.8f * GetCharHeight();
#endif
  wxFont old = m_font; 
  m_pdfDocument->SetTextColour(m_textForegroundColour.Red(), m_textForegroundColour.Green(), m_textForegroundColour.Blue());
#if wxCHECK_VERSION(2,8,10)
  double fontSize = ScaleToPdf(LogicalToDeviceXRel(m_font.GetPointSize() * GetFontPointSizeAdjustment(72.0) * 4./3.));
  m_pdfDocument->SetFontSize(fontSize * m_pdfDocument->GetScaleFactor());
#else
  // GetFontPointSizeAdjustment is not available in wxWidgets versions < 2.8.10
  // TODO: apply a correction nevertheless?
  double fontSize = ScaleToPdf(LogicalToDeviceXRel(m_font.GetPointSize() * 4./3.));
  m_pdfDocument->SetFontSize(fontSize * m_pdfDocument->GetScaleFactor());
#endif  
  m_pdfDocument->RotatedText(ScaleToPdf(LogicalToDeviceX(x)), ScaleToPdf(LogicalToDeviceY(y)), text, angle);
  SetFont(old);
}

bool
wxPdfDC::DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                wxDC* source, wxCoord xsrc, wxCoord ysrc,
                int rop /*= wxCOPY*/, bool useMask /*= false*/, 
                wxCoord xsrcMask /*= -1*/, wxCoord ysrcMask /*= -1*/)
{
  wxCHECK_MSG( IsOk(), false, wxT("wxPdfDC::DoBlit - invalid DC") );
  wxCHECK_MSG( source->Ok(), false, wxT("wxPdfDC::DoBlit - invalid source DC") );

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
  DrawBitmap( bitmap, xdest, ydest );

  return true;
}

void
wxPdfDC::DoGetSize(int* width, int* height) const
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
wxPdfDC::DoGetSizeMM(int* width, int* height) const
{
  // Not yet implemented
  wxUnusedVar(width);
  wxUnusedVar(height);
}

void
wxPdfDC::DoDrawLines(int n, wxPoint points[],
                     wxCoord xoffset, wxCoord yoffset)
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  SetupPen();
  int i;
  for (i = 0; i < n; ++i)
  {
    wxPoint& point = points[i];
    double xx = ScaleToPdf(LogicalToDeviceX(xoffset + point.x));
    double yy = ScaleToPdf(LogicalToDeviceY(yoffset + point.y));
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
wxPdfDC::DoDrawPolygon(int n, wxPoint points[],
                       wxCoord xoffset, wxCoord yoffset,
                       int fillStyle /* = wxODDEVEN_RULE*/)
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
    xp.Add(ScaleToPdf(LogicalToDeviceX(xoffset + point.x)));
    yp.Add(ScaleToPdf(LogicalToDeviceY(yoffset + point.y)));
    CalcBoundingBox(point.x + xoffset, point.y + yoffset);
  }
  int saveFillingRule = m_pdfDocument->GetFillingRule();
  m_pdfDocument->SetFillingRule(fillStyle);
  int style = GetDrawingStyle();
  m_pdfDocument->Polygon(xp, yp, style);
  m_pdfDocument->SetFillingRule(saveFillingRule);
}

void
wxPdfDC::DoDrawPolyPolygon(int n, int count[], wxPoint points[],
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
        xp.Add(ScaleToPdf(LogicalToDeviceX(xoffset + point.x)));
        yp.Add(ScaleToPdf(LogicalToDeviceY(yoffset + point.y)));
        CalcBoundingBox(point.x + xoffset, point.y + yoffset);
      }
      m_pdfDocument->Polygon(xp, yp, style);
    }
    m_pdfDocument->SetFillingRule(saveFillingRule);
  }
}

void
wxPdfDC::DoSetClippingRegionAsRegion(const wxRegion& region)
{
  wxCoord x, y, w, h;
  region.GetBox(x, y, w, h);
  DoSetClippingRegion(x, y, w, h);
}

void
wxPdfDC::DoSetClippingRegion(wxCoord x, wxCoord y,
                             wxCoord width, wxCoord height)
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
  m_pdfDocument->ClippingRect(ScaleToPdf(LogicalToDeviceX(x)), 
                              ScaleToPdf(LogicalToDeviceY(y)), 
                              ScaleToPdf(LogicalToDeviceXRel(width)), 
                              ScaleToPdf(LogicalToDeviceYRel(height)));
  m_clipping = true;
}

void
wxPdfDC::DoGetTextExtent(const wxString& text,
                         wxCoord* x, wxCoord* y,
                         wxCoord* descent,
                         wxCoord* externalLeading,
                         wxFont* theFont) const
{
  wxCHECK_RET(m_pdfDocument, wxT("Invalid PDF DC"));
  // TODO
  wxUnusedVar(externalLeading);
  wxFont* fontToUse = theFont;
  if (!fontToUse)
  {
    fontToUse = const_cast<wxFont*>(&m_font);
  }

  if (fontToUse)
  {
    wxFont old = m_font;
    const_cast<wxPdfDC*>(this)->SetFont(*fontToUse);
    if (descent)
    {
      *descent = DeviceToLogicalYRel(m_pdfDocument->GetFontSize() * (m_pdfDocument->GetFontDescription().GetDescent() / 1000.0));
    }
    *x = DeviceToLogicalXRel(const_cast<wxPdfDC*>(this)->m_pdfDocument->GetStringWidth(text));
    *y = DeviceToLogicalYRel(GetCharHeight());
    const_cast<wxPdfDC*>(this)->SetFont(old);
  }
  else
  {
    *x = *y = 0;
  }
}

bool
wxPdfDC::DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const
{
  // Not yet implemented
  wxUnusedVar(text);
  wxUnusedVar(widths);
  return false;
}

void
wxPdfDC::SetupPen()
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
      style.SetWidth(ScaleToPdf(LogicalToDeviceXRel(curPen.GetWidth())));
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
    m_pdfDocument->SetLineWidth(ScaleToPdf(LogicalToDeviceXRel(1.0)));
  }
}

void
wxPdfDC::SetupBrush()
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
wxPdfDC::GetDrawingStyle()
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
wxPdfDC::ScaleToPdf(wxCoord x) const
{
  return (double) x * (72. / m_ppi);
}
