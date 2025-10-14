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
            // Stop when all philosophers are full
            if (philo->args->needed_meals > 0 && philo->args->num_full_philo == philo->args->num_of_phil)
            {
                pthread_mutex_unlock(philo[i].dead_mutex);
                return;
            }
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

int return_verification(t_philo *philo, int n)
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

void update_full_phillo(t_philo *philo)
{
    pthread_mutex_lock(philo->dead_mutex);
    if (!philo->is_full)
    {
        philo->is_full = true;
        philo->args->num_full_philo++;
    }
    pthread_mutex_unlock(philo->dead_mutex);
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int dead;
    int all_full;

    if (philo->id % 2 != 0)
        usleep(100);
    while (1)
    {
        pthread_mutex_lock(philo->dead_mutex);
        dead = *(philo->dead);
        all_full = (philo->args->needed_meals > 0 && philo->args->num_full_philo == philo->args->num_of_phil);
        pthread_mutex_unlock(philo->dead_mutex);
        if (dead || all_full)
            break;
        if (!pickup(philo))
            break ;
        if (return_verification(philo, 1) == 0)
            break;
        update_last_meal(philo);
        safe_print(philo, philo->args->start_time, "is eating");
        precise_sleep(philo->args->count_eat);
        philo->meals_eaten++;
        unlock_forks(philo);
        if (philo->args->needed_meals > 0 && philo->meals_eaten == philo->args->needed_meals && !philo->is_full)
            update_full_phillo(philo);
        if (return_verification(philo, 1) == 0)
            break;
        safe_print(philo, philo->args->start_time, "is sleeping");
        precise_sleep(philo->args->count_sleep);
        if (return_verification(philo, 1) == 0)
            break;
        safe_print(philo, philo->args->start_time, "is thinking");
        precise_sleep((philo->args->count_die - philo->args->count_sleep - philo->args->count_eat) / 4);
        if (return_verification(philo, 1) == 0)
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
            unlock_forks(philo);
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
            unlock_forks(philo);
            return (0);
        }
        safe_print(philo, philo->args->start_time, "has taken a fork");
    }
    update_last_meal(philo);
    return (1);
}
