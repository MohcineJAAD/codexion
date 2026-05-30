#include "codexion.h"

int main(int argc, char **argv)
{
	Simulation	*sm;
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	*threads;
	int			i;
	

    if (argc != 9)
    {
		fprintf(stderr, "argument not complete\n");
		return 1;
	}
	if (ft_init_simulation(argc, argv, &sm) == 1)
	{
		ft_init_coders(&coders, &dongles, sm);
		threads = ft_init_threads(coders);
		i = 0;
		while(i < sm->number_of_coders)
		{
			printf("dongle id '%d' counter = %d\n", dongles[i].id, dongles[i].counter);
			pthread_mutex_destroy(&(dongles[i].mutex));
			i++;
		}
		free(sm);
		free(dongles);
		free(coders);
		free(threads);
	}
	else
		fprintf(stderr, "Rejected: invalid inputs\n");
}
