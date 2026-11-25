///////////////////////////////////////////////////////////////////////////////
// Name:        private_structure.h
// Purpose:     Internal bookkeeping for tagged PDF (structure tree)
// Author:      wxPdfDocument contributors
// Created:     2026-11-23
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file private_structure.h Internal classes for tagged PDF; not part of the public API

#ifndef _PDF_PRIVATE_STRUCTURE_H_
#define _PDF_PRIVATE_STRUCTURE_H_

#include <map>
#include <vector>

#include "wx/pdfstructure.h"

/// One node of the structure tree (internal)
class wxPdfStructElement
{
public:
  /// A kid of a structure element: either a child element or a marked-content sequence
  struct Kid
  {
    wxPdfStructElement* m_element; ///< child element, or NULL for a marked-content reference
    int                 m_page;    ///< page number (1-based) of the marked-content sequence
    int                 m_mcid;    ///< marked-content identifier on that page
  };

  wxPdfStructElement(const wxPdfTag& tag, wxPdfStructElement* parent)
    : m_tag(tag), m_parent(parent), m_objId(0), m_fileSpecObjId(0)
  {
  }

  ~wxPdfStructElement()
  {
    for (size_t j = 0; j < m_kids.size(); ++j)
    {
      delete m_kids[j].m_element;
    }
  }

  /// Append a child element (takes ownership)
  void AddChild(wxPdfStructElement* child)
  {
    Kid kid = { child, 0, 0 };
    m_kids.push_back(kid);
  }

  /// Append a marked-content sequence
  void AddMarkedContent(int page, int mcid)
  {
    Kid kid = { NULL, page, mcid };
    m_kids.push_back(kid);
  }

  /// Page number of the first marked-content sequence of this element itself, 0 if none
  int GetFirstPage() const
  {
    for (size_t j = 0; j < m_kids.size(); ++j)
    {
      if (m_kids[j].m_element == NULL)
      {
        return m_kids[j].m_page;
      }
    }
    return 0;
  }

  wxPdfTag            m_tag;            ///< properties of the element
  wxPdfStructElement* m_parent;         ///< parent element, NULL for the root
  std::vector<Kid>    m_kids;           ///< kids in logical (reading) order
  int                 m_objId;          ///< PDF object number, assigned when writing
  int                 m_fileSpecObjId;  ///< object number of the MathML file specification, 0 if none

private:
  wxPdfStructElement(const wxPdfStructElement&);
  wxPdfStructElement& operator=(const wxPdfStructElement&);
};

/// State of the tagging machinery of one document (internal)
/**
* Marked content is managed as a small state machine. Page content is always
* written inside exactly one marked-content sequence created by this class:
* either the content of the innermost open structure element
* (<tt>/P &lt;&lt;/MCID n&gt;&gt; BDC ... EMC</tt>) or an artifact
* (<tt>/Artifact BMC ... EMC</tt>) for content that the application did not
* tag, explicit artifacts and page headers/footers.
*
* Sequences are opened lazily, i.e. only when page content is actually
* written (see wxPdfDocument::Out()), so that no empty sequences are created.
* They are closed at every structural boundary: BeginTag(), EndTag(),
* BeginArtifact(), EndArtifact(), EnterLayer(), LeaveLayer() and at the end of
* each page. Closing at every boundary guarantees that our sequences nest
* properly with other marked-content operators (optional content layers)
* and that a structure element spanning several pages gets one sequence per
* page.
*/
class wxPdfTaggingState
{
public:
  /// Kind of the marked-content sequence currently open on the page
  enum MarkedContent
  {
    MC_NONE,      ///< nothing open; the next content opens a sequence
    MC_CONTENT,   ///< content of a structure element
    MC_ARTIFACT,  ///< artifact
    MC_SUSPENDED  ///< no sequence open, and none must be opened (end of page)
  };

  wxPdfTaggingState()
    : m_current(NULL), m_open(MC_NONE), m_artifactDepth(0), m_paginationDepth(0)
  {
    m_root = new wxPdfStructElement(wxPdfTag(wxS("Document")), NULL);
    m_current = m_root;
  }

  ~wxPdfTaggingState()
  {
    delete m_root;
  }

  /// Number of open structure elements (excluding the implicit Document root)
  int GetDepth() const
  {
    int depth = 0;
    for (const wxPdfStructElement* e = m_current; e != m_root; e = e->m_parent)
    {
      ++depth;
    }
    return depth;
  }

  /// Register a new marked-content sequence for the current element on a page; returns its MCID
  int NewMcid(int page)
  {
    std::vector<wxPdfStructElement*>& seqs = m_parentTree[page];
    int mcid = (int) seqs.size();
    seqs.push_back(m_current);
    m_current->AddMarkedContent(page, mcid);
    return mcid;
  }

  wxPdfStructElement* m_root;            ///< implicit Document element
  wxPdfStructElement* m_current;         ///< innermost open element
  MarkedContent       m_open;            ///< currently open marked-content sequence
  int                 m_artifactDepth;   ///< nesting depth of BeginArtifact()
  int                 m_paginationDepth; ///< > 0 while page header or footer is written

  /// For each page (1-based) the structure element of each MCID (index = MCID)
  std::map<int, std::vector<wxPdfStructElement*> > m_parentTree;

private:
  wxPdfTaggingState(const wxPdfTaggingState&);
  wxPdfTaggingState& operator=(const wxPdfTaggingState&);
};

#endif
