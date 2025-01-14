/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_token_into_cmd_01.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:47:56 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:48:05 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// $substituion and *expansion will be done later,
//  but $substitution must be done before
//  *expansion, refer to readme for the reason

/*
#define CMD  0  string
#define ARG  1  string
#define IFIL 2  <
#define HDOC 3  <<
#define OUT1 4  >
#define OUT2 5  >>
#define PIP  6  |
#define OR   7  ||
#define AND  8  &&
#define PAR1 9  (
#define PAR2 10 )
*/

void	free_io(t_c *cmd)
{
	t_c	*temp;

	temp = cmd->io;
	while (temp)
	{
		free(temp->iofile);
		cmd = temp;
		temp = temp->io;
		free(cmd);
	}
}

void	free_command_list(t_c *cmd)
{
	t_c		*temp;
	char	**s;

	while (cmd)
	{
		if (cmd->io)
			free_io(cmd);
		if (cmd->cmd)
		{
			s = cmd->cmd;
			while (*s)
			{
				free(*s);
				s++;
			}
			free (cmd->cmd);
		}
		temp = cmd->next;
		free(cmd);
		cmd = temp;
	}
}


char	**create_cmd_area(void)
{
	int		i;
	char	**re;

	re = malloc(sizeof(char *) * MAXMM);
	if (re == NULL)
	{
		write(2, "memory error\n", 13);
		return (NULL);
	}
	i = 0;
	while (i < MAXMM)
		re[i++] = NULL;
	return (re);
}

t_c	*create_new_command_node(t_c *c)
{
	t_c	*re;

	re = malloc(sizeof(t_c));
	if (re == NULL)
	{
		write(2, "memory error\n", 13);
		return (NULL);
	}
	re->cmd = NULL;
	re->id = -1;
	re->next = NULL;
	re->iofile = NULL;
	re->io = NULL;
	re->pipe_on = -1;
	re->pre = c;
	re->tp = 999;
	re->temp = NULL;
	re->otype = -1;
	return (re);
}
