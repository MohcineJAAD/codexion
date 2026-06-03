#include "codexion.h"

void	*coder_routine(void *coder)
{
	t_coder *cd;
	int		i;

	cd = (t_coder *)coder;
	i = 0;
	while (i < cd->sim->number_of_compiles_required)
	{
		pthread_mutex_lock(&(cd->dongle_left->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		pthread_mutex_lock(&(cd->dongle_right->mutex));
		ft_print_log(cd, "has taken a dongle\n");
		ft_print_log(cd, "is compiling\n");
		usleep(cd->sim->time_to_compile * 1000);
		pthread_mutex_unlock(&(cd->dongle_left->mutex));
		pthread_mutex_unlock(&(cd->dongle_right->mutex));
		ft_print_log(cd, "is debugging\n");
		usleep(cd->sim->time_to_debug * 1000);
		ft_print_log(cd, "is refactoring\n");
		usleep(cd->sim->time_to_refactor * 1000);
		i++;
	}
	return NULL;
}
