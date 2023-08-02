/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/08/02 10:04:07 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// checks for heredoc and append, needed for fill_list
void	update_current_operation(t_ops *curr, char **args,
	int *i, t_parsed *node)
{
	if ((*curr == RED_OUT && args[*i + 1] != NULL)
		&& check_op(args[*i + 1]) == RED_OUT)
	{
		*curr = RED_APP;
		node = get_outfile2(node, args, i);
	}
	else if ((*curr == RED_OUT && args[*i + 1] != NULL)
		&& check_op(args[*i + 1]) == NONE)
		node = get_outfile1(node, args, i);
	else if ((*curr == RED_IN && args[*i + 1] != NULL)
		&& check_op(args[*i + 1]) == RED_IN)
	{
		*curr = HEREDOC;
		node = get_infile2(node, args, i);
	}
	else if ((*curr == RED_IN && args[*i + 1] != NULL)
		&& check_op(args[*i + 1]) == NONE)
		node = get_infile1(node, args, i);
}

// adds new node to linked list with current commands and operations
t_parsed	*add_new_node(char **cmds, t_ops curr,
	t_parsed **head, t_parsed **tail)
{
	t_parsed	*new;

	new = new_parser_node(cmds, curr);
	if (*head == NULL)
		*head = new;
	else
		(*tail)->next = new;
	return (new);
}

t_parsed	*new_parser_node(char **args, t_ops op)
{
	t_parsed	*new;

	new = malloc(sizeof(t_parsed));
	new->args = args;
	new->op = op;
	new->infile = STDIN_FILENO;
	new->outfile = STDOUT_FILENO;
	new->next = NULL;
	return (new);
}

// checks for op type 
t_ops	check_op(char *str)
{
	if (!strcmp(str, "|"))
		return (PIPE);
	if (!strcmp(str, "<"))
		return (RED_IN);
	if (!strcmp(str, ">"))
		return (RED_OUT);
	if (!strcmp(str, ">>"))
		return (RED_APP);
	if (!strcmp(str, "<<"))
		return (HEREDOC);
	return (NONE);
}
