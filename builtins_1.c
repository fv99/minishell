/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:02:20 by x230              #+#    #+#             */
/*   Updated: 2023/08/02 11:35:29 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function that will check if a builtin is called
int	check_builtins(t_parsed *parsed_cmd, t_env *env)
{
	char	**args;

	args = parsed_cmd->args;
	if (!ft_strcmp(args[0], "cd"))
		return (builtin_cd(args));
	if (!ft_strcmp(args[0], "echo"))
		return (builtin_echo(parsed_cmd));
	if (!ft_strcmp(args[0], "export"))
		return (builtin_export(args, &env));
	if (!ft_strcmp(args[0], "env"))
		return (builtin_env(parsed_cmd, env));
	if (!ft_strcmp(args[0], "unset"))
		return (builtin_unset(args, &env));
	if (!ft_strcmp(args[0], "pwd"))
		return (builtin_pwd(parsed_cmd));
	if (!ft_strcmp(args[0], "exit"))
		return (builtin_exit());
	return (0);
}

int	builtin_exit(void)
{
	exit(0);
}

// expands ~ to home dir
char	*builtin_cd_expand_home(char **args, char *home_dir)
{
	char	*expanded;

	expanded = malloc(strlen(home_dir) + strlen(args[1]) + 1);
	if (!expanded)
		you_fucked_up("failed to allocate in builtin_cd_expand_home", -1);
	ft_strcpy(expanded, home_dir);
	ft_strcat(expanded, args[1] + 1);
	return (expanded);
}

int	builtin_pwd(t_parsed *parsed_cmd)
{
	char	dir[1024];
	int		save_stdout;
	int		outfile_fd;

	save_stdout = dup(STDOUT_FILENO);
	outfile_fd = parsed_cmd->outfile;
	if (outfile_fd != STDOUT_FILENO)
	{
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
	}
	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		perror("pwd");
		return (-1);
	}
	printf("%s\n", dir);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return (1);
}
