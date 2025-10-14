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
            if (philo->args->num_full_philo == philo->args->num_of_phil)
                return ;
            if (time_since_meal > philo[i].args->count_die)
            {
                // printf("DEBUG: Philo %d died - time since last meal: %ld ms (limit: %d ms)\n",
                //   philo[i].id + 1, time_since_meal, philo[i].args->count_die);
                *(philo[i].dead) = 1;
                /* unlock the dead mutex before printing to avoid lock ordering deadlocks
                   and to allow safe_print to check the flag itself */
                pthread_mutex_unlock(philo[i].dead_mutex);
                safe_print(&philo[i], philo[i].args->start_time, "died");
                return;
            }
            pthread_mutex_unlock(philo[i].dead_mutex);
            i++;
        }
        usleep(100); // Check every 1ms
    }
}

void join_thread(t_philo *philo, int num_of_phil)
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

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2 != 0)
        usleep(100);
    while (!*(philo->dead) || !(philo->args->num_full_philo == philo->args->num_of_phil))
    {
        if (*(philo->dead))
            break;
        if (!pickup(philo))
            break ;
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
        if (philo->meals_eaten == philo->args->needed_meals && !philo->is_full)
        {
            philo->is_full = true;
            philo->args->num_full_philo++;
            // break ;
        }
        if (*(philo->dead))
            break;
        safe_print(philo, philo->args->start_time, "is sleeping");
        precise_sleep(philo->args->count_sleep);
        if (*(philo->dead))
            break;
        safe_print(philo, philo->args->start_time, "is thinking");
        precise_sleep((philo->args->count_die - philo->args->count_sleep - philo->args->count_eat) / 4);
        if (*(philo->dead))
            break;
    }
    return (NULL);
}

int pickup(t_philo *philo)
{
    if (philo->left_fork == philo->right_fork)
    {
        safe_print(philo, philo->args->start_time, "has taken a fork");
        return (0);
    }
    if (philo->id % 2 == 0)
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
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            return (0);
        }
        safe_print(philo, philo->args->start_time, "has taken a fork");
    }
    else
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
            pthread_mutex_unlock(philo->left_fork);
            pthread_mutex_unlock(philo->right_fork);
            return (0);
        }
        safe_print(philo, philo->args->start_time, "has taken a fork");
    }
    pthread_mutex_lock(philo->dead_mutex);
    philo->last_meal = get_time();
    pthread_mutex_unlock(philo->dead_mutex);
    return (1);
}
