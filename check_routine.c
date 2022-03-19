/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:47:44 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/19 18:33:57 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	microrests(t_var *vars, size_t id, size_t ttw)
{
	size_t	timediff;
	t_philo	*tmp;

	tmp = vars->philo;
	while (tmp->id != id)
		tmp = tmp->next;
	timediff = get_current_time() - tmp->last_meal;
	while (timediff < ttw)
	{
		if (timediff >= vars->ttd)
		{
			pthread_mutex_lock(&vars->death);
			vars->isded = id;
			pthread_mutex_unlock(&vars->death);
			break ;
		}
		usleep(500);
		timediff = get_current_time() - tmp->last_meal;
	}
}