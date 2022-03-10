/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 16:24:22 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/10 22:58:56 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((1000000 * tv.tv_sec + tv.tv_usec) / 1000);
}

void	*thread_routine(void *arg)
{
	int		time;
	t_var	*vars;

	vars = (t_var *)arg;
	sleep(2);
	time = get_current_time();
	while (pthread_mutex_lock(&vars->philo.l_fork) != 0)
		usleep(1);
	printf("time of the day : %lu\n", time - vars->start);
	pthread_exit(NULL);
}

int	main(int ac, char **av)
{
	int					time;
	pthread_t			thread[4];
	t_var				vars;
	(void)av;
	(void)ac;
	// if (ac > 5 || ac < 4)
	// 	return (1);
	time = get_current_time();
	int i = 0;
	vars.start = time; // --> init struct
	pthread_mutex_lock(&vars.philo.l_fork);
	i = 0;
	while (i < 4)
	{
		pthread_create(&thread[i], NULL, thread_routine, &vars);
		i++;
	}
	sleep(1);
	pthread_mutex_unlock(&vars.philo.l_fork);
	i = 0;
	while (i < 4)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	return (0);
}