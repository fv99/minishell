/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:17:51 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/26 10:12:11 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

int main(void) {
    g_status = 1;

    extern char **environ;
    // Find the number of environment variables in the original environ array
    int num_env_vars = 0;
    while (environ[num_env_vars] != NULL) {
        num_env_vars++;
    }

    // Allocate memory for the cust_env array
    char **cust_env = (char **)malloc((num_env_vars + 1) * sizeof(char *));
    if (cust_env == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Calculate the total length required for the cust_env array
    int total_length = 0;
    char **env_ptr = environ;
    while (*env_ptr != NULL) {
        total_length += strlen(*env_ptr) + 1; // Include the null-terminator
        env_ptr++;
    }

    // Allocate a single buffer to store the concatenated environment variables
    char *env_buffer = (char *)malloc(total_length * sizeof(char));
    if (env_buffer == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Copy content from the environ array to the cust_env array
	env_ptr = environ;
	while (*env_ptr != NULL) {
    	cust_env[env_ptr - environ] = strdup(*env_ptr);
    	if (cust_env[env_ptr - environ] == NULL) {
        	perror("Memory allocation failed");
        	return 1;
    	}
    	env_ptr++;
	}


    // The cust_env array should be null-terminated
    cust_env[num_env_vars] = NULL;

    printf("Before shell_loop - environ2 content:\n");
    env_ptr = cust_env;
    while (*env_ptr != NULL) {
        printf("%s\n", *env_ptr);
        env_ptr++;
    }

    shell_loop(cust_env);

    printf("After shell_loop - environ2 content:\n");
    env_ptr = cust_env;
    while (*env_ptr != NULL) {
        printf("%s\n", *env_ptr);
        env_ptr++;
    }

    // Don't forget to free the memory when you are done using the cust_env array and the env_buffer.
    free(env_buffer);

	// Free the environment variables
	for (int i = 0; i < num_env_vars; i++) {
    	free(cust_env[i]);
	}
	// Free the cust_env array
    free(cust_env);

    return (g_status);
}