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
#include <linux/ethtool.h>
#include <dirent.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/types.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>

#include <clr_power.h>

static void turn_off_wol(char *iface)
{
	int sock;
	struct ifreq ifr;
	struct ethtool_wolinfo wol;
	int ret;

	memset(&ifr, 0, sizeof(struct ifreq));

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock<0)
		return;

	strcpy(ifr.ifr_name, iface);

	/* Check if the interface is up */
	ret = ioctl(sock, SIOCGIFFLAGS, &ifr);
	if (ret<0) {
		close(sock);
                return;
	}

	memset(&wol, 0, sizeof(wol));

	wol.cmd = ETHTOOL_GWOL;
	ifr.ifr_data = (caddr_t)&wol;
	ioctl(sock, SIOCETHTOOL, &ifr);


        if (wol.wolopts) {
	        wol.cmd = ETHTOOL_SWOL;
	        wol.wolopts = 0;
	        ioctl(sock, SIOCETHTOOL, &ifr);
	}
        close(sock);
}

static void maximize_queues(char *iface)
{
	int sock;
	struct ifreq ifr;
	int ret;
	struct ethtool_channels echannels;


	memset(&ifr, 0, sizeof(struct ifreq));

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock<0)
		return;

	strcpy(ifr.ifr_name, iface);

	/* Check if the interface is up */
	ret = ioctl(sock, SIOCGIFFLAGS, &ifr);
	if (ret<0) {
		close(sock);
                return;
	}

	memset(&echannels, 0, sizeof(echannels));

	echannels.cmd = ETHTOOL_GCHANNELS;
	ifr.ifr_data = (caddr_t)&echannels;
	ret = ioctl(sock, SIOCETHTOOL, &ifr);
	
        if (ret == 0 && echannels.combined_count < echannels.max_combined / 2 && echannels.max_combined > 0) {
        	echannels.cmd = ETHTOOL_SCHANNELS;
        	echannels.combined_count = echannels.max_combined;
	        ioctl(sock, SIOCETHTOOL, &ifr);
	}
        close(sock);
}

void do_WOL(void)
{
	DIR *dir;
	struct dirent *entry;

	dir = opendir("/sys/class/net/");
	if (!dir)
		return;
	do {
		entry = readdir(dir);
		if (!entry)
			break;
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;
		if (strcmp(entry->d_name, "lo") == 0)
			continue;

//		turn_off_wol(entry->d_name);
		maximize_queues(entry->d_name);
		
	} while (1);


	closedir(dir);
}
