/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_context_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:31:47 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:16:27 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc_bonus/pipex_bonus.h"

/**
 * @brief Closes input and output file descriptors
 *
 * @param context Pointer to the pipex context structure
 */
static void	close_file_descriptors(t_pipex *context)
{
	if (context->in_fd > 2)
		close(context->in_fd);
	if (context->out_fd > 2)
		close(context->out_fd);
}

/**
 * @brief Closes all pipe file descriptors
 *
 * @param context Pointer to the pipex context structure
 */
static void	close_pipe_descriptors(t_pipex *context)
{
	int	i;

	i = 0;
	while (i < context->pipe_count)
	{
		if (context->pipes[i * 2] > 2)
			close(context->pipes[i * 2]);
		if (context->pipes[i * 2 + 1] > 2)
			close(context->pipes[i * 2 + 1]);
		i++;
	}
}

/**
 * @brief Frees all dynamically allocated memory in the context
 *
 * @param context Pointer to the pipex context structure
 */
static void	free_allocated_memory(t_pipex *context)
{
	if (context->pipes)
		free(context->pipes);
	if (context->args)
		free_shell_split(context->args);
	if (context->paths)
		free_tab(context->paths);
	if (context->limiter)
		free(context->limiter);
}

/**
 * @brief Frees all resources in the context structure
 *
 * @param context Pointer to the pipex context structure to be freed
 */
void	free_context(t_pipex *context)
{
	if (context && !context->cleaned)
	{
		context->cleaned = 1;
		close_file_descriptors(context);
		close_pipe_descriptors(context);
		free_allocated_memory(context);
		if (!context->is_child)
			free(context);
	}
}
