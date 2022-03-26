/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:47:44 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/26 19:26:51 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_print_fork_wan(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->print);
	time = get_current_time();
	printf("%.4lu %lu has taken a fork\n", time - vars->start, id);
	pthread_mutex_unlock(&vars->print);
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
