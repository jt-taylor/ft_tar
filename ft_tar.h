/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 11:20:33 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 11:53:35 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAR_H
# define FT_TAR_H

# include "archive.h"
# include "dearchive.h"

typedef struct		s_ft_tar
{
	int			opt_x;
	int			opt_c;
	int			opt_v;
	int			opt_f;
	int			opt_t;
	int			opt_p;
	char		*opt_f_target;
	char		**argv;
}					t_ft_tar;

#endif
