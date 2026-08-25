/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-08-23 20:59:00 by mjaad             #+#    #+#             */
/*   Updated: 2026-08-23 20:59:00 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	ft_get_deadline(t_coder *cd)
{
	long	last_start;

	pthread_mutex_lock(&(cd->sim->stats_mutex));
	last_start = cd->last_compile_start;
	pthread_mutex_unlock(&(cd->sim->stats_mutex));
	return (last_start + cd->sim->time_to_burnout);
}

int	ft_is_burnout(t_coder *cd)
{
	long	time_difference;
	long	last_start;

	pthread_mutex_lock(&(cd->sim->stats_mutex));
	last_start = cd->last_compile_start;
	pthread_mutex_unlock(&(cd->sim->stats_mutex));
	time_difference = ft_get_time() - last_start;
	if (time_difference > cd->sim->time_to_burnout)
		return (1);
	return (0);
}

int	ft_is_compile_max(t_coder *cd)
{
	pthread_mutex_lock(&(cd->sim->stats_mutex));
	if (cd->compile_count >= cd->sim->number_of_compiles_required)
	{
		pthread_mutex_unlock(&(cd->sim->stats_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(cd->sim->stats_mutex));
	return (0);
}

long	ft_find_earliest_deadline(t_environment *env)
{
	long	earliest;
	long	current;
	int		i;

	i = 0;
	earliest = ft_get_deadline(&(env->coders[0]));
	while (i < env->sm->number_of_coders)
	{
		current = ft_get_deadline(&(env->coders[i]));
		if (current < earliest)
			earliest = current;
		i++;
	}
	return (earliest);
}

void	ft_timespec_from_ms(struct timespec *ts, long ms_from_now)
{
	struct timeval	tv;
	long			target_ms;

	gettimeofday(&tv, NULL);
	target_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) + ms_from_now;
	ts->tv_sec = target_ms / 1000;
	ts->tv_nsec = (target_ms % 1000) * 1000000;
}
