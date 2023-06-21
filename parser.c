/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/21 13:31:51 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
    PARSER (not final)

    take input from lexer:
    char** = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, |, wc, -l, >, outfile, NULL}
    
    go over array, split according to redirect, use check op to set the op type,
    set outfile and infile according to operation
        parsed 1:
            **args = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, NULL}
            op = PIPE
            infile = 0;
            outfile = pipe;
            next = parsed 2;
        parsed 2:
            **args = {wc, -l, NULL}
            op = RED_OUT;
            infile = pipe (previous output)
            outfile = fd to open file 'outfile'
            next = NULL;
            
    NOTE
        << will be interpreted as {<, <} and vice versa for >>
        parser should handle this
*/

// checks for op type 
t_ops check_op(char *str)
{
	if (!strcmp(str, "|"))
		return PIPE;
	if (!strcmp(str, "<"))
		return RED_IN;
	if (!strcmp(str, ">"))
		return RED_OUT;
	if (!strcmp(str, ">>"))
		return RED_APP;
	if (!strcmp(str, "<<"))
		return HEREDOC;
	return NONE;
}

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
