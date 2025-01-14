/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commond_04.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:09:57 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:50:26 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 34: ", 39: '
//return situations: 0 for incomplete, 1 for complete,
// return -1 for illegal cmd
// return -2 for found second cmd
int	parse_line(char *s, t_st *st)
{
	int	error;

	error = go_through_chars_one_by_one(s, st);
	if (error == -1 || error == -2)
		return (error);
	else if (st->son || st->don || unclosed_eofs(0, NULL))
	{
		return (0);
	}
	else if (st->pon || st->von || st->oon || st->aon)
	{
		return (0);
	}
	else
		return (1);
}

//this function return the substring of s,
// skipping white spaces of the head of s
//s is freed in this function, no need to free later
char	*skip_head_white_space(char *s)
{
	char	*re;
	char	*temp;

	temp = s;
	while (*s)
	{
		if (white_space(*s))
			s++;
		else
			break ;
	}
	re = ft_strdup(s);
	free(temp);
	return (re);
}

//appnd the new input to the existing commmd, \n added between
char	*connect_cmd_line(char *cmd, char *input)
{
	char	*temp;

	temp = ft_strjoin(cmd, "\n");
	free (cmd);
	cmd = ft_strjoin(temp, input);
	free(temp);
	free(input);
	return (cmd);
}

//print error message depend on the error num n
void	printf_error(int n)
{
	if (n == -1)
		printf("illegal command\n");
	if (n == -2)
		printf("more than 1 command line found, not requrired!!\n");
}

//son: single_quote_on, don: double_quote_on
//input is line read from stdin, cmd is the
//connected inputs which mean to be a command line
char	*get_command_restart(char *tofree, int errornom, t_d *d)
{
	printf_error(errornom);
	free(tofree);
	return (get_command(d));
}
