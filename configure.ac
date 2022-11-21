AC_INIT([droidimg], [0.01], [g.herfray@gahfy.io])
AM_INIT_AUTOMAKE([-W -Wall subdir-objects])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

AC_ARG_WITH([png-dir],
        [AS_HELP_STRING([--with-png-dir=path     path to libpng install dir])],
        [PNG_INSTALL_DIR=$withval],
        [])
AC_ARG_WITH([webp-dir],
        [AS_HELP_STRING([--with-webp-dir=path    path to webp install dir])],
        [WEBP_INSTALL_DIR=$withval],
        [])

dnl ============ Checking for library arguments

AS_IF([test "x$PNG_INSTALL_DIR" = "x"], , [
    CFLAGS="-I$PNG_INSTALL_DIR/include ${CFLAGS}"
    LIBS="-L$PNG_INSTALL_DIR/lib $LIBS"
])

AS_IF([test "x$WEBP_INSTALL_DIR" = "x"], , [
    CFLAGS="-I$WEBP_INSTALL_DIR/include ${CFLAGS}"
    LIBS="-L$WEBP_INSTALL_DIR/lib $LIBS"
])

dnl ============ Checking for library presence (header and function)

AC_CHECK_HEADER(png.h, [
    AC_CHECK_LIB(png, png_create_read_struct, , [AC_MSG_ERROR([Did not find libpng library. Please install libpng and/or set --with-png option to point to libpng $LIBS])])
    LIBS="$LIBS -lpng"
], [AC_MSG_ERROR([Did not find libpng headers. Please install libpng and/or set --with-png option to point to libpng])])
AC_CHECK_HEADER(webp/encode.h, [
    AC_CHECK_LIB(webp, WebPDecodeRGB, , [AC_MSG_ERROR([Did not find libwebp library. Please install libwebp and/or set --with-webp option to point to libwebp])])
    LIBS="$LIBS -lwebp"
], [AC_MSG_ERROR([Did not find libwebp headers. Please install libwebp and/or set --with-png option to point to libwebp])])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_OUTPUT