/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 12:36:59 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/21 13:38:16 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
	following functions called in lexer()
	they tokenize <|> which is later joined with the rest of the args
 */
char **tokenize_opts(char *src, char *delims)
{
	t_lexer	lex;
	char	**ret;
	int		n;

	lex.char_i = 0;
	lex.word_n = 0;
	lex.token_n = 0;
	lex.quotes = 0;
	lex.quote_c = 0;
	if (!src)
		return (NULL);
	n = count_words_opts(src, delims, &lex);
	if (n == -1)
		return (NULL);
	ret = malloc(sizeof(char *) * (n + 1));
	if (ret == NULL)
		return (NULL);
	fill_array_opts(ret, src, delims, &lex);
	ret[n] = NULL;
	return (ret);
}

/* 
	modified count_words, increases the word count (word_n) at the start of each loop, 
	regardless of whether the character at the current position (char_i) is a delimiter
 */
int	count_words_opts(char *s, char *delims, t_lexer *lex)
{
	lex->quotes = 0;
	lex->quote_c = 0;

	while (s[lex->char_i] != '\0')
	{
		lex->word_n++;
		if (!ft_strchr(delims, s[lex->char_i]))
		{
			while ((!ft_strchr(delims, s[lex->char_i]) || lex->quotes || lex->quote_c) && s[lex->char_i] != '\0')
			{
				lex->quotes = (lex->quotes + (!lex->quote_c && s[lex->char_i] == '\'')) % 2;
				lex->quote_c = (lex->quote_c + (!lex->quotes && s[lex->char_i] == '\"')) % 2;
				lex->char_i++;
			}
			if (lex->quotes || lex->quote_c)
				return (-1);
		}
		else
			lex->char_i++;
	}
	return (lex->word_n);
}

// modified fill_array, doesnt check for delimiter
char	**fill_array_opts(char **ret, char *s, char *delims, t_lexer *lex)
{
	lex->quotes = 0;
	lex->quote_c = 0;
	lex->char_i = 0;

	while (s[lex->char_i] != '\0')
	{
		lex->word_n = lex->char_i;
		if (!ft_strchr(delims, s[lex->char_i]))
		{
			while ((!ft_strchr(delims, s[lex->char_i]) || lex->quotes || lex->quote_c) && s[lex->char_i])
			{
				lex->quotes = (lex->quotes + (!lex->quote_c && s[lex->char_i] == '\'')) % 2;
				lex->quote_c = (lex->quote_c + (!lex->quotes && s[lex->char_i] == '\"')) % 2;
				lex->char_i++;
			}
		}
		else
			lex->char_i++;
		ret[lex->token_n++] = ft_substr(s, lex->word_n, lex->char_i - lex->word_n);
	}
	return (ret);
}
