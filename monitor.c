#include "codexion.h"

static int  ft_is_burnout(t_coder *cd)
{
	long	time_difference;

	time_difference = ft_get_time() - cd->last_compile_start;
	if (time_difference > cd->sim->time_to_burnout)
		return 1;
	return 0;
}

static	int	ft_is_compile_max(t_coder *cd)
{
	if (cd->compile_count >= cd->sim->number_of_compiles_required)
		return 1;
	else
		return 0;
}

static	void ft_shutdown(t_environment *env)
{
	int	i;

	pthread_mutex_lock(&(env->sm->running_mutex));
	env->sm->running = 0;
	pthread_mutex_unlock(&(env->sm->running_mutex));

	i = 0;
	while (i < env->sm->number_of_coders)
	{
		pthread_cond_signal(&(env->coders[i].cond));
		i++;
	}
}

void	ft_monitor(t_environment *env)
{
	int	i;
	int	all_compil;

	while (1)
	{
		usleep(1000);
		i = 0;
		all_compil = 1;
		while (i < env->sm->number_of_coders)
		{
			if (ft_is_burnout(&(env->coders[i])))
			{
				ft_print_log(&(env->coders[i]) ,"burned out\n" );
				ft_shutdown(env);
				return;
			}
			if (!ft_is_compile_max(&(env->coders[i])))
				all_compil = 0;
			i++;
		}
		if (all_compil)
		{
			ft_shutdown(env);
			return;
		}
	}
}
