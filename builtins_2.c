/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/07/21 15:55:03 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ; // External reference to the environment variables array

int builtin_env()
{
	char **env_var = environ;

	while (*env_var != NULL)
	{
		printf("%s\n", *env_var);
		env_var++;
	}
	return (0);
}

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

// need to split under 25 lines
// doesnt work right now
int	builtin_export(char **args, char **envp)
{
	char	**env;
	char	*key;
	char	*val;
	char	*equal;
	char	*new_env;
	int		i;
	size_t	l;

	env = envp;
	i = 0;
	while (args[i] != NULL)
	{
		equal = ft_strchr(args[i], '='); // check if arg has format "KEY=VALUE"
		if (equal != NULL)
		{
			*equal = '\0';		// split arg into key and value
			key = args[i];
			val = equal + 1;

			l = strlen(key) + strlen(val) + 2;
			new_env = malloc(l);
			if (new_env == NULL)
            {
                perror("export");
                return (1);
            }
			ft_strcpy(new_env, key);
			ft_strcat(new_env, "=");
			ft_strcat(new_env, val);

			// update environment variable
			if (putenv(new_env) != 0)
			{
				perror("export");
				free(new_env);
				return (-1);
			}
			else
			{
				while(*env != NULL)
				{
					if (ft_strncmp(*env, key, strlen(key)) == 0)
					{
						printf("%s\n", *env);
						
					}
				}


			}
			i++;
		}
	}
	return (0);
} 

/*int bulitin_export(char **args) {
	// Duplicate the input string to avoid modifying the original
	char *var_copy = strdup(args);
	if (var_copy == NULL)
	{
		perror("Error duplicating variable");
		return -1;
			}

	// Find the equals sign that separates the variable name and value
	char *equals_sign = strchr(var_copy, '=');
	if (equals_sign == NULL)
	{
		fprintf(stderr, "Invalid variable format: %s\n", args);
		free(var_copy);
		return -1;
	}

	// Split the string into variable name and value parts
	*equals_sign = '\0';
	char *var_name = var_copy;
	char *var_value = equals_sign + 1;

	// Set the environment variable using the putenv function
	if (setenv(var_name, var_value, 1) != 0)
	{
		perror("Error setting environment variable");
		free(var_copy);
		return -1;
} 

	// Free the allocated memory
	free(var_copy);

	return 0;
}
*/