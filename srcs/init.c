#include "../includes/philosophers.h"

int	setup_forks(pthread_mutex_t **forks, int num_of_phil)
{
	int	i;

	*forks = malloc(sizeof(pthread_mutex_t) * num_of_phil);
	if (!*forks)
		return (write(1, "Failed init\n", 13), 1);
	i = 0;;
	while (i < num_of_phil)
	{
		if (pthread_mutex_init(&((*forks)[i]), NULL) != 0)
			return (write(1, "Failed init\n", 13), 1);
		i++;
	}
	return (0);
}

int setup_philo(t_args args, t_philo **philo, pthread_mutex_t *forks)
{
	int	i;
	int	*dead_f;
	pthread_mutex_t	*dead_mu;

	dead_f = malloc(sizeof(int));
	dead_mu = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(dead_mu, NULL);
	*philo = malloc(sizeof(t_philo) * args.num_of_phil);
	if (!*philo)
		return (1);
	i = 0;
	while (i < args.num_of_phil)
	{
		memset(&(*philo)[i], 0, sizeof(t_philo));
		(*philo)[i].id = i;
		(*philo)[i].meals_eaten = 0;
		(*philo)[i].left_fork = &forks[i];
		(*philo)[i].right_fork = &forks[(i + 1) % args.num_of_phil];
		(*philo)[i].dead = dead_f;
		(*philo)[i].dead_mutex = dead_mu;
		(*philo)[i].args = &args;
		i++;
	}
	return (0);
}

void	set_start_time(t_args args, t_philo **philo)
{
	int				i;
	pthread_mutex_t	*print;

	print = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(print, NULL);
	i = 0;
	while (i < args.num_of_phil)
	{
		(*philo)[i].last_meal = args.start_time;
		(*philo)[i].printing = print;
		i++;
	}
}