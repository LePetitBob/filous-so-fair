/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:19:04 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/16 17:31:46 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	count_digit(const char *str, int index)
{
	int		i;

	i = 0;
	index += 1;
	while (str[index] != '\0' && (str[index] >= '0' && str[index] <= '9'))
	{
		i++;
		index++;
	}
	return (i);
}

long long int	convert_res(const char *str, long long int nbr, int index)
{
	long long int	max;

	max = 9223372036854775807;
	if (str[index] == '-' && (nbr > max || count_digit(str, index) > 19))
		return (nbr);
	else if (nbr > max || count_digit(str, index) > 19)
		return (nbr);
	if (str[index] == '-')
		nbr *= -1;
	if (nbr == 1 && count_digit(str, index) >= 3)
		return (0);
	return (nbr);
}

long long	ft_atoi(const char *str)
{
	int					i;
	long long int		res;

	i = 0;
	res = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (convert_res(str, res, 0));
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (convert_res(str, res, 0));
}
