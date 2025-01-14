/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_four.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:56:30 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:56:33 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_mem_in_subshell_and_exit(t_c *c, t_d *d, t_exe *e)
{
	close_pipe(c, e);
	if (c)
		free_command_list(c);
	free_heap_env(d);
	exit(d->exit_status);
}

void	do_in_subshell(t_c *c, t_d *d, t_exe *e)
{
	in_subshell = 1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	//print_cmd_list("do_in_subshell():", c); //debug
	if (setup_io(c, d, e) == -1)
		clean_mem_in_subshell_and_exit(c, d, e);
	close_pipe(c, e);
	if (execve(c->cmd[0], c->cmd, *(d->env)) == -1)
	{
		perror("execve failed");
		if (errno == EACCES)
			exit(126);
		else if (errno == ENOENT)
			exit(127);
		else
			exit(1);
	}
}

