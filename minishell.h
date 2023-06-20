/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:18:18 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/20 16:25:42 by fvonsovs         ###   ########.fr       */
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

/*
	char_i - counter to iterate input string
	word_n - used to count words in input string, in fill_array as start index
	token_n - used in ft_fill_array, counter of processed tokens

	quotes - tracks if we are inside quotation, true/false
	quote_c - stores type of quote (ascii value for ' or ") or 0
*/
typedef struct s_lexer
{
	int char_i;
	int	word_n;
	int	token_n;
	int	quotes;
	int	quote_c;
}	t_lexer;


// for storing our operations:
typedef enum s_ops
{
	NONE,
	PIPE,		// |
	RED_IN,		// <
	RED_OUT,	// >
	RED_APP,	// >>
	HEREDOC		// <<
}	t_ops;

typedef struct s_parsed
{
	char	**args;
	t_ops		op;
	int		infile;
	int		outfile;
	struct	parsed *next;
}	t_parsed;

// lexer.c

char	**tokenize(char *src, char *delims, char **envp);

int		count_words(char *s, char *delims, t_lexer *lex);

char	**fill_array(char **ret, char *s, char *delims, t_lexer *lex);

// expand.c 

char	*expand_args(char **str_ptr, char **envp);

char	*expand_arg(char *str, int counter, char **envp);

char	*get_arg(char *argname, char **envp);

// parser_lines.c

t_ops	check_op(char *str);

void	sanitize_quotes(char *src, char *dest);

// utils_1.c

int		you_fucked_up(char *msg);

void	free_array(char	**ptr);

char	*ft_strcat(char *dest, char *src);

char    *ft_strcpy(char *s1, char *s2);

int		ft_strcmp(char *s1, char *s2);

// utils_2.c

bool	is_delimiter(char c, const char *delims);

char	*ft_strtok(char *str, const char *delims);

char	*ft_strstr(char *str, char *to_find);

// minishell.c

void    shell_loop(void);

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

void	here_doc_exec(char **argv, int *pid_fd);

// test_functions.c

void	test_tokenize(char* input, char **envp);


#endif