AC_INIT([droidimg], [0.1], [g.herfray@gahfy.io])
AM_INIT_AUTOMAKE([-W -Wall subdir-objects])
ac_abs_confdir=`(
    cd "$srcdir" && test -r "./$ac_unique_file" || as_fn_error $? "$ac_msg"
    pwd)`
AC_SUBST(SRCDIR, "$ac_abs_confdir/src")
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

PKG_CHECK_MODULES([LIBPNG], [libpng])
PKG_CHECK_MODULES([LIBWEBP], [libwebp])

LIBS="$LIBPNG_LIBS $LIBS"
CFLAGS="$LIBPNG_CFLAGS $CFLAGS"

dnl ============ Checking for library presence (header and function)

AC_CHECK_HEADER(
    png.h,
    [
        AC_CHECK_LIB(
            png,
            png_get_libpng_ver,
            ,
            [AC_MSG_ERROR([Did not find libpng library. Please install libpng and/or set LIBPNG_LIBS var.])]
        )
    ],
    [
        AC_MSG_ERROR([Did not find libpng headers. Please install libpng and/or set LIBPNG_CFLAGS var.])
    ]
)

LIBS="$LIBWEBP_LIBS $LIBS"
CFLAGS="$LIBWEBP_CFLAGS $CFLAGS"

AC_CHECK_HEADER(
    webp/encode.h,
    [
        AC_CHECK_LIB(
            webp,
            WebPGetEncoderVersion,
            ,
            [AC_MSG_ERROR([Did not find libwebp library. Please install libwebp and/or set LIBWEBP_LIBS var.])])
    ],
    [
        AC_MSG_ERROR([Did not find libwebp headers. Please install libwebp and/or set LIBWEB_CFLAGS var.])
    ]
)

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
 tests/app_tests/Makefile
 tests/errors/Makefile
 tests/files/Makefile
 tests/logging/Makefile
 tests/pictures/Makefile
])

AC_OUTPUT