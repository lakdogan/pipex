/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:57:00 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/09 02:54:54 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../../libft/inc/libft.h"
# include <sys/wait.h>

typedef struct s_pipex
{
	char	*infile_path;
	char	*outfile_path;
	char	*first_cmd;
	char	*second_cmd;

	int		ends[2];
	int		in_fd;
	int		out_fd;

	char	**env_vars;
	char	**args;
	char	**paths;

	int		is_child;
	int		cleaned;
}			t_pipex;

typedef struct s_token_bounds
{
	size_t	start;
	size_t	end;
}			t_token_bounds;

void		free_tab(char **tab);
char		**shell_split(t_pipex *context, const char *s);
int			is_in_quotes(const char *s, size_t pos);
size_t		skip_spaces(const char *s, size_t j);
size_t		process_token_count(const char *s, size_t j, int *in_sq,
				int *in_dq);
int			count_tokens(const char *s);
char		*process_token(t_pipex *context, const char *s,
				t_token_bounds bounds);
int			ft_isspace(int c);
char		*ft_strndup(const char *s, size_t n);
void		free_shell_split(char **split_result);
void		free_context(t_pipex *context);
void		cleanup_and_exit(t_pipex *ctx, const char *msg, int code);
t_pipex		*init_context(char **argv, char **envp);
void		setup_child(t_pipex *ctx);
void		setup_parent(t_pipex *ctx);
void		launch_command(t_pipex *context, const char *cmd_str, char **envp);
char		*find_path_env(char **env_vars);
char		**split_path_dirs(t_pipex *context, const char *path_env);
void		search_in_path(t_pipex *context, char **args, char **envp);
void		try_exec_in_dirs(char **paths, char **args, char **envp);
char		*check_direct_command(char *cmd);
#endif
