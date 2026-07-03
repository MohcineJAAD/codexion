/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 20:07:06 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 20:07:06 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_print_log(t_coder *cd, char *log)
{
	long	time;

	pthread_mutex_lock(&(cd->sim->print_mutex));
	time = ft_get_time() - cd->sim->start_time;
	printf("%ld %d %s", time, cd->id, log);
	pthread_mutex_unlock(&(cd->sim->print_mutex));
}
