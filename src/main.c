/*
 *    Clear Linux Project for Intel Architecture Power tweaks
 *
 *   This program should do all the things that are hard in udev,
 *   that PowerTOP suggests should be changed from their kernel defaults
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <argp.h>

#include <clr_power.h>


const char *argp_program_version = "Clear Linux Project for Intel Architecture Power Tweaks v" VERSION;

const char doc[] = "Power Tweaks -- adjusts runtime kernel options for optimal power and performance";

static struct argp argp = { NULL, NULL, 0, doc };

int main(int argc, char **argv)
{
	argp_parse (&argp, argc, argv, 0, 0, NULL);

	/* USB autosuspend for non-HID */

	/* Runtime PM for PCI */
	do_pci_pm();
	do_usb_pm();

	/* turn off Wake-on-Lan */
	do_WOL();

	return EXIT_SUCCESS;
}
