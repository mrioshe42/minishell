/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_OK.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:57:14 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:57:18 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// tbd heredoc is disgarded, content of ->heredoc should be read from ->ifile.
int	is_builtin(char **s)
{
	if (!dif_str(s[0], "echo"))
		return (1);
	if (!dif_str(s[0], "cd"))
		return (2);
	if (!dif_str(s[0], "pwd"))
		return (3);
	if (!dif_str(s[0], "export"))
		return (4);
	if (!dif_str(s[0], "unset"))
		return (5);
	if (!dif_str(s[0], "env"))
		return (6);
	if (!dif_str(s[0], "exit"))
		return (7);
	else
		return (0);
}

void	free_heap_env(t_d *d)
{
	char	**key;

	key = *(d->env);
	while (*key)
	{
		free(*key);
		*key = NULL;
		key++;
	}
	free(*(d->env));
	d->env = NULL;
}

int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_with_arg(t_d *d, t_c *c, int i)
{
	long long	re;

	if (is_llong(c->cmd[1], &re))
	{
		if (i == 2)
		{
			free_heap_env(d);
			free_command_list(c);
			exit(re % 256);
		}
		d->exit_status = 2;
		write(2, "exit: too many arguments\n", 26);
	}
	else
	{
		write(2, "exit: ", 7);
		write(2, c->cmd[1], ft_strlen(c->cmd[1]));
		write(2, ": numeric argument required\n", 29);
		free_heap_env(d);
		free_command_list(c);
		exit(2);
	}
}


int	my_exit(t_c *c, t_d *d, t_exe *e)
{
	int	i;

	if (c == NULL)
	{
		free_heap_env(d);
		exit(d->exit_status);
	}
	close_pipe(c, e);
	i = 0;
	while (c->cmd[i] != NULL)
		i++;
	if (i > 1)
		exit_with_arg(d, c, i);
	else
	{
		free_heap_env(d);
		free_command_list(c);
		exit(d->exit_status);
	}
	return (-1);
}
