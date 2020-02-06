bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/pci.c src/ethernet.c src/usb.c src/gfx.c src/verifytime.c \
		    include/clr_power.h include/pci-good.h include/usb-good.h src/data.h

AM_CPPFLAGS = -I$(top_srcdir)/include/
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

systemunitdir = $(SYSTEMD_SYSTEMUNITDIR)
systemunit_DATA = clr-power.service clr-power.timer clr-power-rfkill.service

EXTRA_DIST = $(systemunit_DATA)
