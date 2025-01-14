/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_expand_02.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:45:30 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:45:34 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_file_name(char **f, int fn)
{
	free(f[fn]);
	while (f[fn + 1])
	{
		f[fn] = f[fn + 1];
		fn++;
	}
	f[fn] = NULL;
}

//push lines back re lines, starting from line i
void	push_back(char new[MAXMM][MAXMM], int i, int re)
{
	int	h;
	int	count;

	count = 0;
	while (new[count][0])
		count++;
	count += re - 1;
	while (count > i)
	{
		h = 0;
		while (h < MAXMM)
		{
			new[count + re - 1][h] = new[count][h];
			h++;
		}
		count--;
	}
}

void	clean_char_table(char **f)
{
	int	i;

	i = 0;
	while (f[i])
		free(f[i++]);
	free (f);
}

int	insert(char new[MAXMM][MAXMM], int i, char **f)
{
	int	re;
	int	j;

	re = 0;
	while (f[re])
		re++;
	push_back(new, i, re);
	j = 0;
	while (j < re)
	{
		ft_strlcpy(new[i + j], f[j], ft_strlen(f[j]) + 1);
		j++;
	}
	clean_char_table(f);
	return (re);
}

int	contain_active_star(char *s, int st_on[MAXMM])
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*s == '*')
		{
			i++;
			if (st_on[i] == 1)
			{
				return (1);
			}
		}
		s++;
	}
	return (0);
}
