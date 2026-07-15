/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 19:24:00 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 19:24:00 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	ft_edf_cmp(t_coder *cd1, t_coder *cd2)
{
	long	to_burnout_cd1;
	long	to_burnout_cd2;

	to_burnout_cd1 = cd1->last_compile_start + cd1->sim->time_to_burnout;
	to_burnout_cd2 = cd2->last_compile_start + cd2->sim->time_to_burnout;
	if (to_burnout_cd1 == to_burnout_cd2)
	{
		if (cd1->id > cd2->id)
			return (1);
		return (-1);
	}
	else
		return (to_burnout_cd1 - to_burnout_cd2);
}

int	ft_init_edf(t_environment *env)
{
	int	i;

	i = 0;
	while (i < env->sm->number_of_coders)
	{
		if (ft_init_heap(&(env->dongles[i].heap), 2, ft_edf_cmp) == -1)
			return (-1);
		i++;
	}
	return (1);
}

void	ft_edf_release(t_dongle *dg)
{
	t_coder	*next;

	pthread_mutex_lock(&(dg->mutex));
	dg->released_at = ft_get_time();
	if (dg->heap.size > 0)
	{
		next = dg->heap.heap[0];
		pthread_cond_broadcast(&(next->cond));
	}
	pthread_mutex_unlock(&(dg->mutex));
}

int	ft_edf_acquire(t_coder *cd, t_dongle *dg)
{
	pthread_mutex_lock(&(dg->mutex));
	cd->enqueue_time = ft_get_time();
	ft_insert(&(dg->heap), cd);
	while (ft_is_running(cd->sim) && (ft_cooldown_remaining(dg,
				cd->sim->dongle_cooldown) || dg->heap.heap[0] != cd))
	{
		if (dg->heap.heap[0] == cd)
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
	pthread_mutex_unlock(&dg->mutex);
	return (0);
}
