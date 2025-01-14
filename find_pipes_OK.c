/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_pipes_OK.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:57:41 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:57:43 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_c	*find_upper_group_end(t_c *c)
{
	int	i;

	i = 0;
	while (c)
	{
		if (c->tp == 9)
			i++;
		if (c->tp == 10)
		{
			if (i == 0)
				break ;
			else
				i--;
		}
		c = c->next;
	}
	return (c);
}

t_c	*find_following_pipe(t_c *c)
{
	while (c)
	{
		if (c->next)
		{
			if (c->next->tp == 6 || c->next->tp == 4)
				break ;
			else
				c = find_upper_group_end(c->next);
		}
		else
			c = c->next;
	}
	return (c);
}

int	find_group_pipe(t_c *c)
{
	c = find_upper_group_end(c);
	//if (c) //debug
		//printf("find_group_pipe(): upper group end is %d\n", c->id);//debug
	c = find_following_pipe(c);
	//if(c) //debug
		//printf("find_group_pipe(): before next pipe is %d\n", c->id);//debug
	if (c == NULL)
		return (1);
	c = c->next;
	//printf("find_group_pipe(): next now c is %d\n", c->id);//debug
	if (c->tp == 4)
	{//debug
		//printf("find_group_pipe(): find the pipe: %d", c->id);//debug
		return (c->id);
	}//debug
	while (c)
	{
		if (c->tp == 1 || c->tp == 4)
			break ;
		c = c->next;
	}
	if (c == NULL)
		return (write(2, "wrong command input\n", 21));
	return (c->id);
}

void	mark_read_from_pipe(t_c *c)
{
	while (c)
	{
		while (c)
		{
			if (c->tp == 6)
				break ;
			c = c->next;
		}
		if (c == NULL)
			return ;
		else
		{
			while (c)
			{
				if (c->tp == 1 || c->tp == 4)
					break ;
				c = c->next;
			}
			if (c == NULL)
				write(2, "check the program, it's bad\n", 29);
			else
				c->pipe_on = 1;
		}
		c = c->next;
	}
}

int	find_next_executable(t_c *c)
{
	while (c)
	{
		if (c->tp == 1 || c->tp == 4)
			break ;
		c = c->next;
	}
	if (c == NULL)
		exit(write(2, "wrong input, check your program logic\n", 39));
	return (c->id);
}

//the pipes index start from 0 for command 0
//i used to keep track of the index of
//pipeid[i] will be use for command[i] as input pipe.
//otype is index of the pipid[] to with the cmd should redirect it's output
//  when otype is 1, it is stdout, no need to redirect the output
//  that is why the cmd id start from 2, because 1 is for stdout 2
//is for stderror
void	find_pipes(t_c *c)
{
	mark_read_from_pipe(c);
	while (c)
	{
		if (c->tp == 1 || c->tp == 4)
		{
			if (c->next == NULL)
				c->otype = 1;
			else
			{
				if (c->next->tp == 6)
					c->otype = find_next_executable(c->next);
				else
					c->otype = find_group_pipe(c);
			}
		}
		c = c->next;
	}
}
