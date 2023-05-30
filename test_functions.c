/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:59:43 by x230              #+#    #+#             */
/*   Updated: 2023/05/30 17:04:20 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// dont forget to free after
void test_parse_line(char **tokens)
{
    printf("Tokens found:\n");
    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (tokens[i] != NULL)
        {
            printf("%s\n", tokens[i]);
        }
        else
        {
            printf("Token %d is NULL\n", i);
        }
    }
}