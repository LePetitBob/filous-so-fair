/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:47:44 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/22 05:41:18 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		if (timetodie >= vars->ttd)
		{
			pthread_mutex_lock(&vars->stop);
			if (vars->isded < 0)
				vars->isded = id;
			pthread_mutex_unlock(&vars->stop);
			break ;
		}
		usleep(200);
		timediff = get_current_time() - rest_start;
		timetodie = get_current_time() - tmp->last_meal;
	}
}

int	ft_print_fork(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->stop);
	if (vars->isded <= 0)
	{
		pthread_mutex_lock(&vars->print);
		time = get_current_time();
		printf("%.4lu %lu has taken a fork\n", time - vars->start, id);
		pthread_mutex_unlock(&vars->print);
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}

int	ft_print_thinking(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->stop);
	if (vars->isded <= 0)
	{
		pthread_mutex_lock(&vars->print);
		time = get_current_time();
		printf("%.4lu %lu is thinking\n", time - vars->start, id);
		pthread_mutex_unlock(&vars->print);
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}

int	ft_print_eating(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->stop);
	if (vars->isded <= 0)
	{
		pthread_mutex_lock(&vars->print);
		time = get_current_time();
		printf("%.4lu %lu is eating\n", time - vars->start, id);
		pthread_mutex_unlock(&vars->print);
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}

int	ft_print_sleep(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->stop);
	if (vars->isded <= 0)
	{
		pthread_mutex_lock(&vars->print);
		time = get_current_time();
		printf("%.4lu %lu is sleeping\n", time - vars->start, id);
		pthread_mutex_unlock(&vars->print);
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}

int	ft_print_ded(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->stop);
	if (vars->isded > 0)
	{
		pthread_mutex_lock(&vars->print);
		if (vars->isded == id)
		{
			time = get_current_time();
			printf("%.4lu %d died\n", time - vars->start, vars->isded);
		}
		pthread_mutex_unlock(&vars->print);
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}
