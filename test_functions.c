/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:59:43 by x230              #+#    #+#             */
/*   Updated: 2023/05/30 16:34:39 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// dont forget to free after
void test_parse_line(char *line)
{
    char **tokens = split_line(line);

    printf("Tokens found:\n");
    for (int i = 0; tokens[i] != NULL; i++)
    {
        printf("%s\n", tokens[i]);
    }
}
