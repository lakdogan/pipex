/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:26:05 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:16:27 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/pipex_bonus.h"

/**
 * @brief Checks if a line matches the heredoc limiter
 *
 * @param line Input line to check
 * @param limiter Heredoc delimiter string
 * @return 1 if the line matches the limiter, 0 otherwise
 */
static int	is_limiter_line(char *line, char *limiter)
{
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	if (ft_strncmp(line, limiter, limiter_len) == 0)
	{
		if (line[limiter_len] == '\n' || line[limiter_len] == '\0')
			return (1);
	}
	return (0);
}

/**
 * @brief Reads input lines until the limiter is encountered
 *
 * @param context Pointer to the pipex context structure
 * @param write_fd File descriptor to write the heredoc content
 */
static void	read_heredoc_input(t_pipex *context, int write_fd)
{
	char	*line;

	write(STDOUT_FILENO, "heredoc> ", 9);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (is_limiter_line(line, context->limiter))
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
	}
}

/**
 * @brief Sets up file descriptors for heredoc input and output
 *
 * @param context Pointer to the pipex context structure
 * @param heredoc_pipe Pipe array for heredoc input
 */
static void	setup_heredoc_files(t_pipex *context, int heredoc_pipe[2])
{
	close(heredoc_pipe[1]);
	context->in_fd = heredoc_pipe[0];
	context->out_fd = open(context->outfile_path, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (context->out_fd < 0)
		cleanup_and_exit(context, "could not open output file", 1);
}

/**
 * @brief Handles the heredoc input mechanism
 *
 * @param context Pointer to the pipex context structure
 */
void	handle_heredoc(t_pipex *context)
{
	int	heredoc_pipe[2];

	if (pipe(heredoc_pipe) < 0)
		cleanup_and_exit(context, "pipe failed", 1);
	read_heredoc_input(context, heredoc_pipe[1]);
	setup_heredoc_files(context, heredoc_pipe);
}
