/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_context.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 05:51:49 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 05:27:52 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/pipex.h"

/**
 * @brief Frees all dynamically allocated 
 * resources in the pipex context
 *
 * Safely closes all file descriptors and frees all dynamically allocated
 * memory in the pipex context, including arguments arrays and paths.
 * Sets the 'cleaned' flag to prevent 
 * double frees. Can handle NULL context.
 *
 * @param context The pipex context structure to free
 */
void	free_context(t_pipex *context)
{
	if (context && !context->cleaned)
	{
		context->cleaned = 1;
		if (context->in_fd > 2)
			close(context->in_fd);
		if (context->out_fd > 2)
			close(context->out_fd);
		if (context->ends[0] > 2)
			close(context->ends[0]);
		if (context->ends[1] > 2)
			close(context->ends[1]);
		if (context->args)
			free_shell_split(context->args);
		if (context->paths)
			free_tab(context->paths);
		free(context);
	}
}
