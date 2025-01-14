/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_token_into_cmd_04.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:46:22 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:46:24 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	print_token_error(t_list *t)
{
	if (t->type == 2)
		write(2, "syntax error near unexpected token '<'\n", 39);
	if (t->type == 3)
		write(2, "syntax error near unexpected token '<<'\n", 40);
	if (t->type == 4)
		write(2, "syntax error near unexpected token '>'\n", 39);
	if (t->type == 5)
		write(2, "syntax error near unexpected token '>>'\n", 40);
	if (t->type == 6)
		write(2, "syntax error near unexpected token '|'\n", 39);
	if (t->type == 7)
		write(2, "syntax error near unexpected token '||'\n", 40);
	if (t->type == 8)
		write(2, "syntax error near unexpected token '&&'\n", 40);
	if (t->type == 9)
		write(2, "syntax error near unexpected token '('\n", 39);
	if (t->type == 10)
		write(2, "syntax error near unexpected token ')'\n", 39);
}
t_c	*load_logic_sign(t_c *c, t_list **t)
{
	c->next = create_new_command_node(c);
	if (c->next == NULL)
		return (NULL);
	c = c->next;
	c->tp = (*t)->type;
	(*t) = (*t)->next;
	return (c);
}

t_c	*for_add_redi(t_c *c, t_list **t)
{
	if (c->tp > 5)
	{
		c->next = create_new_command_node(c);
		if (c->next == NULL)
			return (NULL);
		c = c->next;
		c->tp = 1;
	}
	if ((*t)->next == NULL)
		return (write(2, "syntax error near unexpected token `newline'\n", 46), NULL);
	if ((*t)->next->type > 1)
		return (print_token_error(*t) ,NULL);
	else
	{
		c = add_redi_arg(c, *t);
		*t = (*t)->next;
		*t = (*t)->next;
	}
	return (c);
}

t_c	*load_token_different_type(t_list *t, t_c *head)
{
	t_c	*c;

	c = head;
	while (t)
	{
		if (t->type > 5)
			c = load_logic_sign(c, &t);
		else if (t->type < 2)
			c = add_argument(c, &t);
		else if (t->type > 1 && t->type < 6)
			c = for_add_redi(c, &t);
		if (c == NULL)
			break ;
	}
	return (c);
}

t_c	*load_token_into_cmd(t_list *t)
{
	t_c	*c;
	t_c	*head;

	if (t == NULL)
		return (NULL);
	head = create_new_command_node(NULL);
	if (head == NULL)
		return (NULL);
	c = load_token_different_type(t, head);
	if (c == NULL)
		return (free_command_list(head), NULL);
	c = head->next;
	c->pre = NULL;
	free(head);
	return (c);
}
/*
*/ //debug
void    print_cmd_list(char *ss, t_c *c)
{
	char **s;
	t_c *temp;

	printf("%s----------------\n", ss);
	while (c)
	{
		printf("the cmd %d is type %d:------\n", c->id, c->tp);
		if (c->tp == 4)
		{
			printf("io node:\n");
			temp = c;
			while (temp->io)
			{
				if (temp->io->tp == 4)
					printf("> ");
				if (temp->io->tp == 5)
					printf(">> ");
				if (temp->io->tp == 2)
					printf("< ");
				if (temp->io->tp == 5)
					printf("<< ");
				printf("%s\n", temp->io->iofile);
				temp = temp->io;
			}
			printf("\n");
		}
		else if (c->tp >1)
		{
			printf("logic operator: ");
			print_logi(c->tp);
		}
		else
		{
			if (c->cmd)
			{
				s = c->cmd;
				while (*s)
					printf("%s, ", *s++);
			}
			printf("\nread from pipe: ");
			if (c->pipe_on == 1)
				printf("Yes\n");
			else
				printf("No\n");
			printf("output piped to pipe[%d]\n", c->otype);
			temp = c;
			while (temp->io)
			{
				if (temp->io->tp == 2)
					printf("< ");
				if (temp->io->tp == 3)
					printf("<< ");
				if (temp->io->tp == 4)
					printf("> ");
				if (temp->io->tp == 5)
					printf(">> ");
				printf("%s\n", temp->io->iofile);
				temp = temp->io;
			}
			printf("\n");
		}
		c = c->next;
	}
}
/* //debug
*/
