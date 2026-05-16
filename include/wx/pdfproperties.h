/*
** Name:        pdfproperties.h
** Purpose:     Defines and enumerations for various PDF properties
** Author:      Ulrich Telle
** Created:     2006-07-13
** Copyright:   (c) 2006-2026 Ulrich Telle
** Licence:     wxWindows licence
** SPDX-License-Identifier: LGPL-3.0+ WITH WxWindows-exception-3.1
*/

/// \file pdfproperties.h Interface of the several wxPdfDocument property classes

#ifndef _PDF_PROPERTIES_H_
#define _PDF_PROPERTIES_H_

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

/// Border options
#define wxPDF_BORDER_NONE    0x0000 ///< No border
#define wxPDF_BORDER_LEFT    0x0001 ///< Left border
#define wxPDF_BORDER_RIGHT   0x0002 ///< Right border
#define wxPDF_BORDER_TOP     0x0004 ///< Top border
#define wxPDF_BORDER_BOTTOM  0x0008 ///< Bottom border
#define wxPDF_BORDER_FRAME   0x000F ///< Full frame border

/// Corner options
#define wxPDF_CORNER_NONE          0x0000 ///< No rounded corners
#define wxPDF_CORNER_TOP_LEFT      0x0001 ///< Top-left rounded corner
#define wxPDF_CORNER_TOP_RIGHT     0x0002 ///< Top-right rounded corner
#define wxPDF_CORNER_BOTTOM_LEFT   0x0004 ///< Bottom-left rounded corner
#define wxPDF_CORNER_BOTTOM_RIGHT  0x0008 ///< Bottom-right rounded corner
#define wxPDF_CORNER_ALL           0x000F ///< All corners rounded

/// Style options
#define wxPDF_STYLE_NOOP      0x0000 ///< No operation
#define wxPDF_STYLE_DRAW      0x0001 ///< Draw (stroke) the path
#define wxPDF_STYLE_FILL      0x0002 ///< Fill the path
#define wxPDF_STYLE_FILLDRAW  0x0003 ///< Fill and draw (stroke) the path
#define wxPDF_STYLE_DRAWCLOSE 0x0004 ///< Close and draw the path
#define wxPDF_STYLE_MASK      0x0007 ///< Style mask

/// Text render mode
enum wxPdfTextRenderMode
{
  wxPDF_TEXT_RENDER_FILL       = 0, ///< Fill text
  wxPDF_TEXT_RENDER_STROKE     = 1, ///< Stroke text
  wxPDF_TEXT_RENDER_FILLSTROKE = 2, ///< Fill and stroke text
  wxPDF_TEXT_RENDER_INVISIBLE  = 3  ///< Invisible text
};

/// Font style flags
enum
{
  wxPDF_FONTSTYLE_REGULAR    = 0,      ///< Regular font style
  wxPDF_FONTSTYLE_ITALIC     = 1 << 0, ///< Italic font style
  wxPDF_FONTSTYLE_BOLD       = 1 << 1, ///< Bold font style
  wxPDF_FONTSTYLE_BOLDITALIC = wxPDF_FONTSTYLE_ITALIC |
                               wxPDF_FONTSTYLE_BOLD, ///< Bold-italic font style
  wxPDF_FONTSTYLE_UNDERLINE  = 1 << 2, ///< Underlined text
  wxPDF_FONTSTYLE_OVERLINE   = 1 << 3, ///< Overlined text
  wxPDF_FONTSTYLE_STRIKEOUT  = 1 << 4, ///< Strikethrough text

  /// Granular weights (Thin to ExtraHeavy) are fully supported when using wxFont starting with wxWidgets 3.1.2.
  /// In older versions, they fall back to the closest standard weight (Bold or Regular).
  wxPDF_FONTSTYLE_THIN       = 1 << 5,  ///< Thin font weight
  wxPDF_FONTSTYLE_EXTRALIGHT = 1 << 6,  ///< Extra light font weight
  wxPDF_FONTSTYLE_LIGHT      = 1 << 7,  ///< Light font weight
  wxPDF_FONTSTYLE_MEDIUM     = 1 << 8,  ///< Medium font weight
  wxPDF_FONTSTYLE_SEMIBOLD   = 1 << 9,  ///< Semi-bold font weight
  wxPDF_FONTSTYLE_EXTRABOLD  = 1 << 10, ///< Extra bold font weight
  wxPDF_FONTSTYLE_HEAVY      = 1 << 11, ///< Heavy font weight
  wxPDF_FONTSTYLE_EXTRAHEAVY = 1 << 12, ///< Extra heavy font weight

  wxPDF_FONTSTYLE_DECORATION_MASK = wxPDF_FONTSTYLE_UNDERLINE |
                                    wxPDF_FONTSTYLE_OVERLINE  |
                                    wxPDF_FONTSTYLE_STRIKEOUT,  ///< Mask of decoration styles
  wxPDF_FONTSTYLE_WEIGHT_MASK     = wxPDF_FONTSTYLE_BOLD       |
                                    wxPDF_FONTSTYLE_THIN       |
                                    wxPDF_FONTSTYLE_EXTRALIGHT |
                                    wxPDF_FONTSTYLE_LIGHT      |
                                    wxPDF_FONTSTYLE_MEDIUM     |
                                    wxPDF_FONTSTYLE_SEMIBOLD   |
                                    wxPDF_FONTSTYLE_EXTRABOLD  |
                                    wxPDF_FONTSTYLE_HEAVY      |
                                    wxPDF_FONTSTYLE_EXTRAHEAVY, ///< Mask of weight styles
  wxPDF_FONTSTYLE_MASK = wxPDF_FONTSTYLE_REGULAR   |
                         wxPDF_FONTSTYLE_ITALIC    |
                         wxPDF_FONTSTYLE_BOLD      |
                         wxPDF_FONTSTYLE_UNDERLINE |
                         wxPDF_FONTSTYLE_OVERLINE  |
                         wxPDF_FONTSTYLE_STRIKEOUT |
                         wxPDF_FONTSTYLE_THIN       |
                         wxPDF_FONTSTYLE_EXTRALIGHT |
                         wxPDF_FONTSTYLE_LIGHT      |
                         wxPDF_FONTSTYLE_MEDIUM     |
                         wxPDF_FONTSTYLE_SEMIBOLD   |
                         wxPDF_FONTSTYLE_EXTRABOLD  |
                         wxPDF_FONTSTYLE_HEAVY      |
                         wxPDF_FONTSTYLE_EXTRAHEAVY ///< Full font style mask
};

/// Permission options
#define wxPDF_PERMISSION_NONE     0x0000  ///< Allow nothing
#define wxPDF_PERMISSION_PRINT    0x0004  ///< Allow printing
#define wxPDF_PERMISSION_MODIFY   0x0008  ///< Allow modifying
#define wxPDF_PERMISSION_COPY     0x0010  ///< Allow text copying
#define wxPDF_PERMISSION_ANNOT    0x0020  ///< Allow annotations
#define wxPDF_PERMISSION_FILLFORM 0x0100  ///< Allow filling forms
#define wxPDF_PERMISSION_EXTRACT  0x0200  ///< Allow extract text and/or graphics
#define wxPDF_PERMISSION_ASSEMBLE 0x0400  ///< Allow assemble document
#define wxPDF_PERMISSION_HLPRINT  0x0800  ///< Allow high resolution print
#define wxPDF_PERMISSION_ALL      0x0F3C  ///< Allow anything

/// Encryption methods
enum wxPdfEncryptionMethod
{
  wxPDF_ENCRYPTION_RC4V1,   ///< RC4 40-bit encryption
  wxPDF_ENCRYPTION_RC4V2,   ///< RC4 128-bit encryption
  wxPDF_ENCRYPTION_AESV2,   ///< AES 128-bit encryption
  wxPDF_ENCRYPTION_AESV3,   ///< AES 256-bit encryption (PDF 1.7 Extension 3)
  wxPDF_ENCRYPTION_AESV3R6  ///< AES 256-bit encryption (PDF 2.0)
};

/// Page box types
enum wxPdfPageBox
{
  wxPDF_PAGEBOX_MEDIABOX, ///< Boundaries of the physical medium
  wxPDF_PAGEBOX_CROPBOX,  ///< Visible region of default user space
  wxPDF_PAGEBOX_BLEEDBOX, ///< Region to which the contents of the page should be clipped when output in a production environment
  wxPDF_PAGEBOX_TRIMBOX,  ///< Intended dimensions of the finished page after trimming
  wxPDF_PAGEBOX_ARTBOX    ///< Extent of the page's meaningful content
};

/// Form field border styles
enum wxPdfBorderStyle
{
  wxPDF_BORDER_SOLID,     ///< Solid border
  wxPDF_BORDER_DASHED,    ///< Dashed border
  wxPDF_BORDER_BEVELED,   ///< Beveled border (3D look)
  wxPDF_BORDER_INSET,     ///< Inset border (3D look)
  wxPDF_BORDER_UNDERLINE  ///< Underline border
};

/// Alignment options
enum wxPdfAlignment
{
  wxPDF_ALIGN_LEFT,    ///< Left alignment (or top alignment for vertical)
  wxPDF_ALIGN_CENTER,  ///< Center alignment (or middle alignment for vertical)
  wxPDF_ALIGN_RIGHT,   ///< Right alignment (or bottom alignment for vertical)
  wxPDF_ALIGN_JUSTIFY, ///< Justified alignment
  wxPDF_ALIGN_TOP    = wxPDF_ALIGN_LEFT,   ///< Top alignment (alias for left)
  wxPDF_ALIGN_MIDDLE = wxPDF_ALIGN_CENTER, ///< Middle alignment (alias for center)
  wxPDF_ALIGN_BOTTOM = wxPDF_ALIGN_RIGHT  ///< Bottom alignment (alias for right)
};

/// Zoom options
enum wxPdfZoom
{
  wxPDF_ZOOM_FULLPAGE,  ///< Display the entire page
  wxPDF_ZOOM_FULLWIDTH, ///< Display the full width of the page
  wxPDF_ZOOM_REAL,      ///< Display at real size (100%)
  wxPDF_ZOOM_DEFAULT,   ///< Use the default viewer zoom
  wxPDF_ZOOM_FACTOR     ///< Use a specific zoom factor
};

/// Layout options
enum wxPdfLayout
{
  wxPDF_LAYOUT_CONTINUOUS, ///< Continuous display
  wxPDF_LAYOUT_SINGLE,     ///< Single page display
  wxPDF_LAYOUT_TWO,        ///< Two-column display
  wxPDF_LAYOUT_DEFAULT     ///< Use the default viewer layout
};

/// Viewer preferences
#define wxPDF_VIEWER_HIDETOOLBAR     0x0001 ///< Hide tool bar
#define wxPDF_VIEWER_HIDEMENUBAR     0x0002 ///< Hide menu bar
#define wxPDF_VIEWER_HIDEWINDOWUI    0x0004 ///< Hide window UI
#define wxPDF_VIEWER_FITWINDOW       0x0008 ///< Fit window
#define wxPDF_VIEWER_CENTERWINDOW    0x0010 ///< Center window
#define wxPDF_VIEWER_DISPLAYDOCTITLE 0x0020 ///< Display document title
#define wxPDF_VIEWER_NOPRINTSCALING  0x0040 ///< No print scaling

/// Paper handling options
enum wxPdfPaperHandling
{
  wxPDF_PAPERHANDLING_DEFAULT,                 ///< Default paper handling
  wxPDF_PAPERHANDLING_SIMPLEX,                 ///< Simplex printing
  wxPDF_PAPERHANDLING_DUPLEX_FLIP_SHORT_EDGE,  ///< Duplex printing, flip on short edge
  wxPDF_PAPERHANDLING_DUPLEX_FLIP_LONG_EDGE    ///< Duplex printing, flip on long edge
};

/// Marker symbols
enum wxPdfMarker
{
  wxPDF_MARKER_CIRCLE,            ///< Circle marker
  wxPDF_MARKER_SQUARE,            ///< Square marker
  wxPDF_MARKER_TRIANGLE_UP,       ///< Upward triangle marker
  wxPDF_MARKER_TRIANGLE_DOWN,     ///< Downward triangle marker
  wxPDF_MARKER_TRIANGLE_LEFT,     ///< Leftward triangle marker
  wxPDF_MARKER_TRIANGLE_RIGHT,    ///< Rightward triangle marker
  wxPDF_MARKER_DIAMOND,           ///< Diamond marker
  wxPDF_MARKER_PENTAGON_UP,       ///< Upward pentagon marker
  wxPDF_MARKER_PENTAGON_DOWN,     ///< Downward pentagon marker
  wxPDF_MARKER_PENTAGON_LEFT,     ///< Leftward pentagon marker
  wxPDF_MARKER_PENTAGON_RIGHT,    ///< Rightward pentagon marker
  wxPDF_MARKER_STAR,              ///< 5-pointed star marker
  wxPDF_MARKER_STAR4,             ///< 4-pointed star marker
  wxPDF_MARKER_PLUS,              ///< Plus sign marker
  wxPDF_MARKER_CROSS,             ///< Cross marker (X)
  wxPDF_MARKER_SUN,               ///< Sun marker
  wxPDF_MARKER_BOWTIE_HORIZONTAL, ///< Horizontal bowtie marker
  wxPDF_MARKER_BOWTIE_VERTICAL,   ///< Vertical bowtie marker
  wxPDF_MARKER_ASTERISK,          ///< Asterisk marker
  wxPDF_MARKER_LAST               ///< Marks the last available marker symbol; do not use!
};

/// Pattern styles
enum wxPdfPatternStyle
{
  wxPDF_PATTERNSTYLE_NONE,     ///< No pattern
  wxPDF_PATTERNSTYLE_IMAGE,    ///< Image pattern
  wxPDF_PATTERNSTYLE_TEMPLATE, ///< Template pattern
  wxPDF_PATTERNSTYLE_LINEAR_GRADIENT, ///< Linear gradient pattern
  wxPDF_PATTERNSTYLE_RADIAL_GRADIENT, ///< Radial gradient pattern

  // Hatch styles
  wxPDF_PATTERNSTYLE_FIRST_HATCH,                                     ///< First available hatch style
  wxPDF_PATTERNSTYLE_BDIAGONAL_HATCH = wxPDF_PATTERNSTYLE_FIRST_HATCH, ///< Backward diagonal hatch
  wxPDF_PATTERNSTYLE_CROSSDIAG_HATCH,                                 ///< Cross-diagonal hatch
  wxPDF_PATTERNSTYLE_FDIAGONAL_HATCH,                                 ///< Forward diagonal hatch
  wxPDF_PATTERNSTYLE_CROSS_HATCH,                                     ///< Cross hatch
  wxPDF_PATTERNSTYLE_HORIZONTAL_HATCH,                                ///< Horizontal hatch
  wxPDF_PATTERNSTYLE_VERTICAL_HATCH,                                  ///< Vertical hatch
  wxPDF_PATTERNSTYLE_HERRINGBONE_HATCH,                               ///< Herringbone hatch
  wxPDF_PATTERNSTYLE_BASKETWEAVE_HATCH,                               ///< Basketweave hatch
  wxPDF_PATTERNSTYLE_BRICK_HATCH,                                     ///< Brick hatch
  wxPDF_PATTERNSTYLE_LAST_HATCH = wxPDF_PATTERNSTYLE_BRICK_HATCH      ///< Last available hatch style
};

/// Linear gradient types
enum wxPdfLinearGradientType
{
  wxPDF_LINEAR_GRADIENT_HORIZONTAL,       ///< Horizontal linear gradient
  wxPDF_LINEAR_GRADIENT_VERTICAL,         ///< Vertical linear gradient
  wxPDF_LINEAR_GRADIENT_MIDHORIZONTAL,    ///< Mid-horizontal linear gradient
  wxPDF_LINEAR_GRADIENT_MIDVERTICAL,      ///< Mid-vertical linear gradient
  wxPDF_LINEAR_GRADIENT_REFLECTION_LEFT,  ///< Reflection-left linear gradient
  wxPDF_LINEAR_GRADIENT_REFLECTION_RIGHT, ///< Reflection-right linear gradient
  wxPDF_LINEAR_GRADIENT_REFLECTION_TOP,   ///< Reflection-top linear gradient
  wxPDF_LINEAR_GRADIENT_REFLECTION_BOTTOM ///< Reflection-bottom linear gradient
};

/// Blend modes
enum wxPdfBlendMode
{
  wxPDF_BLENDMODE_NORMAL,     ///< Normal: Selects the source color, ignoring the backdrop
  wxPDF_BLENDMODE_MULTIPLY,   ///< Multiply: Multiplies backdrop and source colors; result is always darker
  wxPDF_BLENDMODE_SCREEN,     ///< Screen: Multiplies complements of colors; result is always lighter
  wxPDF_BLENDMODE_OVERLAY,    ///< Overlay: Multiplies or screens depending on backdrop; preserves highlights and shadows
  wxPDF_BLENDMODE_DARKEN,     ///< Darken: Selects the darker of the backdrop and source colors
  wxPDF_BLENDMODE_LIGHTEN,    ///< Lighten: Selects the lighter of the backdrop and source colors
  wxPDF_BLENDMODE_COLORDODGE, ///< ColorDodge: Brightens the backdrop color to reflect the source color
  wxPDF_BLENDMODE_COLORBURN,  ///< ColorBurn: Darkens the backdrop color to reflect the source color
  wxPDF_BLENDMODE_HARDLIGHT,  ///< HardLight: Multiplies or screens depending on source color; adds strong highlights/shadows
  wxPDF_BLENDMODE_SOFTLIGHT,  ///< SoftLight: Darkens or lightens depending on source color; similar to a diffused spotlight
  wxPDF_BLENDMODE_DIFFERENCE, ///< Difference: Subtracts the darker color from the lighter color
  wxPDF_BLENDMODE_EXCLUSION,  ///< Exclusion: Similar to Difference but with lower contrast
  wxPDF_BLENDMODE_HUE,        ///< Hue: Source hue with backdrop saturation and luminosity
  wxPDF_BLENDMODE_SATURATION, ///< Saturation: Source saturation with backdrop hue and luminosity
  wxPDF_BLENDMODE_COLOR,      ///< Color: Source hue and saturation with backdrop luminosity; preserves gray levels
  wxPDF_BLENDMODE_LUMINOSITY  ///< Luminosity: Source luminosity with backdrop hue and saturation
};

/// Shaped text modes
enum wxPdfShapedTextMode
{
  wxPDF_SHAPEDTEXTMODE_ONETIME,      ///< Apply shaping one time
  wxPDF_SHAPEDTEXTMODE_STRETCHTOFIT, ///< Stretch text to fit
  wxPDF_SHAPEDTEXTMODE_REPEAT        ///< Repeat shaped text
};

/// PDF/X and PDF/A conformance types
/**
* For more information on PDF standards, see the official Adobe documentation:
* \see https://helpx.adobe.com/acrobat/using/pdf-x-pdf-a-pdf.html
* For a technical and historical overview, see Wikipedia:
* \see https://en.wikipedia.org/wiki/PDF#PDF_standards
*/
enum wxPdfXConformanceType
{
  wxPDF_PDFXNONE,     ///< No PDF/X or PDF/A conformance
  wxPDF_PDFX1A2001,   ///< PDF/X-1a:2001: CMYK-only printing standard; no transparency allowed
  wxPDF_PDFX32002,    ///< PDF/X-3:2002: Color-managed printing standard; allows RGB and Lab colors
  wxPDF_PDFA1A,       ///< PDF/A-1a: Archiving standard with full accessibility and structural tags
  wxPDF_PDFA1B        ///< PDF/A-1b: Archiving standard for visual preservation only
};

/// Run direction of text
enum wxPdfRunDirection
{
  wxPDF_RUN_DIRECTION_DEFAULT, ///< Default run direction
  wxPDF_RUN_DIRECTION_NO_BIDI, ///< Do not use bidirectional reordering
  wxPDF_RUN_DIRECTION_LTR,     ///< Bidirectional reordering with left-to-right preferential run direction
  wxPDF_RUN_DIRECTION_RTL      ///< Bidirectional reordering with right-to-left preferential run direction
};

#endif
