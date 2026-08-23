/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 20:06:21 by mjaad             #+#    #+#             */
/*   Updated: 2026/07/06 20:07:16 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_cooldown_remaining(t_dongle *dg, int time_cooldown)
{
	long	time_now;
	long	remaining;

	time_now = ft_get_time();
	remaining = time_cooldown - (time_now - dg->released_at);
	if (remaining > 0)
		return (remaining);
	return (0);
}

void	ft_release(t_dongle *dg, t_simulation *sm)
{
	pthread_mutex_lock(&(dg->mutex));
	dg->released_at = ft_get_time();
	dg->taken = 0;
	pthread_mutex_unlock(&(dg->mutex));
	pthread_mutex_lock(&(sm->sched_mutex));
	pthread_cond_broadcast(&(sm->sched_cond));
	pthread_mutex_unlock(&(sm->sched_mutex));
}

int	ft_init_scheduler(t_environment *env)
{
	if (strcmp(env->sm->scheduler, "fifo") == 0)
		return (ft_init_fifo(env));
	else
		return (ft_init_edf(env));
}
