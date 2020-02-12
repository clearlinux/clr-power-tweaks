bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/pci.c src/ethernet.c src/usb.c src/gfx.c src/verifytime.c \
		    include/clr_power.h include/pci-good.h include/usb-good.h src/data.h

AM_CPPFLAGS = -I$(top_srcdir)/include/
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

systemdsystemunitdir = @SYSTEMD_SYSTEMUNITDIR@
systemdsystemunit_DATA = clr-power.service clr-power.timer clr-power-rfkill.service

DISTCHECK_CONFIGURE_FLAGS = \
		--with-systemdsystemunitdir=$(DESTDIR)$(SYSTEMDSYSTEMUNITDIR)

EXTRA_DIST = $(systemdsystemunit_DATA) clr_power.1.md clr-power-tweaks.conf.5.md

dist_man_MANS = clr_power.1 clr-power-tweaks.conf.5

docs: clr_power.1 clr-power-tweaks.conf.5

clr_power.1: clr_power.1.md
	ronn -r $< --pipe > $@

clr-power-tweaks.conf.5: clr-power-tweaks.conf.5.md
	ronn -r $< --pipe > $@

