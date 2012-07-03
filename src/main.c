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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <clr_power.h>


int main(int argc, char **argv)
{
	/* SATA link power management */
	do_sata_links();

	/* VM writeback timeout and dirty %ages */
	do_vm_tweaks();

	/* USB autosuspend for non-HID */

	/* Runtime PM for PCI */
	do_pci_pm();

	/* turn off the NMI wathdog */
	do_nmi_watchdog();

	/* turn off Wake-on-Lan */
	do_WOL();

	return EXIT_SUCCESS;
}