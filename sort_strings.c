/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_strings.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrios-he <mrios-he@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:04:16 by mrios-he          #+#    #+#             */
/*   Updated: 2024/09/14 15:10:45 by mrios-he         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp_helper(char c1, char c2)
{
	if (ft_tolower((unsigned char)c1) == ft_tolower((unsigned char)c2))
	{
		if (c1 != c2)
		{
			if (c1 >= 'a' && c1 <= 'z')
				return (-1);
			if (c2 >= 'a' && c2 <= 'z')
				return (1);
		}
		return (0);
	}
	return (ft_tolower((unsigned char)c1) - ft_tolower((unsigned char)c2));
}

int	ft_strcmp_part1(char *s1, char *s2, int *i, int *j)
{
	char	c1;
	char	c2;
	int		result;

	while (s1[*i] && s2[*j])
	{
		c1 = s1[*i];
		c2 = s2[*j];
		if (c1 == '_')
		{
			(*i)++;
			continue ;
		}
		if (c2 == '_')
		{
			(*j)++;
			continue ;
		}
		result = ft_strcmp_helper(c1, c2);
		if (result != 0)
			return (result);
		(*i)++;
		(*j)++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	j;
	int	result;

	i = 0;
	j = 0;
	result = ft_strcmp_part1(s1, s2, &i, &j);
	if (result != 0)
		return (result);
	return (s1[i] - s2[j]);
}

void	sort_remaining_names(char **s)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (s[i])
	{
		j = i + 1;
		while (s[j])
		{
			if (ft_strcmp(s[i], s[j]) > 0)
			{
				temp = s[i];
				s[i] = s[j];
				s[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	sort_strings(char **s, int n)
{
	int	i;
	int	j;

	i = 0;
	while (s[i] && n)
	{
		if (s[i][0] == '.')
		{
			free(s[i]);
			j = i;
			while (s[j])
			{
				s[j] = s[j + 1];
				j++;
			}
		}
		else
			i++;
	}
	sort_remaining_names(s);
}
