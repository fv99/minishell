/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:02:20 by x230              #+#    #+#             */
/*   Updated: 2023/07/31 17:13:59 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function that will check if a builtin is called
int	check_builtins(t_parsed *parsed_cmd, t_env *env)
{
	char **args = parsed_cmd->args;

	if (!ft_strcmp(args[0], "cd"))
		return(builtin_cd(args));
	if (!ft_strcmp(args[0], "echo"))
		return(builtin_echo(parsed_cmd));
	if (!ft_strcmp(args[0], "export"))
		return(builtin_export(args, &env));
	if (!ft_strcmp(args[0], "env"))
		return(builtin_env(parsed_cmd, env));
	if (!ft_strcmp(args[0], "unset"))
        return (builtin_unset(args, &env));
	if (!ft_strcmp(args[0], "pwd"))
		return(builtin_pwd(parsed_cmd));
	if (!ft_strcmp(args[0], "exit"))
		return(builtin_exit());
	
	return (0);
}

int builtin_exit(void)
{
    // add cleanup functionality before exiting the shell?
    exit(0);
}

int builtin_cd(char **args) {
    const char *home_dir = getenv("HOME");
    const char *oldpwd = getenv("OLDPWD"); // Use getenv for OLDPWD
    char *cmd;

    if (home_dir == NULL)
        you_fucked_up("Could not get home directory", -1);

    // Store the current working directory to update OLDPWD
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    // if dir unspecified change to home
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        if (chdir(home_dir) != 0)
            perror("cd");
        else
            setenv("OLDPWD", cwd, 1); // Update OLDPWD
        return 1;
    }

    // Handle previous directory case
    if (strcmp(args[1], "-") == 0) {
        if (oldpwd != NULL) {
            if (chdir(oldpwd) != 0)
                perror("cd");
            else
                setenv("OLDPWD", cwd, 1); // Update OLDPWD
        } else {
            you_fucked_up("OLDPWD is not set", -1);
        }
        return 1;
    }

    // Handle expansion of '~' to the home directory
    if (args[1][0] == '~')
        cmd = builtin_cd_expand_home(&args[1], (char *)home_dir);
    else
        cmd = strdup(args[1]);

    if (chdir(cmd) != 0)
        perror("cd");
    else
        setenv("OLDPWD", cwd, 1); // Update OLDPWD

    free(cmd);
    return 1;
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

int	builtin_pwd(t_parsed *parsed_cmd)
{
	char dir[1024]; // make bigger if needed

	int save_stdout = dup(STDOUT_FILENO);
	int outfile_fd = parsed_cmd->outfile;
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

	// restore STDOUT
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return (1);
}