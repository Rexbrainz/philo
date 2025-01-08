/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:16:00 by sudaniel          #+#    #+#             */
/*   Updated: 2025/01/08 06:37:27 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * checks if the number of meals was specified, if it was,
 * it increments times eaten for the philo, and checks if the philo
 * has eaten at least the amount of num_meals specified, if true,
 * increments ate_max_meal. It returns 1 if num_meals was not specified,
 * or if it was specified and no error case occured, in case of errors
 * 0 is returned.
 */
int	check_num_meals(t_philo *philo)
{
	if (philo->info->num_meals != -1)
	{
		philo->times_eaten++;
		if (pthread_mutex_lock(&philo->info->args_mutex))
		{
			printf("\nError: Mutex lock failed.\n");
			return (0);
		}
		if (philo->times_eaten == philo->info->num_meals)
			philo->info->ate_max_meal++;
		if (pthread_mutex_unlock(&philo->info->args_mutex))
		{
			printf("\nError: Mutex unlock failed.\n");
			return (0);
		}
	}
	return (1);
}

/*
 * A custom function that unlocks mutexes. Returns 0 in cases of an error,
 * else returns 1.
 */
int	unlock_mutexes(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
	if (mutex1)
	{
		if (pthread_mutex_unlock(mutex1))
		{
			printf("\nError: Mutex unlock failed.\n");
			return (0);
		}
	}
	if (mutex2)
	{
		if (pthread_mutex_unlock(mutex2))
		{
			printf("\nError: Mutex unlock failed.\n");
			return (0);
		}
	}
	return (1);
}

/*
 * Checks if the simulation needs to stop or not, it locks the args mutex,
 * and checks if any philosopher is dead or if they have all eaten
 * at least the number of meals specified. Returns true if simulation
 * must stop, else returns false.
 */
bool	must_simulation_stop(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->info->args_mutex))
	{
		printf("\nError: Mutex lock failed.\n");
		return (true);
	}
	if (philo->info->is_philo_dead)
	{
		pthread_mutex_unlock(&philo->info->args_mutex);
		return (true);
	}
	if (philo->info->have_all_philos_eaten_max_meal)
	{
		pthread_mutex_unlock(&philo->info->args_mutex);
		return (true);
	}
	if (pthread_mutex_unlock(&philo->info->args_mutex))
	{
		printf("\nError: Mutex unlock failed.\n");
		return (true);
	}
	return (false);
}

/*
 * Puts the philo to sleep for milliseconds time, the philosopher wakes
 * regularly to check if the simulation must end. In error cases or if 
 * the simulation must end, 0 is returned else 1 is returned.
 */
int	philo_sleeps(t_philo *philo, time_t milliseconds)
{
	time_t	wake_up_alarm;
	time_t	start_sleep_alarm;

	start_sleep_alarm = get_time_ms();
	if (!start_sleep_alarm)
	{
		printf("\nError: get_time_ms failed in philo_sleeps.\n");
		return (0);
	}
	while (true)
	{
		usleep(250);
		if (must_simulation_stop(philo))
			return (0);
		wake_up_alarm = get_time_ms() - start_sleep_alarm;
		if (wake_up_alarm < 0)
		{
			printf("\nError: get_time_ms failed in philo_sleeps.\n");
			return (0);
		}
		if (wake_up_alarm >= milliseconds)
			break ;
	}
	return (1);
}

/*
 * Reports what the philospher is doing at a specific time, it locks the
 * mutex to that, this is to prevent possible interleaving of printings.
 * it returns 0 in cases of errors else, 1.
 * Most pthread fuctions return 0 on success and non zero int on failure,
 * this is why almost all call of pthread functions are tested for
 * errors.
 */
int	report_philo_state(t_philo *philo, const char *s)
{
	if (pthread_mutex_lock(&philo->info->args_mutex))
	{
		printf("\nError: Mutex lock failed.\n");
		return (0);
	}
	printf("%lu %d %s\n", get_time_ms() - philo->info->sim_start_time,
		philo->philo_id, s);
	if (pthread_mutex_unlock(&philo->info->args_mutex))
	{
		printf("\nError: Mutex unlock failed.\n");
		return (0);
	}
	return (1);
}
