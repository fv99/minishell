/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/13 13:29:58 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define DELIMS " \t\r\n"
#define ARG_SIZE 256 // sets malloc size, change as needed
#define LINE_SIZE 1024


// will get rid of unclosed quotes or special characters
// which are not required by the
// subject such as \ (backslash) or ; (semicolon)
char *sanitize_input(char *line)
{
    bool    single_open = false;
    bool    double_open = false;
    char    current;
    char    *output;
    int     i;
    int     j;

    i = 0;
    j = 0;
    output = malloc(ft_strlen(line) + 1);
    if (!output)
        return NULL;

    while (i < ft_strlen(line))
    {
        current = line[i];

        if (current == '\'')    // handle single quote
        {
            if (single_open) // closing single quote
            {
                single_open = false;
                output[j++] = '\'';
            }
            else
                single_open = true;
            i++;
            continue;
        }
        if (current == '\"')    // handle double quote
        {
            if (double_open) // closing double quote
            {
                double_open = false;
                output[j++] = '\"';
            }
            else
                double_open = true;
            i++;
            continue;
        }
        if ((single_open || double_open) && (current != '\\' && current != ';'))
            output[j++] = current;
        else if (!single_open && !double_open && (current != '\\' && current != ';'))
            output[j++] = current;
        i++;
    }

    if (single_open) output[--j] = '\0';  // remove last single quote if it's unclosed
    if (double_open) output[--j] = '\0';  // remove last double quote if it's unclosed

    output[j] = '\0';
    return (output);
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