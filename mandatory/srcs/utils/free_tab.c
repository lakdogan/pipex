/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 03:05:20 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 04:28:30 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * @brief Frees a NULL-terminated array of strings
 *
 * This function iterates through the
 *  provided array of strings, freeing each
 * string and finally freeing the array itself. It safely handles NULL input.
 *
 * @param tab The array of strings to free
 */
void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
