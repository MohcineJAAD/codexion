/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-08-22 20:45:06 by mjaad             #+#    #+#             */
/*   Updated: 2026-08-22 20:45:06 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_join_heap(t_coder *cd)
{
	pthread_mutex_lock(&(cd->sim->sched_mutex));
	cd->enqueue_time = ft_get_time();
	ft_insert(&(cd->sim->heap), cd);
	pthread_cond_broadcast(&(cd->sim->sched_cond));
	pthread_mutex_unlock(&(cd->sim->sched_mutex));
}

int	ft_wait_to_can_compile(t_coder *cd)
{
	pthread_mutex_lock(&(cd->sim->sched_mutex));
	while (ft_is_running(cd->sim) && !cd->can_compile)
		pthread_cond_wait(&(cd->sim->sched_cond), &(cd->sim->sched_mutex));
	if (!ft_is_running(cd->sim))
	{
		pthread_mutex_unlock(&(cd->sim->sched_mutex));
		return (1);
	}
	cd->can_compile = 0;
	pthread_mutex_unlock(&(cd->sim->sched_mutex));
	return (0);
}
