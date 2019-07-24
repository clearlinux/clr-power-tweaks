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
{ 0x8086, 0x0a26}, /* Haswell-ULT Integrated Graphics Controller */
{ 0x8086, 0x0f00}, /* VLV SSA-CUnit */
{ 0x8086, 0x0f18}, /* VLV SEC */
{ 0x8086, 0x0f1c}, /* VLV PCU */
{ 0x8086, 0x0f31}, /* VLV PCI Generation 7 */
{ 0x8086, 0x0f35}, /* VLV USB xHCI Host Controller */
{ 0x8086, 0x1570}, /* Intel Corporation Ethernet Connection I219-V */
{ 0x8086, 0x15a3}, /* Ethernet controller: Intel Corporation Ethernet Connection (3) I218-V */
//{ 0x8086, 0x15d8}, /* Ethernet controller: Intel Corporation Ethernet Connection (3) I218-V */
{ 0x8086, 0x1604}, /* Host bridge: Intel Corporation Broadwell-U Host Bridge -OPI */
{ 0x8086, 0x160c}, /* Audio device: Intel Corporation Broadwell-U Audio Controller */
{ 0x8086, 0x162b}, /* VGA compatible controller: Intel Corporation Broadwell-U Integrated Graphics */
{ 0x8086, 0x1904}, /* Intel Corporation Skylake Host Bridge/DRAM Registers */
{ 0x8086, 0x1911}, /* System peripheral: Intel Corporation Skylake Gaussian Mixture Model */
{ 0x8086, 0x1926}, /* Intel Corporation Skylake Integrated Graphics */
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
{ 0x8086, 0x24f3}, /* Intel Corporation Wireless 8260 */
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
{ 0x8086, 0x5926}, /* 7500 IO Hub PCI Express Root Port 10 */
{ 0x8086, 0x9c03}, /* LPT-LP SATA Contoller (AHCI */
{ 0x8086, 0x9c10}, /* LPT-LP PCI Express Root Port 1 */
{ 0x8086, 0x9c20}, /* LPT-LP HD Audio Controller */
{ 0x8086, 0x9c22}, /* LPT-LP SMBus Controller */
{ 0x8086, 0x9c26}, /* LPT-LP USB EHCI */
{ 0x8086, 0x9c31}, /* LPT-LP USB xHCI HC */
{ 0x8086, 0x9c3a}, /* LPT-LP HECI 0 */
{ 0x8086, 0x9c43}, /* LPC Controller */
{ 0x8086, 0x9c5a}, /* Network controller: Intel Corporation Wireless 7265 (rev 59) */
{ 0x8086, 0x9c83}, /* SATA controller: Intel Corporation Wildcat Point-LP SATA Controller {AHCI Mode}  */
{ 0x8086, 0x9c90}, /* PCI bridge: Intel Corporation Wildcat Point-LP PCI Express Root Port #1 */
{ 0x8086, 0x9c96}, /* PCI bridge: Intel Corporation Wildcat Point-LP PCI Express Root Port #4 */
{ 0x8086, 0x9ca0}, /* Audio device: Intel Corporation Wildcat Point-LP High Definition Audio Controller */
{ 0x8086, 0x9ca2}, /* SMBus: Intel Corporation Wildcat Point-LP SMBus Controller (rev 03) */
{ 0x8086, 0x9ca6}, /* USB controller: Intel Corporation Wildcat Point-LP USB EHCI Controller */
{ 0x8086, 0x9cba}, /* Communication controller: Intel Corporation Wildcat Point-LP MEI Controller #1 */
{ 0x8086, 0x9cc3}, /* ISA bridge: Intel Corporation Wildcat Point-LP LPC Controller */
{ 0x8086, 0x9d03}, /* Intel Corporation Sunrise Point-LP SATA Controller {AHCI mode} */
{ 0x8086, 0x9d14}, /* Intel Corporation Sunrise Point-LP PCI Express Root Port #5 */
{ 0x8086, 0x9d21}, /* Intel Corporation Sunrise Point-LP PMC */
{ 0x8086, 0x9d23}, /* Intel Corporation Sunrise Point-LP SMBus */
{ 0x8086, 0x9d27}, /* Intel Corporation Sunrise Point-LP Serial IO UART Controller #0 */
{ 0x8086, 0x9d2d}, /* Intel Corporation Sunrise Point-LP Secure Digital IO Controller */
{ 0x8086, 0x9d2f}, /* Intel Corporation Sunrise Point-LP USB 3.0 xHCI Controller */
{ 0x8086, 0x9d31}, /* Intel Corporation Sunrise Point-LP Thermal subsystem */
{ 0x8086, 0x9d3a}, /* Intel Corporation Sunrise Point-LP CSME HECI #1 */
{ 0x8086, 0x9d48}, /* Intel Corporation Sunrise Point-LP LPC Controller */
{ 0x8086, 0x9d4e}, /* Intel Corporation Sunrise Point-LP LPC Controller */
{ 0x8086, 0x9d70}, /* Intel Corporation Sunrise Point-LP HD Audio */
{ 0x8086, 0xa13a}, /* Communication controller: Intel Corporation Sunrise Point-H CSME HECI #1  */
{ 0x8086, 0xa110}, /* PCI bridge: Intel Corporation Sunrise Point-H PCI Express Root Port #1 */
{ 0x8086, 0xa111}, /* PCI bridge: Intel Corporation Sunrise Point-H PCI Express Root Port #1 */
{ 0x8086, 0xa112}, /* PCI bridge: Intel Corporation Sunrise Point-H PCI Express Root Port #1 */
{ 0x8086, 0xa114}, /* PCI bridge: Intel Corporation Sunrise Point-H PCI Express Root Port #1 */
{ 0x8086, 0xa118}, /* PCI bridge: Intel Corporation Sunrise Point-H PCI Express Root Port #1 */
{ 0x8086, 0xa14e}, /* ISA bridge: Intel Corporation Sunrise Point-H LPC Controller  */
{ 0x8086, 0xa121}, /* Memory controller: Intel Corporation Sunrise Point-H PMC */
{ 0x8086, 0xa170}, /* Audio device: Intel Corporation Sunrise Point-H HD Audio */
{ 0x8086, 0xa123}, /* SMBus: Intel Corporation Sunrise Point-H SMBus */
{ 0x8086, 0x15b7}, /* Ethernet controller: Intel Corporation Ethernet Connection (2) I219-LM */
{ 0x8086, 0x24f3}, /* Network controller: Intel Corporation Wireless 8260 */
{ 0x8086, 0x1910}, /* Intel Corporation Skylake Host Bridge/DRAM Registers */
{ 0x8086, 0x193b}, /* Intel Corporation Iris Pro Graphics 580 */
{ 0x8086, 0xa131}, /* Intel Corporation Sunrise Point-H Thermal subsystem */
{ 0x8086, 0xa12f}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x590c}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x591e}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x9d43}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x9d43}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x9d48}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x9d56}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */
{ 0x8086, 0x9d58}, /* Intel Corporation Sunrise Point-H USB 3.0 xHCI Controller */

{ 0x8086, 0x5904}, /* KBL root bridge */
{ 0x8086, 0x5916}, /* KBL gfx */
{ 0x8086, 0x1903}, /* KBL thermal */
{ 0x8086, 0x9d2f}, /* KBL USB */
{ 0x8086, 0x9d31}, /* KBL Audio */
{ 0x8086, 0x9d60}, /* KBL I2C */
{ 0x8086, 0x9d61}, /* KBL I2C */
{ 0x8086, 0x9d3a}, /* KBL HECI */
{ 0x8086, 0x9d10}, /* KBL PCI bridge */
{ 0x8086, 0x9d14}, /* KBL PCI root port */
{ 0x8086, 0x9d15}, /* KBL PCI root port */
{ 0x8086, 0x9d18}, /* KBL PCI root port */
{ 0x8086, 0x9d58}, /* KBL ISA */
{ 0x8086, 0x9d21}, /* KBL MEM */
{ 0x8086, 0x9d71}, /* KBL Audio */
{ 0x8086, 0x9d23}, /* KBL SMBus */

{ 0x8086, 0x591f}, /* KBL root bridge */
{ 0x8086, 0x5912}, /* KBL gfx */
{ 0x8086, 0xa2af}, /* KBL USB */
{ 0x8086, 0xa2ba}, /* KBL root bridge */
{ 0x8086, 0xa282}, /* KBL gfx */
{ 0x8086, 0xa2e7}, /* KBL USB */
{ 0x8086, 0xa290}, /* KBL root bridge */
{ 0x8086, 0xa297}, /* KBL gfx */
{ 0x8086, 0xa298}, /* KBL USB */
{ 0x8086, 0xa2c5}, /* KBL root bridge */
{ 0x8086, 0xa2a1}, /* KBL gfx */
{ 0x8086, 0xa2f0}, /* KBL USB */
{ 0x8086, 0xa2a3}, /* KBL root bridge */

{ 0x8086, 0xa282}, /* Core i9 sata */
{ 0x8086, 0xa2af}, /* Core i9 xhci */
{ 0x8086, 0xa2af}, /* Core i9 xhci */
{ 0x8086, 0xa2f0}, /* Core i9 audio */
{ 0x8086, 0xa298}, /* Core i9 PCI */
{ 0x8086, 0xa298}, /* Core i9 PCI */
{ 0x8086, 0xa294}, /* Core i9 PCI */
{ 0x8086, 0xa290}, /* Core i9 PCI */
{ 0x8086, 0xa2e7}, /* Core i9 PCI */
{ 0x8086, 0x2030}, /* Core i9 PCI */

{ 0x8086, 0x5af0}, /* Apollo lake  PCI */
{ 0x8086, 0x5a8c}, /* Apollo lake  PCI */
{ 0x8086, 0x5a85}, /* Apollo lake  PCI */
{ 0x8086, 0x5a98}, /* Apollo lake  PCI */
{ 0x8086, 0x5a9a}, /* Apollo lake  PCI */
{ 0x8086, 0x5ae3}, /* Apollo lake  PCI */
{ 0x8086, 0x5ad8}, /* Apollo lake  PCI */
{ 0x8086, 0x5ad9}, /* Apollo lake  PCI */
{ 0x8086, 0x5aa8}, /* Apollo lake  PCI */
{ 0x8086, 0x5ac8}, /* Apollo lake  PCI */
{ 0x8086, 0x5acc}, /* Apollo lake  PCI */
{ 0x8086, 0x5ae8}, /* Apollo lake  PCI */
{ 0x8086, 0x5ad4}, /* Apollo lake  PCI */
{ 0x8086, 0x3165}, /* Apollo lake  PCI */

{ 0x8086, 0x15b8}, /* SKX */
{ 0x8086, 0x2014}, /* SKX */
{ 0x8086, 0x2015}, /* SKX */
{ 0x8086, 0x2016}, /* SKX */
{ 0x8086, 0x2018}, /* SKX */
{ 0x8086, 0x2020}, /* SKX */
{ 0x8086, 0x2021}, /* SKX */
{ 0x8086, 0x2024}, /* SKX */
{ 0x8086, 0x2025}, /* SKX */
{ 0x8086, 0x2026}, /* SKX */
{ 0x8086, 0x2030}, /* SKX */
{ 0x8086, 0x2034}, /* SKX */
{ 0x8086, 0x2035}, /* SKX */
{ 0x8086, 0x2036}, /* SKX */
{ 0x8086, 0x2040}, /* SKX */
{ 0x8086, 0x2041}, /* SKX */
{ 0x8086, 0x2042}, /* SKX */
{ 0x8086, 0x2043}, /* SKX */
{ 0x8086, 0x2044}, /* SKX */
{ 0x8086, 0x2045}, /* SKX */
{ 0x8086, 0x2046}, /* SKX */
{ 0x8086, 0x2047}, /* SKX */
{ 0x8086, 0x2048}, /* SKX */
{ 0x8086, 0x2049}, /* SKX */
{ 0x8086, 0x204a}, /* SKX */
{ 0x8086, 0x204b}, /* SKX */
{ 0x8086, 0x204c}, /* SKX */
{ 0x8086, 0x204d}, /* SKX */
{ 0x8086, 0x204e}, /* SKX */
{ 0x8086, 0x2054}, /* SKX */
{ 0x8086, 0x2055}, /* SKX */
{ 0x8086, 0x2056}, /* SKX */
{ 0x8086, 0x2057}, /* SKX */
{ 0x8086, 0x2066}, /* SKX */
{ 0x8086, 0x2080}, /* SKX */
{ 0x8086, 0x2081}, /* SKX */
{ 0x8086, 0x2082}, /* SKX */
{ 0x8086, 0x2083}, /* SKX */
{ 0x8086, 0x2084}, /* SKX */
{ 0x8086, 0x2085}, /* SKX */
{ 0x8086, 0x2086}, /* SKX */
{ 0x8086, 0x208d}, /* SKX */
{ 0x8086, 0x208e}, /* SKX */
{ 0x8086, 0xa290}, /* SKX */
{ 0x8086, 0xa294}, /* SKX */
{ 0x8086, 0xa298}, /* SKX */
{ 0x8086, 0xa2a1}, /* SKX */
{ 0x8086, 0xa2a3}, /* SKX */
{ 0x8086, 0xa2af}, /* SKX */
{ 0x8086, 0xa2ba}, /* SKX */
{ 0x8086, 0xa2d2}, /* SKX */
{ 0x8086, 0xa2e7}, /* SKX */
{ 0x8086, 0xa2f0}, /* SKX */

{ 0x8086, 0x0412},
{ 0x8086, 0x0c00},
{ 0x8086, 0x0c0c},
{ 0x8086, 0x153a},
{ 0x8086, 0x8c18},
{ 0x8086, 0x8c1c},
{ 0x8086, 0x8c20},
{ 0x8086, 0x8c22},
{ 0x8086, 0x8c26},
{ 0x8086, 0x8c2d},
{ 0x8086, 0x8c31},
{ 0x8086, 0x8c3a},
{ 0x8086, 0x8c3d},

{ 0x8086, 0x3ecc }, /* CFL */
{ 0x8086, 0x3ea5 }, /* CFL */
{ 0x8086, 0x1911 }, /* CFL */
{ 0x8086, 0x9df9 }, /* CFL */
{ 0x8086, 0x9ded }, /* CFL */
{ 0x8086, 0x9def }, /* CFL */
{ 0x8086, 0x9df0 }, /* CFL */
{ 0x8086, 0x9de0 }, /* CFL */
{ 0x8086, 0x9dd3 }, /* CFL */
{ 0x8086, 0x9db8 }, /* CFL */
{ 0x8086, 0x9dbc }, /* CFL */
{ 0x8086, 0x9db0 }, /* CFL */
{ 0x8086, 0x9db6 }, /* CFL */
{ 0x8086, 0x9d84 }, /* CFL */
{ 0x8086, 0x9dc8 }, /* CFL */
{ 0x8086, 0x9da3 }, /* CFL */
{ 0x8086, 0x9da4 }, /* CFL */
{ 0x8086, 0x15da }, /* CFL */
{ 0x8086, 0x15da }, /* CFL */
{ 0x8086, 0x15db }, /* CFL */

{ 0x8086, 0x8a12 },
{ 0x8086, 0x8a5a },
{ 0x8086, 0x8a03 },
{ 0x8086, 0x8a19 },
{ 0x8086, 0x8a1d },
{ 0x8086, 0x8a1f },
{ 0x8086, 0x8a21 },
{ 0x8086, 0x8a23 },
{ 0x8086, 0x8a11 },
{ 0x8086, 0x8a13 },
{ 0x8086, 0x8a17 },
{ 0x8086, 0x8a0d },
{ 0x8086, 0x34fc },
{ 0x8086, 0x34ed },
{ 0x8086, 0x34ef },
{ 0x8086, 0x34f0 },
{ 0x8086, 0x34f8 },
{ 0x8086, 0x34e8 },
{ 0x8086, 0x34e9 },
{ 0x8086, 0x34ea },
{ 0x8086, 0x34eb },
{ 0x8086, 0x34e0 },
{ 0x8086, 0x34e4 },
{ 0x8086, 0x34d3 },
{ 0x8086, 0x34c5 },
{ 0x8086, 0x34c6 },
{ 0x8086, 0x34c4 },
{ 0x8086, 0x34b0 },
{ 0x8086, 0x34a8 },
{ 0x8086, 0x34ab },
{ 0x8086, 0x3482 },
{ 0x8086, 0x34c8 },
{ 0x8086, 0x34a3 },
{ 0x8086, 0x34a4 },
{ 0x8086, 0x15e2 },
{ 0x8086, 0xf1a6 },


{ 0x144d, 0xa800 }, /* CFL */
{ 0x19ec, 0x522a}, /* CFL */




/*
 * Devices
 */
{ 0x8086, 0x088e}, /* Centrino Advanced-N 6235 */
/* if 0x1559 is enabled, breaks wired network */
// { 0x8086, 0x1559}, /* Intel tehernet I218-U */
{ 0x168c, 0x0032}, /* AR9580 Wireless Adapter */
{ 0x14e4, 0x1639}, /* Broadcom BCM5709 Gigabit Ethernet */
{ 0x1217, 0x8621}, /* SD Host controller: O2 Micro, Inc. SD/MMC Card Reader Controller */
//{ 0x144d, 0xa802}, /* Non-Volatile memory controller: Samsung Electronics Co Ltd NVMe SSD Controller SM951/PM951 */
{ 0x168c, 0x003e}, /* Qualcomm Atheros QCA6174 802.11ac Wireless Network Adapter  */
{ 0x10ec, 0x525a}, /* Realtek Semiconductor Co., Ltd. RTS525A PCI Express Card Reader */
{ 0x10ec, 0x5259}, /* Realtek Semiconductor Co., Ltd. RTS525A PCI Express Card Reader */
{ 0x1179, 0x0115}, /* Toshiba NVME */
{ 0x1179, 0x0116}, /* Toshiba NVME */
{ 0x1b21, 0x2142}, /* ASMedia Technology Inc. Device */
{ 0x10ec, 0x8168}, /* Realtek RTL8168 */

{0, 0} };

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
			if ( (known_good[i].vendor == vendor && known_good[i].device == device)) {
				if (asprintf(&filename, "/sys/bus/pci/devices/%s/power/control", entry->d_name) < 0)
                       		 	assert(0);
				write_string_to_file(filename, "auto");
				free(filename);
				break;
			}
			i++;
		}



	} while (1);

	closedir(dir);
}
