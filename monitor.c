#include "codexion.h"

static int  ft_is_burnout(t_coder *cd)
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
        usleep(1000);
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
