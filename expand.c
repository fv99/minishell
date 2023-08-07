/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:24:28 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/08/07 13:51:02 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

/* 
	goes through string, when $ is hit outside of '' 
	calls expand_arg to replace variable with its name
	updates the str_ptr pointer
*/
char	*expand_args(char **str_ptr, t_env *env)
{
	int		i;
	int		is_quotes;
	char	*str;

	str = *str_ptr;
	i = -1;
	is_quotes = 0;
	while (str[++i])
	{
		if ((str[i] == '\"' || str[i] == '\''))
		{
			if (is_quotes == 0 && str[i] == '\'')
				is_quotes = 1;
			else if (is_quotes == 0)
				is_quotes = 2;
			else if ((str[i] == '\'' && is_quotes == 1)
				|| (str[i] == '\"' && is_quotes == 2))
				is_quotes = 0;
		}
		else if (str[i] == '$' && is_quotes != 1)
			str = expand_arg (str, i--, env);
	}
	*str_ptr = str;
	return (str);
}

/* 
	when we hit $ in a string, find the name of variable
	get its value with get_arg
	creates new string with variable replaced by value
 */
t_arg	get_argname_and_len(char *str, int counter)
{
	t_arg	arg_data;

	arg_data.argname_len = 0;
	while (!ft_strchr("\'\" ", str[counter + arg_data.argname_len + 1]))
		arg_data.argname_len++;
	arg_data.argname = ft_substr(str, counter + 1, arg_data.argname_len);
	return (arg_data);
}

char	*determine_arg_and_create_str(char *str, int counter,
	t_arg arg_data, t_env *env)
{
	char	*arg;
	char	*newstr;

	if (strcmp(arg_data.argname, "0") == 0)
		arg = ft_strdup("⛧minihell⛧");
	else if (strcmp(arg_data.argname, "?") == 0)
		arg = ft_itoa(g_status % 255);
	else
		arg = get_arg(arg_data.argname, env);
	newstr = ft_calloc((ft_strlen(str) - arg_data.argname_len - 1)
			+ ft_strlen(arg) + 1, sizeof(char));
	ft_strlcpy(newstr, str, counter + 1);
	if (arg)
		ft_strlcpy(ft_strchr(newstr, 0), arg, ft_strlen(arg) + 1);
	else
		counter++;
	ft_strlcpy(ft_strchr(newstr, 0), &str[counter + arg_data.argname_len + 1],
		ft_strlen(&str[counter + arg_data.argname_len + 1]) + 1);
	if (ft_strncmp(arg_data.argname, "?", 2) == 0)
		free(arg);
	return (newstr);
}

char	*expand_arg(char *str, int counter, t_env *env)
{
	t_arg	arg_data;
	char	*newstr;

	arg_data = get_argname_and_len(str, counter);
	newstr = determine_arg_and_create_str(str, counter, arg_data, env);
	free(arg_data.argname);
	free(str);
	return (newstr);
}

/* 
	returns value of variable name from environ
*/
char *get_arg(char *argname, t_env *env)
{
	while (env != NULL)
	{
		if (strcmp(env->key, argname) == 0)
			return env->value;
		env = env->next;
	}
	return "";
}
