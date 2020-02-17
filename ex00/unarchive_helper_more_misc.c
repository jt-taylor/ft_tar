/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unarchive_helper_more_misc.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 14:49:55 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 14:51:31 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

/*
** ignore files of the types that are not dir's or 'plain' files
*/

void			handle_file_type(t_untar *s)
{
	char		opt;

	opt = s->buffer[156];
	if (opt == 1)
		printf("Ignore Hardlink\n");
	else if (opt == 2)
		printf("Ignore symlink\n");
	else if (opt == 3)
		printf("Ignore char device\n");
	else if (opt == 4)
		printf("Ignore block device\n");
	else if (opt == 5)
	{
		cheap_mkdir(&s->buffer[256], cheap_atoo(&s->buffer[256], 8));
		s->file_size = 0;
	}
	else if (opt == 6)
		printf("Ignore FIFO device\n");
	else
	{
		s->f = cheap_make_file(&s->buffer[0], cheap_atoo(&s->buffer[256], 8));
	}
}

/*
** read the file in blocks of 512 from the tar file
** and write them to the open file
*/

void			read_file_entry(t_untar *s)
{
	if (s->bytes_read < 512)
	{
		fprintf(stderr, "Block misalignment:: less than 512 bytes read\n");
		return ;
	}
	if (s->file_size < 512)
		s->bytes_read = 512;
	if (s->f != 0)
	{
		if (fwrite(s->buffer, 1, s->bytes_read, s->f) != s->bytes_read)
		{
			fprintf(stderr, "Failed fwrite\n");
			fclose(s->f);
			s->f = 0;
		}
		s->file_size -= 512;
	}
}

/*
** tar denotes the end each entry with an empty block
** or a block consiting only of null;
** return (1) if not empty
** return (0) if it is empty
*/

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
