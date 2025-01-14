/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_expand_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:49:35 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:49:38 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_file_num(t_d *d)
{
	int		count;
	DIR		*dir;
	char	*temp;

	count = 0;
	temp = get_pwd(d);
	if (temp == NULL)
		return (-1);
	dir = opendir(temp);
	free(temp);
	if (dir == NULL)
	{
		write(2, "2opendir error\n", 14);
		return (-1);
	}
	while (readdir(dir))
		count++;
	closedir(dir);
	return (count);
}

char	**get_pwd_file_list(t_d *d)
{
	struct dirent	*entry;
	char			**filenames;
	DIR				*dir;
	char			**re;

	d->s = get_pwd(d);
	dir = opendir(d->s);
	free(d->s);
	if (dir == NULL)
	{
		write(2, "3opendir error\n", 14);
		return (NULL);
	}
	filenames = malloc(sizeof(char *) * (get_file_num(d) + 1));
	re = filenames;
	entry = readdir(dir);
	while (entry != NULL)
	{
		*filenames++ = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	*filenames = NULL;
	closedir(dir);
	return (re);
}

void	init_indexex(t_i *i)
{
	i->i = 1;
	i->m = 0;
	i->n = 0;
}

//if last char is an active *, return 1, else return 0,
int	get_word(char *pattern, int *st_on, char word[MAXMM][MAXMM])
{
	t_i	i;

	init_indexex(&i);
	while (*pattern)
	{
		if (*pattern == '*' && st_on[i.i] == 1 && i.n != 0)
		{
			word[i.m][i.n] = 0;
			i.n = 0;
			(i.m)++;
			(i.i)++;
		}
		else if (!(*pattern == '*' && st_on[i.i] == 1))
		{
			word[i.m][(i.n)++] = *pattern;
			if (*pattern == '*')
				(i.i)++;
		}
		pattern++;
	}
	word[i.m][i.n] = 0;
	if (pattern[-1] == '*' && st_on[i.i - 1] == 1)
		return (1);
	return (0);
}

int	match(char *s, char *pattern, int st_on[MAXMM])
{
	char	word[MAXMM][MAXMM];
	int		i;
	int		free_end;

	i = 0;
	while (i < MAXMM)
		word[i++][0] = 0;
	free_end = get_word(pattern, st_on, word);
	if (!(*pattern == '*' && st_on[st_on[0]]))
		if (ft_strnstr(s, word[0], ft_strlen(s)) != s)
			return (0);
	i = 0;
	while (word[i][0])
	{
		s = ft_strnstr(s, word[i], ft_strlen(s));
		if (s == NULL)
			return (0);
		s += ft_strlen(word[i]);
		i++;
	}
	if (!free_end && *s)
		return (0);
	return (1);
}
