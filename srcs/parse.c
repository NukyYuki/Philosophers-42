/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 16:58:11 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 17:00:32 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	parse_args(int ac, char **av, t_args *args)
{
	args->num_of_phil = ft_atoi(av[1]);
	args->count_die = ft_atoi(av[2]);
	args->count_eat = ft_atoi(av[3]);
	args->count_sleep = ft_atoi(av[4]);
	args->needed_meals = -1;
	args->num_full_philo = 0;
	if (ac == 6)
	{
		args->needed_meals = ft_atoi(av[5]);
		if (args->needed_meals <= 0)
			return (write(1, "Invalid int value\n", 19), 1);
	}
	if (args->num_of_phil <= 0 || args->count_die <= 0
		|| args->count_eat <= 0 || args->count_sleep <= 0)
	{
		return (write(1, "Invalid int value\n", 19), 1);
	}
	return (0);
}
