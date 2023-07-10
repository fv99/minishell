/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:59:43 by x230              #+#    #+#             */
/*   Updated: 2023/07/10 16:56:00 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_tokenize(char* input, char **envp)
{
	char **result;
	int i;

	result = lexer(input, envp);
	if (result == NULL)
	{
		printf("Test failed: tokenizer returned NULL\n");
		return;
	}

	i = 0;
	while (result[i] != NULL)
	{
		printf("Token %d: %s\n", i, result[i]);
		++i;
	}

	for (i = 0; result[i] != NULL; i++)
		free(result[i]);
	free(result);
}

void test_parser(t_parsed *head) 
{
    t_parsed *current = head;
	int i = 0;
    while (current != NULL) {
        printf("\n\tCommand %d:\n", i);
        for (int i = 0; current->args[i] != NULL; i++) {
            printf("  %s\n", current->args[i]);
        }

        printf("Operation: ");
        switch (current->op) {
            case NONE: printf("NONE"); break;
            case PIPE: printf("PIPE"); break;
            case RED_IN: printf("RED_IN"); break;
            case RED_OUT: printf("RED_OUT"); break;
            case RED_APP: printf("RED_APP"); break;
            case HEREDOC: printf("HEREDOC"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");

        printf("infile: %i\n", current->infile);
        printf("outfile: %i\n", current->outfile);

        t_parsed *next = current->next;
        free(current->args);  // Free the array of arguments
        free(current);  // Free the node itself
        current = next;  // Move to the next node
		i++;
    }
}
