dnl Process this script with autoconf to create configure for wxpdfdoc library
dnl
dnl Copyright (C) 2017 Vadim Zeitlin <vadim@wxwidgets.org>
dnl
dnl This file is covered by the same licence as the entire wxpdfdoc package.

AC_INIT([wxpdfdoc], [0.9.5], [utelle@users.sourceforge.net])

dnl This is the version tested with, might work with earlier ones.
AC_PREREQ([2.69])

AC_CONFIG_SRCDIR([include/wx/pdfdoc.h])
AC_CONFIG_AUX_DIR([admin/build-aux])
AC_CONFIG_MACRO_DIR([admin/m4])

AM_INIT_AUTOMAKE([1.11 foreign subdir-objects])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

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

LT_INIT([win32-dll])
AM_CONDITIONAL([USE_SHARED], [test "x$enable_shared" = xyes])

AC_PROG_CXX
AC_LANG(C++)

AC_ARG_VAR(WX_CONFIG, [Path to wx-config script (default: search in PATH).])
WX_CONFIG_OPTIONS
WX_CONFIG_CHECK([2.8.0],
    [],
    [AC_MSG_FAILURE([wxWidgets required but not detected.])],
    [base,core,xml]
)

dnl We only need the libraries above for the main library itself, but the
dnl pdfdc sample has additional requirements, check for them too (notice that
dnl we can't use "--optional-libs" wx-config option to do it all in one check
dnl for as long as we support 2.8 in which wx-config doesn't have this option
dnl yet).
core_WX_LIBS=$WX_LIBS
WX_CONFIG_CHECK([2.8.0],
    [WX_LIBS_PDFDC_SAMPLE=$WX_LIBS],
    [AC_MSG_WARN([Some wxWidgets libraries not available, pdfdc sample won't be built])],
    [adv,base,core,html,richtext,xml]
)
WX_LIBS=$core_WX_LIBS
AM_CONDITIONAL([BUILD_PDFDC_SAMPLE], [test -n "$WX_LIBS_PDFDC_SAMPLE"])

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
    samples/minimal/smile.jpg:samples/minimal/smile.jpg
    samples/minimal/wxpdfdoc.png:samples/minimal/wxpdfdoc.png
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
