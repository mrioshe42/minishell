/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   function2_OK.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:58:32 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:58:34 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(t_d *d)
{
	char	temp[MAXMM];

	(void)d;
	if (getcwd(temp, MAXMM))
		return (ft_strdup(temp));
	else
	{
		write(2, "getcwd() error", 15);
		return (NULL);
	}
}

/*
//mem checked
char	*get_pwd(t_d *d)
{
	char	**env;
	char	*temp;

	env = *(d->env);
	while (*env)
	{
		temp = get_env_var_name(*env);
		if (dif_str("PWD", temp))
		{
			env++;
			free(temp);
		}
		else
			break ;
	}
	free(temp);
	if (*env == NULL)
		return (NULL);
	return (ft_strdup(*env + 4));
}
*/

//mem checked
void	remove_back_slash(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (*s)
	{
		s++;
		i++;
	}
	s--;
	i--;
	while (*s == '/' && i != 0)
	{
		*s-- = 0;
		i--;
	}
}

//mem checked
void	process_star_for_path_concat2(char **s, char **i)
{
	while (*(*s) != '/' && *(*s))
		*(*i)++ = *(*s)++;
	if (*(*s) == '/')
		*(*i)++ = *(*s)++;
}

//mem checked
void	process_star_for_path_concat(char *dest, char **s, char **i)
{
	if ((*s)[1] == '.')
	{
		if ((*s)[2] == '/' || (*s)[2] == 0)
		{
			(*s) += 2;
			if ((*i) > dest)
			{
				*--(*i) = 0;
				(*i)--;
				while (*(*i) != '/')
					*(*i)-- = 0;
				(*i)++;
			}
		}
		else
			process_star_for_path_concat2(s, i);
	}
	else if ((*s)[1] == '/' || (*s)[1] == 0)
		(*s)++;
	else
		process_star_for_path_concat2(s, i);
}

//mem checked
//this function used to concate char s to dest, both consider as path
//this function does not check path exist or not, can be checked with opendir()
void	concat_path(char *dest, char *s)
{
	char	*i;

	i = dest;
	while (*i)
		i++;
	while (*s)
	{
		while (*s == '/')
			s++;
		if (*s == '.')
			process_star_for_path_concat(dest, &s, &i);
		else
		{
			while (*s != '/' && *s)
				*i++ = *s++;
			if (*s == '/')
				*i++ = *s++;
		}
		*i = 0;
	}
}
