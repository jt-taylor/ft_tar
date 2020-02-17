/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar_unarchive_misc1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:44:04 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 19:18:16 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

int				check_if_block_not_empty(t_untar *s)
{
	int		i;

	i = 511;
	while (i >= 0)
	{
		if (s->buffer[i] != 0)
			return (1);
		i--;
	}
	return (0);
}

int				cheap_atoo(const char *str, int num_len)
{
	int		ret;

	ret = 0;
	while (*str < '0' && *str > '7' && num_len > 0 && num_len--)
		str++;
	while (*str >= '0' && *str <= '7' && num_len > 0 && num_len--)
		ret = 8 * ret + *str++ - '0';
	return (ret);
}

int				get_checksum(t_untar *s)
{
	int		i;
	int		sum;

	i = 0;
	sum = 0;
	while (i < 148)
	{
		sum += (unsigned char)s->buffer[i];
		i++;
	}
	while (i <= 155)
	{
		sum += 32;
		i++;
	}
	while (i < 512)
	{
		sum += (unsigned char)s->buffer[i];
		i++;
	}
	return (sum == cheap_atoo(&s->buffer[148], 8));
}
