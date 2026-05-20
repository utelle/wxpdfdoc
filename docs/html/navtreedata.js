/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "wxPdfDocument", "index.html", [
    [ "What is wxPdfDocument?", "index.html#intro", null ],
    [ "Current Version", "index.html#version", null ],
    [ "Known issues", "index.html#issues", null ],
    [ "Acknowledgements", "index.html#acknowledgement", null ],
    [ "Version history", "history.html", null ],
    [ "Reference Manual", "overview.html", [
      [ "wxPdfDocument", "overview.html#refpdfdoc", null ],
      [ "wxPdfFontManager", "overview.html#refpdffontmanager", null ],
      [ "wxPdfBarCodeCreator", "overview.html#refpdfbarcode", null ]
    ] ],
    [ "MakeFont Utility", "makefont.html", [
      [ "Adding new fonts and encoding support", "makefont.html#mkfontadd", null ],
      [ "Generation of the metric file", "makefont.html#mkfontgen1", null ],
      [ "Generation of the font definition file", "makefont.html#mkfontgen2", null ],
      [ "Declaration of the font in the script", "makefont.html#mkfontdecl", null ],
      [ "Reducing the size of TrueType fonts", "makefont.html#mkfontreduce", null ]
    ] ],
    [ "ShowFont Utility", "showfont.html", [
      [ "Usage", "showfont.html#useshowfont", null ],
      [ "Ranges", "showfont.html#showfontranges", null ],
      [ "Colours", "showfont.html#showfontcolour", null ],
      [ "Examples", "showfont.html#showfontexample", null ]
    ] ],
    [ "Styling text using a simple markup language", "writexml.html", [
      [ "Overview", "writexml.html#tagoverview", null ],
      [ "Reference of supported tags", "writexml.html#tagref", [
        [ "Notes", "writexml.html#notes", null ],
        [ "Simple text markup", "writexml.html#simpletags", null ],
        [ "Structuring text markup", "writexml.html#structtags", null ],
        [ "Miscellaneous text markup", "writexml.html#misctags", null ],
        [ "Unordered lists", "writexml.html#ulist", null ],
        [ "Ordered lists", "writexml.html#olist", null ],
        [ "Paragraph", "writexml.html#ptag", null ],
        [ "Horizontal rule", "writexml.html#hrtag", null ],
        [ "Internal or external link", "writexml.html#atag", null ],
        [ "Font specification", "writexml.html#fonttag", null ],
        [ "Translatable text", "writexml.html#msgtag", null ],
        [ "Images", "writexml.html#imgtag", null ]
      ] ],
      [ "Tables", "writexml.html#tabletag", [
        [ "Table cells", "writexml.html#tdtag", null ]
      ] ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Enumerator", "functions_eval.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"classwx_pdf_cell_context.html#aab1c7587234eaec1336bfe6fb44344ca",
"classwx_pdf_document.html#a23a5a03ddaf89cd5c4997111684ec5d1",
"classwx_pdf_document.html#aebb513eb375c17ee3d0afde2f948af2e",
"classwx_pdf_font_data.html#addd6691f9516b6e4a78225d373ef5c84",
"classwx_pdf_font_extended.html#af6c16a1ae704d00eda8a43a2e786871c",
"classwx_pdf_image.html#af7ef8722e230aa23432313b5e2e5ffea",
"classwx_pdf_parser.html#aadf750f76489fe30f55ba950364a93cc",
"classwx_pdf_push_button.html#a40f4ce1c3c53bf81fca993dbe9f0fa85",
"functions_func.html",
"namespacewx_pdf_barcode.html#afff100d44cbab54155b8e89156444ed5",
"pdfparser_8h.html#a452e313d21aa8088981fc1fc1470d83c",
"struct__wx_pdf_cjk_font_desc.html#ad062c9f56ab6faf1f111ab2a835ed525"
];

const SYNCONMSG = 'click to disable panel synchronization';
const SYNCOFFMSG = 'click to enable panel synchronization';
const LISTOFALLMEMBERS = 'List of all members';