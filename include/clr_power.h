#include <stdbool.h>

#ifndef __INCLUDE_GUARD_CLR_POWER_H__
#define __INCLUDE_GUARD_CLR_POWER_H__

struct pci_ids {
	unsigned int vendor;
	unsigned int device;
};

struct usb_dev {
	unsigned int vendor;
	unsigned int device;
	unsigned int class;
};

#define CLASS(x) {0,0,x},
#define DEVICE(v,d) {v,d,0},

extern void lib_init(const bool dbg);
extern int write_string_to_file(const char *filename, const char *string);
extern int write_string_to_files(const char *glob, const char *string);
extern void do_pci_pm(void);
extern void do_WOL(void);
extern void do_usb_pm(void);
extern void do_gfx_pm(void);
extern void verify_time(void);
#endif
