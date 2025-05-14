/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 05:46:31 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:27:52 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * @brief Frees a NULL-terminated array 
 * of strings (like the result of shell_split)
 *
 * Iterates through the provided array of strings from shell_split,
 * freeing each individual string and finally freeing the array itself.
 * Safely handles NULL input by returning immediately.
 *
 * @param split_result The array of strings to free
 */
void	free_shell_split(char **split_result)
{
	int	i;

	if (!split_result)
		return ;
	i = 0;
	while (split_result[i])
	{
		free(split_result[i]);
		i++;
	}
	free(split_result);
}
