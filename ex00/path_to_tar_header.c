/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_to_tar_header.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkubli <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 11:28:42 by zkubli            #+#    #+#             */
/*   Updated: 2020/02/16 03:00:16 by zkubli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <diren
#include <sys/stat.h>
#include <string.h>
#include "archive.h"

/*
** returns -1 if whole file can fit in name[100]
** otherwise returns index of last slash to be put in the prefix.
*/

static int	get_split(char *str)
{
	int i;
	int re_val;

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

static int	checksum(char *path)
{
	DIR				my_dir;
	struct reader	my_reader;

	my_dir = opendir(path);
}

static char	get_link(struct stat *stats, char *path_to_file)
{
	static t_hashnode	*table[HASH_SIZE];
	char				*tmp;

	if ((tmp = table_checkadd(table, stats->st_ino)))
	{
		strncpy(path_to_file, tmp, 100);
		return ('1');
	}
	else
		bzero(path_to_file, 100);
	if ((stats->st_mode & ST_IFMT) == ST_IFLNK)
		return ('2');
	if ((stats->st_mode & ST_IFMT) == ST_IFCHR)
		return ('3');
	if ((stats->st_mode & ST_IFMT) == ST_IFBLK)
		return ('4');
	if ((stats->st_mode & ST_IFMT) == ST_IFDIR)
		return ('5');
	if ((stats->st_mode & ST_IFMT) == ST_IFIFO)
		return ('6');
	return ('0');
}

/*
** ? Error handling for paths > 155 or names > 100 ?
** if (split > 154 || strlen(path) > 255 || strlen(path) - split > 99)
**	error("there's no room!")
**
** Sets the tar_header addrss based on file given in path,
** zeros any unused space.
**
*/
void		path_to_tar_header(char *path, t_tar *tar)
{
	int			split;
	char		*tmp;
	struct stat	my_stat;

	split = last_slash(path) + 1;
	bzero(stpncpy(tar->file_name_prefix, path, split), 155 - split);
	strncpy(tar->file_name, path + split, 100);
	stat(path, &my_stat);
	snprintf(tar->file_mode, "6o ", my_stat.st_mode, 7);
	snprintf(tar->owner_id, "6o ", my_stat.st_uid, 7);
	snprintf(tar->group_id, "6o ", my_stat.st_gid, 7);
	snprintf(tar->file_size, "11o ", my_stat.st_gid, 12);
	snprintf(tar->last_mod_time, "11o ", my_stat.st_gid, 12);
	snprintf(tar->checksum, "            ", my_stat.st_gid, 12);
	tmp = ((my_sta);
	if (tar->)


}
