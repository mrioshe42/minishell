/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_02.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrios-he <mrios-he@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:56:50 by hwu               #+#    #+#             */
/*   Updated: 2024/11/28 14:56:25 by mrios-he         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	call_builtin(int i, t_d *d, t_c *c)
{
	int	exit_s;

	exit_s = 999;
	if (i == 1)
		exit_s = my_echo(c);
	else if (i == 2)
		exit_s = my_cd(d, c);
	else if (i == 3)
		exit_s = my_pwd(d);
	else if (i == 4)
		exit_s = export_env(d, c);
	else if (i == 5)
		exit_s = unset_env(d, c);
	else if (i == 6)
		exit_s = prt_env(d);
	if (exit_s != 1)
		d->exit_status = 1;
	else
		d->exit_status = 0;
}

char	*add_temp_file(char *eof, char *eofs[MAXMM])
{
	int	i;

	i = 0;
	while (i < MAXMM)
	{
		if (eofs[i])
			i++;
		else
			break ;
	}
	if (i == MAXMM)
	{
		write(2, "two many unclosed syntex\n", 26);
		return (NULL);
	}
	else
	{
		eofs[i] = eof;
		return (eof);
	}
}

// add = 1 for add, -1 for remove first, 0 for check first,
// return
char	*temp_file_keeper(int add, char *eof, char *eofs[MAXMM])
{
	int	i;

	if (add == 1)
		return (add_temp_file(eof, eofs));
	else if (add == 0)
	{
		i = 0;
		while (i < MAXMM)
		{
			if (eofs[i] != NULL)
			{
				unlink(eofs[i]);
				free(eofs[i]);
			}
			i++;
		}
		return (NULL);
	}
	return (NULL);
}

// i: 1 for new name, 0 for remove all;
char	*get_temp_file(int i)
{
	static int	num;
	char		*re;
	char		*cnum;
	static char	*eofs[MAXMM];

	if (i == 1)
	{
		cnum = ft_itoa(num);
		re = ft_strjoin("/temp", cnum);
		free(cnum);
		temp_file_keeper(1, re, eofs);
		num++;
		return (re);
	}
	if (i == 0)
	{
		temp_file_keeper(0, NULL, eofs);
		return (NULL);
	}
	return (NULL);
}
