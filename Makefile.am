bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/pci.c src/ethernet.c src/usb.c src/gfx.c src/verifytime.c src/server.c src/cpu.c \ 
		    include/clr_power.h include/pci-good.h include/usb-good.h src/data.h 

AM_CPPFLAGS = -I$(top_srcdir)/include/
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

systemdsystemunitdir = @SYSTEMD_SYSTEMUNITDIR@
systemdsystemunit_DATA = clr-power.service clr-power.timer clr-power-rfkill.service

DISTCHECK_CONFIGURE_FLAGS = \
		--with-systemdsystemunitdir=$(DESTDIR)$(SYSTEMDSYSTEMUNITDIR)

EXTRA_DIST = $(systemdsystemunit_DATA) man/clr_power.1.md man/clr-power-tweaks.conf.5.md

dist_man_MANS = man/clr_power.1 man/clr-power-tweaks.conf.5

docs: man/clr_power.1 man/clr-power-tweaks.conf.5

man/clr_power.1: man/clr_power.1.md
	ronn -r $< --pipe > $@

man/clr-power-tweaks.conf.5: man/clr-power-tweaks.conf.5.md
	ronn -r $< --pipe > $@

