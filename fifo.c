/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 19:32:25 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 19:32:25 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	ft_fifo_cmp(t_coder *cd1, t_coder *cd2)
{
	return (cd1->enqueue_time - cd2->enqueue_time);
}

int	ft_init_fifo(t_environment *env)
{
	if (ft_init_heap(&(env->sm->heap), env->sm->number_of_coders,
			ft_fifo_cmp) == -1)
		return (-1);
	return (1);
}
