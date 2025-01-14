/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_03.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:10:55 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:51:58 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heap_env(t_d *d)
{
	int		i;
	char	**key;
	char	**nvs;

	i = 0;
	key = *(d->env);
	while (*key)
	{
		i++;
		key++;
	}
	nvs = malloc(sizeof(char *) * (i + 1));
	if (nvs == NULL)
		return (0);
	nvs[i] = NULL;
	key = *(d->env);
	*(d->env) = nvs;
	while (*key)
		*nvs++ = ft_strdup(*key++);
	return (1);
}

