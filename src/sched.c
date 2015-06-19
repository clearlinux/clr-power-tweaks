/*
 *    Clear Linux Project for Intel Architecture Power tweaks  - VM section
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
 *         Mario Carrillo  <mario.alfredo.c.arevalo@intel.com>
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <clr_power.h>


void do_sched_tweaks(void)
{
	/*This tuneable decides the minimum time a task will be be allowed to run on CPU before being pre-empted out*/
	write_int_to_file("/proc/sys/kernel/sched_min_granularity_ns", 10000000);
	/* Ability of tasks being woken to preempt the current task*/
	write_int_to_file("/proc/sys/kernel/sched_wakeup_granularity_ns",  15000000);
}
