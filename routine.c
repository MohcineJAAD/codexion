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
		printf("%d has taken a dongle\n", cd->id);
		pthread_mutex_lock(&(cd->dongle_right->mutex));
		printf("%d has taken a dongle\n", cd->id);
		printf("%d is compiling\n", cd->id);
		usleep(cd->sim->time_to_compile * 1000);
		pthread_mutex_unlock(&(cd->dongle_left->mutex));
		pthread_mutex_unlock(&(cd->dongle_right->mutex));
		printf("%d is debugging\n", cd->id);
		usleep(cd->sim->time_to_debug * 1000);
		printf("%d is refactoring\n", cd->id);
		usleep(cd->sim->time_to_refactor * 1000);
		i++;
	}
	return NULL;
}