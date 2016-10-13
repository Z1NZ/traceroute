#include "ft_traceroute.h"

void	ft_error(void)
{
	if (errno == 0)
		fprintf(stderr ,"%s", "Unknown error");
	else
		fprintf(stderr ,"%s", sys_errlist[errno]);
	exit(-1);
}

void	ft_how_use(void)
{
	printf("%s\n", "Usage: traceroute [OPTION...] HOST\nPrint the route packets trace to network host.\n  -f, --first-hop=NUM        set initial hop distance, i.e., time-to-live\n  -g, --gateways=GATES       list of gateways for loose source routing\n  -I, --icmp                 use ICMP ECHO as probe\n  -m, --max-hop=NUM          set maximal hop count (default: 64)\n  -M, --type=METHOD          use METHOD (`icmp' or `udp') for traceroute\n                             operations, defaulting to `udp'\n  -p, --port=PORT            use destination PORT port (default: 33434)\n  -q, --tries=NUM            send NUM probe packets per hop (default: 3)\n      --resolve-hostnames    resolve hostnames\n  -t, --tos=NUM              set type of service (TOS) to NUM\n  -w, --wait=NUM             wait NUM seconds for response (default: 3)\n  -?, --help                 give this help list\n      --usage                give a short usage message\n  -V, --version              print program version\nMandatory or optional arguments to long options are also mandatory or optional\nfor any corresponding short options.\n");
	exit(-1);	
}
