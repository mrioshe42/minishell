/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:49:00 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:49:05 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//no use
int	reset_old_pwd(t_d *d)
{
	char	*old;
	char	*temp;

	temp = get_pwd(d);
	if (temp == NULL)
		return (-1);
	old = ft_strjoin("OLDPWD=", temp);
	free(temp);
	if (old == NULL)
		return (-1);
	export_one_env(d, old);
	return (1);
}

int	back_to_old_pwd(t_d *d)
{
	char	*new;
	char	*old;
	char	*temp;

	temp = get_pwd(d);
	if (temp == NULL)
		return (-1);
	old = ft_strjoin("OLDPWD=", temp);
	free(temp);
	temp = get_env_value(d, "OLDPWD");
	if (temp == NULL)
	{
		write(2, "OLDPWD is not set\n", 19);
		free(old);
		return (-1);
	}
	chdir(temp);
	new = ft_strjoin("PWD=", temp);
	free(temp);
	export_one_env(d, old);
	export_one_env(d, new);
	free(old);
	free(new);
	return (1);
}

int	cd_home(t_d *d)
{
	char	*new;
	char	*temp;

	new = get_pwd(d);
	if (new == NULL)
		return (-1);
	temp = ft_strjoin("OLDPWD=", new);
	export_one_env(d, temp);
	free(temp);
	free(new);
	new = get_env_value(d, "HOME");
	temp = ft_strjoin("PWD=", new);
	export_one_env(d, temp);
	chdir(new);
	free(temp);
	free(new);
	return (1);
}

int	my_chdir(t_d *d, t_c *c)
{
	char	*temp;
	char	*new;
	char	*old;

	temp = get_pwd(d);
	if (temp == NULL)
		return (-1);
	old = ft_strjoin("OLDPWD=", temp);
	free(temp);
	if (chdir(c->cmd[1]) == -1)
	{
		free(old);
		return (-1);
	}
	export_one_env(d, old);
	free(old);
	temp = get_pwd(d);
	new = ft_strjoin("PWD=", temp);
	free(temp);
	export_one_env(d, new);
	free(new);
	return (1);
}

int	my_cd(t_d *d, t_c *c)
{
	if ((c->cmd)[1] == NULL)
		return (cd_home(d));
	else if ((c->cmd)[2])
	{
		write(2, "Too many args for cd command\n", 30);
		return (-1);
	}
	else if (!dif_str((c->cmd)[1], "-"))
		return (back_to_old_pwd(d));
	else
	{
		if (my_chdir(d, c) == -1)
		{
			write(2, "No such file or directory!\n", 28);
			return (-1);
		}
		return (1);
	}
}
