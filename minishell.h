/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:18:18 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/08/07 11:03:00 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define EXEC_ERROR 127
# define WRITE_END 1
# define READ_END 0

typedef struct s_arg
{
	char	*argname;
	int		argname_len;
}	t_arg;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_lexer
{
	int	char_i;
	int	word_n;
	int	token_n;
	int	quotes;
	int	quote_c;
}	t_lexer;

// for storing our operations:
/*
	NONE,
	PIPE,		// |
	RED_IN,		// <
	RED_OUT,	// >
	RED_APP,	// >>
	HEREDOC		// <<
*/
typedef enum s_ops
{
	NONE,
	PIPE,
	RED_IN,
	RED_OUT,
	RED_APP,
	HEREDOC
}	t_ops;

typedef struct s_parsed
{
	char			**args;
	t_ops			op;
	int				infile;
	int				outfile;
	struct s_parsed	*next;
}	t_parsed;

typedef struct s_fill_list_vars
{
	char		**cmds;
	t_parsed	*head;
	t_parsed	*tail;
	t_ops		curr;
	int			i;
	int			j;
}	t_fill_list_vars;

// lexer_1.c
char		**lexer(char *s, char **envp);
char		**tokenize(char *src, char *delims);
int			count_words(char *s, char *delims, t_lexer *lex);
void		skip_and_parse(char *s, char *delims, t_lexer *lex);
char		**fill_array(char **ret, char *s, char *delims, t_lexer *lex);

// lexer_2.c
char		**tokenize_opts(char *src, char *delims);
int			count_words_opts(char *s, char *delims, t_lexer *lex);
char		**fill_array_opts(char **ret, char *s, char *delims, t_lexer *lex);

// expand.c 
char		*expand_args(char **str_ptr, char **envp);
t_arg		get_argname_and_len(char *str, int counter);
char		*determine_arg_and_create_str(char *str, int counter,
				t_arg arg_data, char **envp);
char		*expand_arg(char *str, int counter, char **envp);
char		*get_arg(char *argname, char **envp);

// fill_list.c
void		init_fill_list_vars(t_fill_list_vars *fl_vars);
t_parsed	*add_last_command_if_not_empty(t_fill_list_vars *fl_vars);
void		add_node_and_update_current_operation(t_fill_list_vars *fl_vars,
				char **args);
t_parsed	*fill_list(char **args);

// parser.c
void		update_current_operation(t_ops *curr, char **args, int *i,
				t_parsed *node);
t_parsed	*add_new_node(char **cmds, t_ops curr,
				t_parsed **head, t_parsed **tail);
t_parsed	*new_parser_node(char **args, t_ops op);
t_ops		check_op(char *str);

// utils_1.c
int			you_fucked_up(char *msg, int status);
void		free_array(char	**ptr);
char		*ft_strcat(char *dest, char *src);
char		*ft_strcpy(char *s1, char *s2);
int			ft_strcmp(char *s1, char *s2);

// utils_2.c
bool		is_delimiter(char c, const char *delims);
char		*ft_strtok(char *str, const char *delims);
char		*ft_strstr(char *str, char *to_find);

// utils_3.c
char		**ft_matrix_replace_in(char ***big, char **small, int n);
int			ft_matrixlen(char **m);
void		ft_free_matrix(char ***m);

// minishell.c
void		process_line(char *line, char **environ, t_env *env);
void		shell_loop(t_env *env);
void		execute_commands(t_parsed *head, char **envp, t_env *env);
void		sigint_handler(int sig);
int			execute(t_parsed *cmd, char **envp, t_env *env);

// execute.c
char		*get_executable_path(t_parsed *cmd, t_env *env);
void		handle_child_process(t_parsed *cmd, char **envp, char *path);
void		handle_parent_process(pid_t pid, t_parsed *cmd);
void		fork_and_exec(t_parsed *cmd, char **envp, char *path);
int			execute(t_parsed *cmd, char **envp, t_env *env);

// get.c
char		*get_here_str(char *str[2], size_t len, char *limit, char *warn);
int			get_here_doc(char *str[2], char *aux[2]);
int			get_fd(int oldfd, char *path, int flags[2]);
t_parsed	*get_outfile1(t_parsed *node, char **args, int *i);
t_parsed	*get_outfile2(t_parsed *node, char **args, int *i);

// builtin_cd.c
void		handle_cd_home(const char *home_dir, char *cwd);
void		handle_cd_oldpwd(const char *oldpwd, char *cwd);
void		handle_cd_cmd(char *cmd, char *cwd);
char		*prepare_cd_cmd_and_cwd(char **args,
				const char *home_dir, char **cwd);
int			builtin_cd(char **args);

// builtin_export.c
t_env		*create_env_node(char *key, char *value);
t_env		*find_env_key(t_env *env, char *key);
void		add_env_node(t_env **env, t_env *new_node);
int			builtin_export(char **args, t_env **env);

// builtin_echo.c
void		handle_outfile_redirection(t_parsed *parsed_cmd);
void		print_arguments(char **args, int start, bool n_opt);
int			builtin_echo(t_parsed *parsed_cmd);

// builtins_1.c
int			check_builtins(t_parsed *parsed_cmd, t_env *env);
int			builtin_exit(void);
char		*builtin_cd_expand_home(char **args, char *home_dir);
int			builtin_pwd(t_parsed *parsed_cmd);

// builtins_2.c
int			builtin_echo(t_parsed *parsed_cmd);
int			builtin_export(char **args, t_env **env);
int			builtin_env(t_parsed *parsed_cmd, t_env *env);
int			builtin_unset(char **args, t_env **env);

// pipex.c
t_parsed	*get_infile1(t_parsed *node, char **args, int *i);
t_parsed	*get_infile2(t_parsed *node, char **args, int *i);
void		pipex2(t_parsed *curr, char **envp, t_env *env);
void		parent_process(t_parsed *curr, pid_t pid, int *pid_fd, char *path);
void		child_process(t_parsed *curr, char **envp, int *pid_fd, char *path);

//path.c
char		*get_path(char *cmd, t_env *env);
char		*get_path_token(char *cmd, char *path_env, int cmd_len);

#endif