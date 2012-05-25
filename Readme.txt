wxPdfDocument component info
----------------------------

Website:      http://wxcode.sourceforge.net/components/wxpdfdoc
Version:      0.9.3
Description:
wxPdfDocument allows wxWidgets applications to generate PDF documents.
The code is a port of FPDF - a free PHP class for generating PDF files
- to C++ using the wxWidgets library. Several add-on PHP scripts found
on the FPDF web site are incorporated into wxPdfDocument.
Embedding of PNG, JPEG, GIF and WMF images is supported. In addition to
the 14 standard Adobe fonts it is possible to use other Type1, TrueType
or OpenType fonts - with or without embedding them into the generated
document. CJK fonts are supported, too. Graphics primitives allow the
creation of simple drawings.


Installation
------------

Starting with wxPdfDocument version 0.9.0 build support is provided
separately for wxWidgets version 2.8.x resp. 2.9.x. Subdirectory
'build' contains the build files for wxWidgets 2.8.x; subdirectory
'build29' contains the build files for wxWidgets 2.9.x.

For autoconf-based systems the root directory of the distribution
contains

- configure28, Makefile28.in for wxWidgets 2.8.x
- configure29, Makefile29.in for wxWidgets 2.9.x

Rename the files appropriate for you to 'configure' resp. 'Makefile.in'.

For makefile-based systems please inspect the makefile appropriate for
your environment. At the beginning of the makefile you find several
configurable options. Adjust them to match the wxWidgets build you
plan to build the component for.

a) wxMSW

When building on win32, you can use the makefiles or one of the
Microsoft Visual Studio solution files in the BUILD folder.

While the wxWidgets version for which the component should be compiled
can be set via the option WX_VERSION for makefiles, this doesn't work
for Visual C++ project files. You have to edit the version number in the
names of the wxWidgets libraries manually or you could recreate the project
files using bakefile setting explicitly the wxWidgets version number.

If you build wxPdfDocument as a DLL you may need to copy the DLL from the
'lib' folder to the 'samples' folder to be able to execute the sample
program. Another solution is to add the 'lib' folder path to the Windows
search path (PATH environment variable).

b) wxGTK

When building on an autoconf-based system (like Linux/GNU-based
systems), you can use the existing configure script in the component's
ROOT folder or you can recreate the configure script doing:

  cd build
  ./acregen.sh
  cd ..

  ./configure [here you should use the same flags you used to configure wxWidgets]
  make
 
Type "./configure --help" for more info.

Note: Recreating the configure script requires the following prerequisites:
- automake 1.9.6 or higher
- bakefile 0.2.5
- wxCode autoconf and bakefile files (to be downloaded from CVS or SVN)

The autoconf-based systems also support a "make install" target which
builds the library and then copies the headers of the component to
/usr/local/include and the lib to /usr/local/lib.

Executing the sample applications
---------------------------------

If you build the wxPdfDocument library as a shared object resp. DLL you
have to take different measures on different platforms to execute the
sample applications. The sample applications try to locate themselves
using relative paths, but this might not work under all circumstances.

wxMSW:
Copy the wxPdfDocument DLL to the samples' directories or change the PATH
environment variable appropriately. Set environment variable WXPDF_FONTPATH:

set WXPDF_FONTPATH=<wxPdfDocument-path>\lib\fonts

wxGTK:
Set environment variables before starting the applications, i.e.

LD_LIBRARY_PATH=<wxPdfDocument>/lib:$LD_LIBRARY_PATH WXPDF_FONTPATH=<wxPdfDocument>/lib/fonts ./application

wxMAC:
You should set environment variable WXPDF_FONTPATH:

WXPDF_FONTPATH=<wxPdfDocument>/lib/fonts

If wxPdfDocument has been built as a shared object you have to make it accessible,
for example by copying it to /usr/local/lib.

Acknowledgements
----------------

I'm very grateful to Bruno Lowagie, the main author of the iText Java library
(http://www.lowagie.com/iText), for allowing to take lots of ideas and
inspirations from this great Java PDF library. Especially the font handling
and font subsetting was influenced in that way.

Many thanks go to Ben Moores who provided code for layers and patterns he
wrote for his PDF extension for Mapnik (http://www.mapnik.org). This code
has been extended based on ideas from the iText Java library and was
incorporated into wxPdfDocument.

Support for Indic scripts is based on the efforts of Ian Back, creator of
the PHP library mPDF (http://mpdf.bpm1.com); special thanks to K Vinod Kumar
of the Centre for Development of Advanced Computing, Mumbai
(http://www.cdacmumbai.in), for clearing license issues of the Raghu font
series.

Kudos to Mark Dootson for contributing major enhancements of wxPdfDC and
it's integration into the wxWidgets printing framework.

Since wxPdfDocument is based on the great FPDF PHP class and several of the
contributions to it found on the FPDF website (http://www.fpdf.org) I would
like to thank 

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

Known bugs
----------

None

Please send bug reports, feature requests, suggestions etc. to the author.


Authors' info
-------------

Ulrich Telle   utelle@users.sourceforge.net
