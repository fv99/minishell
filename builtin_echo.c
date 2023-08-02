/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phelebra <xhelp00@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 14:01:36 by phelebra          #+#    #+#             */
/*   Updated: 2023/08/02 14:16:27 by phelebra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_outfile_redirection(t_parsed *parsed_cmd)
{
	int	outfile_fd;

	outfile_fd = parsed_cmd->outfile;
	if (outfile_fd != STDOUT_FILENO)
	{
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
	}
}

void	print_arguments(char **args, int index, bool n_opt)
{
	while (args[index] != NULL)
	{
		printf("%s", args[index]);
		if (args[index + 1] != NULL)
			printf(" ");
		index++;
	}
	if (!n_opt)
		printf("\n");
}

int	builtin_echo(t_parsed *parsed_cmd)
{
	bool	n_opt;
	int		i;
	char	**args;
	int		save_stdout;

	n_opt = false;
	i = 1;
	args = parsed_cmd->args;
	save_stdout = dup(STDOUT_FILENO);
	handle_outfile_redirection(parsed_cmd);
	if (args[i] != NULL && strcmp(args[i], "-n") == 0)
	{	
		n_opt = true;
		i++;
	}
	print_arguments(args, i, n_opt);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return (1);
}
