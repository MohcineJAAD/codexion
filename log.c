#include "codexion.h"

void	ft_print_log(t_coder *cd, char *log)
{
	long	time;

	pthread_mutex_lock(&(cd->sim->print_mutex));
	time = ft_get_time() - cd->sim->start_time;
	printf("%ld %d %s", time, cd->id, log);
	pthread_mutex_unlock(&(cd->sim->print_mutex));
}