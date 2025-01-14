/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <hwu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 08:18:09 by hwu               #+#    #+#             */
/*   Updated: 2024/08/26 13:29:39 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*null_check(char const *s1, char const *s2)
{
	if (s1 == NULL)
	{
		if (s2 == NULL)
			return (NULL);
		return (ft_strdup(s2));
	}
	return (ft_strdup(s1));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	char	*join;

	if (s1 == NULL || s2 == NULL)
		return (null_check(s1, s2));
	join = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (join == NULL)
		return (NULL);
	i = 0;
	while (*s1 != 0)
	{
		join[i] = *s1;
		i++;
		s1++;
	}
	while (*s2 != 0)
	{
		join[i] = *s2;
		i++;
		s2++;
	}
	join[i] = 0;
	return (join);
}
/*
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>



int	main(void)
{
	char	name[]="test.txt\0jjjs";
	char	is[] = " word";
	char	*join;
	size_t	i;
	size_t	j;

	i = ft_strlen(name) + ft_strlen(is);
	printf("length of joined string should be %zu\n", i);
	join = ft_strjoin(name, is);
	for (j=0;j<i;j++)
	{
		printf("%c", join[j]);
	}
	free(join);
}
*/
