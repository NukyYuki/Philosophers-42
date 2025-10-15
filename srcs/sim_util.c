/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:33:53 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 18:01:11 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	take_fork_even(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (is_dead(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	safe_print(philo, philo->args->start_time, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	if (is_dead(philo))
	{
		unlock_forks(philo);
		return (0);
	}
	safe_print(philo, philo->args->start_time, "has taken a fork");
	return (1);
}

int	take_fork_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (is_dead(philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	safe_print(philo, philo->args->start_time, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	if (is_dead(philo))
	{
		unlock_forks(philo);
		return (0);
	}
	safe_print(philo, philo->args->start_time, "has taken a fork");
	return (1);
}

void	update_full_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (!philo->is_full)
	{
		philo->is_full = true;
		philo->args->num_full_philo++;
	}
	pthread_mutex_unlock(philo->dead_mutex);
}

int	is_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(philo->dead_mutex);
	dead = *(philo->dead);
	pthread_mutex_unlock(philo->dead_mutex);
	return (dead);
}

int	return_verification(t_philo *philo, int n)
{
	if (n == 0)
	{
		if (is_dead(philo))
		{
			unlock_forks(philo);
			return (0);
		}
	}
	else
	{
		if (is_dead(philo))
			return (0);
	}
	return (1);
}
