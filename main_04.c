/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_04.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:44:42 by hwu               #+#    #+#             */
/*   Updated: 2024/09/09 18:44:49 by hwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_input_loop(char *input, t_d *data)
{
	t_list	*tokens;
	t_c		*cmd;

	(void)data;
	if (trimmed(input))
		add_history(trimmed(input));
	tokens = tokenize_input(trimmed(input));
	//print_token_list(tokens);//debug
	free(input);
	cmd = load_token_into_cmd(tokens);
	free_token_list(tokens);
	//print_cmd_list("process_input_loop():\n", cmd);//debug
	cmd = treat_cmdline_for_pipe(cmd);
	//print_cmd_list("process_input_loop():after treat for pipe\n", cmd);//debug
	if (cmd)
	{
		execute_command_line(cmd, data);
		free_command_list(cmd);
	}
	//usleep(10000);
	input = get_command(data);
	return (input);
}

void	init_data_d(t_d *d)
{
	d->exit_status = 0;
	d->raw_exit_received = 0;
	d->used = 0;
	d->s = NULL;
	d->syn = 0;
	d->invalid_command = 0;
}

void	signal_handle(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		if (in_subshell)
			return;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_d		data;

	signal(SIGINT, signal_handle);
	signal(SIGQUIT, SIG_IGN);
	if (argc > 0)
		argv[0] = argv[0];
	data.env = &envp;
	init_data_d(&data);
	if (!heap_env(&data))
		exit(1);
	input = get_command(&data);
	while (input)
		input = process_input_loop(input, &data);
	my_exit(NULL, &data, NULL);
}
