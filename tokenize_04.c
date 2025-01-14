/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_04.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:07:42 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:49:53 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_one_heredoc(char **s, char *d)
{
	char	*arg;
	char	*line;
	char	*temp;

	arg = NULL;
	line = get_next_line_from_string(s);
	while (line && dif_str(line, d))
	{
		if (arg == NULL)
			arg = ft_strdup(line);
		else
		{
			temp = ft_strjoin(arg, "\n");
			free(arg);
			arg = ft_strjoin(temp, line);
			free(temp);
		}
		free(line);
		line = get_next_line_from_string(s);
	}
	free(line);
	return (arg);
}

//*d is a temp var to store the delimiter
int	load_heredoc(char **s, t_list **l)
{
	char	*d;
	char	*heredoc;

	move_to_first_heredoc(l);
	d = unclosed_eofs(-1, NULL);
	while (d)
	{
		heredoc = get_one_heredoc(s, d);
		if (heredoc == NULL)
			insert_arg(l, "");
		else
			insert_arg(l, heredoc);
		if (heredoc)
			free(heredoc);
		free(d);
		move_to_next_heredoc(l);
		if (*l == NULL)
			break ;
		d = unclosed_eofs(-1, NULL);
	}
	return (1);
}

//fail return -1, success return 1
int	find_new_line(char **s, t_st *st, t_list **l, int i)
{
	(void)st;
	if (i != 0)
	{
		if (add_new_token(l, s, i, 1) == -1)
			return (-1);
	}
	*s += 1;
	if (**s)
	{
		if (unclosed_eofs(0, NULL))
			return (load_heredoc(s, l));
		else
		{
			write(2, "more than 1 line of cmd is not required\n", 40);
			return (-1);
		}
	}
	return (-1);
}

int	find_next_token(char **s, t_st *st)
{
	int	i;

	i = 0;
	while ((*s)[i])
	{
		if (special_char((*s)[i]) && !st->son && !st->don
			&& !((*s)[i] == '&' && (*s)[i + 1] != '&'))
			break ;
		if (!(st->son) && (*s)[i] == 34)
			st->don = 1 - st->don;
		if (!(st->don) && (*s)[i] == 39)
			st->son = 1 - st->son;
		if (!st->son && !st->don && (*s)[i] == '(')
			st->pon++;
		if (!st->son && !st->don && (*s)[i] == ')')
			st->pon--;
		i++;
	}
	return (i);
}

int	get_token_2(char **s, int i, t_st *st, t_list **l)
{
	if ((*s)[i] == '(')
		return (process_left_paren(s, st, l, i));
	if ((*s)[i] == ')')
		return (process_right_paren(s, st, l, i));
	if ((*s)[i] == '|')
		return (process_pipe(s, st, l, i));
	if ((*s)[i] == '&')
		return (process_and(s, st, l, i));
	if ((*s)[i] == '<')
		return (process_input(s, st, l, i));
	if ((*s)[i] == '>')
		return (process_output(s, st, l, i));
	return (-1);
}
