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


static void set_governor(const char *gov)
{
	DIR *dir;
	struct dirent *entry;
	/* /sys/devices/system/cpu/cpufreq/policy0/scaling_governor */

	dir = opendir("/sys/devices/system/cpu/cpufreq/");
	if (!dir)
		return;

	do {
		char *filename;

		entry = readdir(dir);
		if (!entry)
			break;
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;

		if (asprintf(&filename, "/sys/devices/system/cpu/cpufreq/%s/scaling_governor", entry->d_name) < 0)
			assert(0);

		write_string_to_file(filename, gov);
		free(filename);
	} while (1);

	closedir(dir);

}


void do_cpu_pm(void)
{
	if (is_server() >= 0) {
		set_governor("performance");	
	} else {
		set_governor("powersave");	
		set_governor("schedutil");
	}
}
