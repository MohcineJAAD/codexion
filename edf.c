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
	if (ft_init_heap(&(env->sm->heap), env->sm->number_of_coders,
			ft_edf_cmp) == -1)
		return (-1);
	return (1);
}
