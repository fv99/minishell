/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/10 18:19:43 by fvonsovs         ###   ########.fr       */
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
        curr = check_op(args[i]);                           // checks operation
        if(curr != NONE) {
            if (curr == PIPE)                               // if found a pipe operation
            {
                cmds[j] = NULL;                             // ends current command
                tail = add_new_node(cmds, curr, &head, &tail); // adds a new node to linked list
                tail->outfile = 1;
                cmds = malloc(sizeof(char *) * ARG_SIZE);   // allocates new command array
                j = 0;
            }
            else if (curr == RED_OUT || curr == RED_IN || curr == RED_APP || curr == HEREDOC)
            {
                update_current_operation(&curr, args, &i, tail); // checks for operations sets infiles
                handle_redirection(tail, curr, args[++i]); // handle redirection and update file descriptors
            }
        }
        else {
            cmds[j++] = args[i];                            // adds argument to current command, if no operation
        }
        i++;
    }
    cmds[j] = NULL;
    add_new_node(cmds, NONE, &head, &tail);                 // adds last command to the list with op NONE
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
    {
        node = get_outfile1(node, args, i);
    } 
    else if ((*curr == RED_IN && args[*i + 1] != NULL) && check_op(args[*i + 1]) == RED_IN) 
    {
        *curr = HEREDOC;
        node = get_infile2(node, args, i);
    } 
    else if ((*curr == RED_IN && args[*i + 1] != NULL) && check_op(args[*i + 1]) == NONE) 
    {
        node = get_infile1(node, args, i);
    }
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

int open_file(char *file, t_ops op)
{
    if (op == RED_OUT || op == RED_APP)
        return open(file, op == RED_APP ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    else if (op == RED_IN)
        return open(file, O_RDONLY);
    return -1;
}

void handle_redirection(t_parsed *node, t_ops op, char *file)
{
    int flags[2];
    if (op == RED_OUT) {
        flags[0] = 1;
        flags[1] = 0;
    } else if (op == RED_APP) {
        flags[0] = 1;
        flags[1] = 1;
    } else if (op == RED_IN) {
        flags[0] = 0;
        flags[1] = 0;
    }

    int fd = get_fd((op == RED_OUT || op == RED_APP) ? node->outfile : node->infile, file, flags);
    if (fd != -1)
    {
        if (op == RED_OUT || op == RED_APP)
            node->outfile = fd; // set outfile to the file descriptor that corresponds to the opened file
        else if (op == RED_IN)
            node->infile = fd;  // set infile to the file descriptor that corresponds to the opened file
    }
    // Handle fd == -1 case if necessary
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