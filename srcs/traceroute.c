#include <ft_traceroute.h>
#include <limits.h>
#include <math.h>



unsigned short
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

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
	g_env.pack = (struct packet*)ft_memalloc(sizeof(struct packet));
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

static void 	ft_out(int i)
{
	(void)i;
	exit (0);
}

static void	ft_ping_old(int i)
{
	(void)i;

	int len;
	char buf[1024];
	struct sockaddr_in server;
	struct  msghdr msg;
	struct iovec iov[1]; /* Data array */
	struct recv_packet *p;

	memset(&msg, '\0', sizeof(msg));
	memset(g_env.pack, '\0', (sizeof(struct packet)));
	g_env.pack->ip.ip_hl = 5;
	g_env.pack->ip.ip_v = 4;
	g_env.pack->ip.ip_tos = 0;
	g_env.pack->ip.ip_len = 20 + 8;
	g_env.pack->ip.ip_off = 0;
	g_env.pack->ip.ip_p = IPPROTO_ICMP;
	inet_pton(AF_INET, g_env.name, &(g_env.pack->ip.ip_dst));

	iov[0].iov_base = &(g_env.pack->hdr);
	iov[0].iov_len = sizeof(*g_env.pack);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	len = 0;
	server.sin_family = g_env.pinfo->ai_family;
	inet_pton(AF_INET, g_env.name, &server.sin_addr);
	int toto;
	if (setsockopt(g_env.sd, IPPROTO_IP, IP_HDRINCL, (const int *)&toto, sizeof(toto)) < 0)
		perror("setsocket() failed ");
	i = 0;
			struct sockaddr_storage sender;
			socklen_t sendsize = sizeof(sender);
	while(g_env.ttl < 255)
	{
		g_env.pack->ip.ip_ttl = ++g_env.ttl;
		g_env.pack->ip.ip_id = htons(getpid() - g_env.ttl);

		g_env.pack->ip.ip_sum = ft_in_cksum((unsigned short*)&(g_env.pack->ip), sizeof(struct ip));
		i = 0;
		while(i < 3)
		{
			memset(&(g_env.pack->hdr), '\0', sizeof(g_env.pack->hdr));
			g_env.pack->hdr.icmp_type = ICMP_ECHOREQ;
			g_env.pack->hdr.icmp_code = 0;
			g_env.pack->hdr.icmp_id = htons(getpid() - g_env.ttl);
			g_env.pack->hdr.icmp_seq = i;
			g_env.pack->hdr.icmp_cksum = 0;
			g_env.pack->hdr.icmp_cksum =  ft_in_cksum((unsigned short*)&(g_env.pack->hdr), sizeof(struct icmp));		
			if (sendto (g_env.sd, g_env.pack, PACKETSIZE, 0, (struct sockaddr *)&server, sizeof (struct sockaddr)) < 0)
			{
				perror ("sendto() failed ");
				return ;
			}

			jump:
			bzero(&sender, sizeof(sender));
			bzero(&buf, sizeof(1024));
			len = recvfrom(g_env.sd, buf, sizeof(buf), 0, (struct sockaddr*)&sender, &sendsize);
			if (len)
			{
				p = (struct recv_packet *)buf;
				if (p->ip.ip_id != g_env.pack->ip.ip_id && (p->hdr.icmp_code != ICMP_ECHOREPLY && p->hdr.icmp_type != 3))
					goto jump;
				if (i == 0)
					printf("%d == > %s\t", g_env.ttl, inet_ntoa(p->ip.ip_src));
				else if (i == 1)
					printf("%s\t","ok");
				else
					printf("%s\n","ok");
			}
			i++;
		}
		if (p->ip.ip_src.s_addr == g_env.pack->ip.ip_dst.s_addr)
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
	g_env.host = ptr;
	g_env.opt = opt;
	signal(SIGINT, ft_out);
		ft_ping_old(0);
	return(0);
}