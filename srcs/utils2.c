/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:04:23 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 18:25:01 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(philo->dead_mutex);
}

int	pickup(t_philo *philo)
{
	if (philo->left_fork == philo->right_fork)
	{
		safe_print(philo, philo->args->start_time, "has taken a fork");
		return (0);
	}
	if (philo->id % 2 == 0)
	{
		if (take_fork_even(philo) == 0)
			return (0);
	}
	else
	{
		if (take_fork_odd(philo) == 0)
		{
			return (0);
		}
	}
	update_last_meal(philo);
	return (1);
}

void	handle_monitor_exit(t_philo *philo)
{
	*(philo->dead) = 1;
	pthread_mutex_unlock(philo->dead_mutex);
	safe_print(philo, philo->args->start_time, "died");
}

int	break2_routine(t_philo *ph, int dead, int all_full)
{
	if (dead || all_full)
		return (0);
	if (!pickup(ph))
		return (0);
	if (return_verification(ph, 1) == 0)
		return (0);
	update_last_meal(ph);
	safe_print(ph, ph->args->start_time, "is eating");
	precise_sleep(ph->args->count_eat);
	ph->meals_eaten++;
	unlock_forks(ph);
	return (1);
}

int	break_routine(t_philo *ph)
{
	if (return_verification(ph, 1) == 0)
		return (0);
	safe_print(ph, ph->args->start_time, "is sleeping");
	precise_sleep(ph->args->count_sleep);
	if (return_verification(ph, 1) == 0)
		return (0);
	safe_print(ph, ph->args->start_time, "is thinking");
	precise_sleep((ph->args->count_die - ph->args->count_sleep
			- ph->args->count_eat) / 4);
	if (return_verification(ph, 1) == 0)
		return (0);
	return (1);
}
