/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:56:26 by x230              #+#    #+#             */
/*   Updated: 2023/08/07 12:37:24 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define LINE_SIZE 1024
#define PATH_SEP ":"

extern int	g_status;

void	process_line(char *line, char **environ, t_env *env)
{
	char		**ebloid;
	t_parsed	*head;

	add_history(line);
	ebloid = lexer(line, environ);
	if (ebloid != NULL)
	{
		head = fill_list(ebloid);
		execute_commands(head, environ, env);
		free_array(ebloid);
	}
}

void	shell_loop(t_env *env)
{
	char		*line;
	extern char	**environ;
	int			save_fd;

	g_status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		save_fd = dup(STDIN_FILENO);
		line = readline("\033[31;5m⛧ minihell ⛧\033[0m > ");
		if (!line)
		{
			close(save_fd);
			break ;
		}
		if (*line)
			process_line(line, environ, env);
		free(line);
		dup2(save_fd, STDIN_FILENO);
		close(save_fd);
	}
}

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		printf("\n");
		rl_redisplay();
	}
}

void	execute_commands(t_parsed *head, char **envp, t_env *env)
{
	t_parsed	*current;
	t_parsed	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->op == PIPE)
			pipex2(current, envp, env);
		else
			execute(current, envp, env);
		free(current->args);
		free(current);
		current = next;
	}
}
