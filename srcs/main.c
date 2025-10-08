#include "../includes/philosophers.h"

int	main(int ac, char **av)
{
	t_args			args;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if (ac < 5 && ac > 6)
		return (1);
	if (parse_args(ac, av, &args) != 0)
		return (1);
	init_philos(args, &philos);
	if (init_sim());

	
}
