/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:17:51 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/05/19 12:52:27 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;
	char	**args;
	char	*path;
	int		status;

	status = 1;
	while (status)
	{
		printf("> ");
		line = readline(NULL);
		args = split_line(line);
		// execute function goes here which sets status val
		// should always return 1
	}
	return (0);
}