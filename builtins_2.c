/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:43:54 by x230              #+#    #+#             */
/*   Updated: 2023/07/26 16:24:55 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ; // External reference to the environment variables array

int builtin_env(t_env *env)
{
	t_env *temp = env;
    while (temp != NULL) {
        printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
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
int builtin_export(char **args, t_env **env)
{
    if (args[1] == NULL) {
        fprintf(stderr, "export: expected argument\n");
        return (1);
    }

    // Extract key and value from args[1]
    char *key = strtok(args[1], "=");
    char *value = strtok(NULL, "=");

    // Create new node
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    if (new_node == NULL) {
        fprintf(stderr, "export: allocation error\n");
        return (1);
    }

    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = NULL;

    // Add new node to the end of the list
    if (*env == NULL) {
        // The list is empty
        *env = new_node;
    } else {
        // Find the last node
        t_env *last = *env;
        while (last->next != NULL) {
            last = last->next;
        }

        // Add the new node to the end of the list
        last->next = new_node;
    }
    return (1);
}

int builtin_unset(char **args, t_env **env)
{
    if (args[1] == NULL) {
        fprintf(stderr, "unset: expected argument\n");
        return (1);
    }

    char *key = args[1];

    // Special case: deleting the first node
    if (*env != NULL && strcmp((*env)->key, key) == 0) {
        t_env *temp = *env;
        *env = (*env)->next;
        free(temp->key);
        free(temp->value);
        free(temp);
        return (1);
    }

    // General case: deleting a node other than the first
    t_env *current = *env;
    while (current != NULL && current->next != NULL) {
        if (strcmp(current->next->key, key) == 0) {
            t_env *temp = current->next;
            current->next = current->next->next;
            free(temp->key);
            free(temp->value);
            free(temp);
            return (1);
        }
        current = current->next;
    }

    fprintf(stderr, "unset: variable not found\n");
    return (0);
}