#include "codexion.h"

int ft_init_simulation(int argc, char **argv, Simulation **sm)
{
	int	i;
	int	*data;

	
	data = ft_validator(argc, argv);
	if (data == NULL) {
		return -1;
	}
	i = 0;
	*sm = malloc(sizeof(Simulation));
	if (!*sm)
		return (free(data), -1);
	(*sm)->number_of_coders = data[i++];
	(*sm)->time_to_burnout = data[i++];
	(*sm)->time_to_compile = data[i++];
	(*sm)->time_to_debug = data[i++];
	(*sm)->time_to_refactor = data[i++];
	(*sm)->number_of_compiles_required = data[i++];
	(*sm)->dongle_cooldown = data[i];
	(*sm)->scheduler = argv[argc - 1];
	free(data);
	return 1;
}

void ft_init_coders(t_coder **coder, t_dongle **dg, Simulation *sm)
{
	int			index;

	*coder = malloc(sizeof(t_coder) * sm->number_of_coders);
	if (!*coder)
		return;
	*dg = malloc(sizeof(t_dongle) * sm->number_of_coders);
	if (!*dg)
		return(free(*coder), (void)(*coder = NULL));
	index = 0;
	while (index < sm->number_of_coders)
	{
		(*dg)[index].id = index + 1;
		(*dg)[index].counter = 0;
		pthread_mutex_init(&((*dg)[index].mutex), NULL);
		(*coder)[index].id = index + 1;
		(*coder)[index].sim = sm;
		(*coder)[index].dongle_left = *dg + (index % sm->number_of_coders);
		(*coder)[index].dongle_right = *dg + ((index + 1) % sm->number_of_coders);
		index++;
	}
}


pthread_t	*ft_init_threads(t_coder *coders)
{
	int			i;
	pthread_t	*th;


	i = 0;
	th = malloc(sizeof(pthread_t) * coders->sim->number_of_coders);
	if (!th)
		return NULL;
	while(i < coders->sim->number_of_coders)
	{
		pthread_create(&th[i], NULL, coder_routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < coders->sim->number_of_coders)
	{
		pthread_join(th[i], NULL);
		i++;
	}
	return th;
}
