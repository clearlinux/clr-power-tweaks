#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([clr-power-tweaks],[211],[dev@lists.clearlinux.org])
AM_INIT_AUTOMAKE([foreign -Wall -Wextra-portability subdir-objects])
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIRS([m4])

AC_LANG(C)

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h sys/socket.h unistd.h time.h argp.h glob.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset socket strstr strdup strerror strtoul strtoull clock_gettime])


AC_CONFIG_HEADERS([config.h])

PKG_CHECK_MODULES([SYSTEMD], [systemd])

AC_ARG_WITH([systemdsystemunitdir], AS_HELP_STRING([--with-systemdsystemunitdir=DIR],[path to systemd system service directory]), [path_systemdsystemunit=${withval}],
	[path_systemdsystemunit="`$PKG_CONFIG --variable=systemdsystemunitdir systemd`"])
if (test -n "${path_systemdsystemunit}"); then
SYSTEMD_SYSTEMUNITDIR="${path_systemdsystemunit}"
AC_SUBST(SYSTEMD_SYSTEMUNITDIR)
fi

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
