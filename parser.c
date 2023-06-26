/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/26 16:48:52 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define ARG_SIZE 256 // sets malloc size, change if needed

/* 
    PARSER (not final)

    take input from lexer:
    char** = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, |, wc, -l, >, >, outfile, NULL}
    
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

// need to split this below 25 lines
t_parsed *fill_list(char **args)
{
    char        **cmds;
    t_parsed    *new;
    t_parsed    *head = NULL;
    t_parsed    *tail = NULL;
    t_ops       curr;
    int         i;
    int         j;

    cmds = malloc(sizeof(char *) * ARG_SIZE);
    i = 0;
    j = 0;
    while (args[i] != NULL)
    {
        curr = check_op(args[i]);
        if ((curr == RED_OUT && args[i + 1] != NULL) && check_op(args[i + 1]) == RED_OUT)
        {
            curr = RED_APP;     // for two consecutive >
            i++;
        }
        else if ((curr == RED_IN && args[i + 1] != NULL) && check_op(args[i + 1]) == RED_IN)
        {
            curr = HEREDOC;     // ditto for <
            i++;
        }
        if (curr != NONE)
        {
            cmds[j] = NULL;
            new = new_parser_node(cmds, curr);
            if (head == NULL)
                head = new;
            else
                tail->next = new;
            tail = new;
            cmds = malloc(sizeof(char *) * ARG_SIZE);
            j = 0;
        }
        else
            cmds[j++] = args[i];
        i++;
    }
    cmds[j] = NULL;
    new = new_parser_node(cmds, NONE);
    if (head == NULL)
        head = new;
    else
        tail->next = new;
    return (head);
}


t_parsed *new_parser_node(char **args, t_ops op)
{
    t_parsed *new = malloc(sizeof(t_parsed));
    new->args = args;
    new->op = op;
    new->next = NULL;
    return (new);
}

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
