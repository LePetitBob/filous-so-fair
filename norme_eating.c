/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norme_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 19:18:12 by vduriez           #+#    #+#             */
/*   Updated: 2022/04/04 22:38:23 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	solo_player(t_var *vars)
{
	if (vars->number == 1)
	{
		pthread_join(vars->philo_th[0], NULL);
		return (1);
	}
	return (0);
}

void	print_ded_unock_lock(t_var *vars, t_philo *tmp)
{
	pthread_mutex_unlock(&vars->stop);
	ft_print_ded(vars, tmp->id);
	pthread_mutex_lock(&vars->stop);
	vars->isded = vars->number + 1;
	pthread_mutex_unlock(&vars->stop);
}

void	unlock_lr(pthread_mutex_t *left, pthread_mutex_t *right
	, t_var *vars, t_philo *self)
{
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
	pthread_mutex_lock(&vars->stop);
	self->nb_of_meal++;
	if (self->nb_of_meal == vars->meals_max)
		vars->imfull++;
	pthread_mutex_unlock(&vars->stop);
}

void	decide_first_fork(t_philo *self, pthread_mutex_t *fork[2], t_var *vars)
{
	int		meals;
	size_t	nb;

	pthread_mutex_lock(&vars->stop);
	meals = self->nb_of_meal;
	nb = vars->number;
	pthread_mutex_unlock(&vars->stop);
	fork[0] = &self->prev->fork;
	fork[1] = &self->fork;
	if (self->id == nb)
	{
		fork[0] = &self->fork;
		fork[1] = &self->prev->fork;
	}
	if (!meals && self->id % 2 == 1 && self->id != nb)
		usleep(200);
	pthread_mutex_lock(fork[1]);
	pthread_mutex_lock(fork[0]);
}

int	check_meals(t_var *vars)
{
	pthread_mutex_lock(&vars->stop);
	if (vars->meals_max > 0 && vars->imfull >= vars->number)
	{
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}
