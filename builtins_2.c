/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/06/07 15:50:08 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// make it so it interprets "" as one argument
int	builtin_echo(char **args)
{
	bool	n_opt;
	int		i;

	n_opt = false;
	i = 1;

	if (args[i] != NULL && strcmp(args[i], "-n") == 0)
	{	
		n_opt = true;
		i++;
	}
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
            printf(" ");
		i++;
	}
	if (!n_opt)
		printf("\n");
	return (1);
}