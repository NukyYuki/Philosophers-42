#ifndef STRUCT_H
# define STRUCT_H

#include "philosophers.h"

typedef struct  s_args
{
    int 	num_of_phil;
    int 	count_die;
    int 	count_eat;
    int 	count_sleep;
    int 	needed_meals;
	long	start_time;
}           t_args;

typedef struct  s_philo
{
    int     		id;
    int     		meals_eaten;
	pthread_t		thread;
    pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				*dead;
	pthread_mutex_t	*dead_mutex;
    t_args  		*args;
}           t_philo;

int	setup_forks(pthread_mutex_t **forks, int num_of_phil);
int setup_philo(t_args args, t_philo **philo, pthread_mutex_t *forks);
int parse_args(int ac, char **av, t_args *args);
void    clean_struct(t_args args, t_philo philo, pthread_mutex_t *forks);

#endif
