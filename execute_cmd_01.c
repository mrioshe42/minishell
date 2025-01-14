/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_seven.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:56:00 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:56:04 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_raw_exit_status(t_d *d)
{
    if (WIFEXITED(d->raw_exit_received))
        return (WEXITSTATUS(d->raw_exit_received));
	else if (WIFSIGNALED(d->raw_exit_received))
		return (128 + WTERMSIG(d->raw_exit_received));
    return (1);
}

t_c	*execute_command_new_subshell(t_c *c, t_d *d, t_exe *e)
{
	(void) d;
	(e->ilevel)++;
	e->pid[e->ilevel] = fork();
	if (e->pid[e->ilevel] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		in_subshell = 1;
		e->slevel = e->ilevel; //modified
		c = c->next;
	}
	else
	{
		close_subshell_pipes(c, e);
		c = skip_next_group(c);
	}
	return (c);
}

t_c	*execute_command_line3(t_c *c, t_d *d, t_exe *e)
{
	if (c->tp == AND)
	{
		close(((e->pipeid)[c->id])[1]);
		close(((e->pipeid)[c->id])[0]);
		c = c->next;
		wait_cocurrent(c, d, e);
		if (d->exit_status != 0)
			c = skip_next_group(c);
	}
	else if (c->tp == 9)
		c = execute_command_new_subshell(c, d, e);
	else if (c->tp == 10)
	{
		//close(((e->pipeid)[c->id])[1]);
		//close(((e->pipeid)[c->id])[0]);
		close_pipe(c, e); //modified
		//printf("execute_command_line3():at cmd%d exit status before wait: %d\n",c->id, d->exit_status);//debug
		wait_cocurrent(c, d, e);
		//printf("execute_command_line3():exit with cmd%d exit status %d\n", c->id, d->exit_status);//debug
		free_command_list(c);
		free_heap_env(d);
		exit(d->exit_status);
	}
	return (c);
}

int	execute_command_line2(t_c *c, t_d *d, t_exe *e)
{
	while (c)
	{
		if (c->tp == 1 || c->tp == 4)
		{
			execute_one_cmd(c, d, e);
			c = c->next;
		}
		else if (c->tp == PIP)
		{
			close(((e->pipeid)[c->id])[1]);
			close(((e->pipeid)[c->id])[0]);
			c = c->next;
		}
		else if (c->tp == OR)
		{
			close(((e->pipeid)[c->id])[1]);
			close(((e->pipeid)[c->id])[0]);
			wait_cocurrent(c, d, e);
			c = c->next;
			if (d->exit_status == 0)
				c = skip_next_group(c);
		}
		else
			c = execute_command_line3(c, d, e);
	}
	wait_cocurrent(c, d, e);
	return (0);
}

int	execute_command_line(t_c *c, t_d *d)
{
	t_exe	e;

	in_subshell = 0;
	if (!valid_check(c))
	{
		write(2, "not a valid command line\n", 26);
		d->exit_status = 2;
		return (-1);
	}
	find_pipes(c);
	//print_cmd_list("execute_command_line():after find pipe:\n", c);//debug
	if (!init_exe(c, &e))
	{
		write(2, "not enough memory\n", 19);
		d->exit_status = ENOMEM;
		return (-1);
	}
	//print_cmd_list("execute_command_line():\n", c); //debug
	return (execute_command_line2(c, d, &e));
}
