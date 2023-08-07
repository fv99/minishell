/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 16:43:48 by phelebra          #+#    #+#             */
/*   Updated: 2023/08/07 09:21:15 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define PATH_SEP ":"

extern int	g_status;

// Gets PATH environment variable and saves into path_env
// Loops through each directory in path_env separated by PATH_SEP
// Uses get_path_token function to get full path, returns it

char *get_path(char *cmd, t_env *env)
{
	int		cmd_len;
	char	*path_env;
	char	*path;

	path_env = NULL;
	cmd_len = ft_strlen(cmd);
	while (env != NULL)
	{
		if (ft_strncmp(env->key, "PATH", 4) == 0)
		{
			path_env = ft_strdup(env->value);
			break;
		}
		env = env->next;
	}
	if (path_env == NULL)
		return (NULL);
	path = get_path_token(cmd, path_env, cmd_len);
	//free(path_env); // Free the allocated memory for path_env
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
