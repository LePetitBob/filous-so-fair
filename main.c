/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:24:22 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/21 23:14:02 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((1000000 * tv.tv_sec + tv.tv_usec) / 1000);
}

void	grab_forks_eat(t_var *vars, size_t id, t_philo *self)
{
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	size_t	time;
	size_t	timetodie;

	right = self->id & 1 ? &self->fork : &self->prev->fork;
	left = self->id & 1 ? &self->prev->fork : &self->fork;
	timetodie = get_current_time() - self->last_meal;
	if (timetodie >= vars->ttd)
	{
		pthread_mutex_lock(&vars->stop);
		if (vars->isded < 0)
			vars->isded = id;
		pthread_mutex_unlock(&vars->stop);
		return ;
	}
	pthread_mutex_lock(left);
	if (!ft_print_fork(vars, id) || (vars->meals_max != 0 && vars->imfull >= vars->number))
	{
		pthread_mutex_unlock(left);
		return ;
	}
	pthread_mutex_lock(right);
	if (!ft_print_fork(vars, id) || (vars->meals_max != 0 && vars->imfull >= vars->number))
	{
		pthread_mutex_unlock(left);
		pthread_mutex_unlock(right);
		return ;
	}
	time = get_current_time();
	if (!ft_print_eating(vars, id) || (vars->meals_max != 0 && vars->imfull >= vars->number))
		return ;
	self->last_meal = get_current_time();
	microrests(vars, id, vars->tte);
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
	self->nb_of_meal++;
	// pthread_mutex_lock(&vars->yummytummyfullybelly);
	if (self->nb_of_meal == vars->meals_max)
	if (self->nb_of_meal == vars->meals_max)
		vars->imfull++;
	// pthread_mutex_unlock(&vars->yummytummyfullybelly);
}

void	*philo_routine(void *arg)
{
	int		time;
	size_t	id;
	t_var	*vars;
	t_philo	*tmp;

	vars = (t_var *)arg;
	pthread_mutex_lock(&vars->whoami);
	id = vars->count + 1;
	vars->count++;
	pthread_mutex_unlock(&vars->whoami);
	tmp = vars->philo;
	while (tmp->id != id)
		tmp = tmp->next;
	tmp->last_meal = get_current_time();
	pthread_mutex_lock(&vars->starting_blocks);
	pthread_mutex_unlock(&vars->starting_blocks);
	if (vars->number % 2 == 0)
	{
		if (id % 2 == 0)
			microrests(vars, id, vars->tte);
	}
	else
	{
		if (id % 3 == 0)
			microrests(vars, id, vars->tte);
		if (id % 3 == 2)
			microrests(vars, id, vars->tte * 2);
	}
	while (1)
	{
		pthread_mutex_lock(&vars->stop);
		if ((vars->meals_max != 0 && vars->imfull >= vars->number) || vars->isded > 0)
		{
			pthread_mutex_unlock(&vars->stop);
			break ;
		}
		pthread_mutex_unlock(&vars->stop);
		grab_forks_eat(vars, id, tmp);
		if (ft_print_ded(vars, id) || (vars->meals_max != 0 && vars->imfull >= vars->number))
			return (NULL);
		time = get_current_time() - tmp->last_meal;
		if (time >= vars->ttd)
		{
			pthread_mutex_lock(&vars->stop);
			if (vars->isded <= 0)
				vars->isded = tmp->id;
			pthread_mutex_unlock(&vars->stop);
			ft_print_ded(vars, id);
			return (NULL);
		}
		if (!ft_print_sleep(vars, id))
		{
			ft_print_ded(vars, id);
			return (NULL);
		}
		microrests(vars, id, vars->tts);
		if (ft_print_ded(vars, id))
			return (NULL);
	}
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
	usleep(5000);
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
