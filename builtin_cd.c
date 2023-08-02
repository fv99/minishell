/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 11:27:31 by phelebra          #+#    #+#             */
/*   Updated: 2023/08/02 13:36:07 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd_home(const char *home_dir, char *cwd)
{
	if (chdir(home_dir) != 0)
		perror("cd");
	else
		setenv("OLDPWD", cwd, 1);
}

void	handle_cd_oldpwd(const char *oldpwd, char *cwd)
{
	if (oldpwd != NULL)
	{
		if (chdir(oldpwd) != 0)
			perror("cd");
		else
			setenv("OLDPWD", cwd, 1);
	}
	else
	{
		you_fucked_up("OLDPWD is not set", -1);
	}
}

void	handle_cd_cmd(char *cmd, char *cwd)
{
	if (chdir(cmd) != 0)
		perror("cd");
	else
		setenv("OLDPWD", cwd, 1);
}

char	*prepare_cd_cmd_and_get_cwd(char **args, const char *home_dir,
	char **cwd)
{
	char	*cmd;

	cmd = NULL;
	*cwd = malloc(1024 * sizeof(char));
	if (*cwd == NULL)
		return (NULL);
	if (getcwd(*cwd, 1024) == NULL)
	{
		perror("getcwd");
		free(*cwd);
		*cwd = NULL;
		return (NULL);
	}
	if (args[1] != NULL)
	{
		if (args[1][0] == '~')
			cmd = builtin_cd_expand_home(&args[1], (char *)home_dir);
		else
			cmd = strdup(args[1]);
	}
	return (cmd);
}

int	builtin_cd(char **args)
{
	const char	*home_dir = getenv("HOME");
	const char	*oldpwd = getenv("OLDPWD");
	char		*cmd;
	char		*cwd;

	cmd = NULL;
	cwd = NULL;
	if (home_dir == NULL)
		you_fucked_up("Could not get home directory", -1);
	if (args[1] == NULL)
	{
		cwd = getcwd(cwd, 1024);
		handle_cd_home(home_dir, cwd);
	}
	else
	{
		cmd = prepare_cd_cmd_and_get_cwd(args, home_dir, &cwd);
		if (cwd == NULL || cmd == NULL)
			return (free(cwd), free(cmd), 1);
		if (strcmp(args[1], "-") == 0)
			handle_cd_oldpwd(oldpwd, cwd);
		else
			handle_cd_cmd(cmd, cwd);
	}
	return (free(cwd), free(cmd), 1);
}
