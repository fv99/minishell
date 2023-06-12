/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/12 14:02:44 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define DELIMS " \t\r\n"
#define BUFSIZE 128 //sets malloc size, change as needed

// assumes | is always between spaces
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
		cmd->args = split_args(cmdline, &(cmd->op));
		cmdline = ft_strtok(NULL, "|");
		cmds[i++] = cmd;
	}
	cmds[i] = NULL;
	return (cmds);
}

char	**split_args(char *line, ops *op)
{
	int		i;
	char	**tokens;
	char	*token;

	i = 0;
	tokens = malloc(sizeof(char *) * BUFSIZE);
	token = ft_strtok(line, " \t\r\n\a"); // we're going to handle operators separately
	while (token != NULL)
	{
		if (strcmp(token, "<") == 0)
			*op = RED_IN;
		else if (strcmp(token, ">") == 0)
			*op = RED_OUT;
		else if (strcmp(token, ">>") == 0)
			*op = RED_APP;
		else if (strcmp(token, "<<") == 0)
			*op = HEREDOC;
		else
			tokens[i++] = ft_strdup(token);
		token = ft_strtok(NULL, " \t\r\n\a");
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