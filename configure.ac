# Process this file with autoconf to produce a configure script.
AC_INIT([libdebug],[1.0],[martin@blom.org])
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CANONICAL_TARGET([])
AC_PROG_MAKE_SET

: ${CPPFLAGS="-W -Wall -Wstrict-prototypes"}
: ${CFLAGS="-O2 -fomit-frame-pointer"}

# Checks for programs.
AC_CHECK_PROGS( SFDC, SFDC sfdc ) 
AC_CHECK_TOOL( CC, gcc, : ) 
AC_CHECK_TOOL( AR, ar, : )
AC_CHECK_TOOL( RANLIB, ranlib, : )
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_INSTALL
	 
# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_ARFLAGS
AC_C_CONST
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_COMMANDS([exec_dbg_inline.h],
	           [${SFDC} --target=$host --mode=macros \
		    ${srcdir}/exec_dbg_lib.sfd \
		    -o exec_dbg_inline.h],
		   [SFDC=${SFDC} host=${host}])
AC_CONFIG_FILES([Makefile],
	        [echo timestamp > stamp-h])
AC_OUTPUT
