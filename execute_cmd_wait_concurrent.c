/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_eight.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:55:50 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:55:53 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_cocurrent(t_c *c, t_d *d, t_exe *e)
{
	int	i;

	//printf("wait_cocurrent():cmd[%d]:ilevel:%d, slevel:%d\n", c->id, e->ilevel, e->slevel);//debug
	//d->exit_status = 999; //debug
	(void)c;
	if (e->ilevel == e->slevel)
		return ;
	else
	{
		i = e->slevel + 1;
		while (i <= e->ilevel)
		{
			waitpid(e->pid[i], &(d->raw_exit_received), 0);
			e->pid[i] = -1;
			in_subshell--;
			i++;
		}
		e->ilevel = e->slevel;
	}
	//printf("wait_cocurrent()a: now at cmd[%d], d->exit_status is:%d\n", c->id, d->exit_status); //debug

	d->exit_status = parse_raw_exit_status(d);
	//printf("wait_cocurrent()b: now at cmd[%d], d->exit_status is:%d\n", c->id, d->exit_status); //debug
	check_core_dump(d);
}
