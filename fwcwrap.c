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
			"   -l IPaddr      IP address to list from zone\n"
			"   -?             Help (this message)\n"
			, prog);
	exit(1);
}

int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind;
	int c;
	int zoneopt = 0, ipadd = 0, ipdel = 0, iplist = 0;
	char zone[32],ipaddr[40];

	while ((c = getopt(argc, argv, "z:d:a:l:?")) != EOF)
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
			case 'l':	/* IP to list */
				if (strlen(optarg) >= sizeof(ipaddr)) {
					fprintf(stderr, "%s: IP address name too long for -%c\n", argv[0], c);
					usage(argv[0]);
				}
				strcpy(ipaddr, optarg);
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

	if ((ipadd+ipdel+iplist) > 1) {
		fprintf(stderr, "%s: cannot specify more than one of -a, -d and -l\n", argv[0]);
		usage(argv[0]);
	}

	/* we deliberately don't use --permanent, so changes are only until reboot */
	if (ipadd || ipdel || iplist) {
		argv[0] = COMMAND;
		argv[1] = "-q";
		argv[2] = "--zone";
		argv[3] = zone;
		argv[4] = iplist ? "--query-source" : ipadd ? "--add-source" : "--remove-source";
		argv[5] = ipaddr;
		argv[6] = NULL;

		return cmd(argv);
	} else {
		fprintf(stderr, "%s: must specify either -a, -d or -l\n", argv[0]);
		usage(argv[0]);
	}

	return 0;
}
