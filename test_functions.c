/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:59:43 by x230              #+#    #+#             */
/*   Updated: 2023/06/13 12:40:08 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// wont pass norme obviously
void test_cmd_parser(command *cmd)
{
    while (cmd != NULL) 
    {
        int i;

        // print command arguments
        printf("Command arguments: ");
        for (i = 0; cmd->args[i] != NULL; i++)
        {
            printf("%s ", cmd->args[i]);
        }
        printf("\n");

        // print command operation
        printf("Command operation: ");
        switch (cmd->op) {
            case NONE: 
                printf("NONE");
                break;
            case PIPE:
                printf("PIPE");
                break;
            case RED_IN: 
                printf("RED_IN");
                break;
            case RED_OUT: 
                printf("RED_OUT");
                break;
            case RED_APP: 
                printf("RED_APP");
                break;
            case HEREDOC: 
                printf("HEREDOC");
                break;
        }
        printf("\n");
        cmd = cmd->next;
    }
    printf("No more commands.\n");
}
