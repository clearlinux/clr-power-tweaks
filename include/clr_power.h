#ifndef __INCLUDE_GUARD_CLR_POWER_H__
#define __INCLUDE_GUARD_CLR_POWER_H__

extern void write_int_to_file(char *filename, int value);
extern void write_string_to_file(char *filename, char *string);

extern void do_vm_tweaks(void);
extern void do_nmi_watchdog(void);
extern void do_sata_links(void);
extern void do_pci_pm(void);
extern void do_WOL(void);
#endif
