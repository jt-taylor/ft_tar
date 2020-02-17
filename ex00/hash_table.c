/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkubli <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 01:49:43 by zkubli            #+#    #+#             */
/*   Updated: 2020/02/16 21:14:12 by zkubli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "archive.h"
#include <stdlib.h>
#include <string.h>

/*
** Frees one row of table, doesn't set anything to zero
*/

static void	clear_chain(t_hashnode *table)
{
	if (table)
	{
		clear_chain(table->next);
		free(table);
	}
}

void		table_free(t_hashnode **table)
{
	int i;

	i = 0;
	while (i < HASH_SIZE)
	{
		clear_chain(table[i]);
		i++;
	}
}

char		*table_checkadd(t_hashnode **table, ino_t my_ino, char *s)
{
	t_hashnode **tmp;

	if (!(table))
		return (NULL);
	tmp = table + (my_ino % HASH_SIZE);
	while (*tmp)
	{
		if ((*tmp)->my_ino == my_ino)
			return ((*tmp)->my_name);
		tmp = &(*tmp)->next;
	}
	(*tmp) = malloc(sizeof(t_hashnode));
	(*tmp)->my_ino = my_ino;
	strncpy((*tmp)->my_name, s, 100);
	(*tmp)->next = NULL;
	return (NULL);
}
