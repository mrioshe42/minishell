/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_01.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:07:12 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:07:17 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//when success return pointer to new node, when fail return NULL
/*
*/ //debug
void	print_token_list(t_list *head)
{
	t_list	*current;

	current = head;
	while (current != NULL)
	{
		printf("Token: %s, Type: %d\n", current->token, current->type);
		current = current->next;
	}
}
/* //debug
*/

void	free_token_list(t_list *head)
{
	t_list	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->token);
		free(tmp);
	}
}

char	*ft_strsub(char *s, int start, int len)
{
	char	*re;
	int		i;

	if (s == NULL)
		return (NULL);
	re = malloc(len + 1);
	i = 0;
	while (i < len && s[start + i])
	{
		re[i] = s[start + i];
		i++;
	}
	re[i] = 0;
	return (re);
}

//fail return -1, success return 1
int	add_new_token(t_list **l, char **s, int i, int type)
{
	(*l)->next = malloc(sizeof(t_list));
	if ((*l)->next == NULL)
		return (-1);
	(*l)->next->type = type;
	(*l)->next->next = NULL;
	(*l)->next->prev = (*l);
	if (type < 2)
	{
		(*l)->next->token = ft_strsub(*s, 0, i);
		if ((*l)->next->token == NULL)
			return (-1);
	}
	else
		(*l)->next->token = NULL;
	*s += i;
	*l = (*l)->next;
	return (1);
}

//fail return -1, success return 1
int	add_last(char **s, t_list **l, int i)
{
	int	re;

	if (i != 0)
	{
		re = add_new_token(l, s, i, 1);
		if (re == -1)
			return (-1);
	}
	return (1);
}
