/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/06/10 19:06:43 by x230             ###   ########.fr       */
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
