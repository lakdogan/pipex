/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 01:43:45 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 07:18:01 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * @brief Performs complete cleanup and exits with specified code
 *
 * Handles all resource cleanup including closing file descriptors, freeing
 * allocated memory, and displaying an error message if provided. Sets a
 * 'cleaned' flag to prevent double frees.
 * Finally, exits with the specified
 * error code.
 *
 * @param ctx The pipex context structure containing resources to clean up
 * @param msg Error message to display (can be NULL for no message)
 * @param code Exit code to use when terminating
 */
/**
 * @brief Performs cleanup of resources
 *
 * Closes file descriptors and frees allocated memory.
 * Sets a 'cleaned' flag to prevent double frees.
 *
 * @param ctx The pipex context structure containing resources to clean up
 */
static void	perform_cleanup(t_pipex *ctx)
{
	if (ctx && !ctx->cleaned)
	{
		ctx->cleaned = 1;
		if (ctx->in_fd > 2)
			close(ctx->in_fd);
		if (ctx->out_fd > 2)
			close(ctx->out_fd);
		if (ctx->ends[0] > 2)
			close(ctx->ends[0]);
		if (ctx->ends[1] > 2)
			close(ctx->ends[1]);
		if (ctx->args)
			free_shell_split(ctx->args);
		if (ctx->paths)
			free_tab(ctx->paths);
		if (!ctx->is_child)
			free(ctx);
	}
}

/**
 * @brief Performs complete cleanup and exits with specified code
 *
 * @param ctx The pipex context structure containing resources to clean up
 * @param msg Error message to display (can be NULL for no message)
 * @param code Exit code to use when terminating
 */
void	cleanup_and_exit(t_pipex *ctx, const char *msg, int code)
{
	perform_cleanup(ctx);
	if (msg)
	{
		write(STDERR_FILENO, "pipex_bonus: command not found: ", 32);
		if (ctx && ctx->args && ctx->args[0])
			write(STDERR_FILENO, ctx->args[0], ft_strlen(ctx->args[0]));
		write(STDERR_FILENO, "\n", 1);
	}
	exit(code);
}
