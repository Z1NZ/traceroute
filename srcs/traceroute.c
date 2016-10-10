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
	g_env.seq++;
	int len;

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
	g_env.pack->ip.ip_id = 10000;
	g_env.pack->ip.ip_off = 0;
	g_env.pack->ip.ip_ttl = 0;
	g_env.pack->ip.ip_p = IPPROTO_ICMP;
	inet_pton(AF_INET, g_env.name, &(g_env.pack->ip.ip_dst));
	g_env.pack->ip.ip_sum = ft_in_cksum((unsigned short*)&(g_env.pack->ip), sizeof(struct ip));
	g_env.pack->hdr.type = ICMP_ECHOREQ;
	g_env.pack->hdr.un.echo.id = getpid();
	g_env.pack->hdr.un.echo.sequence = g_env.seq;
	g_env.pack->hdr.checksum =  ft_in_cksum((unsigned short*)&(g_env.pack->hdr), sizeof(struct icmphdr));
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
	if (sendto (g_env.sd, g_env.pack, PACKETSIZE, 0, (struct sockaddr *)&server, sizeof (struct sockaddr)) < 0)
	{
		perror ("sendto() failed ");
		alarm(g_env.interval);
		return ;
	}
	jump:
		len = recvmsg(g_env.sd, &msg, 0);
	if (len)
	{
		p = (struct recv_packet *)msg.msg_iov[0].iov_base;
		if (p->hdr.icmp_hun.ih_idseq.icd_id != getpid() && (p->hdr.icmp_code != ICMP_ECHOREPLY && p->hdr.icmp_code != ICMP_FILTER))
			goto jump;
		printf("%zd bytes from %s: icmp_seq=%u ttl=%d\n", msg.msg_iov[0].iov_len, g_env.name, p->hdr.icmp_hun.ih_idseq.icd_seq, p->ip.ip_ttl);
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
	while(1)
		;
	return(0);
}