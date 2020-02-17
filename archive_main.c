/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkubli <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:58:11 by zkubli            #+#    #+#             */
/*   Updated: 2020/02/16 21:51:28 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "archive.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

char	*path_join(char *alpha, char *beta)
{
	int		a;
	int		b;
	int		i;
	char	*re_val;

	a = strlen(alpha);
	b = strlen(beta);
	if (a >= 2 && alpha[a - 1] == '/')
		alpha[--a] = '\0';
	re_val = malloc(a + b + 2);
	i = 0;
	while (i < a)
	{
		re_val[i] = alpha[i];
		i++;
	}
	re_val[i++] = '/';
	while (i < a + b + 1)
	{
		re_val[i] = beta[i - a - 1];
		i++;
	}
	re_val[i] = '\0';
	return (re_val);
}

char	*read_block(int fd)
{
	static char	buffer[512];
	int			read_bytes;

	if (fd == 0 || (read_bytes = read(fd, buffer, 512)) <= 0)
		return (NULL);
	bzero(buffer + read_bytes, 512 - read_bytes);
	return (buffer);
}

void	add_file(int fd_out, char *input)
{
	DIR				*dr;
	struct dirent	*de;
	char			*tmp;
	int				fd_in;

	printf("\ninput:%s\n", input);
	fd_in = open(input, O_RDONLY);
	write(fd_out, (char *)path_to_tar_header(input), 512);
	while ((tmp = read_block(fd_in)))
		write(fd_out, tmp, 512);
	printf("\ninput:%s\n", input);
	if ((dr = opendir(input)))
	{
		while ((de = readdir(dr)))
		{
			if (de->d_name[0] == '.')
				continue ;
			tmp = path_join(input, de->d_name);
			add_file(fd_out, tmp);
			free(tmp);
		}
		closedir(dr);
	}
	close(fd_in);
}

void	archive(char *outname, char **inputs)
{
	int		fd_out;
	char	allzeros[1024];

	bzero(allzeros, 1024);
	fd_out = open(outname, O_WRONLY | O_CREAT | O_TRUNC);
	while (*inputs)
	{
		add_file(fd_out, *inputs);
		(inputs)++;
	}
	write(fd_out, allzeros, 1024);
	close(fd_out);
}
