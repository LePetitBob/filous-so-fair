/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:24:22 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/19 18:39:17 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((1000000 * tv.tv_sec + tv.tv_usec) / 1000);
}

void	grab_forks_eat(t_var *vars, size_t id)
{
	t_philo	*self;
	// t_philo	*prev;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	size_t	time;

	self = vars->philo;
	while (self->id != id)
		self = self->next;
	right = self->id & 1 ? &self->fork : &self->prev->fork;
	left = self->id & 1 ? &self->prev->fork : &self->fork;
	pthread_mutex_lock(left);
	time = get_current_time();
	pthread_mutex_lock(&vars->print);
	printf("%.4lu %lu has taken a fork\n", time - vars->start, id);
	pthread_mutex_unlock(&vars->print);
	pthread_mutex_lock(right);
	time = get_current_time();
	pthread_mutex_lock(&vars->print);
	printf("%.4lu %lu has taken a fork\n", time - vars->start, id);
	pthread_mutex_unlock(&vars->print);
	time = get_current_time();
	pthread_mutex_lock(&vars->print);
	printf("%.4lu %lu is eating\n", time - vars->start, id);
	pthread_mutex_unlock(&vars->print);
	//TODO
	// pthread_mutex_lock(&vars->death);
	// if (vars-> isded)
	// 	cleanexit
	// pthread_mutex_unlock(&vars->death);
	//TODO
	self->last_meal = (size_t)get_current_time;
	microrests(vars, id, vars->tte); //TODO	microsleep + checks
	// if (ft_urests(vars, self) == -1)
	// {
	// 	clean_exit;
	// }
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
}

int	check_routine(t_var *vars, size_t id)
{
	(void)vars;
	(void)id;

	return (0);
}

void	*philo_routine(void *arg)
{
	int		time;
	size_t	id;
	t_var	*vars;

	vars = (t_var *)arg;
	pthread_mutex_lock(&vars->whoami);
	id = vars->count + 1;
	vars->count++;
	printf("self = %lu\n", id);
	pthread_mutex_unlock(&vars->whoami);
	pthread_mutex_lock(&vars->starting_blocks);
	pthread_mutex_unlock(&vars->starting_blocks);
	if (vars->number % 2 == 0)
	{
		if (id % 2 == 0)
			usleep(1000 * vars->tte);
	}
	else
	{
		if (id % 3 == 0)
			usleep(1000 * vars->tte);
		if (id % 3 == 2)
			usleep(1000 * vars->tte * 2);
	}
	// int i = 0;
	while (1)//TODO	-->not all philos have had their min nb of meals
	{
		grab_forks_eat(vars, id);
		time = get_current_time();
		printf("%.4lu %lu is sleeping\n", time - vars->start, id);
		microrests(vars, id, vars->tts);
		pthread_mutex_lock(&vars->death);
		if (vars->isded != 0)
		{
			time = get_current_time();
			pthread_mutex_lock(&vars->print);
			printf("%.4lu %d died\n", time - vars->start, vars->isded);
			pthread_mutex_unlock(&vars->print);
			break ;
		}
		pthread_mutex_unlock(&vars->death);
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
	usleep(50000);
	vars->start = get_current_time();
	pthread_mutex_unlock(&vars->starting_blocks);
	while (1)
		check_routine(vars, 0);
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
	if (av[5])
		vars->meals_max = ft_atoi(av[5]);
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
