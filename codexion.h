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
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include "heap.h"

typedef struct s_coder t_coder;

typedef struct s_simulation
{
	pthread_mutex_t print_mutex;
	pthread_mutex_t	running_mutex;
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	long			start_time;
	int				running;
}	t_simulation;

typedef struct s_dongle
{
	int				id;
	long			released_at;
	pthread_mutex_t	mutex;
	t_heap			heap;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile_start;
	long			enqueue_time;
	t_dongle		*dongle_left;
	t_dongle		*dongle_right;
	t_simulation	*sim;
	pthread_cond_t	cond;
}	t_coder;

typedef struct s_environment
{
	t_simulation	*sm;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*threads;
}	t_environment;



void	*ft_clear(int *location);
void	*coder_routine(void *coder);
int		ft_init_threads(t_environment *env);
int		*ft_validator(int argc, char **argv);
int		is_overflowed(char *str);
int		ft_is_just_nums(char *str);
int		ft_init_simulation(int argc, char **argv, t_simulation **sm);
int		ft_init_coders(t_coder **coder, t_dongle **dg, t_simulation *sm);
int		ft_init_all(int argc, char **argv, t_environment *env);
void	ft_join_threads(t_environment *env, int index);
long    ft_get_time(void);
void	ft_print_log(t_coder *cd, char *log);
void	*ft_monitor(void *env);
int		ft_is_running(t_simulation *sim);
int		ft_init_scheduler(t_environment *env);
int		ft_init_fifo(t_environment *env);
void	ft_fifo_release(t_dongle *dg);
int		ft_fifo_acquire(t_coder *cd, t_dongle *dg);
int		ft_acquire(t_coder *cd, t_dongle *dg);
void	ft_release(t_dongle *dg, t_simulation *sim);
int		ft_edf_acquire(t_coder *cd, t_dongle *dg);
void	ft_edf_release(t_dongle *dg);
int		ft_init_edf(t_environment *env);

#endif