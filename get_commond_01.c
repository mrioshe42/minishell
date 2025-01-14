/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commond_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:08:30 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:08:41 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
here we as required by the project, we only consider () as priority symble,
	and run command inside the () in a subshell.
when ( is open, the minishell will search for )
	to complete the command line input,
	after that minishell will try to close unclosed <<eof
in bash, after unclosed (, each line are treated as a new command line,
	we will not write our minishell like this,
	as it is not required by the project,
	and in this way bash has bug it self, which we should avoid.
*/

int	spcial_char(char c)
{
	if (ft_strrchr("'\"<>()|&", c) || white_space(c))
		return (1);
	return (0);
}

char	*get_eof(char *s)
{
	int		i;
	char	*re;

	while (white_space(*s))
		s++;
	i = 0;
	while (*(s + i) && !spcial_char(*(s + i)))
		i++;
	re = malloc(i + 1);
	if (re == NULL)
	{
		printf("mem eorror\n");
		return (NULL);
	}
	else
	{
		ft_strlcpy(re, s, i + 1);
		re[i] = 0;
		return (re);
	}
}

//return 1 if there is unclosed eof
//return 0 if all eof are closed and s ended
//return -2 if all eof are closed but cmd s is not ended
int	check_unclosed_eof(char *s)
{
	char	*eof;
	char	*line;

	eof = unclosed_eofs(0, NULL);
	while (eof)
	{
		line = get_next_line_from_string(&s);
		while (line && dif_str(line, eof))
		{
			free(line);
			line = get_next_line_from_string(&s);
			if (line == NULL)
				break ;
		}
		if (line == NULL)
			return (1);
		free(line);
		free(unclosed_eofs(-1, NULL));
		eof = unclosed_eofs(0, NULL);
	}
	if (*s == 0)
		return (0);
	return (-2);
}

void	init_status(t_st *s)
{
	s->don = 0;
	s->pon = 0;
	s->son = 0;
	s->n = 0;
	s->aon = 0;
	s->von = 0;
	s->oon = 0;
	s->s = NULL;
	s->fll = 1;
}
