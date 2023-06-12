/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:18:18 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/12 14:20:41 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

// structs go here

// for storing our operations:
typedef enum ops
{
	NONE,
	PIPE,		// |
	RED_IN,		// <
	RED_OUT,	// >
	RED_APP,	// >>
	HEREDOC		// <<
}	ops;

typedef struct command
{
	char	**args;
	ops		op;
	struct command *next;
}	command;


// parser_lines.c

command	**split_line(char *line);

char	**split_args(char *line, ops *op);

// utils_1.c

int		you_fucked_up(char *msg);

void	free_array(char	**ptr);

char	*ft_strcat(char *dest, char *src);

char    *ft_strcpy(char *s1, char *s2);

int		ft_strcmp(char *s1, char *s2);

// utils_2.c

bool	is_delimiter(char c, const char *delims);

char	*ft_strtok(char *str, const char *delims);

void    free_cmds(command **cmds);

// minishell.c

void    shell_loop(void);

int		check_opts(command *cmd, char **envp);

void	execute_pipe(command *cmd, char **envp);

void	execute_redirect(command *cmd, char **envp);


int		execute(command *cmd, char **envp);

char	*get_path(char *cmd, char **envp);

char	*get_path_token(char *cmd, char *path_env, int cmd_len);

// builtins_1.c

int		check_builtins(char **args, char **envp);

int		builtin_exit(void);

int		builtin_cd(char **args);

char	*builtin_cd_expand_home(char **args, char *home_dir);

int		builtin_pwd(void);

// builtins_2.c

int		builtin_echo(char **args);

int		builtin_export(char **args, char **envp);

// pipex.c

void	here_doc(command *cmd);

void	here_doc_exec(char **argv, int *pid_fd);

void	pipex(command *cmd, char **envp);

// test_functions.c

void 	test_parse_line(char **tokens);

#endif