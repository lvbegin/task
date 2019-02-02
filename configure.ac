#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([task], [1.0], [laurent.vanbegin76@gmail.com])
: ${CXXFLAGS=""}
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_PROG_RANLIB

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
# Checks for libraries.
AM_INIT_AUTOMAKE()
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AX_CHECK_LINK_FLAG(-pthread, AC_SUBST([PTHREAD], [-pthread]), AC_SUBST([PTHREAD],[])) 
AX_CHECK_LINK_FLAG(-O3, AC_SUBST([DEFAULTCXXFLAGS], [-O3]), AC_SUBST([DEFAULTCXXFLAGS], [])) 

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
