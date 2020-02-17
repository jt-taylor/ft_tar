/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar_unarchive_handle.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:39:46 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 20:35:55 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

void			untar_handle(t_ft_tar *tar)
{
	struct s_untar		s;

	if (tar->opt_f_target)
		s.ar = fopen(tar->opt_f_target, "rb");
	else
		s.ar = stdin;
	if (s.ar == 0)
	{
		fprintf(stderr, "Cannot open %s\n", tar->opt_f_target);
		exit(1);
	}
	untar_handle_mode(tar, &s);
	if (s.ar != stdin)
		fclose(s.ar);
}

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
		s->f = cheap_make_file(&s->buffer[0], cheap_atoo(&s->buffer[103], 3));
	}
}

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
		if (s->to_write && fwrite(s->buffer, 1, s->bytes_read, s->f)
				!= s->bytes_read)
		{
			fprintf(stderr, "Failed fwrite\n");
			if (s->f != stdout)
			{
				fclose(s->f);
				s->f = 0;
			}
		}
		s->file_size -= 512;
	}
}

int				check_file_exists_in_arglist(t_ft_tar *tar, t_untar *s)
{
	char	*name;
	char	**tmp;

	tmp = tar->argv;
	if (s->buffer[156] == '5')
		name = s->buffer;
	else
	{
		name = strrchr(s->buffer, '/');
		name = name ? name : s->buffer;
		*name == '/' ? name++ : 0;
	}
	while (*tmp)
	{
		if (strcmp(name, *tmp) == 0)
			return (1);
		tmp++;
	}
	return (0);
}
