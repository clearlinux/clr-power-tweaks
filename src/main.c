/*
 *    Clear Linux Project for Intel Architecture Power tweaks
 *
 *   This program should do all the things that are hard in udev,
 *   that PowerTOP suggests should be changed from their kernel defaults
 *
 *      Copyright (C) 2012-2018 Intel Corporation
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <clr_power.h>

#include "data.h"


const char *argp_program_version = "Clear Linux Project for Intel Architecture Power Tweaks v" VERSION;

const char *argp_program_bug_address = PACKAGE_BUGREPORT;

const char doc[] = "Power Tweaks -- adjusts runtime kernel options for optimal power and performance";

const char args_doc[] = "";

static struct argp_option options[] = {
	{ "debug", 'd', 0, OPTION_ARG_OPTIONAL, "Display debug/verbose output" },
	{ 0 },
};

struct arguments
{
	char *args[1];
	bool debug;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key) {
	case 'd':
		arguments->debug = true;
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num > 0)
			argp_usage(state);
		arguments->args[state->arg_num] = arg;
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

#define MSR_IA32_ENERGY_PERF_BIAS       0x000001b0

static unsigned long long  write_msr(int cpu, unsigned long long new_msr, int offset)
{
	unsigned long long old_msr;
	char msr_path[32];
	int retval;
	int fd;

	sprintf(msr_path, "/dev/cpu/%d/msr", cpu);
	fd = open(msr_path, O_RDWR);
	if (fd < 0) {
		perror(msr_path);
		exit(1);
	}

	retval = pread(fd, &old_msr, sizeof old_msr, offset);
	if (retval != sizeof old_msr) {
		close(fd);
		return retval;
	}

	retval = pwrite(fd, &new_msr, sizeof new_msr, offset);
	if (retval != sizeof new_msr) {
		printf("pwrite cpu%d 0x%x = %d\n", cpu, offset, retval);
	}

	close(fd);

	return old_msr;
}

int main(int argc, char **argv)
{
	struct arguments arguments;
	int status = 0;

	arguments.debug = false;
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	lib_init(arguments.debug);

	for (int i = 0; write_list[i].pathglob != 0; i++) {
		status |= write_string_to_files(write_list[i].pathglob,
				write_list[i].string);
	}

	write_string_to_file("/proc/sys/kernel/nmi_watchdog", "0");
	write_string_to_file("/sys/block/<disk>/queue/iosched/slice_idle", "0");

//	usleep(150000);

	/* USB autosuspend for non-HID */

	/* Runtime PM for PCI */
	do_usb_pm();

	/* turn off Wake-on-Lan */
	do_WOL();

/*	
	for (i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++)
		write_msr(i, 0, MSR_IA32_ENERGY_PERF_BIAS);
*/
	do_pci_pm();
	do_gfx_pm();
	verify_time();

	return EXIT_SUCCESS;
}
