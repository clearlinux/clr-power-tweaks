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

/*
 * For settings, the `where` field should normally be '0' - assume server
 * If you need to specify a setting for desktop only, use '-1'.
 * For servers, prefer '0' but use '1' if there's a desktop-alternate value.
 * You shouldn't have 2 entries where '0' is used as a value for `where`.
 */

struct write_struct {
	char *pathglob;
	char *string;
	int where; // -1 client, 0 neutral (assume server), 1 server only
};

struct write_struct write_list[] = {
	// synchronous dirty ratio --> 50%
	{"/proc/sys/vm/dirty_ratio", "50", 0},
	{"/proc/sys/kernel/unprivileged_bpf_disabled", "1", 0},

	// start IO at 5% not 10%... start IO a little earlier asynchronously, since memory sizes are bigger now
	{"/proc/sys/vm/dirty_background_ratio", "5", 0},

	// 15 seconds before the VM starts writeback, allowing the FS to deal with this better
	{"/proc/sys/vm/dirty_writeback_centisecs", "1500", 0},
	{"/proc/sys/vm/swappiness", "10", 0},
	{"/sys/kernel/mm/transparent_hugepage/khugepaged/scan_sleep_millisecs", "30000"},
	{"/sys/devices/virtual/graphics/fbcon/cursor_blink", "0", 0},
	{"/sys/kernel/rcu_expedited", "0", 0},
	{"/proc/sys/vm/mmap_min_addr", "65536", 0},

	// oom less
	{"/proc/sys/vm/extfrag_threshold", "100", 0},
	{"/sys/kernel/mm/ksm/sleep_millisecs", "4000", 0},

	// w /sys/kernel/mm/ksm/run - - - - 1
	{"/sys/kernel/mm/ksm/pages_to_scan", "1000", 0},

	// This tuneable decides the minimum time a task will be be allowed to
	// run on CPU before being pre-empted out
	{"/proc/sys/kernel/sched_min_granularity_ns", "2250000", 0},
	{"/proc/sys/kernel/sched_migration_cost_ns", "50000", 0},

	// Ability of tasks being woken to preempt the current task
	{"/proc/sys/kernel/sched_wakeup_granularity_ns", "15000000", 0},

	// sched_autogroup would improve interactive desktop performance in the face of
	// multi‐ process, CPU-intensive workloads. Whereas it would harm performance,
	// thus disable it on Server
	{"/proc/sys/kernel/sched_autogroup_enabled", "0", 1},

	// audio pm
	{"/sys/module/snd_hda_intel/parameters/power_save", "1", 0},

	// P state stuff
	{"/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor", "performance", 0},

	// we want at least half performance, this helps us in race-to-halt and
	// to give us reasonable responses
	{"/sys/devices/system/cpu/intel_pstate/min_perf_pct", "50", 0},
	{"/proc/sys/net/core/default_qdisc", "fq", 0},

	// Disable acceptance of all ICMP redirected packets on all interfaces.
	{"/proc/sys/net/ipv4/conf/all/accept_redirects", "0", 0},
	{"/proc/sys/net/ipv6/conf/all/accept_redirects", "0", 0},
	{"/proc/sys/net/ipv4/conf/default/accept_redirects", "0", 0},
	{"/proc/sys/net/ipv6/conf/default/accept_redirects", "0", 0},

	// Disables sending of all IPv4 ICMP redirected packets on all interfaces.
	{"/proc/sys/net/ipv4/conf/all/send_redirects", "0", 0},
	{"/proc/sys/net/ipv4/conf/default/send_redirects", "0", 0},

	// Disables acceptance of secure ICMP redirected packets on all interfaces.
	{"/proc/sys/net/ipv4/conf/all/secure_redirects", "0", 0},
	{"/proc/sys/net/ipv4/conf/default/secure_redirects", "0", 0},

	{"/proc/sys/net/ipv4/tcp_slow_start_after_idle", "0", 0},

	// SATA link power management
	{"/sys/class/scsi_host/*/link_power_management_policy", "med_power_with_dipm", 0},

	// Performance tuning for SATA and NVME storage
	{"/sys/block/sd*/queue/scheduler", "bfq", 0},
	{"/sys/block/sd*/queue/nr_requests", "4096", 0},
	{"/sys/block/sd*/queue/read_ahead_kb", "1024", 0},
	{"/sys/block/sd*/queue/add_random", "1", 0},

	// For server, prefer mq-deadline to max throughput
	{"/sys/block/nvme*/queue/scheduler", "mq-deadline", 1},

	// For desktop, prefer bfq low_latency
	{"/sys/block/nvme*/queue/scheduler", "bfq", -1},
	{"/sys/block/nvme*/queue/iosched/low_latency", "0", 1},
	{"/sys/block/nvme*/queue/iosched/low_latency", "1", -1},

	{"/sys/block/nvme*/queue/nr_requests", "2048", 0},
	{"/sys/block/nvme*/queue/read_ahead_kb", "1024", 0},
	{"/sys/block/nvme*/queue/add_random", "1", 0},

	// Enable turbo mode max
	{"/proc/sys/kernel/sched_itmt_enabled", "1", 0},

	// Reload the microcode at boot
	{"/sys/devices/system/cpu/microcode/reload", "1", 0},
	{"/sys/devices/system/cpu/cpu0/power/energy_perf_bias", "performance", 0}, 

	{"/proc/sys/kernel/nmi_watchdog", "0", 0},
	{"/sys/block/{sd,mmc,nvme, 0}*/queue/iosched/slice_idle", "0"},
	
	// SPR uncore
	{"/sys/devices/system/cpu/intel_uncore_frequency/package_00_die_00/max_freq_khz", "2300000"},
	{"/sys/devices/system/cpu/intel_uncore_frequency/package_01_die_00/max_freq_khz", "2300000"},

	// End of list.
	{NULL, NULL, 0}
};
