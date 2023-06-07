/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:02:20 by x230              #+#    #+#             */
/*   Updated: 2023/06/07 14:46:51 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function that will check if a builtin is called
int	check_builtins(char **args)
{
	if (!ft_strcmp(args[0], "exit"))
		return(builtin_exit());
	if (!ft_strcmp(args[0], "cd"))
		return(builtin_cd(args));
	return (0);
}

int 	builtin_exit(void)
{
    // add cleanupfunctionality before exiting the shell?
    exit(0);
}

int	builtin_cd(char **args)
{
	const char *home_dir = getenv("HOME");
		
	if (home_dir == NULL)
		you_fucked_up("Could not get home directory");
	// if dir unspecified change to home
	if (args[1] == NULL)
	{
		if (chdir(home_dir) != 0)
			perror("cd");
		return(1);
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("cd");
		return(1);
	}
	return (0);
}