/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:00:37 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/10 22:55:19 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		*philo_th;
	pthread_mutex_t	l_fork;
	pthread_mutex_t	r_fork;
	size_t			number;
	size_t			total_number;
	size_t			ttd;
	size_t			tte;
	size_t			tts;
	size_t			start_time;
	size_t			last_meal;
}				t_philo;

typedef struct s_var
{
	struct s_philo	philo;
	size_t			start;
}				t_var;

#endif