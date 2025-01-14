#include "minishell.h"

void	insert_leftp(t_c *p)
{
	if (p->pre)
	{
		p->pre->next = create_new_command_node(p->pre);
		p->pre->next->tp = 9;
		p->pre->next->next = p;
		p->pre = p->pre->next;
		return ;
	}
	p->pre = create_new_command_node(NULL);
	p->pre->tp = 9;
	p->pre->next = p;
}

void	insert_rightp(t_c *p)
{
	if (p->next)
	{
		p->next->pre = create_new_command_node(p);
		p->next->pre->next = p->next;
		p->next = p->next->pre;
		p->next->tp = 10;
		p->next->pre = p;
		return ;
	}
		p->next = create_new_command_node(p);
		p->next->tp = 10;
}

//return NULL, if inserting new token failed
//return the newly add s
void	add_subshell_for_one_pipe(t_c *p)
{
	if (p->next)
	{
		if (p->next->tp == 1)
		{
			insert_rightp(p->next);
			insert_leftp(p->next);
		}
	}
	if (p->pre)
	{
		if (p->pre->tp == 1)
		{
			insert_leftp(p->pre);
			insert_rightp(p->pre);
		}
	}
}

void	set_id_and_tp_for_cmdline(t_c *c)
{
	t_c	*temp;
	int	i;

	i = 3;
	temp = c;
	while (temp)
	{
		temp->id = i++;
		if (temp->tp == 1 && temp->cmd == NULL)
		{
			temp->tp = 4;
			temp->pipe_on = 1;
		}
		temp = temp->next;
	}
}

//this funciton add () to cmd unit before and after a pipe,
//	so that don't need to consider running cmd unit
//	in a subshell for pipe again when executing command line
//b[i] indicate if both side of the pipe need to be treat at level i;
//return NULL, if inserting new token failed
t_c	*treat_cmdline_for_pipe(t_c *c)
{
	t_c	*temp;

	if (c == NULL)
		return (NULL);
	temp = c;
	while (c)
	{
		if (c->tp == 6)
			break;
		c = c->next;
	}
	//printf("treat_cmdline_for_pipe(): a\n");//debug
	while (c)
	{
		add_subshell_for_one_pipe(c);
		c = c->next;
		while (c)
		{
			if (c->tp == 6)
				break;
			c = c->next;
		}
	}
	//printf("treat_cmdline_for_pipe(): b\n");//debug
	while (temp->pre)
		temp = temp->pre;
	set_id_and_tp_for_cmdline(temp);
	//printf("treat_cmdline_for_pipe(): c\n");//debug
	return (temp);
}
