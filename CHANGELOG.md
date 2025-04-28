# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and from version 0.9.0 onwards this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.3.1] - 2025-04-28

### Fixed

- Adjusted make files to properly handle white space in path names (issue #96)

## [1.3.0] - 2025-02-12

### Added

- Added support for fonts in _Web Open Font Format_ (WOFF/WOFF2)
- Added support for TrueType/OpenType fonts given as byte arrays
- Added support for additional barcodes (especially 2D barcodes like QR code, DataMatrix etc).
  In principle, all barcodes supported by the [**Zint library**](https://zint.org.uk/) can be used.

### Fixed

- Fixed wxPdfDocument::Translate by adding missing unit conversion factor

## [1.2.1] - 2025-01-06

### Changed

- Improved font lookup in wxPdfFontManager to search also under the font family alias, if given
- Replaced use of wxScreenDC by wxDisplay
- Restored support for wxWidgets 3.0.x

### Fixed

- Check for availability of header file `Security/SecRandom.h` (fix issue #94)
- Fixed wxPdfDCImpl::DoDrawText for multiline text

## [1.2.0] - 2024-02-18

**wxPdfDocument** is compatible with wxWidgets versions 3.2.x and above.

### Added

- Added support for document protection with AES-256 encryption (PDF 2.0).
- Added support for importing PDF documents protected with AES encryption.

### Changed

- The library now requires a C++11 compiler for compilation.
- Build support for Visual C++ versions below 2015 has been removed.

### Fixed

- Fixed problem with page breaks in XML tables.

## [1.1.0] - 2024-01-27

### Changed

- The signature of methods `ScaleX`, `ScaleY`, `ScaleXY`, `Scale`, `MirrorH`, `MirrorV`, `SkewX`, `SkewY`, `Skew`, `Rotate`, and `UseTemplate` has been changed to get rid of the negative default values for the position parameters. This was necessary to support negative coordinates in all graphics operations. If an application had explicitly specified the default value (`-1`) in calls to one of the above methods, these values have to be replaced by calls to method `GetX` resp `GetY`.

### Fixed

- Fixed issue #92 "Rotated text at wrong position". The problem arose from the use of negative coordinates. All graphics operations now support negative coordinates, too.
- Fixed output of wrong text background colour for rotated text in `wxPdfDC`.

## [1.0.3] - 2023-11-16

### Fixed

- Fixed handling of long table: take top page margin, page header and table header into account for page breaks.
- Fixed handling for empty table bodies.
- Fixed handling of the height of (optional) table header rows (taking it into account for calculating table parts fitting on a page).
- Fixed isue #81. Improve handling of table cell borders in markup. Table cell borders could be (partially) hidden by table cell backgrounds.
- Actually use wxPdfDocument::GetImageSize() MIME type parameter.
- Fixed handling of surrogates for TrueType fonts.
- Fixed problem with automatic page break

## [1.0.2] - 2021-12-23

**wxPdfDocument** is compatible with wxWidgets versions 3.0.x and 3.1.x.

### Changed

- Added attribute `viewport` for XML markup element `img`
- Added option to specify the unit for numeric attributes in XML markup elements
- Slightly optimized handling for the XML markup element `img`
- Added new flag `wxPDF_VIEWER_NOPRINTSCALING` for method `wxPdfDocument::SetViewerPreferences`
- Added method `wxPdfDocument::SetPaperHandling` for better printout handling

### Fixed

- Fixed markup handling for paragraphs in table cells
- Fixed markup table handling (wrong total height used to reserve space)
- Fixed justification issue in markup text
- Adjusted handling of pens and brushes in `wxPdfDC`

## [1.0.1] - 2021-09-19

**wxPdfDocument** is compatible with wxWidgets versions 3.0.x and 3.1.x.

### Fixed

- Fixed hatch pattern scaling in `wxPdfDC`
- Fixed compile time error in ANSI mode
- Fixed some documentation glitches

## [1.0.0] - 2021-09-14

**wxPdfDocument** is compatible with wxWidgets versions 3.0.x and 3.1.x.

### Added

- Added support for PDF/A-1B conformance
- Added transformation matrix support for wxPdfDC
- Added attribute "char-spacing" for XML markup element "span"
- Added maximum height attribute for table rows in XML markup

### Changed

- Implemented extended support for fill patterns (template based patterns, various hatch patterns)
- Enhanced support for `wxBrush` styles in `wxPdfDC` (stipple and hatch styles)
- Changed data type of image measures in XML markup (from integer to double)
- Optimized `wxPdfDC` output (setting of pens, brushes, state changes)

### Fixed

- Fixed issue with bitmap images in `wxPdfDC` (now using globally unique identifiers)
- Fixed `wxPdfDC` issue with pen and brush color
- Fixed issue with patterns in templates
- Use the transparent background mode by default (relevant for alpha support in `wxPdfDC`)

### Removed

- Remove support for wxWidgets 2.x

## [0.9.8] - 2019-09-21

**wxPdfDocument** is compatible with wxWidgets versions 2.8.12, 3.0.x and 3.1.x.

### Added

- Added alpha transparency support for wxPdfDC (wxWidgets 3.x only)
- Added configure options to skip building samples/utilities
- Added build support for Visual C++ 2019
- Added AppVeyor CI

### Changed

- Eliminated error messages issued from method `wxPdfFontManager::RegisterSystemFonts` on registering bitmap fonts files
- Removed call to deprecated function `wxFillLogFont`

### Fixed

- Fixed issue with XML tables: Rows affected by common row spans are now kept together on the same page

## [0.9.7] - 2019-01-09

**wxPdfDocument** is compatible with wxWidgets versions 2.8.12, 3.0.x and 3.1.x.

### Added

- Added a new map mode style `wxPDF_MAPMODESTYLE_PDFFONTSCALE`
- Added support for justified text alignment when using Unicode fonts
- Added border width and colour attributes to XML table output
- Added speed up for text drawing in `wxPdfDC` class
- Added multiline text support for `wxPdfDC::DoDrawRotatedText`
- Added text background color support for `wxPdfDC::DoDrawText` and `wxPdfDC::DoDrawRotatedText`

### Changed

- Adjusted dash patterns to get (hopefully) perfect dots for all pen cap styles

### Fixed

- Fixed a bug when using a protected/encrypted external template
- Fixed an issue on failing to load an image from simple XML due to missing mimetype
- Fixed an issue on parsing the document information of an existing PDF document
- Fixed a wxPdfParser issue with "startxref" not found
- Fixed a crash on importing a completely empty page
- Fixed a problem with empty content streams on import
- Fixed several glitches in XML support
- Fixed issue using wrong pen on drawing splines in method wxPdfDC::DoDrawSpline
- Fixed issues in `wxPdfDC` class by taking transparency of pen and brush into account.

## [0.9.6] - 2017-04-03

**wxPdfDocument** is compatible with wxWidgets version 2.8.12 and version 3.1.0.

### Changed

- Overhauled the build system
- Added continuous integration (Travis CI)
- Replaced all occurrences of macro `wxT` by `wxS`
- Added version information as Windows resource for DLL builds on Windows

### Fixed

- Fixed adjustment of angles in method `wxPdfDocument::Ellipse`
- Fixed issue with drawing arcs from `wxPdfDC` by forcing angles into proper range for counter-clockwise drawing
- Replaced MD5 implementation to resolve a license incompatibility issue
- Replaced image in transparency sample to resolve a potential license issue

## [0.9.5] - 2015-12-18

**wxPdfDocument** is compatible with wxWidgets version 2.8.12 and version 3.0.2.

### Changed

- respect `join` and `cap` attributes of `wxPen` in `wxPdfDC` (2.9)
- cleaned up the use of `wxMemoryOutputStream` instances
- cleaned up dependencies on wxWidgets libraries for graphics formats GIF and JPEG

### Fixed

- fixed a bug in positioning rotated text
- fixed a bug in determining line lengths in XML formatted output
- fixed a bug in drawing a point in `wxPdfDC` (2.9)
- fixed a bug in drawing elliptic arcs in `wxPdfDC` (2.9)

## [0.9.4] - 2013-08-10

**wxPdfDocument** is compatible with wxWidgets version 2.8.12 and version 2.9.5.
Compatibility with older wxWidgets versions is not guaranteed, but it should
work with all 2.8.x versions.

### Changes

- added handling of Unicode surrogates for TrueType Unicode fonts (in wxPdfDocument library and in ShowFont utility)
- added missing initialization for image format type to use in `wxPdfDC`
- added support to write `wxImage` objects in JPEG format to PDF (instead of PNG format only)

### Fixed

- corrected the handling of sectors in method wxPdfDocument::Ellipse
- fixed a bug in `ShowFont` (font index for TrueType collections was not selectable)
- fixed a bug in the Type1 font parser (parsing `/Subrs` could hang)
- fixed a bug in the Type1 font parser (endless loop when parsing Type1 multi master font files)
- fixed a bug in TrueType font parser reading wrong values from OS/2 table version 0
- adjusted default font metrics in `wxPdfDC` and external leading to always >= 0
- fixed a GDI object leak in TrueType font parser (Windows only)

## [0.9.3] - 2012-05-31

**wxPdfDocument** is compatible with wxWidgets version 2.8.12 and version 2.9.3.
Compatibility with older wxWidgets versions is not guaranteed, but it should
work with all 2.8.x versions.

### Added

- added methods to access the bottom right coordinates of the last inserted image
- added span tag to XML markup
- added method wxPdfDocument::AttachFile to attach files to PDF documents
- added compile time option to derive wxPdfDocument from wxObject (makes interfacing to wxPerl easier)
- added support for the wxWidgets printing framework (contributed by Mark Dootson)

### Changed

- integrated enhancements to `wxPdfDC` and `MakeFont` contributed by Mark Dootson
- enhanced `wxPdfDC` sample application to demonstrate the integration with the printing framework (contributed by Mark Dootson)

### Fixed

- fixed a bug in `pdffontdatacore.cpp` (non-ASCII characters didn't show on OSX)
- fixed a bug in `pdfencrypt.cpp` (setting a non-empty document id)
- fixed a bug in `pdfxml.cpp` (force line break for words too long to fit on a line)
- fixed in bug in handling external templates in conjunction with protection (crypting used the wrong object id for strings and streams)

## [0.9.2] - 2011-09-06

**wxPdfDocument** is compatible with wxWidgets version 2.8.12 and version 2.9.2.
Compatibility with older wxWidgets versions is not guaranteed, but it should
work with all 2.8.x versions.

### Added

- added method `wxPdfDC::GetPdfDocument` for builds based on wxWidgets 2.9.x
- added method `wxPdfDocument::WriteXml` for direct handling of `wxXmlNode` instances
- added support for optionally activating/deactivating message translation (`msg` tag) in XML output method
- added optional document id parameter for method `wxPdfEncrypt::GenerateEncryptionKey`

### Fixed

- fixed a bug in method `wxPdfDocument::CalculatePageSize`
- fixed a bug in `wxPdfFontDataCore::ConvertCID2GID`
- fixed several scaling bugs in `wxPdfDC`
- fixed several warnings in `pdfkernel` regarding formatted output of `size_t` variables
- fixed minor issues with Bengali font

## [0.9.1] - 2011-01-15

**wxPdfDocument** is compatible with wxWidgets version 2.8.11 and version 2.9.1.
Compatibility with older wxWidgets versions is not guaranteed, but it should
now work with all 2.8.x versions.

### Added

- added support for Apple Unicode TrueType fonts
- added the `showfont` utility

### Changes

- optimized the processing speed of VOLT rules
- modified the code for wxMac support
- modified the sample `tutorial7` to test the new wxMac font loading code
- added check for valid `cmap` table in `wxPdfFontParserTrueType`
- added call to method `wc_str` for `wxString` parameters in calls to `FromWChar`
- implemented method `RegisterSystemFonts` for wxMac
- samples changed to set the executable path as the current working directory

### Fixed

- fixed a memory leak on registering a font identified by a `wxFont` object
- fixed a bug in method `ShowGlyph`
- fixed several wxMac compile time bugs (missing includes, some typos)
- changed the wxMac print dialog includes in the printing sample

## [0.9.0] - 2010-12-29

**wxPdfDocument** is compatible with wxWidgets version 2.8.11 and version 2.9.1.
Compatibility with older wxWidgets versions is not guaranteed.

This is the first release of **wxPdfDocument** containing a **PDF drawing context** (`wxPdfDC`).
There are implementations for wxWidgets 2.8.x and 2.9.x; the matching implementation
is selected automatically at compile time. Please report your experiences with `wxPdfDC`
to the author of **wxPdfDocument**, be it bug reports, contributions or feature requests.

### Added

- methods to draw Bezier splines through a list of points;
the drawing sample has been extended to show the new functionality
- PDF drawing context (`wxPdfDC`); not yet all methods are implemented
- support for fonts with VOLT (_Visual Ordering and Layout Tables_) data
(currently visual ordering only, preprocessing of the fonts required);
fonts for 9 Indic scripts are included to demonstrate this feature

### Changes

- increased output speed for method `SaveAsFile` (if large graphics files are involved)
- all currently supported CJK font families are now registered automatically at startup of the font manager
- MS CJK fonts aren't automatically registered as Type0 fonts,
since this conflicts with registering these fonts as TrueType Unicode fonts
- handling of image masks has been improved

### Fixed

- opening font files could fail if the file path contained non-latin characters
Now `wxFileSystem::FileNameToURL` is used to create valid file names for use in method `wxFileSystem::OpenFile` 
- invalid format codes in method `wxPdfUtility::Double2String` could cause problems in _MinGW_ environment
- registering half-width CJK fonts didn't work
- bug in page size handling
- no file was written when `Close` was called before `SaveAsFile`
- bug in the handling of transparency for image masks
- uninitialized member variables in layer objects possibly causing invisibility of layers
- cleaned up output formatting codes for building on 64-bit systems
- compile time bugs for wxWidgets built with `wxUSE_STL`
- several minor bugs

## [0.8.5] - 2009-10

**wxPdfDocument** is compatible with wxWidgets version 2.8.10. Some preparations were done
to make **wxPdfDocument** compatible with version 2.9.x and above, too.

### Added (**all** builds)

- support for individual page sizes
- support for setting fill rule to _odd/even_ or _winding_
- support for setting the text render mode
- support for layers (optional content groups)
- support for patterns as draw and fill colours
- support for Code 128 barcodes

### Added (**Unicode** build)

- support for kerning
- support for different encodings for Type1 and TrueType fonts
- support for using TrueType and OpenType fonts loaded directly from `.ttf` or `.otf` file
- support for using Type1 fonts loaded directly from `.pfb` and `.afm` file
- support for using TrueType and OpenType fonts defined by a `wxFont` object
- font subsetting for OpenType Unicode fonts (experimental, currently non-CID fonts only)
- direct positioning and writing of glyph numbers for TrueType/OpenType Unicode fonts (this may be used in conjunction with tools for writing complex scripts like ICU)

### Added (**ANSI** build)

- support for fonts defined by a wxFont object (mapped by family, weight and style to the Adobe core fonts)

### Changes

- coordinate transformation (location of origin and y axis orientation)
is now done directly in PDF. This was a prerequisite to add `wxGraphicsContext` support
in an upcoming version. User unit scaling is done programmatically.
- unified the naming of all methods manipulating colours. Now always the
British spelling is used, i.e. _colour_ instead of _color_.

### Fixed

- line style measurements did not use user units
- encryption support for big endian platforms
- method `wxPdfDocument::ClippingText`

## [0.8.0] - 2006-12

**wxPdfDocument** is compatible with wxWidgets version 2.8.0 as well as with version 2.6.x.

### Added

- support for external templates: pages of existing PDF documents may be imported and used as templates
- font subsetting for TrueType and TrueType Unicode fonts, often resulting in much smaller PDF file sizes
- support for using and embedding OpenType Unicode fonts
- enhanced support for password based encryption, encryption key length freely definable between 40 and 128 bits (**Attention**: Adobe Reader supports only keys with 40 or 128 bits.)
- support for AES encryption (additional standard encryption method in PDF version 1.6 and above)

### Changes

- As an add-on preprocessed font files for the free [DejaVu fonts](http://dejavu.sourceforge.net)
(version 2.12) are provided in the file release [wxPdfDocument Add-Ons](http://sourceforge.net/project/showfiles.php?group_id=51305&package_id=45182&release_id=468705).

- For supporting font subsetting for ordinary non-Unicode TrueType fonts
the format of the font definition files has been extended. Font definition files created
with prior versions of the `makefont` utility are still usable, but do not support font subsetting.
It is recommended to regenerate own font definition files. Unfortunately common AFM font metric
files do not contain glyph information which is required by the `makefont` utility to create the
character-to-glyph mapping. Therefore the utility `ttf2ufm` had to be changed.
The modified version including a Windows executable is available in the file release [wxPdfDocument Add-Ons](http://sourceforge.net/project/showfiles.php?group_id=51305&package_id=45182&release_id=468705).

## [0.7.6] - 2006-10

**wxPdfDocument** is now compatible with wxWidgets version 2.7.1 and above as well as with version 2.6.x.

### Added

- setting/getting the default path used for loading font files
- getting the description of the current font
- loading images from a wxInputStream (in addition to loading from file or wxImage)

**Attention**: The structure of the font definition files has changed due to
the addition of the font attribute `xHeight`. It is necessary to regenerate own
font definition files using `makefont`. To support the `xHeight` attribute the utility
`ttf2ufm` had to be changed.

### Fixed

- several bugs

## [0.7.5] - 2006-09

### Added

- support for tables in simple XML markup spanning more than a page
- support for internal links in simple XML markup
- support for transparency
- support for image masks
- support for internal templates
- support for polygon and shape clipping
- support for printing text along a path

### Changed

- extended support for fill gradients
- internal colour management reworked

### Fixed

- some minor bugs

## [0.7] - 2006-04

### Added

- support for CMYK and spot colours
- support for named colours (486 predefined names for RGB colours) (`wxPdfColour`)
- support for colour names in HTML notation (`#rrggbb`) (`wxPdfColour`)
- text annotations
- additional font decorations: overline, strikeout
- PDF forms
- JavaScript at the document level
- Simple XML markup language for styling and structuring text
- `wxPdfDocument::LineCount`, `wxPdfDocument::TextBox` and several getter methods were added for convenience
- `wxPdfDocument::WriteXml` allows to print text containing simple XML markup

### Changed

- `wxPdfDocument::MultiCell` now respects a maximal line count

## [0.6] - 2005-11

### Added

- gradients
- transformations
- barcodes
- `makefont` utility

### Changed

- Changed API of graphics primitives: line style and fill colour parameters deleted,
line style and fill colour have to be set using `wxPdfDocument::SetLineStyle` and `wxPdfDocument::SetFillColour`.

## [0.5] - 2005-09

First public release

## [0.4] - 2005-08

### Added

- Support for embedding fonts

## [0.3] - 2005-07

### Added

- Support for embedding images

## [0.2] - 2005-06

### Added

- Several add-ons implemented

## [0.1] - 2005-05

### Added

- Planning and basic PDF features implemented

[Unreleased]: ../../compare/v1.3.1...HEAD
[1.3.1]: ../../compare/v1.3.0...v1.3.1
[1.3.0]: ../../compare/v1.2.1...v1.3.0
[1.2.1]: ../../compare/v1.2.0...v1.2.1
[1.2.0]: ../../compare/v1.1.0...v1.2.0
[1.1.0]: ../../compare/v1.0.3...v1.1.0
[1.0.3]: ../../compare/v1.0.2...v1.0.3
[1.0.2]: ../../compare/v1.0.1...v1.0.2
[1.0.1]: ../../compare/v1.0.0...v1.0.1
[1.0.0]: ../../compare/v0.9.8...v1.0.0
[0.9.8]: ../../compare/v0.9.7...v0.9.8
[0.9.7]: ../../compare/v0.9.6...v0.9.7
[0.9.6]: ../../compare/v0.9.5...v0.9.6
