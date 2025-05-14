/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_lookup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:02:12 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 05:28:06 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/pipex.h"

/**
 * @brief Find the PATH variable in environment variables
 *
 * @param env_vars Array of environment variables
 * @return char* Pointer to the PATH value (without "PATH="), 
 * or NULL if not found
 */
char	*find_path_env(char **env_vars)
{
	int	i;

	if (!env_vars)
		return (NULL);
	i = 0;
	while (env_vars[i])
	{
		if (ft_strncmp(env_vars[i], "PATH=", 5) == 0)
			return (env_vars[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * @brief Split the PATH variable by ':' into separate directories
 *
 * @param context The pipex context for error handling
 * @param path_env The PATH environment variable value
 * @return char** Array of directories from PATH, or NULL on error
 */
char	**split_path_dirs(t_pipex *context, const char *path_env)
{
	char	**paths;

	if (!path_env)
		return (NULL);
	paths = ft_split((char *)path_env, ':');
	if (!paths)
		cleanup_and_exit(context, "pipex: failed to split PATH", 1);
	return (paths);
}

/**
 * @brief Search for the command in PATH directories and execute if found
 * 
 * This function never returns on success as it executes the command.
 *
 * @param context The pipex context for error handling
 * @param args The command arguments array
 * @param envp The environment variables array
 */
void	search_in_path(t_pipex *context, char **args, char **envp)
{
	char		*path_env;
	char		**paths;
	static char	default_path[] = "/bin:/usr/bin:/usr/local/bin";

	path_env = find_path_env(context->env_vars);
	if (!path_env && context->env_vars)
	{
		path_env = default_path;
	}
	paths = split_path_dirs(context, path_env);
	context->paths = paths;
	try_exec_in_dirs(paths, args, envp);
}
