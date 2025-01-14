/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:50:01 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:50:04 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_check_one_2(t_c *c)
{
	if (c->tp == 8)
	{
		if (c->next == NULL)
			return (0);
		if (c->next->tp == 6 || c->next->tp == 7
			|| c->next->tp == 8 || c->next->tp == 10)
			return (0);
	}
	else if (c->tp == 9)
	{
		if (c->next == NULL)
			return (0);
		if (c->next->tp == 6 || c->next->tp == 7
			|| c->next->tp == 8 || c->next->tp == 10)
			return (0);
	}
	else if (c->tp == 10)
	{
		if (c->next)
			if (c->next->tp == 1 || c->next->tp == 9)
				return (0);
	}
	return (1);
}

int	valid_check_one(t_c *c)
{
	if (c->tp == 1)
	{
		if (c->next)
			if (c->next->tp == 1 || c->next->tp == 9)
				return (0);
	}
	else if (c->tp == 6)
	{
		if (c->next == NULL)
			return (0);
		if (c->next->tp == 6 || c->next->tp == 7 || c->next->tp == 8)
			return (0);
	}
	else if (c->tp == 7)
	{
		if (c->next == NULL)
			return (0);
		if (c->next->tp == 6 || c->next->tp == 7
			|| c->next->tp == 8 || c->next->tp == 10)
			return (0);
	}
	return (valid_check_one_2(c));
}

//return 0, if cmd is not valid, and 1 if valid
int	valid_check(t_c *c)
{
	int	validity;

	validity = 1;
	if (c)
		if (c->tp != 1 && c->tp != 9 && c->tp != 4)
			return (0);
	while (c)
	{
		validity = valid_check_one(c);
		if (validity == 0)
			return (0);
		c = c->next;
	}
	return (1);
}
