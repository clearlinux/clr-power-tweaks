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
 *   the Free Software Foundation, version 2 or later of the License.
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

#pragma once

struct write_struct {
	char *pathglob;
	char *string;
};

struct write_struct write_list[] = {
	// synchronous dirty ratio --> 50%
	{"/proc/sys/vm/dirty_ratio", "50"},
	{"/proc/sys/kernel/unprivileged_bpf_disabled", "1"},

	// start IO at 5% not 10%... start IO a little earlier asynchronously, since memory sizes are bigger now
	{"/proc/sys/vm/dirty_background_ratio", "5"},

	// 15 seconds before the VM starts writeback, allowing the FS to deal with this better
	{"/proc/sys/vm/dirty_writeback_centisecs", "1500"},
	{"/proc/sys/vm/swappiness", "10"},
	{"/sys/kernel/mm/transparent_hugepage/khugepaged/scan_sleep_millisecs", "30000"},
	{"/sys/devices/virtual/graphics/fbcon/cursor_blink", "0"},
	{"/sys/kernel/rcu_expedited", "0"},
	{"/proc/sys/vm/mmap_min_addr", "65536"},

	// oom less
	{"/proc/sys/vm/extfrag_threshold", "100"},
	{"/sys/kernel/mm/ksm/sleep_millisecs", "4000"},

	// w /sys/kernel/mm/ksm/run - - - - 1
	{"/sys/kernel/mm/ksm/pages_to_scan", "1000"},
	{"/proc/sys/net/core/rmem_max", "1703936"},
	{"/proc/sys/net/core/wmem_max", "1703936"},
	{"/proc/sys/net/core/somaxconn", "512"},

	// This tuneable decides the minimum time a task will be be allowed to
	// run on CPU before being pre-empted out
	{"/proc/sys/kernel/sched_min_granularity_ns", "2250000"},
	{"/proc/sys/kernel/sched_migration_cost_ns", "50000"},

	// Ability of tasks being woken to preempt the current task
	{"/proc/sys/kernel/sched_wakeup_granularity_ns", "15000000"},

	// audio pm
	{"/sys/module/snd_hda_intel/parameters/power_save", "1"},

	// P state stuff
	{"/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor", "performance"},

	// we want at least half performance, this helps us in race-to-halt and
	// to give us reasonable responses
	{"/sys/devices/system/cpu/intel_pstate/min_perf_pct", "50"},
	{"/proc/sys/net/core/default_qdisc", "fq"},

	// Disable acceptance of all ICMP redirected packets on all interfaces.
	{"/proc/sys/net/ipv4/conf/all/accept_redirects", "0"},
	{"/proc/sys/net/ipv6/conf/all/accept_redirects", "0"},
	{"/proc/sys/net/ipv4/conf/default/accept_redirects", "0"},
	{"/proc/sys/net/ipv6/conf/default/accept_redirects", "0"},

	// Disables sending of all IPv4 ICMP redirected packets on all interfaces.
	{"/proc/sys/net/ipv4/conf/all/send_redirects", "0"},
	{"/proc/sys/net/ipv4/conf/default/send_redirects", "0"},

	// Disables acceptance of secure ICMP redirected packets on all interfaces.
	{"/proc/sys/net/ipv4/conf/all/secure_redirects", "0"},
	{"/proc/sys/net/ipv4/conf/default/secure_redirects", "0"},

	// SATA link power management
	{"/sys/class/scsi_host/*/link_power_management_policy", "med_power_with_dipm"},

	// Performance tuning for SATA and NVME storage
	{"/sys/block/sd*/queue/scheduler", "bfq"},
	{"/sys/block/sd*/queue/nr_requests", "1024"},
	{"/sys/block/sd*/queue/read_ahead_kb", "1024"},
	{"/sys/block/sd*/queue/add_random", "1"},
	{"/sys/block/nvme*/queue/scheduler", "bfq"},
	{"/sys/block/nvme*/queue/iosched/low_latency", "1"},
	{"/sys/block/nvme*/queue/nr_requests", "2048"},
	{"/sys/block/nvme*/queue/read_ahead_kb", "256"},
	{"/sys/block/nvme*/queue/add_random", "1"},

	// Enable turbo mode max
	{"/proc/sys/kernel/sched_itmt_enabled", "1"},

	// Reload the microcode at boot
	{"/sys/devices/system/cpu/microcode/reload", "1"},

	{"/proc/sys/kernel/nmi_watchdog", "0"},
	{"/sys/block/{sd,mmc,nvme}*/queue/iosched/slice_idle", "0"},

	// End of list.
	{NULL, NULL}
};
