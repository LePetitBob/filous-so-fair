/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:24:22 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/26 19:25:30 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	init_philos(t_var *vars)
{
	int		i[2];
	t_philo	*tmp;

	tmp = create_threads(vars);
	if (solo_player(vars))
		return ;
	while (1)
	{
		usleep(200);
		pthread_mutex_lock(&vars->stop);
		i[0] = get_current_time() - tmp->last_meal;
		if (i[0] >= (int)vars->ttd && vars->isded <= 0)
			vars->isded = tmp->id;
		if (vars->isded > 0)
			print_ded_unock_lock(vars, tmp);
		if (vars->isded > 0)
			break ;
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
