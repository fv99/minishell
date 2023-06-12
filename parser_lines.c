/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/12 12:24:09 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define DELIMS " \t\r\n"
#define BUFSIZE 128 //sets malloc size, change as needed

// move this to header file later
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
}	command;

// assumes | is always surrounded by spaces
// todo: add redirects
command	**split_line(char *line)
{
	command **cmds;
	command	*cmd;
	char	*cmdline;
	int		i;
	
	cmds = malloc(sizeof(command) * BUFSIZE);
	cmdline = ft_strtok(line, "|");
	i = 0;
	while (cmdline != NULL)
	{
		cmd = malloc(sizeof(command));
		cmd->args = split_args(cmdline);
		cmdline = ft_strtok(NULL, "|");
		if (cmdline != NULL)
			cmd->op = PIPE;
		else
			cmd->op = NONE;
		cmds[i++] = cmd;
	}
	cmds[i] = NULL;
	return (cmds);
}

// modified split_line from below
// splits according to delims 
char	**split_args(char *line)
{
	int		i;
	char	**tokens;
	char	*token;

	i = 0;
	tokens = malloc(sizeof(char *) * BUFSIZE);
	token = ft_strtok(line, DELIMS);
	while (token != NULL)
	{
		tokens[i] = ft_strdup(token);
		token = ft_strtok(NULL, delims);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

/* 
// splits line according to delimiters with strtok
char	**split_line(char *line)
{
	int		i;
	char	**tokens;
	char	*token;

	i = 0;
	tokens = malloc(sizeof(char *) * BUFSIZE);
	token = ft_strtok(line, DELIMS);
	while (token != NULL)
	{
		tokens[i] = malloc(sizeof(char) * (ft_strlen(token) + 1));
		ft_strcpy(tokens[i], token);
		token = ft_strtok(NULL, DELIMS);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

 */