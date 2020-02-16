/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 09:53:06 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/15 20:38:32 by jtaylor          ###   ########.fr       */
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

struct s_tar
{
	char		file_name[100];
	char		file_mode[8];
	char		owner_id[8];
	char		group_id[8];
	char		file_size[12];
	char		last_mod_time[12];
	char		checksum[8];
	char		file_type[1]; // link indicator
	char		name_of_link[100];
	// if we need to do ustar
	char		ustar_indicator[6]; // ustar indicator "ustar" then null
	char		ustar_ver[2]; // "00"
	char		owner_user_name[32];
	char		owner_group_name[32];
	char		device_major_number[8];
	char		device_minor_number[8];
	char		file_name_prefix[155];
};

#endif
