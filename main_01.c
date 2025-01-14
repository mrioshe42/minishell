/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_01.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:10:26 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:10:28 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	return (b);
}

char	*clean_all_eofs(char *eofs[MAXMM])
{
	int	i;

	i = 0;
	while (i < MAXMM)
	{
		if (eofs[i] != NULL)
		{
			free(eofs[i]);
			eofs[i] = NULL;
		}
		i++;
	}
	return (NULL);
}

char	*remove_one_eof(char *eofs[MAXMM])
{
	int		i;
	char	*temp;

	i = 0;
	while (i < MAXMM)
	{
		if (eofs[i] == NULL)
			i++;
		else
			break ;
	}
	if (i == MAXMM)
	{
		return (NULL);
	}
	else
	{
		temp = eofs[i];
		eofs[i] = NULL;
		return (temp);
	}
}

char	*add_eof(char *eofs[MAXMM], char *eof)
{
	int	i;

	i = 0;
	while (i < MAXMM)
	{
		if (eofs[i])
			i++;
		else
			break ;
	}
	if (i == MAXMM)
	{
		write(2, "two many unclosed heredoc\n", 26);
		return (NULL);
	}
	else
	{
		eofs[i] = eof;
		return (eof);
	}
}
