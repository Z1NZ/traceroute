#include <ft_traceroute.h>

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
	printf("%s\n", "Usage: traceroute [-ahvq] [-i interval] [-t ttl] destination\n");
	exit(-1);	
}
