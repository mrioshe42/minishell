/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   function_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:58:12 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:58:14 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strspn(const char *str1, const char *str2)
{
	size_t	count;
	int		i;
	int		j;
	int		found;

	count = 0;
	i = 0;
	while (str1[i] != '\0')
	{
		found = 0;
		j = 0;
		while (str2[j] != '\0' && str1[i] != str2[j])
		{
			j++;
		}
		if (str2[j] != 0)
			found = 1;
		if (!found)
			break ;
		count++;
		i++;
	}
	return (count);
}

//mem checked
//non: is -n on of off
int	my_echo(t_c *c)
{
	char	**ss;
	int		non;

	non = 0;
	ss = c->cmd + 1;
	while (*ss && !ft_strncmp(*ss, "-n", 2))
	{
		if ((*ss)[2] == '\0' || ((*ss)[2] == 'n'
			&& ft_strspn(*ss + 2, "n") == ft_strlen(*ss + 2)))
		{
			non = 1;
			ss++;
		}
		else
			break ;
	}
	while (*ss)
	{
		printf("%s", *ss++);
		if (*ss != NULL)
			printf(" ");
	}
	if (!non)
		printf("\n");
	return (1);
}

char	**find_same_name_env_var(char **env, char *key)
{
	char	*temp;

	temp = NULL;
	while (*env)
	{
		temp = get_env_var_name(*env);
		if (dif_str(key, temp))
		{
			env++;
			free(temp);
			temp = NULL;
		}
		else
			break ;
	}
	if (temp != NULL)
		free(temp);
	return (env);
}

//mem checked
char	*get_env_value(t_d *d, char *key)
{
	char	**env;
	char	*re;

	//printf("get_env_value(): start, key is %s\n", key);//debug
	if (key == NULL)
		return (NULL);
	env = *(d->env);
	//printf("get_env_value()a: %s\n", *env);//debug
	env = find_same_name_env_var(env, key);
	//printf("get_env_value()b: %s\n", *env);//debug
	if (*env == NULL)
		return (NULL);
	//printf("get_env_value(): env is %s\n", *env);//debug
	re = *env;
	while (*re != '=' && *re != 0)
		re++;
	if (*re == 0)
		return (NULL);
	re++;
	return (ft_strdup(re));
}

//mem checked
int	my_pwd(t_d *d)
{
	char	temp[MAXMM];

	(void)d;
	if (getcwd(temp, MAXMM))
		printf("%s\n", temp);
	else
		write(2, "getcwd() error", 15);
	return (1);
}
