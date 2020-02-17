/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar_unarchive_modes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:26:00 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 20:18:43 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

/*
** TODO :: make the options that are mutually exclusive actually mutually
** exclusive instead of just having a priority in how we check them
*/

void		untar_handle_mode(t_ft_tar *tar, t_untar *s)
{
	s->f = stdout;
	if (tar->opt_t)
		handle_unarchive_t_flag(tar, s);
	else
		handle_unarchive_extract(tar, s);
	// ----------------------------------------------------------------------------
	if (s->f && s->f != stdout)
		fclose(s->f);
	if (s->ar && s->ar != stdin)
		fclose(s->ar);
}

/*
** handle the -t option
** which prints the archive to stdout
** use with the -f flag to read from a specific tarfile
** this just prints the filenames that are inside of the tar porogram
** so basically go throught the archive and list all of the file names
*/

void			read_file_entry_t_flag(t_untar *s)
{
	if (s->bytes_read < 512)
	{
		fprintf(stderr, "Block misalignment:: less than 512 bytes read\n");
		return ;
	}
	if (s->file_size < 512)
		s->bytes_read = 512;
	if (s->f != 0)
		s->file_size -= 512;
}

/*
** TODO ::
** handle the other types if files ie more than just dirs and files
*/

static char		*cheap_get_mode(t_untar *s)
{
	static char		mode[11] = {0};
	int				i = -1;
	while (++i < 11)
		mode[i] = 0;
	mode[0] = (s->buffer[156] == '0') ? '-' : '-';
	mode[0] = (s->buffer[156] == '5') ? 'd' : mode[0];
	mode[1] = (s->buffer[103] <= '7' && s->buffer[103] >= '4') ? 'r' : '-';
	mode[2] = (s->buffer[103] == '6' || s->buffer[103] == '2'
			|| s->buffer[103] == '7') ? 'w' : '-';
	mode[3] = (s->buffer[103] == '5' || s->buffer[103] == '1'
			|| s->buffer[103] == '7') ? 'x' : '-';
	mode[4] = (s->buffer[104] <= '7' && s->buffer[104] >= '4') ? 'r' : '-';
	mode[5] = (s->buffer[104] == '6' || s->buffer[104] == '2'
			|| s->buffer[104] == '7') ? 'w' : '-';
	mode[6] = (s->buffer[104] == '5' || s->buffer[104] == '1'
			|| s->buffer[104] == '7') ? 'x' : '-';
	mode[7] = (s->buffer[105] <= '7' && s->buffer[105] >= '4') ? 'r' : '-';
	mode[8] = (s->buffer[105] == '6' || s->buffer[105] == '2'
			|| s->buffer[105] == '7') ? 'w' : '-';
	mode[9] = (s->buffer[105] == '5' || s->buffer[105] == '1'
			|| s->buffer[105] == '7') ? 'x' : '-';
	mode[10] = '\0';
	return (mode);
}


void			handle_unarchive_t_flag(t_ft_tar *tar, t_untar *s)
{
	while (1)
	{
		s->bytes_read = fread(s->buffer, 1, 512, s->ar);
		if (s->bytes_read < 512)
		{
			fprintf(stderr, "Block misalignment:: less than 512 bytes read\n");
			return ;
		}
		if (!check_if_block_not_empty(s))
			return ;
		if (get_checksum(s) == 0)
			return ;
		s->file_size = cheap_atoo(&s->buffer[124], 12);
		(!tar->opt_v) ? fprintf(s->f, "%s\n", s->buffer) :
			fprintf(s->f, "%9s %s %s\n", cheap_get_mode(s), &s->buffer[265],
					s->buffer);
		while (s->file_size > 0)
		{
			s->bytes_read = fread(s->buffer, 1, 512, s->ar);
			read_file_entry_t_flag(s);
		}
	}
}

void			handle_unarchive_extract(t_ft_tar *tar, t_untar *s)
{
	printf("Extracting from archive::%s\n", tar->opt_f_target);
	while (1)
	{
		s->bytes_read = fread(s->buffer, 1, 512, s->ar);
		s->to_write = (check_file_exists_in_arglist(tar, s) || !*tar->argv) ? 1 : 0;
		if (s->bytes_read < 512)
		{
			fprintf(stderr, "Block misalignment:: less than 512 bytes read\n");
			return ;
		}
		if (!check_if_block_not_empty(s))
			return ;
		if (get_checksum(s) == 0)
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
