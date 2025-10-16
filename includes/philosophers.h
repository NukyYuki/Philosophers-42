/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:32:03 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/16 10:55:21 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h> 
# include <limits.h>
# include <stdbool.h>
# include "structs.h"

// INITS
int		setup_forks(pthread_mutex_t **forks, int num_of_phil);
int		setup_philo(t_args *args, t_philo **philo, pthread_mutex_t *forks);
int		parse_args(int ac, char **av, t_args *args);
int		check_values(int ac, char **av);
int		init_var(int *i, int **dead_f, pthread_mutex_t **dead_mu);

// SIMULATION
int		start_sim(t_args *args, t_philo *philo);
int		thread_create(t_philo *philo, int num_of_phil);
void	monitor(t_philo *ph, int n_ph);
void	join_thread(t_philo *philo, int num_of_phil);
void	*philo_routine(void *arg);
int		pickup(t_philo *philo);
int		is_dead(t_philo *philo);
void	unlock_forks(t_philo *philo);
void	update_last_meal(t_philo *philo);
void	update_full_philo(t_philo *philo);
int		take_fork_even(t_philo *philo);
int		take_fork_odd(t_philo *philo);
void	handle_monitor_exit(t_philo *philo);
int		return_verification(t_philo *philo, int n);
int		break_routine(t_philo *ph);
int		break2_routine(t_philo *ph, int dead, int all_full);

// UTILS
long	get_time(void);
void	precise_sleep(long duration);
int		ft_atoi(char *str);
int		is_valid_int32(char *str);
void	set_start_time(t_args *args, t_philo **philo);
void	safe_print(t_philo *philo, long start_time, const char *msg);
void	skip_whitespace(char *str, int *i, int *sign);
void	init_valid_int32(int *i, int *sign, long long *res, int *has_digit);
int		ft_strcmp(const char *s1, const char *s2);

// CLEANUP
void	clean_struct(t_args args, t_philo *philo, pthread_mutex_t *forks);

#endif
