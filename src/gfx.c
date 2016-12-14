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



void do_gfx_pm(void)
{
	FILE *file;
	char line[4096];
	int i;
	
	file = fopen("/sys/kernel/debug/dri/0/i915_max_freq", "r");
	if (!file)
		return;
	line[0] = 0;
	fgets(line, 496, file);
	fclose(file);
	i = strtoull(line, NULL, 10);
	if (i == 0)
		return;
	
	i = i - 150;
	file = fopen("/sys/kernel/debug/dri/0/i915_min_freq", "w");
	if (!file)
		return;
	fprintf(file, "%i\n", i);
	fclose(file);
	
}
