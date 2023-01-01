AC_INIT([simple-snake], [0.1])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_CONFIG_FILES([
				 Makefile 
				 src/Makefile
				 ])
AX_WITH_CURSES
if test "x$ax_cv_curses" != xyes; then
	AC_MSG_ERROR([requires an X/Open-compatible Curses library with color])
fi
AC_OUTPUT
