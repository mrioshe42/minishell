#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARG 0
#define CMD 1
#define RED 2
#define PIP 3
#define ENV 4

typedef	struct t_list {
	char			*token;
	int				type;
	struct t_list	*prev;
	struct t_list	*next;
}	t_list;

typedef	struct {
	char	*input;
	t_list	**head;
	int		*j;
	char	*token;
}	DollarSignParams;

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 && *s2) && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

size_t ft_strlen(const char *str)
{
	int count;

	count = 0;
	while (*str != '\0')
	{
		count++;
		str++;
	}
	return count;
}

int ft_isalnum(int c)
{
	return (isalpha(c) || isdigit(c));
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
	char *temp_dst = (char *)dst;
	const char *temp_src = (const char *)src;

	if ((dst == src) || n == 0)
		return dst;
	if (!dst && !src)
		return 0;
	while (n--)
		temp_dst[n] = temp_src[n];
	return dst;
}

char *ft_strdup(const char *s1)
{
	size_t	len;
	char	*rtn;

	len = ft_strlen(s1) + 1;
	rtn = malloc(sizeof(char) * len);
	if (rtn == NULL)
		return NULL;
	rtn = ft_memcpy(rtn, s1, len);
	return rtn;
}

t_list *create_node(const char *token, int type)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node) {
		printf("Memory allocation error\n");
		exit(1);
	}
	new_node->token = ft_strdup(token);
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}

void append_node(t_list **head, const char *token, int type)
{
	t_list	*new_node;
	t_list	*current;

	new_node = create_node(token, type);
	if (*head == NULL) {
		*head = new_node;
	} else {
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
}

int ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\f' || c == '\v');
}

int	is_command(const char *token)
{
	char	*cmd[10];
	int		i;

	cmd[0] = "echo";
	cmd[1] = "cd";
	cmd[2] = "pwd";
	cmd[3] = "export";
	cmd[4] = "unset";
	cmd[5] = "env";
	cmd[6] = "exit";
	cmd[7] = "ls";
	cmd[8] = "grep";
	cmd[9] = "cat";
	i = 0;
	while (i < 10)
	{
		if (ft_strcmp(cmd[i], token) == 0)
			return (1);
		i++;
	}
	return (0);
}

void set_type(t_list **head, const char *token)
{
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		append_node(head, token, RED);
	else if (ft_strcmp(token, "|") == 0)
		append_node(head, token, PIP);
	else if (token[0] == '$')
		append_node(head, token, ENV);
	else if (is_command(token))
		append_node(head, token, CMD);
	else
		append_node(head, token, ARG);
}

void add_token(t_list **head, char *token, int *j)
{
	token[*j] = '\0';
	set_type(head, token);
	*j = 0;
}

int is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int process_word(char *input, t_list **head, int start)
{
	char	token[1024];
	int		i;
	int		j;

	i = start;
	j = 0;
	while (input[i] != '\0' && !ft_is_space(input[i]) &&
			input[i] != '\'' && input[i] != '"' &&
			!is_operator(input[i]))
	{
		if (input[i] == '$')
		{
			if (j > 0)
				add_token(head, token, &j);
			token[j++] = input[i++];
			while (isalnum(input[i]) || input[i] == '_')
				token[j++] = input[i++];
			add_token(head, token, &j);
		} else
			token[j++] = input[i++];
	}
	if (j > 0)
		add_token(head, token, &j);
	return (i - start);
}

int	process_operator(char *input, t_list **head, int start)
{
	char	token[3];
	int		i;
	int		j;

	i = start;
	j = 0;
	token[j++] = input[i++];
	if ((token[0] == '>' || token[0] == '<') && input[i] == token[0])
		token[j++] = input[i++];
	token[j] = '\0';
	set_type(head, token);
	return (i - start);
}

int	handle_dollar_sign(DollarSignParams params, int *i, char quote_char)
{
	int	j;

	if (params.input[*i] == '$' && quote_char == '"')
	{
		if (*(params.j) > 0)
		{
			params.token[*(params.j)] = '\0';
			add_token(params.head, params.token, params.j);
		}
		params.token[0] = params.input[(*i)++];
		j = 1;
		while (isalnum(params.input[*i]) || params.input[*i] == '_')
			params.token[j++] = params.input[(*i)++];
		params.token[j] = '\0';
		add_token(params.head, params.token, &j);
		*(params.j) = 0;
		return (1);
	}
	return (0);
}


int process_quotes(char *input, t_list **head, int start)
{
	char quote_char;
	int i;
	char token[1024];
	int j;

	j = 0;
	i = start;
	quote_char = input[i++];
	while (input[i] != '\0')
	{
		if (input[i] == quote_char)
		{
			i++;
			break ;
		}
		if (handle_dollar_sign((DollarSignParams){input, head, &j, token},
			&i, quote_char))
			continue ;
		else
			token[j++] = input[i++];
	}
	if (j > 0)
		add_token(head, token, &j);
	return (i - start);
}

void tokenize_and_classify(char *input, t_list **head)
{
	int i;

	i = 0;
	while (input[i] != '\0')
	{
		while (ft_is_space(input[i]))
			i++;
		if (input[i] != '\0')
		{
			if (input[i] == '\'' || input[i] == '"')
				i += process_quotes(input, head, i);
			else if (is_operator(input[i]))
				i += process_operator(input, head, i);
			else
				i += process_word(input, head, i);
		}
	}
}

void	print_list(t_list *head)
{
	t_list	*current;

	current = head;
	while (current != NULL) {
		printf("Token: %s, Type: %d\n", current->token, current->type);
		current = current->next;
	}
}

void	free_list(t_list *head)
{
	t_list	*tmp;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->token);
		free(tmp);
	}
}

int main(void) {
	t_list *head = NULL;
	// char *input = "cd /home/user \"$HOME\"    | echo  val= '$HOME' | env m=abc";
	char *input = "echo \"Hello, $USER!\" > output.txt | cat output.txt >> log.txt | echo 'This is a test' | grep test | export VAR=123 | echo '$VAR' <<EOF\nLine1\nLine2\nEOF";

	tokenize_and_classify(input, &head);
	print_list(head);
	free_list(head);
	return 0;
}
