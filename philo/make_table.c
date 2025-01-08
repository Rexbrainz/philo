/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudaniel <sudaniel@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:09:01 by sudaniel          #+#    #+#             */
/*   Updated: 2025/01/07 07:18:11 by sudaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * destroys all the initialized mutexes and frees all the nodes(seats) in 
 * the circular doubly linked list.
 */
void	destroy_mutex_and_free_table(t_shared *info)
{
	t_philo	*current;

	current = info->table->right;
	while (current != info->table)
	{
		pthread_mutex_destroy(&current->l_chopstick_mutex);
		current = current->right;
		free(current->left);
	}
	pthread_mutex_destroy(&info->table->l_chopstick_mutex);
	free(info->table);
	pthread_mutex_destroy(&info->args_mutex);
}

/*
 * A new_node created represents memory areas (seat at the table) 
 * for each of the philosophers.
 * If malloc fails or initialization of mutex fails, NULL is returned, else
 * a pointer to the node created is returned.
 */
static t_philo	*create_seat(t_shared *info, int i)
{
	t_philo	*new_node;

	new_node = (t_philo *)malloc(sizeof(t_philo));
	if (!new_node)
	{
		printf("Error: Malloc failed.\n");
		return (NULL);
	}
	new_node->philo_id = i;
	if (pthread_mutex_init(&new_node->l_chopstick_mutex, NULL))
	{
		printf("Error: Mutex initialization failed.\n");
		return (NULL);
	}
	new_node->times_eaten = 0;
	new_node->last_meal_time = info->sim_start_time;
	new_node->info = info;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

/*
 * Each seat created is added to the table which is a circular doubly
 * linked list.
 */
static void	
	add_seat_to_table(t_philo *new_seat, t_philo **table, t_philo **tail)
{
	if (!*table)
	{
		*table = new_seat;
		*tail = new_seat;
	}
	else
	{
		new_seat->left = *tail;
		(*tail)->right = new_seat;
		*tail = new_seat;
	}
}

/*
 * Every seat on the table is assigned a right chopstick (mutex). This is to
 * help with competition and synchronization.
 * As each chopstick on the table is shared only between two philosophers. 
 * It is assumed that every chopstick to the left of a philosopher's seat
 * belongs to the philospher and he would need to acquire the chopstick
 * to his right to eat.
 * If the number of philosphers is just 1, obviously he cannot eat, we
 * assign the right chopstick to NULL.
 */
static void	assign_right_chopsticks(t_philo *table, int num_philos)
{
	t_philo	*current;

	current = table;
	while (true)
	{
		if (num_philos == 1)
		{
			current->r_chopstick_mutex = NULL;
			break ;
		}
		current->r_chopstick_mutex = &current->right->l_chopstick_mutex;
		current = current->right;
		if (current == table)
			break ;
	}
}

/*
 * A node(seat) is created based on the number of philosophers specified.
 * It declares a circular doubly linked list table, seat for each
 * philosopher on the table and tail to keep track of the last seat(node)
 * in the list. Keeping track of the last node in the list makes adding 
 * to the end of the list a lot easier as we do not have to always loop
 * through the list searching for the last node when we want to add a node
 * to the list.
 * If the new_seat returned is NULL, all mutexes are destroyed, all nodes
 * in the list freed, and NULL is returned.
 * Else the created node(seat) is added to the list.
 * When done, a pointer to the head of the list is returned.
 */
t_philo	*make_table(t_shared *info)
{
	t_philo			*table;
	t_philo			*new_seat;
	t_philo			*tail;
	unsigned int	i;

	table = NULL;
	tail = NULL;
	i = 1;
	while (i <= info->num_philos)
	{
		new_seat = create_seat(info, i++);
		if (!new_seat)
		{
			destroy_mutex_and_free_table(info);
			return (NULL);
		}
		add_seat_to_table(new_seat, &table, &tail);
	}
	tail->right = table;
	table->left = tail;
	assign_right_chopsticks(table, info->num_philos);
	return (table);
}
