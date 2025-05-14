/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 01:52:46 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 07:18:39 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/**
 * @brief Opens the input file for reading
 *
 * Attempts to open the input file specified in the context.
 * If opening fails,
 * it displays an error message, creates an
 * empty output file (to match shell
 * behavior) and exits. This handles the
 * case where the input file doesn't exist.
 *
 * @param context The pipex context containing file paths
 */
static void	open_infile(t_pipex *context)
{
	context->in_fd = open(context->infile_path, O_RDONLY);
	if (context->in_fd < 0)
	{
		ft_putstr_fd("pipex: No such file or directory: ", STDERR_FILENO);
		ft_putendl_fd(context->infile_path, STDERR_FILENO);
		context->out_fd = open(context->outfile_path,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		exit(0);
	}
}

/**
 * @brief Redirects standard input to read from the input file
 *
 * Duplicates the input file descriptor to standard input (fd 0)
 * and closes the original file descriptor to avoid leaks.
 * If duplication fails, cleans up and exits.
 *
 * @param context The pipex context
 * containing the input file descriptor
 */
static void	redirect_stdin(t_pipex *context)
{
	if (dup2(context->in_fd, STDIN_FILENO) < 0)
		cleanup_and_exit(context, "dup2 infile", 4);
	close(context->in_fd);
}

/**
 * @brief Redirects standard output to write to the pipe
 *
 * Duplicates the write end of the pipe to standard output (fd 1)
 * and closes the original pipe write end to avoid leaks.
 * If duplication fails, cleans up and exits.
 *
 * @param context The pipex context containing pipe file descriptors
 */
static void	redirect_stdout_to_pipe(t_pipex *context)
{
	if (dup2(context->ends[1], STDOUT_FILENO) < 0)
		cleanup_and_exit(context, "dup2 pipe write", 4);
	close(context->ends[1]);
}

/**
 * @brief Closes the unused read end of the pipe in the child process
 *
 * Prevents pipe leaks by closing file
 * descriptors that are no longer needed.
 *
 * @param context The pipex context containing pipe file descriptors
 */
static void	close_unused_pipe_ends_in_child(t_pipex *context)
{
	close(context->ends[0]);
}

/**
 * @brief Sets up the child process for executing the first command
 *
 * Prepares the child process by opening the input file,
 * redirecting I/O,
 * closing unused pipe ends, and launching the first command.
 * If any step fails, performs cleanup and exits with an error.
 *
 * @param context The pipex context containing
 * necessary execution information
 */
void	setup_child(t_pipex *context)
{
	open_infile(context);
	redirect_stdin(context);
	redirect_stdout_to_pipe(context);
	close_unused_pipe_ends_in_child(context);
	launch_command(context, context->first_cmd, context->env_vars);
	cleanup_and_exit(context, "child process error", 1);
}
