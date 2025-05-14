/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_context_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:19:29 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:33:17 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc_bonus/pipex_bonus.h"

/**
 * @brief Allocates and initializes basic context structure
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @param envp Array of environment variables
 * @return t_pipex* Pointer to the initialized 
 * context structure or NULL on failure
 */
static t_pipex	*setup_context(int argc, char **argv, char **envp)
{
	t_pipex	*context;

	context = (t_pipex *)malloc(sizeof(t_pipex));
	if (!context)
		return (NULL);
	ft_memset(context, 0, sizeof(t_pipex));
	context->env_vars = envp;
	context->infile_path = argv[1];
	context->outfile_path = argv[argc - 1];
	context->is_child = 0;
	context->cleaned = 0;
	context->is_heredoc = 0;
	context->limiter = NULL;
	context->cmd_count = argc - 3;
	context->pipe_count = context->cmd_count - 1;
	context->paths = NULL;
	context->args = NULL;
	return (context);
}

/**
 * @brief Creates pipes for command execution
 * 
 * @param context Pointer to the pipex context structure
 * @return int 1 on success, 0 on failure
 */
static int	setup_pipes(t_pipex *context)
{
	int	i;

	context->pipes = malloc(sizeof(int) * 2 * context->pipe_count);
	if (!context->pipes)
		return (0);
	i = 0;
	while (i < context->pipe_count)
	{
		if (pipe(context->pipes + (i * 2)) < 0)
		{
			while (--i >= 0)
			{
				close(context->pipes[i * 2]);
				close(context->pipes[i * 2 + 1]);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Opens input and output files
 * 
 * @param context Pointer to the pipex context structure
 * @return int 1 on success, 0 on failure
 */
static int	setup_files(t_pipex *context)
{
	context->in_fd = open(context->infile_path, O_RDONLY);
	if (context->in_fd < 0)
	{
		ft_putstr_fd("pipex_bonus: no such file or directory: ", STDERR_FILENO);
		ft_putendl_fd(context->infile_path, STDERR_FILENO);
		context->in_fd = open("/dev/null", O_RDONLY);
		context->input_missing = 1;
		exit(0);
	}
	context->out_fd = open(context->outfile_path, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (context->out_fd < 0)
	{
		perror(context->outfile_path);
		return (0);
	}
	return (1);
}

/**
 * @brief Cleans up context on error
 * 
 * @param context Pointer to the pipex context structure to be cleaned
 */
static void	cleanup_context(t_pipex *context)
{
	int	i;

	if (context->in_fd > 2)
		close(context->in_fd);
	i = 0;
	while (i < context->pipe_count)
	{
		close(context->pipes[i * 2]);
		close(context->pipes[i * 2 + 1]);
		i++;
	}
	free(context->pipes);
	free(context);
}

/**
 * @brief Initializes the context structure for the pipex program
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @param envp Array of environment variables
 * @return t_pipex* Pointer to the fully initialized 
 * context structure or NULL on failure
 */
t_pipex	*init_context(int argc, char **argv, char **envp)
{
	t_pipex	*context;

	context = setup_context(argc, argv, envp);
	if (!context)
		return (NULL);
	if (!setup_pipes(context))
	{
		free(context);
		return (NULL);
	}
	if (!setup_files(context))
	{
		cleanup_context(context);
		return (NULL);
	}
	return (context);
}
