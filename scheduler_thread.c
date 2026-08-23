/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_thread.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-08-22 18:53:16 by mjaad             #+#    #+#             */
/*   Updated: 2026-08-22 18:53:16 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	ft_earliest_dongle_free(t_environment *env)
{
	long	earliest;
	long	remaining;
	int		i;

	earliest = -1;
	i = 0;
	while (i < env->sm->number_of_coders)
	{
		remaining = ft_cooldown_remaining(&(env->dongles[i]),
				env->sm->dongle_cooldown);
		if (remaining > 0 && (earliest == -1 || remaining < earliest))
			earliest = remaining;
		i++;
	}
	if (earliest == -1)
		return (1);
	return (earliest);
}

static int  ft_can_take_dongles(t_coder *cd)
{
    if (cd->dongle_left->taken || cd->dongle_right->taken)
        return (0);
    if (ft_cooldown_remaining(cd->dongle_left, cd->sim->dongle_cooldown) > 0)
        return (0);
    if (ft_cooldown_remaining(cd->dongle_right, cd->sim->dongle_cooldown) > 0)
        return (0);
    return (1);
}

static void ft_get_dongles(t_coder *cd)
{
    cd->dongle_left->taken = 1;
    cd->dongle_right->taken = 1;
    cd->can_compile = 1;
    pthread_cond_broadcast(&(cd->sim->sched_cond));
}

void	*ft_scheduler(void *arg)
{
	t_environment	*env;
	t_coder			*cd;
	int				i;
	long			wait_ms;
	struct timespec	ts;

	env = (t_environment *)arg;
	while (ft_is_running(env->sm))
	{
		pthread_mutex_lock(&(env->sm->sched_mutex));
		i = 0;
		while (i < env->sm->heap.size)
		{
			cd = env->sm->heap.heap[i];
			if (ft_can_take_dongles(cd))
			{
				ft_get_dongles(cd);
				ft_remove_from_heap(&(env->sm->heap), i);
			}
			else
				i++;
		}
		wait_ms = ft_earliest_dongle_free(env);
		ft_timespec_from_ms(&ts, wait_ms);
		pthread_cond_timedwait(&(env->sm->sched_cond), &(env->sm->sched_mutex), &ts);
		pthread_mutex_unlock(&(env->sm->sched_mutex));
	}
	return (NULL);
}
