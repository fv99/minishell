/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/12 15:44:18 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define ARG_SIZE 256 // sets malloc size, change if needed

/* 
    PARSER

    take input from lexer:
    char** = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, |, wc, -l, >, >, outfile, NULL}
    
    go over array, split according to redirect, use check op to set the op type
        parsed 0:
            **args = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, NULL}
            op = PIPE
            infile = 0 (default)
            outfile = 3 (pipe write end)
            next = parsed 1;
        parsed 1:
            **args = {NULL}
            op = NONE;
            infile = 4 (pipe read end)
            outfile = 5 (fd corresponding to open file 'outfile')
            next = NULL;
            
    NOTE
        << will be interpreted as {<, <} and vice versa for >>
        parser should handle this

    TODO
        handle closing file descriptors after we are done with them
*/
t_parsed *fill_list(char **args)
{
    char        **cmds;
    t_parsed    *head = NULL;
    t_parsed    *tail = NULL;
    t_ops       curr;
    int         i = 0;
    int         j = 0;

    cmds = malloc(sizeof(char *) * ARG_SIZE);               // allocates new command array
    while (args[i] != NULL)
    {
        curr = check_op(args[i]); // Update current operation
        if (curr != NONE)
        {
            cmds[j] = NULL; // Ends current command
            tail = add_new_node(cmds, curr, &head, &tail); // Adds a new node to linked list
            cmds = malloc(sizeof(char *) * ARG_SIZE); // Allocates new command array
            j = 0;
            if (curr == PIPE) // If found a pipe operation
                tail->outfile = 3; // Pipe output to next command
            else if (curr == RED_OUT || curr == RED_IN || curr == RED_APP || curr == HEREDOC)
                update_current_operation(&curr, args, &i, tail); // Checks for operations and sets infiles
        }
        else
            cmds[j++] = args[i]; // Adds argument to current command, if no operation
        i++;
    }
    if (j != 0) // If the last command is not empty
    {
        cmds[j] = NULL; // Ends last command
        tail = add_new_node(cmds, NONE, &head, &tail); // Adds last node to linked list
    }
    tail->next = NULL; // Ends the list
    // free(args);
    return (head);
}


// checks for heredoc and append, needed for fill_list
void update_current_operation(t_ops *curr, char **args, int *i, t_parsed *node) 
{
    if ((*curr == RED_OUT && args[*i + 1] != NULL) && check_op(args[*i + 1]) == RED_OUT) 
    {
        *curr = RED_APP;
        node = get_outfile2(node, args, i);
    } 
    else if ((*curr == RED_OUT && args[*i + 1] != NULL) && check_op(args[*i + 1]) == NONE) 
        node = get_outfile1(node, args, i);
    else if ((*curr == RED_IN && args[*i + 1] != NULL) && check_op(args[*i + 1]) == RED_IN) 
    {
        *curr = HEREDOC;
        node = get_infile2(node, args, i);
    } 
    else if ((*curr == RED_IN && args[*i + 1] != NULL) && check_op(args[*i + 1]) == NONE) 
        node = get_infile1(node, args, i);
}

// adds new node to linked list with current commands and operations
t_parsed *add_new_node(char **cmds, t_ops curr, t_parsed **head, t_parsed **tail) 
{
    t_parsed *new = new_parser_node(cmds, curr);
    if (*head == NULL)
        *head = new;
    else
        (*tail)->next = new;
    return new;
}

t_parsed *new_parser_node(char **args, t_ops op)
{
    t_parsed *new = malloc(sizeof(t_parsed));
    new->args = args;
    new->op = op;
    new->infile = STDIN_FILENO;
    new->outfile = STDOUT_FILENO;
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

/* // strips quotes from src string
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
 */