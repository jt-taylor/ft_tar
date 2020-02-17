/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar_unarchive_handle_extract.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 21:00:12 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 21:13:14 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

/*
** Norm line limits are fun and make my code much better
** yes yes
*/

static inline int	handle_unarchive_extract_inner(t_ft_tar *tar, t_untar *s)
{
	(void)tar;
	if (s->bytes_read < 512)
	{
		fprintf(stderr, "Block misalignment:: less than 512 bytes read\n");
		return (-1);
	}
	if (!check_if_block_not_empty(s))
		return (-1);
	if (get_checksum(s) == 0)
		return (-1);
	return (1);
}

void				handle_unarchive_extract(t_ft_tar *tar, t_untar *s)
{
	printf("Extracting from archive::%s\n", tar->opt_f_target);
	while (1)
	{
		s->bytes_read = fread(s->buffer, 1, 512, s->ar);
		s->to_write = (check_file_exists_in_arglist(tar, s) ||
				!*tar->argv) ? 1 : 0;
		if (handle_unarchive_extract_inner(tar, s) == -1)
			return ;
		s->file_size = cheap_atoo(&s->buffer[124], 12);
		handle_file_type(s);
		if (tar->opt_v)
			fprintf(stdout, "x %s\n", s->buffer);
		while (s->file_size > 0)
		{
			s->bytes_read = fread(s->buffer, 1, 512, s->ar);
			read_file_entry(s);
		}
		if (s->f)
			fclose(s->f);
		s->f = 0;
	}
}
