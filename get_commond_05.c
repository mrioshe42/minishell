/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commond_05.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:10:11 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:10:14 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	check_core_dump(t_d *d)
{
//    printf("check_core_dump(): raw_exit_received is:%d\n", raw_exit_received);//debug
	if (WIFSIGNALED(d->raw_exit_received))
	{
        if (WCOREDUMP(d->raw_exit_received))
            printf("Quit (core dumped)\n");
    }
}

char	*get_command(t_d *d)
{
	char	*input;
	char	*cmd;
	t_st	st;
	int		n;

	init_status(&st);
	//check_core_dump(d);
	input = readline("$> ");
	if (input == NULL)
		return (NULL);
	cmd = skip_head_white_space(input);
	n = parse_line(cmd, &st);
	while (n != 1)
	{
		if (n == -1 || n == -2)
			return (get_command_restart(cmd, n, d));
		input = readline("> ");
		cmd = connect_cmd_line(cmd, input);
		unclosed_eofs(-2, NULL);
		init_status(&st);
		n = parse_line(cmd, &st);
	}
	return (cmd);
}
