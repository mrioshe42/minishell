#include "minishell.h"

char	*name_of_env_var(char *s)
{
    int i;

    i = 0;
    while(s[i] && s[i] != '=')
        i++;
    return (ft_strsub(s, 0, i));
}

char	*value_of_env_var(char *s)
{
    while (*s && *s != '=')
        s++;
    if (*s == '=')
    {
        s++;
        if (*s == 0)
            return (NULL);
        else
            return (ft_strdup(s));
    }
    else
        return (NULL);
}

// to print all envs no matter env is with value or not
int	export_NULL(t_d *d)
{
	char	**ent;
    char    *name;
    char    *value;

	ent = *(d->env);
	while (*ent != NULL)
    {
		name = name_of_env_var(*ent);
        value = value_of_env_var(*ent);
        printf("declare -x %s", name);
        if (ft_strchr(*ent, '='))
        {
            printf("=\"");
            if (value)
                printf("%s", value);
            printf("\"");
        }
        printf("\n");
        free(name);
        free(value);
        ent++;
    }
	return (1);
}