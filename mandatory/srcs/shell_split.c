/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 05:21:42 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:31:19 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/**
 * @brief Finds the index where a token should 
 * be split by the first unquoted space
 *
 * This helper function scans through a token 
 * looking for the first space
 * that is not enclosed in single or double quotes.
 *
 * @param token The token string to analyze
 * @return The index where the token should be split
 */
static size_t	get_token_split_index(const char *token)
{
	size_t	i;
	int		in_sq;
	int		in_dq;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !in_dq)
			in_sq ^= 1;
		else if (token[i] == '"' && !in_sq)
			in_dq ^= 1;
		else if (!in_sq && !in_dq && ft_isspace((unsigned char)token[i]))
			break ;
		i++;
	}
	return (i);
}

/**
 * @brief Splits a command token into command and argument parts
 *
 * Creates an array of two strings where the first string is the command
 * and the second string is the argument portion. Handles special cases
 * where quotes are present.
 *
 * @param context Pipex context for error handling
 * @param token The token to split
 * @return An allocated array containing the split command and arguments
 */
static char	**split_command_arg(t_pipex *context, const char *token)
{
	char	**result;
	size_t	i;

	result = malloc(3 * sizeof(char *));
	if (!result)
		cleanup_and_exit(context, "malloc failed", 1);
	i = get_token_split_index(token);
	result[0] = ft_strndup(token, i);
	while (token[i] && ft_isspace((unsigned char)token[i]))
		i++;
	result[1] = ft_strdup(token + i);
	result[2] = NULL;
	return (result);
}

/**
 * @brief Processes and stores a token in the result array
 *
 * Handles special cases like the "awk" command 
 * which needs special processing.
 * For regular commands, it simply stores 
 * the token in the result array.
 *
 * @param context Pipex context for error handling
 * @param res The result array where tokens are stored
 * @param token_index Pointer to the current index in the result array
 * @param token The token to process and store
 */
static void	store_token(t_pipex *context, char **res, int *token_index,
		char *token)
{
	char	**parts;

	if (ft_strncmp(token, "awk ", 4) == 0)
	{
		parts = split_command_arg(context, token);
		free(token);
		res[(*token_index)++] = parts[0];
		res[(*token_index)++] = process_token(context, parts[1],
				(t_token_bounds){0, ft_strlen(parts[1])});
		free(parts[1]);
		free(parts);
	}
	else
		res[(*token_index)++] = token;
}

/**
 * @brief Fills the result array with 
 * extracted tokens from the input string
 *
 * Processes the input string, extracting 
 * tokens while respecting quotes
 * and spaces. Stores each extracted token in the result array.
 *
 * @param context Pipex context for error handling
 * @param res The result array to fill with tokens
 * @param s The input string to process
 * @param max_count Maximum number of tokens to extract
 */
void	fill_tokens(t_pipex *context, char **res, const char *s, int max_count)
{
	int				token_index;
	size_t			j;
	size_t			len;
	t_token_bounds	bounds;
	char			*token;

	token_index = 0;
	j = 0;
	len = ft_strlen(s);
	while (j < len && token_index < max_count)
	{
		j = skip_spaces(s, j);
		if (j >= len)
			break ;
		bounds.start = j;
		j = process_token_count(s, j, &(int){0}, &(int){0});
		bounds.end = j;
		token = process_token(context, s, bounds);
		store_token(context, res, &token_index, token);
	}
	res[token_index] = NULL;
}

/**
 * @brief Splits a command string into an 
 * array of tokens respecting shell syntax
 *
 * This function analyzes a command string 
 * and splits it into tokens while
 * respecting shell syntax rules like 
 * quoted strings. It handles special
 * commands like "awk" appropriately.
 *
 * @param context Pipex context for error handling
 * @param s The command string to split
 * @return A NULL-terminated array of token strings
 */
char	**shell_split(t_pipex *context, const char *s)
{
	char	**res;
	int		count;

	count = count_tokens(s);
	res = malloc((count + 1) * sizeof(char *));
	if (!res)
		cleanup_and_exit(context, "malloc failed", 1);
	fill_tokens(context, res, s, count);
	return (res);
}
