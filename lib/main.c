#include "ft_traceroute.h"

void	ft_opt_h(void)
{
	printf("%s\n", "Usage: traceroute [-m NUM] [-w NUM] HOST");
	exit(-1);
}
void	ft_opt_m(char *option)
{
	char *tmp;


	tmp = option;
	if (option == NULL)
	{
		fprintf(stderr, "%s `%s`" ,"traceroute: invalid hops value", option);
		exit(-1);
	}
	while(ft_isdigit(*tmp) == 1)
		tmp++;
	if (*tmp != '\0')
	{
		fprintf(stderr, "%s `%s`" ,"traceroute: invalid hops value", option);
		exit(-1);
	}
	else
		g_env.hops = ft_atoi(option);
}

void	ft_opt_w(char *option)
{
	char *tmp;

	tmp = option;
	if (option == NULL)
	{
		fprintf(stderr, "%s `%s`" ,"traceroute: invalid hops value", option);
		exit(-1);
	}
	while(ft_isdigit(*tmp) == 1)
		tmp++;
	if (*tmp != '\0')
	{
		fprintf(stderr, "%s `%s`" ,"traceroute: invalid wait value", option);
		exit(-1);
	}
	else
		g_env.tmp = ft_atoi(option);
}

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

				if ( ***option == 'h')
					ft_opt_h();
				else if (***option == 'm')
				{
					opt |= OPT_M;
					++*option;
					ft_opt_m(**option);
					break ;
				}
				else if (***option == 'w')
				{
					opt |= OPT_W;
					++*option;
					ft_opt_w(**option);
					break ;
				}
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

	if (getuid())
	{
		printf("root require\n");
		return 0;
	}
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