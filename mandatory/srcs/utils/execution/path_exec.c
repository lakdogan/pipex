/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:06:58 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 07:12:07 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/pipex.h"

/**
 * @brief Build a full command path by joining directory and command
 *
 * @param dir Directory path
 * @param cmd Command name
 * @return char* The full path (dir/cmd), must be freed by caller
 */
static char	*get_full_path(const char *dir, const char *cmd)
{
	char	*prefix;
	char	*full_path;

	prefix = ft_strjoin(dir, "/");
	if (!prefix)
		cleanup_and_exit(NULL, "malloc failed", 1);
	full_path = ft_strjoin(prefix, cmd);
	free(prefix);
	return (full_path);
}

/**
 * @brief Try to execute the command in each directory from PATH
 *
 * If the command is found but not executable,
 * prints error and exits(126).
 * If the command is not found at all, prints error and exits(127).
 * Does not return if command is found and executed.
 *
 * @param paths Array of directory paths to search
 * @param args Command and its arguments
 * @param envp Environment variables array
 */
void	try_exec_in_dirs(char **paths, char **args, char **envp)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths && paths[i])
	{
		full_path = get_full_path(paths[i], args[0]);
		if (!access(full_path, X_OK))
		{
			execve(full_path, args, envp);
			write(STDERR_FILENO, "pipex: ", 7);
			write(STDERR_FILENO, args[0], ft_strlen(args[0]));
			write(STDERR_FILENO, ": permission denied\n", 21);
			free(full_path);
			_exit(126);
		}
		free(full_path);
		i++;
	}
	write(STDERR_FILENO, "pipex: command not found: ", 26);
	write(STDERR_FILENO, args[0], ft_strlen(args[0]));
	write(STDERR_FILENO, "\n", 1);
	_exit(127);
}
