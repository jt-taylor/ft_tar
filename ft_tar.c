/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 23:39:47 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 21:54:20 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "archive.h"
#include "dearchive.h"
#include "ft_tar.h"

/*
** wann try using getopt for this
** seeing as it'll probably be a while before i ever get to use it for 42 stuff
** again
*/

static void			ft_get_opts_inner(t_ft_tar *tar, int ac, char **argv)
{
	int		c;

	while ((c = getopt(ac, argv, "xcvf:tp")) != -1)
	{
		if (c == 'x')
			tar->opt_x = 1;
		else if (c == 'c')
			tar->opt_c = 1;
		else if (c == 'v')
			tar->opt_v = 1;
		else if (c == 'f')
		{
			tar->opt_f = 1;
			tar->opt_f_target = optarg;
		}
		else if (c == 't')
			tar->opt_t = 1;
		else if (c == 'p')
			tar->opt_p = 1;
	}
}

static void			ft_tar_get_options(t_ft_tar *tar, const int ac, char **argv)
{
	int		arg_offset;

	arg_offset = 1;
	ft_get_opts_inner(tar, ac, argv);
	while (argv[arg_offset] && argv[arg_offset][0] == '-')
		arg_offset += 1;
	if (tar->opt_f)
		arg_offset++;
	tar->argv = argv + arg_offset;
}

static inline void	ft_init_tar_struct_options(t_ft_tar *tar)
{
	tar->opt_x = 0;
	tar->opt_c = 0;
	tar->opt_v = 0;
	tar->opt_f = 0;
	tar->opt_t = 0;
	tar->opt_p = 0;
	tar->opt_f_target = 0;
}

/*
** static void		test_print_options(t_ft_tar *tar)
** {
** 	printf("opt_x = %d,\
** 			opt_c = %d,\
** 			opt_v = %d,\
** 			opt_f = %d,\
** 			opt_t = %d,\
** 			opt_p = %d,\
** 			opt_f_targets = %s\n",
** 			tar->opt_x, tar->opt_c, tar->opt_v,
** 			tar->opt_f, tar->opt_t, tar->opt_p,
** 			tar->opt_f_target);
** }
*/

/*
** insert the logic for going to either the archive handle
** or the uynarchive handle
*/

int					main(int ac, char **argv)
{
	t_ft_tar		tar;

	ft_init_tar_struct_options(&tar);
	ft_tar_get_options(&tar, ac, argv);
	if (tar.opt_x || tar.opt_t)
		untar_handle(&tar);
	else if (tar.opt_c == 1 && tar.opt_f)
		archive(tar.opt_f_target, tar.argv);
	else
		perror("missing input arguments");
}
