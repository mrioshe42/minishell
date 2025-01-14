/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrios-he <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:05:19 by mrios-he          #+#    #+#             */
/*   Updated: 2024/08/28 14:05:22 by mrios-he         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <sys/stat.h>
# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <termios.h>
# include <fcntl.h>
# include <sys/select.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>

# define ARGU 2
# include <stddef.h>

# define CMD  0
# define ARG  1
# define IFIL 2
# define HDOC 3
# define OUT1 4
# define OUT2 5
# define PIP  6
# define OR   7
# define AND  8
# define PAR1 9
# define PAR2 10
# define MA 1024

extern int in_subshell;

typedef struct t_list
{
	char			*token;
	int				type;
	struct t_list	*prev;
	struct t_list	*next;
}	t_list;

typedef struct s_indices
{
	int	m;
	int	n;
	int	i;
}	t_indices;

/*spec 01 start*/
// here we deal with only <, <<, >, >>, |, ||, &&, ()
// <if not in quotatin>: |, ||, &&, (, ), marks the end of a command
// <if not in quotatin>: |, ||, &&, (, ), white_spaces marks the end
// of a argument
// <if not in single quotatin>:$ marks a env var; ::to expand
// <if not in quotatin && not in type 'command'>: marks a wildcard char;
//::to expand

// type: 
// ***************************** member of command
// command:  0  *s      1
// argument: 1  *s      0-n
// ifile:    2  *s      0-n       <
// heredoc:  3  *s      0-n       <<EOF
// overwrite:4  *s      0,1       >
// append:   5  *s      0,1       >>
// ***************************** logical operators
// pipe:     6  null    |
// or:       7  null    ||
// and:      8  null    &&
// (:        9  null    (
// ):        10 null    )
/*spec 01 end*/

//list of command prepared for execution:
//id: should start from 3, corresponding the pipe it is using, as output type
//number < 3 are reserved for other output type.
//tp(type) as in spec 01;
//cmd: command + args, first is executable, others are expended string
// as arguments
//cmd is a pointer to area of pointer which point to strings, the NULL indicate
// the end of this area
//pipe_on: is there a pipe directed to STDIN_FILENO, when pre->type is 6, 
//pipe_on should be 1
//heredoc: expaned string as stdin input when is executable is running
//ifile: input file as stdin input when is executable is running
//otype: output type, -1 for write to file; -2 for append to file;
// 1 for stdout; other as pipe num;
//ofile: output file name, NULL when otype > 0;
//out: is link list of output ways, including > and >>; 
//in: is link list of input way, including < and <<;
//in the head of a command node heredoc and ifile, and ofile should be NULL,
// if there is in/out,
//  it should be listed in ->in/->out, ->tp shows which type it is < << > >>
//all output node will be defined as type 4
typedef struct s_command_node
{
	int						id;
	int						tp;
	char					**cmd;
	int						pipe_on;
	char					*iofile;
	int						otype;
	struct s_command_node	*next;
	struct s_command_node	*pre;
	struct s_command_node	*io;
	struct s_command_node	*temp;
}	t_c;

// son: ' is on, don " is on, pon ( is on, full_line parsing a full_line?
// oon: || is on, aon: && is on, von: | is on vertical-bar
//cmdl: is parsing commond line, 1 for parsing command line, 0 for parsing
// heredoc content
typedef struct s_status
{
	int		cmdl;
	int		son;
	int		don;
	int		pon;
	int		oon;
	int		aon;
	int		von;
	int		fll;
	int		n;
	char	*s;
}	t_st;

typedef struct s_data
{
	char	***env;
	char	*s;
	int		syn;
	int		used;
	int		exit_status;
	int		raw_exit_received;
	int		invalid_command;
}	t_d;

typedef struct s_string_list
{
	struct s_string_list	*next;
	char					*s;
}	t_s;

//str: 0 if not a literal string, 1 literal string
typedef struct s_token_node
{
	int					str;
	int					ttp;
	char				*s;
	struct s_token_node	*next;
}	t_t;

//slevel, the start pid index of the current command block
//ilevel, the current pid index at current step, when encounter ')',
//  the pid[slevel + 1 to ilevel] should be waited
//  if the i level = slevel, then no fork() need to be waited when encouter ')'
// i means ith fork(), in current process
typedef struct s_status_of_current_execution
{
	int	slevel;
	int	ilevel;
	int	pipeid[MAXMM][2];
	int	pid[MAXMM];
	int	exits;
	int	i;
}	t_exe;

typedef struct indexes
{
	int	i;
	int	m;
	int	n;
}	t_i;

typedef struct s_my_data
{
	t_st	st;
	int		star_on[MAXMM];
	int		i;
	int		j;
	char	*temp;
	char	*temp2;
	char	*s;
	int		env_len;
}	t_a;

char	*get_env_value(t_d *d, char *key);;
void	print_new(char new[MAXMM][MAXMM]);
char	*get_env_name(char *s);
void	process_dsub_when_no_quote_on(char new[MAXMM][MAXMM], t_a *a);
void	process_dsubstitution(char new[MAXMM][MAXMM], t_a *a);
int		count_string(char new[MAXMM][MAXMM]);
void	print_star_on(int st_on[MAXMM]);
int		have_wildcard(int *st_on);
void	init_struct_a(t_a *a, char *arg);
void	dollar_exp_process_no_d(t_a *a, char new[MAXMM][MAXMM]);
void	dollar_exp_processd(t_a *a, t_d *d, char new[MAXMM][MAXMM]);
void	protect_cmd(char new[MAXMM][MAXMM], t_a *a);
int		dollar_expand_dequotation(t_c *c, int argi, t_d *d, char new[MA][MA]);
int		insert_new_arg(t_c *c, int i, int num, char new[MAXMM][MAXMM]);
int		process_one_arg(t_c *c, t_d *d, int i);
int		dollar_expand_dequo_io_file(t_c *io, t_d *d, char new[MAXMM][MAXMM]);
int		convert_file_name(t_c *io, t_d *d);
int		convert_redirection_2_string(t_c *c, t_d *d);
int		convert_cmd_2_string(t_c *c, t_d *d);
int		convert_arg_2_string(t_c *c, t_d *d);
int		is_builtin(char **s);
void	free_heap_env(t_d *d);
int		is_number(char *str);
void	exit_with_arg(t_d *d, t_c *c, int i);
int		my_exit(t_c *c, t_d *d, t_exe *e);
void	call_builtin(int i, t_d *d, t_c *c);
char	*add_temp_file(char *eof, char *eofs[MAXMM]);
char	*temp_file_keeper(int add, char *eof, char *eofs[MAXMM]);
char	*get_temp_file(int i);
int		set_heredoc_input(int *last_in, t_c *inlink);
int		set_input(t_c *c, t_d *d, t_exe *e);
int		set_output(t_c *c, t_d *d, t_exe *e);
int		setup_io(t_c *c, t_d *d, t_exe *e);
void	close_pipe(t_c *c, t_exe *e);
int		next_logic(t_c *c);
void	do_in_mainshell(t_c *c, t_d *d, t_exe *e);
void	print_newline(int sig);
void	do_in_subshell(t_c *c, t_d *d, t_exe *e);
int		check_cmd_ok(t_c *c, t_d *d);
int		execute_cmd(t_c *c, t_d *d, t_exe *e);
int		open_file(t_c *outlink, t_d *d);
int		execute_output_node(t_c *c, t_d *d, t_exe *e);
int		execute_inbuilt(t_c *c, t_d *d, t_exe *e);
int		execute_one_cmd(t_c *c, t_d *d, t_exe *e);
int		count_command_nodes(t_c *c);
int		init_exe(t_c *c, t_exe *e);
t_c		*skip_next_group(t_c *c);
void	wait_cocurrent(t_c *c, t_d *d, t_exe *e);
void	close_subshell_pipes(t_c *c, t_exe *e);
t_c		*execute_command_line4(t_c *c, t_d *d, t_exe *e);
t_c		*execute_command_line3(t_c *c, t_d *d, t_exe *e);
int		execute_command_line2(t_c *c, t_d *d, t_exe *e);
int		execute_command_line(t_c *c, t_d *d);
char	*file_name_to_open(char *s, t_d *d);
t_c		*find_upper_group_end(t_c *c);
t_c		*find_following_pipe(t_c *c);
int		find_group_pipe(t_c *c);
void	mark_read_from_pipe(t_c *c);
int		find_next_executable(t_c *c);
void	find_pipes(t_c *c);
char	*find_name_in_folder(char *folder, char *s);
void	free_string_list(char **s);
char	*find_in_path(char *s, t_d *d);
char	*file_in_current_folder(char *s, t_d *d);
char	*full_function_name(char *s, t_d *d);
size_t	ft_strspn(const char *str1, const char *str2);
int		my_echo(t_c *c);
char	**find_same_name_env_var(char **env, char *key);
char	*get_env_value(t_d *d, char *key);
int		my_pwd(t_d *d);
char	*get_pwd(t_d *d);
void	remove_back_slash(char *s);
void	process_star_for_path_concat2(char **s, char **i);
void	process_star_for_path_concat(char *dest, char **s, char **i);
void	concat_path(char *dest, char *s);
int		prt_env(t_d *d);
int		get_len(char *s);
int		add_node(t_s **list, char *s);
void	wfree_list(t_s **l);
t_s		*into_ss(char *s);
int		correct_char(int in, char c);
int		valid_env(char *s);
int		count_before_equal(char *s);
int		count_after_equal(char *s);
int		regularized_size(char *s);
void	load_env(char *s, char *t);
int		add_env(t_d *d, char *new);
char	*get_env_var_name(char *s);
char	**find_env_by_name(t_d *d, char *name);
void	remove_env(t_d *d, char *todelete);
int		unset_env(t_d *d, t_c *c);
int		export_one_env(t_d *d, char *s);
int		export_env(t_d *d, t_c *c);
char	*get_command(t_d *d);
char	*get_eof(char *s);
int		spcial_char(char c);
char	*get_eof(char *s);
void	print_eofs(void);
int		check_unclosed_eof(char *s);
void	init_status(t_st *s);
void	close_special_char(t_st *st);
char	next_char(char *s);
int		is_bad_output(char *s);
int		check_illegal_cmd2(char *s);
int		check_illegal_cmd(char *s);
int		register_eof(char *s);
int		process_logi_symbol2(char *s, t_st *st);
int		process_logi_symbol(char *s, t_st *st);
int		process_special_char(char *s, t_st *st);
int		go_through_chars_one_by_one(char *s, t_st *st);
int		parse_line(char *s, t_st *st);
char	*skip_head_white_space(char *s);
char	*connect_cmd_line(char *cmd, char *input);
void	printf_error(int n);
char	*get_command_restart(char *tofree, int errornom, t_d *d);
void	init_cmd_node(t_c *c);
void	clean_command_out(t_c *c);
void	clean_command_in(t_c *c);
t_c		*create_new_command_node(t_c *c);
void	load_output_to_independent_node(t_c *c, t_list **t, int id);
int		is_output(t_list *t);
t_c		*load_logic_sign(t_c *c, t_list **t);
char	**create_cmd_area(void);
t_c		*add_argument(t_c *c, t_list **t);
t_c		*add_redi_arg(t_c *c, t_list *t);
void	print_logi(int tp);
t_c		*for_add_redi(t_c *c, t_list **t);
t_c		*load_token_into_cmd3(t_c *c, t_list **t, int *id);
t_c		*do_when_logic_sign(t_c *c, t_list **t, int *id);
t_c		*load_token_into_cmd2(t_list *t, t_c *head);
t_c		*load_token_into_cmd(t_list *t);
void	print_cmd_list(char *ss, t_c *c);
size_t	max(size_t a, size_t b);
int		clean_exit(void);
char	*clean_all_eofs(char *eofs[MAXMM]);
char	*remove_one_eof(char *eofs[MAXMM]);
char	*add_eof(char *eofs[MAXMM], char *eof);
char	*unclosed_eofs(int add, char *eof);
void	printsini(char *s);
char	*trimmed(char *s);
int		dif_str(char *s, char *t);
int		heap_env(t_d *d);
void	free_command_list(t_c *cmd);
char	*process_input_loop(char *input, t_d *data);
void	init_data_d(t_d *d);
int		main(int argc, char **argv, char **envp);
int		reset_old_pwd(t_d *d);
int		back_to_old_pwd(t_d *d);
int		cd_home(t_d *d);
int		my_chdir(t_d *d, t_c *c);
int		my_cd(t_d *d, t_c *c);
char	*ft_strchr(const char *s, int c);
int		white_space(char c);
int		special_char(char c);
int		get_file_num(t_d *d);
char	**get_pwd_file_list(t_d *d);
void	init_indexex(t_i *i);
int		get_word(char *pattern, int *st_on, char word[MAXMM][MAXMM]);
int		match(char *s, char *pattern, int st_on[MAXMM]);
void	remove_file_name(char **f, int fn);
void	push_back(char new[MAXMM][MAXMM], int i, int re);
void	clean_char_table(char **f);
int		insert(char new[MAXMM][MAXMM], int i, char **f);
int		contain_active_star(char *s, int st_on[MAXMM]);
void	print_strings(char **s);
int		wild_star(t_d *d, char new[MAXMM][MAXMM], int st_on[MAXMM], int i);
int		count_star(char *s);
void	star_expansion(t_d *d, char new[MAXMM][MAXMM], int st_on[MAXMM]);
void	print_token_list(t_list *head);
void	free_token_list(t_list *head);
char	*ft_strsub(char *s, int start, int len);
int		add_new_token(t_list **l, char **s, int i, int type);
int		add_last(char **s, t_list **l, int i);
int		process_pipe(char **s, t_st *st, t_list **l, int i);
int		process_input(char **s, t_st *st, t_list **l, int i);
int		process_output(char **s, t_st *st, t_list **l, int i);
int		process_left_paren(char **s, t_st *st, t_list **l, int i);
int		process_right_paren(char **s, t_st *st, t_list **l, int i);
int		process_and(char **s, t_st *st, t_list **l, int i);
void	move_to_first_heredoc(t_list **l);
void	move_to_next_heredoc(t_list **l);
char	*get_next_line_from_string(char **s);
int		insert_arg(t_list **l, char *arg);
char	*get_one_heredoc(char **s, char *d);
int		load_heredoc(char **s, t_list **l);
int		find_new_line(char **s, t_st *st, t_list **l, int i);
int		find_next_token(char **s, t_st *st);
int		get_token_2(char **s, int i, t_st *st, t_list **l);
int		get_token(char **s, t_st *st, t_list **l);
void	init_token_head(t_list *re);
t_list	*tokenize_input(char *s);
int		valid_check_one_2(t_c *c);
int		valid_check_one(t_c *c);
int		valid_check(t_c *c);
void	print_error_next_char(char *s);
t_c		*treat_cmdline_for_pipe(t_c *c);
int		execute_pure_io_node(t_c *c, t_d *d, t_exe *e);
void	sort_strings(char **f, int n);
int		is_llong(char *s, long long *re);
char	*name_of_env_var(char *s);
char	*value_of_env_var(char *s);
int		export_NULL(t_d *d);
int		check_cmd_and_redirection_file(t_c *c, t_d *d, t_exe *e);
int		status_value_for_print(t_d *d);
int		parse_raw_exit_status(t_d *d);
void	check_core_dump(t_d *d);
int		check_file_ok(t_c *io);
#endif
