/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_five.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:56:21 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:56:23 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external_cmd(t_c *c, t_d *d, t_exe *e)
{
	char	*temp;

	temp = c->cmd[0];
	c->cmd[0] = full_function_name(c->cmd[0], d);
	free(temp);
	if (check_cmd_ok(c, d) == -1)
	{
		close(((e->pipeid)[c->id])[0]);
		close(((e->pipeid)[c->id])[1]);
		return (-1);
	}
	e->ilevel++;
	e->pid[e->ilevel] = fork();
	if (e->pid[e->ilevel] == -1)
	{
		write(2, "fork error\n", 11);
		d->exit_status = -1;
	}
	else if (e->pid[e->ilevel] == 0)
		do_in_subshell(c, d, e);
	else
	{
		in_subshell++;
		close(((e->pipeid)[c->id])[0]);
		close(((e->pipeid)[c->id])[1]);
	}
	return (1);
}

int	execute_one_cmd(t_c *c, t_d *d, t_exe *e)
{
	int		error_num;

	if (c->cmd == NULL)
		return (execute_pure_io_node(c, d, e));
	error_num = convert_cmd_2_string(c, d);
	//print_cmd_list("execute_one_cmd(): after convert cmd:", c);//debug
	if (error_num != 1)
	{
		setup_io(c, d, e);
		d->exit_status = 1;
		close(((e->pipeid)[c->id])[0]);
		close(((e->pipeid)[c->id])[1]);
		return (-1);
	}
	if (c->cmd[0] == NULL)
		return (execute_pure_io_node(c, d, e));
	if (is_builtin(c->cmd))
		return (execute_inbuilt(c, d, e));
	return (execute_external_cmd(c, d, e));
}
//do nothing for heredoc, just return (0)
int	open_file(t_c *io, t_d *d)
{
	char	*temp;
	int		fd;

	if (io->tp == 3)
		return (0);
	fd = -1;
	temp = file_name_to_open(io->iofile, d);
	if (check_file_ok(io) == -1)
		return (-1);
	if (io->tp == 2)
		fd = open(temp, O_RDONLY);
	else if (io->tp == 4)
		fd = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	else if (io->tp == 5)
		fd = open(temp, O_WRONLY | O_CREAT | O_APPEND, 0600);
	free(temp);
	if (fd == -1)
		return (write(2, "minishell: open file error!\n", 29), -1);
	return (fd);
}
void	write_to_ofile(int fd, t_c *c, t_exe *e)
{
	char	buff[128];
	ssize_t	bytes_read;

	if (c->pre == NULL)
		return ;
	if (c->pre->tp == 10)
	{
		bytes_read = read((e->pipeid)[c->id][0], buff, sizeof(buff));
		while (bytes_read > 1)
		{
			write(fd, buff, bytes_read);
			bytes_read = read((e->pipeid)[c->id][0], buff, sizeof(buff));
		}
	}
	close(fd);
}

int	execute_pure_io_node(t_c *c, t_d *d, t_exe *e)
{
	int	fd;
	int	last_out;
	t_c	*io;

	close(((e->pipeid)[c->id])[1]);
	last_out = -1;
	io = c->io;
	while (io)
	{
		if (io->tp == 3)
		{
			io = io->io;
			continue ;
		}
		if (convert_file_name(io, d) == -1)
		{
			if (last_out != -1)
				close(last_out);
			return (d->exit_status = 1, -1);
		}
		fd = open_file(io, d);
		if (fd == -1)
		{
			if (last_out != -1)
				close(last_out);
			return (d->exit_status = 1, -1);
		}
		if ((io->tp == 4 || io->tp == 5) && last_out != -1)
			close(last_out);
		if (io->tp == 4 || io->tp == 5)
			last_out = fd;
		else
			if (fd != 0)
				close(fd);
		io = io->io;
	}
	if (last_out != -1)
		write_to_ofile(last_out, c, e);
	return (close(((e->pipeid)[c->id])[0]), d->exit_status);
}

int	execute_inbuilt(t_c *c, t_d *d, t_exe *e)
{
	int	saved_stdout;
	int	saved_stdin;

	if (!dif_str((c->cmd)[0], "exit"))
		my_exit(c, d, e);
//	d->exit_status = 0;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (setup_io(c, d, e) == -1)
	{
		close(saved_stdout);
		close(saved_stdin);
		d->exit_status = 1;
		return (-1);
	}
	close(((e->pipeid)[c->id])[0]);
	close(((e->pipeid)[c->id])[1]);
	call_builtin(is_builtin(c->cmd), d, c);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return (d->exit_status);
}
