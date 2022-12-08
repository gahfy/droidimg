AC_INIT([droidimg], [0.1], [g.herfray@gahfy.io])
AM_INIT_AUTOMAKE([-W -Wall subdir-objects])
ac_abs_confdir=`(
    cd "$srcdir" && test -r "./$ac_unique_file" || as_fn_error $? "$ac_msg"
    pwd)`
AC_SUBST(SRCDIR, "$ac_abs_confdir/src")
AC_CONFIG_HEADERS([config.h])

AC_ARG_ENABLE([png],
  [AS_HELP_STRING([--enable-png[[=yes|no]] | --disable-png], [Enable/Disable conversion from PNG files])],,
  [enable_png=maybe])

PKG_CHECK_MODULES([LIBWEBP], [libwebp])

dnl ============ Checking for library presence (header and function)

AS_IF([test "$enable_png" != no], [
    PKG_CHECK_MODULES([LIBPNG], [libpng], [], [AC_MSG_WARN([pkg-config did not find libpng])])
    LIBS="$LIBPNG_LIBS $LIBS"
    CFLAGS="$LIBPNG_CFLAGS $CFLAGS"
    AC_CHECK_HEADER(
        png.h,
        [
            AC_CHECK_LIB(
                png,
                png_get_libpng_ver,
                ,
                AS_IF([test "$enable_png" = maybe], [
                    AC_MSG_WARN([Did not find libpng library. PNG support will be disabled])
                ], [
                    AC_MSG_ERROR([Did not find libpng library. Please install libpng and/or set LIBPNG_LIBS var.])
                ])
            )
        ],
        [
            AS_IF([test "$enable_png" = maybe], [
                AC_MSG_WARN([Did not find libpng headers. PNG support will be disabled])
            ], [
                AC_MSG_ERROR([Did not find libpng headers. Please install libpng and/or set LIBPNG_CFLAGS var.])
            ])
        ]
    )
],[
    AC_MSG_NOTICE([Not testing libpng as feature is disabled])
])

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
 tests/webp/Makefile
 tests/png/Makefile
])

AC_OUTPUT