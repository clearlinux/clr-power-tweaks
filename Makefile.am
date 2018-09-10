bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/pci.c src/ethernet.c src/usb.c src/gfx.c generated.c

systemdsystemunit_DATA = clr-power.service

generated.c: clr-power-tweaks.conf $(top_srcdir)/generate-tweaks.pl
	$(top_srcdir)/generate-tweaks.pl $< > $@

AM_CPPFLAGS = -I$(top_srcdir)/include/

noinst_HEADERS = include/clr_power.h
noinst_SCRIPTS = generate-tweaks.pl
noinst_DATA = clr-power-tweaks.conf

EXTRA_DIST = COPYING $(systemdsystemunit_DATA)
