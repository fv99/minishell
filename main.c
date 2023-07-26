/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:17:51 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/26 16:08:45 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

t_env *create_node(char *env_str) {
    t_env *node = malloc(sizeof(t_env));
    if (!node) return NULL;

    char *equal_sign = strchr(env_str, '=');
    if (!equal_sign) return NULL;

    int key_len = equal_sign - env_str;
    node->key = strndup(env_str, key_len);
    node->value = strdup(equal_sign + 1);
    node->next = NULL;
    
    return node;
}

t_env *create_list(char **envp) {
    t_env *head = NULL;
    t_env *tail = NULL;

    for (int i = 0; envp[i]; i++) {
        t_env *node = create_node(envp[i]);
        if (!node) continue; // handle error as needed

        if (!head) {
            head = node;
        } else {
            tail->next = node;
        }
        
        tail = node;
    }
    
    return head;
}

void free_list(t_env *env) {
    t_env *temp;

    while (env != NULL) {
        temp = env;
        env = env->next;

        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

int main(void) {
    g_status = 1;
	extern char **environ;

	t_env *env = create_list(environ);

    shell_loop(env);

	free_list(env);
    return (g_status);
}