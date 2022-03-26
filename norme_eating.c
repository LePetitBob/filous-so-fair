/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norme_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 19:18:12 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/26 19:20:33 by vduriez          ###   ########.fr       */
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

void	unlock_lr(pthread_mutex_t *left, pthread_mutex_t *right)
{
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
}

void	decide_first_fork(t_philo *self, pthread_mutex_t *fork[2])
{
	fork[0] = &self->prev->fork;
	fork[1] = &self->fork;
	if (self->id % 2 == 1)
	{
		fork[0] = &self->fork;
		fork[1] = &self->prev->fork;
	}
	pthread_mutex_lock(fork[1]);
}

int	check_meals(t_var *vars)
{
	int	i;

	pthread_mutex_lock(&vars->stop);
	i = vars->number;
	if (vars->meals_max != 0 && vars->imfull >= i)
	{
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}
