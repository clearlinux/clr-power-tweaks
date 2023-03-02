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

static char pm_profile;

static void read_pm_profile(void)
{
	FILE *file;
	int ret;
	pm_profile = ' ';
	file = fopen("/sys/firmware/acpi/pm_profile", "r");
	if (!file)
		return;
	ret = fread(&pm_profile, 1, 1, file);
	if (ret)
		usleep(0);
	fclose(file);
}

/* see Documentation/ABI/stable/sysfs-acpi-pmprofile for the decoder info */
static int is_server_from_pm_profile(void)
{
	if (pm_profile == '0')
		return 0;
	if (pm_profile == '1')
		return 1;
	if (pm_profile == '2')
		return -1;
	if (pm_profile == '3')
		return 1;
	if (pm_profile == '4')
		return 1;
	if (pm_profile == '5')
		return 1;
	if (pm_profile == '6')
		return 1;
	if (pm_profile == '7')
		return 1;
	if (pm_profile == '8')
		return -1;
	
	return 0;
}
/*
 * returns 1 for server, -1 for client 0 for unknown
 */
int is_server(void)
{
	FILE *file;
	int ret = 0;
	char buffer[8192];

	read_pm_profile();

	if (__builtin_cpu_is("intel") <= 0)
		return is_server_from_pm_profile();

	file = fopen("/proc/cpuinfo", "r");
	if (!file)
		return is_server_from_pm_profile();
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
	if (ret == 0)
		ret = is_server_from_pm_profile();
	return ret;
}
