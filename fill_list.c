/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 10:03:28 by phelebra          #+#    #+#             */
/*   Updated: 2023/08/02 11:09:09 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define ARG_SIZE 256 

void	init_fill_list_vars(t_fill_list_vars *fl_vars)
{
	fl_vars->cmds = malloc(sizeof(char *) * ARG_SIZE);
	fl_vars->head = NULL;
	fl_vars->tail = NULL;
	fl_vars->curr = NONE;
	fl_vars->i = 0;
	fl_vars->j = 0;
}

t_parsed	*add_last_command_if_not_empty(t_fill_list_vars *fl_vars)
{
	if (fl_vars->j != 0)
	{
		fl_vars->cmds[fl_vars->j] = NULL;
		fl_vars->tail = add_new_node(fl_vars->cmds, NONE, &fl_vars->head,
				&fl_vars->tail);
		(fl_vars->tail)->next = NULL;
	}
	else
		free(fl_vars->cmds);
	return (fl_vars->head);
}

void	add_node_and_update_current_operation(t_fill_list_vars *fl_vars,
	char **args)
{
	fl_vars->cmds[(fl_vars->j)] = NULL;
	(fl_vars->tail) = add_new_node(fl_vars->cmds, fl_vars->curr,
			&fl_vars->head, &fl_vars->tail);
	fl_vars->cmds = malloc(sizeof(char *) * ARG_SIZE);
	fl_vars->j = 0;
	if (fl_vars->curr == RED_OUT || fl_vars->curr == RED_IN
		|| fl_vars->curr == RED_APP || fl_vars->curr == HEREDOC)
		update_current_operation(&fl_vars->curr, args,
			&fl_vars->i, fl_vars->tail);
}

t_parsed	*fill_list(char **args)
{
	t_fill_list_vars	fl_vars;

	init_fill_list_vars(&fl_vars);
	while (args[fl_vars.i] != NULL)
	{
		fl_vars.curr = check_op(args[fl_vars.i]);
		if (fl_vars.curr != NONE)
			add_node_and_update_current_operation(&fl_vars, args);
		else
			fl_vars.cmds[fl_vars.j++] = args[fl_vars.i];
		fl_vars.i++;
	}
	return (add_last_command_if_not_empty(&fl_vars));
}
