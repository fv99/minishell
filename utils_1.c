/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:46:54 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/05/30 16:19:42 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	you_fucked_up(char *msg)
{
	printf("\tERROR: %s\n", msg);
	exit(1);
}

void	free_array(char	**ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	l;

	i = 0;
	l = ft_strlen(dest);
	while (src[i] != '\0')
	{
		dest[i + l] = src[i];
		i++;
	}
	dest[i + l] = '\0';
	return (dest);
}

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
