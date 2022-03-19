/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vduriez <vduriez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:55:05 by vduriez           #+#    #+#             */
/*   Updated: 2022/03/16 20:56:22 by vduriez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long long	ft_atoull(char *nb)
{
	unsigned long long	nbr;
	int					i;

	i = -1;
	nbr = 0;
	while (nb[++i] != '\0')
		nbr = 10 * nbr + (nb[i] - 48);
	return (nbr);
}

int	is_bigger(char *nb, int neg)
{
	unsigned long long	nbr;
	unsigned long long	max;

	max = 9223372036854775807;
	if (neg == 1)
		max = max + 1;
	nbr = ft_atoull(nb);
	free(nb);
	if (nbr > max)
		return (1);
	return (0);
}

int	is_long_long(char *s)
{
	int		neg;
	int		i;
	int		len;
	char	*nb;

	neg = 0;
	i = 0;
	if (s[i] == '-')
		neg = 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (s[i] == '+' || s[i] == '-')
		return (0);
	while (s[i] == '0')
		i++;
	len = ft_strlen(s + i);
	if (len > 19)
		return (0);
	nb = ft_strdup(s + i);
	return (!is_bigger(nb, neg));
}

int	is_num(char *s)
{
	int	i;

	i = 0;
	if (!is_long_long(s) || s[0] == '-')
		return (0);
	if (s[0] == '+')
		i++;
	while (s[i])
	{
		if (s[i] < 48 || s[i] > 57)
			return (0);
		i++;
	}
	return (1);
}
