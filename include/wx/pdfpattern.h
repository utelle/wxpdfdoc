///////////////////////////////////////////////////////////////////////////////
// Name:        pdfpattern.h
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-06-18
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfpattern.h Interface of the pattern classes

#ifndef _PDF_PATTERN_H_
#define _PDF_PATTERN_H_

// wxWidgets headers
#include <wx/graphics.h>
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfproperties.h"

class WXDLLIMPEXP_FWD_PDFDOC wxPdfImage;

/// Class representing patterns.
class WXDLLIMPEXP_PDFDOC wxPdfPattern
{
public:
  /// Constructor for pattern
  /**
  * \param index The pattern index
  * \param width The pattern width
  * \param height The pattern height
  */
  wxPdfPattern(int index, double width, double height);

  /// Constructor for pattern
  /**
  * \param index The pattern index
  * \param width The pattern width
  * \param height The pattern height
  * \param templateId The id of the template to be used as a pattern
  */
  wxPdfPattern(int index, double width, double height, int templateId);

  /// Constructor for pattern
  /**
  * \param index The pattern index
  * \param width The pattern width
  * \param height The pattern height
  * \param patternStyle The hatch style pattern to be used
  * \param drawColour The foreground colour to be used for hatching
  * \param fillColour The background colour to be used to fill the pattern background (optional)
  */
  wxPdfPattern(int index, double width, double height, wxPdfPatternStyle patternStyle, const wxColour& drawColour, const wxColour& fillColour = wxColour());

  /// Constructor for linear gradient pattern
  /**
  * \param index The pattern index
  * \param x1 x-coordinate of start point
  * \param y1 y-coordinate of start point
  * \param x2 x-coordinate of end point
  * \param y2 y-coordinate of end point
  * \param stops List of gradient stops
  * \param matrix Transformation matrix (optional)
  */
  wxPdfPattern(int index, double x1, double y1, double x2, double y2, const wxGraphicsGradientStops& stops, const wxAffineMatrix2D& matrix);

  /// Constructor for radial gradient pattern
  /**
  * \param index The pattern index
  * \param startX x-coordinate of start circle
  * \param startY y-coordinate of start circle
  * \param startRadius radius of start circle
  * \param endX x-coordinate of end circle
  * \param endY y-coordinate of end circle
  * \param endRadius radius of end circle
  * \param stops List of gradient stops
  * \param matrix Transformation matrix (optional)
  */
  wxPdfPattern(int index, double startX, double startY, double startRadius, double endX, double endY, double endRadius,
               const wxGraphicsGradientStops& stops, const wxAffineMatrix2D& matrix);

  /// Copy constructor
  /**
  * \param pattern The pattern to copy
  */
  wxPdfPattern(const wxPdfPattern& pattern);

  /// Set object index
  /**
  * \param index The object index
  */
  void SetObjIndex(int index) { m_objIndex = index; };

  /// Get object index
  /**
  * \return The object index
  */
  int GetObjIndex() const { return m_objIndex; };

  /// Get pattern index
  /**
  * \return The pattern index
  */
  int GetIndex() const { return m_index; };

  /// Set image
  /**
  * \param image The image to use as a pattern
  */
  void SetImage(wxPdfImage* image) { m_image = image; };

  /// Get image
  /**
  * \return The image used as a pattern
  */
  wxPdfImage* GetImage() const {return m_image; };

  /// Get pattern width
  /**
  * \return The pattern width
  */
  double GetWidth() const {return m_width; };

  /// Get pattern height
  /**
  * \return The pattern height
  */
  double GetHeight() const {return m_height; };

  /// Get template id
  /**
  * \return The template ID
  */
  int GetTemplateId() const { return m_templateId; }

  ///  Get pattern style
  /**
  * \return The pattern style
  */
  wxPdfPatternStyle GetPatternStyle() const { return m_patternStyle; }

  ///  Get draw color
  /**
  * \return The draw colour
  */
  wxColour GetDrawColour() const { return m_drawColour; }

  ///  Set fill color
  /**
  * \param fillColour The fill colour
  */
  void SetFillColour(const wxColour& fillColour) { m_fillColour = fillColour; m_hasFillColour = true; }
 
  ///  Get fill color
  /**
  * \return The fill colour
  */
  wxColour GetFillColour() const { return m_fillColour; }

  /// Check whether fill color is set
  /**
  * \return @c true if the pattern has a fill colour, @c false otherwise
  */
  bool HasFillColour() const { return m_hasFillColour; }

  /// Get x-coordinate of start point (linear gradient)
  /**
  * \return x1
  */
  double GetX1() const { return m_x1; }

  /// Get y-coordinate of start point (linear gradient)
  /**
  * \return y1
  */
  double GetY1() const { return m_y1; }

  /// Get x-coordinate of end point (linear gradient)
  /**
  * \return x2
  */
  double GetX2() const { return m_x2; }

  /// Get y-coordinate of end point (linear gradient)
  /**
  * \return y2
  */
  double GetY2() const { return m_y2; }

  /// Get x-coordinate of center of start circle (radial gradient)
  /**
  * \return start x-coordinate
  */
  double GetStartX() const { return m_x1; }

  /// Get y-coordinate of center of start circle (radial gradient)
  /**
  * \return start y-coordinate
  */
  double GetStartY() const { return m_y1; }

  /// Get x-coordinate of center of end circle (radial gradient)
  /**
  * \return end x-coordinate
  */
  double GetEndX() const { return m_x2; }

  /// Get y-coordinate of center of end circle (radial gradient)
  /**
  * \return end y-coordinate
  */
  double GetEndY() const { return m_y2; }

  /// Get radius of start circle (radial gradient)
  /**
  * \return start radius
  */
  double GetStartRadius() const { return m_radius1; }

  /// Get radius of end circle (radial gradient)
  /**
  * \return end Radius
  */
  double GetEndRadius() const { return m_radius2; }

  /// Get list of gradient stops
  /**
  * \return list of gradient stops
  */
  wxGraphicsGradientStops GetStops() const { return m_stops; }

  /// Get transformation matrix
  /**
  * \return affine transformation matrix
  */
  const wxAffineMatrix2D GetMatrix() const { return m_matrix; }

private:
  int    m_objIndex;   ///< object index
  int    m_index;      ///< pattern index

  wxPdfPatternStyle m_patternStyle; ///< pattern style
  wxPdfImage* m_image; ///< image
  int      m_templateId; ///< template id
  wxColour m_drawColour; ///< foreground colour
  wxColour m_fillColour; ///< background colour
  bool     m_hasFillColour; ///< flag whether background colour is defined for the pattern

  double m_width;      ///< pattern width
  double m_height;     ///< pattern height

  // Gradients
  double m_x1;                     ///< x1 or startX, (Linear / Radial)
  double m_y1;                     ///< y1 or startY, (Linear / Radial)
  double m_x2;                     ///< x2 or endX, (Linear / Radial)
  double m_y2;                     ///< y2 or endY, (Linear / Radial)
  double m_radius1;                ///< Start radius (Radial)
  double m_radius2;                ///< End radius (Radial)
  wxGraphicsGradientStops m_stops; ///< gradient stops
  wxAffineMatrix2D m_matrix;       ///< transformation matrix
};

#endif
