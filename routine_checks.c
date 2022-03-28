/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:47:44 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/28 13:58:33 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	is_it_ded_yet(t_var *vars, size_t id)
{
	pthread_mutex_lock(&vars->stop);
	if (vars->isded < 0)
		vars->isded = id;
	pthread_mutex_unlock(&vars->stop);
}

void	microrests(t_var *vars, size_t id, size_t ttw)
{
	size_t	rest_start;
	size_t	timediff;
	size_t	timetodie;
	t_philo	*tmp;

	tmp = vars->philo;
	while (tmp->id != id)
		tmp = tmp->next;
	rest_start = get_current_time();
	timetodie = rest_start - tmp->last_meal;
	timediff = get_current_time() - rest_start;
	while (timediff < ttw)
	{
		if (check_meals(vars))
			return ;
		if (timetodie >= vars->ttd)
		{
			is_it_ded_yet(vars, id);
			return ;
		}
		usleep(200);
		timediff = get_current_time() - rest_start;
		timetodie = get_current_time() - tmp->last_meal;
	}
}

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((1000000 * tv.tv_sec + tv.tv_usec) / 1000);
}

int	check_ded(t_var *vars, t_philo *self, size_t id)
{
	size_t	timetodie;
	size_t	ttd;

	pthread_mutex_lock(&vars->stop);
	ttd = vars->ttd;
	timetodie = get_current_time() - self->last_meal;
	if (timetodie >= ttd)
	{
		if (vars->isded < 0)
			vars->isded = id;
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}

void	only_wan(t_var *vars)
{
	size_t			timetodie;

	pthread_mutex_lock(&vars->stop);
	timetodie = get_current_time() - vars->start;
	pthread_mutex_unlock(&vars->stop);
	pthread_mutex_lock(&vars->philo->fork);
	ft_print_fork_wan(vars, 1);
	while (timetodie < vars->ttd)
	{
		usleep(200);
		pthread_mutex_lock(&vars->stop);
		timetodie = get_current_time() - vars->start;
		pthread_mutex_unlock(&vars->stop);
	}
	pthread_mutex_lock(&vars->stop);
	vars->isded = 1;
	pthread_mutex_unlock(&vars->stop);
	pthread_mutex_unlock(&vars->philo->fork);
	ft_print_ded(vars, 1);
	free_everything(vars);
}
