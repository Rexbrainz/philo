/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 08:44:34 by sudaniel          #+#    #+#             */
/*   Updated: 2025/01/07 07:06:04 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * get_time_ms:	Uses the gettimeofday function which sets the time_tv.sec and
 * 				usec members. It returns the number of milliseconds that has
 * 				elapsed since January 1st, 1970. if gettimeofday returns -1
 * 				which denotes an error to gettimeofday, 0 is returned.
 * 				gettimeofday returns 0 upon success.
 */
time_t	get_time_ms(void)
{
	time_t			current_time_in_milliseconds;
	struct timeval	time;

	if (gettimeofday(&time, NULL))
	{
		printf("Error: gettimeofday failed.\n");
		return (0);
	}
	current_time_in_milliseconds = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (current_time_in_milliseconds);
}

/*
 * Initializes the shared info struct with the valid input.
 * If the number of philosophers or number of meals is zero or
 * if any of the mutex initializations fails, false is returned, 
 * else true is returned signifying that the initialization was done
 * successfully.
 */
static bool	init_args(t_shared *info, char **argv)
{
	info->num_philos = ft_atoi(argv[0]);
	if (info->num_philos == 0)
		return (false);
	info->time_to_die = ft_atol(argv[1]);
	info->time_to_eat = ft_atol(argv[2]);
	info->time_to_sleep = ft_atol(argv[3]);
	if (argv[4])
	{
		info->num_meals = ft_atoi(argv[4]);
		if (!info->num_meals)
			return (false);
	}
	else
		info->num_meals = -1;
	info->ate_max_meal = 0;
	info->sim_start_time = get_time_ms();
	if (!info->sim_start_time)
		return (false);
	if (pthread_mutex_init(&info->args_mutex, NULL))
		return (false);
	info->is_philo_dead = false;
	info->have_all_philos_eaten_max_meal = false;
	info->table = NULL;
	return (true);
}

/*
 * Checks if the inputs are non numerical, since any value entered
 * must be a positive value, the char '-' is considered as an error,
 * and false is returned.
 * If everything is fine after calling the init_args function, true
 * is returned.
 */
static bool	check_args_and_init(char **argv, t_shared *info)
{
	int		i;
	char	*args;

	i = 0;
	while (argv[i])
	{
		args = argv[i];
		while (*args)
		{
			if (*args == '+')
				args++;
			if ('0' > *args || *args > '9')
				return (false);
			args++;
		}
		i++;
	}
	if (!init_args(info, argv))
		return (false);
	return (true);
}

/*
 * Declares the shared info struct data type, validates the user input,
 * returns an error code if the argument count does not match the required
 * or the inputs are invalid. Creates a circlular linked list to mimic
 * a round table, if making the table fails, an error code is returned.
 * The Philosophers(threads) are created and if an error occurs at any point
 * The already made table and the mutexes in them are destroyed and freed,
 * and an error code is returned.
 * Upon creating the philosophers, the simulation starts, the main thread
 * proceeds to refereeing the simulation, he would flag the end of 
 * simulation when a philosopher starves or they have all had at least the
 * number of meals required.
 * When the simulation ends, all mutexes are destroyed and the
 * list (table) freed.
 */
int	main(int argc, char **argv)
{
	t_shared	info;

	if (argc < 5 || argc > 6 || !check_args_and_init(argv + 1, &info))
	{
		printf("Error: Invalid Input or mutex initialization failed.\n");
		return (1);
	}
	info.table = make_table(&info);
	if (!info.table)
		return (1);
	if (!create_philos(info.table))
	{
		destroy_mutex_and_free_table(&info);
		return (1);
	}
	referee(&info);
	destroy_mutex_and_free_table(&info);
	return (0);
}
