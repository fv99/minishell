/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x230 <x230@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:18:18 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/05/30 17:06:56 by x230             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// structs go here



// parser_lines.c

char	**split_line(char *line);

// utils_1.c

int		you_fucked_up(char *msg);

void	free_array(char	**ptr);

char	*ft_strcat(char *dest, char *src);

char    *ft_strcpy(char *s1, char *s2);

// utils_2.c

bool	is_delimiter(char c, const char *delims);

char	*ft_strtok(char *str, const char *delims);

// minishell.c

void    shell_loop(void);

// test_functions.c

void 	test_parse_line(char **tokens);

#endif