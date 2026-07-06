/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 19:30:12 by mjaad             #+#    #+#             */
/*   Updated: 2026/07/06 20:05:30 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_compile(t_coder *cd, int direction)
{
	t_dongle	*dongles[2];

	dongles[0] = cd->dongle_left;
	dongles[1] = cd->dongle_right;
	if (direction == 1)
	{
		dongles[0] = cd->dongle_right;
		dongles[1] = cd->dongle_left;
	}
	cd->last_compile_start = ft_get_time();
	if (ft_acquire(cd, dongles[0]))
		return ;
	ft_print_log(cd, "has taken a dongle\n");
	if (ft_acquire(cd, dongles[1]))
	{
		ft_release(dongles[0], cd->sim);
		return ;
	}
	ft_print_log(cd, "has taken a dongle\n");
	ft_print_log(cd, "is compiling\n");
	usleep(cd->sim->time_to_compile * 1000);
	ft_release(dongles[0], cd->sim);
	ft_release(dongles[1], cd->sim);
	cd->compile_count++;
}

static void	ft_debugging(t_coder *cd)
{
	ft_print_log(cd, "is debugging\n");
	usleep(cd->sim->time_to_debug * 1000);
}

static void	ft_refactoring(t_coder *cd)
{
	ft_print_log(cd, "is refactoring\n");
	usleep(cd->sim->time_to_refactor * 1000);
}

int	ft_is_running(t_simulation *sim)
{
	int	running;

	pthread_mutex_lock(&(sim->running_mutex));
	running = sim->running;
	pthread_mutex_unlock(&(sim->running_mutex));
	return (running);
}

void	*coder_routine(void *coder)
{
	t_coder	*cd;

	cd = (t_coder *)coder;
	if (cd->sim->number_of_coders == 1)
	{
		pthread_mutex_lock(&(cd->dongle_left->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		while (ft_is_running(cd->sim))
			usleep(1000);
		pthread_mutex_unlock(&(cd->dongle_left->mutex));
		return (NULL);
	}
	while (ft_is_running(cd->sim))
	{
		ft_compile(cd, cd->id == cd->sim->number_of_coders);
		if (!ft_is_running(cd->sim))
			break ;
		ft_debugging(cd);
		if (!ft_is_running(cd->sim))
			break ;
		ft_refactoring(cd);
	}
	return (NULL);
}
