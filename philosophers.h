/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:00:37 by vduriez           #+#    #+#             */
/*   Updated: 2022/04/04 20:27:11 by vduriez          ###   ########.fr       */
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
	pthread_mutex_t	eating;
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
	pthread_mutex_t		print;
	pthread_mutex_t		stop;
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

unsigned long long	ft_atoull(char *nb);
long long			ft_atoi(const char *str);
t_philo				*ft_create_elem(int id);
int					is_num(char *s);
int					is_long_long(char *s);
int					is_bigger(char *nb, int neg);
int					get_current_time(void);
int					ft_strlen(char *s);
int					ft_print_eating(t_var *vars, size_t id);
int					ft_print_fork(t_var *vars, size_t id);
int					ft_print_sleep(t_var *vars, size_t id);
int					ft_print_ded(t_var *vars, size_t id);
int					ft_print_thinking(t_var *vars, size_t id);
int					solo_player(t_var *vars);
int					check_meals(t_var *vars);
int					valid_args(char **av);
int					check_ded(t_var *vars, t_philo *self, size_t id);
char				*ft_strdup(const char *s1);
void				microrests(t_var *vars, size_t id, size_t ttw);
void				*philo_routine(void *arg);
void				ft_print_fork_wan(t_var *vars, size_t id);
void				free_everything(t_var *vars);
void				print_ded_unock_lock(t_var *vars, t_philo *tmp);
void				unlock_lr(pthread_mutex_t *left, pthread_mutex_t *right,
						t_var *vars, t_philo *self);
void				decide_first_fork(t_philo *self, pthread_mutex_t *fork[2],
						t_var *vars);
void				free_everything(t_var *vars);
void				ft_addlast(t_var *vars, int id);
void				only_wan(t_var *vars);
void				grab_forks_eat(t_var *vars, size_t id, t_philo *self);
void				routine_loop(t_var *vars, t_philo *tmp);
void				*philo_routine(void *arg);
void				is_it_ded_yet(t_var *vars, size_t id);

#endif
