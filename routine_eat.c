/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_eat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:47:44 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/26 20:06:01 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *arg)
{
	t_var	*vars;
	t_philo	*tmp;
	size_t	i[2];

	vars = (t_var *)arg;
	pthread_mutex_lock(&vars->stop);
	i[0] = vars->number;
	i[1] = vars->count + 1;
	vars->count++;
	tmp = vars->philo;
	while (tmp->id != i[1])
		tmp = tmp->next;
	pthread_mutex_unlock(&vars->stop);
	pthread_mutex_lock(&vars->starting_blocks);
	pthread_mutex_unlock(&vars->starting_blocks);
	pthread_mutex_lock(&vars->stop);
	tmp->last_meal = get_current_time();
	pthread_mutex_unlock(&vars->stop);
	if (i[0] == 1)
		only_wan(vars);
	if (i[0] == 1)
		return (NULL);
	delay_to_sync(vars, i[0]);
	routine_loop(vars, tmp);
	return (NULL);
}

int	ft_print_ded(t_var *vars, size_t id)
{
	size_t	time;

	pthread_mutex_lock(&vars->stop);
	if (vars->isded > 0)
	{
		if (vars->isded == (int)id)
		{
			time = get_current_time();
			pthread_mutex_lock(&vars->print);
			printf("%.4lu %d died\n", time - vars->start, vars->isded);
			pthread_mutex_unlock(&vars->print);
		}
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	pthread_mutex_unlock(&vars->stop);
	return (0);
}

void	routine_loop(t_var *vars, t_philo *tmp)
{
	size_t	time;

	while (1)
	{
		grab_forks_eat(vars, tmp->id, tmp);
		if (check_meals(vars) || ft_print_ded(vars, tmp->id))
			break ;
		time = get_current_time() - tmp->last_meal;
		if (time >= vars->ttd)
		{
			pthread_mutex_lock(&vars->stop);
			if (vars->isded <= 0)
				vars->isded = tmp->id;
			pthread_mutex_unlock(&vars->stop);
			ft_print_ded(vars, tmp->id);
			break ;
		}
		if (!ft_print_sleep(vars, tmp->id) || ft_print_ded(vars, tmp->id))
			break ;
		microrests(vars, tmp->id, vars->tts);
		if (!ft_print_thinking(vars, tmp->id) || ft_print_ded(vars, tmp->id))
			break ;
	}
}

void	grab_forks_eat(t_var *vars, size_t id, t_philo *self)
{
	pthread_mutex_t	*fork[2];

	decide_first_fork(self, fork);
	if (check_ded(vars, self, id) || !ft_print_fork(vars, id)
		|| check_meals(vars))
	{
		pthread_mutex_unlock(fork[1]);
		return ;
	}
	pthread_mutex_lock(fork[0]);
	if (!ft_print_fork(vars, id) || check_meals(vars))
	{
		unlock_lr(fork[1], fork[0]);
		return ;
	}
	if (!ft_print_eating(vars, id) || check_meals(vars))
		return ;
	pthread_mutex_lock(&vars->stop);
	self->last_meal = get_current_time();
	pthread_mutex_unlock(&vars->stop);
	microrests(vars, id, vars->tte);
	unlock_lr(fork[1], fork[0]);
	pthread_mutex_lock(&vars->stop);
	self->nb_of_meal++;
	if (self->nb_of_meal == vars->meals_max)
		vars->imfull++;
	pthread_mutex_unlock(&vars->stop);
}
