/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:56:00 by phelebra          #+#    #+#             */
/*   Updated: 2023/08/02 14:30:09 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
	{
		fprintf(stderr, "export: allocation error\n");
		return (NULL);
	}
	new_node->key = strdup(key);
	new_node->value = strdup(value);
	new_node->next = NULL;
	return (new_node);
}

t_env	*find_env_key(t_env *env, char *key)
{
	t_env	*temp;

	temp = env;
	while (temp != NULL)
	{
		if (strcmp(temp->key, key) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*last;

	if (*env == NULL)
		*env = new_node;
	else
	{
		last = *env;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
}

int	builtin_export(char **args, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*temp;
	t_env	*new_node;

	if (args[1] == NULL)
	{
		fprintf(stderr, "export: expected argument\n");
		return (1);
	}
	key = strtok(args[1], "=");
	value = strtok(NULL, "=");
	temp = find_env_key(*env, key);
	if (temp != NULL)
	{
		free(temp->value);
		temp->value = strdup(value);
		return (1);
	}
	new_node = create_env_node(key, value);
	if (new_node == NULL)
		return (1);
	add_env_node(env, new_node);
	return (1);
}
