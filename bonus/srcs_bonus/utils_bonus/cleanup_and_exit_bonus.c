/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_exit_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:39:53 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:32:57 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc_bonus/pipex_bonus.h"

/**
 * @brief Closes open file descriptors
 *
 * Safely closes input and output 
 * file descriptors stored in the context structure
 * if they are valid (greater than 2, as 0, 1, and 2 are standard streams).
 *
 * @param ctx Pointer to the pipex context structure
 */
static void	close_fds(t_pipex *ctx)
{
	if (ctx->in_fd > 2)
		close(ctx->in_fd);
	if (ctx->out_fd > 2)
		close(ctx->out_fd);
}

/**
 * @brief Closes all pipe file descriptors
 * 
 * Safely closes all pipe file descriptors stored in the pipes array
 * and frees the memory allocated for the pipe array.
 *
 * @param ctx Pointer to the pipex context structure
 */
static void	close_pipes(t_pipex *ctx)
{
	int	i;

	if (!ctx->pipes)
		return ;
	i = 0;
	while (i < ctx->pipe_count * 2)
	{
		if (ctx->pipes[i] > 2)
			close(ctx->pipes[i]);
		i++;
	}
	free(ctx->pipes);
}

/**
 * @brief Frees allocated memory resources
 *
 * Deallocates all dynamically allocated memory in the pipex context structure,
 * including command arguments, paths, and the limiter string.
 * If the context is not a child process, 
 * it also frees the context structure itself.
 *
 * @param ctx Pointer to the pipex context structure
 */
static void	free_resources(t_pipex *ctx)
{
	if (ctx->args)
		free_shell_split(ctx->args);
	if (ctx->paths)
		free_tab(ctx->paths);
	if (ctx->limiter)
		free(ctx->limiter);
	if (!ctx->is_child)
		free(ctx);
}

/**
 * @brief Displays an error message to stderr
 *
 * Writes the provided error message to the standard error output
 * with a "pipex: " prefix followed by a newline character.
 * If the message is NULL, no error is displayed.
 *
 * @param msg The error message to display
 */
static void	display_error(const char *msg)
{
	if (!msg)
		return ;
	write(STDERR_FILENO, "pipex: ", 7);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

/**
 * @brief Handles cleanup of resources and exits with a specific error code
 *
 * Performs a complete cleanup of all resources allocated by the program,
 * including closing file descriptors, closing pipes, and freeing memory.
 * Then displays an error message (if provided)
 * and exits with the specified code.
 * The cleanup is performed only once per context to avoid double free issues.
 *
 * @param ctx Pointer to the pipex context structure
 * @param msg Error message to display (can be NULL)
 * @param code Exit code to use when terminating the program
 */
void	cleanup_and_exit(t_pipex *ctx, const char *msg, int code)
{
	if (ctx && !ctx->cleaned)
	{
		ctx->cleaned = 1;
		close_fds(ctx);
		close_pipes(ctx);
		free_resources(ctx);
	}
	display_error(msg);
	exit(code);
}
