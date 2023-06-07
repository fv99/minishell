/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/07 14:30:01 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define DELIMS " \t\r\n"
#define BUFSIZE 128 //sets malloc size, change as needed

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

