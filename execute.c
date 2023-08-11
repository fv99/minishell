/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 17:16:13 by phelebra          #+#    #+#             */
/*   Updated: 2023/08/11 11:23:29 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define LINE_SIZE 1024
#define PATH_SEP ":"

extern int	g_status;

char	*get_executable_path(t_parsed *cmd, t_env *env)
{
	char	*path;
	if (cmd->args[0] != NULL)
	{
		if (cmd->args[0][0] == '/')
			path = ft_strdup(cmd->args[0]);
		else
			path = get_path(cmd->args[0], env);
		return (path);
	}
	return (NULL);
}

void	handle_child_process(t_parsed *cmd, char **envp, char *path)
{
	int		infile_fd;
	int		outfile_fd;

	infile_fd = cmd->infile;
	outfile_fd = cmd->outfile;
	if (infile_fd != STDIN_FILENO)
	{
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
	if (outfile_fd != STDOUT_FILENO)
	{
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
	}
	if (execve(path, cmd->args, envp) == -1)
	{
		free(path);
		g_status = 127;
		exit(g_status);
	}
}

void	handle_parent_process(pid_t pid, t_parsed *cmd)
{
	g_status = 1;
	waitpid(pid, &g_status, WUNTRACED);
	if (WIFEXITED(g_status) && WEXITSTATUS(g_status) == EXEC_ERROR)
		ft_printf("Command not found: %s \n", cmd->args[0]);
}

void	fork_and_exec(t_parsed *cmd, char **envp, char *path)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		handle_child_process(cmd, envp, path);
	}
	else if (pid < 0)
	{
		you_fucked_up("Fork failed", 127);
	}
	else
	{
		handle_parent_process(pid, cmd);
	}
}

int	execute(t_parsed *cmd, char **envp, t_env *env)
{
	char	*path;

	if (check_builtins(cmd, env))
		return (1);
	path = get_executable_path(cmd, env);
	if (path != NULL)
	{
		fork_and_exec(cmd, envp, path);
		free(path);
	}
	return (g_status);
}
