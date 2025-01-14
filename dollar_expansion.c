/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:55:27 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:55:30 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dollar_exp_process_no(t_a *a, char new[MAXMM][MAXMM])
{
	if (*(a->s) == '*')
	{
		(a->star_on)[0]++;
		if (a->st.son || a->st.don)
			(a->star_on)[(a->star_on)[0]] = 0;
		else
			(a->star_on)[(a->star_on)[0]] = 1;
	}
	new[a->i][(a->j)++] = *(a->s);
}

void	dollar_exp_process(t_a *a, t_d *d, char new[MAXMM][MAXMM])
{
	/*if (white_space((a->s)[1]) || (a->s)[1] == 0)
	{
		a->temp = ft_strdup("$");
		(a->s) += 1;
	}*/
	if ((a->s)[1] == '?')
	{
		a->temp = ft_itoa(d->exit_status);
		(a->s) += 2;
	}
	else
	{
		a->temp2 = get_env_name((a->s) + 1);
		//printf("env var name of %s is %s\n", a->s, a->temp2); //debug
		a->env_len = ft_strlen(a->temp2);
		if (a->temp2[0] == 0)
			a->temp = ft_strdup("$");
		else
			a->temp = get_env_value(d, a->temp2);
		free(a->temp2);
	}
	if (a->temp)
		process_dsubstitution(new, a);
	(a->s) += a->env_len;
}

//to set the star_on off for * in cmd not in arg
void	protect_cmd(char new[MAXMM][MAXMM], t_a *a)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (new[0][j])
	{
		if (new[0][j] == '*')
		{
			i++;
			a->star_on[i] = 0;
		}
		j++;
	}
}

//st_on is used to keep track of if a star* is a wildcard char or not
//  st_on[0] keep the number of star* found in the arg
//this funciton expand c->cmd[argi] into new[][]
int	dollar_expand_dequotation(t_c *c, int argi, t_d *d, char new[MAXMM][MAXMM])
{
	t_a	a;

	init_struct_a(&a, c->cmd[argi]);
	while (*(a.s))
	{
		if (*(a.s) == 34 && !a.st.son)
			a.st.don = 1 - a.st.don;
		else if (*(a.s) == 39 && !a.st.don)
			a.st.son = 1 - a.st.son;
		else if (*(a.s) == '$' && !a.st.son)
			dollar_exp_process(&a, d, new);
		else
			dollar_exp_process_no(&a, new);
		(a.s)++;
	}
	new[a.i][a.j] = 0;
	if (argi == 0)
		protect_cmd(new, &a);
	if (have_wildcard(a.star_on))
		star_expansion(d, new, a.star_on);
	return (count_string(new));
}

int	dollar_expand_dequo_io_file(t_c *io, t_d *d, char new[MAXMM][MAXMM])
{
	t_a		a;
	char	*s;

	if (io->tp == 2)
		s = io->iofile;
	else
		s = io->iofile;
	init_struct_a(&a, s);
	while (*(a.s))
	{
		if (*(a.s) == 34 && !a.st.son)
			a.st.don = 1 - a.st.don;
		else if (*(a.s) == 39 && !a.st.don)
			a.st.son = 1 - a.st.son;
		else if (*(a.s) == '$' && !a.st.son)
			dollar_exp_process(&a, d, new);
		else
			dollar_exp_process_no(&a, new);
		(a.s)++;
	}
	new[a.i][a.j] = 0;
	if (have_wildcard(a.star_on))
		star_expansion(d, new, a.star_on);
	return (count_string(new));
}
