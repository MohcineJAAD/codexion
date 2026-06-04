/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-31 19:02:14 by mjaad             #+#    #+#             */
/*   Updated: 2026-05-31 19:02:14 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_init_simulation(int argc, char **argv, t_simulation **sm)
{
	int	i;
	int	*data;

	data = ft_validator(argc, argv);
	if (data == NULL)
		return (-1);
	i = 0;
	*sm = malloc(sizeof(t_simulation));
	if (!*sm)
	{
		free(data);
		return (-1);
	}
	(*sm)->number_of_coders = data[i++];
	(*sm)->time_to_burnout = data[i++];
	(*sm)->time_to_compile = data[i++];
	(*sm)->time_to_debug = data[i++];
	(*sm)->time_to_refactor = data[i++];
	(*sm)->number_of_compiles_required = data[i++];
	(*sm)->dongle_cooldown = data[i];
	(*sm)->scheduler = argv[argc - 1];
	free(data);
	return (1);
}

int	ft_init_coders(t_coder **coder, t_dongle **dg, t_simulation *sm)
{
	int	index;

	*coder = malloc(sizeof(t_coder) * sm->number_of_coders);
	if (!*coder)
		return (-1);
	*dg = malloc(sizeof(t_dongle) * sm->number_of_coders);
	if (!*dg)
		return (free(*coder), *coder = NULL, -1);
	index = 0;
	while (index < sm->number_of_coders)
	{
		(*dg)[index].id = index + 1;
		pthread_mutex_init(&((*dg)[index].mutex), NULL);
		(*coder)[index].id = index + 1;
		(*coder)[index].sim = sm;
		(*coder)[index].compile_count = 0;
		(*coder)[index].last_compile_start = 0;
		(*coder)[index].dongle_left = *dg + (index % sm->number_of_coders);
		(*coder)[index].dongle_right = *dg + ((index + 1) % sm->number_of_coders);
		index++;
	}
	return (1);
}

void	ft_join_threads(t_environment *env, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		pthread_join(env->threads[i], NULL);
		i++;
	}
}

int	ft_init_threads(t_environment *env)
{
	int			i;

	env->threads = malloc(sizeof(pthread_t) * env->sm->number_of_coders);
	if (!env->threads)
		return (-1);
	i = 0;
	env->sm->start_time = ft_get_time();
	env->sm->running = 1;
	pthread_mutex_init(&(env->sm->running_mutex), NULL);
	pthread_mutex_init(&(env->sm->print_mutex), NULL);
	while (i < env->sm->number_of_coders)
	{
		if (pthread_create(&env->threads[i], NULL, coder_routine, &env->coders[i]) != 0)
		{
			ft_join_threads(env, i);
			free(env->threads);
			env->threads = NULL;
			return (-1);
		}
		i++;
	}
	return (1);
}

int	ft_init_all(int argc, char **argv, t_environment *env)
{
	int	i;

	if (ft_init_simulation(argc, argv, &(env->sm)) != 1)
		return (-1);
	if (ft_init_coders(&(env->coders), &(env->dongles), env->sm) != 1)
	{
		free(env->sm);
		return (-1);
	}
	if (ft_init_threads(env) != 1)
	{
		i = 0;
		while (i < env->sm->number_of_coders)
			pthread_mutex_destroy(&(env->dongles[i++].mutex));
		free(env->sm);
		free(env->coders);
		free(env->dongles);
		return (-1);
	}
	return (1);
}
