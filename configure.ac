AC_INIT([simple-snake], [0.1])
AC_CONFIG_SRCDIR([src/draw.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([m], [log10])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([mkdir])

AC_CONFIG_FILES([Makefile 
				 src/Makefile])

# Additional Macro Checks
AX_WITH_CURSES
if test "x$ax_cv_curses" != xyes; then
	AC_MSG_ERROR([requires an X/Open-compatible Curses library with color])
fi

AC_OUTPUT
