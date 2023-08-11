/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:46:54 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/08/11 13:42:46 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	you_fucked_up(char *msg, int status)
{
	g_status = status;
	printf("\tERROR status %i: %s\n", status, msg);
	exit(1);
}

void	free_array(char	**ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i] != NULL)
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

char	*ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = s2[i];
	return (s1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (s1 == NULL || s2 == NULL)
	{
		return (-1);
	}
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}
