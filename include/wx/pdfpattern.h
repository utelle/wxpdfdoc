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
  * \param drawColour The foreground colour to be used for hatching
  * \param fillColour The background colour to be used to fill the pattern background
  */
  wxPdfPattern(int index, double width, double height, wxPdfPatternStyle patternStyle, const wxColour& drawColour, const wxColour& fillColour = wxColour());

  /// Copy constructor
  wxPdfPattern(const wxPdfPattern& pattern);

  /// Set object index
  void SetObjIndex(int index) { m_objIndex = index; };

  /// Get object index
  int GetObjIndex() const { return m_objIndex; };

  /// Get pattern index
  int GetIndex() const { return m_index; };

  /// Set image
  void SetImage(wxPdfImage* image) { m_image = image; };

  /// Get image
  wxPdfImage* GetImage() const {return m_image; };

  /// Get pattern width
  double GetWidth() const {return m_width; };

  /// Get pattern height
  double GetHeight() const {return m_height; };

  /// Get template id
  int GetTemplateId() const { return m_templateId; }

  ///  Get pattern style
  wxPdfPatternStyle GetPatternStyle() const { return m_patternStyle; }

  ///  Get draw color
  wxColour GetDrawColour() const { return m_drawColour; }

  ///  Set fill color
  void SetFillColour(const wxColour& fillColour) { m_fillColour = fillColour; m_hasFillColour = true; }
 
  ///  Get fill color
  wxColour GetFillColour() const { return m_fillColour; }

  /// Check whether fill color is set
  bool HasFillColour() const { return m_hasFillColour; }

private:
  int    m_objIndex;   ///< object index
  int    m_index;      ///< pattern index

  wxPdfPatternStyle m_patternStyle; ///< pattern style
  wxPdfImage* m_image; ///< image
  int      m_templateId; ///< template id
  wxColour m_drawColour; ///< foregorund colour
  wxColour m_fillColour; ///< background colour
  bool     m_hasFillColour; ///< flag whether background colour is defined for the pattern

  double m_width;      ///< pattern width
  double m_height;     ///< pattern height

  double m_xStep;      ///< repeat offset in x direction
  double m_yStep;      ///< repeat offset in y direction
  double m_matrix[6];  ///< transformation matrix
};

#endif
