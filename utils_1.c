/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:46:54 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/05/19 12:48:40 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	you_fucked_up(char *msg)
{
	printf("\tERROR: %s\n", msg);
	exit(1);
}

// Gets PATH environment variable and saves into path_env
// Loops through each directory in path_env separated by PATH_SEP
// Uses get_path_token function to get full path, returns it
char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path_env;
	char	*path;
	int		cmd_len;

	i = 0;
	cmd_len = ft_strlen(cmd);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = ft_strdup(envp[i] + 5);
			break ;
		}
		i++;
	}
	path = get_path_token(cmd, path_env, cmd_len);
	return (path);
}

// Searches through each path in the PATH environment variable
// Construct the full path to the command
// Uses access() to check if path is executable
// Returns it if it is
char	*get_path_token(char *cmd, char *path_env, int cmd_len)
{
	char	*path_token;
	char	*path;

	path_token = ft_strtok(path_env, PATH_SEP);
	while (path_token != NULL)
	{
		path = ft_calloc(ft_strlen(path_token) + cmd_len + 2, 1);
		ft_strcat(path, path_token);
		ft_strcat(path, "/");
		ft_strcat(path, cmd);
		if (access (path, X_OK) == 0)
		{
			free(path_env);
			return (path);
		}
		free(path);
		path_token = ft_strtok(NULL, PATH_SEP);
	}
	free(path_env);
	return (NULL);
}
