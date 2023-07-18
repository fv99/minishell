/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:24:28 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/07/18 13:48:28 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;


/* 
	goes through string, when $ is hit outside of '' 
	calls expand_arg to replace variable with its name
	updates the str_ptr pointer
*/
char	*expand_args(char **str_ptr, char **envp)
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
			str = expand_arg (str, i--, envp);
	}
	*str_ptr = str;
	return (str);
}

/* 
	when we hit $ in a string, find the name of variable
	get its value with get_arg
	creates new string with variable replaced by value
 */
char	*expand_arg(char *str, int counter, char **envp)
{
	char	*argname;
	char	*arg;
	int		argname_len;
	char	*newstr;

	argname_len = 0;
	while (!ft_strchr("\'\" ", str[counter + argname_len + 1]))
		argname_len++;
	argname = ft_substr (str, counter + 1, argname_len);
	if (strcmp(argname, "0") == 0)
		arg = ft_strdup("⛧minihell⛧");
	else if (strcmp(argname, "?") == 0)
		arg = ft_itoa(g_status % 255);
	else
		arg = get_arg(argname, envp);
	newstr = ft_calloc((ft_strlen(str) - argname_len - 1)
			+ ft_strlen(arg) + 1, sizeof(char));
	ft_strlcpy(newstr, str, counter + 1);
	if (arg)
		ft_strlcpy(ft_strchr(newstr, 0), arg, ft_strlen(arg) + 1);
	else
		counter++;
	ft_strlcpy(ft_strchr(newstr, 0),
		&str[counter + argname_len + 1],
		ft_strlen(&str[counter + argname_len + 1]) + 1);
	if (ft_strncmp(argname, "?", 2) == 0)
		free(arg);
	free(argname);
	free(str);
	return (newstr);
}

/* 
	returns value of variable name from environ
*/
char *get_arg(char *argname, char **envp)
{
    int i = 0;
    int argname_len = strlen(argname);

    while (envp[i])
    {
        int env_len = strlen(envp[i]);
        if (env_len >= argname_len && envp[i][argname_len] == '=' &&
            strncmp(envp[i], argname, argname_len) == 0)
            return &envp[i][argname_len + 1];
        i++;
    }
	argname = "";
    return argname;
}
