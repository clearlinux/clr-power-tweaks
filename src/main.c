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

#define OPT_VM 1 // vm key

static struct argp_option options[] = {
	{"vm", OPT_VM, 0, 0, "Execute tweaks for Virtual Machines only"},
	{ 0 }
};

struct arguments {
	int vm;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
	switch (key) {
	case OPT_VM:
		arguments->vm = 1;
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, 0, doc };

int main(int argc, char **argv)
{
	struct arguments arguments;
	arguments.vm = 0;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	/* VM writeback timeout and dirty %ages */
	do_vm_tweaks();

	if (arguments.vm)
		return EXIT_SUCCESS;

	/* SATA link power management */
	//do_sata_links();

	/* USB autosuspend for non-HID */

	/* Runtime PM for PCI */
	do_pci_pm();
	do_usb_pm();

	/* turn off the NMI wathdog */
	do_nmi_watchdog();

	/* turn off Wake-on-Lan */
	do_WOL();

	/* audio pm */
	write_int_to_file("/sys/module/snd_hda_intel/parameters/power_save", 1);

	/* P state stuff */
	write_string_to_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", "performance");
	/* we want at least half performance, this helps us in race-to-halt and to give us reasonable responses */
	write_int_to_file("/sys/devices/system/cpu/intel_pstate/min_perf_pct", 50);

	return EXIT_SUCCESS;
}
