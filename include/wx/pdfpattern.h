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
  * \param patternStyle The hatch style pattern to be used
  * \param drawColour The foreground colour to be used for hatching
  * \param fillColour The background colour to be used to fill the pattern background (optional)
  */
  wxPdfPattern(int index, double width, double height, wxPdfPatternStyle patternStyle, const wxColour& drawColour, const wxColour& fillColour = wxColour());

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
  * \return TRUE if the pattern has a fill colour, FALSE otherwise
  */
  bool HasFillColour() const { return m_hasFillColour; }

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
};

#endif
