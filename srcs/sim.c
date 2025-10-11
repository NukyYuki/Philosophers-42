#include "philosophers.h"

int start_sim(t_args *args, t_philo *philo)
{
    args->start_time = get_time();
    set_start_time(*args, &philo);
    if (!thread_create(philo, args->num_of_phil))
        return (0);
    monitor(philo, args->num_of_phil);
    join_thread(philo, args->num_of_phil);
    return (1);
}

int thread_create(t_philo *philo, int num_of_phil)
{
    int i;

    i = 0;
    while (i < num_of_phil)
    {
        if (pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]) != 0)
            return (write(1, "Error creating thread", 22), 0);
        i++;
    }
    return (1);
}

void    monitor(t_philo *philo, int num_phil)
{
    int end;
    int i;

    end = 1;
    i = 0;
    while (end)
    {
        i = 0;
        while (i < num_phil)
        {
          pthread_mutex_lock(philo[i].dead_mutex);
			if (get_time() - philo[i].last_meal > philo[i].args->count_die)
			{
				*(philo[i].dead) = 1;
				safe_print(&philo[i], philo->args->start_time, "died");
				pthread_mutex_unlock(philo[i].dead_mutex);
				return;
			}
			pthread_mutex_unlock(philo[i].dead_mutex);
            i++;
        }
        usleep(1000);
    }
}

void    join_thread(t_philo *philo, int num_of_phil)
{
    int i;

    i = 0;
    while (i < num_of_phil)
    {
        pthread_join(philo[i].thread, NULL);
        i++;
    }
}

void    *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (!*(philo->dead))
    {
        safe_print(philo, philo->args->start_time, "is thinking");
		// Pick up forks (lock left then right)
		pickup(philo);
		philo->last_meal = get_time();
		safe_print(philo, philo->args->start_time, "is eating");
		precise_sleep(philo->args->count_eat);
		philo->meals_eaten++;
		// Put down forks
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		// Sleep
		safe_print(philo, philo->args->start_time, "is sleeping");
		precise_sleep(philo->args->count_sleep);
    }
    return NULL;
}

void    pickup(t_philo  *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->left_fork);
		safe_print(philo, philo->args->start_time, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		safe_print(philo, philo->args->start_time, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
		safe_print(philo, philo->args->start_time, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		safe_print(philo, philo->args->start_time, "has taken a fork");
    }
}
