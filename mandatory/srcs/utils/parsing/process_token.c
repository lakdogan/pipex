/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:25:35 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 05:28:10 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/pipex.h"

/**
 * @brief Handles quoted strings by removing surrounding quotes
 *
 * @param context The pipex context for error handling
 * @param tmp The string to process
 * @return char* A new string with quotes removed
 */
static char	*handle_quotes(t_pipex *context, const char *tmp)
{
	char	*buf;
	size_t	len;
	size_t	i;
	size_t	j;

	len = ft_strlen(tmp);
	buf = malloc(len + 1);
	if (!buf)
		cleanup_and_exit(context, "malloc failed", 1);
	i = 0;
	j = 0;
	if (len >= 2 && ((tmp[0] == '"' && tmp[len - 1] == '"') || (tmp[0] == '\''
				&& tmp[len - 1] == '\'')))
	{
		i = 1;
		while (i < len - 1)
			buf[j++] = tmp[i++];
	}
	else
	{
		while (i < len)
			buf[j++] = tmp[i++];
	}
	buf[j] = '\0';
	return (buf);
}

/**
 * @brief Handles escape sequences in strings
 *
 * @param context The pipex context for error handling
 * @param buf The string containing escape sequences
 * @return char* A new string with proper escape handling
 */
static char	*handle_escapes(t_pipex *context, const char *buf)
{
	char	*newbuf;
	size_t	len;
	size_t	k;
	size_t	m;

	len = ft_strlen(buf);
	newbuf = malloc(len + 1);
	if (!newbuf)
		cleanup_and_exit(context, "malloc failed", 1);
	k = 0;
	m = 0;
	while (k < len)
	{
		if (buf[k] == '\\' && (k + 1 < len) && buf[k + 1] == '"')
		{
			newbuf[m++] = '"';
			k += 2;
		}
		else
			newbuf[m++] = buf[k++];
	}
	newbuf[m] = '\0';
	return (newbuf);
}

/**
 * @brief Processes a token by 
 * handling quotes and escape sequences
 *
 * @param context The pipex context for error handling
 * @param s The source string
 * @param bounds The boundaries of the token to process
 * @return char* The processed token
 */
char	*process_token(t_pipex *context, const char *s, t_token_bounds bounds)
{
	char	*tmp;
	char	*buf;
	char	*newbuf;

	tmp = ft_strndup(s + bounds.start, bounds.end - bounds.start);
	if (!tmp)
		cleanup_and_exit(context, "malloc failed", 1);
	buf = handle_quotes(context, tmp);
	free(tmp);
	newbuf = handle_escapes(context, buf);
	free(buf);
	return (newbuf);
}
