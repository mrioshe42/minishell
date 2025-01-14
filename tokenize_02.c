/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_02.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:07:24 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:50:42 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fail return -1, success return 1
int	process_pipe(char **s, t_st *st, t_list **l, int i)
{
	if ((*s)[1] == '|')
		st->oon = 1;
	else
		st->von = 1;
	if (i != 0)
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	if ((*s)[1] != '|')
		return (add_new_token(l, s, 1, PIP));
	return (add_new_token(l, s, 2, OR));
}

//fail return -1, success return 1
int	process_input(char **s, t_st *st, t_list **l, int i)
{
	char	*eof;

	(void)st;
	if (i != 0)
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	if ((*s)[1] != '<' )
		return (add_new_token(l, s, 1, IFIL));
	if (add_new_token(l, s, 2, HDOC) == -1)
		return (-1);
	eof = get_eof(*s);
	unclosed_eofs(1, eof);
	while (white_space(**s))
		(*s)++;
	*s += ft_strlen(eof);
	return (1);
}

//fail return -1, success return 1
int	process_output(char **s, t_st *st, t_list **l, int i)
{
	(void)st;
	if (i != 0)
	{
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	}
	if ((*s)[1] != '>')
		return (add_new_token(l, s, 1, OUT1));
	return (add_new_token(l, s, 2, OUT2));
}

//fail return -1, success return 1
int	process_left_paren(char **s, t_st *st, t_list **l, int i)
{
	(st->pon)++;
	if (i != 0)
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	return (add_new_token(l, s, 1, 9));
}

//fail return -1, success return 1
int	process_right_paren(char **s, t_st *st, t_list **l, int i)
{
	(st->pon)--;
	if (i != 0)
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	return (add_new_token(l, s, 1, 10));
}
