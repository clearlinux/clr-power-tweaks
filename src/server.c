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
 * returns 1 for server, -1 for client 0 for unknown
 */
int is_server(void)
{
	FILE *file;
	int ret = 0;
	char buffer[8192];
	if (__builtin_cpu_is("intel") <= 0)
		return 0;

	file = fopen("/proc/cpuinfo", "r");
	if (!file)
		return 0;
	while (!feof(file)) {
		buffer[0] = 0;
		char *c;
		c = fgets(buffer, 8191, file);
		if (c == NULL)
			break;
		if (strstr(buffer, "model name")) {
			if (strstr(buffer, "Xeon") || strstr(buffer, "Core(TM) i9"))
				ret = 1;
			else if ((strstr(buffer, "Core(TM)")) ||
			    (strstr(buffer, "Celeron")) ||
			    (strstr(buffer, "Pentium")))
				ret = -1;
			/*
			 * Atom - neutral for now, some Atom SoC's are client, but there are
			 * also Atom based servers.
			 */
		}
	}
	fclose(file);
	return ret;
}
