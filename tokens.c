#define ARG 0
#define CMD 1
#define RED 2
#define PIP 3
#define ENV 4

typedef struct	t_list 
{
	char *token;
	int type;
	struct t_list *prev;
	struct t_list *next;
}		t_list;

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 && *s2) && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

size_t	ft_strlen(const char *str)
{
	int	count;

	count = 0;
	while (*str != '\0')
	{
		count++;
		str++;
	}
	return (count);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*temp_dst;
	const char	*temp_src;

	if ((dst == src) || n == 0)
		return (dst);
	if (!dst && !src)
		return (0);
	temp_dst = (char *)dst;
	temp_src = (const char *)src;
	while (n--)
		temp_dst[n] = temp_src[n];
	return (dst);
}

char	*ft_strdup(const char *s1)
{
	char	*rtn;
	size_t	len;

	len = ft_strlen(s1) + 1;
	rtn = malloc(sizeof(char) * len);
	if (rtn == NULL)
		return (NULL);
	rtn = ft_memcpy(rtn, s1, len);
	return (rtn);
}

t_list *create_node(const char *token, int type)
{
	t_list *new_node = (t_list*)malloc(sizeof(t_list));
	if (!new_node)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	new_node->token = ft_strdup(token);
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void append_node(t_list **head, const char *token, int type)
{
	t_list *new_node;
	t_list *current;

	new_node = create_node(token, type);
	if (*head == NULL)
	{
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
}

int ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

int	is_command(const char *token)
{
	char *cmd[7];
	int		i;

	cmd[0] = "echo";
	cmd[1] = "cd";
	cmd[2] = "pwd";
	cmd[3] = "export";
	cmd[4] = "unset";
	cmd[5] = "env";
	cmd[6] = "exit";
	i = 0;
	while (i < 7)
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

void tokenize_and_classify(const char *input, t_list **head)
{
    int i = 0, j = 0;
    char token[1024] = {0};
    char buffer[1024] = {0};
    int buf_index = 0;
    char quote_char = '\0';
    int in_quotes = 0;
    int in_single_quotes = 0;

    while (input[i] != '\0')
    {
        // Skip leading spaces
        while (input[i] != '\0' && ft_is_space(input[i]))
        {
            i++;
        }

        if (input[i] == '\'' || input[i] == '"')
        {
            // Handle quotes
            quote_char = input[i++];
            in_quotes = 1;
            in_single_quotes = (quote_char == '\'');
            j = 0;

            while (input[i] != '\0')
            {
                if (input[i] == quote_char)
                {
                    i++;
                    in_quotes = 0;
                    break;
                }
                else if (input[i] == '$' && !in_single_quotes)
                {
                    if (j > 0)
                    {
                        token[j] = '\0';
                        strcat(buffer, token);
                        buf_index += j;
                        j = 0;
                    }
                    token[j++] = input[i++];
                    while (input[i] != '\0' && !ft_is_space(input[i]) && input[i] != quote_char && input[i] != '$')
                    {
                        token[j++] = input[i++];
                    }
                    token[j] = '\0';
                    strcat(buffer, token);
                    buf_index += strlen(token);
                    j = 0;
                    continue;
                }
                else
                {
                    token[j++] = input[i++];
                }
            }
            if (j > 0)
            {
                token[j] = '\0';
                strcat(buffer, token);
                buf_index += j;
            }
            if (buf_index > 0)
            {
                buffer[buf_index] = '\0';
                set_type(head, buffer);
                buf_index = 0;
                buffer[0] = '\0';
            }
        }
        else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
        {
            // Handle operators
            if (buf_index > 0)
            {
                buffer[buf_index] = '\0';
                set_type(head, buffer);
                buf_index = 0;
                buffer[0] = '\0';
            }
            j = 0;
            token[j++] = input[i++];
            if ((token[0] == '>' || token[0] == '<') && input[i] == token[0])
                token[j++] = input[i++];
            token[j] = '\0';
            set_type(head, token);
        }
        else
        {
            // Handle unquoted text and variables
            j = 0;
            while (input[i] != '\0' && !ft_is_space(input[i])
                && input[i] != '\'' && input[i] != '"' && input[i] != '|' && input[i] != '<' && input[i] != '>')
            {
                if (input[i] == '$')
                {
                    if (j > 0)
                    {
                        token[j] = '\0';
                        strcat(buffer, token);
                        buf_index += j;
                        j = 0;
                    }
                    token[j++] = input[i++];
                    while (input[i] != '\0' && !ft_is_space(input[i]) && input[i] != '\'' && input[i] != '"'
                        && input[i] != '|' && input[i] != '<' && input[i] != '>')
                    {
                        token[j++] = input[i++];
                    }
                    token[j] = '\0';
                    if (in_quotes && quote_char == '"')
                    {
                        strcat(buffer, "$");
                        strcat(buffer, token);
                        buf_index += strlen("$") + strlen(token);
                    }
                    else
                    {
                        set_type(head, token);
                    }
                    j = 0;
                    continue;
                }
                token[j++] = input[i++];
            }
            if (j > 0)
            {
                token[j] = '\0';
                strcat(buffer, token);
                buf_index += j;
            }
            if (buf_index > 0)
            {
                buffer[buf_index] = '\0';
                set_type(head, buffer);
                buf_index = 0;
                buffer[0] = '\0';
            }
        }
    }
    if (buf_index > 0)
    {
        buffer[buf_index] = '\0';
        set_type(head, buffer);
    }
}

void print_list(t_list *head)
{
    t_list *current = head;
    while (current != NULL)
    {
        printf("Token: %s, Type: %d\n", current->token, current->type);
        current = current->next;
    }
}

void free_list(t_list *head)
{
    t_list *tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp->token);
        free(tmp);
    }
}

int main(void)
{
    t_list *head = NULL;
    const char *input = "cd /home/user \"$HOME\"    | echo  val= ' $HOME' | env m=abc";

    tokenize_and_classify(input, &head);
    print_list(head);
	free_list(head);
    return 0;
}
