#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <curses.h>

/*
** define Flag option
*/
#define CHECK_BIT(var,pos)	(var & pos)
#define OPT_M				1
#define OPT_W				2



#define ICMP_ECHOREPLY		0
#define ICMP_ECHOREQ		8
#define ICMP_FILTER			13
#define REQ_DATASIZE		160
#define PACKETSIZE			64


typedef struct			s_env
{
	int					opt;
	ushort				ttl;
	long int			sd;
	long int			sd_r;
	unsigned long int	seq;
	int					tmp;
	unsigned long int	recvpack;
	char				*name;
	int					hops;
	struct addrinfo		*pinfo;
	struct packet		*pack;
	struct timeval		timer;
	char				*host;
	fd_set				rw;
}						t_env;

struct					packet
{
	struct				ip ip;
	struct				icmp hdr;
	char				msg[PACKETSIZE - (sizeof(struct icmp) + sizeof (struct ip))];
};

struct					recv_packet
{
	struct ip			ip;
	struct icmp			icmp;
	struct icmp			icmp_recv;
	// char				msg[PACKETSIZE-(sizeof(struct icmp) + sizeof(struct icmp) + sizeof( struct ip))];
};

t_env g_env;

/*
** opt
*/
int						ft_option(char ***option);
/*
** network
*/
struct addrinfo 		*ft_get_info(char *ptr, int opt);
u_short					ft_in_cksum(u_short *addr, int len);
u_short					ft_cksum(struct icmp *hdr);
int						ft_traceroute(int opt, char *ptr);
/*
** error
*/
void					ft_error(void);
void					ft_how_use(void);

/*
** libft
*/
int		ft_atoi(const char *str);
void	ft_bzero(void *s, int n);
int		ft_isdigit(int c);
int		ft_isspace(int c);
void	*ft_memalloc(size_t size);
void	*ft_memcpy(void *s1, const void *s2, size_t n);
void	*ft_memset(void *b, int c, size_t len);
size_t	ft_strlen(const char *s);
