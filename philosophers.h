/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:00:37 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/21 16:51:30 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_mutex_t	fork;
	size_t			id;
	size_t			last_meal;
	size_t			nb_of_meal;
	struct s_philo	*next;
	struct s_philo	*prev;
}				t_philo;

typedef struct s_var
{
	t_philo				*philo;
	pthread_t			*philo_th;
	pthread_mutex_t		starting_blocks;
	pthread_mutex_t		whoami;
	pthread_mutex_t		print;
	pthread_mutex_t		stop;
	pthread_mutex_t		yummytummyfullybelly;
	int					imfull;
	int					isded;
	int					number;
	size_t				count;
	size_t				start;
	size_t				ttd;
	size_t				tte;
	size_t				tts;
	size_t				meals_max;
}				t_var;

int					is_num(char *s);
int					is_long_long(char *s);
int					is_bigger(char *nb, int neg);
unsigned long long	ft_atoull(char *nb);
void				microrests(t_var *vars, size_t id, size_t ttw);
void				*philo_routine(void *arg);
int					get_current_utime(void);
int					get_current_time(void);
int					ft_strlen(char *s);
char				*ft_strdup(const char *s1);
long long			ft_atoi(const char *str);
int					ft_print_eating(t_var *vars, size_t id);
int					ft_print_fork(t_var *vars, size_t id);
int					ft_print_sleep(t_var *vars, size_t id);
int					ft_print_ded(t_var *vars, size_t id);

#endif