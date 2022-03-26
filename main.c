/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:24:22 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/26 18:17:26 by vduriez          ###   ########.fr       */
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
	self->nb_of_meal++;
	if (self->nb_of_meal == vars->meals_max)
		vars->imfull++;
}

void	routine_loop(t_var *vars, t_philo *tmp)
{
	size_t	time;

	while (1)
	{
		grab_forks_eat(vars, tmp->id, tmp);
		if (ft_print_ded(vars, tmp->id) || check_meals(vars))
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

void	delay_to_sync(t_var *vars, size_t id)
{
	int	i;

	pthread_mutex_lock(&vars->stop);
	i = vars->number;
	pthread_mutex_unlock(&vars->stop);
	if (i % 2 == 0)
	{
		if (id % 2 == 0)
			usleep(200);
	}
	else
	{
		if (id % 3 == 0)
			usleep(200);
		if (id % 3 == 2)
			usleep(400);
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
	size_t	i[2];

	vars = (t_var *)arg;
	pthread_mutex_lock(&vars->stop);
	i[0] = vars->number;
	// pthread_mutex_lock(&vars->whoami);
	i[1] = vars->count + 1;
	vars->count++;
	tmp = vars->philo;
	while (tmp->id != i[1])
		tmp = tmp->next;
	pthread_mutex_unlock(&vars->stop);
	// pthread_mutex_unlock(&vars->whoami);
	pthread_mutex_lock(&vars->starting_blocks);
	pthread_mutex_unlock(&vars->starting_blocks);
	tmp->last_meal = get_current_time();
	if (i[0] == 1)
		only_wan(vars);
	if (i[0] == 1)
		return (NULL);
	delay_to_sync(vars, i[0]);
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

t_philo	*create_threads(t_var *vars)
{
	int		i[2];
	t_philo	*tmp;

	i[0] = -1;
	i[1] = vars->number;
	while (++i[0] < i[1])
		ft_addlast(vars, i[0] + 1);
	tmp = vars->philo;
	pthread_mutex_init(&vars->starting_blocks, NULL);
	pthread_mutex_lock(&vars->starting_blocks);
	i[0] = -1;
	while (++i[0] < i[1])
		pthread_create(&vars->philo_th[i[0]], NULL, philo_routine, vars);
	vars->start = get_current_time();
	pthread_mutex_unlock(&vars->starting_blocks);
	return (tmp);
}

void	print_ded_unock_lock(t_var *vars, t_philo *tmp)
{
	pthread_mutex_unlock(&vars->stop);
	ft_print_ded(vars, tmp->id);
	pthread_mutex_lock(&vars->stop);
	vars->isded = vars->number + 1;
	pthread_mutex_unlock(&vars->stop);
}

void	init_philos(t_var *vars)
{
	int		i[2];
	t_philo	*tmp;

	tmp = create_threads(vars);
	while (1)
	{
		usleep(200);
		pthread_mutex_lock(&vars->stop);
		i[0] = get_current_time() - tmp->last_meal;
		if (i[0] >= (int)vars->ttd && vars->isded <= 0)
			vars->isded = tmp->id;
		if (vars->isded > 0)
		{
			print_ded_unock_lock(vars, tmp);
			break ;
		}
		tmp = tmp->next;
		pthread_mutex_unlock(&vars->stop);
	}
	i[0] = -1;
	i[1] = vars->number;
	while (++i[0] < i[1])
		pthread_join(vars->philo_th[i[0]], NULL);
	free_everything(vars);
}

void	init_threads(char **av)
{
	t_var	vars;

	vars.number = ft_atoi(av[1]);
	vars.philo_th = malloc(sizeof(pthread_t *) * vars.number);
	if (!vars.philo_th)
	{
		free(vars.philo);
		return ;
	}
	vars.count = 0;
	// pthread_mutex_init(&vars.whoami, NULL);
	pthread_mutex_init(&vars.print, NULL);
	pthread_mutex_init(&vars.stop, NULL);
	vars.ttd = ft_atoi(av[2]);
	vars.tte = ft_atoi(av[3]);
	vars.tts = ft_atoi(av[4]);
	vars.meals_max = -1;
	vars.isded = -1;
	if (av[5])
		vars.meals_max = ft_atoi(av[5]);
	vars.imfull = 0;
	init_philos(&vars);
}

int	main(int ac, char **av)
{
	if (ac > 6 || ac < 5 || !valid_args(av))
	{
		write(2, "Error\nExpected : ./philo", 24);
		write(2, "  nb_of_philo  time_to_die ", 27);
		write(2, "time_to_eat  time_to_sleep  (min_nb_of_meals)\n", 46);
		return (1);
	}
	init_threads(av);
	return (0);
}
