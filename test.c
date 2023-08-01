/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 17:06:28 by x230              #+#    #+#             */
/*   Updated: 2023/08/01 15:42:00 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if character is a delimiter
bool    is_delimiter(char c, const char *delims)
{
    while (*delims != '\0')
    {
        if (c == *delims)
            return (true);
        delims++;
    }
    return (false);
}

// Updated strtok function
char    *ft_strtok(char *str, const char *delims)
{
    static char *stock;
    char        *ptr;
    bool        found_token;

    stock = NULL;
    ptr = NULL;
    found_token = false;
    
    if (str != NULL)
        stock = str;
    
    while (*stock != '\0')
    {
        if (!found_token && !is_delimiter(*stock, delims))
        {
            found_token = true;
            ptr = stock;
        }
        else if (found_token && is_delimiter(*stock, delims))
        {
            *stock = '\0';
            stock += 1;
            break ;
        }
        stock++;
    }
    return (ptr);
}

char    *ft_strstr(char *str, char *to_find)
{
    int i;
    int j;

    i = 0;
    if (to_find[0] == '\0')
        return (str);
    while (str[i] != '\0')
    {
        j = 0;
        while (str[i + j] != '\0' && str[i + j] == to_find[j])
        {
            if (to_find[j + 1] == '\0')
                return (&str[i]);
            ++j;
        }
        ++i;
    }
    return (0);
}

