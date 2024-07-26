#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG 0
#define CMD 1
#define TRUNC 2
#define APPEND 3
#define INPUT 4
#define PIPE 5
#define END 6
#define EMPTY 7

typedef struct t_list {
    char *token;
    int type;
    struct t_list *prev;
    struct t_list *next;
} t_list;

int ft_strcmp(const char *s1, const char *s2) {
    while ((*s1 && *s2) && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

size_t ft_strlen(const char *str) {
    size_t count = 0;
    while (*str++) {
        count++;
    }
    return count;
}

void *ft_memcpy(void *dst, const void *src, size_t n) {
    if (dst == src || n == 0) return dst;
    if (!dst && !src) return NULL;

    char *temp_dst = (char *)dst;
    const char *temp_src = (const char *)src;
    while (n--) {
        temp_dst[n] = temp_src[n];
    }
    return dst;
}

char *ft_strdup(const char *s1) {
    size_t len = ft_strlen(s1) + 1;
    char *rtn = malloc(len);
    if (!rtn) return NULL;
    return ft_memcpy(rtn, s1, len);
}

void type_arg(t_list *token, int separator) {
    if (ft_strcmp(token->token, "") == 0)
        token->type = EMPTY;
    else if (ft_strcmp(token->token, ">") == 0 && separator == 0)
        token->type = TRUNC;
    else if (ft_strcmp(token->token, ">>") == 0 && separator == 0)
        token->type = APPEND;
    else if (ft_strcmp(token->token, "<") == 0 && separator == 0)
        token->type = INPUT;
    else if (ft_strcmp(token->token, "|") == 0 && separator == 0)
        token->type = PIPE;
    else if (ft_strcmp(token->token, ";") == 0 && separator == 0)
        token->type = END;
    else if (token->prev == NULL || token->prev->type >= TRUNC)
        token->type = CMD;
    else
        token->type = ARG;
}

void ft_skip_space(const char *str, int *i) {
    while ((str[*i] == ' ' || str[*i] == '\t')
        || (str[*i] == '\r' || str[*i] == '\v' || str[*i] == '\f'))
        (*i)++;
}

int next_alloc(char *line, int *i) {
    int count = 0;
    int j = 0;
    char c = ' ';
    while (line[*i + j] && (line[*i + j] != ' ' || c != ' ')) {
        if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
            c = line[*i + j++];
        else if (c != ' ' && line[*i + j] == c) {
            count += 2;
            c = ' ';
            j++;
        } else
            j++;
        if (line[*i + j - 1] == '\\')
            count--;
    }
    return (j - count + 1);
}

t_list *next_token(char *line, int *i) {
    t_list *token;
    int j = 0;
    char c = ' ';
    if (!(token = malloc(sizeof(t_list)))
        || !(token->token = malloc(sizeof(char) * next_alloc(line, i))))
        return (NULL);
    while (line[*i] && (line[*i] != ' ' || c != ' ')) {
        if (c == ' ' && (line[*i] == '\'' || line[*i] == '\"'))
            c = line[(*i)++];
        else if (c != ' ' && line[*i] == c) {
            c = ' ';
            (*i)++;
        } else if (line[*i] == '\\' && (*i)++)
            token->token[j++] = line[(*i)++];
        else
            token->token[j++] = line[(*i)++];
    }
    token->token[j] = '\0';
    token->prev = NULL;
    token->next = NULL;
    return (token);
}

int ignore_sep(char *line, int i) {
    if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == ';')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '|')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>'
             && line[i + 2] && line[i + 2] == '>')
        return (1);
    return (0);
}

t_list *get_tokens(char *line) {
    t_list *prev = NULL;
    t_list *next = NULL;
    int i = 0;
    int sep;

    ft_skip_space(line, &i);
    while (line[i]) {
        sep = ignore_sep(line, i);
        next = next_token(line, &i);
        next->prev = prev;
        if (prev)
            prev->next = next;
        prev = next;
        type_arg(next, sep);
        ft_skip_space(line, &i);
    }
    if (next)
        next->next = NULL;
    while (next && next->prev)
        next = next->prev;
    return next;
}

void print_list(t_list *head) {
    t_list *current = head;
    while (current) {
        printf("Token: %s, Type: %d\n", current->token, current->type);
        current = current->next;
    }
}

void free_list(t_list *head) {
    t_list *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp->token);
        free(tmp);
    }
}

int main(void) {
    t_list *head = NULL;
    const char *input = "cd /home/user \"$HOME\"    | echo val= ' $HOME' | env m=abc";

    head = get_tokens(input);
    print_list(head);
    free_list(head);
    return 0;
}
