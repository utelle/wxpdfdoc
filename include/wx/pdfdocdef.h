///////////////////////////////////////////////////////////////////////////////
// Name:        pdfdocdef.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-08-04
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfdocdef.h Compile time switches for the \b wxPdfDocument component

/** \mainpage wxPdfDocument

\section intro What is wxPdfDocument?

wxPdfDocument is a C++ class which allows wxWidgets applications to generate PDF files.
The code is a port of <a href="http://www.fpdf.org"><b>FPDF</b></a> - a free PHP class for
generating PDF files - to C++ using the <a href="http://www.wxwidgets.org"><b>wxWidgets</b></a>
library. wxPdfDocument does not make use of any libraries like
<a href="http://www.pdflib.com"><b>PDFlib</b></a> or
<a href="http://www.fastio.com"><b>ClibPDF</b></a> which require a fee at least for
commercial usage. wxPdfDocument is published under the <b>wxWidgets (formerly wxWindows)
license</b>. This means you may use it for any kind of usage and modify it to suit your needs.

wxPdfDocument offers all advantages of \b FPDF.  Several add-on PHP scripts found on the
FPDF web site are incorporated into wxPdfDocument. The main features are:

- Choice of measure unit, page format and margins 
- Page header and footer management 
- Automatic page break 
- Automatic line break and text justification 
- Image support (GIF, JPEG, PNG and WMF) 
- Colors (Grayscale, RGB, CMYK, Spot colors)
- Links (internal and external)
- 14 Adobe standard fonts
- TrueType and Type1 fonts (with or without embedding) and encoding support
- TrueType Unicode and Type0 fonts (for Chinese, Japanese and Korean) support in the Unicode build
- Page compression 
- Graphics primitives for the creation of simple drawings
- Definition of clipping areas
- Bookmarks for outlining the document 
- Rotation
- Protecting the document by passwords and/or access permissions
- Text annotations
- PDF forms (supported field types: text, combo box, check box, radio button, push button)
- JavaScript
- Fill gradients
- Templates

The class can produce documents in many languages other than the Western European ones:
Central European, Cyrillic, Greek, Baltic and Thai, provided you own TrueType or Type1
fonts with the desired character set. In the Unicode build Chinese, Japanese and Korean
are supported, too.

A \ref overview showing all available methods in alphabetical order is provided.
A sample application including more than 20 examples demonstrates the different features.
A separate detailed description is available for the \ref makefont. The chapter \ref writexml
describes the supported tags of the simple XML markup language used by the method wxPdfDocument::WriteXml.

wxPdfDocument is hosted as a component of <a href="http://wxcode.sourceforge.net"><b>wxCode</b></a>.
For any remark, question or problem, you can leave a message on the appropriate \b wxCode
tracker accessible from the <a href="http://wxcode.sourceforge.net/support.php"> wxCode support</a>
page. Or you can send a mail to me 
<a href="&#109;&#97;&#105;&#108;&#116;&#111;:&#117;&#108;&#114;&#105;&#99;&#104;&#46;&#116;&#101;&#108;&#108;&#101;&#64;&#103;&#109;&#120;&#46;&#100;&#101;">directly</a>.

\section version Version history

\todo Although all features were thoroughly tested individually, not all possible combinations were
verified to function properly. This means: wxPdfDocument still needs intensive testing. 
<b>If you find bugs please report them to the author!</b>

<dl>
<dt><b>0.8.0</b> - <i>December 2006</i></dt>
<dd>
Added features:<br>
- support for external templates: pages of existing PDF documents may be imported and used as templates
- font subsetting for TrueType and TrueType Unicode fonts, often resulting in much smaller PDF file sizes
- support for using and embedding OpenType Unicode fonts
- enhanced support for password based encryption, encryption key length freely definable between 40 and 128 bits (<b>Attention</b>: Adobe Reader supports only keys with 40 or 128 bits.)
- support for AES encryption (additional standard encryption method in PDF version 1.6 and above)

wxPdfDocument is compatible with wxWidgets version 2.8.0 as well as with version 2.6.x.

As an add-on preprocessed font files for the free <a href="http://dejavu.sourceforge.net">DejaVu fonts</a>
(version 2.12) are provided in the file release <b><a href="http://sourceforge.net/project/showfiles.php?group_id=51305&package_id=45182&release_id=468705">wxPdfDocument Add-Ons</a></b>.

<b>Attention</b>: For supporting font subsetting for ordinary non-Unicode TrueType fonts
the format of the font definition files has been extended. Font definition files created
with prior versions of the \ref makefont are still usable, but do not support font subsetting.
It is recommended to regenerate own font definition files. Unfortunately common AFM font metric
files do not contain glyph information which is required by the \ref makefont to create the
character-to-glyph mapping. Therefore the utility <tt>ttf2ufm</tt> had to be changed.
The modified version including a Windows executable is available in the file release <b><a href="http://sourceforge.net/project/showfiles.php?group_id=51305&package_id=45182&release_id=468705">wxPdfDocument Add-Ons</a></b>.
</dd>

<dt><b>0.7.6</b> - <i>October 2006</i></dt>
<dd>
Added features (thanks to Stuart Smith):<br>
- setting/getting the default path used for loading font files
- getting the description of the current font
- loading images from a wxInputStream (in addition to loading from file or wxImage)

<b>Attention</b>: The structure of the font definition files has changed due to
the addition of the font attribute <tt>xHeight</tt>. It is necessary to regenerate own
font definition files using the \ref makefont. To support the <tt>xHeight</tt> attribute the utility
<tt>ttf2ufm</tt> had to be changed.

wxPdfDocument is now compatible with wxWidgets version 2.7.1 and above as well as with version 2.6.x.

Fixed several bugs
</dd>

<dt><b>0.7.5</b> - <i>September 2006</i></dt>
<dd>
Added or enhanced features:<br>
- support for tables in simple XML markup spanning more than a page
- support for internal links in simple XML markup
- support for transparency
- support for image masks
- support for internal templates
- support for polygon and shape clipping
- support for printing text along a path
- extended support for fill gradients (<b>API changed!</b>)
- internal color management reworked

Fixed some minor bugs
</dd>

<dt><b>0.7</b> - <i>April 2006</i></dt>
<dd>
Added features:<br>
- support for CMYK and spot colors
- support for named colors (486 predefined names for RGB colors) (wxPdfColour)
- support for color names in HTML notation (\#rrggbb) (wxPdfColour)
- text annotations
- additional font decorations: overline, strikeout
- PDF forms
- JavaScript at the document level
- Simple XML markup language for styling and structuring text

Added or modified methods:<br>
- wxPdfDocument::LineCount, wxPdfDocument::TextBox and several getter methods were added for convenience
- wxPdfDocument::MultiCell now respects a maximal line count
- wxPdfDocument::WriteXml allows to print text containing simple XML markup
</dd>

<dt><b>0.6</b> - <i>November 2005</i></dt>
<dd>
Added features:
- gradients
- transformations
- barcodes<br>
- \ref makefont<br>

Changed API of graphics primitives: line style and fill color parameters deleted,
line style and fill color have to be set using wxPdfDocument::SetLineStyle and wxPdfDocument::SetFillColor.
</dd>

  <dt><b>0.5</b> - <i>September 2005</i></dt>
<dd>
First public release
</dd>

<dt><b>0.4</b> - <i>August 2005</i></dt>
<dd>
Support for embedding fonts
</dd>

<dt><b>0.3</b> - <i>July 2005</i></dt>
<dd>
Support for embedding images
</dd>

<dt><b>0.2</b> - <i>June 2005</i></dt>
<dd>
Several add-ons implemented
</dd>

<dt><b>0.1</b> - <i>May 2005</i></dt>
<dd>
Planning and basic PDF features implemented
</dd>
</dl>

\author Ulrich Telle (<a href="&#109;&#97;&#105;&#108;&#116;&#111;:&#117;&#108;&#114;&#105;&#99;&#104;&#46;&#116;&#101;&#108;&#108;&#101;&#64;&#103;&#109;&#120;&#46;&#100;&#101;">ulrich DOT telle AT gmx DOT de</a>)

\section issues Known issues

\section acknowledgement Acknowledgement

Since wxPdfDocument is based on the great \b FPDF PHP class and several of the contributions to it
found on the <a href="http://www.fpdf.org"><b>FPDF website</b></a> I would like to thank 

- Olivier Plathey (FPDF, Barcodes, Bookmarks, Rotation),
- Maxime Delorme (Sector)
- Johannes Guentert (JavaScript)
- Martin Hall-May (WMF images, Transparency)
- Emmanuel Havet (Code39 barcodes)
- Shailesh Humbad (POSTNET barcodes)
- Matthias Lau (i25 barcodes)
- Pierre Marletta (Diagrams)
- Laurent Passebecq (Labels)
- David Hernandez Sanz (additional graphics primitives)
- Valentin Schmidt (Transparency, Alpha channel)
- Jan Slabon (FPDI)
- Klemen Vodopivec (Protection)
- Moritz Wagner (Transformation)
- Andreas Wuermser (Clipping, Gradients, Transformation)

The wxPdfDocument encryption methods use the RSA Data Security, Inc. MD5 Message
Digest Algorithm (RSA Data Security license) and the Rijndael cipher implementation
of Szymon Stefanek (Public Domain). For detailed license information \see files
pdfencrypt.cpp and pdfrijndael.h.

*/

/** \page overview Reference Manual
The documentation of wxPdfDocument is created by Doxygen. To make it easier to locate the description
of a specific method the following alphabetical list shows all available methods:

\section refpdfdoc wxPdfDocument

\li wxPdfDocument::AcceptPageBreak - accept or not automatic page break
\li wxPdfDocument::AddFont - add a new font
\li wxPdfDocument::AddFontCJK - add a CJK (Chinese, Japanese or Korean) font
\li wxPdfDocument::AppendJavascript - add document level JavaScript
\li wxPdfDocument::AddLink - create an internal link
\li wxPdfDocument::AddPage - add a new page
\li wxPdfDocument::AddSpotColor - add a spot color
\li wxPdfDocument::AliasNbPages - define an alias for number of pages
\li wxPdfDocument::Annotate - add a text annotation
\li wxPdfDocument::Arrow - draw an arrow
\li wxPdfDocument::AxialGradient - define axial gradient shading

\li wxPdfDocument::BeginTemplate - start template creation
\li wxPdfDocument::Bookmark - add a bookmark

\li wxPdfDocument::Cell - print a cell
\li wxPdfDocument::CheckBox - add a check box to a form
\li wxPdfDocument::Circle - draw a circle
\li wxPdfDocument::ClippingText - define text as clipping area
\li wxPdfDocument::ClippingRect - define rectangle as clipping area
\li wxPdfDocument::ClippingEllipse - define ellipse as clipping area
\li wxPdfDocument::ClippingPath - start defining a clipping path
\li wxPdfDocument::ClippingPolygon - define polygon as clipping area
\li wxPdfDocument::ClippedCell - print a clipped cell
\li wxPdfDocument::Close - terminate the document
\li wxPdfDocument::CloseAndGetBuffer - terminate the document and return the document buffer
\li wxPdfDocument::ClosePath - close a clipping path
\li wxPdfDocument::ComboBox - add a combo box to a form
\li wxPdfDocument::CoonsPatchGradient - define coons patch mesh gradient shading
\li wxPdfDocument::Curve - draw a Bezier curve
\li wxPdfDocument::CurveTo - append a cubic Bezier curve to a clipping path

\li wxPdfDocument::Ellipse - draw an ellipse
\li wxPdfDocument::EndTemplate - end template creation

\li wxPdfDocument::Footer - page footer.

\li wxPdfDocument::GetBreakMargin - get the page break margin
\li wxPdfDocument::GetCellMargin - get the cell margin
\li wxPdfDocument::GetFontDescription - get description of current font
\li wxPdfDocument::GetFontFamily - get current font family
\li wxPdfDocument::GetFontPath - get current default path for font files
\li wxPdfDocument::GetFontSize - get current font size in points
\li wxPdfDocument::GetFontStyle - get current font style
\li wxPdfDocument::GetFontSubsetting - get font embedding mode
\li wxPdfDocument::GetImageScale - get image scale
\li wxPdfDocument::GetLeftMargin - get the left margin
\li wxPdfDocument::GetPageWidth - get page width
\li wxPdfDocument::GetPageHeight - get page height
\li wxPdfDocument::GetRightMargin - get the right margin
\li wxPdfDocument::GetScaleFactor - get scale factor
\li wxPdfDocument::GetStringWidth - compute string length
\li wxPdfDocument::GetTemplateSize - get size of template
\li wxPdfDocument::GetTemplateBBox - get bounding box of template
\li wxPdfDocument::GetTopMargin - get the top margin
\li wxPdfDocument::GetX - get current x position
\li wxPdfDocument::GetY - get current y position

\li wxPdfDocument::Header - page header

\li wxPdfDocument::Image - output an image
\li wxPdfDocument::ImageMask - define an image mask
\li wxPdfDocument::ImportPage - import page of external document for use as template
\li wxPdfDocument::IsInFooter - check whether footer output is in progress

\li wxPdfDocument::Line - draw a line
\li wxPdfDocument::LinearGradient - define linear gradient shading
\li wxPdfDocument::LineCount - count the number of lines a text would occupy
\li wxPdfDocument::LineTo - append straight line segment to a clipping path
\li wxPdfDocument::Link - put a link
\li wxPdfDocument::Ln - line break

\li wxPdfDocument::Marker - draw a marker symbol
\li wxPdfDocument::MidAxialGradient - define mid axial gradient shading
\li wxPdfDocument::MirrorH - mirror horizontally
\li wxPdfDocument::MirrorV - mirror vertically
\li wxPdfDocument::MoveTo - begin new subpath of a clipping path
\li wxPdfDocument::MultiCell - print text with line breaks

\li wxPdfDocument::Open - start output to the PDF document

\li wxPdfDocument::PageNo - page number
\li wxPdfDocument::Polygon - draw a polygon
\li wxPdfDocument::PushButton - add a push button to a form

\li wxPdfDocument::RadialGradient - define radial gradient shading
\li wxPdfDocument::RadioButton - add a radio button to a form
\li wxPdfDocument::Rect - draw a rectangle
\li wxPdfDocument::RegularPolygon -  draw a regular polygon
\li wxPdfDocument::Rotate - rotate around a given center
\li wxPdfDocument::RotatedImage - rotate image
\li wxPdfDocument::RotatedText - rotate text string
\li wxPdfDocument::RoundedRect - draw a rounded rectangle

\li wxPdfDocument::SaveAsFile - save the document to a file
\li wxPdfDocument::Scale - scale in X and Y direction
\li wxPdfDocument::ScaleX - scale in X direction only
\li wxPdfDocument::ScaleXY - scale equally in X and Y direction
\li wxPdfDocument::ScaleY - scale in Y direction only
\li wxPdfDocument::Sector - draw a sector
\li wxPdfDocument::SetAlpha - set alpha transparency
\li wxPdfDocument::SetAlphaState - set alpha state
\li wxPdfDocument::SetAuthor - set the document author
\li wxPdfDocument::SetAutoPageBreak - set the automatic page breaking mode
\li wxPdfDocument::SetCellMargin - set cell margin
\li wxPdfDocument::SetCompression - turn compression on or off
\li wxPdfDocument::SetCreator - set document creator
\li wxPdfDocument::SetDisplayMode - set display mode
\li wxPdfDocument::SetDrawColor - set drawing color
\li wxPdfDocument::SetFillColor - set filling color
\li wxPdfDocument::SetFillGradient - paint a rectangular area using a fill gradient
\li wxPdfDocument::SetFont - set font
\li wxPdfDocument::SetFontPath - set default path for font files
\li wxPdfDocument::SetFontSize - set font size
\li wxPdfDocument::SetFontSubsetting - set font embedding mode
\li wxPdfDocument::SetFormBorderStyle - set form field border style
\li wxPdfDocument::SetFormColors - set form field colors (border, background, text)
\li wxPdfDocument::SetImageScale - set image scale
\li wxPdfDocument::SetKeywords - associate keywords with document
\li wxPdfDocument::SetLeftMargin - set left margin
\li wxPdfDocument::SetLineStyle - set line style
\li wxPdfDocument::SetLineWidth - set line width
\li wxPdfDocument::SetLink - set internal link destination
\li wxPdfDocument::SetMargins - set margins
\li wxPdfDocument::SetProtection - set permissions and/or passwords
\li wxPdfDocument::SetRightMargin - set right margin
\li wxPdfDocument::SetSourceFile - set source file of external template document
\li wxPdfDocument::SetSubject - set document subject
\li wxPdfDocument::SetTemplateBBox - set bounding box of template
\li wxPdfDocument::SetTextColor - set text color
\li wxPdfDocument::SetTitle - set document title
\li wxPdfDocument::SetTopMargin - set top margin
\li wxPdfDocument::SetViewerPreferences - set viewer preferences
\li wxPdfDocument::SetX - set current x position
\li wxPdfDocument::SetXY - set current x and y positions
\li wxPdfDocument::SetY - set current y position
\li wxPdfDocument::Shape - draw shape
\li wxPdfDocument::ShapedText - print text along a path
\li wxPdfDocument::Skew - skew in X and Y direction
\li wxPdfDocument::SkewX - skew in Y direction only
\li wxPdfDocument::SkewY - skew in Y direction only
\li wxPdfDocument::StarPolygon - draw star polygon
\li wxPdfDocument::StartTransform - begin transformation
\li wxPdfDocument::StopTransform - end transformation

\li wxPdfDocument::Text - print a string
\li wxPdfDocument::TextBox - print a string horizontally and vertically aligned in a box
\li wxPdfDocument::TextField - add a text field to a form
\li wxPdfDocument::Translate - move the origin 
\li wxPdfDocument::TranslateX - move the X origin only
\li wxPdfDocument::TranslateY - move the Y origin only

\li wxPdfDocument::UseTemplate - use template
\li wxPdfDocument::UnsetClipping - remove clipping area

\li wxPdfDocument::Write - print flowing text
\li wxPdfDocument::WriteCell - print flowing text with cell attributes
\li wxPdfDocument::WriteXml - print flowing text containing simple XML markup

\li wxPdfDocument::wxPdfDocument - constructor


\section refpdfbarcode wxPdfBarCodeCreator

\li wxPdfBarCodeCreator::Code39
\li wxPdfBarCodeCreator::EAN13
\li wxPdfBarCodeCreator::UPC_A
\li wxPdfBarCodeCreator::I25
\li wxPdfBarCodeCreator::PostNet

*/

/** \page makefont MakeFont Utility
\section mkfontadd Adding new fonts and encoding support

This section explains how to use \b TrueType or \b Type1 fonts so that you are not
limited to the standard fonts any more. The other interest is that you can
choose the font encoding, which allows you to use other languages than the
Western ones (the standard fonts having too few available characters). 
 
There are two ways to use a new font: embedding it in the PDF or not. When a
font is not embedded, it is sought in the system. The advantage is that the
PDF file is lighter; on the other hand, if it is not available, a substitution
font is used. So it is preferable to ensure that the needed font is installed
on the client systems. If the file is to be viewed by a large audience, it is
better to embed the fonts. 
 
Adding a new font requires three steps for \b TrueType fonts: 

\li Generation of the metric file (.afm) 
\li Generation of the font definition file (.xml) 
\li Declaration of the font in the program 

For \b Type1, the first one is theoretically not necessary because the AFM file is
usually shipped with the font. In case you have only a metric file in PFM format,
it must be converted to AFM first. 

\section mkfontgen1 Generation of the metric file

The first step for a \b TrueType font consists in generating the AFM file (or UFM file in case of a 
<b>Unicode TrueType</b> font). A utility exists to do this task: <tt>ttf2ufm</tt> - a special version of
<tt>ttf2pt1</tt> - allowing to create AFM and/or UFM files. <tt>ttf2ufm</tt> has been modified to
generate AFM and UFM files containing all the information which is required by the utility program
\b makefont. An archive containing the modified source code of <tt>ttf2ufm</tt> and a Windows executable can be
downloaded from <b><a href="http://wxcode.sourceforge.net/docs/wxpdfdoc/ttf2ufm.zip">here</a></b>.
The command line to use is the following: 
 
<tt>ttf2ufm -a font.ttf font </tt>
 
For example, for Comic Sans MS Regular: 
 
<tt>ttf2ufm -a c:/windows/fonts/comic.ttf comic </tt>
 
Two files are created; the one we are interested in is comic.afm. 

\section mkfontgen2 Generation of the font definition file

The second step consists in generating a wxPdfDocument font metrics XML file containing
all the information needed by wxPdfDocument; in addition, the font file is compressed.
To do this, a utility program, \b makefont, is provided.

<tt>makefont {-a font.afm | -u font.ufm } [-f font.{ttf|pfb}] [-e encoding] [-p patch] [-t {ttf|otf|t1}]</tt>

<table border=0>
<tr><td><tt>-a font.afm</tt></td><td>AFM font metric file for \b TrueType or \b Type1 fonts</td></tr>
<tr><td><tt>-u font.ufm</tt></td><td>UFM font metric file for <b>TrueType Unicode</b> or <b>OpenType Unicode</b> fonts</td></tr>
<tr><td valign="top"><tt>-f font.{ttf|otf|pfb}</tt></td><td>font file (<tt>.ttf</tt> = TrueType, <tt>.otf</tt> = OpenType, <tt>.pfb</tt> = Type1).
<br>If you own a Type1 font in ASCII format (<tt>.pfa</tt>), you can convert it to binary format with
<a href="http://www.lcdf.org/~eddietwo/type/#t1utils">t1utils</a>.
<br>If you don't want to embed the font, omit this parameter. In this case, type is given by the type parameter. 
</td></tr>
<tr><td valign="top"><tt>-e encoding</tt></td><td>font encoding, i.e. cp1252. Omit this parameter for a symbolic font.like <i>Symbol</i>
or <i>ZapfDingBats</i>.

The encoding defines the association between a code (from 0 to 255) and a character.
The first 128 are fixed and correspond to ASCII; the following are variable.
The encodings are stored in .map files. Those available are: 

\li cp1250 (Central Europe) 
\li cp1251 (Cyrillic) 
\li cp1252 (Western Europe) 
\li cp1253 (Greek) 
\li cp1254 (Turkish) 
\li cp1255 (Hebrew) 
\li cp1257 (Baltic) 
\li cp1258 (Vietnamese) 
\li cp874 (Thai) 
\li iso-8859-1 (Western Europe) 
\li iso-8859-2 (Central Europe) 
\li iso-8859-4 (Baltic) 
\li iso-8859-5 (Cyrillic) 
\li iso-8859-7 (Greek) 
\li iso-8859-9 (Turkish) 
\li iso-8859-11 (Thai) 
\li iso-8859-15 (Western Europe) 
\li iso-8859-16 (Central Europe) 
\li koi8-r (Russian) 
\li koi8-u (Ukrainian) 

Of course, the font must contain the characters corresponding to the chosen encoding. 
The encodings which begin with cp are those used by Windows; Linux systems usually use ISO. 
Remark: the standard fonts use cp1252. 

\b Note: For TrueType Unicode and OpenType Unicode fonts this parameter is ignored.
</td></tr>
<tr><td valign="top"><tt>-p patch</tt></td><td>patch file for individual encoding changes.
Use the same format as the <tt>.map</tt> files for encodings.
A patch file gives the possibility to alter the encoding.
Sometimes you may want to add some characters. For instance, ISO-8859-1 does not contain
the euro symbol. To add it at position 164, create a file containing the line
<p><tt>!A0 U+20AC Euro</tt>
<p>\b Note: The Unicode character id will not be interpreted.

For TrueType Unicode and OpenType Unicode fonts this parameter is ignored.
</td></tr>
<tr><td><tt>-t {ttf|otf|t1}</tt></td><td>font type (ttf = TrueType, otf = OpenType, t1 = Type1). Only needed if omitting the font file.</td></tr>
</table>

\b Note: in the case of a font with the same name as a standard one, for instance arial.ttf,
it is mandatory to embed. If you don't, Acrobat will use its own font. 

Executing <tt>makefont</tt> generates an .xml file, with the same name as the
<tt>.afm</tt> file resp. <tt>.ufm</tt> file. You may rename it if you wish. In case of
embedding the font file is compressed and gives a file with <tt>.z</tt> as extension.
For <b>Unicode TrueType</b> fonts a file with extension <tt>.ctg.z</tt> is created containing
the character to glyph mapping.You may rename these files, too, but in this case you have to
alter the file name(s) in the file tag in the <tt>.xml</tt> file accordingly.

You have to copy the generated file(s) to the font directory.

\section mkfontdecl Declaration of the font in the script

The last step is the most simple. You just need to call the AddFont() method. For instance: 
 
<tt>pdf.AddFont(_T("Comic"),_T(""),_T("comic.xml"));</tt>
  
or simply: 
 
<tt>pdf.AddFont(_T("Comic"));</tt>
  
And the font is now available (in regular and underlined styles), usable like the others.
If we had worked with Comic Sans MS Bold (comicbd.ttf), we would have put: 
 
<tt>pdf.AddFont(_T("Comic"),_T("B"),_T("comicbd.xml"));</tt>
  
\section mkfontreduce Reducing the size of TrueType fonts

Font files are often quite voluminous; this is due to the
fact that they contain the characters corresponding to many encodings. zlib compression
reduces them but they remain fairly big. A technique exists to reduce them further.
It consists in converting the font to the \b Type1 format with <tt>ttf2pt1</tt> by specifying the
encoding you are interested in; all other characters will be discarded. 
For instance, the arial.ttf font shipped with Windows 98 is 267KB (it contains 1296
characters). After compression it gives 147. Let's convert it to \b Type1 by keeping
only cp1250 characters: 
 
<tt>ttf2ufm -b -L cp1250.map c:/windows/fonts/arial.ttf arial </tt>
 
The <tt>.map</tt> files are located in the <tt>makefont</tt> directory.
The command produces arial.pfb and arial.afm. The arial.pfb file is only 35KB,
and 30KB after compression. 
 
It is possible to go even further. If you are interested only in a subset of the
encoding (you probably don't need all 217 characters), you can open the .map file
and remove the lines you are not interested in. This will reduce the file size
accordingly. 

Since wxPdfDocument version 0.8.0 automatic font subsetting is supported for
TrueType und TrueType Unicode fonts. <b>Note</b>: The font license must allow embedding and
subsetting.
*/

/** \page writexml Styling text using a simple markup language
\section tagoverview Overview

The method wxPdfDocument::WriteXML allows to write text to PDF using a simple markup language.
This allows for example to change font attributes within a cell, which is not supported by
methods like wxPdfDocument::WriteCell or wxPdfDocument::MultiCell. The supported markup
language consists of a small subset of HTML. Although the subset might be extended in future
versions of \b wxPdfDocument, it is not the goal of this method to allow to convert
full fledged HTML pages to PDF. 

\b Important! The XML dialect used is very strict. Each tag must have a corresponding closing tag
and all attribute values must be enclosed in double quotes.

Usually the current position should be at the left margin when calling wxPdfDocument::WriteXML.
If the current position is \b not at left margin and the text passed to wxPdfDocument::WriteXML
occupies more than a single line, you may get strange results. Until version \b 1.0 of wxPdfDocument
will be released the behaviour of wxPdfDocument::WriteXML might change without prior notice.

Currently there is only limited error handling. You will get strange results or no results at all
if tags are incorrectly used. Unknown tags and all their content are silently ignored.

\section tagref Reference of supported tags

The following sections describe the tags supported by the wxPdfDocument markup language.

\subsection simpletags Simple text markup

There are several tags to influence the size and weight of the font used for displaying the text
and the relative vertical position within a line:

<table border="0">
<tr bgcolor="#6699dd"><td><b>Tag</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;b&gt; ... &lt;/b&gt;</tt></td><td>bold text</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;i&gt; ... &lt;/i&gt;</tt></td><td>italic text</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;u&gt; ... &lt;/u&gt;</tt></td><td>underlined text</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;o&gt; ... &lt;/o&gt;</tt></td><td>overlined text</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;s&gt; ... &lt;/s&gt;</tt></td><td>strike-through text</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;strong&gt; ... &lt;/strong&gt;</tt></td><td>bold text (same as <tt>&lt;b&gt;</tt>)</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;em&gt; ... &lt;/em&gt;</tt></td><td>emphasized text (same as <tt>&lt;i&gt;</tt>)</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;small&gt; ... &lt;/small&gt;</tt></td><td>text with reduced font size</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;sup&gt; ... &lt;/sup&gt;</tt></td><td>superscripted text</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;sub&gt; ... &lt;/sub&gt;</tt></td><td>subscripted text</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;h1&gt; ... &lt;/h1&gt;</tt></td><td>headline level 1</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;h2&gt; ... &lt;/h2&gt;</tt></td><td>headline level 2</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;h3&gt; ... &lt;/h3&gt;</tt></td><td>headline level 3</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;h4&gt; ... &lt;/h4&gt;</tt></td><td>headline level 4</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;h5&gt; ... &lt;/h5&gt;</tt></td><td>headline level 5</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;h6&gt; ... &lt;/h6&gt;</tt></td><td>headline level 6</td></tr>
</table>

\subsection structtags Structuring text markup

Some tags for structuring the text layout are available. Most of these tags have one or more
attributes to change its properties. Click on the tag description to see a detailed description
of the attributes.

<table border="0">
<tr bgcolor="#6699dd"><td><b>Tag</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;ul&gt; ... &lt;/ul&gt;</tt></td><td>\ref ulist</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;ol&gt; ... &lt;/ol&gt;</tt></td><td>\ref olist</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;li&gt; ... &lt;/li&gt;</tt></td><td><b>List item</b> of an ordered or unordered list</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;br /&gt;</tt></td><td><b>Line break</b>, positions the current position to the left margin of the next line</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;p&gt; ... &lt;/p&gt;</tt></td><td>\ref ptag</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;hr /&gt;</tt></td><td>\ref hrtag</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;a&gt; ... &lt;/a&gt;</tt></td><td>\ref atag</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;font&gt; ... &lt;/font&gt;</tt></td><td>\ref fonttag</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;table&gt; ... &lt;/table&gt;</tt></td><td>\ref tabletag</td></tr>
</table>

\subsection misctags Miscelleaneous text markup

This section lists a few additional tags not fitting in any other category.
Click on the tag description to see a detailed description of the attributes.

<table border="0">
<tr bgcolor="#6699dd"><td><b>Tag</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;msg&gt; ... &lt;/msg&gt;</tt></td><td>\ref msgtag</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;img ... /&gt;</tt></td><td>\ref imgtag</td></tr>
</table>

\subsection ulist Unordered lists

Unordered lists start on a new line. Each list item is preceded by a list item marker and the content of the
list item is indented.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;ul&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>type="bullet|dash|<i>number</i>"</tt></td><td>Sets the type of the list item marker
<p><tt><b>bullet</b></tt> displays a bullet character</p>
<p><tt><b>dash</b></tt> displays a dash character</p>
<p><tt><i>number</i></tt> has a value between 0 and 255. The corresponding character of the \b ZapfDingBats font
is used as the list item marker</p></td></tr>
</table>

\subsection olist Ordered lists

Ordered lists start on a new line. Each list item is preceded by a list item enumerator and the content of the
list item is indented.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;ol&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>type="1|a|A|i|I|z1|z2|z3|z4"</tt></td><td>Sets the type of the list item enumerator
<p><tt><b>1</b></tt> displays a decimal number as the list item enumerator</p>
<p><tt><b>a</b></tt> displays a lowercase alphabetic character as the list item enumerator</p>
<p><tt><b>A</b></tt> displays a uppercase alphabetic character as the list item enumerator</p>
<p><tt><b>i</b></tt> displays a lowercase roman number as the list item enumerator</p>
<p><tt><b>I</b></tt> displays a uppercase roman number as the list item enumerator</p>
<p><tt><b>z1|z2|z3|z4</b></tt> displays number symbols of one of the 4 number series in the \b ZapfDingBats font. This option should only be used for lists of at most 10 items.</p>
</td></tr>
<tr bgcolor="#ddeeff"><td><tt>start="<i>number</i>"</tt></td><td><i>number</i> represents the enumerator value of the first list item</td></tr>
</table>

\subsection ptag Paragraph

A paragraph starts on a new line and forces an empty line after the closing paragraph tag.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;p&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>align="left|right|center|justify"</tt></td><td>As specified by this
option the content of the paragraph will be \b left or \b right aligned, \b centered or \b justified.
The default is \b left aligned.</td></tr>
</table>

\subsection hrtag Horizontal rule

A horizontal rule is a line of specified width which is drawn on a separate line.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;hr&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>width="<i>number</i>"</tt></td><td>The width of the horizontal rule
is an integer <i>number</i> between 1 and 100 giving the width in percent of the available width (from left to right margin).
The default value is 100.</td></tr>
</table>

\subsection atag Internal or external link

An internal or external link is displayed as blue underlined text. Clicking on the text opens a browser window
loading the referenced URL.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;a&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td><tt>href="<i>url</i>"</tt></td><td><i>url</i> is an unified resource locator.
If <i>url</i> starts with <b>#</b> it is interpreted as a reference to an internal link anchor;
the characters following <b>#</b> are used as the name of the anchor.</td></tr>
<tr bgcolor="#6699dd"><td><tt>name="<i>anchor</i>"</tt></td><td><i>anchor</i> is the name of an internal link anchor.</td></tr>
</table>

<b>Note:</b> Either the <b><tt>name</tt></b> or the <b><tt>href</tt></b> attribute may be specified, but not both.

\subsection fonttag Font specification

This tag allows to specify several font attributes for the embedded content. Font family,
font size and color can be set. Attributes not given retain their previous value.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;font&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>face="<i>fontfamily</i>"</tt></td><td>The name of the font family. It can be the name of one of the
14 core fonts or the name of a font previously added by wxPdfDocument::AddFont.</td></tr>
<tr bgcolor="#ddeeff"><td><tt>size="<i>fontsize</i>"</tt></td><td>The font size in points</td></tr>
<tr bgcolor="#eeeeee"><td><tt>color="<i>fontcolor</i>"</tt></td><td>The font color in HTML notation, i.e. <b><i>\#rrggbb</i></b>,
or as a named color, i.e. <b><i>red</i></b>.</td></tr>
</table>

\subsection msgtag Translatable text

For international applications a simple mechanism is provided to pass a string to <b><code>wxGetTranslation</code></b>.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;msg&gt;</b></td></tr>
</table>

The text string included in the <b><tt>msg</tt></b> tag will be translated if a translation is available before it is written to PDF.

<b>Note:</b> Within the <b><tt>msg</tt></b> tag additional markup is not allowed.

\subsection imgtag Images

In the current implementation output of an image always starts on a new line.

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;img&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>src="<i>imagefile</i>"</tt></td><td>The name of the image file.</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>width="<i>image width</i>"</tt></td><td>The width of the image measured in pixels.</td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>height="<i>image height</i>"</tt></td><td>The height of the image measured in pixels.</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>align="left|right|center"</tt></td><td>As specified by this
option the image will be \b left or \b right aligned, or \b centered.
The default is \b left aligned.</td></tr>
</table>

\section tabletag Tables

Very often information is presented in a tabular structure. This is also supported by the wxPdfDocument markup language
by using a specific kind of HTML table syntax. The structure is as follows:
<pre>
    &lt;table&gt;
      &lt;colgroup&gt;
        &lt;col ... /&gt;
        ...
      &lt;/colgroup&gt;
      &lt;thead&gt;
        &lt;tr&gt;&lt;td&gt; ... &lt;/td&gt;&lt;/tr&gt;
        ...      
      &lt;/thead&gt;
      &lt;tbody&gt;
        &lt;tr&gt;&lt;td&gt; ... &lt;/td&gt;&lt;/tr&gt;
        ...      
      &lt;/tbody&gt;
    &lt;/table&gt;
</pre>
The <b><tt>colgroup</tt></b> tag and embedded <b><tt>col</tt></b> tags are always required since all column widths have to be specified
a priori. <b><tt>width</tt></b> attributes are not interpreted when used in other table tags.

The <b><tt>thead</tt></b> tag and embedded table rows and cells are allowed, but since the current implementation only supports
tables fitting completely on one page, the rows are handled as ordinary rows. (A future release will support tables
spanning more than one page. Header rows will be repeated on each page.)

The use of the <b><tt>tbody</tt></b> tag is always required.

Nested tables are supported.

The <b><tt>table</tt></b> tag may have the following attributes:

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;table&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>border="<i>number</i>"</tt></td><td>Table cells may have borders on each side.
This attribute specifies whether cells will have borders on every side or not. This may be overriden for each individual cell.
The attribute value consists of the combination of up to 4 letters:
<p>\b 0 - no borders<br>
<b> &gt; 0</b> - borders on all sides of each cell<br>
</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>align="left|right|center"</tt></td><td>Defines the horizontal alignment of the table. Default is the alignment of the surrounding context.</td></tr>
<tr bgcolor="#eeeeee"><td><tt>valign="top|middle|bottom"</tt></td><td>Defines the vertical alignment of the table. Default is <i>top</i>.</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>cellpadding="<i>number</i>"</tt></td><td><i>Number</i> defines the padding width on each side of a cell. Default is 0.</td></tr>
</table>

The supported tags and their attributes are shown in the following tables:

<table border="0">
<tr bgcolor="#6699dd"><td><b>Tag</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;table&gt; ... &lt;/table&gt;</tt></td><td>Groups the definitions of column widths. Contains one or more &lt;col&gt; tags.</td></tr>
<tr bgcolor="#eeeeee"><td><tt>&lt;colgroup&gt; ... &lt;/colgroup&gt;</tt></td><td>Groups the definitions of column widths. Contains one or more &lt;col&gt; tags.</td></tr>
<tr bgcolor="#ddeeff"><td><tt>&lt;col width="<i>width</i>" span="<i>number</i>"&gt; ... &lt;/col&gt;</tt></td><td>
Defines the <i>width</i> of one or more columns. <i>number</i> specifies for how many columns the width is specified, default is 1.
</td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>&lt;thead odd="<i>background color for odd numbered rows</i>" even="<i>background color for even numbered rows</i>"&gt; ... &lt;/thead&gt;</tt></td>
<td>Defines a group of table header rows.
Contains one or more &lt;tr&gt; tags. If a table does not fit on a single page these rows are repeated on each page.
The attributes <b><tt>odd</tt></b> and <b><tt>even</tt></b> are optional.
</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>&lt;tbody odd="<i>background color for odd numbered rows</i>" even="<i>background color for even numbered rows</i>"&gt; ... &lt;/tbody&gt;</tt></td>
<td>Defines a group of table body rows. Contains one or more &lt;tr&gt; tags.
The attributes <b><tt>odd</tt></b> and <b><tt>even</tt></b> are optional.
</td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>&lt;tr bgcolor="<i>background color</i>" height="<i>height</i>"&gt; ... &lt;/tr&gt;</tt></td>
<td>Defines a table row. Contains one or more &lt;td&gt; tags.
<p>The <i>background color</i> may be specified in HTML notation, i.e. <b><i>\#rrggbb</i></b>,
or as a named color, i.e. <b><i>red</i></b>. If no background color is given the background is transparent.</p>
<p>Usually the height of the highest cell in a row is used as the row height, but a minimal row <i>height</i> may be specified, too</p>
</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>&lt;td&gt; ... &lt;/td&gt;</tt></td><td>Defines a table cell. 
<p>The available attributes are described in section \ref tdtag.</p></td></tr>
</table>

\subsection tdtag Table cells

A table cell can have several attributes:

<table border="0">
<tr bgcolor="#6699dd"><td colspan="2"><b>Tag</b></td></tr>
<tr bgcolor="#eeeeee"><td colspan="2"><b>&lt;td&gt;</b></td></tr>
<tr bgcolor="#6699dd"><td><b>Attribute</b></td><td><b>Description</b></td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>border="LTBR"</tt></td><td>A cell may have a border on each side.
This attribute overrides the border specification in the &lt;table&gt; tag. The attribute value consists of 
the combination of up to 4 letters:
<p>\b L - border on the left side of the cell<br>
\b T - border on the top side of the cell<br>
\b B - border on the bottom side of the cell<br>
\b R - border on the right side of the cell</p>
.</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>align="left|right|center"</tt></td><td>Defines the horizontal alignment of the cell content. Default is <i>left</i>.</td></tr>
<tr bgcolor="#eeeeee"><td><tt>valign="top|middle|bottom"</tt></td><td>Defines the vertical alignment of the cell content. Default is <i>top</i>.</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>bgcolor="<i>background color</i>"</tt></td><td>The background color of the cell in HTML notation, i.e. <b><i>\#rrggbb</i></b>,
or as a named color, i.e. <b><i>red</i></b>. This attribute overrides the background color specification of the row.
If neither a row nor a cell background color is specified the background is transparent.</td></tr>
<tr bgcolor="#eeeeee"><td valign="top"><tt>rowspan="<i>number</i>"</tt></td><td><i>Number</i> of rows this cell should span. Default is 1.</td></tr>
<tr bgcolor="#ddeeff"><td valign="top"><tt>colspan="<i>number</i>"</tt></td><td><i>Number</i> of columns this cell should span. Default is 1.</td></tr>
</table>

*/

#ifndef _PDFDOC_DEF_H_
#define _PDFDOC_DEF_H_

#ifdef WXMAKINGDLL_WXPDFDOC
    #define WXDLLIMPEXP_PDFDOC WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_PDFDOC WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_PDFDOC
#endif

#endif // _PDFDOC_DEF_H_

