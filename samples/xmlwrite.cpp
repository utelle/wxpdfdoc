///////////////////////////////////////////////////////////////////////////////
// Name:        xmlwrite.cpp
// Purpose:     Demonstration of xml formatted text in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-01-21
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/pdfdoc.h"

/**
* XML write
*
* This example demonstrates the use of "rich text" cells,
* i.e. cells containing a subset of HTML markup.
*/

void xmlwrite()
{
  wxString xmlString1 = wxString(wxT("<p align=\"justify\">This example shows text <b>styling</b> with <i>simple</i> XML. ")) +
                        wxString(wxT("H<sub>2</sub>SO<sub>4</sub> is an acid. Nesting of super- or subscripting is possible: x<sup>i<sup>2</sup></sup> + y<sub>k<sub>3</sub></sub>. ")) +
                        wxString(wxT("Now follows a <i>rather</i> long text, showing whether the justification algorithm <b>really</b> works. At least one <b>additional</b> line should be printed, further <b><font color=\"red\">demonstrating</font></b> the algorithm.")) +
                        wxString(wxT("<br/><u>Underlined <b>and </b> <s>striked through</s></u> or <o>overlined</o></p><img src=\"wxpdfdoc.png\" width=\"160\" height=\"120\" align=\"center\"/>"));
  wxString xmlString2 = wxString(wxT("<h1>Header 1 (left)</h1><h2 align=\"right\">Header 2 (right)</h2><h3 align=\"center\">Header 3 (centered)</h3><h4 align=\"justify\">Header 4 (justified)</h4><h5>Header 5</h5><h6>Header 6</h6>"));
  wxString xmlString3 = wxString(wxT("Let's start an enumeration with roman numerals at offset 100:<ol type=\"i\" start=\"100\"><li>Anton</li><li>Berta</li><li>Caesar</li></ol>Who would be next?")) +
                        wxString(wxT("<p align=\"right\">This section should be right aligned.<br/>Do you want to go to the <a href=\"http://www.wxwidgets.org\">wxWidgets website</a>?</p>"));
  wxString xmlString4 = wxString(wxT("<h1>Nested tables example</h1><br/><table border=\"1\"><colgroup><col width=\"40\" span=\"4\" /></colgroup><tbody><tr height=\"12\"><td bgcolor=\"#cccccc\">Cell 1,1</td><td colspan=\"2\" align=\"center\" valign=\"middle\">Cell 1,2-1,3</td><td>Cell 1,4</td></tr><tr><td>Cell 2,1</td><td>Cell 2,2</td><td>Cell 2,3</td><td>Cell 2,4</td></tr><tr><td>Cell 3,1</td><td>Cell 3,2</td><td colspan=\"2\" rowspan=\"2\" align=\"center\">")) +
                        wxString(wxT("<table border=\"1\"><colgroup><col width=\"19\" span=\"4\" /></colgroup><tbody odd=\"#cceeff\" even=\"#ccffee\"><tr><td bgcolor=\"#cccccc\">Cell 1,1</td><td colspan=\"2\">Cell 1,2-1,3</td><td>Cell 1,4</td></tr><tr><td>Cell 2,1</td><td>Cell 2,2</td><td>Cell 2,3</td><td>Cell 2,4</td></tr><tr><td>Cell 3,1</td><td>Cell 3,2</td><td colspan=\"2\" rowspan=\"2\">Cell 3,3-4.4</td></tr><tr><td>Cell 4,1</td><td>Cell 4,2</td></tr></tbody></table>")) +
                        wxString(wxT("</td></tr><tr><td>Cell 4,1</td><td>Cell 4,2</td></tr></tbody></table>"));
  wxString xmlString5 = wxString(wxT("<h1>Table with row and column spans</h1><br/><table border=\"1\" align=\"center\">\n")) +
                        wxString(wxT("<colgroup><col width=\"50\"/><col width=\"20\"/><col width=\"30\"/><col width=\"20\" span=\"3\" /></colgroup>\n")) +
                        wxString(wxT("<tbody><tr><td rowspan=\"2\" valign=\"middle\" border=\"0\">rowspan=2, valign=middle</td><td>Normal</td><td>Normal</td><td>Normal</td><td colspan=\"2\" rowspan=\"2\" valign=\"bottom\" bgcolor=\"#FF00FF\">colspan=2<br/>rowspan=2<br/>valign=bottom</td></tr>\n")) +
                        wxString(wxT("<tr><td height=\"15\">Normal</td><td rowspan=\"2\" align=\"right\" bgcolor=\"#aaaaaa\" border=\"0\">rowspan=2</td><td border=\"0\">border=0</td></tr>\n")) +
                        wxString(wxT("<tr><td>Normal</td><td>Normal</td><td>Normal</td><td rowspan=\"3\" valign=\"top\" bgcolor=\"#CC3366\">rowspan=3</td><td>Normal</td></tr>\n")) +
                        wxString(wxT("<tr bgcolor=\"#cccccc\"><td>Normal</td><td colspan=\"3\" align=\"center\">align center, colspan=3</td><td>Normal</td></tr>\n")) +
                        wxString(wxT("<tr height=\"20\"><td align=\"right\" valign=\"bottom\">align=right<br/>valign=bottom</td><td>Normal</td><td>&#160;</td><td>Normal</td><td>height=20</td></tr>\n")) +
                        wxString(wxT("</tbody></table>"));
  wxString xmlString6 = wxString(wxT("<h1>Table with more rows than fit on a page</h1><br/><table border=\"1\"><colgroup><col width=\"20\"/></colgroup>")) +
                        wxString(wxT("<thead><tr bgcolor=\"#999999\"><td><b>Headline</b></td></tr></thead>")) +
                        wxString(wxT("<tbody odd=\"#ffffff\" even=\"#dddddd\">")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("<tr><td>This is a table cell with some text. This is a table cell with some text. This is a table cell with some text.</td></tr>")) +
                        wxString(wxT("</tbody></table>"));
  wxString xmlString7 = wxString(wxT("<table border=\"1\"><tbody><tr bgcolor=\"#ffff00\"><td><font face=\"Courier\">")) +
                        wxString(wxT("This is a very simple table with text in font face Courier.")) +
                        wxString(wxT("<code>    // 4 leading spaces\n    <b>if</b> (condition)\n      x++;\n    <b>else</b>&#160;\n      x--;</code>")) +
                        wxString(wxT("<br/></font></td></tr></tbody></table>")) +
                        wxString(wxT("<code>    // 4 leading spaces\n    <b>if</b> (condition)\n      x++;\n    <b>else</b>&#160;\n      x--;</code>"));

  wxPdfDocument pdf;
  pdf.AddPage();
  pdf.SetFont(wxT("Arial"),wxT(""),10);
  pdf.SetRightMargin(50);
  pdf.WriteXml(wxT("<a name=\"top\">Top of first page</a><br/>"));
  pdf.WriteXml(wxT("<a href=\"#bottom\">Jump to bottom of last page</a><br/>"));
  pdf.WriteXml(xmlString1);
  pdf.Ln();
  pdf.WriteXml(wxT("<a name=\"second\">Second anchor</a><br/>"));
  pdf.WriteXml(xmlString2);
  pdf.WriteXml(wxT("<a name=\"third\">Third anchor</a><br/>"));
  pdf.WriteXml(xmlString3);
  pdf.AddPage();
  pdf.SetLeftMargin(20);
  pdf.SetRightMargin(20);
  pdf.SetFont(wxT("Arial"),wxT(""),10);
  pdf.WriteXml(wxT("<a name=\"fourth\">Fourth anchor</a><br/>"));
  pdf.WriteXml(xmlString4);
  pdf.Ln(20);
  pdf.WriteXml(xmlString5);
  pdf.Ln(20);
  pdf.WriteXml(xmlString6);
  pdf.AddPage();
  pdf.WriteXml(xmlString7);
  pdf.WriteXml(wxT("<a name=\"bottom\">Bottom anchor</a><br/>"));
  pdf.WriteXml(wxT("<a href=\"#top\">Jump to top of first page</a><br/>"));
  pdf.WriteXml(wxT("<a href=\"#second\">Jump to second anchor</a><br/>"));
  pdf.WriteXml(wxT("<a href=\"#third\">Jump to third anchor</a><br/>"));

  pdf.SaveAsFile(wxT("xmlwrite.pdf"));
}

