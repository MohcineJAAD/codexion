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
		i++;
	}
	pthread_mutex_destroy(&(env->sm->print_mutex));
	free(env->sm);
	free(env->dongles);
	free(env->coders);
	free(env->threads);
}

int	ft_is_burnout(t_coder *cd)
{
	long	time_difference;

	time_difference = ft_get_time() - cd->last_compile_start;
	if (time_difference > cd->sim->time_to_burnout)
		return 1;
	return 0;
}

void	ft_monitor(t_environment *env)
{
	int	i;
	int	all_done;
	while (1)
	{
		i = 0;
		all_done = 1;
		while (i < env->sm->number_of_coders)
		{
			if (ft_is_burnout(&(env->coders[i])))
			{
				pthread_mutex_lock(&(env->sm->running_mutex));
				env->sm->running = 0;
				pthread_mutex_unlock(&(env->sm->running_mutex));
				ft_print_log(&(env->coders[i]), "burned out\n");
				return;
			}
			else if (env->coders[i].compile_count < env->sm->number_of_compiles_required)
				all_done = 0;
			i++;
		}
		if (all_done)
		{
			pthread_mutex_lock(&(env->sm->running_mutex));
			env->sm->running = 0;
			pthread_mutex_unlock(&(env->sm->running_mutex));
			return;
		}
	}
	
}

int	main(int argc, char **argv)
{
	t_environment	env;

	if (argc != 9)
		return (fprintf(stderr, "argument not complete\n"), 1);
	if (ft_init_all(argc, argv, &env) != 1)
		return (fprintf(stderr, "initialization failed\n"), 1);
	ft_cleanup(&env);
	return (0);
}
