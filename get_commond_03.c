/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commond_03.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:09:44 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:09:47 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// register new eof, and return the num
//of chars to be skipped before next process
// after skip, *s should point to the
//first char following the heredoc dilimiter
int	register_eof(char *s)
{
	char	*eof;
	int		re;

	eof = get_eof(s + 2);
	unclosed_eofs(1, eof);
	re = 2;
	while (white_space(s[re]))
		re++;
	return (re + ft_strlen(eof));
}

//for nomineette
int	process_logi_symbol2(char *s, t_st *st)
{
	if (*s == '&')
	{
		if (s[1] == '&')
		{
			st->aon = 1;
			return (2);
		}
	}
	else if (*s == '(')
		st->pon += 1;
	else if (*s == ')')
	{
		st->pon -= 1;
		if (st->pon == -1)
			return (-1);
	}
	return (1);
}

//return the num of characters which should be skipped, befor next process
//if return -1, it mean illegal command
// if return -2, it mean found second command line
int	process_logi_symbol(char *s, t_st *st)
{
	if (check_illegal_cmd(s) == -1)
		return (-1);
	if (!white_space(*s))
		close_special_char(st);
	if (*s == '>')
	{
		if (s[1] == '>')
			return (2);
	}
	else if (*s == '<')
	{
		if (s[1] == '<')
			return (register_eof(s));
	}
	else if (*s == '|')
	{
		if (s[1] == '|')
		{
			st->oon = 1;
			return (2);
		}
		st->von = 1;
	}
	return (process_logi_symbol2(s, st));
}

//return the num of characters which should be skipped, befor next process
//if return -1, it mean illegal command
int	process_special_char(char *s, t_st *st)
{
	int	next;

	next = 1;
	if (*s == 34)
		st->don = 1;
	else if (*s == 39)
		st->son = 1;
	else
		next = process_logi_symbol(s, st);
	return (next);
}

// return -1 for illegal cmd
// return -2 for found second cmd
int	go_through_chars_one_by_one(char *s, t_st *st)
{
	int	next;
	int	eof_status;

	while (*s)
	{
		next = 1;
		if (!st->son && !st->don)
		{
			if (*s == '\n' && !st->von && !st->pon && !st->oon && !st->aon)
			{
				eof_status = check_unclosed_eof(s + 1);
				return (eof_status);
			}
			next = process_special_char(s, st);
			if (next == -1)
				return (-1);
		}
		else if (st->don && *s == 34)
			st->don = 0;
		else if (st->son && *s == 39)
			st->son = 0;
		s += next;
	}
	return (1);
}
