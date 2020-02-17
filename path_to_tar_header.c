/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_to_tar_header.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkubli <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 11:28:42 by zkubli            #+#    #+#             */
/*   Updated: 2020/02/16 21:51:40 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "archive.h"

/*
** returns -1 if whole file:wq can fit in name[100]
** otherwise returns index of last slash to be put in the prefix.
*/

static int	get_split(char **path)
{
	char	*str;
	int		i;
	int		re_val;

	if (**path == '~')
		(*path)++;
	if (**path == '/')
		(*path)++;
	str = *path;
	i = strlen(str);
	if (i < 100)
		return (-1);
	re_val = -1;
	while (i >= 0)
	{
		if (str[i] == '/')
			re_val = i;
		i--;
	}
	return (re_val);
}

static int	checksum(t_tar *header)
{
	int i;
	int	total;

	total = 0;
	i = 0;
	while (i < 512)
	{
		total += ((unsigned char *)(header))[i];
		i++;
	}
	return (total);
}

static char	get_link(struct stat *stats, char *l_name)
{
	static t_hashnode	*table[HASH_SIZE];
	char				*tmp;

	if ((tmp = table_checkadd(table, stats->st_ino, l_name)))
	{
		strncpy(l_name, tmp, 100);
		return ('1');
	}
	else
		bzero(l_name, 100);
	if ((stats->st_mode & S_IFMT) == S_IFLNK)
		return ('2');
	if ((stats->st_mode & S_IFMT) == S_IFCHR)
		return ('3');
	if ((stats->st_mode & S_IFMT) == S_IFBLK)
		return ('4');
	if ((stats->st_mode & S_IFMT) == S_IFDIR)
		return ('5');
	if ((stats->st_mode & S_IFMT) == S_IFIFO)
		return ('6');
	return ('0');
}

static void	force_slash(char *str, struct stat *st)
{
	if ((st->st_mode & S_IFMT) == S_IFDIR)
		str[strlen(str)] = '/';
}

/*
** ? Error handling for paths > 155 or names > 100 ?
** if (split > 154 || strlen(path) > 255 || strlen(path) - split > 99)
**	error("there's no room!")
** Sets the tar_header addrss based on file given in path,
** zeros any unused space.
*/

t_tar		*path_to_tar_header(char *path)
{
	int				split;
	static t_tar	tar;
	struct stat		my_stat;

	stat(path, &my_stat);
	split = get_split(&path) + 1;
	bzero(stpncpy(tar.file_name_prefix, path, split), 155 - split);
	force_slash(strncpy(tar.file_name, path + split, 100), &my_stat);
	snprintf(tar.file_mode, 8, "%06o ", my_stat.st_mode & 0777);
	snprintf(tar.owner_id, 8, "%06o ", my_stat.st_uid);
	snprintf(tar.group_id, 8, "%06o ", my_stat.st_gid);
	snprintf(((char *)&tar) + 124, 13, "%011llo ", my_stat.st_size);
	snprintf(((char *)&tar) + 136, 13, "%011lo ", my_stat.st_mtime);
	snprintf(((char *)&tar) + 148, 9, "        ");
	*tar.file_type = get_link(&my_stat, tar.name_of_link);
	strncpy(stpncpy(((char *)&tar) + 257, "ustar", 6) + 1, "00", 2);
	strncpy(tar.owner_user_name, getpwuid(my_stat.st_uid)->pw_name, 32);
	strncpy(tar.owner_group_name, getgrgid(my_stat.st_gid)->gr_name, 32);
	snprintf(tar.device_major_number, 8, "%06o ", my_stat.st_rdev >> 24);
	snprintf(tar.device_minor_number, 8, "%06o ", my_stat.st_rdev & 0xFFFF);
	bzero(tar.zeros, 12);
	snprintf(tar.checksum, 7, "%06o", checksum(&tar));
	tar.checksum[7] = ' ';
	return (&tar);
}
