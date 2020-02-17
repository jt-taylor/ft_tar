/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 09:53:06 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 21:52:07 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**https://www.gnu.org/software/tar/manual/html_node/Standard.html
*/

/*
** from the tar wikipedia page
** i don't know if we need to use the pre posix or the ustar format
*/

#ifndef ARCHIVE_H
# define ARCHIVE_H
# define HASH_SIZE 32

# include <sys/stat.h>

typedef struct			s_hashnode
{
	ino_t				my_ino;
	char				my_name[100];
	struct s_hashnode	*next;
}						t_hashnode;

typedef struct			s_lstnode
{
	char				file[255];
	struct s_lstnode	*next;
}						t_lstnode;

typedef struct			s_tar
{
	char				file_name[100];
	char				file_mode[8];
	char				owner_id[8];
	char				group_id[8];
	char				file_size[12];
	char				last_mod_time[12];
	char				checksum[8];
	char				file_type[1];
	char				name_of_link[100];
	char				ustar_indicator[6];
	char				ustar_ver[2];
	char				owner_user_name[32];
	char				owner_group_name[32];
	char				device_major_number[8];
	char				device_minor_number[8];
	char				file_name_prefix[155];
	char				zeros[12];
}						t_tar;
/*
** Direct Utility Functions
*/

t_tar					*path_to_tar_header(char *path);

/*
** Hash table
*/

void					table_free(t_hashnode **table);
char					*table_checkadd(t_hashnode **tab, ino_t i, char *s);
void					archive(char *outname, char **inputs);

#endif
