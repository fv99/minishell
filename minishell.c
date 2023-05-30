/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:56:26 by x230              #+#    #+#             */
/*   Updated: 2023/05/30 17:04:37 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define LINE_SIZE 1024

// main loop to run the shell
void    shell_loop(void)
{
    char    *line;
    char    **args;
    int     status;

	status = 1;
	while (status)
	{
		line = readline("> ");
        if (!line) // if readline returns NULL, it means we hit an EOF (like Ctrl+D)
            break;
		// have call to save to history here
		args = split_line(line);
		test_parse_line(args);
		// set status according to command execution, implement this later
		free_array(args);
		free(line);
		status = 0;
	}
}