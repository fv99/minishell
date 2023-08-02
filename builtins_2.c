/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/08/02 14:08:14 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

int	builtin_env(t_parsed *parsed_cmd, t_env *env)
{
	t_env	*temp;
	int		save_stdout;
	int		outfile_fd;

	temp = env;
	save_stdout = dup(STDOUT_FILENO);
	outfile_fd = parsed_cmd->outfile;
	if (outfile_fd != STDOUT_FILENO)
	{
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
	}
	while (temp != NULL)
	{
		printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return (1);
}

int	builtin_unset(char **args, t_env **env)
{
	t_env	*current;
	t_env	*temp;
	char	*key;

	if (args[1] == NULL)
	{
		fprintf(stderr, "unset: not enough arguments\n");
		return (1);
	}
	key = args[1];
	current = *env;
	while (current != NULL && current->next != NULL)
	{
		if (strcmp(current->next->key, key) == 0)
		{
			temp = current->next;
			current->next = current->next->next;
			return (free(temp->key), free(temp->value), free(temp), 1);
		}
		current = current->next;
	}
	return (0);
}
