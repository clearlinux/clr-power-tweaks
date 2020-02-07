/*
 *   Clear Linux Project for Intel Architecture Power tweaks
 *
 *      Copyright (C) 2018 Intel Corporation
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
#include <glob.h>

#include <clr_power.h>

struct exclude_struct {
	const char *glob;
	struct exclude_struct *next;
};

static struct exclude_struct *excludes = NULL;

static bool debug_mode;

/**
 * Initializes lib functionality. Takes one argument (dbg). If dbg is "true",
 * then verbose/debug output will be printed to stderr-- otherwise no output
 * will be printed.
 */
void lib_init(const bool dbg)
{
	debug_mode = dbg;
}

/**
 * Block a glob pattern from future writes
 */
void string_exclude(const char *glob)
{
	if (!excludes) {
		excludes = calloc(1, sizeof(struct exclude_struct));
		if (!excludes)
			exit(EXIT_FAILURE);
		excludes->glob = strdup(glob);
	} else {
		struct exclude_struct *oldp = excludes;
		excludes = calloc(1, sizeof(struct exclude_struct));
		if (!excludes)
			exit(EXIT_FAILURE);
		excludes->glob = strdup(glob);
		excludes->next = oldp;
	}
}

static bool is_excluded(const char *glob)
{
	struct exclude_struct *s = excludes;
	while (s) {
		if (strcmp(s->glob, glob) == 0)
			return true;
		s = s->next;
	}
	return false;
}

/**
 * Writes @p string to file @p filename, with a newline. Returns zero
 * on success and -1 on failure.
 */
int write_string_to_file(const char *filename, const char *string)
{
	if (is_excluded(filename))
		return 0;

	FILE *file;
	file = fopen(filename, "w");
	if (!file) {
		if (debug_mode)
			fprintf(stderr, "Cannot write \"%s\" to %s: %s\n", string, filename, strerror(errno));
		return -1;
	}
	if (debug_mode)
		fprintf(stderr, "Writing \"%s\" to %s\n", string, filename);
	fprintf(file, "%s\n", string);
	fclose(file);
	return 0;
}


/**
 * Writes @p string to all files matched by @p match, with a
 * newline. Returns zero if there was a failure or non-zero on
 * success. All files that are matched are written to, even if a
 * previous one failed to.
 */
int write_string_to_files(const char *match, const char *string)
{
	size_t i;
	glob_t globbuf;
	int status = 0;

	if (is_excluded(match))
		return 0;

	int r = glob(match, GLOB_NOSORT | GLOB_BRACE, NULL, &globbuf);
	if (r == GLOB_NOMATCH) {
		if (debug_mode)
			fprintf(stderr, "No match for pattern \"%s\"\n", match);
		return 0;
	}
	if (r != 0)
		return -1;
	for (i = 0; i < globbuf.gl_pathc; ++i)
		status |= write_string_to_file(globbuf.gl_pathv[i], string);
	globfree(&globbuf);
	return status;
}
