# Process this file with automake to create Makefile.in for wxpdfdoc library
#
# Copyright (C) 2017 Vadim Zeitlin <vadim@wxwidgets.org>
#
# This file is covered by the same licence as the entire wxpdfdoc package.

ACLOCAL_AMFLAGS = -I admin/m4

PDFDOC_LIBS=
ZINT_SRCS=
ZINT_HDRS = \
  thirdparty/zint/backend/fonts/normal_ttf.h \
  thirdparty/zint/backend/fonts/upcean_ttf.h
ZINT_CFLAGS = -I$(top_srcdir)/thirdparty/zint/backend/fonts
ZINT_CXXFLAGS = -I$(top_srcdir)/thirdparty/zint/backend/fonts
WOFF_SRCS=
WOFF_HDRS=
WOFF_CFLAGS=
WOFF_CXXFLAGS=

if HAVE_ZINT
$(info HAVE_ZINT is defined.)
PDFDOC_LIBS += -lzint
else
$(info HAVE_ZINT is NOT defined.)
ZINT_SRCS += \
  thirdparty/zint/backend/2of5.c \
  thirdparty/zint/backend/2of5inter.c \
  thirdparty/zint/backend/2of5inter_based.c \
  thirdparty/zint/backend/auspost.c \
  thirdparty/zint/backend/aztec.c \
  thirdparty/zint/backend/bc412.c \
  thirdparty/zint/backend/bmp.c \
  thirdparty/zint/backend/channel.c \
  thirdparty/zint/backend/codabar.c \
  thirdparty/zint/backend/codablock.c \
  thirdparty/zint/backend/code.c \
  thirdparty/zint/backend/code1.c \
  thirdparty/zint/backend/code11.c \
  thirdparty/zint/backend/code128.c \
  thirdparty/zint/backend/code128_based.c \
  thirdparty/zint/backend/code16k.c \
  thirdparty/zint/backend/code49.c \
  thirdparty/zint/backend/common.c \
  thirdparty/zint/backend/composite.c \
  thirdparty/zint/backend/dllversion.c \
  thirdparty/zint/backend/dmatrix.c \
  thirdparty/zint/backend/dotcode.c \
  thirdparty/zint/backend/dxfilmedge.c \
  thirdparty/zint/backend/eci.c \
  thirdparty/zint/backend/emf.c \
  thirdparty/zint/backend/filemem.c \
  thirdparty/zint/backend/general_field.c \
  thirdparty/zint/backend/gif.c \
  thirdparty/zint/backend/gridmtx.c \
  thirdparty/zint/backend/gs1.c \
  thirdparty/zint/backend/hanxin.c \
  thirdparty/zint/backend/imail.c \
  thirdparty/zint/backend/large.c \
  thirdparty/zint/backend/library.c \
  thirdparty/zint/backend/mailmark.c \
  thirdparty/zint/backend/maxicode.c \
  thirdparty/zint/backend/medical.c \
  thirdparty/zint/backend/output.c \
  thirdparty/zint/backend/pcx.c \
  thirdparty/zint/backend/pdf417.c \
  thirdparty/zint/backend/plessey.c \
  thirdparty/zint/backend/png.c \
  thirdparty/zint/backend/postal.c \
  thirdparty/zint/backend/ps.c \
  thirdparty/zint/backend/qr.c \
  thirdparty/zint/backend/raster.c \
  thirdparty/zint/backend/reedsol.c \
  thirdparty/zint/backend/rss.c \
  thirdparty/zint/backend/svg.c \
  thirdparty/zint/backend/telepen.c \
  thirdparty/zint/backend/tif.c \
  thirdparty/zint/backend/ultra.c \
  thirdparty/zint/backend/upcean.c \
  thirdparty/zint/backend/vector.c
ZINT_HDRS += \
  thirdparty/zint/backend/aztec.h \
  thirdparty/zint/backend/big5.h \
  thirdparty/zint/backend/bmp.h \
  thirdparty/zint/backend/channel_precalcs.h \
  thirdparty/zint/backend/code1.h \
  thirdparty/zint/backend/code128.h \
  thirdparty/zint/backend/code49.h \
  thirdparty/zint/backend/common.h \
  thirdparty/zint/backend/composite.h \
  thirdparty/zint/backend/dmatrix.h \
  thirdparty/zint/backend/dmatrix_trace.h \
  thirdparty/zint/backend/eci.h \
  thirdparty/zint/backend/eci_sb.h \
  thirdparty/zint/backend/emf.h \
  thirdparty/zint/backend/filemem.h \
  thirdparty/zint/backend/gb18030.h \
  thirdparty/zint/backend/gb2312.h \
  thirdparty/zint/backend/gbk.h \
  thirdparty/zint/backend/general_field.h \
  thirdparty/zint/backend/gridmtx.h \
  thirdparty/zint/backend/gs1.h \
  thirdparty/zint/backend/gs1_lint.h \
  thirdparty/zint/backend/hanxin.h \
  thirdparty/zint/backend/iso3166.h \
  thirdparty/zint/backend/iso4217.h \
  thirdparty/zint/backend/ksx1001.h \
  thirdparty/zint/backend/large.h \
  thirdparty/zint/backend/maxicode.h \
  thirdparty/zint/backend/output.h \
  thirdparty/zint/backend/pcx.h \
  thirdparty/zint/backend/pdf417.h \
  thirdparty/zint/backend/pdf417_tabs.h \
  thirdparty/zint/backend/pdf417_trace.h \
  thirdparty/zint/backend/qr.h \
  thirdparty/zint/backend/raster_font.h \
  thirdparty/zint/backend/reedsol.h \
  thirdparty/zint/backend/reedsol_logs.h \
  thirdparty/zint/backend/rss.h \
  thirdparty/zint/backend/sjis.h \
  thirdparty/zint/backend/tif.h \
  thirdparty/zint/backend/tif_lzw.h \
  thirdparty/zint/backend/zfiletypes.h \
  thirdparty/zint/backend/zint.h \
  thirdparty/zint/backend/zintconfig.h
ZINT_CFLAGS += -I$(top_srcdir)/thirdparty/zint/backend -DZINT_NO_PNG
ZINT_CXXFLAGS += -I$(top_srcdir)/thirdparty/zint/backend -DZINT_NO_PNG
endif

if HAVE_WOFF2
$(info HAVE_WOFF2 is defined.)
PDFDOC_LIBS += -lwoff2dec
else
$(info HAVE_WOFF2 is NOT defined.)
WOFF_SRCS += \
  thirdparty/woff2/src/woff2_dec.cc \
  thirdparty/woff2/src/woff2_common.cc \
  thirdparty/woff2/src/woff2_out.cc \
  thirdparty/woff2/src/table_tags.cc \
  thirdparty/woff2/src/variable_length.cc \
  thirdparty/woff2/brotli/dec/bit_reader.c \
  thirdparty/woff2/brotli/dec/decode.c \
  thirdparty/woff2/brotli/dec/huffman.c \
  thirdparty/woff2/brotli/dec/state.c \
  thirdparty/woff2/brotli/common/constants.c \
  thirdparty/woff2/brotli/common/context.c \
  thirdparty/woff2/brotli/common/dictionary.c \
  thirdparty/woff2/brotli/common/platform.c \
  thirdparty/woff2/brotli/common/shared_dictionary.c \
  thirdparty/woff2/brotli/common/transform.c
WOFF_HDRS += \
  thirdparty/woff2/src/buffer.h \
  thirdparty/woff2/src/port.h \
  thirdparty/woff2/src/round.h \
  thirdparty/woff2/src/store_bytes.h \
  thirdparty/woff2/src/table_tags.h \
  thirdparty/woff2/src/variable_length.h \
  thirdparty/woff2/src/woff2_common.h \
  thirdparty/woff2/brotli/dec/bit_reader.h \
  thirdparty/woff2/brotli/dec/huffman.h \
  thirdparty/woff2/brotli/dec/prefix.h \
  thirdparty/woff2/brotli/dec/state.h \
  thirdparty/woff2/brotli/common/constants.h \
  thirdparty/woff2/brotli/common/context.h \
  thirdparty/woff2/brotli/common/dictionary.h \
  thirdparty/woff2/brotli/common/platform.h \
  thirdparty/woff2/brotli/common/shared_dictionary_internal.h \
  thirdparty/woff2/brotli/common/transform.h \
  thirdparty/woff2/brotli/common/version.h
WOFF_CFLAGS += \
  -I$(top_srcdir)/thirdparty/woff2/include \
  -I$(top_srcdir)/thirdparty/woff2/src \
  -I$(top_srcdir)/thirdparty/woff2/brotli/include
WOFF_CXXFLAGS += \
  -I$(top_srcdir)/thirdparty/woff2/include \
  -I$(top_srcdir)/thirdparty/woff2/src \
  -I$(top_srcdir)/thirdparty/woff2/brotli/include
endif

if USE_MSW
MSW_LIBS = -lgdi32
endif

# Flags used for compiling all the targets and linking all the executables
# (libraries use LIBADD which is set for lib@WXPDFDOC_LIBNAME@.la only).
AM_CFLAGS = -I$(top_srcdir)/include $(WOFF_CFLAGS) $(ZINT_CFLAGS) -std=c99
AM_CXXFLAGS = -I$(top_srcdir)/include $(WX_CXXFLAGS) $(WOFF_CXXFLAGS) $(ZINT_CXXFLAGS) -std=c++11
LDADD = lib@WXPDFDOC_LIBNAME@.la $(WX_LIBS) $(PDFDOC_LIBS)

if HOST_OSX
AM_LDFLAGS = -framework Security
endif

# Target-specific assignments are GNU Make-specific, but there doesn't seem to
# be any other way to define different compilation options when building
# shared and static libraries with libtool.
if USE_SHARED
%.lo: AM_CXXFLAGS += -DWXMAKINGDLL_PDFDOC
%.o: AM_CXXFLAGS += -DWXUSINGDLL_PDFDOC
else
%.lo: AM_CXXFLAGS += -DWXMAKINGLIB_PDFDOC
%.o: AM_CXXFLAGS += -DWXUSINGLIB_PDFDOC
endif

if USE_MSW
# Use old style suffix rule instead of the pattern one as automake only
# recognizes this kind of rules.
#
# Also notice that we use .res_o_o extension for the object files obtained by
# compiling resource files, instead of just the usual .o, because we'd have
# conflicts between object file names for foo.cpp and foo.rc otherwise. Due to
# this we have to add the .res_o files manually to LDADD later, but such is life.
.rc.res_o:
	$(AM_V_GEN) $(WINDRES) -I $(top_srcdir) $(WX_CPPFLAGS) -O coff $< $@
endif

# Define the directory where the library headers are installed.
includewxdir = $(includedir)/wx

# Ensure our pkg-config file is installed in the correct location.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = wxpdfdoc.pc

# The main library. Notice that it uses dynamic, i.e. determined when running
# configure, name because it depends on the version of wxWidgets used. This is
# rather unusual, but Automake doesn't seem to mind.
lib_LTLIBRARIES = lib@WXPDFDOC_LIBNAME@.la

lib@WXPDFDOC_LIBNAME@_la_SOURCES = \
    src/pdfannotation.cpp \
    src/pdfbarcode.cpp \
    src/pdfbarcodezint.cpp \
    src/pdfcffdecoder.cpp \
    src/pdfcffindex.cpp \
    src/pdfcolour.cpp \
    src/pdfdc.cpp \
    src/pdfdecode.cpp \
    src/pdfdocument.cpp \
    src/pdfencoding.cpp \
    src/pdfencrypt.cpp \
    src/pdffont.cpp \
    src/pdffontdata.cpp \
    src/pdffontdatacore.cpp \
    src/pdffontdataopentype.cpp \
    src/pdffontdatatruetype.cpp \
    src/pdffontdatatype0.cpp \
    src/pdffontdatatype1.cpp \
    src/pdffontdescription.cpp \
    src/pdffontdetails.cpp \
    src/pdffontextended.cpp \
    src/pdffontmanager.cpp \
    src/pdffontparser.cpp \
    src/pdffontparsertruetype.cpp \
    src/pdffontparsertype1.cpp \
    src/pdffontsubsetcff.cpp \
    src/pdffontsubsettruetype.cpp \
    src/pdffontvolt.cpp \
    src/pdfform.cpp \
    src/pdfgradient.cpp \
    src/pdfgraphics.cpp \
    src/pdfimage.cpp \
    src/pdfkernel.cpp \
    src/pdflayer.cpp \
    src/pdfobjects.cpp \
    src/pdfocg.cpp \
    src/pdfparser.cpp \
    src/pdfpattern.cpp \
    src/pdfprint.cpp \
    src/pdfrijndael.cpp \
    src/pdftemplate.cpp \
    src/pdfutility.cpp \
    src/pdfxml.cpp \
    src/crypto/random.cpp \
    src/crypto/saslprep.cpp \
    src/crypto/sha256.cpp \
    src/crypto/sha384.cpp \
    src/crypto/sha512.cpp \
    src/crypto/unicode_norm.cpp \
    src/woff/woff2converter.cpp \
    src/woff/woffconverter.cpp

includewx_HEADERS = \
    include/wx/pdfannotation.h \
    include/wx/pdfarraydouble.h \
    include/wx/pdfarraytypes.h \
    include/wx/pdfbarcode.h \
    include/wx/pdfbarcodezint.h \
    include/wx/pdfbookmark.h \
    include/wx/pdfcffdecoder.h \
    include/wx/pdfcffindex.h \
    include/wx/pdfcjkfontdata.h \
    include/wx/pdfcolour.h \
    include/wx/pdfcoonspatchmesh.h \
    include/wx/pdfcorefontdata.h \
    include/wx/pdfdc.h \
    include/wx/pdfdoc.h \
    include/wx/pdfdoc_version.h \
    include/wx/pdfdocdef.h \
    include/wx/pdfdocument.h \
    include/wx/pdfencoding.h \
    include/wx/pdfencrypt.h \
    include/wx/pdffont.h \
    include/wx/pdffontdata.h \
    include/wx/pdffontdatacore.h \
    include/wx/pdffontdataopentype.h \
    include/wx/pdffontdatatruetype.h \
    include/wx/pdffontdatatype0.h \
    include/wx/pdffontdatatype1.h \
    include/wx/pdffontdescription.h \
    include/wx/pdffontdetails.h \
    include/wx/pdffontextended.h \
    include/wx/pdffontmacosx.h \
    include/wx/pdffontmanager.h \
    include/wx/pdffontparser.h \
    include/wx/pdffontparsertruetype.h \
    include/wx/pdffontparsertype1.h \
    include/wx/pdffontsubsetcff.h \
    include/wx/pdffontsubsettruetype.h \
    include/wx/pdffontvolt.h \
    include/wx/pdfform.h \
    include/wx/pdfgradient.h \
    include/wx/pdfgraphics.h \
    include/wx/pdfimage.h \
    include/wx/pdfinfo.h \
    include/wx/pdflayer.h \
    include/wx/pdflinestyle.h \
    include/wx/pdflinks.h \
    include/wx/pdfobjects.h \
    include/wx/pdfparser.h \
    include/wx/pdfpattern.h \
    include/wx/pdfprint.h \
    include/wx/pdfproperties.h \
    include/wx/pdfrijndael.h \
    include/wx/pdfshape.h \
    include/wx/pdfspotcolour.h \
    include/wx/pdftemplate.h \
    include/wx/pdfutility.h \
    include/wx/pdfxml.h

noinst_HEADERS = \
    src/pdfcjkfontdata.inc \
    src/pdfcmapdata.inc \
    src/pdfcodepagedata.inc \
    src/pdfcolourdata.inc \
    src/pdfcorefontdata.inc \
    src/pdfcorefontkerning.inc \
    src/pdfglyphnames.inc \
    src/crypto/random.h \
    src/crypto/saslprep.h \
    src/crypto/sha256.h \
    src/crypto/sha384.h \
    src/crypto/sha512.h \
    src/crypto/unicode_norm.h \
    src/crypto/unicode_norm_table.h \
    src/woff/woff2converter.h \
    src/woff/woffconverter.h

if !HAVE_ZINT
$(info HAVE_ZINT NOT defined - Add Zint sources and headers.)
lib@WXPDFDOC_LIBNAME@_la_SOURCES += $(ZINT_SRCS)
noinst_HEADERS += $(ZINT_HDRS) 
endif

if !HAVE_WOFF2
$(info HAVE_WOFF2 NOT defined - Add WOFF2 sources and headers.)
lib@WXPDFDOC_LIBNAME@_la_SOURCES += $(WOFF_SRCS)
noinst_HEADERS += $(WOFF_HDRS) 
endif

lib@WXPDFDOC_LIBNAME@_la_LDFLAGS = -no-undefined $(AM_LDFLAGS)
if USE_SHARED
lib@WXPDFDOC_LIBNAME@_la_LIBADD = $(WX_LIBS) $(MSW_LIBS) $(PDFDOC_LIBS)
endif


noinst_PROGRAMS=
CLEANFILES=

if BUILD_SAMPLES
# Samples (don't need to be installed).
noinst_PROGRAMS += samples/minimal/minimal

if BUILD_PDFDC_SAMPLE
noinst_PROGRAMS += samples/pdfdc/pdfdc
endif

samples_minimal_minimal_SOURCES = \
    samples/minimal/attachment.cpp \
    samples/minimal/barcodes.cpp \
    samples/minimal/bookmark.cpp \
    samples/minimal/charting.cpp \
    samples/minimal/cjktest.cpp \
    samples/minimal/clipping.cpp \
    samples/minimal/drawing.cpp \
    samples/minimal/glyphwriting.cpp \
    samples/minimal/gradients.cpp \
    samples/minimal/indicfonts.cpp \
    samples/minimal/jsform.cpp \
    samples/minimal/kerning.cpp \
    samples/minimal/labels.cpp \
    samples/minimal/layers.cpp \
    samples/minimal/minimal.cpp \
    samples/minimal/protection.cpp \
    samples/minimal/rotation.cpp \
    samples/minimal/templates.cpp \
    samples/minimal/transformation.cpp \
    samples/minimal/transparency.cpp \
    samples/minimal/tutorial1.cpp \
    samples/minimal/tutorial2.cpp \
    samples/minimal/tutorial3.cpp \
    samples/minimal/tutorial4.cpp \
    samples/minimal/tutorial5.cpp \
    samples/minimal/tutorial6.cpp \
    samples/minimal/tutorial7.cpp \
    samples/minimal/wmf.cpp \
    samples/minimal/xmlwrite.cpp

samples_minimal_minimal_LDADD = $(LDADD)

if USE_MSW
samples_minimal_minimal_SOURCES += samples/minimal/minimal.rc
samples_minimal_minimal_LDADD += samples/minimal/minimal.res_o
CLEANFILES += samples/minimal/minimal.res_o
else
# libtool complains about unknown "-no-install" option when targetting MSW, so
# use it only in the "else" branch.
samples_minimal_minimal_LDFLAGS = -no-install
endif

samples_pdfdc_pdfdc_SOURCES = \
    samples/pdfdc/printing.cpp \
    samples/pdfdc/printing.h

samples_pdfdc_pdfdc_LDADD = $(WX_LIBS_PDFDC_SAMPLE) $(LDADD)

if USE_MSW
samples_pdfdc_pdfdc_SOURCES += samples/pdfdc/printing.rc
samples_pdfdc_pdfdc_LDADD += samples/pdfdc/printing.res_o
CLEANFILES += samples/pdfdc/printing.res_o
else
samples_pdfdc_pdfdc_LDFLAGS = -no-install
endif

endif

if BUILD_UTILITIES

# Utilities (currently not installed neither, but could be).
noinst_PROGRAMS += makefont/makefont showfont/showfont

makefont_makefont_SOURCES = makefont/makefont.cpp

makefont_makefont_LDADD = $(LDADD)

if USE_MSW
makefont_makefont_SOURCES += makefont/makefont.rc
makefont_makefont_LDADD += makefont/makefont.res_o
CLEANFILES += makefont/makefont.res_o
endif

showfont_showfont_SOURCES = \
    showfont/showfont.cpp \
    showfont/unicodeblocks.h \
    showfont/unicoderanges.h

showfont_showfont_LDADD = $(LDADD)

if USE_MSW
showfont_showfont_SOURCES += showfont/showfont.rc
showfont_showfont_LDADD += showfont/showfont.res_o
CLEANFILES += showfont/showfont.res_o
endif

endif
