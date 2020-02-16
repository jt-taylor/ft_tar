/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unarchive_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 17:29:55 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/15 21:08:12 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "archive.h"
#include "dearchive.h"

/*
** mostly for the checksum
** we need to give it a len because there is no padding in the tar headers
** also all of the numbers are stored as octals because tar is old
** and performance optimazaiotns mattered a lot more on old hardware
*/

static int	cheap_atoo(const char *str, int num_len)
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
** ignore files of the types that are not dir's or 'plain' files
*/

static void	handle_file_type(t_untar *s)
{
	char		opt = s->buffer[156];
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
		//create directorry func
		s->file_size = 0;
	}
	else if (opt == 6)
		printf("Ignore FIFO device\n");
	else
	{
		s->f = 0;//create file func
	}
}

/*
** read the file in blocks of 512 from the tar file
** and write them to the open file
*/

static void	read_file_entry(t_untar *s)
{
	if (s->bytes_read < 512)
	{
		fprintf(stderr, "Block misalignment:: less than 512 bytes read\n");
		return ;
	}
	if (s->file_size < 512)
		s->bytes_read = 512; // stay aligned to the blocks
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
	if (s->f)
		fclose(s->f);//close if open
	s->f = 0;
}

/*
** tar denotes the end each entry with an empty block
** or a block consiting only of null;
** return (1) if not empty
** return (0) if it is empty
*/

static int	check_if_block_not_empty(t_untar *s)
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

static int	get_checksum(t_untar *s)
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

/*
** go through the tar entries and make the file from them
** read blocks at a time
*/

static void	untar(t_untar *s, const char *path)
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
		if (!check_if_block_not_empty(s))//check end of archive;// tar denotes this with a completely empty bloc
			return ;
		if (get_checksum(s) == 0)// if this fails then indicate file corruption
			return ;
		s->file_size = //parse filesize from octal number ;
		handle_file_type(s);//switch case to handle the filetypes ie HARDLINK // symlink // block //char device // fifo
		while (s->file_size > 0)
		{
			s->bytes_read = fread(s->buffer, 1, 512, s->ar);
			while (s->file_size > 0)
				read_file_entry(s);
		}
	}
}

/*
** init the tar struct
*/

static void	init_untar_struct(t_untar *s)
{
	int		i = 0;

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

static void	open_archive_file(char *path_to_archive)
{
	t_untar		s;

	init_untar_struct(&s);
	s.ar = fopen(path_to_archive, "rb");
	if (s.ar == NULL)
	{
		fprintf(stderr, "Cannot open %s\n", path_to_archive);
		exit(1);
	}
	//untar(s);
	fclose(s.ar);
}

int		main(int ac, char **argv)
{
	if (ac == 2)
	{
		if (argv[1])
			open_archive_file(argv[1]);
	}
	else
	{
		fprintf(stderr, "missing arguments");
		exit(1);
	}
	return (0);
}
