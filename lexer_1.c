/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvonsovs <fvonsovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 15:01:41 by fvonsovs          #+#    #+#             */
/*   Updated: 2023/06/21 13:38:22 by fvonsovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
	LEXER

	should take input such as this:
		char * = {cmd1 "blah blah" whats 'up 'you? $USER |wc -l > out}
	
	step 1: separate string according to spaces, take quotes into account
		char ** = {cmd1, "blah blah", whats, 'up 'you, $USER, |wc, -l, >outfile, NULL}

	step 2: expand variables
		char ** = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, |wc, -l, >outfile, NULL}

	step 3: split opts such as | < > << >> outside of quotes
		char ** = {cmd1, "blah blah", whats, 'up 'you, fvonsovs, |, wc, -l, >, outfile, NULL}

    NOTE
        << will be interpreted as {<, <} and vice versa for >>
        parser should handle this
*/
char	**lexer(char *s, char **envp)
{
    char **tokenized;
    char **opt_split;
    int i;

    tokenized = tokenize(s, " ");
    i = -1;
    while (tokenized && tokenized[++i])
    {
        tokenized[i] = expand_args(&tokenized[i], envp);
        opt_split = tokenize_opts(tokenized[i], "<|>"); // tokenize the current token
        ft_matrix_replace_in(&tokenized, opt_split, i);
        i += ft_matrixlen(opt_split) - 1;
        ft_free_matrix(&opt_split);
    }
    return tokenized;
}

/*
	used to tokenize command string
	counts tokens with count_words, allocated memory
	fills array with tokens from fill_array
*/
char **tokenize(char *src, char *delims)
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
	n = count_words(src, delims, &lex);
	if (n == -1)
		return (NULL);
	// printf("tokenize n: %i \n", n);
	ret = malloc(sizeof(char *) * (n + 1));
	if (ret == NULL)
		return (NULL);
	ret = fill_array(ret, src, delims, &lex);
	ret[n] = NULL;
	return (ret);
}

/* 
	counts number of tokens in the string s
	token = characters separated by delims except if in quotes
	checks that quotes are properly closed, ret -1 if unclosed
 */
int	count_words(char *s, char *delims, t_lexer *lex)
{
	lex->quotes = 0;
	lex->quote_c = 0;

	while (s[lex->char_i] != '\0')
	{
		if (!ft_strchr(delims, s[lex->char_i]))
		{
			lex->word_n++;
			while ((!ft_strchr(delims, s[lex->char_i]) || lex->quotes) && s[lex->char_i] != '\0')
			{
				if (!lex->quote_c && (s[lex->char_i] == '\"' || s[lex->char_i] == '\''))
					lex->quote_c = s[lex->char_i];
				lex->quotes = (lex->quotes + (s[lex->char_i] == lex->quote_c)) % 2;
				lex->quote_c *= lex->quotes != 0;
				lex->char_i++;
			}
			if (lex->quotes)
				return (-1);
		}
		else
			lex->char_i++;
	}
	return (lex->word_n);
}

/* 
	fills array of strings ret with tokens from string s
	goes over string, separating by delims, except if in quotes or escaped \ 
	substr creates copy of each token
 */
char	**fill_array(char **ret, char *s, char *delims, t_lexer *lex)
{
	int		s_len;

	lex->quotes = 0;
	lex->quote_c = 0;
	lex->char_i = 0;
	s_len = ft_strlen(s);
	while (s[lex->char_i])
	{
		while (ft_strchr(delims, s[lex->char_i]) && s[lex->char_i] != '\0')
			lex->char_i++;
		lex->word_n = lex->char_i;
		while ((!ft_strchr(delims, s[lex->char_i]) || lex->quotes || lex->quote_c) && s[lex->char_i])
		{
			lex->quotes = (lex->quotes + (!lex->quote_c && s[lex->char_i] == '\'')) % 2;
			lex->quote_c = (lex->quote_c + (!lex->quotes && s[lex->char_i] == '\"')) % 2;
			lex->char_i++;
		}
		if (lex->word_n >= s_len)
			ret[lex->token_n++] = "\0";
		else
			ret[lex->token_n++] = ft_substr(s, lex->word_n, lex->char_i - lex->word_n);
	}
	return (ret);
}

/* // splits line according to delimiters with strtok
// taking quotes into account
char	**split_line(char *line)
{
	int		i;
	char	**tokens;
	char	*token;

	i = 0;
	tokens = malloc(sizeof(char *) * ARG_SIZE);
	token = ft_strtok(line, DELIMS);
	while (token != NULL)
	{
		tokens[i] = malloc(sizeof(char) * (ft_strlen(token) + 1));
		ft_strcpy(tokens[i], token);
		token = ft_strtok(NULL, DELIMS);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
*/
