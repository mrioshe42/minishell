/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:59:27 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:59:28 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//mem checked
char	**find_env_by_name(t_d *d, char *name)
{
	char	**env;
	char	*temp;

	env = *(d->env);
	while (*env)
	{
		temp = get_env_var_name(*env);
		if (dif_str(name, temp))
		{
			env++;
			free(temp);
			temp = NULL;
		}
		else
			break ;
	}
	free(temp);
	return (env);
}

//mem checked
//todelete is the name of the env
void	remove_env(t_d *d, char *todelete)
{
	char	**env;

	env = find_env_by_name(d, todelete);
	if (*env)
	{
		free(*env);
		while (env[1])
		{
			*env = env[1];
			env++;
		}
		*env = NULL;
	}
}

//mem checked
int	unset_env(t_d *d, t_c *c)
{
	char	**ss;
	char	**temp;

	ss = c->cmd + 1;
	temp = ss;
	while (*temp)
	{
		remove_env(d, *temp);
		temp++;
	}
	return (1);
}

//mem checked
int	export_one_env(t_d *d, char *s)
{
	char	*name;

	if (valid_env(s) == 1)
	{
		name = get_env_var_name(s);
		remove_env(d, name);
		free(name);
		add_env(d, s);
	}
	else if (valid_env(s) == -1 || valid_env(s) == 0)
	{
		write(2, " not a valid indentifier\n", 26);
		return (-1);
	}
	return (1);
}

//mem checked
int	export_env(t_d *d, t_c *c)
{
	char	**temp;
	int		error;
	int		re;

	re = 1;
	error = 1;
	temp = c->cmd;
	temp++;
	if (*temp == NULL)
		return (export_NULL(d));
	while (*temp)
	{
		error = export_one_env(d, *temp);
		if (error == -1)
			re = -1;
		temp++;
	}
	return (re);
}
