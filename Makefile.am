EXTRA_DIST = COPYING clr-power.service clr-power-vm.service

bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/vm.c src/watchdog.c src/sata.c src/pci.c src/ethernet.c src/usb.c


systemdsystemunitdir = @SYSTEMDSYSTEMUNITDIR@
systemdsystemunit_DATA = \
        clr-power.service \
        clr-power-vm.service

AM_CPPFLAGS = -I$(top_srcdir)/include/

noinst_HEADERS = include/clr_power.h
