/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/19 13:12:39 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define DELIMS " \t\r\n"
#define ARG_SIZE 256 // sets malloc size, change as needed
#define LINE_SIZE 1024


// strips quotes from src string
void sanitize_quotes(char *src, char *dest)
{
    int     n;
    int     i;
    int     j;
    char    lastquote;
    char    c;

    i = 0;
    j = 0;
    lastquote = '\0';
    n = ft_strlen(src);
    if (n <= 1)
    {
        ft_strcpy(dest, src);
        return ;
    }
    while (i < n)
    {
        c = src[i];
        if ((c == '\'' || c == '\"') && lastquote == 0)
			lastquote = c;
		else if (c == lastquote)
			lastquote = 0;
		else
			dest[j++] = c;
        i++;
    }
    dest[j] = '\0';
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