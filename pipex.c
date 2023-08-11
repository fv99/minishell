/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 13:33:44 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/08/07 11:02:39 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

t_parsed	*get_infile1(t_parsed *node, char **args, int *i)
{
	char	*nl;
	int		flags[2];

	flags[0] = 0;
	flags[1] = 0;
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->infile = get_fd(node->infile, args[*i], flags);
	if (!args[*i] || node->infile == -1)
	{
		*i = -1;
		if (node->infile != -1)
		{
			ft_putendl_fd(nl, 2);
			g_status = 2;
		}
		else
			g_status = 1;
	}
	return (node);
}

t_parsed	*get_infile2(t_parsed *node, char **args, int *i)
{
	char	*aux[2];
	char	*nl;
	char	*str[2];

	str[0] = NULL;
	str[1] = NULL;
	aux[0] = NULL;
	aux[1] = "minishell: warning: here-document delimited by end-of-file";
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
	{
		aux[0] = args[*i];
		node->infile = get_here_doc(str, aux);
	}
	if (!args[*i] || node->infile == -1)
	{
		*i = -1;
		if (node->infile != -1)
		{
			ft_putendl_fd(nl, 2);
			g_status = 2;
		}
	}
	return (node);
}

void	child_process(t_parsed *curr, char **envp, int *pid_fd, char *path)
{
	close(pid_fd[0]);
	if (curr->outfile != STDOUT_FILENO)
		dup2(curr->outfile, STDOUT_FILENO);
	else
		dup2(pid_fd[1], STDOUT_FILENO);
	close(pid_fd[1]);
	execve(path, curr->args, envp);
	free(path);
}

void	parent_process(t_parsed *curr, pid_t pid, int *pid_fd, char *path)
{
	close(pid_fd[1]);
	if (curr->infile != STDIN_FILENO)
	{
		dup2(pid_fd[0], STDIN_FILENO);
		close(pid_fd[0]);
	}
	else
		dup2(pid_fd[0], STDIN_FILENO);
	waitpid(pid, &g_status, WUNTRACED);
	if (WIFEXITED(g_status) && WEXITSTATUS(g_status) == EXEC_ERROR)
		ft_printf("Command not found: %s \n", curr->args[0]);
	free(path);
}

void	pipex2(t_parsed *curr, char **envp, t_env *env)
{
	pid_t	pid;
	int		pid_fd[2];
	char	*path;

	if (curr->args[0][0] == '/')
		path = ft_strdup(curr->args[0]);
	else
		path = get_path(curr->args[0], env);
	if (pipe(pid_fd) == -1)
		you_fucked_up("Pipe error", 9);
	pid = fork();
	if (pid == -1)
		you_fucked_up("Fork error", 8);
	if (pid == 0)
		child_process(curr, envp, pid_fd, path);
	else
		parent_process(curr, pid, pid_fd, path);
}
