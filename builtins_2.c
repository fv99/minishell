/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/07/24 14:42:47 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ; // External reference to the environment variables array

int builtin_env(char ***envp)
{
	char **env_ptr = *envp;
	while (*env_ptr != NULL)
	{
		printf("%s\n", *env_ptr);
		env_ptr++;
	}
	return (1);
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
// export is able to add key and value into the array, but for some reason I dont know, after calling env again it is not stored there :D
int builtin_export(char **args, char ***envp) {
    printf("Content of **args array: ");
    char **arg_ptr = args;
    while (*arg_ptr != NULL) {
        printf("%s ", *arg_ptr);
        arg_ptr++;
    }
    printf("\n");

    char *key;
    char *val;
    char *equal;
    char *new_env;

    equal = strchr(args[1], '='); // check if arg has format "KEY=VALUE"
    if (equal != NULL) {
        *equal = '\0'; // split arg into key and value
        key = args[1];
        val = equal + 1;

        size_t l = strlen(key) + strlen(val) + 2;
        new_env = malloc(l);
        if (new_env == NULL) {
            perror("export");
            return 1;
        }

        ft_strcpy(new_env, key);
        ft_strcat(new_env, "=");
        ft_strcat(new_env, val);

        // Find the number of environment variables in the envp array
        int num_env_vars = 0;
        while ((*envp)[num_env_vars] != NULL) {
            num_env_vars++;
        }

        // Allocate memory for the updated envp array, including the new environment variable
        char **updated_envp = malloc((num_env_vars + 2) * sizeof(char *));
        if (updated_envp == NULL) {
            perror("export");
            free(new_env);
            return 1;
        }

        // Copy the existing environment variables to the updated envp array using pointer arithmetic
        char **env_ptr = *envp;
        char **updated_env_ptr = updated_envp;
        while (*env_ptr != NULL) {
            *updated_env_ptr = *env_ptr;
            env_ptr++;
            updated_env_ptr++;
        }

        // Add the new environment variable to the updated envp array
        *updated_env_ptr = new_env;
        *(updated_env_ptr + 1) = NULL;

        // Restore the original value of args[1]
        *equal = '=';

        // Update the envp pointer to point to the updated envp array
        *envp = updated_envp;
    }

    printf("Updated content of **envp array: ");
    char **env_ptr = *envp;
    while (*env_ptr != NULL) {
        printf("%s ", *env_ptr);
        env_ptr++;
    }
    printf("\n");

    return 1;
}

// Helper function to remove an element from an array of strings
void remove_element(char **array, int index) {
    int i;
    for (i = index; array[i] != NULL; i++) {
        array[i] = array[i + 1];
    }
}

int builtin_unset(char **args, char ***envp) {
    if (args == NULL || args[1] == NULL) {
        fprintf(stderr, "unset: Missing argument\n");
        return 1;
    }

    if (*envp == NULL) {
        fprintf(stderr, "Environment variables array is NULL\n");
        return 1;
    }

    char **env_ptr = *envp;
    int index = 0;
    while (env_ptr[index] != NULL) {
        char *equal = strchr(env_ptr[index], '=');
        if (equal != NULL) {
            *equal = '\0'; // Split key from value
            if (strcmp(env_ptr[index], args[1]) == 0) {
                //free(env_ptr[index]); // Free the key=value string
                remove_element(env_ptr, index); // Remove the element from the array
                break;
            }
            *equal = '='; // Restore the original string
        }
        index++;
    }

    return 1;
}