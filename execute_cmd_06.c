/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_six.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:56:10 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:56:14 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_command_nodes(t_c *c)
{
	int	re;

	//print_cmd_list("count_command_nodes(): a", c);//debug
	while (c->pre)
		c = c->pre;
	//print_cmd_list("count_command_nodes(): b", c);//debug
	re = 0;
	while (c)
	{
		re++;
		c = c->next;
	}
	//print_cmd_list("count_command_nodes(): c", c);//debug
	return (re);
}

int	init_exe(t_c *c, t_exe *e)
{
	int	i;
	int	num;

	e->i = 0;
	e->exits = 0;
	e->ilevel = 0;
	e->slevel = 0;
	num = count_command_nodes(c);
	i = 0;
	while (i <= num)
		(e->pid)[i++] = 0;
	i = 3;
	while (i <= num + 2)
	{
		pipe((e->pipeid)[i]);
		i++;
	}
	return (1);
}

// arg c is the start of a group, returns pointer
//to the next node after the current group
// i represent the level, level at start is 0;
t_c	*skip_next_group(t_c *c)
{
	int	i;

	i = 0;
	if (c == NULL)
		return (NULL);
	if (c->tp == 1)
		return (c->next);
	if (c->tp == 9)
	{
		c = c->next;
		while (c)
		{
			if (c->tp == 9)
				i++;
			if (c->tp == 10)
			{
				if (i == 0)
					return (c->next);
				else
					i--;
			}
			c = c->next;
		}
	}
	return (NULL);
}

//current star level i is set to 0 at the start of this function
void	close_subshell_pipes(t_c *c, t_exe *e)
{
	int	i;

	if (c->tp != 9)
		exit(write(2, "close_subshell_pipes(): wrong!!\n", 33));
	i = 0;
	while (c)
	{
		close(((e->pipeid)[c->id])[0]);
		close(((e->pipeid)[c->id])[1]);
		if (c->tp == 9)
			i++;
		if (c->tp == 10)
			i--;
		if (i == 0 && c->tp == 10)
			break ;
		else
			c = c->next;
		if (c == NULL)
			exit(write(2, "close_subshell_pipes(): wrong!!\n", 33));
	}
}
