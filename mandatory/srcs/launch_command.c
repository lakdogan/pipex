/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 02:24:16 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:29:32 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/**
 * @brief Executes a command with appropriate argument parsing
 *
 * Splits the command string into arguments 
 * using shell-like parsing rules,
 * attempts to locate the executable (either as direct path or in PATH),
 * and executes it. Handles error reporting for invalid commands or
 * permission issues.
 *
 * @param context The pipex context for 
 * resource management and error handling
 * @param cmd_str The command string to execute
 * @param envp The environment variables array to pass to execve
 */
void	launch_command(t_pipex *context, const char *cmd_str, char **envp)
{
	char	**args;
	char	*cmd_path;

	args = shell_split(context, cmd_str);
	if (!args || !args[0])
	{
		write(STDERR_FILENO, "Invalid command\n", 16);
		cleanup_and_exit(context, NULL, 127);
	}
	cmd_path = check_direct_command(args[0]);
	if (cmd_path)
	{
		execve(cmd_path, args, envp);
		free(cmd_path);
		write(STDERR_FILENO, "pipex: ", 7);
		write(STDERR_FILENO, args[0], ft_strlen(args[0]));
		write(STDERR_FILENO, ": permission denied\n", 21);
		cleanup_and_exit(context, NULL, 126);
	}
	search_in_path(context, args, envp);
	cleanup_and_exit(context, "Command execution failed", 126);
}
