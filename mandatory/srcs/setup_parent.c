/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_parent.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 01:53:00 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:30:42 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/**
 * @brief Opens the output file for writing
 *
 * Opens the output file specified in the context, 
 * creating it if it doesn't exist
 * and truncating it if it does. 
 * Uses permission 0644 for created files.
 * If opening fails, displays an error and exits.
 *
 * @param context The pipex context containing the outfile path
 */
static void	open_outfile(t_pipex *context)
{
	char	err_msg[256];

	context->out_fd = open(context->outfile_path, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (context->out_fd < 0)
	{
		ft_strlcpy(err_msg, "pipex: ", 8);
		ft_strlcat(err_msg, context->outfile_path, sizeof(err_msg));
		perror(err_msg);
		cleanup_and_exit(context, NULL, 1);
	}
}

/**
 * @brief Redirects standard input to read from the pipe
 *
 * Duplicates the read end of the pipe to standard input (fd 0)
 * and closes the original pipe read end to avoid leaks.
 * If duplication fails, cleans up and exits.
 *
 * @param context The pipex context containing pipe file descriptors
 */
static void	redirect_stdin_from_pipe(t_pipex *context)
{
	if (dup2(context->ends[0], STDIN_FILENO) < 0)
		cleanup_and_exit(context, "dup2 pipe read", 4);
	close(context->ends[0]);
}

/**
 * @brief Redirects standard output to the output file
 *
 * Duplicates the output file descriptor to standard output (fd 1)
 * and closes the original file descriptor to avoid leaks.
 * If duplication fails, cleans up and exits.
 *
 * @param context The pipex context containing the output file descriptor
 */
static void	redirect_stdout_to_outfile(t_pipex *context)
{
	if (dup2(context->out_fd, STDOUT_FILENO) < 0)
		cleanup_and_exit(context, "dup2 outfile", 4);
	close(context->out_fd);
}

/**
 * @brief Closes the unused write end of the pipe in the parent process
 *
 * Prevents pipe leaks by closing file 
 * descriptors that are no longer needed.
 *
 * @param context The pipex context containing pipe file descriptors
 */
static void	close_unused_pipe_ends_in_parent(t_pipex *context)
{
	close(context->ends[1]);
}

/**
 * @brief Sets up the parent process for executing the second command
 *
 * Prepares the parent process by opening the 
 * output file, redirecting I/O,
 * closing unused pipe ends, and launching the second command.
 * If any step fails, performs cleanup and exits with an error.
 *
 * @param context The pipex context containing 
 * necessary execution information
 */
void	setup_parent(t_pipex *context)
{
	open_outfile(context);
	redirect_stdin_from_pipe(context);
	redirect_stdout_to_outfile(context);
	close_unused_pipe_ends_in_parent(context);
	launch_command(context, context->second_cmd, context->env_vars);
	cleanup_and_exit(context, "parent process failed", 1);
}
