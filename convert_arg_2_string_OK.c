/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_arg_2_string_OK.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:51:02 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:51:23 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
* by experiment we know that dequotation, $substitution and *expansion are done
after tokens loaded into command structure
* $substitution and *expansion is done during dequotation, not before not after,
* $substitution is done before expansion
* after $substitution, a simple tokenization is done, just do the spliting with
space ' ',
	splitted token will be treated as argument of a command line
* after tokens loaded into cmd structure, $substitution produced out put and
   input sign >< in c->cmd[] doen't trigger redirections any more
*
*/

//to parse command line and find the name of env var after char $
char	*get_env_name(char *s)
{
	int		i;
	char	*re;

	/*if (!ft_isalnum(s[0]) && s[0] != '_')
		return (NULL);*/
	i = 0;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			break ;
		i++;
	}
	re = malloc(i + 1);
	if (re == NULL)
		return (NULL);
	re[i] = 0;
	i--;
	while (i >= 0)
	{
		re[i] = s[i];
		i--;
	}
	return (re);
}

void	process_sub_when_no_quote_on(char new[MAXMM][MAXMM], t_a *a)
{
	char	*s;

	s = a->temp;
	while (*(a->temp))
	{
		if (white_space(*(a->temp)))
		{
			new[a->i][a->j] = 0;
			a->i += 1;
			a->j = 0;
			new[a->i][a->j] = 0;
			while (white_space(*(a->temp)))
				(a->temp)++;
		}
		else
		{
			if (*(a->temp) == '*')
				(a->star_on)[++((a->star_on)[0])] = 1;
			new[a->i][(a->j)++] = *(a->temp)++;
		}
	}
	free(s);
	a->temp = NULL;
}

//except * and white space, all character in a env var, when doing $substitude,
// is treated as literal char, e.g. "" won't trigger quotation any more
void	process_dsubstitution(char new[MAXMM][MAXMM], t_a *a)
{
	char	*s;

	s = a->temp;
	if ((a->st).son || (a->st).don)
	{
		while (*(a->temp))
		{
			if (*(a->temp) == '*')
				(a->star_on)[((a->star_on)[0])++] = 0;
			new[a->i][(a->j)++] = *(a->temp)++;
		}
		free(s);
		a->temp = NULL;
		return ;
	}
	process_sub_when_no_quote_on(new, a);
}

//mem checked
int	count_string(char new[MAXMM][MAXMM])
{
	int	i;

	i = 0;
	while (new[i][0] && i < MAXMM - 1)
		i++;
	return (i);
}

//mem checked
int	have_wildcard(int *st_on)
{
	int	i;

	i = 1;
	while (i <= st_on[0])
	{
		if (st_on[i])
			return (1);
		i++;
	}
	return (0);
}
