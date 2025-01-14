/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star_expand_03.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:45:46 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:45:48 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*/ //debug
void	print_strings(char **s)
{
	while (*s)
	{
		printf("%s\n", *s);
		s++;
	}
}
/* //debug
*/

//expand new[i], and insert them back to new;
//return new[i] is substituded by how many new strings
int	wild_star(t_d *d, char new[MAXMM][MAXMM], int st_on[MAXMM], int i)
{
	char	**f;
	int		fn;

	if (!contain_active_star(new[i], st_on))
		return (1);
	f = get_pwd_file_list(d);
	fn = 0;
	while (f[fn])
	{
		if (match(f[fn], new[i], st_on))
			fn++;
		else
			remove_file_name(f, fn);
	}
	if (f[0] == NULL)
		return (free(f), 1);
	if (ft_strncmp(new[i], "*", 1) == 0)
        sort_strings(f, 1);
    else
    {    
		sort_strings(f, 0);
	}
	return (insert(new, i, f));
}

int	count_star(char *s)
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*s == '*')
			i++;
		s++;
	}
	return (i);
}

void	star_expansion(t_d *d, char new[MAXMM][MAXMM], int st_on[MAXMM])
{
	int	i;
	int	count_s;

	st_on[0] = 1;
	i = 0;
	while (new[i][0])
	{
		count_s = count_star(new[i]);
		i += wild_star(d, new, st_on, i);
		st_on[0] += count_s;
	}
}
