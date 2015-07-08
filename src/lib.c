/*
 *   Clear Linux Project for Intel Architecture Power tweaks 
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
#include <errno.h>

#include <clr_power.h>

void write_string_to_file(char *filename, char *string)
{
	FILE *file;
	file = fopen(filename, "w");
	if (!file) {
		printf("Cannot write %s to %s: %s\n", string, filename, strerror(errno));
		return;
	}
	fprintf(file, "%s\n", string);
	fclose(file);
}


