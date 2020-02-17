/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unarchive_helper_funcions.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 14:44:21 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 17:01:44 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

/*
** Theres not many ways to rewrite the un(de?)archiver utilities from tar
** so it's cool to see how tar actually is working under the hood
** but actually coding this has been pretty uninteresting so far
** probably not going to be that much more fun implementing all the opts
** tomorrow
** at least its fun to mess around with the whole standard library
** this also won't handle names that require the use of the longer name format
** with the file name prefix provided from the ustar format
*/

/*
** this goes through the path name and at each '/' it tries to make a dir
** of that name
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

/*
** im not sure how I don't have a variant of this in my libft yet
** also there is mkdirat which makes relative dirs created from the associated
** file which looks neat but not usefool for this project
*/

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
	free(path);
}

/*
** don't write to existing files
** try to make the file
** if cant make file try to make the dirs to it
*/

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
	return (f);
}
