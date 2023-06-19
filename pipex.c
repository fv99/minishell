/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 13:33:44 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/12 13:44:04 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// taken from my pipex, modified to accept command struct
// Creates pipe storing its fd in pid_fd
// Forks a new process, storing pid in pid
// Calls here_doc_exec 
void	here_doc(command *cmd)
{
	int		pid_fd[2];
	pid_t	pid;

	if (pipe(pid_fd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
		here_doc_exec(cmd->args, pid_fd);
	else
	{
		close(pid_fd[1]);
		dup2(pid_fd[0], 0);
		wait(NULL);
	}
}

void	here_doc_exec(char **argv, int *pid_fd)
{
	char	*line;

	close(pid_fd[0]);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
		{
			free(line);
			exit(0);
		}
		if (ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
		{
			free(line);
			exit(0);
		}
		ft_printf("%s", line);
		free(line);
	}
}

void	pipex(command *cmd, char **envp)
{
	pid_t	pid;
	int		pid_fd[2];

	if (pipe(pid_fd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
	{
		close(pid_fd[0]);
		dup2(pid_fd[1], 1);
		execute(cmd, envp);
	}
	else
	{
		close(pid_fd[1]);
		dup2(pid_fd[0], 0);
	}
}
