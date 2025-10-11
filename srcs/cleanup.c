#include "../includes/philosophers.h"

void    clean_struct(t_args args, t_philo *philo, pthread_mutex_t *forks)
{
    int i;

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
