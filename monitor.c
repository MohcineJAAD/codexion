/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 20:07:53 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 20:07:53 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_shutdown(t_environment *env)
{
	pthread_mutex_lock(&(env->sm->running_mutex));
	env->sm->running = 0;
	pthread_mutex_unlock(&(env->sm->running_mutex));
	pthread_mutex_lock(&(env->sm->sched_mutex));
	pthread_cond_broadcast(&(env->sm->sched_cond));
	pthread_mutex_unlock(&(env->sm->sched_mutex));
}

static int	ft_all_compiled(t_environment *env)
{
	int	i;

	i = 0;
	while (i < env->sm->number_of_coders)
	{
		if (!ft_is_compile_max(&(env->coders[i])))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_check_burnout(t_environment *env)
{
	int	i;

	i = 0;
	while (i < env->sm->number_of_coders)
	{
		if (ft_is_burnout(&(env->coders[i])))
			return (i);
		i++;
	}
	return (-1);
}

void	*ft_monitor(void *arg)
{
	t_environment	*env;
	long			earliest_dl;
	long			wait_ms;
	struct timespec	ts;
	int				burned_id;

	env = (t_environment *)arg;
	while (1)
	{
		if (ft_all_compiled(env))
			return (ft_shutdown(env), NULL);
		earliest_dl = ft_find_earliest_deadline(env);
		wait_ms = earliest_dl - ft_get_time();
		if (wait_ms < 0)
			wait_ms = 0;
		ft_timespec_from_ms(&ts, wait_ms);
		pthread_mutex_lock(&(env->sm->sched_mutex));
		pthread_cond_timedwait(&(env->sm->sched_cond), &(env->sm->sched_mutex), &ts);
		pthread_mutex_unlock(&(env->sm->sched_mutex));
		burned_id = ft_check_burnout(env);
		if (burned_id != -1)
		{
			ft_shutdown(env);
			ft_print_log(&(env->coders[burned_id]), "burned out\n");
			return (NULL);
		}
	}
	return (NULL);
}