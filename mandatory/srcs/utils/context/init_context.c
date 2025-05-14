/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_context.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 01:36:29 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 05:27:57 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/pipex.h"

/**
 * @brief Initialize the pipex context structure
 *
 * Sets up a new context with the provided command line arguments and
 * environment variables. Initializes 
 * file descriptors to -1 and provides
 * default environment if none is provided.
 *
 * @param argv Command line arguments array
 * @param envp Environment variables array
 * @return t_pipex* Initialized context structure
 */
t_pipex	*init_context(char **argv, char **envp)
{
	static char	*default_env[] = {"PATH=/bin:/usr/bin:/usr/local/bin", NULL};
	t_pipex		*ctx;

	ctx = malloc(sizeof(*ctx));
	if (!ctx)
		cleanup_and_exit(NULL, "malloc failed", 1);
	ctx->infile_path = argv[1];
	ctx->first_cmd = argv[2];
	ctx->second_cmd = argv[3];
	ctx->outfile_path = argv[4];
	ctx->env_vars = envp;
	if (!envp)
		ctx->env_vars = default_env;
	ctx->in_fd = -1;
	ctx->out_fd = -1;
	ctx->ends[0] = -1;
	ctx->ends[1] = -1;
	ctx->args = NULL;
	ctx->paths = NULL;
	ctx->is_child = 0;
	ctx->cleaned = 0;
	return (ctx);
}
