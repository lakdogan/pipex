/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_processes_bonus2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 06:05:21 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:16:27 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/pipex_bonus.h"

/**
 * @brief Sets up stdin/stdout and executes command in a child process
 *
 * @param context Pointer to the pipex context structure
 * @param argv Array of command line arguments
 * @param i Index of the current command
 */
static void	setup_child_process(t_pipex *context, char **argv, int i)
{
	setup_stdin_stdout(context, i);
	close_all_pipe_fds(context);
	execute_command(context, argv, i);
}

/**
 * @brief Closes all pipe file descriptors in the parent process
 *
 * @param context Pointer to the pipex context structure
 */
static void	close_all_pipes(t_pipex *context)
{
	int	j;

	j = 0;
	while (j < 2 * context->pipe_count)
	{
		close(context->pipes[j]);
		j++;
	}
}

/**
 * @brief Creates child processes for each command
 *
 * @param context Pointer to the pipex context structure
 * @param argv Array of command line arguments
 * @param pids Array to store process IDs
 */
static void	create_processes(t_pipex *context, char **argv, pid_t *pids)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < context->cmd_count)
	{
		pid = fork();
		if (pid < 0)
			cleanup_and_exit(context, "fork failed", 1);
		pids[i] = pid;
		if (pid == 0)
		{
			context->is_child = 1;
			setup_child_process(context, argv, i);
		}
		i++;
	}
}

/**
 * @brief Handles the creation and management of child processes
 *
 * @param context Pointer to the pipex context structure
 * @param argv Array of command line arguments
 * @return Exit code of the last command
 */
int	handle_processes(t_pipex *context, char **argv)
{
	pid_t	*pids;
	int		exit_code;

	pids = malloc(sizeof(pid_t) * context->cmd_count);
	if (!pids)
		cleanup_and_exit(context, "malloc failed", 1);
	create_processes(context, argv, pids);
	close_all_pipes(context);
	exit_code = wait_children(context, pids);
	free(pids);
	return (exit_code);
}
