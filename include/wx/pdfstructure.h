///////////////////////////////////////////////////////////////////////////////
// Name:        pdfstructure.h
// Purpose:     Description of logical structure elements for tagged PDF
// Author:      wxPdfDocument contributors
// Created:     2026-11-23
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfstructure.h Description of structure elements for tagged (accessible) PDF

#ifndef _PDF_STRUCTURE_H_
#define _PDF_STRUCTURE_H_

// wxWidgets headers
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

/// Description of a logical structure element ("tag") of a tagged PDF document
/**
* A tagged PDF document contains, in addition to its visual content, a
* <em>structure tree</em> that tells assistive technology (screen readers,
* braille displays, reflowing viewers) and text extraction tools what the
* content means: which drawing commands form a paragraph, a heading, a
* figure or a mathematical formula, how they should be read aloud, and in
* which order.
*
* An instance of this class describes one structure element. It is passed to
* wxPdfDocument::BeginTag(); every drawing command issued until the matching
* wxPdfDocument::EndTag() becomes the content of that element.
*
* The structure type is one of the standard structure types of ISO 32000-1,
* section 14.8.4, for example
* - grouping elements: \c Document, \c Part, \c Art, \c Sect, \c Div,
*   \c BlockQuote, \c Caption, \c TOC, \c TOCI, \c Index
* - block level elements: \c P, \c H, \c H1 ... \c H6, \c L, \c LI, \c Lbl,
*   \c LBody, \c Table, \c TR, \c TH, \c TD
* - inline elements: \c Span, \c Quote, \c Note, \c Reference, \c Code,
*   \c Link
* - illustrations: \c Figure, \c Formula, \c Form
*
* Optional properties:
* - <b>alternate text</b> (\c /Alt): a human readable description of the
*   content, read by screen readers <em>instead of</em> the content. Required
*   for \c Figure and \c Formula elements by PDF/UA.
* - <b>actual text</b> (\c /ActualText): the exact text the content
*   represents. Text extraction and copy &amp; paste use it instead of the
*   glyphs that were drawn. Useful when text is drawn with lines, with
*   decorative glyphs or as a sequence of glyphs that does not map to the
*   intended characters (e.g. a big square root sign assembled from parts).
* - <b>expansion</b> (\c /E): the expanded form of an abbreviation.
* - <b>language</b> (\c /Lang): the natural language of the content as a
*   BCP 47 language tag (e.g. \c "en-US", \c "de"), if it differs from the
*   document language.
* - <b>title</b> (\c /T): a title of the element (not read as content).
* - <b>MathML</b>: a MathML representation of a formula. It is embedded in
*   the PDF as an <em>associated file</em> of the structure element
*   (\c /AF, relationship \c /Supplement, MIME type
*   \c application/mathml+xml), as recommended by the PDF Association's
*   best practice guide "Math in PDF" and as done by the LaTeX tagging
*   project. Software that understands MathML (e.g. NVDA with MathCAT)
*   can then present the formula semantically; all other software falls
*   back to the alternate text.
*
* All setters return a reference to the object, so that calls can be
* chained:
* \code
* pdf.BeginTag(wxPdfTag(wxS("Formula"))
*                .SetAltText(wxS("x squared plus one"))
*                .SetMathML(wxS("<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
*                               "<msup><mi>x</mi><mn>2</mn></msup><mo>+</mo><mn>1</mn></math>")));
* // ... draw the formula ...
* pdf.EndTag();
* \endcode
*
* \see wxPdfDocument::SetTagged(), wxPdfDocument::BeginTag(), wxPdfDocument::EndTag()
*/
class WXDLLIMPEXP_PDFDOC wxPdfTag
{
public:
  /// Constructor
  /**
  * \param structureType the structure type of the element, e.g. \c "P" or \c "Formula".
  * An empty string is replaced by \c "Span".
  */
  explicit wxPdfTag(const wxString& structureType = wxS("Span"));

  /// Set the structure type
  wxPdfTag& SetStructureType(const wxString& structureType);

  /// Get the structure type
  const wxString& GetStructureType() const { return m_type; }

  /// Set the alternate description (/Alt)
  wxPdfTag& SetAltText(const wxString& altText) { m_altText = altText; return *this; }

  /// Get the alternate description
  const wxString& GetAltText() const { return m_altText; }

  /// Set the replacement text (/ActualText)
  wxPdfTag& SetActualText(const wxString& actualText) { m_actualText = actualText; return *this; }

  /// Get the replacement text
  const wxString& GetActualText() const { return m_actualText; }

  /// Set the expanded form of an abbreviation (/E)
  wxPdfTag& SetExpansion(const wxString& expansion) { m_expansion = expansion; return *this; }

  /// Get the expanded form of an abbreviation
  const wxString& GetExpansion() const { return m_expansion; }

  /// Set the natural language of the element content (/Lang), e.g. "en-US"
  wxPdfTag& SetLanguage(const wxString& language) { m_language = language; return *this; }

  /// Get the natural language of the element content
  const wxString& GetLanguage() const { return m_language; }

  /// Set the title of the element (/T)
  wxPdfTag& SetTitle(const wxString& title) { m_title = title; return *this; }

  /// Get the title of the element
  const wxString& GetTitle() const { return m_title; }

  /// Set a MathML representation of the element content
  /**
  * The MathML markup (a complete \c &lt;math&gt; element, preferably with the
  * MathML namespace declaration) is embedded as an associated file of the
  * structure element. It is normally used with \c Formula elements.
  * \param mathML the MathML markup. It is stored UTF-8 encoded.
  */
  wxPdfTag& SetMathML(const wxString& mathML) { m_mathML = mathML; return *this; }

  /// Get the MathML representation of the element content
  const wxString& GetMathML() const { return m_mathML; }

private:
  wxString m_type;        ///< structure type
  wxString m_altText;     ///< alternate description
  wxString m_actualText;  ///< replacement text
  wxString m_expansion;   ///< expansion of an abbreviation
  wxString m_language;    ///< natural language
  wxString m_title;       ///< title
  wxString m_mathML;      ///< MathML representation
};

#endif
