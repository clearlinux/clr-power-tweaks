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

struct pci_ids {
	int vendor;
	int device;
};

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
{ 0x1b73, 0x1009}, /* Fresco Logic xHCI Controller */
{ 0x8086, 0x0104}, /* 2nd Gen Core DRAM Controller */
{ 0x8086, 0x0116}, /* 2nd Gen Core Integrated Graphics Processor */
{ 0x8086, 0x0153}, /* 3rd Gen Core Thermal Subsystem */
{ 0x8086, 0x0154}, /* 3rd Gen Core DRAM Controller */
{ 0x8086, 0x0166}, /* 3rd Gen Core Graphics Controller */
{ 0x8086, 0x0a04}, /* HSW-U DRAM Controller */
{ 0x8086, 0x0a0c}, /* HSW-U HD Audio Controller */
{ 0x8086, 0x15a3}, /* Ethernet controller: Intel Corporation Ethernet Connection (3) I218-V */
{ 0x8086, 0x1604}, /* Host bridge: Intel Corporation Broadwell-U Host Bridge -OPI */
{ 0x8086, 0x160c}, /* Audio device: Intel Corporation Broadwell-U Audio Controller */
{ 0x8086, 0x162b}, /* VGA compatible controller: Intel Corporation Broadwell-U Integrated Graphics */
{ 0x8086, 0x1c03}, /* 6Series SATA AHCI Controller */
{ 0x8086, 0x1c10}, /* 6Series PCI Express Root Port 1 */
{ 0x8086, 0x1c12}, /* 6Series PCI Express Root Port 2 */
{ 0x8086, 0x1c16}, /* 6Series PCI Express Root Port 4 */
{ 0x8086, 0x1c20}, /* 6Series HD Audio Controller */
{ 0x8086, 0x1c22}, /* 6Series SMBus Controller */
{ 0x8086, 0x1c26}, /* 6Series USB Enhanced Host Controller 1 */
{ 0x8086, 0x1c3a}, /* 6Series MEI Controller */
{ 0x8086, 0x1c4d}, /* QS67 Express LPC Controller */
{ 0x8086, 0x1e03}, /* 7Series SATA Controller AHCI */
{ 0x8086, 0x1e10}, /* 7Series PCI Express Root Port 1 */
{ 0x8086, 0x1e12}, /* 7Series PCI Express Root Port 2 */
{ 0x8086, 0x1e20}, /* 7Series HD Audio Controller */
{ 0x8086, 0x1e22}, /* 7Series SMBus Controller */
{ 0x8086, 0x1e24}, /* 7Series Thermal Management Controller */
{ 0x8086, 0x1e26}, /* 7Series USB Enhanced Host Controller 1 */
{ 0x8086, 0x1e2d}, /* 7Series USB Enhanced Host Controller 2 */
{ 0x8086, 0x1e3a}, /* 7Series MEI Controller */
{ 0x8086, 0x1e59}, /* HM76 Express Chipset LPC Controller */
{ 0x8086, 0x2921}, /* ICH9 SATA Controller IDE */
{ 0x8086, 0x2934}, /* ICH9 USB UCHI Controller 1 */
{ 0x8086, 0x2935}, /* ICH9 USB UCHI Controller 2 */
{ 0x8086, 0x2936}, /* ICH9 USB UHCI Controller 3 */
{ 0x8086, 0x2937}, /* ICH9 USB UCHI Controller 4 */
{ 0x8086, 0x2938}, /* ICH9 USB UHCI Controller 5 */
{ 0x8086, 0x2939}, /* ICH9 USB UHCI Controller 6 */
{ 0x8086, 0x293a}, /* ICH9 USB2 ECHI Controller */
{ 0x8086, 0x293c}, /* ICH9 HD Audio Controller */
{ 0x8086, 0x3406}, /* 5520 IO Hub to ESI Port */
{ 0x8086, 0x3408}, /* 5520 IO Hub PCI Express Root Port 1 */
{ 0x8086, 0x340a}, /* 5520 IO Hub PCI Express Root Port 3 */
{ 0x8086, 0x340b}, /* 5520 IO Hub PCI Express Root Port 4 */
{ 0x8086, 0x340c}, /* 5520 IO Hub PCI Express Root Port 5 */
{ 0x8086, 0x340e}, /* 5520 IO Hub PCI Express Root Port 7 */
{ 0x8086, 0x3410}, /* 7500 IO Hub PCI Express Root Port 9 */
{ 0x8086, 0x3411}, /* 7500 IO Hub PCI Express Root Port 10 */
{ 0x8086, 0x9c03}, /* LPT-LP SATA Contoller (AHCI */
{ 0x8086, 0x9c10}, /* LPT-LP PCI Express Root Port 1 */
{ 0x8086, 0x9c20}, /* LPT-LP HD Audio Controller */
{ 0x8086, 0x9c22}, /* LPT-LP SMBus Controller */
{ 0x8086, 0x9c26}, /* LPT-LP USB EHCI */
{ 0x8086, 0x9c31}, /* LPT-LP USB xHCI HC */
{ 0x8086, 0x9c3a}, /* LPT-LP HECI 0 */
{ 0x8086, 0x9c43}, /* LPC Controller */
{ 0x8086, 0x9c5a}, /* Network controller: Intel Corporation Wireless 7265 (rev 59) */
{ 0x8086, 0x9c90}, /* PCI bridge: Intel Corporation Wildcat Point-LP PCI Express Root Port #1 */
{ 0x8086, 0x9c96}, /* PCI bridge: Intel Corporation Wildcat Point-LP PCI Express Root Port #4 */
{ 0x8086, 0x9c83}, /* SATA controller: Intel Corporation Wildcat Point-LP SATA Controller [AHCI Mode]  */
{ 0x8086, 0x9ca0}, /* Audio device: Intel Corporation Wildcat Point-LP High Definition Audio Controller */
{ 0x8086, 0x9ca2}, /* SMBus: Intel Corporation Wildcat Point-LP SMBus Controller (rev 03) */
{ 0x8086, 0x9ca6}, /* USB controller: Intel Corporation Wildcat Point-LP USB EHCI Controller */
{ 0x8086, 0x9cba}, /* Communication controller: Intel Corporation Wildcat Point-LP MEI Controller #1 */
{ 0x8086, 0x9cc3}, /* ISA bridge: Intel Corporation Wildcat Point-LP LPC Controller */
{ 0x8086, 0x0f00}, /* VLV SSA-CUnit */
{ 0x8086, 0x0f18}, /* VLV SEC */
{ 0x8086, 0x0f1c}, /* VLV PCU */
{ 0x8086, 0x0f31}, /* VLV PCI Generation 7 */
{ 0x8086, 0x0f35}, /* VLV USB xHCI Host Controller */
{ 0x8086, 0x0a26}, /* Haswell-ULT Integrated Graphics Controller */
/*
 * Devices
 */
{ 0x8086, 0x088e}, /* Centrino Advanced-N 6235 */
{ 0x8086, 0x1559}, /* Intel tehernet I218-U */
{ 0x168c, 0x0032}, /* AR9580 Wireless Adapter */
{ 0x14e4, 0x1639}, /* Broadcom BCM5709 Gigabit Ethernet */

{0, 0, NULL} };

void do_pci_pm(void)
{
	DIR *dir;
	struct dirent *entry;

	dir = opendir("/sys/bus/pci/devices/");
	if (!dir)
		return;

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
			}
			i++;
		}



	} while (1);

	closedir(dir);
}
