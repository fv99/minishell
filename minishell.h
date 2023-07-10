/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:18:18 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/10 17:18:46 by fvonsovs         ###   ########.fr       */
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

#define EXEC_ERROR 126
#define NDIR "No such file or directory"
#define NPERM "Permission denied"
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

void	update_current_operation(t_ops *curr, char **args, int *i);

t_parsed *add_new_node(char **cmds, t_ops curr, t_parsed **head, t_parsed **tail);

t_parsed *fill_list(char **args);

t_parsed *new_parser_node(char **args, t_ops op);

t_ops	check_op(char *str);

int open_file(char *file, t_ops op);

void handle_redirection(t_parsed *node, t_ops op, char *file);


// void	sanitize_quotes(char *src, char *dest);

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

// utils_3.c

char	**ft_matrix_replace_in(char ***big, char **small, int n);

int		ft_matrixlen(char **m);

void	ft_free_matrix(char ***m);

// minishell.c

void    shell_loop(void);

int		check_opts(t_parsed *cmd, char **envp);

int		execute(t_parsed *cmd, char **envp);

char	*get_path(char *cmd, char **envp);

char	*get_path_token(char *cmd, char *path_env, int cmd_len);

// execute.c



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

void test_parser(t_parsed *head);


#endif