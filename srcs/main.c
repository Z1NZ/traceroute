#include <ft_ping.h>


/*
	exemple de recuperation de valeur sur ping
void	ft_opt_i(char *option)
{
	char *tmp;

	tmp = option;
	while(ft_isdigit(*tmp) == 1)
		tmp++;
	if (*tmp != '\0')
	{
		fprintf(stderr, "%s","ping: bad timing interval" );
		exit(-1);
	}
	else
		g_env.interval = ft_atoi(option);
}
*/


int		ft_option(char ***option)
{
	int	opt;


	opt = 0;
	if (***option == '-')
	{
		while (*option && **option && ***option == '-')
		{
			++**option;
			while(***option != '\0')
			{

				if (***option == 'h')
					opt |= OPT_H;     // exemple de recuperation de flag
				// else if (***option == 'i')
				// {
				// 	opt |= OPT_I;
				// 	++*option;   exemple comment recupere de option sur ping 
				// 	ft_opt_i(**option);
				// 	break ;
				// }
				else
					return(-1);
				++**option;
			}
			++*option;
		}

		return (opt);
	}
	else
		return (-2);
}


int main(int argc, char *argv[])
{
	int opt;
	char **ptr;

	opt = 0;
	if (argc > 1)
	{
		if (argv[1][0] == '-')
		{
			ptr = argv + 1;
			if ((opt = ft_option(&ptr)) < 0)
			{
				if (opt == -1)
					fprintf(stderr ,"traceroute: -%c flag: %s\n", *argv[1], sys_errlist[1]);
				else
					fprintf(stderr ,"traceroute: %s\n", sys_errlist[1]);
				exit(-1);
			}
			ft_traceroute(opt, *ptr);
		}
		else
			ft_traceroute(0 , argv[1]);
	}
	else
		fprintf(stderr ,"%s\n", sys_errlist[22]);
	return 0;
}