/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dearchive.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 17:46:58 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 20:52:21 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEARCHIVE_H
# define DEARCHIVE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <errno.h>
# include <ctype.h>
# include "ft_tar.h"

# ifndef UN_TAR
#  define UN_TAR

typedef struct	s_untar
{
	FILE			*ar;
	FILE			*f;
	char			buffer[512];
	size_t			bytes_read;
	int				file_size;
	int				to_write;
}				t_untar;

# endif

/*
** -----------------------------------------------------------------------------
*/

void			read_file_entry_t_flag(t_untar *s);
void			handle_unarchive_t_flag(t_ft_tar *tar, t_untar *s);
int				check_if_block_not_empty(t_untar *s);
int				cheap_atoo(const char *str, int num_len);
void			handle_file_type(t_untar *s);
int				get_checksum(t_untar *s);
int				get_checksum(t_untar *s);
FILE			*cheap_make_file(char *path_name, const unsigned int perm_bits);
void			cheap_mkdir(char *dir_name, const unsigned int perm_bits);
int				cheap_mkdir_inner(char *path_name,
					const unsigned int perm_bits);
void			untar_handle(t_ft_tar *tar);
void			untar_handle_mode(t_ft_tar *tar, t_untar *s);
void			read_file_entry(t_untar *s);
int				check_file_exists_in_arglist(t_ft_tar *tar, t_untar *s);
void			handle_unarchive_extract(t_ft_tar *tar, t_untar *s);

#endif
