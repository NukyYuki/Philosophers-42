/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 16:45:18 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 18:25:09 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	start_sim(t_args *args, t_philo *philo)
{
	args->start_time = get_time();
	set_start_time(args, &philo);
	if (!thread_create(philo, args->num_of_phil))
		return (0);
	monitor(philo, args->num_of_phil);
	join_thread(philo, args->num_of_phil);
	return (1);
}

int	thread_create(t_philo *philo, int num_of_phil)
{
	int	i;

	i = 0;
	while (i < num_of_phil)
	{
		if (pthread_create(&philo[i].thread, NULL,
				philo_routine, &philo[i]) != 0)
			return (write(1, "Error creating thread", 22), 0);
		i++;
	}
	return (1);
}

void	monitor(t_philo *ph, int n_ph)
{
	int		i;
	long	time_since_meal;

	while (1)
	{
		i = 0;
		while (i < n_ph)
		{
			pthread_mutex_lock(ph[i].dead_mutex);
			time_since_meal = get_time() - ph[i].last_meal;
			if (ph->args->needed_meals > 0 && ph->args->num_full_philo == n_ph)
			{
				pthread_mutex_unlock(ph[i].dead_mutex);
				return ;
			}
			if (time_since_meal > ph[i].args->count_die)
			{
				handle_monitor_exit(&ph[i]);
				return ;
			}
			pthread_mutex_unlock(ph[i].dead_mutex);
			i++;
		}
		usleep(100);
	}
}

void	join_thread(t_philo *philo, int num_of_phil)
{
	int	i;

	i = 0;
	while (i < num_of_phil)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*ph;
	int		dead;
	int		all_full;

	ph = (t_philo *)arg;
	if (ph->id % 2 != 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(ph->dead_mutex);
		dead = *(ph->dead);
		all_full = (ph->args->needed_meals > 0 && ph->args->num_full_philo
				== ph->args->num_of_phil);
		pthread_mutex_unlock(ph->dead_mutex);
		if (!break2_routine(ph, dead, all_full))
			break ;
		if (ph->args->needed_meals > 0 && ph->meals_eaten
			== ph->args->needed_meals && !ph->is_full)
			update_full_philo(ph);
		if (!break_routine(ph))
			break ;
	}
	return (NULL);
}
