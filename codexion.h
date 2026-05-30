/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-29 15:41:12 by mjaad             #+#    #+#             */
/*   Updated: 2026-05-29 15:41:12 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct s_simulation
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;

}	Simulation;

typedef struct s_dongle{
	int				id;
	int				counter;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	t_dongle	*dongle_left;
	t_dongle	*dongle_right;
	Simulation	*sim;
}	t_coder;


int	is_overflowed(char *str);
int	ft_is_just_nums(char *str);
int	*ft_validator(int argc, char **argv);
void *ft_clear(int *location);
void	*coder_routine(void *coder);
int ft_init_simulation(int argc, char **argv, Simulation **sm);
void ft_init_coders(t_coder **coder, t_dongle **dg, Simulation *sm);
pthread_t	*ft_init_threads(t_coder *coders);


#endif