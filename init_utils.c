/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-31 19:20:06 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-31 19:20:06 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_init_dongles(t_dongle *dg, int index)
{
	dg->id = index + 1;
	dg->released_at = 0;
	dg->taken = 0;
	pthread_mutex_init(&(dg->mutex), NULL);
}

void	ft_join_threads(t_environment *env, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		pthread_join(env->threads[i], NULL);
		i++;
	}
}
