/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_05.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:07:49 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:08:07 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//fail return -1, success return 1, i is the first
//	char following the current token
int	get_token(char **s, t_st *st, t_list **l)
{
	int	i;

	while (white_space(**s) && **s != '\n')
		(*s)++;
	if (**s != '\n' )
		close_special_char(st);
	i = find_next_token(s, st);
	if ((*s)[i] == 0)
		return (add_last(s, l, i));
	if (white_space((*s)[i]) && (*s)[i] != '\n')
		return (add_new_token(l, s, i, 1));
	if (ft_strrchr("()|&<>", (*s)[i]))
		return (get_token_2(s, i, st, l));
	if ((*s)[i] == '\n')
	{
		if (!st->von && !st->pon && !st->oon && !st->aon)
			return (find_new_line(s, st, l, i));
		else
		{
			if (i != 0)
				i = add_new_token(l, s, i, 1);
			return ((*s)++, i);
		}
	}
	return (-1);
}

void	init_token_head(t_list *re)
{
	re->prev = NULL;
	re->type = -999;
	re->token = NULL;
	re->next = NULL;
}

// s is the input, re is the pointer to the
//	token list to return, l point to the last token node
t_list	*tokenize_input(char *s)
{
	t_st	st;
	t_list	*re;
	t_list	*l;

	if (s == NULL)
		return (NULL);
	init_status(&st);
	re = malloc(sizeof(t_list));
	if (re == NULL)
		return (NULL);
	init_token_head(re);
	l = re;
	while (*s)
	{
		if (get_token(&s, &st, &l) == -1)
		{
			free_token_list(re);
			return (NULL);
		}
	}
	l = re->next;
	l->prev = NULL;
	free(re);
	return (l);
}
