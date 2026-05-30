#include "codexion.h"

void	*coder_routine(void *coder)
{
	t_coder *cd;
	int i;

	cd = (t_coder *)coder;
	i = 0;
	while (i <100000)
	{
		pthread_mutex_lock(&(cd->dongle_left->mutex));
		cd->dongle_left->counter++;
		pthread_mutex_unlock(&(cd->dongle_left->mutex));
		pthread_mutex_lock(&(cd->dongle_right->mutex));
		cd->dongle_right->counter++;
		pthread_mutex_unlock(&(cd->dongle_right->mutex));
		i++;
	}
	
	printf("coder %d is alive\n", cd->id);
	return NULL;
}