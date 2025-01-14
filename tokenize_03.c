/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_03.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:07:34 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:50:52 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fail return -1, success return 1
int	process_and(char **s, t_st *st, t_list **l, int i)
{
	st->aon = 1;
	(void)st;
	if (i != 0)
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	return (add_new_token(l, s, 2, AND));
}

void	move_to_first_heredoc(t_list **l)
{
	t_list	*t;

	t = *l;
	while (t->prev)
	{
		t = t->prev;
	}
	t = t->next;
	while (t->type != HDOC)
		t = t->next;
	*l = t;
}

void	move_to_next_heredoc(t_list **l)
{
	t_list	*t;

	t = *l;
	while (t)
	{
		if (t->type != HDOC)
			t = t->next;
		else
			break ;
	}
	*l = t;
}

char	*get_next_line_from_string(char **s)
{
	int		i;
	char	*l;

	i = 0;
	while ((*s)[i] && (*s)[i] != '\n')
		i++;
	if (i == 0)
		return (NULL);
	l = malloc(i + 1);
	if (l == NULL)
		return (NULL);
	ft_strlcpy(l, *s, i + 1);
	*s += i;
	if (**s == '\n')
		*s += 1;
	return (l);
}

int	insert_arg(t_list **l, char *arg)
{
	t_list	*temp;

	temp = (*l)->next;
	if (add_new_token(l, &arg, ft_strlen(arg), 1) == -1)
		return (-1);
	(*l)->next = temp;
	return (1);
}
