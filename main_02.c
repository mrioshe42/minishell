/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_02.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:10:36 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:48:48 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//add = 1 for add, -1 for remove first, 0 for check first,
//return
char	*unclosed_eofs(int add, char *eof)
{
	static char	*eofs[MAXMM];
	int			i;

	if (add == 1)
		return (add_eof(eofs, eof));
	else if (add == -1)
		return (remove_one_eof(eofs));
	else if (add == -2)
		return (clean_all_eofs(eofs));
	else if (add == 0)
	{
		i = 0;
		while (i < MAXMM)
		{
			if (eofs[i] == NULL)
				i++;
			else
				break ;
		}
		if (i == MAXMM)
			return (NULL);
		else
			return (eofs[i]);
	}
	return (NULL);
}
/*
*/ //debug
void	printsini(char *s)
{
	size_t	i;

	i = 0;
	while (i <= strlen(s))
		printf("%d, ", s[i++]);
}
/* //debug
*/
char	*trimmed(char *s)
{
	char	*end;

	if (s == NULL)
		return (NULL);
	if (*s == 0)
		return (NULL);
	end = s + ft_strlen(s) - 1;
	while (end >= s && white_space(*end))
	{
		*end-- = '\0';
	}
	while (white_space(*s))
	{
		s++;
	}
	return (s);
}

int	dif_str(char *s, char *t)
{
	if (s == NULL && t == NULL)
		return (0);
	if (s == NULL || t == NULL)
		return (1);
	else
	{
		while (*s == *t && *s != 0)
		{
			s++;
			t++;
		}
		if (*s == *t)
			return (0);
		return (1);
	}
}
