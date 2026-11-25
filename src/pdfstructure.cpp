///////////////////////////////////////////////////////////////////////////////
// Name:        pdfstructure.cpp
// Purpose:     Tagged PDF: logical structure tree and marked content
// Author:      wxPdfDocument contributors
// Created:     2026-11-23
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfstructure.cpp Implementation of tagged PDF output (structure tree, marked content)

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/zstream.h>

#include "wx/pdfdocument.h"
#include "wx/pdfstructure.h"
#include "wx/pdfutility.h"

#include "private_structure.h"

// --- wxPdfTag

wxPdfTag::wxPdfTag(const wxString& structureType)
{
  SetStructureType(structureType);
}

wxPdfTag&
wxPdfTag::SetStructureType(const wxString& structureType)
{
  m_type = structureType.IsEmpty() ? wxString(wxS("Span")) : structureType;
  return *this;
}

// --- Helpers

/// Format a string as a PDF name object (ISO 32000-1, 7.3.5), including the leading slash
static wxString
FormatPdfName(const wxString& name)
{
  wxString result = wxS("/");
  const wxCharBuffer utf8 = name.utf8_str();
  for (const char* p = utf8.data(); *p != '\0'; ++p)
  {
    unsigned char c = (unsigned char) *p;
    // Regular characters are written as is; whitespace, delimiters, '#' and
    // non-ASCII bytes must be written as #xx.
    if (c > 0x20 && c < 0x7f && strchr("#/()<>[]{}%", c) == NULL)
    {
      result += wxUniChar(c);
    }
    else
    {
      result += wxString::Format(wxS("#%02X"), c);
    }
  }
  return result;
}

// --- Tagging API of wxPdfDocument

void
wxPdfDocument::SetTagged(bool tagged)
{
  if (tagged && m_tagging == NULL)
  {
    m_tagging = new wxPdfTaggingState();
  }
  else if (!tagged && m_tagging != NULL)
  {
    TagCloseMarkedContent();
    delete m_tagging;
    m_tagging = NULL;
  }
}

void
wxPdfDocument::BeginTag(const wxPdfTag& tag)
{
  if (m_tagging == NULL)
  {
    return;
  }
  TagCloseMarkedContent();
  wxPdfStructElement* element = new wxPdfStructElement(tag, m_tagging->m_current);
  m_tagging->m_current->AddChild(element);
  m_tagging->m_current = element;
}

void
wxPdfDocument::BeginTag(const wxString& structureType, const wxString& altText, const wxString& actualText)
{
  BeginTag(wxPdfTag(structureType).SetAltText(altText).SetActualText(actualText));
}

void
wxPdfDocument::EndTag()
{
  if (m_tagging == NULL)
  {
    return;
  }
  if (m_tagging->m_current == m_tagging->m_root)
  {
    wxLogError(wxString(wxS("wxPdfDocument::EndTag: ")) +
               wxString(_("No structure element is open.")));
    return;
  }
  TagCloseMarkedContent();
  m_tagging->m_current = m_tagging->m_current->m_parent;
}

void
wxPdfDocument::BeginFormula(const wxString& altText, const wxString& mathML, const wxString& actualText)
{
  BeginTag(wxPdfTag(wxS("Formula")).SetAltText(altText).SetMathML(mathML).SetActualText(actualText));
}

int
wxPdfDocument::GetTagDepth() const
{
  return (m_tagging != NULL) ? m_tagging->GetDepth() : 0;
}

void
wxPdfDocument::BeginArtifact()
{
  if (m_tagging == NULL)
  {
    return;
  }
  TagCloseMarkedContent();
  m_tagging->m_artifactDepth++;
}

void
wxPdfDocument::EndArtifact()
{
  if (m_tagging == NULL)
  {
    return;
  }
  if (m_tagging->m_artifactDepth <= 0)
  {
    wxLogError(wxString(wxS("wxPdfDocument::EndArtifact: ")) +
               wxString(_("No artifact is open.")));
    return;
  }
  TagCloseMarkedContent();
  m_tagging->m_artifactDepth--;
}

// --- Marked content state machine

void
wxPdfDocument::TagPagination(bool begin)
{
  if (m_tagging == NULL)
  {
    return;
  }
  TagCloseMarkedContent();
  m_tagging->m_paginationDepth += (begin) ? 1 : -1;
}

void
wxPdfDocument::TagOpenMarkedContent()
{
  // Called from Out() right before page content is written while no
  // marked-content sequence is open. m_open is set *before* writing, so
  // that the Out() calls below don't recurse into this method.
  wxPdfTaggingState* tagging = m_tagging;
  if (tagging->m_paginationDepth > 0)
  {
    tagging->m_open = wxPdfTaggingState::MC_ARTIFACT;
    Out("/Artifact <</Type /Pagination>> BDC");
  }
  else if (tagging->m_artifactDepth > 0 || tagging->m_current == tagging->m_root)
  {
    // Explicit artifact, or content the application did not tag
    tagging->m_open = wxPdfTaggingState::MC_ARTIFACT;
    Out("/Artifact BMC");
  }
  else
  {
    tagging->m_open = wxPdfTaggingState::MC_CONTENT;
    int mcid = tagging->NewMcid(m_page);
    OutAscii(FormatPdfName(tagging->m_current->m_tag.GetStructureType()) +
             wxString::Format(wxS(" <</MCID %d>> BDC"), mcid));
  }
}

void
wxPdfDocument::TagSuspendMarkedContent()
{
  if (m_tagging != NULL)
  {
    TagCloseMarkedContent();
    m_tagging->m_open = wxPdfTaggingState::MC_SUSPENDED;
  }
}

void
wxPdfDocument::TagResumeMarkedContent()
{
  if (m_tagging != NULL && m_tagging->m_open == wxPdfTaggingState::MC_SUSPENDED)
  {
    m_tagging->m_open = wxPdfTaggingState::MC_NONE;
  }
}

void
wxPdfDocument::TagCloseMarkedContent()
{
  if (m_tagging != NULL &&
      m_tagging->m_open != wxPdfTaggingState::MC_NONE &&
      m_tagging->m_open != wxPdfTaggingState::MC_SUSPENDED)
  {
    // Only page content is enclosed in marked-content sequences (see Out())
    if (m_state == 2 && !m_inTemplate)
    {
      Out("EMC");
    }
    m_tagging->m_open = wxPdfTaggingState::MC_NONE;
  }
}

// --- Output of the structure tree

/*
 * Object numbers are assigned in exactly the order in which the objects are
 * written, because NewObj() derives the encryption key from the running
 * object counter m_n. The layout is:
 *
 *   StructTreeRoot
 *   structure elements, in pre-order (Document element first)
 *   ParentTree
 *   for each element with MathML (in pre-order): Filespec, EmbeddedFile
 */

/// Assign object numbers to an element and its descendants in pre-order
static void
AssignStructObjIds(wxPdfStructElement* element, int& nextId)
{
  element->m_objId = nextId++;
  for (size_t j = 0; j < element->m_kids.size(); ++j)
  {
    if (element->m_kids[j].m_element != NULL)
    {
      AssignStructObjIds(element->m_kids[j].m_element, nextId);
    }
  }
}

/// Assign object numbers to the MathML file specifications in pre-order
static void
AssignFileSpecObjIds(wxPdfStructElement* element, int& nextId)
{
  if (!element->m_tag.GetMathML().IsEmpty())
  {
    element->m_fileSpecObjId = nextId;
    nextId += 2; // file specification + embedded file stream
  }
  for (size_t j = 0; j < element->m_kids.size(); ++j)
  {
    if (element->m_kids[j].m_element != NULL)
    {
      AssignFileSpecObjIds(element->m_kids[j].m_element, nextId);
    }
  }
}

/// Collect the elements with MathML in pre-order
static void
CollectMathML(wxPdfStructElement* element, std::vector<wxPdfStructElement*>& list)
{
  if (!element->m_tag.GetMathML().IsEmpty())
  {
    list.push_back(element);
  }
  for (size_t j = 0; j < element->m_kids.size(); ++j)
  {
    if (element->m_kids[j].m_element != NULL)
    {
      CollectMathML(element->m_kids[j].m_element, list);
    }
  }
}

void
wxPdfDocument::PutStructElement(wxPdfStructElement* element, int parentObjId)
{
  NewObj();
  wxASSERT_MSG(m_n == element->m_objId, wxS("Structure element object number mismatch"));

  const wxPdfTag& tag = element->m_tag;
  Out("<</Type /StructElem /S ", false);
  OutAscii(FormatPdfName(tag.GetStructureType()), false);
  OutAscii(wxString::Format(wxS(" /P %d 0 R"), parentObjId));

  // The page of the element's own first marked-content sequence becomes the
  // default page (/Pg); sequences on other pages need a full MCR dictionary.
  int defaultPage = element->GetFirstPage();
  if (defaultPage > 0)
  {
    OutAscii(wxString::Format(wxS("/Pg %d 0 R"), m_firstPageId + 2 * (defaultPage - 1)));
  }

  if (!element->m_kids.empty())
  {
    Out("/K [", false);
    for (size_t j = 0; j < element->m_kids.size(); ++j)
    {
      const wxPdfStructElement::Kid& kid = element->m_kids[j];
      if (kid.m_element != NULL)
      {
        OutAscii(wxString::Format(wxS("%d 0 R "), kid.m_element->m_objId), false);
      }
      else if (kid.m_page == defaultPage)
      {
        OutAscii(wxString::Format(wxS("%d "), kid.m_mcid), false);
      }
      else
      {
        OutAscii(wxString::Format(wxS("<</Type /MCR /Pg %d 0 R /MCID %d>> "),
                                  m_firstPageId + 2 * (kid.m_page - 1), kid.m_mcid), false);
      }
    }
    Out("]");
  }

  if (!tag.GetTitle().IsEmpty())
  {
    Out("/T ", false);
    OutTextstring(tag.GetTitle());
  }
  if (!tag.GetLanguage().IsEmpty())
  {
    Out("/Lang ", false);
    OutTextstring(tag.GetLanguage());
  }
  if (!tag.GetAltText().IsEmpty())
  {
    Out("/Alt ", false);
    OutTextstring(tag.GetAltText());
  }
  if (!tag.GetExpansion().IsEmpty())
  {
    Out("/E ", false);
    OutTextstring(tag.GetExpansion());
  }
  if (!tag.GetActualText().IsEmpty())
  {
    Out("/ActualText ", false);
    OutTextstring(tag.GetActualText());
  }
  if (element->m_fileSpecObjId > 0)
  {
    OutAscii(wxString::Format(wxS("/AF [%d 0 R]"), element->m_fileSpecObjId));
  }
  Out(">>");
  Out("endobj");

  for (size_t j = 0; j < element->m_kids.size(); ++j)
  {
    if (element->m_kids[j].m_element != NULL)
    {
      PutStructElement(element->m_kids[j].m_element, element->m_objId);
    }
  }
}

void
wxPdfDocument::PutStructTree()
{
  if (m_tagging == NULL)
  {
    return;
  }

  // Assign object numbers (see comment above)
  int nextId = m_n + 1;
  m_structTreeRootObjId = nextId++;
  AssignStructObjIds(m_tagging->m_root, nextId);
  int parentTreeObjId = nextId++;
  AssignFileSpecObjIds(m_tagging->m_root, nextId);

  // Structure tree root
  NewObj();
  wxASSERT(m_n == m_structTreeRootObjId);
  Out("<</Type /StructTreeRoot");
  OutAscii(wxString::Format(wxS("/K [%d 0 R]"), m_tagging->m_root->m_objId));
  OutAscii(wxString::Format(wxS("/ParentTree %d 0 R"), parentTreeObjId));
  OutAscii(wxString::Format(wxS("/ParentTreeNextKey %d"), m_page));
  Out(">>");
  Out("endobj");

  // Structure elements
  PutStructElement(m_tagging->m_root, m_structTreeRootObjId);

  // Parent tree: maps the /StructParents key of each page (page number - 1)
  // to the array of structure elements owning its marked-content sequences
  // (indexed by MCID). The keys must be sorted, which std::map guarantees.
  NewObj();
  wxASSERT(m_n == parentTreeObjId);
  Out("<</Nums [");
  std::map<int, std::vector<wxPdfStructElement*> >::const_iterator page;
  for (page = m_tagging->m_parentTree.begin(); page != m_tagging->m_parentTree.end(); ++page)
  {
    OutAscii(wxString::Format(wxS("%d ["), page->first - 1), false);
    for (size_t j = 0; j < page->second.size(); ++j)
    {
      OutAscii(wxString::Format(wxS("%d 0 R "), page->second[j]->m_objId), false);
    }
    Out("]");
  }
  Out("]>>");
  Out("endobj");

  // MathML representations as associated files (PDF 2.0 / PDF/A-3 style, as
  // recommended by the PDF Association's best practice guide "Math in PDF")
  std::vector<wxPdfStructElement*> mathElements;
  CollectMathML(m_tagging->m_root, mathElements);
  for (size_t j = 0; j < mathElements.size(); ++j)
  {
    wxPdfStructElement* element = mathElements[j];
    wxString fileName = wxString::Format(wxS("mathml-%d.xml"), (int) (j + 1));

    NewObj();
    wxASSERT(m_n == element->m_fileSpecObjId);
    Out("<</Type /Filespec");
    Out("/F ", false);
    OutAsciiTextstring(fileName);
    Out("/UF ", false);
    OutTextstring(fileName);
    Out("/Desc ", false);
    OutTextstring(wxS("MathML representation"));
    Out("/AFRelationship /Supplement");
    OutAscii(wxString::Format(wxS("/EF <</F %d 0 R /UF %d 0 R>>"), m_n + 1, m_n + 1));
    Out(">>");
    Out("endobj");

    const wxCharBuffer utf8 = element->m_tag.GetMathML().utf8_str();
    size_t rawLength = strlen(utf8.data());
    wxMemoryOutputStream content;
    if (m_compress)
    {
      wxZlibOutputStream zout(content);
      zout.Write(utf8.data(), rawLength);
    }
    else
    {
      content.Write(utf8.data(), rawLength);
    }

    NewObj();
    Out("<</Type /EmbeddedFile /Subtype /application#2Fmathml+xml", false);
    if (m_compress)
    {
      Out(" /Filter /FlateDecode", false);
    }
    OutAscii(wxString::Format(wxS(" /Params <</Size %lu>> /Length %lu>>"),
                              (unsigned long) rawLength,
                              (unsigned long) CalculateStreamLength(content.TellO())));
    PutStream(content);
    Out("endobj");
  }
}
