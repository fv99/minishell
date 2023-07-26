/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:02:20 by x230              #+#    #+#             */
/*   Updated: 2023/07/26 16:23:18 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function that will check if a builtin is called
int	check_builtins(char **args, t_env *env)
{
	if (!ft_strcmp(args[0], "exit"))
		return(builtin_exit());
	if (!ft_strcmp(args[0], "cd"))
		return(builtin_cd(args));
	if (!ft_strcmp(args[0], "pwd"))
		return(builtin_pwd());
	if (!ft_strcmp(args[0], "echo"))
		return(builtin_echo(args));
	if (!ft_strcmp(args[0], "export"))
		return(builtin_export(args, &env));
	if (!ft_strcmp(args[0], "env"))
		return(builtin_env(env));
	if (!ft_strcmp(args[0], "unset"))
        return (builtin_unset(args, &env));
	return (0);
}

int builtin_exit(void)
{
    // add cleanup functionality before exiting the shell?
    exit(0);
}

int	builtin_cd(char **args)
{
	const char	*home_dir = getenv("HOME");
	char		*cmd;
		
	if (home_dir == NULL)
		you_fucked_up("Could not get home directory", -1);
	// if dir unspecified change to home
	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		if (chdir(home_dir) != 0)
			perror("cd");
		return(1);
	}
	else
	{
		if (args[1][0] == '~')
			cmd = builtin_cd_expand_home(args, (char *)home_dir);
		else
			cmd = ft_strdup(args[1]);
		if (chdir(cmd) != 0)
			perror("cd");
		free(cmd);
		return(1);
	}
	return (0);
}

// expands ~ to home dir
char *builtin_cd_expand_home(char **args, char *home_dir)
{
	char *expanded;

	expanded = malloc(strlen(home_dir) + strlen(args[1]) + 1);
	if (!expanded)
		you_fucked_up("failed to allocate in builtin_cd_expand_home", -1);
	ft_strcpy(expanded, home_dir);
	ft_strcat(expanded, args[1] + 1);
	// printf("expanded dir: %s\n", expanded);
	return(expanded);
}

int	builtin_pwd(void)
{
	char dir[1024]; // make bigger if needed

	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		perror("pwd");
		return (-1);
	}
	printf("%s\n", dir);
	return (1);
} 