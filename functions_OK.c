/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_OK.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:59:20 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:59:22 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//mem checked
int	prt_env(t_d *d)
{
	char	**ent;

	ent = *(d->env);
	while (*ent != NULL)
	{
		if (ft_strchr(*ent, '='))
			printf("%s\n", *(ent));
		ent++;
	}
	return (1);
}

//mem checked
//this function check if the charactor is allowed for env var name
//@ is allowed as expending with @(var_name) is allowed
// int in meaning is the char c in a quotation or not
int	correct_char(int in, char c)
{
	if (in == 1 && (ft_isalnum(c) || c == '_' || c == '\''))
		return (1);
	if (in == 2 && (ft_isalnum(c) || c == '_' || c == '\"'))
		return (1);
	if (in == 0 && (ft_isalnum(c) || c == '_' || c == '\'' || c == '\"'))
		return (1);
	if ((in == 0 || in == 2) && c == '@')
		return (1);
	return (0);
}

//mem checked
//env name can only contain alpnum _, can not star with num
//in: inquotation, 0 for not, 1 for ', 2 for "
int	valid_env(char *s)
{
	int	inquote;

	if (s == NULL)
		return (0);
	if (!ft_isalpha(*s) && *s != 34 && *s != 39 && *s != '_')
		return (0);
	inquote = 0;
	while (correct_char(inquote, *s))
	{
		if (inquote == 2 && *s == '\"')
			inquote = 0;
		else if (inquote == 1 && *s == '\'')
			inquote = 0;
		else if (inquote == 0 && *s == '\"')
			inquote = 2;
		else if (inquote == 0 && *s == '\'')
			inquote = 1;
		s++;
	}
	if (*s == '=')
		return (1);
	if (*s == 0)
		return (1);
	return (-1);
}

//mem checked
int	add_env(t_d *d, char *new)
{
	int		i;
	int		j;
	char	**nvs;

	nvs = *(d->env);
	i = 0;
	while (*nvs)
	{
		i++;
		nvs++;
	}
	nvs = malloc(sizeof(char *) * (i + 2));
	if (nvs == NULL)
		return (0);
	j = 0;
	while (j < i)
	{
		nvs[j] = (*(d->env))[j];
		j++;
	}
	nvs[j] = ft_strdup(new);
	nvs[j + 1] = NULL;
	free(*(d->env));
	*(d->env) = nvs;
	return (1);
}

//used to get name for existing env var
//mem checked
char	*get_env_var_name(char *s)
{
	int		count;
	char	*re;

	re = s;
	count = 0;
	while (*s != '=' && *s != 0)
	{
		if (*s != 34 && *s != 39)
			count++;
		s++;
	}
	s = re;
	re = malloc(count + 1);
	if (re == NULL)
		return (NULL);
	re[count] = 0;
	count = 0;
	while (*s != '=' && *s != 0)
	{
		if (*s != 34 && *s != 39)
			re[count++] = *s;
		s++;
	}
	return (re);
}
