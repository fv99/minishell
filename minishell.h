/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:18:18 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/26 10:21:36 by phelebra         ###   ########.fr       */
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
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define EXEC_ERROR 127
#define WRITE_END 1
#define READ_END 0

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
	struct	s_parsed *next;
}	t_parsed;

// lexer_1.c

char	**lexer(char *s, char **envp);

char	**tokenize(char *src, char *delims);

int		count_words(char *s, char *delims, t_lexer *lex);

char	**fill_array(char **ret, char *s, char *delims, t_lexer *lex);

// lexer_2.c

char	**tokenize_opts(char *src, char *delims);

int		count_words_opts(char *s, char *delims, t_lexer *lex);

char	**fill_array_opts(char **ret, char *s, char *delims, t_lexer *lex);

// expand.c 

char	*expand_args(char **str_ptr, char **envp);

char	*expand_arg(char *str, int counter, char **envp);

char	*get_arg(char *argname, char **envp);

// parser.c

t_parsed *fill_list(char **args);

void update_current_operation(t_ops *curr, char **args, int *i, t_parsed *node);

t_parsed *add_new_node(char **cmds, t_ops curr, t_parsed **head, t_parsed **tail);


t_parsed *new_parser_node(char **args, t_ops op);

t_parsed *add_null_node(t_parsed **head, t_parsed **tail);

t_ops	check_op(char *str);


// void	sanitize_quotes(char *src, char *dest);

// utils_1.c

int		you_fucked_up(char *msg, int status);

void	free_array(char	**ptr);

char	*ft_strcat(char *dest, char *src);

char    *ft_strcpy(char *s1, char *s2);

int		ft_strcmp(char *s1, char *s2);

// utils_2.c

bool	is_delimiter(char c, const char *delims);

char	*ft_strtok(char *str, const char *delims);

char	*ft_strstr(char *str, char *to_find);

// utils_3.c

char	**ft_matrix_replace_in(char ***big, char **small, int n);

int		ft_matrixlen(char **m);

void	ft_free_matrix(char ***m);

// minishell.c

void    shell_loop(char **environ2);

void	execute_commands(t_parsed *head, char **envp);

void	sigint_handler(int sig);

void	pipex2(t_parsed *curr, char **envp);

int		execute(t_parsed *cmd, char ***envp);

char	*get_path(char *cmd, char **envp);

char	*get_path_token(char *cmd, char *path_env, int cmd_len);

// execute.c

void	*mini_perror(int err_type, char *param, int err);

char	*get_here_str(char *str[2], size_t len, char *limit, char *warn);

int		get_here_doc(char *str[2], char *aux[2]);

int		get_fd(int oldfd, char *path, int flags[2]);

t_parsed	*get_outfile1(t_parsed *node, char **args, int *i);

t_parsed	*get_outfile2(t_parsed *node, char **args, int *i);

t_parsed	*get_infile1(t_parsed *node, char **args, int *i);

t_parsed	*get_infile2(t_parsed *node, char **args, int *i);



// builtins_1.c

int		check_builtins(char **args, char ***envp);

int		builtin_exit(void);

int		builtin_cd(char **args);

char	*builtin_cd_expand_home(char **args, char *home_dir);

int		builtin_pwd(void);

// builtins_2.c

int		builtin_echo(char **args);

int		builtin_export(char **args, char ***envp, int *num_env_vars);

int		builtin_env(char ***envp);

int		builtin_unset(char **args, char ***envp);


// pipex.c

void	here_doc_exec(char **argv, int *pid_fd);

// test_functions.c

void	test_tokenize(char* input, char **envp);

void test_parser(t_parsed *head);


#endif