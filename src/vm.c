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


void do_vm_tweaks(void)
{
	/* synchronous dirty ratio --> 50% */
	write_int_to_file("/proc/sys/vm/dirty_ratio", 50);
	/* start IO at 30% not 10%... the FS/timeout based write generates better IO patterns */
	write_int_to_file("/proc/sys/vm/dirty_background_ratio", 20);
	/* 15 seconds before the VM starts writeback, allowing the FS to deal with this better */
	write_int_to_file("/proc/sys/vm/dirty_writeback_centisecs", 1500);
	write_int_to_file("/sys//kernel/mm/transparent_hugepage/khugepaged/scan_sleep_millisecs", 30000);


	write_int_to_file("/sys/block/sda/queue/nr_requests", 4096);
	write_int_to_file("/proc/sys/vm/mmap_min_addr", 64*1024);
	write_int_to_file("/proc/sys/vm/extfrag_threshold", 100); /* oom less */
	write_int_to_file("/sys/kernel/mm/ksm/sleep_millisecs", 1000);
	write_int_to_file("/sys/kernel/mm/ksm/run", 1);
	write_int_to_file("/sys/kernel/mm/ksm/pages_to_scan", 1000);
}
