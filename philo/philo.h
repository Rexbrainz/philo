/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 08:45:44 by sudaniel          #+#    #+#             */
/*   Updated: 2024/12/23 14:14:20 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_philo
{
	pthread_t		thread_id;
	int				times_eaten;
	unsigned int	philo_id;
	pthread_mutex_t	l_chop_stick_mutex;
	pthread_mutex_t	r_chop_stick_mutex;
	struct s_philo	*left;
	struct s_philo	*right;
}	t_philo;

typedef struct s_general
{
	int				num_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				num_meals;
	struct s_philo	*table;

}	t_general;

int		ft_atoi(const char *str);
long	ft_atol(const char *str);

#endif
