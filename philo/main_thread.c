/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 06:34:01 by sudaniel          #+#    #+#             */
/*   Updated: 2025/01/08 06:39:19 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Locks the args_mutex, but checks if the pthread function 
 * returned an error code. If so an error message is 
 * printed the is_philo_dead bool flag is set to true,
 * and true is returned. else false is returned. 
 * This is simply to end the simulation. Perhaps an error bool
 * variable could be used instead of flagging the is_philo_dead true.
 */
static bool	check_mutex_lock_error(t_shared *info)
{
	if (pthread_mutex_lock(&info->args_mutex))
	{
		printf("\nError: Mutex lock failed.\n");
		info->is_philo_dead = true;
		return (true);
	}
	return (false);
}

/*
 * Similar to the check_mutex_lock_error function,
 * it simply unlocks the locked mutex.
 */
static bool	check_mutex_unlock_error(t_shared *info)
{
	if (pthread_mutex_unlock(&info->args_mutex))
	{
		printf("\nError: Mutex unlock failed.\n");
		info->is_philo_dead = true;
		return (true);
	}
	return (false);
}

/*
 * Checks if any philosopher is dead or if they have all eaten the
 * required amount of meal, to do this he needs to lock the args
 * mutex which is done in the check_mutex_lock_error function, and 
 * unlocks the same using the check_mutex_unlock_error function.
 * This is done to simply keep the function readable.
 */
static void	check_death_or_all_philo_full(t_shared *info)
{
	t_philo	*philo;

	philo = info->table;
	while (true)
	{
		if (check_mutex_lock_error(info))
			break ;
		if (get_time_ms() - philo->last_meal_time >= info->time_to_die)
		{
			info->is_philo_dead = true;
			pthread_mutex_unlock(&info->args_mutex);
			report_philo_state(philo, "died.");
			break ;
		}
		if (info->ate_max_meal == info->num_philos)
		{
			info->have_all_philos_eaten_max_meal = true;
			printf("All philos have eaten %d meals.\n", info->num_meals);
			pthread_mutex_unlock(&info->args_mutex);
			break ;
		}
		if (check_mutex_unlock_error(info))
			break ;
	}
}

/*
 * This is the main thread, he simply referees the simulation by monitoring
 * if any of the philosophers are dead or if they have all eaten the 
 * required amount of meals.
 * When the simulation ends, he simply waits to join the threads together
 * which automatically detaches the threads.
 */
void	referee(t_shared *info)
{
	t_philo	*current;

	check_death_or_all_philo_full(info);
	current = info->table;
	while (true)
	{
		pthread_join(current->thread_id, NULL);
		current = current->right;
		if (current == info->table)
			break ;
	}
}
