/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_creation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 19:18:12 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/28 14:00:28 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	valid_args(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (!is_num(av[i]))
			return (0);
		i++;
	}
	if (av[5] && ft_atoi(av[5]) < 1)
		return (0);
	return (1);
}

t_philo	*ft_create_elem(int id)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (NULL);
	new->id = id;
	new->last_meal = get_current_time();
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	ft_addlast(t_var *vars, int id)
{
	t_philo	*a;

	a = ft_create_elem(id);
	if (!a)
		return ;
	a->next = NULL;
	a->prev = NULL;
	a->nb_of_meal = 0;
	pthread_mutex_init(&a->fork, NULL);
	if (id == 1)
		vars->philo = a;
	else if (id == 2)
	{
		vars->philo->next = a;
		vars->philo->prev = a;
		a->next = vars->philo;
		a->prev = vars->philo;
	}
	else
	{
		a->next = vars->philo;
		a->prev = vars->philo->prev;
		vars->philo->prev->next = a;
		vars->philo->prev = a;
	}
}

void	free_everything(t_var *vars)
{
	int		i;
	t_philo	*tmp;
	t_philo	*tmp2;

	i = vars->number;
	tmp = vars->philo;
	while (i)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
		i--;
	}
	free(vars->philo_th);
}

void	delay_to_sync(t_var *vars, size_t id)
{
	int		i;
	size_t	delay;

	pthread_mutex_lock(&vars->stop);
	i = vars->number;
	delay = vars->tte / 2;
	pthread_mutex_unlock(&vars->stop);
	if (i % 2 == 0)
	{
		if (id % 2 == 0)
			usleep(delay);
	}
	else
	{
		if (id % 3 == 0)
			usleep(delay);
		if (id % 3 == 2)
			usleep(delay * 2);
	}
}
