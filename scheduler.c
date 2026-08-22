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

void	ft_wait_cooldown(t_coder *cd, t_dongle *dg)
{
	struct timeval	tv;
	struct timespec	ts;
	long			remaining;

	remaining = ft_cooldown_remaining(dg, cd->sim->dongle_cooldown);
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (remaining / 1000);
	ts.tv_nsec = (tv.tv_usec * 1000) + (remaining % 1000) * 1000 * 1000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	pthread_cond_timedwait(&(cd->cond), &(dg->mutex), &ts);
}

void	ft_release(t_dongle *dg)
{
	t_coder	*next;

	pthread_mutex_lock(&(dg->mutex));
	dg->released_at = ft_get_time();
	dg->taken = 0;
	if (dg->heap.size > 0)
	{
		next = dg->heap.heap[0];
		pthread_cond_broadcast(&(next->cond));
	}
	pthread_mutex_unlock(&(dg->mutex));
}

int	ft_acquire(t_coder *cd, t_dongle *dg)
{
	pthread_mutex_lock(&(dg->mutex));
	cd->enqueue_time = ft_get_time();
	ft_insert(&(dg->heap), cd);
	while (ft_is_running(cd->sim) && (ft_cooldown_remaining(dg,
				cd->sim->dongle_cooldown) || dg->heap.heap[0] != cd || dg->taken))
	{
		if (dg->heap.heap[0] == cd && !(dg->taken))
			ft_wait_cooldown(cd, dg);
		else
			pthread_cond_wait(&(cd->cond), &(dg->mutex));
	}
	if (!ft_is_running(cd->sim))
	{
		ft_extract_min(&(dg->heap));
		pthread_mutex_unlock(&dg->mutex);
		return (1);
	}
	ft_extract_min(&(dg->heap));
	dg->taken = 1;
	pthread_mutex_unlock(&dg->mutex);
	return (0);
}

int	ft_init_scheduler(t_environment *env)
{
	if (strcmp(env->sm->scheduler, "fifo") == 0)
		return (ft_init_fifo(env));
	else
		return (ft_init_edf(env));
}
