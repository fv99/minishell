/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:17:51 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/08/01 15:35:50 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

t_env	*create_node(char *env_str)
{
	char	*equal_sign;
	int		key_len;
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal_sign = strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	key_len = equal_sign - env_str;
	node->key = strndup(env_str, key_len);
	node->value = strdup(equal_sign + 1);
	node->next = NULL;
	return (node);
}

t_env	*create_list(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_node(envp[i]);
		if (!node)
		{
			i++;
			continue ;
		}
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	return (head);
}

void	free_list(t_env *env)
{
	t_env	*temp;

	while (env != NULL)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

int	main(void)
{
	extern char	**environ;
	t_env		*env;

	g_status = 1;
	env = create_list(environ);
	shell_loop(env);
	free_list(env);
	return (g_status);
}
