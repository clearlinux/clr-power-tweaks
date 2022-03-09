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



void do_zero_pages(void)
{
	FILE *file;
	if (access("/proc/sys/kernel/vm/zero_pages", W_OK))
		return;

	if (nice(16))
		return;
	while (1) {
		sleep(5);
		file = fopen("/proc/sys/vm/zero_pages", "w");
		if (!file)
			return;
		fprintf(file, "%i\n", 5000);
		fclose(file);
	}

}
