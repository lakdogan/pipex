/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_shell_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 05:29:00 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 05:28:15 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/pipex.h"

/**
 * @brief Determines if a position in a string is inside quotes
 *
 * @param s The string to examine
 * @param pos The position to check
 * @return int 1 if position is inside quotes, 0 otherwise
 */
int	is_in_quotes(const char *s, size_t pos)
{
	int		in_sq;
	int		in_dq;
	size_t	i;

	in_sq = 0;
	in_dq = 0;
	i = 0;
	while (i < pos && s[i])
	{
		if (s[i] == '\\')
		{
			i += 2;
			continue ;
		}
		if (s[i] == '\'' && !in_dq)
			in_sq ^= 1;
		else if (s[i] == '"' && !in_sq)
			in_dq ^= 1;
		i++;
	}
	return (in_sq || in_dq);
}

/**
 * @brief Skips spaces in a string, respecting quoted sections
 *
 * @param s The string to process
 * @param j The starting position
 * @return size_t The position after skipping spaces
 */
size_t	skip_spaces(const char *s, size_t j)
{
	while (s[j] && ft_isspace((unsigned char)s[j]) && !is_in_quotes(s, j))
		j++;
	return (j);
}

/**
 * @brief Processes a token to determine its boundaries, 
 * respecting quotes
 *
 * @param s The string containing the token
 * @param j The starting position of the token
 * @param in_sq Pointer to single quote state
 * @param in_dq Pointer to double quote state
 * @return size_t The ending position of the token
 */
size_t	process_token_count(const char *s, size_t j, int *in_sq, int *in_dq)
{
	size_t	len;

	len = ft_strlen(s);
	while (j < len)
	{
		if (s[j] == '\\')
		{
			j += 2;
			continue ;
		}
		if (s[j] == '\'' && !(*in_dq))
			*in_sq ^= 1;
		else if (s[j] == '"' && !(*in_sq))
			*in_dq ^= 1;
		else if (!(*in_sq) && !(*in_dq) && ft_isspace((unsigned char)s[j]))
			break ;
		j++;
	}
	return (j);
}

/**
 * @brief Counts the number of tokens in a string, respecting shell syntax
 *
 * @param s The string to analyze
 * @return int The number of tokens found
 */
int	count_tokens(const char *s)
{
	int		count;
	int		in_sq;
	int		in_dq;
	size_t	j;
	size_t	len;

	count = 0;
	in_sq = 0;
	in_dq = 0;
	j = 0;
	len = ft_strlen(s);
	while (j < len)
	{
		j = skip_spaces(s, j);
		if (j >= len)
			break ;
		count++;
		j = process_token_count(s, j, &in_sq, &in_dq);
	}
	return (count);
}
