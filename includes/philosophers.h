#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
# include <stdbool.h>
#include "structs.h"

// INITS
int		setup_forks(pthread_mutex_t **forks, int num_of_phil);
int 	setup_philo(t_args *args, t_philo **philo, pthread_mutex_t *forks);
int 	parse_args(int ac, char **av, t_args *args);

// SIMULATION
int     start_sim(t_args *args, t_philo *philo);
int     thread_create(t_philo *philo, int num_of_phil);
void    monitor(t_philo *philo, int num_phil);
void    join_thread(t_philo *philo, int num_of_phil);
void    *philo_routine(void *arg);
int		pickup(t_philo  *philo);
int     is_dead(t_philo *philo);
void	unlock_forks(t_philo *philo);
void    update_last_meal(t_philo *philo);
void update_full_phillo(t_philo *philo);


// UTILS
long    get_time(void);
void    precise_sleep(long duration);
int		ft_atoi(char *str);
void	set_start_time(t_args *args, t_philo **philo);
void    safe_print(t_philo *philo, long start_time, const char *msg);

// CLEANUP
void    clean_struct(t_args args, t_philo *philo, pthread_mutex_t *forks);

#endif
