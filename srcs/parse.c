/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 16:58:11 by mipinhei          #+#    #+#             */
/*   Updated: 2025/10/16 10:28:45 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	parse_args(int ac, char **av, t_args *args)
{
	if (!check_values(ac, av))
		return (1);
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

int	check_values(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		if (!is_valid_int32(av[i]))
			return (write(1, "Invalid int value\n", 19), 0);
		j = 0;
		if (av[i][j] == '+' || av[i][j] == '-')
			j++;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (write(1, "Invalid int value\n", 19), 0);
			j++;
		}
		i++;
	}
	return (1);
}

int	is_valid_int32(char *str)
{
	int			i;
	int			sign;
	long long	res;
	int			has_digit;

	if (!str || !*str)
		return (0);
	init_valid_int32(&i, &sign, &res, &has_digit);
	skip_whitespace(str, &i, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		has_digit = 1;
		res = res * 10 + (str[i] - '0');
		if (sign == 1 && res > INT_MAX)
			return (0);
		if (sign == -1 && res > (long long)INT_MAX + 1)
			return (0);
		i++;
	}
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (!has_digit || str[i] != '\0')
		return (0);
	return (1);
}

void	init_valid_int32(int *i, int *sign, long long *res, int *has_digit)
{
	*i = 0;
	*sign = 1;
	*res = 0;
	*has_digit = 0;
}

void	skip_whitespace(char *str, int *i, int *sign)
{
	int	j;

	j = *i;
	while ((str[j] >= 9 && str[j] <= 13) || str[j] == 32)
		j++;
	if (str[j] == '+' || str[j] == '-')
	{
		if (str[j] == '-')
			*sign = -1;
		j++;
	}
	*i = j;
}
