/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:55:39 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 13:11:12 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	clean_struct(t_args args, t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < args.num_of_phil)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	pthread_mutex_destroy(philo->dead_mutex);
	free(philo->dead_mutex);
	pthread_mutex_destroy(philo->printing);
	free(philo->printing);
	free(philo->dead);
	free(philo);
}
