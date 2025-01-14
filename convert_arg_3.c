/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_arg_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:54:38 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:54:42 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//maybe access permission should be check if, >iofile doesn't exist
int	check_file_ok(t_c *io)
{
	struct stat	path_stat;

	if (io->iofile == NULL)
		return (write(2, "No such file or directory\n", 27), -1);
	if (io->iofile[0] == 0)
		return (write(2, "No such file or directory\n", 27), -1);
	if (stat(io->iofile, &path_stat) == 0)
		if (S_ISDIR(path_stat.st_mode))
			return (write(2, "it is a directory.\n", 20), -1);
	if (io->tp == 2)
	{
		if (access(io->iofile, F_OK) == -1)
			return (write(2, "No such file or directory\n", 27), -1);
		if (access(io->iofile, R_OK) == -1)
			return (write(2, "Permission denied\n", 19), -1);
	}
	else
	{
		if (access(io->iofile, F_OK) == 0)
			if (access(io->iofile, W_OK) == -1)
				return (write(2, "Permission denied\n", 19), -1);
	}
	return (1);
}

//only when io->tp is 2 or 4 or 5, this function will be called
//return -1 for fail, else return 1, -2 for ambiguous
int	convert_file_name(t_c *io, t_d *d)
{
	int		num;
	char	new[MAXMM][MAXMM];

	num = 0;
	while (num < MAXMM)
		new[num++][0] = 0;
	num = dollar_expand_dequo_io_file(io, d, new);
	//printf("convert_file_name(): num is %d\n", num);//debug
	if (num > 1)
	{
		write(2, "ambiguous rederection\n", 23);
		return (-1);
	}
	free(io->iofile);
	io->iofile = ft_strdup(new[0]);
	//printf("convert_file_name(): iofile is x%sx\n", io->iofile);//debug

	return (1);
}

//i is use to mark which arg is to be processed
//when error happen when process_one_arg(), i will be the error_num
int	convert_cmd_2_string(t_c *c, t_d *d)
{
	int	i;

	if (c->tp != 1)
		return (1);
	if (c->cmd == NULL)
		return (1);
	i = 0;
	while ((c->cmd)[i])
	{
		i = process_one_arg(c, d, i);
		if (i < 0)
			return (i);
	}
	return (1);
}
