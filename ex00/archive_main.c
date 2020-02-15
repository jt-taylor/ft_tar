/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaylor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 10:11:34 by jtaylor           #+#    #+#             */
/*   Updated: 2020/02/15 10:58:11 by jtaylor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** this will be the main for just the ft_archive utility
** the functions that this calls will get rolled into the complete tar utility
** if we get that far
*/

static inline int	add_to_entry(char *file_path, struct s_tar *entry)
{
	struct stat		our_stat;

	if (lstat(file_path, &our_stat))
	{
		fprintf(2, "cannot stat::%s\tError::%s\n",
				file_path, strerror(errno))
			return (-1); // for erorr
	}
	// go through a set of functions to write the stat info to the entry
	return (1); // for success
	// or we can just stop if one of the files doesn't get stat'd
}

static struct s_tar	*create_tar_entry(char *file_path)
{
	struct s_tar	*head;

	head = malloc(sizeof(struct s_tar));
	add_to_entry(file_path, head);
	return (head);
}

static void			add_file_to_archive(FILE *ar, char *file_path)
{
	struct s_tar		*header;
	FILE				*file;
	int					dir;
}

int		main(int ac, char **file_list)
{
	FILE		*ar;
	int			i;

	//protect this
	ar = fopen("ft_archive", "w");//later make this based on the input flags
	if (ac < 2)
	{
		perror("no file list given");
		exit(0);
	}
	i = 1;
	while (i < ac)
		//add_file_to_archive(ar, file_list[i++]);
	fclose(ar);
}
