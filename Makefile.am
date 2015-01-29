EXTRA_DIST = COPYING clr-power.service

bin_PROGRAMS = clr_power
clr_power_SOURCES = src/main.c src/lib.c src/vm.c src/watchdog.c src/sata.c src/pci.c src/ethernet.c src/usb.c

clr_power_LDADD= -lpci


systemdsystemunitdir = @SYSTEMDSYSTEMUNITDIR@
systemdsystemunit_DATA = \
        clr-power.service
        
AM_CPPFLAGS = -Iinclude/

noinst_HEADERS = include/clr_power.h
