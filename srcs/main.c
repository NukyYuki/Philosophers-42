/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:36:40 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/15 12:55:29 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int ac, char **av)
{
	t_args			args;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if (ac != 5 && ac != 6)
		return (write(1, "Wrong Number of arguments\n", 27), 1);
	if (parse_args(ac, av, &args) != 0)
		return (1);
	setup_forks(&forks, args.num_of_phil);
	setup_philo(&args, &philos, forks);
	if (!start_sim(&args, philos))
		return (1);
	clean_struct(args, philos, forks);
	return (0);
}
