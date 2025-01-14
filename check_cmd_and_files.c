#include "minishell.h"

int	check_cmd_ok(t_c *c, t_d *d)
{
	struct stat	path_stat;

	if (stat((c->cmd)[0], &path_stat) == -1)
		return (d->exit_status = 127, write(2, "command not found\n", 19), -1);
	if (S_ISDIR(path_stat.st_mode))
		return (d->exit_status = 126, write(2, "it is a directory\n", 19), -1);
	if (access((c->cmd)[0], X_OK) == -1)
		return (d->exit_status = 126, write(2, "Permission denied\n", 19), -1);
	return (1);
}
