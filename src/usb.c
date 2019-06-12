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



struct usb_dev {
	int vendor;
	int device;
	int class;
};

#define CLASS(x) {0,0,x},
#define DEVICE(v,d) {v,d,0},



static struct usb_dev known_good[] =
{ 
/* {0x8087, 0x09fa, 0x0000} */ /* Intel Sensor Hub. Bust on USB autosusped */
{0x8087, 0x07dc, 0x0000},
{0x8087, 0x8001, 0x0009},	/* EHCI/XHCI built in hub */
{0x8087, 0x0a2a, 224},
{0x8087, 0x0a2b, 224},
/* BYT-T additions from T100 - need to verify they are all BYT-T systems */
{0x0b05, 0x17e0, 0x0000},
{0x0b95, 0x7720, 0x00ff},
{0x1d6b, 0x0002, 0x0009},
{0x1d6b, 0x0003, 0x0009},
/* BYT-T additions end */
/* Dell XPS 13 9360 (KBL) additions */
{0x04f3, 0x2234, 0x0000},	/* Elan HID-Touchscreen, needed for >PkgC8 */
/* Dell XPS 13 9360 (KBL) additions end */
{0xFFFF, 0xFFFF, 0} };

void do_usb_pm(void)
{
	DIR *dir;
	struct dirent *entry;

	dir = opendir("/sys/bus/usb/devices/");
	if (!dir)
		return;
	do {
		FILE *file;
		char *filename;
		char *line;
		size_t size;
		int vendor, device, class;
		int i;

		entry = readdir(dir);
		if (!entry)
			break;
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;
		
		if (asprintf(&filename, "/sys/bus/usb/devices/%s/idVendor", entry->d_name) < 0)
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

		if (asprintf(&filename, "/sys/bus/usb/devices/%s/idProduct", entry->d_name) < 0)
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

		if (asprintf(&filename, "/sys/bus/usb/devices/%s/bDeviceClass", entry->d_name) < 0)
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
		class = strtoull(line, NULL, 16);
		free(line);
		free(filename);
		fclose(file);
		
		//printf("Found USB device %04x:%04x  class %04x\n", vendor, device, class);
		i = 0;
		while (1) {
			if (known_good[i].vendor == 0xFFFF)
				break;
			if ((known_good[i].vendor == vendor && known_good[i].device == device) || 
			    (known_good[i].class == class && class != 0)) {
				if (asprintf(&filename, "/sys/bus/usb/devices/%s/power/control", entry->d_name) < 0)
                       		 	assert(0);
				write_string_to_file(filename, "auto");
				free(filename);
			}

			i++;
		}



	} while (1);


	closedir(dir);
}
