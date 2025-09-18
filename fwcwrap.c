/*-----------------------------------------------------------------------------
 * Copyright (C) 2023, Software Insight Ltd. All Rights Reserved.
 *-----------------------------------------------------------------------------
 *
 * File:	fwcwrap.c
 *
 * Description:	Main program for firewall-cmd wrapper.
 *		This program invokes fwcables as root, called from apache.
 *
 * Author:	Tony Mountifield
 * Date:	14-Jul-2023
 *
 *-----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "fwcwrap.h"
#include "cmd.h"

static void usage(char *prog)
{
	fprintf(stderr, "usage: %s [options]\n"
			"options:\n"
			"   -z zone        Name of firewall zone\n"
			"   -a IPaddr      IP address to add to zone\n"
			"   -d IPaddr      IP address to delete from zone\n"
			"   -q IPaddr      IP address to query from zone\n"
			"   -L             List all IPs from zone\n"
			"   -?             Help (this message)\n"
			, prog);
	exit(1);
}

int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind;
	int c;
	int zoneopt = 0, ipadd = 0, ipdel = 0, ipquery = 0, iplist = 0;
	char zone[32],ipaddr[40];

	while ((c = getopt(argc, argv, "z:d:a:q:L?")) != EOF)
		switch (c) {
			case 'z':	/* name of zone */
				if (strlen(optarg) >= sizeof(zone)) {
					fprintf(stderr, "%s: zone name too long for -%c\n", argv[0], c);
					usage(argv[0]);
				}
				strcpy(zone, optarg);
				zoneopt = 1;
				break;
			case 'a':	/* IP to add */
				if (strlen(optarg) >= sizeof(ipaddr)) {
					fprintf(stderr, "%s: IP address too long for -%c\n", argv[0], c);
					usage(argv[0]);
				}
				strcpy(ipaddr, optarg);
				ipadd = 1;
				break;
			case 'd':	/* IP to delete */
				if (strlen(optarg) >= sizeof(ipaddr)) {
					fprintf(stderr, "%s: IP address too long for -%c\n", argv[0], c);
					usage(argv[0]);
				}
				strcpy(ipaddr, optarg);
				ipdel = 1;
				break;
			case 'q':	/* IP to query */
				if (strlen(optarg) >= sizeof(ipaddr)) {
					fprintf(stderr, "%s: IP address name too long for -%c\n", argv[0], c);
					usage(argv[0]);
				}
				strcpy(ipaddr, optarg);
				ipquery = 1;
				break;
			case 'L':	/* List IPs in zone */
				iplist = 1;
				break;
			case '?':	/* help */
				usage(argv[0]);
				break;
			default:
				usage(argv[0]);
		}

	if (optind < argc)
		usage(argv[0]);

	/* check that we have all the info we need */

	if (!zoneopt) {
		fprintf(stderr, "%s: -z zone must be supplied\n", argv[0]);
		usage(argv[0]);
	}

	if ((ipadd+ipdel+ipquery+iplist) > 1) {
		fprintf(stderr, "%s: cannot specify more than one of -a, -d, -q and -L\n", argv[0]);
		usage(argv[0]);
	}

	/* we deliberately don't use --permanent, so changes are only until reboot */
	if (ipadd || ipdel || ipquery || iplist) {
		argv[0] = COMMAND;
		argv[1] = "--zone";
		argv[2] = zone;
		if (iplist) {
			argv[3] = "--list-sources";
			argv[4] = NULL;
		} else {
			argv[3] = "-q";
			argv[4] = ipquery ? "--query-source" : ipadd ? "--add-source" : "--remove-source";
			argv[5] = ipaddr;
			argv[6] = NULL;
		}

		return cmd(argv);
	} else {
		fprintf(stderr, "%s: must specify either -a, -d, -q or -L\n", argv[0]);
		usage(argv[0]);
	}

	return 0;
}
