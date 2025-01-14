/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_token_into_cmd_03.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:47:10 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:47:14 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//when it is a new command, a new command node will be created
//  other wise the argument will be add to current command node
t_c	*add_argument(t_c *c, t_list **t)
{
	int	i;

	if (c->tp >5)
	{
		c->next = create_new_command_node(c);
		if (c->next == NULL)
			return (NULL);
		c = c->next;
		c->tp = 1;
	}
	if (c->cmd == NULL)
	{
		c->cmd = create_cmd_area();
		if (c->cmd == NULL)
			return (NULL);
	}
	i = 0;
	while ((c->cmd)[i] && i < MAXMM)
		i++;
	if (i == MAXMM)
		return (printf("add_argument(): too many arguments\n"), NULL);
	(c->cmd)[i] = ft_strdup((*t)->token);
	*t = (*t)->next;
	return (c);
}

t_c	*add_redi_arg(t_c *c, t_list *t)
{
	t_c	*temp;

	temp = c;
	while (temp->io)
		temp = temp->io;
	temp->io = create_new_command_node(NULL);
	if (temp->io == NULL)
		return (NULL);
	temp = temp->io;
	temp->tp = t->type;
	temp->iofile = ft_strdup(t->next->token);
	return (c);
}

void	print_logi(int tp)
{
	if (tp == 2)
		printf("<\n");
	if (tp == 3)
		printf("<<\n");
	if (tp == 4)
		printf(">\n");
	if (tp == 5)
		printf(">>\n");
	if (tp == 6)
		printf("|\n");
	if (tp == 7)
		printf("||\n");
	if (tp == 8)
		printf("&&\n");
	if (tp == 9)
		printf("(\n");
	if (tp == 10)
		printf(")\n");
}
