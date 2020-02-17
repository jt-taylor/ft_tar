/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unarchiver_helper_misc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 14:46:25 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 14:52:49 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

/*
** mostly for the checksum
** we need to give it a len because there is no padding in the tar headers
** also all of the numbers are stored as octals because tar is old
** and performance optimazaiotns mattered a lot more on old hardware
*/

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

/*
** init the tar struct
*/

void			init_untar_struct(t_untar *s)
{
	int		i;

	i = 0;
	while (i < 512)
	{
		s->buffer[i] = 0;
		i++;
	}
	s->ar = 0;
	s->f = 0;
	s->bytes_read = 0;
	s->file_size = 0;
}

/*
** pretty much just calls untar
*/

void			open_archive_file(char *path_to_archive)
{
	t_untar		s;

	init_untar_struct(&s);
	s.ar = fopen(path_to_archive, "rb");
	if (s.ar == NULL)
	{
		fprintf(stderr, "Cannot open %s\n", path_to_archive);
		exit(1);
	}
	untar(&s, path_to_archive);
	fclose(s.ar);
}

/*
** go through the tar entries and make the file from them
** read blocks at a time
** infin loop
** 	read from archive
** 	if empty break out of loop
** 	validate checksum
** 	handle filetype
**	while filesize
**		write file
*/

void			untar(t_untar *s, const char *path)
{
	printf("Extracting from archive::%s\n", path);
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
		handle_file_type(s);
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

/*
** from the gnu tar manual
** The chksum field represents the simple sum of all bytes in the header block.
** Each 8-bit byte in the header is added to an unsigned integer,
** initialized to zero,
** the precision of which shall be no less than seventeen bits.
** When calculating the checksum,
** the chksum field is treated as if it were all blanks.
** httpos://www.gnu.org/software/tar/manual/html_node/Standard.html
**
** basically just add the value of all of the bytes together
** apparently the standard tar uses unsigned chars for the this
** but i coudn't find that in the gnu tar manual and i really don't want to look
** throught the tar source code -- its a mess
**
** checksum of the tar header and not the actual file if it wasn't clear
** space is used for where the checksum is in the header
*/

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
