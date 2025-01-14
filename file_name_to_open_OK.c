/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_name_to_open_OK.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:57:32 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:57:34 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*file_name_to_open(char *s, t_d *d)
{
	char	*re;
	int		n;
	char	*temp;

	if (s == NULL)
		return (NULL);
	if (*s == '/')
		return (ft_strdup(s));
	else
	{
		temp = get_pwd(d);
		if (temp == NULL)
			return (NULL);
		n = ft_strlen(temp) + ft_strlen(s) + 3;
		re = malloc (n);
		if (re == NULL)
			return (NULL);
		re[0] = 0;
		ft_strlcat(re, temp, n);
		ft_strlcat(re, "/", n);
		concat_path(re, s);
		free(temp);
	}
	return (re);
}

