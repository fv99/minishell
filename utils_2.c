/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 17:06:28 by x230              #+#    #+#             */
/*   Updated: 2023/06/12 13:07:04 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if character is a delimiter
bool is_delimiter(char c, const char *delims)
{
    while (*delims != '\0')
	{
        if (c == *delims)
            return true;
        delims++;
    }
    return false;
}

// Updated strtok function
char *ft_strtok(char *str, const char *delims)
{
    static char *stock = NULL;
    char *ptr = NULL;
    bool found_token = false;

    if (str != NULL)
        stock = str;
    while (*stock != '\0') {
        if (!found_token && !is_delimiter(*stock, delims)) 
		{
            found_token = true;
            ptr = stock;
        } 
		else if (found_token && is_delimiter(*stock, delims)) 
		{
            *stock = '\0';
            stock += 1;
            break;
        }
        stock++;
    }
    return (ptr);
}

// function to free commands struct
void    free_cmds(command **cmds)
{
    int i;

    i = 0;
    if (cmds == NULL)
        return ;
    while (cmds[i])
    {
        free_array(cmds[i]->args);
        free(cmds[i]);
        i++;
    }
    free(cmds);
}