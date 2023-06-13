/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:56:26 by x230              #+#    #+#             */
/*   Updated: 2023/06/13 13:15:55 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define LINE_SIZE 1024
#define PATH_SEP ":"
#define EXEC_ERROR 42

// main loop to run the shell
void    shell_loop(void)
{
	// command	*cmd;
    char    *line;
	char	*out;
    int     status;
	extern char **__environ;	// our only global variable ?

	status = 1;
	while (status)
	{
		line = readline("> ");
        if (!line) // if readline returns NULL, it means we hit an EOF (like Ctrl+D)
            break;
		if (line && *line)
            add_history(line);
		out = sanitize_input(line);
		free(line);
		ft_printf("sanitized line: %s \n", out);
		
			// reenable this after fixing the parser
/* 		cmd = parse_input(line);
		while (cmd != NULL)
		{
			test_cmd_parser(cmd);
			// status = execute(cmds[i], __environ);
			cmd = cmd->next;
		}
		free_commands(cmd); */
		free(out);
	}
}

// check for pipes or redirects
int	check_opts(command *cmd, char **envp)
{
	if (cmd->op == PIPE)
		execute_pipe(cmd, envp);
	else if (cmd->op == RED_IN || cmd->op == RED_OUT || cmd->op == RED_APP)
		execute_redirect(cmd, envp);

	else
		execute(cmd, envp);
	return (1);
}

// modified pipex to accept our cmd struct
void	execute_pipe(command *cmd, char **envp)
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
		dup2(pid_fd[1], STDOUT_FILENO);
		close(pid_fd[1]);
		check_opts(cmd, envp);
		exit(1);
	}
	else
	{
		close(pid_fd[1]);
		dup2(pid_fd[0], STDIN_FILENO);
		close(pid_fd[0]);
		check_opts(cmd->next, envp);
		waitpid(pid, NULL, 0);
	}
}

void	execute_redirect(command *cmd, char **envp)
{
	int		fd;
	pid_t	pid;

	if (cmd->op == RED_OUT)
		fd = open(cmd->next->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->op == RED_IN)
		fd = open(cmd->next->args[0], O_RDONLY);
	if (cmd->op == RED_APP)
		fd = open(cmd->next->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		you_fucked_up("Error opening fd");
	
	pid = fork();
	if (pid == 0)
	{
		// child process
		if (cmd->op == RED_OUT || cmd->op == RED_APP)
			dup2(fd, STDOUT_FILENO);
		else if (cmd->op == RED_IN)
			dup2(fd, STDIN_FILENO);
		close(fd);
		check_opts(cmd, envp);
		exit(0);
	}
	else if (pid < 0)
		you_fucked_up("Error forking process");
	else
	{
		// parent process
		close(fd);
		waitpid(pid, NULL, 0);
	}
}

// need to make it so args passed are only until pipe or other delim
int	execute(command *cmd, char **envp)
{
	char	*path;
	pid_t	pid;
	int		status;

	if (check_builtins(cmd->args, envp)) // check for builtins first
        return (1);
	path = get_path(cmd->args[0], envp);
	pid = fork();
	if (pid == 0)
	{
		// child process
		if (execve(path, cmd->args, envp) == -1)
		{
			free(path);
			exit(EXEC_ERROR);
		}
	}
	else if (pid < 0)
		you_fucked_up("Fork failed");
	else
	{
		// parent process
		waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXEC_ERROR)
			ft_printf("Command not found: %s \n", cmd->args[0]);
	}
	free(path);
	return (1);
}


// Gets PATH environment variable and saves into path_env
// Loops through each directory in path_env separated by PATH_SEP
// Uses get_path_token function to get full path, returns it
char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path_env;
	char	*path;
	int		cmd_len;

	i = 0;
	cmd_len = ft_strlen(cmd);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = ft_strdup(envp[i] + 5);
			break ;
		}
		i++;
	}
	if (path_env == NULL) // if PATH= was not found in envp
		return NULL;
	path = get_path_token(cmd, path_env, cmd_len);
	return (path);
}

// Searches through each path in the PATH environment variable
// Construct the full path to the command
// Uses access() to check if path is executable
// Returns it if it is
char	*get_path_token(char *cmd, char *path_env, int cmd_len)
{
	char	*path_token;
	char	*path;

	path_token = ft_strtok(path_env, PATH_SEP);
	while (path_token != NULL)
	{
		path = ft_calloc(ft_strlen(path_token) + cmd_len + 2, 1);
		ft_strcat(path, path_token);
		ft_strcat(path, "/");
		ft_strcat(path, cmd);
		if (access (path, X_OK) == 0)
		{
			free(path_env);
			return (path);
		}
		free(path);
		path_token = ft_strtok(NULL, PATH_SEP);
	}
	free(path_env);
	return (NULL);
}
