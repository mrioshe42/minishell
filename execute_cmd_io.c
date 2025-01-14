/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:56:39 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:56:43 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_heredoc_input(int *last_in, t_c *inlink)
{
	char	*f;
	int		fd;

	f = get_temp_file(1);
	fd = open(f, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (-1);
	write(fd, inlink->iofile, ft_strlen(inlink->iofile));
	write(fd, "\n", 1);
	close(fd);
	fd = open(f, O_RDONLY);
	if (fd == -1)
		return (-1);
	free(f);
	if (*last_in != -1)
		close(*last_in);
	*last_in =fd;
	return (1);
}

int	try_write_open(t_c *io, t_d *d)
{
	struct stat	path_stat;
	int			exist;
	int			fd;

	(void) d;
	fd = -999;
	exist = 1;
	if (io->iofile == NULL)
		exist = 0;
	else if (io->iofile[0] == 0)
		exist = 0;
	else if (stat(io->iofile, &path_stat) == -1)
		exist = 0;
	if (exist == 1)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (write(2, "it is a directory.\n", 20), -1);
		if (access(io->iofile, W_OK) == -1)
			return (d->exit_status = 1, write(2, "Permission denied\n", 19), -1);
	}
	if (io->tp == 4)
		fd = open(io->iofile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	else
		fd = open(io->iofile, O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (fd == -1)
		write(2, "No such file or directory\n", 27);
	return (fd);
}


// if success, return fd, otherwise return -1
int	try_readonly_open(t_c *io, t_d *d)
{
	struct stat	path_stat;
	int			exist;

	(void) d;
	exist = 1;
	if (io->iofile == NULL)
		exist = 0;
	else if (io->iofile[0] == 0)
		exist = 0;
	else if (stat(io->iofile, &path_stat) == -1)
		exist = 0;
	if (exist == 0)
		return (d->exit_status = 1, write(2, "No such file or directory\n", 27), -1);
	if (S_ISDIR(path_stat.st_mode))
		return (d->exit_status = 1, write(2, "it is a directory.\n", 20), -1);
	if (access(io->iofile, R_OK) == -1)
		return (d->exit_status = 1, write(2, "Permission denied\n", 19), -1);
	return (open(io->iofile, O_RDONLY));
}
//fd is used as a temp for return of convert_file_name_and_check()
int	link_io_file(int *last_in, int *last_out, t_c *io, t_d *d)
{
	int			fd;

	fd = convert_file_name(io, d);
	if (fd != 1)
		return (fd);
	if (io->tp == 2)
	{
		fd = try_readonly_open(io, d);
		if (fd == -1)
			return (d->exit_status = 1, -1);
		if (*last_in != -1)
			close(*last_in);
		*last_in = fd;
		return (1);
	}
	fd = try_write_open(io, d);
	if (fd == -1)
		return (d->exit_status = 1, -1);
	if (*last_out != -1)
		close(*last_out);
	*last_out = fd;
	return (1);
}

int	process_io_node(int *last_in, int *last_out, t_c *io, t_d *d)
{
	int error_num;

	error_num = -999;
	if (io->tp == 3)
		error_num = set_heredoc_input(last_in, io);
	else
		error_num = link_io_file(last_in, last_out, io, d);
	if (error_num != 1)
		return (d->exit_status = 1, error_num);
	return (1);
}

int	setup_io(t_c *c, t_d *d, t_exe *e)
{
	t_c		*io;
	int		error_num;
	int		last_in;
	int		last_out;

	error_num = -999;
	last_in = -1;
	last_out = -1;
	io = c->io;
	while (io)
	{
		error_num = process_io_node(&last_in, &last_out, io, d);
		if (error_num != 1)
		{
			if (last_in != -1)
				close(last_in);
			if (last_out != -1)
				close(last_out);
			return (error_num);
		}
		io = io->io;
	}
	if (last_out != -1)
		dup2(last_out, STDOUT_FILENO);
	else if (c->otype > 1)
		dup2((e->pipeid)[c->otype][1], STDOUT_FILENO);
	if (last_in != -1)
		dup2(last_in, STDIN_FILENO);
	else if (c->pipe_on == 1)
		dup2(((e->pipeid)[c->id])[0], STDIN_FILENO);
	if (last_in != -1)
		close(last_in);
	if (last_out != -1)
		close(last_out);
	return (1);  //check if close(-1) will trigger error
}

void	close_pipe(t_c *c, t_exe *e)
{
	int	i;
	int	num;

	i = 3;
	num = count_command_nodes(c);
	while (i <= num + 2)
	{
		if ((e->pipeid)[i][0])
			close((e->pipeid)[i][0]);
		if ((e->pipeid)[i][1])
			close((e->pipeid)[i][1]);
		i++;
	}
}
