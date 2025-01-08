/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philos.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:00:33 by sudaniel          #+#    #+#             */
/*   Updated: 2025/01/08 06:40:40 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Each philo, takes both chopsticks and reports they have
 * taken a chopstick.
 * If an error occures, 0 is returned, else 1 is returned.
 * There would be easier ways to handle error cases instead
 * of wraping every call of the pthread functions in an if statement.
 */
static int	take_chopsticks(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->l_chopstick_mutex))
	{
		printf("Error: Chopstick mutex lock failed.\n");
		return (0);
	}
	if (must_simulation_stop(philo))
		return (0);
	report_philo_state(philo, "has taken chopstick.");
	if (philo->info->num_philos == 1)
	{
		philo_sleeps(philo, philo->info->time_to_die);
		return (0);
	}
	if (pthread_mutex_lock(philo->r_chopstick_mutex))
	{
		pthread_mutex_unlock(&philo->l_chopstick_mutex);
		printf("Error: Chopstick mutex lock failed.\n");
		return (0);
	}
	if (must_simulation_stop(philo))
		return (0);
	report_philo_state(philo, "has taken a chopstick.");
	return (1);
}

/*
 * The philospher sleeps for time_to_sleep, when he wakes up,
 * he checks if the simulation must stop, if it must stop
 * he returns 0, else he moves on to think.
 * time_to_think, is used to regulate how much time a philosopher
 * thinks. This is important for
 * synchronization, because if a philospher can spare some
 * time to think and not starve, then that allows the other, the time
 * to grab the chopsticks and eat. It is calculated by dividing
 * the result of subtracting the time that has elapsed since the last meal
 * of a philosopher (this is roughly the sum of time_to_eat + time_to_sleep)
 * from the time_to_die.
 * If this time is less than or equal 10, he thinks for 0ms, if this
 * time is greater than or equal to 200, he thinks for 100ms else
 * he thinks for the calculated time.
 * why is it divided by 2? A philosopher is blocked, if he cannot lock
 * a mutex, this is managed by the operating system, therefore, it
 * is not known how long he is blocked for, to account for this possibility
 * it is divided by 2.
 */
static int	philo_sleeps_then_thinks(t_philo *philo)
{
	time_t	time_to_think;

	if (must_simulation_stop(philo))
		return (0);
	if (!report_philo_state(philo, "is sleeping."))
		return (0);
	philo_sleeps(philo, philo->info->time_to_sleep);
	if (must_simulation_stop(philo))
		return (0);
	time_to_think = (philo->info->time_to_die
			- (get_time_ms() - philo->last_meal_time)) / 2;
	if (time_to_think >= 200)
		time_to_think = 100;
	else if (time_to_think <= 10)
		time_to_think = 0;
	if (!report_philo_state(philo, "is thinking."))
		return (0);
	philo_sleeps(philo, time_to_think);
	if (must_simulation_stop(philo))
		return (0);
	return (1);
}

/*
 * A philosopher needs both chopsticks to his left and right to eat.
 * He acquires them and eats, before he eats, his start of last meal time
 * is recorded, he reports he is eating and he eats for time to eat.
 * After eating he unlocks the mutexes. If number of meals was specified,
 * then he needs to update times_eaten and if he has eaten number
 * of meals every philopher must eat, In the check_num_meals function
 * he locks the ate_max_meal_mutex, records that he has eaten max_meal times,
 * and unlocks the mutex.
 */
static int	philo_eats(t_philo	*philo)
{
	if (!take_chopsticks(philo))
		return (0);
	pthread_mutex_lock(&philo->info->args_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->info->args_mutex);
	if (must_simulation_stop(philo))
		return (0);
	if (!report_philo_state(philo, "is eating."))
	{
		pthread_mutex_unlock(philo->r_chopstick_mutex);
		pthread_mutex_unlock(&philo->l_chopstick_mutex);
		return (0);
	}
	if (!philo_sleeps(philo, philo->info->time_to_eat))
	{
		pthread_mutex_unlock(philo->r_chopstick_mutex);
		pthread_mutex_unlock(&philo->l_chopstick_mutex);
		return (0);
	}
	if (!unlock_mutexes(philo->r_chopstick_mutex, &philo->l_chopstick_mutex))
		return (0);
	if (!check_num_meals(philo))
		return (0);
	return (1);
}

/*
 * The simulation proper. Every even numbered philospher is delayed a bit.
 * This is the primary form of synchronization to avoid deadlock, as it
 * gives a little form of control over competition for the chopsticks.
 * The philosphers go to eat, sleep and think. Upon completion of the
 * simulation they return to the main thread where they are joined.
 */
static void	*simulation(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)
		if (!philo_sleeps(philo, 5))
			return (NULL);
	while (true)
	{
		if (!philo_eats(philo))
			break ;
		if (!philo_sleeps_then_thinks(philo))
			break ;
	}
	return (NULL);
}

/*
 * For every seat(node) at the table, a thread (philosopher) is created
 * and assigned the simulation function as its starting routine, the seat
 * or node is passed as argument to the simulation function.
 * If thread creation fails, 0 is returned, else, 1 is returned.
 */
int	create_philos(t_philo *table)
{
	t_philo	*seat;

	seat = table;
	while (true)
	{
		if (pthread_create(&seat->thread_id, NULL, simulation, (void *)seat))
		{
			printf("Error: pthread_create failed.\n");
			return (0);
		}
		seat = seat->right;
		if (seat == table)
			break ;
	}
	return (1);
}
