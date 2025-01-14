/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commond_02.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:09:21 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:09:25 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//return situations: 0 for complete, 1 for
//	< error, 2 for << error, 3 for > error,
//4 for >> error, 5 for | error, 6 for
//	(error, -1 for incomplete input, -2 for unexpected EOF
//inquote = 1 for ' quote, inquote = 2 for ",
//	inquote = 0 for not in quote
// this function check if | || && is on, if yes, close it
void	close_special_char(t_st *st)
{
	if (st->von)
		st->von = 0;
	else if (st->oon)
		st->oon = 0;
	else if (st->aon)
		st->aon = 0;
}

//return 1, just because 1 is not a special
// syntax in this project
// so that the program know it is a literal char,
// as we treat & as a literal char
char	next_char(char *s)
{
	s++;
	while (whitespace(*s))
		s++;
	if (*s == '&')
	{
		if (s[1] == '&')
			return ('&');
		else
			return (1);
	}
	return (*s);
}

// return 1 if the output is bad, else return 0
int	is_bad_output(char *s)
{
	if (*s == ')')
	{
		s++;
		while (white_space(*s))
			s++;
		if (*s == '>')
		{
			s++;
			if (*s == '>')
				s++;
			while (white_space(*s))
				s++;
			if (ft_strchr("<>|()", *s))
				return (1);
			else if (*s == '&')
				if (s[1] == '&')
					return (print_error_next_char(s), 1);
		}
	}
	return (0);
}

void	print_error_next_char(char *s)
{
	char	temp[2];

	temp[0] = next_char(s);
	temp[1] = 0;
	write(2, "syntax error near unexpected token '", 36);
	write(2, temp, 1);
	write(2, "'\n", 2);
}

//return -1 for illegal cmd
int	check_illegal_cmd2(char *s)
{
	if (*s == '&')
	{
		if (s[1] != '&')
			return (1);
		s++;
		if (ft_strchr("|&)", next_char(s)))
			if (*ft_strchr("><|&()", next_char(s)))
				return (print_error_next_char(s), -1);
		return (1);
	}
	if (*s == '(')
		if (ft_strchr("|&)", next_char(s)))
			if (*ft_strchr("><|&()", next_char(s)))
				return (print_error_next_char(s), -1);
	if (*s == ')')
		if (ft_strchr("<(", next_char(s)))
			if (*ft_strchr("<(", next_char(s)))
				return (print_error_next_char(s), -1);
	if (is_bad_output(s))
		return (-1);
	return (1);
}

//return -1 for illegal cmd, 1 for legal
int	check_illegal_cmd(char *s)
{
	if (*s == '>')
	{
		if (s[1] == '>')
			s++;
		if (ft_strchr("><|&()", next_char(s)))
			if (*ft_strchr("><|&()", next_char(s)))
				return (print_error_next_char(s), -1);
	}
	else if (*s == '<')
	{
		if (s[1] == '<')
			s++;
		if (ft_strchr("><|&()", next_char(s)))
			if (*ft_strchr("><|&()", next_char(s)))
				return (print_error_next_char(s), -1);
	}
	else if (*s == '|')
	{
		if (s[1] == '|')
			s++;
		if (ft_strchr("|&)", next_char(s)))
			if (*ft_strchr("|&)", next_char(s)))
				return (print_error_next_char(s), -1);
	}
	return (check_illegal_cmd2(s));
}
