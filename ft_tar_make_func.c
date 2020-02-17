/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar_unarchive.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:39:36 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 19:53:40 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

/*
** these are mostly yanked from the ft_unarchive program in the ex00 dir
** because these don't reall need to be updated for the new flags, just 
** libc mkdir applies the umask of the owner of the process calling it,
** so you only make files with less permissions than what the calling process
** the permissions are stored in the mode offset of the header
** you can also just chmod(2) the file afterwords
*/

int			cheap_mkdir_inner(char *path_name, const unsigned int perm_bits)
{
	char	*p;
	int		i;

	p = path_name + 1;
	i = 0;
	while (*p)
	{
		if (*p == '/')
		{
			*p = '\0';
			if ((i = mkdir(path_name, perm_bits ? perm_bits : 0777)))
				if (i != EEXIST)
				{
					fprintf(stderr, "Error making dir %s", path_name);
					return (i);
				}
			*p = '/';
		}
		p++;
	}
	return (i);
}

void		cheap_mkdir(char *dir_name, const unsigned int perm_bits)
{
	int					i;
	size_t				dir_len;
	char				*path;

	i = 0;
	dir_len = strlen(dir_name);
	if (dir_len <= 0)
		return ;
	path = malloc(sizeof(char) * (dir_len + 1));
	strncpy(path, dir_name, dir_len);
	if (path[dir_len - 1] == '/')
		path[dir_len - 1] = 0;
	cheap_mkdir_inner(dir_name, perm_bits);
	if ((i = mkdir(dir_name, perm_bits ? perm_bits : 0777)))
		fprintf(stderr, "Error making dir %s", dir_name);
}

FILE		*cheap_make_file(char *path_name, const unsigned int perm_bits)
{
	FILE		*f;
	char		*p;

	f = fopen(path_name, "wb+");
	if (f == 0)
	{
		p = strrchr(path_name, '/');
		if (p != '\0')
		{
			*p = '\0';
			cheap_mkdir(path_name, perm_bits);
			*p = '/';
			f = fopen(path_name, "wb+");
		}
	}
	chmod(path_name, perm_bits);
	return (f);
}
