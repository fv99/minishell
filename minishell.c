/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:56:26 by x230              #+#    #+#             */
/*   Updated: 2023/07/20 11:30:54 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
	TODO LIST
	- redirects and heredoc
	- builtins dont work with new linked list parser
 */
#include "minishell.h"

#define LINE_SIZE 1024
#define PATH_SEP ":"

extern int	g_status;

// main loop to run the shell
void shell_loop(void)
{
    t_parsed *head;
    char *line;
    char **ebloid;
    extern char **environ;
	int save_fd;

    g_status = 0;
	signal(SIGINT, sigint_handler);
    while (1)
    {
        save_fd = dup(STDIN_FILENO); // Duplicate the original file descriptor
        line = readline("\033[30m\033[101;5;7m⛧ minihell ⛧\033[0m\033[31m >\033[0m ");
        if (line == NULL)
        {
            close(save_fd); // Close the duplicated file descriptor before breaking the loop
            break;
        }
        if (line && *line)
		{
            add_history(line);
			// test_tokenize(line, environ);
			ebloid = lexer(line, environ);
			head = fill_list(ebloid);
			// test_parser(head);
			execute_commands(head, environ);
			free_array(ebloid);
		}
		free(line);
        dup2(save_fd, STDIN_FILENO); // Restore the original file descriptor
        close(save_fd); // Close the duplicated file descriptor
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

void	execute_commands(t_parsed *head, char **envp)
{
	t_parsed *current;
	t_parsed *next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->op == PIPE)
			pipex2(current, envp);
		else
			execute(current, envp);
        free(current->args);  // Free the array of arguments
        free(current);  // Free the node itself
        current = next;  // Move to the next node
	}
}

void pipex2(t_parsed *curr, char **envp)
{
    pid_t pid;
    int pid_fd[2];
    char *path;

    if (pipe(pid_fd) == -1)
        you_fucked_up("Pipe error", 9);
    pid = fork();
    if (pid == -1)
        you_fucked_up("Fork error", 8);	
	else if (curr->args[0][0] == '/')
		path = ft_strdup(curr->args[0]);
	else
    	path = get_path(curr->args[0], envp);
    if (!pid)
    {
        close(pid_fd[0]); // Close unused read end of the pipe
        if (curr->outfile != STDOUT_FILENO)
            dup2(curr->outfile, STDOUT_FILENO);
        else
            dup2(pid_fd[1], STDOUT_FILENO);
        close(pid_fd[1]);
        execve(path, curr->args, envp);
        free(path);
    }
    else
    {
        close(pid_fd[1]); // Close unused write end of the pipe
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
}

// only single commands
int execute(t_parsed *cmd, char **envp)
{
	char *path;
	pid_t pid;
	int infile_fd;
	int outfile_fd;

	if (check_builtins(cmd->args, envp))
		return 1;
	if (cmd->args[0][0] == '/')
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], envp);
	pid = fork();
	if (pid == 0)
	{
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
	else if (pid < 0)
		you_fucked_up("Fork failed", 127);
	else
	{
		g_status = 1;
		waitpid(pid, &g_status, WUNTRACED);
		if (WIFEXITED(g_status) && WEXITSTATUS(g_status) == EXEC_ERROR)
			ft_printf("Command not found: %s \n", cmd->args[0]);
	}
	free(path);
	return g_status;
}

// Gets PATH environment variable and saves into path_env
// Loops through each directory in path_env separated by PATH_SEP
// Uses get_path_token function to get full path, returns it
char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path_env = NULL;
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
