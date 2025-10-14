#include "../includes/philosophers.h"

int	ft_atoi(char *str)
{
	int	res;
	int	sign;
	int	i;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}


long	get_time(void)
{
	struct timeval	time_v;
	gettimeofday(&time_v, NULL);
	return (time_v.tv_sec * 1000L + time_v.tv_usec / 1000L);
}

void precise_sleep(long duration)
{
	//long start;
	//start = get_time();
	//while (get_time() - start < duration)
	usleep(duration * 1000); // Sleep for 100 microseconds
}

void safe_print(t_philo *philo, long start_time, const char *msg)
{
    long timestamp;
	timestamp = get_time() - start_time;
	pthread_mutex_lock(philo->printing);
	pthread_mutex_lock(philo->dead_mutex);
	int dead = *(philo->dead);
	pthread_mutex_unlock(philo->dead_mutex);
	if (!dead || strcmp(msg, "died") == 0)
		printf("%ld %d %s\n", timestamp, philo->id + 1, msg);
	pthread_mutex_unlock(philo->printing);
}

void unlock_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void    update_last_meal(t_philo *philo)
{
    pthread_mutex_lock(philo->dead_mutex);
    philo->last_meal = get_time();
    pthread_mutex_unlock(philo->dead_mutex);
}
