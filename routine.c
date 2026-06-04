#include "codexion.h"

static void ft_compile(t_coder *cd, int direction)
{
	cd->last_compile_start = ft_get_time();
	if (direction == 1)
	{
		pthread_mutex_lock(&(cd->dongle_right->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		pthread_mutex_lock(&(cd->dongle_left->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		ft_print_log(cd, "is compiling\n");
		usleep(cd->sim->time_to_compile * 1000);
		pthread_mutex_unlock(&(cd->dongle_right->mutex));
		pthread_mutex_unlock(&(cd->dongle_left->mutex));
	}
	else
	{
		pthread_mutex_lock(&(cd->dongle_left->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		pthread_mutex_lock(&(cd->dongle_right->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		ft_print_log(cd, "is compiling\n");
		usleep(cd->sim->time_to_compile * 1000);
		pthread_mutex_unlock(&(cd->dongle_left->mutex));
		pthread_mutex_unlock(&(cd->dongle_right->mutex));
	}
	pthread_mutex_lock(&(cd->sim->running_mutex));
	cd->compile_count++;
	pthread_mutex_unlock(&(cd->sim->running_mutex));
}

static void	ft_debugging(t_coder *cd)
{
	ft_print_log(cd, "is debugging\n");
	usleep(cd->sim->time_to_debug * 1000);
}

static void ft_refactoring(t_coder *cd)
{
	ft_print_log(cd, "is refactoring\n");
	usleep(cd->sim->time_to_refactor * 1000);
}

int		ft_is_running(t_simulation *sim)
{
	int	running;

	pthread_mutex_lock(&(sim->running_mutex));
	running = sim->running;
	pthread_mutex_unlock(&(sim->running_mutex));
	return (running);
}

void	*coder_routine(void *coder)
{
	t_coder *cd;
	int		i;

	cd = (t_coder *)coder;
	i = 0;
	while (ft_is_running(cd->sim))
	{
		if (cd->id == cd->sim->number_of_coders)
			ft_compile(cd, 1);
		else
			ft_compile(cd, 0);
		ft_debugging(cd);
		ft_refactoring(cd);
		i++;
	}
	return NULL;
}
