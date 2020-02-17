/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unarchive_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 17:29:55 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/16 14:52:55 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dearchive.h"

int		main(int ac, char **argv)
{
	if (ac == 2)
	{
		if (argv[1])
			open_archive_file(argv[1]);
	}
	else
	{
		fprintf(stderr, "missing arguments");
		exit(1);
	}
	return (0);
}
