/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-31 18:53:15 by mjaad             #+#    #+#             */
/*   Updated: 2026-05-31 18:53:15 by mjaad            ###   ########.fr       */
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
		ft_destroy_queue(&(env->dongles[i].queue));
		i++;
	}
	pthread_mutex_destroy(&(env->sm->print_mutex));
	pthread_mutex_destroy(&(env->sm->running_mutex));
	free(env->sm);
	free(env->dongles);
	free(env->coders);
	free(env->threads);
}

int	main(int argc, char **argv)
{
	t_environment	env;

	if (argc != 9)
		return (fprintf(stderr, "argument not complete\n"), 1);
	if (ft_init_all(argc, argv, &env) != 1)
		return (fprintf(stderr, "initialization failed\n"), 1);
	ft_monitor(&env);
	ft_join_threads(&env, env.sm->number_of_coders);
	ft_cleanup(&env);
	return (0);
}
