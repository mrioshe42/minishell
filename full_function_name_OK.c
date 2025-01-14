/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_function_name_OK.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:57:55 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:57:57 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_name_in_folder(char *folder, char *s)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*re;
	char			*temp;

	dir = opendir(folder);
	if (dir == NULL)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strlen(entry->d_name) == ft_strlen(s)
			&& !ft_strncmp(entry->d_name, s, ft_strlen(s)))
		{
			re = ft_strjoin(folder, "/");
			temp = re;
			re = ft_strjoin(re, s);
			free(temp);
			closedir(dir);
			return (re);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (NULL);
}

//this funciton is used to free string list and pointer
//to the address of string list pointers
void	free_string_list(char **s)
{
	char	**temp;

	if (s == NULL)
		return ;
	temp = s;
	while (*s)
	{
		free(*s);
		s++;
	}
	free(temp);
}

char	*find_in_path(char *s, t_d *d)
{
	char	**fds;
	char	**temp;
	char	*full_name;
	char	*temps;

	full_name = NULL;
	temps = get_env_value(d, "PATH");
	if (temps == NULL)
		return (NULL);
	fds = ft_split(temps, ':');
	free(temps);
	temp = fds;
	while (*fds)
	{
		//printf("find_in_path(): searchi folder %s\n", *fds);//debug
		full_name = find_name_in_folder(*fds, s);
		if (full_name)
			break ;
		fds++;
	}
	free_string_list(temp);
	return (full_name);
}

char	*file_in_current_folder(char *s, t_d *d)
{
	int		n;
	char	*re;
	char	*temp;

	temp = get_pwd(d);
	if (temp == NULL)
		return (NULL);
	n = ft_strlen(temp) + ft_strlen(s) + 3;
	free(temp);
	re = malloc (n);
	if (re == NULL)
		return (NULL);
	re[0] = 0;
	temp = get_pwd(d);
	if (temp == NULL)
		return (NULL);
	ft_strlcat(re, temp, n);
	free(temp);
	ft_strlcat(re, "/", n);
	concat_path(re, s);
	return (re);
}

char	*full_function_name(char *s, t_d *d)
{
	char	*re;
	char	*temp;

	re = NULL;
	if (s == NULL)
		return (ft_strdup(""));
	if (*s == '/')
		return (ft_strdup(s));
	else if (ft_strchr(s, '/'))
		re = file_in_current_folder(s, d);
	else
	{
		temp = get_env_value(d, "PATH");
		if (temp == NULL)
		{
			//write(2, "PATH is not set!\n", 18);
			return (ft_strdup(""));
		}
		re = find_in_path(s, d);
		free(temp);
		if (re == NULL)
			return (ft_strdup(""));
	}
	return (re);
}
