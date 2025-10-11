#include "../includes/philosophers.h"

/* static void debug_print_philo(const t_philo *philo)
{
    printf("------ Philosopher Debug ------\n");
    printf("ID: %d\n", philo->id);
    printf("Meals eaten: %d\n", philo->meals_eaten);
    printf("Thread: %p\n", (void*)&(philo->thread));
    printf("Left fork (mutex): %p\n", (void*)philo->left_fork);
    printf("Right fork (mutex): %p\n", (void*)philo->right_fork);
    printf("Dead flag pointer: %p, value: %d\n", (void*)philo->dead, philo->dead ? *(philo->dead) : -1);
    printf("Dead mutex pointer: %p\n", (void*)philo->dead_mutex);
//    printf("Args pointer: %p\n", (void*)philo->args);
    printf("  num_of_phil: %d\n", philo->args.num_of_phil);
    printf("  count_die: %d\n", philo->args.count_die);
    printf("  count_eat: %d\n", philo->args.count_eat);
    printf("  count_sleep: %d\n", philo->args.count_sleep);
    printf("  needed_meals: %d\n", philo->args.needed_meals);
    printf("  start_time: %ld\n", philo->args.start_time);
    printf("-------------------------------\n");
}
 */
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
	setup_philo(args, &philos, forks);
//	if (init_sim());
//	int i = 0;
/* 	while (i < args.num_of_phil)
	{
		debug_print_philo(&philos[i]);
		i++;
	} */
	if (!start_sim(&args, philos))
		return (1);
	clean_struct(args, philos, forks);
	return (0);
}
