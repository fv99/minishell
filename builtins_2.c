/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/07/26 10:34:18 by phelebra         ###   ########.fr       */
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
int builtin_export(char **args, char ***envp, int *num_env_vars) {
    printf("Content of **args array: ");
    char **arg_ptr = args;
    while (*arg_ptr != NULL) {
        printf("%s ", *arg_ptr);
        arg_ptr++;
    }
    printf("\n");

    char *key;
    char *equal;
    char *arg_dup = strdup(args[1]); // Duplicate the argument to avoid modification

    equal = strchr(arg_dup, '=');
    if (equal != NULL) {
        *equal = '\0'; // split arg into key and value
        key = arg_dup;

        // Search for the key in the existing environment variables
        int key_index = -1;
        for (int i = 0; i < *num_env_vars; i++) {
            if (strncmp(key, (*envp)[i], strlen(key)) == 0 && (*envp)[i][strlen(key)] == '=') {
                key_index = i;
                break;
            }
        }

        // Update the existing environment variable or add a new one
        if (key_index >= 0) {
            // Update the existing environment variable
            free((*envp)[key_index]); // Free the old value
            (*envp)[key_index] = strdup(args[1]); // Duplicate the new value
        } else {
            // Add a new environment variable
            (*num_env_vars)++;

            // Resize the envp array
            *envp = realloc(*envp, (*num_env_vars + 1) * sizeof(char *));
            if (*envp == NULL) {
                perror("export");
                free(arg_dup);
                return 1;
            }

            // Add the new environment variable to the end of the envp array
            (*envp)[*num_env_vars - 1] = strdup(args[1]);
            (*envp)[*num_env_vars] = NULL; // Null-terminate the array
        }
    }

    free(arg_dup); // Free the duplicated argument

    printf("Updated content of **envp array: ");
    char **env_ptr = *envp;
    while (*env_ptr != NULL) {
        printf("%s ", *env_ptr);
        env_ptr++;
    }
    printf("%d\n", *num_env_vars);
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
                free(env_ptr[index]); // Free the key=value string
                remove_element(env_ptr, index); // Remove the element from the array
                break;
            }
            *equal = '='; // Restore the original string
        }
        index++;
    }

    return 1;
}