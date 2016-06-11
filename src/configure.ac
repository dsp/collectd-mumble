AC_PREREQ([2.69])
AC_INIT([collectd-mumbe], 1.0, [dsp@php.net])
AC_LANG(C++)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
LT_INIT

AX_CHECK_COMPILE_FLAG([-std=c++14], [CXXFLAGS="$CXXFLAGS -std=c++14"])
AC_CONFIG_SRCDIR([plugin.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lIce':
AC_CHECK_LIB([Ice], [main])
# FIXME: Replace `main' with a function in `-lIceUtil':
AC_CHECK_LIB([IceUtil], [main])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDC
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT32_T

# Checks for header files.
AC_CHECK_HEADERS([Ice/Ice.h], [], [], [])
AC_CHECK_HEADERS([endian.h], [], [], [])
AC_CHECK_HEADERS([oconfig.h],[], [AC_MSG_ERROR([oconfig.h not found])], [ #include <stdbool.h> ])
AC_CHECK_HEADERS([collectd.h],[], [AC_MSG_ERROR([collectd.h not found])], [ #include <stdbool.h> ])

# Checks for library functions.
AC_CHECK_FUNCS([strtol])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
