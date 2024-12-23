/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 08:44:34 by sudaniel          #+#    #+#             */
/*   Updated: 2024/12/23 14:18:30 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error(const char *s)
{
	printf("%s\n", s);
	exit(EXIT_SUCCESS);
}

static void	check_args(char **argv, t_general *info)
{
	int		i;
	char	*args;

	i = 0;
	while (argv[i])
	{
		args = argv[i];
		while (*args)
		{
			if (*args < '0' || *args > '9')
				error("Error: Invalid input.");
			args++;
		}
		i++;
	}
	info->num_philos = ft_atoi(argv[0]);
	info->time_to_die = ft_atol(argv[1]);
	info->time_to_eat = ft_atol(argv[2]);
	info->time_to_sleep = ft_atol(argv[3]);
	if (i == 5)
		info->num_meals = ft_atoi(argv[4]);
	info->num_meals = -1;
}

int	main(int argc, char **argv)
{
	t_general	info;

	if (argc < 5 || argc > 6)
		error("Error: Invalid Input, 5 or 6 argument counts.");
	check_args(argv + 1, &info);
}
