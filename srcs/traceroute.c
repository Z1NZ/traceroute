#include <ft_traceroute.h>
#include <limits.h>
#include <math.h>

struct addrinfo *ft_get_info(char *ptr, int opt)
{
	struct addrinfo hints;
	struct addrinfo *result;
	struct sockaddr_in *h;

	(void)opt;
	if (!ptr) 
	{
		fprintf(stderr, "Usage: <ip>\n");
		exit(-1);
	}
	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(ptr, NULL, &hints, &result))
	{
		if (errno == 0)
		{
			fprintf(stderr ,"%s%s", "ping: unknown host ", ptr);
			exit(-1);
		}
		else
			ft_error();
	}
	h = (struct sockaddr_in *) result->ai_addr;
	g_env.name =  inet_ntoa( h->sin_addr );
	return(result);
}

void	tvsub(struct timeval *out, struct timeval*in)
{
	if( (out->tv_usec -= in->tv_usec) < 0 )   {
		out->tv_sec--;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}
void debug(void)
{
	printf("ip.hl %d\nip.v %d\nop.tos%d\nip.len%d\nip.off%d\nip.p%d\nip.ttl%d\nip.id%d\nip.sum%d\n", g_env.pack->ip.ip_hl, g_env.pack->ip.ip_v, g_env.pack->ip.ip_tos
		, g_env.pack->ip.ip_len, g_env.pack->ip.ip_off, g_env.pack->ip.ip_p, g_env.pack->ip.ip_ttl,g_env.pack->ip.ip_id, g_env.pack->ip.ip_sum);

\
	printf("icmp.id%d\nicmp.seq%d\nicmp.type%d\nicmp.code%d\nicmp.sum%d\n",
					g_env.pack->hdr.icmp_id, 
		g_env.pack->hdr.icmp_seq ,
	g_env.pack->hdr.icmp_type ,
	g_env.pack->hdr.icmp_code ,
g_env.pack->hdr.icmp_cksum );
}
static void	ft_ping_old(int i)
{
	int len;
	double tmp;
	struct timeval tp, tv;
	struct recv_packet *p = NULL;
	struct sockaddr_storage sender;
	char buf[1024];
	struct sockaddr_in server;
	socklen_t sendsize;

	g_env.pack = (struct packet*)ft_memalloc(sizeof (struct packet));
	g_env.pack->ip.ip_hl = 5;
	g_env.pack->ip.ip_v = 4;
	g_env.pack->ip.ip_tos = 0;
	g_env.pack->ip.ip_len = sizeof(struct packet);
	g_env.pack->ip.ip_off = 0;
	g_env.pack->ip.ip_p = IPPROTO_ICMP;
	inet_pton(AF_INET, g_env.name, &(g_env.pack->ip.ip_dst));
	len = 0;
	server.sin_family = g_env.pinfo->ai_family;
	inet_pton(AF_INET, g_env.name, &(server.sin_addr));
	i = 0;
	while(g_env.ttl < 255)
	{
		g_env.pack->ip.ip_ttl = ++g_env.ttl;
		g_env.pack->ip.ip_id = ntohs(getpid() - g_env.ttl);
		g_env.pack->ip.ip_sum = ft_in_cksum((unsigned short*)&(g_env.pack->ip), sizeof(struct ip));
		if (setsockopt(g_env.sd, IPPROTO_IP, IP_HDRINCL, (const int *)&g_env.ttl, sizeof(g_env.ttl)) < 0)
			perror("setsocket() failed ");
		i = 0;
		while(i < 3)
		{
			if (i == 0)
				printf("  %d", g_env.ttl);
			FD_ZERO(&(g_env.rw));
			FD_SET(g_env.sd, &(g_env.rw));
			g_env.timer.tv_usec = 0;
			g_env.timer.tv_sec = g_env.tmp;
			memset(&(g_env.pack->hdr), '\0', sizeof(struct icmp));
			g_env.pack->hdr.icmp_id = ntohs(getpid() - g_env.ttl);
			g_env.pack->hdr.icmp_seq = i;
			g_env.pack->hdr.icmp_type = ICMP_ECHOREQ;
			g_env.pack->hdr.icmp_code = 0;
			g_env.pack->hdr.icmp_cksum = 0;
			g_env.pack->hdr.icmp_cksum =  ft_in_cksum((unsigned short*)&(g_env.pack->hdr), sizeof(struct icmp));	
			gettimeofday(&tp, NULL);
			if (sendto (g_env.sd, g_env.pack, sizeof(struct packet), 0, (struct sockaddr *)&server, sizeof (struct sockaddr_in)) < 0)
			{
				perror ("sendto() failed ");
				return ;
			}
			select(g_env.sd + 1, &(g_env.rw), NULL, NULL, &(g_env.timer));
			if (FD_ISSET(g_env.sd, &(g_env.rw)))
			{
						jump:
						sendsize = sizeof(sender);
						ft_bzero(&sender, sizeof(sender));
						ft_bzero(&buf, sizeof(1024));
						len = recvfrom(g_env.sd, buf, sizeof(buf), 0, (struct sockaddr*)&sender, &sendsize);
						gettimeofday(&tv, NULL);
						if (len)
						{
							p = (struct recv_packet *)buf;
							if ((p->icmp.icmp_id == g_env.pack->hdr.icmp_id) || (p->icmp_recv.icmp_id == g_env.pack->hdr.icmp_id))
							{
								if (p->icmp.icmp_type == ICMP_ECHOREPLY || p->icmp.icmp_type == ICMP_TIME_EXCEEDED)
								{
									tvsub(&tv, &tp);
									tmp = tv.tv_sec * 10000000L + tv.tv_usec;
									if (i == 0)
										printf("  %s  ", inet_ntoa(p->ip.ip_src));
									printf("%.3f  ", tmp/1000);
								}
								else if(p->icmp.icmp_type == ICMP_DEST_UNREACH)
								{
									switch (p->icmp.icmp_code) {
										case ICMP_UNREACH_PORT:
											printf(" !");
											break;

										case ICMP_UNREACH_NET:
											break;

										case ICMP_UNREACH_HOST:
											printf(" !H");
											break;

										case ICMP_UNREACH_PROTOCOL:
											printf(" !P");
											break;

										case ICMP_UNREACH_NEEDFRAG:
											printf(" !F");
											break;

										case ICMP_UNREACH_SRCFAIL:
											printf(" !S");
											break;

										case ICMP_UNREACH_FILTER_PROHIB:
											printf(" !X");
											break;

										case ICMP_UNREACH_HOST_PRECEDENCE:
											printf(" !V");
											break;

										case ICMP_UNREACH_PRECEDENCE_CUTOFF:
											printf(" !C");
											break;

										default:
											printf(" !<%d>", p->icmp.icmp_code);
											break;
									}
								}
							}
							else
								goto jump;
						}
			}
			else
				printf("  *");
			if (i == 2)
				printf("\n");
			i++;
		}
		if (p && p->ip.ip_src.s_addr == g_env.pack->ip.ip_dst.s_addr)
			break ;
	}
}
int		ft_traceroute(int opt, char *ptr)
{
	g_env.pinfo = ft_get_info(ptr, opt);
	if ((g_env.sd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		printf("%s\n", "socket");
		ft_error();
	}
	if (!CHECK_BIT(opt, OPT_H))
		g_env.hops = 255;
	if (!CHECK_BIT(opt, OPT_W))
		g_env.tmp = 3;
	g_env.host = ptr;
	g_env.opt = opt;
	ft_ping_old(0);
	return(0);
}