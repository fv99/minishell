/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 15:59:43 by x230              #+#    #+#             */
/*   Updated: 2023/06/20 16:12:39 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_tokenize(char* input, char **envp)
{
	char **result;
	int i;

	result = tokenize(input, " ", envp);
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
