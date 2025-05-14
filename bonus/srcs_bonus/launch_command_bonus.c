/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_command_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:39:05 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:16:27 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/pipex_bonus.h"

/**
 * @brief Finds the PATH environment variable
 *
 * @param envp Array of environment variables
 * @return Pointer to the PATH value or NULL if not found
 */
static char	*find_path_variable(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * @brief Attempts to execute a command with a direct path
 *
 * @param context Pointer to the pipex context structure
 * @param cmd_args Array of command arguments
 * @param envp Array of environment variables
 * @return 1 if successful, 0 if failed
 */
static int	try_direct_path(t_pipex *context, char **cmd_args, char **envp)
{
	context->cmd_path = ft_strdup(cmd_args[0]);
	if (!context->cmd_path)
		cleanup_and_exit(context, "malloc failed", 1);
	if (execve(context->cmd_path, cmd_args, envp) == -1)
	{
		free(context->cmd_path);
		return (0);
	}
	return (1);
}

/**
 * @brief Builds a full command path by joining directory and command
 *
 * @param dir Directory path
 * @param cmd Command name
 * @return Full path string or NULL on allocation failure
 */
static char	*build_cmd_path(char *dir, char *cmd)
{
	char	*dir_slash;
	char	*full_path;

	dir_slash = ft_strjoin(dir, "/");
	if (!dir_slash)
		return (NULL);
	full_path = ft_strjoin(dir_slash, cmd);
	free(dir_slash);
	return (full_path);
}

/**
 * @brief Tries to execute a command by searching in PATH directories
 *
 * @param context Pointer to the pipex context structure
 * @param paths Array of PATH directories
 * @param cmd_args Array of command arguments
 * @param envp Array of environment variables
 */
static void	try_path_dirs(t_pipex *context, char **paths, char **cmd_args,
		char **envp)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		context->cmd_path = build_cmd_path(paths[i], cmd_args[0]);
		if (!context->cmd_path)
		{
			free_tab(paths);
			cleanup_and_exit(context, "malloc failed", 1);
		}
		execve(context->cmd_path, cmd_args, envp);
		free(context->cmd_path);
		context->cmd_path = NULL;
		i++;
	}
	free_tab(paths);
	cleanup_and_exit(context, "command not found", 127);
}

/**
 * @brief Launches a command with proper path resolution
 *
 * @param context Pointer to the pipex context structure
 * @param cmd_str Command string to execute
 * @param envp Array of environment variables
 */
void	launch_command_bonus(t_pipex *context, char *cmd_str, char **envp)
{
	char	**cmd_args;
	char	*path_env;
	char	**paths;

	cmd_args = shell_split(context, cmd_str);
	if (!cmd_args || !cmd_args[0])
		cleanup_and_exit(context, "command parsing failed", 127);
	context->args = cmd_args;
	if (ft_strchr(cmd_args[0], '/'))
	{
		if (!try_direct_path(context, cmd_args, envp))
			cleanup_and_exit(context, "command execution failed", 126);
	}
	else
	{
		path_env = find_path_variable(envp);
		if (!path_env)
			cleanup_and_exit(context, "command not found", 127);
		paths = ft_split(path_env, ':');
		if (!paths)
			cleanup_and_exit(context, "malloc failed", 1);
		try_path_dirs(context, paths, cmd_args, envp);
	}
}
