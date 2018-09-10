#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT(clr-power-tweaks, 175, arjan@linux.intel.com)
AM_INIT_AUTOMAKE([foreign -Wall -Wextra subdir-objects])
AM_SILENT_RULES([yes])
AC_PROG_CC
AC_LANG(C)
AC_CONFIG_HEADERS([config.h])
PKG_CHECK_MODULES([SYSTEMD], [systemd])
systemdsystemunitdir="`$PKG_CONFIG --variable=systemdsystemunitdir systemd`"
AC_SUBST(systemdsystemunitdir)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
