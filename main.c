#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_simulation
{
	int number_of_coders;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
	char *scheduler;

} Simulation;


int ft_is_just_nums(char *str)
{
    int len;
    int i;

    i = 0;
    len = strlen(str);
	if (len == 0)
		return -1;
    while (i < len)
    {
        if (str[i] < '0' || str[i] > '9')
            return -1;
        i++;
    }
    return 1;
}

void *ft_clear(int *location)
{
	free(location);
	return NULL;
}

int	*ft_validator(int argc, char **argv)
{
	int i;
	int value;
	int *buffer;

	i = 1;
	buffer = malloc(sizeof(int) * 7);
	if (strcmp("fifo", argv[argc - 1]) != 0 && strcmp("edf", argv[argc - 1]) != 0)
		return ft_clear(buffer);
	while (i < argc - 1)
	{
		if (ft_is_just_nums(argv[i]) == -1)
			return ft_clear(buffer);
		else
		{
			buffer[i - 1] = atoi(argv[i]);
			if (buffer[i - 1] <= 0)
				return ft_clear(buffer);				
		}
		i++;
	}
	return buffer;
}

int ft_init_simulation(int argc, char **argv, Simulation *sm)
{
	int i;
	int *data;

	data = ft_validator(argc, argv);
	if (data == NULL)
		return -1;

	i = 0;
	sm->number_of_coders = data[i++];
	sm->time_to_burnout = data[i++];
	sm->time_to_compile = data[i++];
	sm->time_to_debug = data[i++];
	sm->time_to_refactor = data[i++];
	sm->number_of_compiles_required = data[i++];
	sm->dongle_cooldown = data[i];
	sm->scheduler = argv[argc - 1];
	free(data);
	return 1;
}

int main(int argc, char **argv)
{
	Simulation sm;

    if (argc != 9)
    {
		printf("argument not complete\n");
		return 1;
	}
	if (ft_init_simulation(argc, argv, &sm) == 1)
	{
		printf("%d\n", sm.number_of_coders);
		printf("%d\n", sm.time_to_burnout);
		printf("%d\n", sm.time_to_compile);
		printf("%d\n", sm.time_to_debug);
		printf("%d\n", sm.time_to_refactor);
		printf("%d\n", sm.number_of_compiles_required);
		printf("%d\n", sm.dongle_cooldown);
		printf("%s\n", sm.scheduler);
	}
	else
		fprintf
}