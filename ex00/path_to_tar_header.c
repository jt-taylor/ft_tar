/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_to_tar_header.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkubli <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 11:28:42 by zkubli            #+#    #+#             */
/*   Updated: 2020/02/15 15:33:53 by zkubli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <string.h>
#include "archive.h"

/*
** Returns index of last forward slash in string, -1 if no slash found.
*/

static int	last_slash(char *str)
{
	int i;
	int re_val;

	i = 0;
	re_val = -1;
	while (str[i])
	{
		if (str[i] == '/')
			re_val = i;
		i++;
	}
	return (re_val);
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
void		path_to_tar(char *path, t_tar *tar)
{
	int			split;
	struct stat	my_stat;

	split = last_slash(path) + 1;
	bzero(stpncpy(tar->file_name_prefix, path, split), 155 - split);
	strncpy(tar->file_name, path + split, 100);
	stat(path, &my_stat);
	memcpy(&tar->file_mode, &my_stat.st_mode, 8);
	memcpy(&tar->owner_id, &my_stat.st_uid, 8);
	memcpy(&tar->group_id, &my_stat.st_gid, 8);
	memcpy(&tar->file_size, &my_stat.st_size, 12);
	//final time is 12 bytes
	//st_mtime is 8 bytes, st_mtimespec is 16, which to use?
	memcpy(&tar->last_mode_time, &my_stat.st_mtime, 8);
	memcpy(&tar->last_mode_time + 8, &my_stat.st_mtimespec + 4, 4);
	//first 8 of mtime and last 4 of mtimespec ?
	//4 bytes covers all nanoseconds so I guess this makes sense
	memcpy(&tar->file_mode, &my_stat.st_mode, 8);
	bzero(&tar->checksum, 8);
	tar->file_type = ((my_stat.st_mode & ST_IFMT) == ST_IFLNK);
	if (tar->file_type)


}
