///////////////////////////////////////////////////////////////////////////////
// Name:        pdfgraphics.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-08-24
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfgraphics.h Interface of PDF graphics classes

#ifndef _PDFGRAPHICS_H_
#define _PDFGRAPHICS_H_

#include "wx/pdfdocdef.h"

/// Class representing ExtGState. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfExtGState
{
public:
  /// Constructor
  wxPdfExtGState(double lineAlpha, double fillAlpha, wxPdfBlendMode blendMode);

  /// Destructor
  virtual ~wxPdfExtGState();

  /// Set ExtGState object index
  void SetObjIndex(int n) { m_n = n; }

  /// Get ExtGState object index
  int  GetObjIndex() { return m_n; }

  double GetLineAlpha() const { return m_lineAlpha; }
  double GetFillAlpha() const { return m_fillAlpha; }
  wxPdfBlendMode GetBlendMode() const { return m_blendMode; };

private:
  int            m_n;         ///< ExtGState index
  double         m_lineAlpha;
  double         m_fillAlpha;
  wxPdfBlendMode m_blendMode;
};

/// Class representing a coons patch. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfCoonsPatch
{
public:
  wxPdfCoonsPatch(int edgeFlag, wxPdfColour colors[], double x[], double y[]);
  virtual ~wxPdfCoonsPatch();

  int GetEdgeFlag() { return m_edgeFlag; }
  wxPdfColour* GetColors() { return m_colors; }
  double* GetX() { return m_x; }
  double* GetY() { return m_y; }
private:
  int m_edgeFlag;
  wxPdfColour m_colors[4];
  double m_x[12];
  double m_y[12];
};

enum wxPdfGradientType
{
  wxPDF_GRADIENT_AXIAL,
  wxPDF_GRADIENT_MIDAXIAL,
  wxPDF_GRADIENT_RADIAL,
  wxPDF_GRADIENT_COONS
};

/// Class representing gradients. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfGradient
{
public:
  /// Constructor
  wxPdfGradient(wxPdfGradientType type);

  /// Destructor
  virtual ~wxPdfGradient();

  /// Set gradient object index
  void SetObjIndex(int n) { m_n = n; }

  /// Get gradient object index
  int  GetObjIndex() { return m_n; }

  /// Get the gradient type
  const wxPdfGradientType GetType() const { return m_type; };

protected:
  wxPdfGradientType m_type;      ///< Gradient type

private:
  int               m_n;         ///< Gradient index
};

/// Class representing axial gradients. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfAxialGradient : public wxPdfGradient
{
public:
  /// Constructor
  wxPdfAxialGradient(const wxPdfColour& color1, const wxPdfColour& color2, double x1, double y1, double x2, double y2, double intexp);

  /// Destructor
  virtual ~wxPdfAxialGradient();

  /// Get the gradient color 1
  const wxPdfColour& GetColor1() const { return m_color1; };

  /// Get the gradient color 2
  const wxPdfColour& GetColor2() const { return m_color2; };

  double GetX1() const { return m_x1; }
  double GetY1() const { return m_y1; }
  double GetX2() const { return m_x2; }
  double GetY2() const { return m_y2; }
  double GetIntExp() const { return m_intexp; }

private:
  wxPdfColour m_color1;    ///< Gradient color 1
  wxPdfColour m_color2;    ///< Gradient color 2
  double      m_x1;
  double      m_y1;
  double      m_x2;
  double      m_y2;
  double      m_intexp;
};

/// Class representing mid axial gradients. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfMidAxialGradient : public wxPdfAxialGradient
{
public:
  /// Constructor
  wxPdfMidAxialGradient(const wxPdfColour& color1, const wxPdfColour& color2, double x1, double y1, double x2, double y2, double midpoint, double intexp);

  /// Destructor
  virtual ~wxPdfMidAxialGradient();

  double GetMidPoint() const { return m_midpoint; }

private:
  double      m_midpoint;
};

/// Class representing radial gradients. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfRadialGradient : public wxPdfAxialGradient
{
public:
  /// Constructor
  wxPdfRadialGradient(const wxPdfColour& color1, const wxPdfColour& color2, double x1, double y1, double r1, double x2, double y2, double r2, double intexp);

  /// Destructor
  virtual ~wxPdfRadialGradient();

  double GetR1() const { return m_r1; }
  double GetR2() const { return m_r2; }

private:
  double      m_r1;
  double      m_r2;
};

/// Class representing coons patch mesh gradients. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfCoonsPatchGradient : public wxPdfGradient
{
public:
  /// Constructor
  wxPdfCoonsPatchGradient(const wxPdfCoonsPatchMesh& mesh, double minCoord, double maxCoord);

  /// Destructor
  virtual ~wxPdfCoonsPatchGradient();

  wxPdfColourType GetColorType() { return m_colorType; }
  wxMemoryOutputStream* GetBuffer() { return &m_buffer; } 

private:
  wxPdfColourType      m_colorType;
  wxMemoryOutputStream m_buffer;    ///< buffer holding in-memory gradient data
};

/// Class representing a flattened path
class WXDLLIMPEXP_PDFDOC wxPdfFlatPath
{
public:
  /// Constructor
  wxPdfFlatPath(const wxPdfShape* shape, double flatness = 1, int limit = 10);

  /// Destructor
  virtual ~wxPdfFlatPath();

  /// Initialize path iterator
  void InitIter();

  /// Fetch current path segment
  void FetchSegment();

  /// Advance path iterator
  void Next();

  /// Get current path segment
  /**
  * \param[out] coords coordinates of current segment
  * \returns current segment type
  */
  int CurrentSegment(double coords[]);

  /// Subdivide cubic bezier curve path
  void SubdivideCubic();

  /// Check whether path iterator is done
  bool IsDone() { return m_done; }

  /// Measure flattened path length
  double MeasurePathLength();

private:
  const wxPdfShape* m_shape;    ///< associated shape
  double      m_flatnessSq;     ///< square of flatness
  int         m_recursionLimit; ///< resursion limit
  int         m_stackMaxSize;   ///< maximal stack size
  int         m_stackSize;      ///< current stack size
  double*     m_stack;          ///< recursion stack
  int*        m_recLevel;       ///< level stack
  double      m_scratch[6];     ///< coordinate array for current segment
  int         m_iterType;       ///< iterator for segment type
  int         m_iterPoints;     ///< iterator for segment points
  int         m_srcSegType;     ///< current segment type
  double      m_srcPosX;        ///< current segment abscissa
  double      m_srcPosY;        ///< current segment ordinate
  bool        m_done;           ///< flag whether iterator is done
};

#endif
