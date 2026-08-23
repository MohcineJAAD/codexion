/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 20:07:26 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 20:07:26 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_cleanup(t_environment *env)
{
	int	i;

	i = 0;
	while (i < env->sm->number_of_coders)
	{
		pthread_mutex_destroy(&(env->dongles[i].mutex));
		pthread_cond_destroy(&(env->coders[i].cond));
		i++;
	}
	ft_heap_destroy(&(env->sm->heap));
	pthread_mutex_destroy(&(env->sm->print_mutex));
	pthread_mutex_destroy(&(env->sm->running_mutex));
	pthread_mutex_destroy(&(env->sm->stats_mutex));
	pthread_mutex_destroy(&(env->sm->sched_mutex));
	pthread_cond_destroy(&(env->sm->sched_cond));
	free(env->sm);
	free(env->dongles);
	free(env->coders);
	free(env->threads);
}

int	main(int argc, char **argv)
{
	t_environment	env;
	pthread_t		monitor;
	pthread_t		scheduler;

	if (argc != 9)
		return (fprintf(stderr, "argument not complete\n"), 1);
	if (ft_init_all(argc, argv, &env) != 1)
		return (fprintf(stderr, "initialization failed\n"), 1);
	pthread_create(&monitor, NULL, ft_monitor, &env);
	pthread_create(&scheduler, NULL, ft_scheduler, &env);
	ft_join_threads(&env, env.sm->number_of_coders);
	pthread_join(monitor, NULL);
	pthread_join(scheduler, NULL);
	ft_cleanup(&env);
	return (0);
}
