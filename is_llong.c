/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_int.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:04:07 by hwu               #+#    #+#             */
/*   Updated: 2024/07/04 11:04:19 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <limits.h>

static int	is_ok(int sign, long long re, int b)
{
	if (sign == -1)
		if (LLONG_MIN / 10 <= re)
			if (LLONG_MIN - re * 10 <= sign * b)
				return (1);
	if (sign == 1)
		if (LLONG_MAX / 10 >= re)
			if (LLONG_MAX - re * 10 >= b)
				return (1);
	return (0);
}

int	is_llong(char *s, long long *re)
{
	int	sign;

	sign = 1;
	if (*s == '-')
		sign = -1;
	if (*s == '-' || *s == '+')
		s++;
	*re = 0;
	if (!(*s >= '0' && *s <= '9'))
		return (0);
	while (*s != 0)
	{
		if (*s > '9' || *s < '0')
			return (0);
		if (!is_ok(sign, *re, *s - '0'))
			return (0);
		*re = *re * 10 + sign * (*s - '0');
		s++;
	}
	return (1);
}
