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

static int	ft_is_burnout(t_coder *cd)
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

static	int	ft_is_compile_max(t_coder *cd)
{
	pthread_mutex_lock(&(cd->sim->stats_mutex));
	if (cd->compile_count >= cd->sim->number_of_compiles_required)
	{
		pthread_mutex_unlock(&(cd->sim->stats_mutex));
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&(cd->sim->stats_mutex));
		return (0);
	}
}

static void	ft_shutdown(t_environment *env)
{
	pthread_mutex_lock(&(env->sm->running_mutex));
	env->sm->running = 0;
	pthread_mutex_unlock(&(env->sm->running_mutex));
	pthread_mutex_lock(&(env->sm->sched_mutex));
	pthread_cond_broadcast(&(env->sm->sched_cond));
	pthread_mutex_unlock(&(env->sm->sched_mutex));
}

void	*ft_monitor(void *arg)
{
	t_environment	*env;
	int				i;
	int				all_compil;

	env = (t_environment *)arg;
	while (1)
	{
		usleep(1000);
		i = 0;
		all_compil = 1;
		while (i < env->sm->number_of_coders)
		{
			if (ft_is_burnout(&(env->coders[i])))
			{
				ft_shutdown(env);
				ft_print_log(&(env->coders[i]), "burned out\n" );
				return (NULL);
			}
			if (!ft_is_compile_max(&(env->coders[i])))
				all_compil = 0;
			i++;
		}
		if (all_compil)
			return (ft_shutdown(env), NULL);
	}
}
