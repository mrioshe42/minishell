/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_arg_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:54:48 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:54:53 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_struct_a(t_a *a, char *arg)
{
	a->s = arg;
	a->i = 0;
	a->j = 0;
	(a->star_on)[0] = 0;
	init_status(&(a->st));
	a->env_len = -1;
	a->temp = NULL;
	a->temp2 = NULL;
}

//mem checked
int	insert_new_arg(t_c *c, int i, int num, char new[MAXMM][MAXMM])
{
	int	all;
	int	j;

	all = 0;
	while ((c->cmd)[all])
		all++;
	if (all + num > MAXMM)
		return(write(2, "insert_new_arg(): too many arguments\n", 38), -3);
	j = all - 1;
	while (j > i)
	{
		(c->cmd)[j + num - 1] = (c->cmd)[j];
		j--;
	}
	all = 0;
	free((c->cmd)[i]);
	(c->cmd)[i] = NULL;
	while (all < num)
	{
		(c->cmd)[i + all] = ft_strdup(new[all]);
		all++;
	}
	return (1);
}

int	pure_dollar_arg(char *s)
{
	while (*s)
	{
		if (*s != '$')
			return (0);
		s++;
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		while (ft_isalnum(*s) || *s == '_')
			s++;
		if (*s == 0)
			return (1);
	}
	return (0);
}
void	remove_cmd_i(t_c *c, int i)
{
	free((c->cmd)[i]);
	while ((c->cmd)[i])
	{
		(c->cmd[i] = c->cmd[i + 1]);
		i++;
	}
}
//new[][] is used to store the new string, for each cmd[i]
//, new[][] will be renewed
//  so, it only store expanded strings for one of cmd[i]
int	process_one_arg(t_c *c, t_d *d, int i)
{
	int		num;
	char	new[MAXMM][MAXMM];
	int		error_num;

	//printf("process_one_arg():start processing c->cmd[%d]:%s\n", i, c->cmd[i]);//debug
	num = 0;
	while (num < MAXMM)
		new[num++][0] = 0;

	num = dollar_expand_dequotation(c, i, d, new);
	//printf("process_one_arg():num is:%d\n", num);//debug
	//if (num == 0 && !pure_dollar_arg(c->cmd[i]))
	if (num == 0)
	{
		if (pure_dollar_arg((c->cmd)[i]))
			return (remove_cmd_i(c, i), i);
		free(c->cmd[i]);
		c->cmd[i] = ft_strdup("");
		//printf("%sxx\n", c->cmd[i]);
		return (i + 1);
	}
	else
	{
		error_num = insert_new_arg(c, i, num, new);
		if (error_num < 0)
			return (error_num);
		return (i + num);
	}

}
