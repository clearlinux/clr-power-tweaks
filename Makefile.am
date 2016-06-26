bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/pci.c src/ethernet.c src/usb.c src/gfx.c

systemdsystemunit_DATA = clr-power.service

tmpfiles_DATA = clr-power-tweaks.conf

AM_CPPFLAGS = -I$(top_srcdir)/include/

noinst_HEADERS = include/clr_power.h

EXTRA_DIST = COPYING $(systemdsystemunit_DATA) $(tmpfiles_DATA)
