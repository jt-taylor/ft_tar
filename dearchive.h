/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dearchive.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 17:46:58 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/15 21:10:12 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEARCHIVE_H
# define DEARCHIVE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# ifndef UN_TAR
# define UN_TAR

typedef struct			s_untar
{
	FILE			*ar;
	FILE			*f;
	char			buffer[512];
	size_t			bytes_read;
	int				file_size;
}						t_untar;

# endif

#endif
