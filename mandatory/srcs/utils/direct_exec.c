/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direct_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:10:49 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 07:11:30 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * @brief Handles direct command execution when path is included
 *
 * If the command contains a slash, this function handles it directly:
 * - If the file exists and is executable, returns a duplicate of the command
 * - If it exists but is not executable,
	prints "permission denied" and exits(126)
 * - If it does not exist, prints "command not found" and exits(127)
 * - If no slash is found in the command, returns NULL
 *
 * @param cmd The command string to check
 * @return Duplicated command path if valid, NULL otherwise
 */
char	*check_direct_command(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			else
			{
				write(STDERR_FILENO, "pipex: ", 7);
				write(STDERR_FILENO, cmd, ft_strlen(cmd));
				write(STDERR_FILENO, ": permission denied\n", 21);
				_exit(126);
			}
		}
		else
		{
			write(STDERR_FILENO, "pipex: command not found: ", 26);
			write(STDERR_FILENO, cmd, ft_strlen(cmd));
			write(STDERR_FILENO, "\n", 1);
			_exit(127);
		}
	}
	return (NULL);
}
