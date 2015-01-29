/*
 *    Clear Linux Project for Intel Architecture Power tweaks 
 *
 *      Copyright (C) 2012 Intel Corporation
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, version 3 of the License.
 *  
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Authors:
 *         Arjan van de Ven <arjan@linux.intel.com>
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <clr_power.h>

#include <sys/types.h>
#include <dirent.h>

/*
 * From external/pciutils
 */
#include <pci/pci.h>

struct pci_ids {
	int vendor;
	int device;
	void (*tweaks_cb)(struct pci_ids *id, struct pci_access *pacc);
};

void enable_aspm(struct pci_dev *dev, int pos);
void lynx_point_tweaks(struct pci_ids *id, struct pci_access *pacc);
void wilkins_peak_tweaks(struct pci_ids *id, struct pci_access *pacc);

/*
 * This structure requires some maintenance on it.  Specifically the host
 * controllers need to be listed before any of the devices.  This allows 
 * the clear tweaks to operate on a host controller first then a device for 
 * enabling config space changes (for example).  
 */
static struct pci_ids known_good[] =
{
/*
 * Host Controllers 
 */
{ 0x1b73, 0x1009, NULL}, /* Fresco Logic xHCI Controller */
{ 0x8086, 0x0104, NULL}, /* 2nd Gen Core DRAM Controller */
{ 0x8086, 0x0116, NULL}, /* 2nd Gen Core Integrated Graphics Processor */
{ 0x8086, 0x0153, NULL}, /* 3rd Gen Core Thermal Subsystem */
{ 0x8086, 0x0154, NULL}, /* 3rd Gen Core DRAM Controller */
{ 0x8086, 0x0166, NULL}, /* 3rd Gen Core Graphics Controller */
{ 0x8086, 0x0a04, NULL}, /* HSW-U DRAM Controller */
{ 0x8086, 0x0a0c, NULL}, /* HSW-U HD Audio Controller */
{ 0x8086, 0x1c03, NULL}, /* 6Series SATA AHCI Controller */
{ 0x8086, 0x1c10, NULL}, /* 6Series PCI Express Root Port 1 */
{ 0x8086, 0x1c12, NULL}, /* 6Series PCI Express Root Port 2 */
{ 0x8086, 0x1c16, NULL}, /* 6Series PCI Express Root Port 4 */
{ 0x8086, 0x1c20, NULL}, /* 6Series HD Audio Controller */
{ 0x8086, 0x1c22, NULL}, /* 6Series SMBus Controller */
{ 0x8086, 0x1c26, NULL}, /* 6Series USB Enhanced Host Controller 1 */
{ 0x8086, 0x1c3a, NULL}, /* 6Series MEI Controller */
{ 0x8086, 0x1c4d, NULL}, /* QS67 Express LPC Controller */
{ 0x8086, 0x1e03, NULL}, /* 7Series SATA Controller AHCI */
{ 0x8086, 0x1e10, NULL}, /* 7Series PCI Express Root Port 1 */
{ 0x8086, 0x1e12, NULL}, /* 7Series PCI Express Root Port 2 */
{ 0x8086, 0x1e20, NULL}, /* 7Series HD Audio Controller */
{ 0x8086, 0x1e22, NULL}, /* 7Series SMBus Controller */
{ 0x8086, 0x1e24, NULL}, /* 7Series Thermal Management Controller */
{ 0x8086, 0x1e26, NULL}, /* 7Series USB Enhanced Host Controller 1 */
{ 0x8086, 0x1e2d, NULL}, /* 7Series USB Enhanced Host Controller 2 */
{ 0x8086, 0x1e3a, NULL}, /* 7Series MEI Controller */
{ 0x8086, 0x1e59, NULL}, /* HM76 Express Chipset LPC Controller */
{ 0x8086, 0x2921, NULL}, /* ICH9 SATA Controller IDE */
{ 0x8086, 0x2934, NULL}, /* ICH9 USB UCHI Controller 1 */
{ 0x8086, 0x2935, NULL}, /* ICH9 USB UCHI Controller 2 */
{ 0x8086, 0x2936, NULL}, /* ICH9 USB UHCI Controller 3 */
{ 0x8086, 0x2937, NULL}, /* ICH9 USB UCHI Controller 4 */
{ 0x8086, 0x2938, NULL}, /* ICH9 USB UHCI Controller 5 */
{ 0x8086, 0x2939, NULL}, /* ICH9 USB UHCI Controller 6 */
{ 0x8086, 0x293a, NULL}, /* ICH9 USB2 ECHI Controller */
{ 0x8086, 0x293c, NULL}, /* ICH9 HD Audio Controller */
{ 0x8086, 0x3406, NULL}, /* 5520 IO Hub to ESI Port */
{ 0x8086, 0x3408, NULL}, /* 5520 IO Hub PCI Express Root Port 1 */
{ 0x8086, 0x340a, NULL}, /* 5520 IO Hub PCI Express Root Port 3 */
{ 0x8086, 0x340b, NULL}, /* 5520 IO Hub PCI Express Root Port 4 */
{ 0x8086, 0x340c, NULL}, /* 5520 IO Hub PCI Express Root Port 5 */
{ 0x8086, 0x340e, NULL}, /* 5520 IO Hub PCI Express Root Port 7 */
{ 0x8086, 0x3410, NULL}, /* 7500 IO Hub PCI Express Root Port 9 */
{ 0x8086, 0x3411, NULL}, /* 7500 IO Hub PCI Express Root Port 10 */
{ 0x8086, 0x9c03, NULL}, /* LPT-LP SATA Contoller (AHCI */
{ 0x8086, 0x9c10, NULL}, /* LPT-LP PCI Express Root Port 1 */
{ 0x8086, 0x9c14, lynx_point_tweaks}, /* LPT-LP PCI Express Root Port 3 */
{ 0x8086, 0x9c20, NULL}, /* LPT-LP HD Audio Controller */
{ 0x8086, 0x9c22, NULL}, /* LPT-LP SMBus Controller */
{ 0x8086, 0x9c26, NULL}, /* LPT-LP USB EHCI */
{ 0x8086, 0x9c31, NULL}, /* LPT-LP USB xHCI HC */
{ 0x8086, 0x9c3a, NULL}, /* LPT-LP HECI 0 */
{ 0x8086, 0x9c43, NULL}, /* LPC Controller */
{ 0x8086, 0x0f00, NULL}, /* VLV SSA-CUnit */
{ 0x8086, 0x0f18, NULL}, /* VLV SEC */
{ 0x8086, 0x0f1c, NULL}, /* VLV PCU */
{ 0x8086, 0x0f31, NULL}, /* VLV PCI Generation 7 */
{ 0x8086, 0x0f35, NULL}, /* VLV USB xHCI Host Controller */
{ 0x8086, 0x0a26, NULL}, /* Haswell-ULT Integrated Graphics Controller */
/*
 * Devices
 */
{ 0x8086, 0x088e, NULL}, /* Centrino Advanced-N 6235 */
{ 0x8086, 0x1559, NULL}, /* Intel tehernet I218-U */
{ 0x8086, 0x08b1, wilkins_peak_tweaks}, /* Wireless 7260 */
{ 0x168c, 0x0032, NULL}, /* AR9580 Wireless Adapter */
{ 0x14e4, 0x1639, NULL}, /* Broadcom BCM5709 Gigabit Ethernet */

{0, 0, NULL} };

void enable_aspm(struct pci_dev *dev, int pos)
{
	int rc = 0;
	u8 aspm = 0x0;
	aspm = pci_read_byte(dev, pos);

	if (aspm == 0x40) {
		/*
		 * 0x40 = no APSM
		 * 0x41 = L0s only
		 * 0x42 = L1
		 * 0x43 = L0s and L1
		 * Enable only L1 and not L0s for now
		 */
		aspm = 0x42;
		rc = pci_write_byte(dev, pos, aspm);
	}
	return;
}

void lynx_point_tweaks(struct pci_ids *id, struct pci_access *pacc)
{
	struct pci_dev *rp = NULL;

	for (rp = pacc->devices; rp; rp = rp->next) {
		if (rp->vendor_id == id->vendor && 
		    rp->device_id == id->device) {
			/*
			 * Byte 0x50 is the ASPM bit for Intel's
			 * LynxPoint-LP root port .  
			 */
			enable_aspm(rp, 0x50);
			break;
		}
	}

	return;
}


void wilkins_peak_tweaks(struct pci_ids *id, struct pci_access *pacc)
{
	struct pci_dev *dev  = NULL;

	for (dev = pacc->devices; dev; dev = dev->next) {
		if (dev->vendor_id == id->vendor && 
		    dev->device_id == id->device) {
			/*
			 * Byte 0x50 is the ASPM bit for Intel's
			 * Wilkins Peak card.  
			 */
			enable_aspm(dev, 0x50);
			break;
		}
	}

	return;
}

void do_pci_pm(void)
{
	DIR *dir;
	struct dirent *entry;
	struct pci_access *pacc = NULL;

	dir = opendir("/sys/bus/pci/devices/");
	if (!dir)
		return;

	pacc = pci_alloc();
	pci_init(pacc);
	pci_scan_bus(pacc);

	do {
		FILE *file;
		char *filename;
		char *line;
		size_t size;
		int vendor, device;
		int i;

		entry = readdir(dir);
		if (!entry)
			break;
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;

		if (asprintf(&filename, "/sys/bus/pci/devices/%s/vendor", entry->d_name) < 0)
			assert(0);

		file = fopen(filename, "r");
		if (!file) {
			free(filename);
			continue;
		}
		line = NULL;
		size = 0;
		if (getline(&line, &size, file) < 0)
			assert(0);
		vendor = strtoull(line, NULL, 16);
		free(line);
		free(filename);
		fclose(file);

		if (asprintf(&filename, "/sys/bus/pci/devices/%s/device", entry->d_name) < 0)
			assert(0);

		file = fopen(filename, "r");
		if (!file) {
			free(filename);
			continue;
		}
		line = NULL;
		size = 0;
		if (getline(&line, &size, file) < 0)
			assert(0);
		device = strtoull(line, NULL, 16);
		free(line);
		free(filename);
		fclose(file);

//		printf("Found PCI device %04x:%04x\n", vendor, device);
		i = 0;
		while (1) {
			if (known_good[i].vendor == 0)
				break;
			if (known_good[i].vendor == vendor && known_good[i].device == device) {
				if (asprintf(&filename, "/sys/bus/pci/devices/%s/power/control", entry->d_name) < 0)
                       		 	assert(0);
				write_string_to_file(filename, "auto");
				free(filename);
				if (known_good[i].tweaks_cb)
					(*known_good[i].tweaks_cb)(&known_good[i], pacc);
			}
			i++;
		}



	} while (1);

	pci_cleanup(pacc);
	closedir(dir);
}
