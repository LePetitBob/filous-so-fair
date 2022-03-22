/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:24:22 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/22 20:30:20 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((1000000 * tv.tv_sec + tv.tv_usec) / 1000);
}

int	check_ded(t_var *vars, t_philo *self, size_t id)
{
	size_t			timetodie;

	timetodie = get_current_time() - self->last_meal;
	if (timetodie >= vars->ttd)
	{
		pthread_mutex_lock(&vars->stop);
		if (vars->isded < 0)
			vars->isded = id;
		pthread_mutex_unlock(&vars->stop);
		return (1);
	}
	return (0);
}

void	unlock_lr(pthread_mutex_t *left, pthread_mutex_t *right, t_philo *self)
{
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
	self->fork_usage = FREE;
	self->prev->fork_usage = FREE;
}

void	decide_first_fork(t_philo *self, pthread_mutex_t *fork[2])
{
	fork[0] = &self->prev->fork;
	if (self->id % 2 == 1)
		fork[0] = &self->fork;
	fork[1] = &self->fork;
	if (self->id % 2 == 1)
		fork[1] = &self->prev->fork;
	if (self->id & 1)
		self->fork_usage = USED;
	else
		self->prev->fork_usage = USED;
	pthread_mutex_lock(fork[1]);
}

int	check_meals(t_var *vars)
{
	if (vars->meals_max != 0 && vars->imfull >= vars->number)
		return (1);
	return (0);
}

void	grab_forks_eat(t_var *vars, size_t id, t_philo *self)
{
	pthread_mutex_t	*fork[2];

	decide_first_fork(self, fork);
	if (check_ded(vars, self, id))
		return ;
	if (!ft_print_fork(vars, id) || check_meals(vars))
	{
		pthread_mutex_unlock(fork[1]);
		if (self->id & 1)
			self->fork_usage = FREE;
		else
			self->prev->fork_usage = FREE;
		return ;
	}
	pthread_mutex_lock(fork[0]);
	if (self->id & 1)
		self->prev->fork_usage = USED;
	else
		self->fork_usage = USED;
	if (!ft_print_fork(vars, id) || check_meals(vars))
	{
		unlock_lr(fork[1], fork[0], self);
		return ;
	}
	if (!ft_print_eating(vars, id) || check_meals(vars))
		return ;
	self->last_meal = get_current_time();
	microrests(vars, id, vars->tte);
	unlock_lr(fork[1], fork[0], self);
	self->nb_of_meal++;
	if (self->nb_of_meal == vars->meals_max)
		vars->imfull++;
}

int	thinking_in_progress(t_var *vars, t_philo *tmp)
{
	while ((tmp->fork_usage == USED
			&& tmp->prev->fork_usage == USED) || vars->isded <= 0)
	{
		if (check_ded(vars, tmp, tmp->id))
			break ;
		usleep(200);
	}
}

void	routine_loop(t_var *vars, t_philo *tmp)
{
	size_t	time;

	while (1)
	{
		grab_forks_eat(vars, tmp->id, tmp);
		if (ft_print_ded(vars, tmp->id) || check_meals(vars))
			return ;
		time = get_current_time() - tmp->last_meal;
		if (time >= vars->ttd)
		{
			pthread_mutex_lock(&vars->stop);
			if (vars->isded <= 0)
				vars->isded = tmp->id;
			pthread_mutex_unlock(&vars->stop);
			ft_print_ded(vars, tmp->id);
			return ;
		}
		if (!ft_print_sleep(vars, tmp->id) || ft_print_ded(vars, tmp->id))
			return ;
		microrests(vars, tmp->id, vars->tts);
		if (!ft_print_thinking(vars, tmp->id) || ft_print_ded(vars, tmp->id))
			return ;
		thinking_in_progress(vars, tmp);
	}
}

void	delay_to_sync(t_var *vars, size_t id)
{
	if (vars->number % 2 == 0)
	{
		if (id % 2 == 0)
			usleep(20);
	}
	else
	{
		if (id % 3 == 0)
			usleep(20);
		if (id % 3 == 2)
			usleep(40);
	}
}

void	only_wan(t_var *vars)
{
	size_t			timetodie;

	timetodie = get_current_time() - vars->start;
	pthread_mutex_lock(&vars->philo->fork);
	ft_print_fork(vars, 1);
	while (timetodie < vars->ttd)
	{
		pthread_mutex_lock(&vars->stop);
		vars->isded = 1;
		pthread_mutex_unlock(&vars->stop);
		usleep(200);
		timetodie = get_current_time() - vars->start;
	}
	pthread_mutex_unlock(&vars->philo->fork);
	ft_print_ded(vars, 1);
}

void	*philo_routine(void *arg)
{
	t_var	*vars;
	t_philo	*tmp;
	size_t	id;
	int		time;

	vars = (t_var *)arg;
	pthread_mutex_lock(&vars->whoami);
	id = vars->count + 1;
	vars->count++;
	tmp = vars->philo;
	pthread_mutex_unlock(&vars->whoami);
	while (tmp->id != id)
		tmp = tmp->next;
	tmp->last_meal = get_current_time();
	pthread_mutex_lock(&vars->starting_blocks);
	pthread_mutex_unlock(&vars->starting_blocks);
	if (vars->number == 1)
	{
		only_wan(vars);
		return (NULL);
	}
	delay_to_sync(vars, id);
	routine_loop(vars, tmp);
	return (NULL);
}

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
	new->last_meal = 0;
	new->fork_usage = FREE;
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

void	init_philos(t_var *vars)
{
	int		i;

	i = 0;
	while (i < vars->number)
	{
		ft_addlast(vars, i + 1);
		i++;
	}
	pthread_mutex_init(&vars->starting_blocks, NULL);
	pthread_mutex_lock(&vars->starting_blocks);
	i = -1;
	while (++i < vars->number)
		pthread_create(&vars->philo_th[i], NULL, philo_routine, vars);
	vars->start = get_current_time();
	pthread_mutex_unlock(&vars->starting_blocks);
	i = -1;
	while (++i < vars->number)
		pthread_join(vars->philo_th[i], NULL);
}

void	init_threads(char **av, t_var *vars)
{
	vars = malloc(sizeof(t_var *));
	vars->number = ft_atoi(av[1]);
	vars->philo = malloc(sizeof(t_philo *) * vars->number);
	vars->philo_th = malloc(sizeof(pthread_t *) * vars->number);
	vars->count = 0;
	pthread_mutex_init(&vars->whoami, NULL);
	pthread_mutex_init(&vars->print, NULL);
	vars->ttd = ft_atoi(av[2]);
	vars->tte = ft_atoi(av[3]);
	vars->tts = ft_atoi(av[4]);
	vars->meals_max = -1;
	if (av[5])
		vars->meals_max = ft_atoi(av[5]);
	vars->imfull = 0;
	init_philos(vars);
}

int	main(int ac, char **av)
{
	t_var	vars;

	if (ac > 6 || ac < 5 || !valid_args(av))
	{
		write(2, "Error\nExpected : ./philo", 24);
		write(2, "  nb_of_philo  time_to_die ", 27);
		write(2, "time_to_eat  time_to_sleep  (min_nb_of_meals)\n", 46);
		return (1);
	}
	init_threads(av, &vars);
	return (0);
}
