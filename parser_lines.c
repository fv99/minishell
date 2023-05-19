/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:38:20 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/05/19 12:49:18 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
splits with split

	TODO:
	- add handling for '' and ""
	- add call for saving to history somewhere
 */

char	**split_line(char *line)
{
	char	**args;

	// '' and "" handling goes here
	args = ft_split(line, ' ');
	return (args);
}

