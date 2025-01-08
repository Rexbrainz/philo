/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 08:45:44 by sudaniel          #+#    #+#             */
/*   Updated: 2025/01/07 07:59:25 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

/*
 * s_shared contains information that are common to all philosophers
 * when created.
 * ate_max_meal:	Every philosopher increments it when they have eaten
 * 					at least the number of meals when specified(num_meals).
 * args_mutex:		Used to allow access to threads reading and writing
 * 					to the variables in the shared struct.
 * is_philo_dead
 * and 
 * have_all_philos_eaten_max_meal:
 * 					These are flags every philosopher reads/checks to
 * 					know if the simulation should stop or not.
 * 					The main thread is reponsible for setting these flags.
 * table:			table is a pointer to the head seat(node) of a
 * 					circular linked list.
 */
typedef struct s_shared
{
	unsigned int	num_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				num_meals;
	time_t			sim_start_time;
	unsigned int	ate_max_meal;
	pthread_mutex_t	args_mutex;
	bool			is_philo_dead;
	bool			have_all_philos_eaten_max_meal;
	struct s_philo	*table;
}					t_shared;

/*
 * thread_id:	Every philosopher upon created is assigned an id by the 
 * 				pthread_create funtion.
 * philo_id:	Every philo is given an id ranging from 1 to num_philos.
 * l_chopstick_mutex and r_chopstick_mutex
 * 				Each philosopher must acqurire a left and right chopstick
 * 				before they can eat. Notice that r_chopstick_mutex is
 * 				a pointer, since the number of chopsticks equals the number
 * 				of philosophers.
 * times_eaten:	Each philospher reports the number of times it has eaten
 * 				this is necessary when number of meals to eat is specified.
 * last_meal_time:
 * 				The philosphers would starve if they do not eat before 
 * 				time_to_die, therefor, everytime a philospher eats, he
 * 				needs to record this time, which would be used to check
 * 				against time_to_die the next time he eats.
 * info:		Each philospher gets a pointer to the shared info, this is
 * 				important, because they need to write to the ate_max_meal
 * 				variable and need to read/check the is_philo_dead and
 * 				have_all_philos_eaten_max_meal variables.
 * left and right:
 * 				Each philosopher has a pointer to his left and right 
 * 				philosphers, this is because we are implementing a 
 * 				circular doubly linked list. Yes, the philosophers
 * 				do not communicate.
 */
typedef struct s_philo
{
	pthread_t		thread_id;
	unsigned int	philo_id;
	pthread_mutex_t	l_chopstick_mutex;
	pthread_mutex_t	*r_chopstick_mutex;
	int				times_eaten;
	time_t			last_meal_time;
	t_shared		*info;
	struct s_philo	*left;
	struct s_philo	*right;
}					t_philo;

//	ft_atoi:	Converts the numerical characters in a string (str) to integers
//				and returns the integer.
int		ft_atoi(const char *str);

//	ft_atol:	Similar to ft_atoi, but retruns a long integer.
long	ft_atol(const char *str);

// get_time_ms:	Returns the time of day in milliseconds upon success,
// 				else it returns 0.
time_t	get_time_ms(void);

//	make_table:	Creates a circular doubly linked list, returns NULL
//				if anything fails else, returns a pointer to the created list.
t_philo	*make_table(t_shared *info);

//	destroy_mutex_and_free_table:	Destroys all mutexes initialized 
//									in the shared info and nodes in the
//									circular linked list.
//									It returns nothing.
void	destroy_mutex_and_free_table(t_shared *info);

// create_philos:	Creates threads for the specified number of philos, amd
// 					assigns the threads to their start routine.
// 					Returns 0 if thread creation fails else returns 1.
int		create_philos(t_philo *table);

// philo_sleeps:	A philosopher sleeps milliseconds time, waking up
// 					regularly for a short while to check if the simulation
// 					must stop or not. If the simulation must stop or an
// 					error case occured it returns 0, else it returns 1.
int		philo_sleeps(t_philo *philo, time_t milliseconds);

// must_simulation_stop:	Returns true if the simulation must stop,
//							it returns false.
bool	must_simulation_stop(t_philo *philo);

//	report_philo_state:	Prints the state of a philosopher, which is
//						either eating, sleeping, thinking or when he
//						grabs a chopstick. It returns 0 on error case,
//						else returns 1.
int		report_philo_state(t_philo *philo, const char *s);

//	unlock_mutex:	It takes the address of two mutexes and unlocks them.
//					Returns 0 in cases of error, else 1.
int		unlock_mutexes(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2);

// check_num_meals:	checks if the num_meals was specified or not.
// 					Returns 0 in case of errors, and 1 if everything is fine.
int		check_num_meals(t_philo *philo);

// referee:	This is the main thread, it simply monitors the simulation
// 			and reports when a philosopher dies or if they have all
// 			eaten the required number of meals.
void	referee(t_shared *info);

#endif
