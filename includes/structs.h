/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:30:56 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 12:36:12 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "philosophers.h"

typedef struct s_args
{
	int		num_of_phil;
	int		count_die;
	int		count_eat;
	int		count_sleep;
	int		needed_meals;
	long	start_time;
	int		num_full_philo;
}		t_args;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				*dead;
	bool			is_full;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*dead_mutex;
	pthread_mutex_t	*printing;
	t_args			*args;
}			t_philo;
#endif
