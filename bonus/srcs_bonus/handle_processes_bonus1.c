/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_processes_bonus1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:24:10 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:32:17 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/pipex_bonus.h"

/**
 * @brief Waits for all child processes 
 * to complete and returns the last exit code
 *
 * @param context Pointer to the pipex context structure
 * @param pids Array of process IDs to wait for
 * @return Exit code of the last command or 1 if input was missing
 */
int	wait_children(t_pipex *context, pid_t *pids)
{
	int	status;
	int	i;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < context->cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			if (i == context->cmd_count - 1)
				last_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			if (i == context->cmd_count - 1)
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	if (context->input_missing && last_status == 0)
		return (1);
	return (last_status);
}

/**
 * @brief Sets up standard input and output for a child process
 *
 * @param context Pointer to the pipex context structure
 * @param i Index of the current command
 */
void	setup_stdin_stdout(t_pipex *context, int i)
{
	if (i == 0)
	{
		dup2(context->in_fd, STDIN_FILENO);
		dup2(context->pipes[1], STDOUT_FILENO);
	}
	else if (i == context->cmd_count - 1)
	{
		dup2(context->pipes[(i - 1) * 2], STDIN_FILENO);
		dup2(context->out_fd, STDOUT_FILENO);
	}
	else
	{
		dup2(context->pipes[(i - 1) * 2], STDIN_FILENO);
		dup2(context->pipes[i * 2 + 1], STDOUT_FILENO);
	}
}

/**
 * @brief Closes all pipe file descriptors in a child process
 *
 * @param context Pointer to the pipex context structure
 */
void	close_all_pipe_fds(t_pipex *context)
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
 * @brief Executes a command in a child process
 *
 * @param context Pointer to the pipex context structure
 * @param argv Array of command line arguments
 * @param i Index of the current command
 */
void	execute_command(t_pipex *context, char **argv, int i)
{
	char	*cmd_str;

	if (context->is_heredoc)
		cmd_str = argv[i + 3];
	else
		cmd_str = argv[i + 2];
	launch_command_bonus(context, cmd_str, context->env_vars);
	cleanup_and_exit(context, "command execution failed", 1);
}
