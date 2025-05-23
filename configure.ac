dnl Process this script with autoconf to create configure for wxpdfdoc library
dnl
dnl Copyright (C) 2018-2025 Ulrich Telle <github@telle-online.de> 
dnl Copyright (C) 2017 Vadim Zeitlin <vadim@wxwidgets.org>
dnl
dnl This file is covered by the same licence as the entire wxpdfdoc package.

AC_INIT([wxpdfdoc], [1.3.1], [github@telle-online.de])

dnl This is the version tested with, might work with earlier ones.
AC_PREREQ([2.69])

AC_CONFIG_SRCDIR([include/wx/pdfdoc.h])
AC_CONFIG_AUX_DIR([admin/build-aux])
AC_CONFIG_MACRO_DIR([admin/m4])

AM_INIT_AUTOMAKE([1.11 foreign subdir-objects])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

AC_ARG_ENABLE(samples,
             [AS_HELP_STRING([--disable-samples],
                             [don't build the samples])],
             [case "x${enableval}" in
                   x) ;;
                xyes) BUILD_SAMPLES=1 ;;
                 xno) ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-samples) ;;
             esac],
             [BUILD_SAMPLES=1])
AM_CONDITIONAL([BUILD_SAMPLES], [test -n "$BUILD_SAMPLES"])

AC_ARG_ENABLE(utilities,
             [AS_HELP_STRING([--disable-utilities],
                             [don't build the utilities])],
             [case "x${enableval}" in
                   x) ;;
                xyes) BUILD_UTILITIES=1 ;;
                 xno) ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-utilities) ;;
             esac],
             [BUILD_UTILITIES=1])
AM_CONDITIONAL([BUILD_UTILITIES], [test -n "$BUILD_UTILITIES"])

AC_ARG_WITH([zint-builtin],
    [AS_HELP_STRING([--with-zint-builtin],
        [Use builtin Zint library])],
    [],
    [with_zint_builtin=no])

AC_ARG_WITH([woff2-builtin],
    [AS_HELP_STRING([--with-woff2-builtin],
        [Use builtin WOFF2 library])],
    [],
    [with_woff2_builtin=no])

dnl Detect the target system

AC_CANONICAL_HOST

build_linux=no
build_windows=no
build_mac=no

case "${host_os}" in
    linux*)
        build_linux=yes
        ;;
    cygwin*|mingw*)
        build_windows=yes
        ;;
    darwin*)
        build_mac=yes
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac

if [test "x$build_mac" = xyes]; then
CFLAGS="-I$(brew --prefix)/include $CFLAGS"
CPPFLAGS="-I$(brew --prefix)/include $CPPFLAGS"
LDFLAGS="-L$(brew --prefix)/lib $LDFLAGS"
fi

dnl Check for Zint
haveZint=false
AC_CHECK_HEADERS(zint.h, [
  AC_SEARCH_LIBS(ZBarcode_Version, zint, [haveZint=true], [haveZint=false])
], [haveZint=false])

dnl Check for Woff2
haveWoff2=false
AC_LANG(C++)
dnl AC_CHECK_HEADERS(woff2/decode.h, [
dnl   AC_SEARCH_LIBS([BrotliDecoderDecompress],[woff2dec], [haveWoff2=true], [haveWoff2=false])
dnl ], [haveWoff2=false])

AC_LANG_PUSH([C++])

AC_MSG_CHECKING([for libwoff2dec])
SAVE_LIBS="$LIBS"
LIBS="$LIBS -lwoff2dec"
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
     [[#include <woff2/decode.h>]],
     [[woff2::ConvertWOFF2ToTTF(nullptr, 0, nullptr, 0);]]
   )],
  [haveWoff2=true],
  [haveWoff2=false]
)
LIBS="$SAVE_LIBS"
AC_LANG_POP([C++])

dnl Allow adding MSW-specific fragments to the makefile.
case "${host}" in
    *-*-cygwin* )
        USE_MSW=1
        ;;

    *-*-mingw32* )
        USE_MSW=1
        ;;
esac
AM_CONDITIONAL(USE_MSW, [test -n "$USE_MSW"])

if test -n "$USE_MSW"; then
    AC_CHECK_TOOL([WINDRES], [windres], no)
fi

dnl Pass the conditionals to automake
AM_CONDITIONAL([HOST_LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([HOST_WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([HOST_OSX], [test "$build_mac" = "yes"])

AM_CONDITIONAL([HAVE_ZINT], [test x$haveZint = xtrue && test "x$with_zint_builtin" = xno])
AM_CONDITIONAL([HAVE_WOFF2], [test x$haveWoff2 = xtrue && test "x$with_woff2_builtin" = xno])

LT_INIT([win32-dll])
AM_CONDITIONAL([USE_SHARED], [test "x$enable_shared" = xyes])

AC_PROG_CXX
AC_LANG(C++)

AC_ARG_VAR(WX_CONFIG, [Path to wx-config script (default: search in PATH).])
WX_CONFIG_OPTIONS
WX_CONFIG_CHECK([3.0.0],
    [],
    [AC_MSG_FAILURE([wxWidgets required but not detected.])],
    [base,core,xml]
)

if test -n "$BUILD_SAMPLES"; then

dnl We only need the libraries above for the main library itself, but the
dnl pdfdc sample has additional requirements, check for them too (notice that
dnl we can't use "--optional-libs" wx-config option to do it all in one check
dnl for as long as we support 2.8 in which wx-config doesn't have this option
dnl yet).
core_WX_LIBS=$WX_LIBS
WX_CONFIG_CHECK([3.0.0],
    [WX_LIBS_PDFDC_SAMPLE=$WX_LIBS],
    [AC_MSG_WARN([Some wxWidgets libraries not available, pdfdc sample won't be built])],
    [adv,base,core,html,richtext,xml]
)
WX_LIBS=$core_WX_LIBS

fi

AM_CONDITIONAL([BUILD_PDFDC_SAMPLE], [test -n "$WX_LIBS_PDFDC_SAMPLE"])

dnl Check if the system has support for ELF visibility (this is a waste of time
dnl when targeting MSW, so don't do it in this case).
if test -z "$USE_MSW"; then
    CXXFLAGS_VISIBILITY="-fvisibility=hidden -fvisibility-inlines-hidden"
    AC_CACHE_CHECK([for visibility support],
        [wxpdfdoc_cv_cxx_visibility],
        [CXXFLAGS_ORIG="$CXXFLAGS"
        CXXFLAGS="$CXXFLAGS $CXXFLAGS_VISIBILITY"
        AC_COMPILE_IFELSE(
            [AC_LANG_SOURCE([class __attribute__((__visibility__("default"))) Foo {
               Foo() {}
             };])],
            [wxpdfdoc_cv_cxx_visibility=yes],
            [wxpdfdoc_cv_cxx_visibility=no]
        )
        CXXFLAGS=$CXXFLAGS_ORIG]
    )
    if test "$wxpdfdoc_cv_cxx_visibility" = yes; then
        CXXFLAGS="$CXXFLAGS $CXXFLAGS_VISIBILITY"
        AC_DEFINE([WXPDFDOC_HAVE_VISIBILITY])
    fi
fi

dnl With wxGTK, we also need Fontconfig library, see src/pdffontmanager.cpp.
case "$WX_CXXFLAGS" in
    *-D__WXGTK__* )
        USE_GTK=1
        ;;
esac

if test -n "$USE_GTK"; then
    PKG_PROG_PKG_CONFIG()
    PKG_CHECK_MODULES(FONTCONFIG, fontconfig,
                      [CXXFLAGS="$CXXFLAGS $FONTCONFIG_CFLAGS"
                       LIBS="$LIBS $FONTCONFIG_LIBS"],
                      [AC_MSG_FAILURE(Required Fontconfig library not detected.)])
fi

dnl This is needed by WX_LIKE_LIBNAME
WX_DETECT_STANDARD_OPTION_VALUES
dnl This macro is used to preserve the same name as was used with the previous
dnl build systems only.
WX_LIKE_LIBNAME([WXPDFDOC_LIBNAME], [wxcode], [pdfdoc])

AC_SUBST(WXPDFDOC_LIBNAME)
AC_SUBST(WX_LIBS_PDFDC_SAMPLE)

dnl Ensure that the samples can find the files they need when running from the
dnl build directory.
AC_CONFIG_LINKS([samples/pdfdc/smile.jpg:samples/pdfdc/smile.jpg
    samples/pdfdc/up.gif:samples/pdfdc/up.gif
    samples/minimal/20k_c1.txt:samples/minimal/20k_c1.txt
    samples/minimal/20k_c2.txt:samples/minimal/20k_c2.txt
    samples/minimal/attached.txt:samples/minimal/attached.txt
    samples/minimal/apple.gif:samples/minimal/apple.gif
    samples/minimal/chart2d.pdf:samples/minimal/chart2d.pdf
    samples/minimal/circle.png:samples/minimal/circle.png
    samples/minimal/clips.jpg:samples/minimal/clips.jpg
    samples/minimal/countries.txt:samples/minimal/countries.txt
    samples/minimal/flower.jpg:samples/minimal/flower.jpg
    samples/minimal/glasses.png:samples/minimal/glasses.png
    samples/minimal/glyphsample.txt:samples/minimal/glyphsample.txt
    samples/minimal/image.png:samples/minimal/image.png
    samples/minimal/image_with_alpha.png:samples/minimal/image_with_alpha.png
    samples/minimal/linear_gradient_coords.jpg:samples/minimal/linear_gradient_coords.jpg
    samples/minimal/logo.png:samples/minimal/logo.png
    samples/minimal/mask.png:samples/minimal/mask.png
    samples/minimal/pattern1.png:samples/minimal/pattern1.png
    samples/minimal/pattern2.png:samples/minimal/pattern2.png
    samples/minimal/radial_gradient_coords.jpg:samples/minimal/radial_gradient_coords.jpg
    samples/minimal/ringmaster.wmf:samples/minimal/ringmaster.wmf
    samples/minimal/smile.jpg:samples/minimal/smile.jpg
    samples/minimal/wxpdfdoc.png:samples/minimal/wxpdfdoc.png
    samples/minimal/indic-assamese.txt:samples/minimal/indic-assamese.txt
    samples/minimal/indic-bengali.txt:samples/minimal/indic-bengali.txt
    samples/minimal/indic-gujarati.txt:samples/minimal/indic-gujarati.txt
    samples/minimal/indic-hindi.txt:samples/minimal/indic-hindi.txt
    samples/minimal/indic-kannada.txt:samples/minimal/indic-kannada.txt
    samples/minimal/indic-malayalam.txt:samples/minimal/indic-malayalam.txt
    samples/minimal/indic-nepali.txt:samples/minimal/indic-nepali.txt
    samples/minimal/indic-oriya.txt:samples/minimal/indic-oriya.txt
    samples/minimal/indic-punjabi.txt:samples/minimal/indic-punjabi.txt
    samples/minimal/indic-tamil.txt:samples/minimal/indic-tamil.txt
    samples/minimal/indic-telugu.txt:samples/minimal/indic-telugu.txt
])

ZINT_LIBS=
if [test x$haveZint = xtrue && test "x$with_zint_builtin" = xno]; then
ZINT_LIBS=-lzint
fi
AC_SUBST(ZINT_LIBS)

WOFF2_LIBS=
if [test x$haveWoff2 = xtrue && test "x$with_woff2_builtin" = xno]; then
WOFF2_LIBS=-lwoff2dec
fi
AC_SUBST(WOFF2_LIBS)

AC_CONFIG_FILES([Makefile wxpdfdoc.pc])
AC_OUTPUT
