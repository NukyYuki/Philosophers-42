#include "philosophers.h"

int start_sim(t_args *args, t_philo *philo)
{
    args->start_time = get_time();
    set_start_time(args, &philo);
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

/* void    monitor(t_philo *philo, int num_phil)
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
} */
void monitor(t_philo *philo, int num_phil)
{
    int i;
    while (1)
    {
        i = 0;
        while (i < num_phil)
        {
            pthread_mutex_lock(philo[i].dead_mutex);
            long time_since_meal = get_time() - philo[i].last_meal;
            if (time_since_meal > philo[i].args->count_die)
            {
                printf("DEBUG: Philo %d died - time since last meal: %ld ms (limit: %d ms)\n", 
                       philo[i].id + 1, time_since_meal, philo[i].args->count_die);
                *(philo[i].dead) = 1;
                safe_print(&philo[i], philo[i].args->start_time, "died");
                pthread_mutex_unlock(philo[i].dead_mutex);
                return;
            }
            pthread_mutex_unlock(philo[i].dead_mutex);
            i++;
        }
        usleep(1000); // Check every 1ms
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

int is_dead(t_philo *philo)
{
    int dead;
    pthread_mutex_lock(philo->dead_mutex);
    dead = *(philo->dead);
    pthread_mutex_unlock(philo->dead_mutex);
    return dead;
}

void    *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (is_dead(philo))
        return (NULL);
    while (!*(philo->dead))
    {
        safe_print(philo, philo->args->start_time, "is thinking");
        if (*(philo->dead))
            break ;
		pickup(philo);
        if (*(philo->dead)) 
        {
            pthread_mutex_unlock(philo->left_fork);
            pthread_mutex_unlock(philo->right_fork);
            break;
        }
		pthread_mutex_lock(philo->dead_mutex);
        philo->last_meal = get_time();
        pthread_mutex_unlock(philo->dead_mutex);
		safe_print(philo, philo->args->start_time, "is eating");
		precise_sleep(philo->args->count_eat);
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
        if (*(philo->dead))
            break ;
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
    pthread_mutex_lock(philo->dead_mutex);
    philo->last_meal = get_time();
    pthread_mutex_unlock(philo->dead_mutex);
}
