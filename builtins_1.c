/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:02:20 by x230              #+#    #+#             */
/*   Updated: 2023/06/07 14:30:01 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function that will check if a builtin is called
void	check_builtins(char *arg)
{
	if (!ft_strcmp(arg, "exit"))
		builtin_exit();

}

int builtin_exit(void)
{
    // add cleanupfunctionality before exiting the shell?
    exit(0);
}