#include <stdio.h>
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
    while (i < len)
    {
        if (str[i] < 48 || str[i] > 57)
            return -1;
        i++;
    }
    return 1;
}

int ft_validator(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (ft_is_just_nums(argv[i]) == -1)
			return -1;
		i++;
	}
	return 1;
}
// int ft_init_simulation()

int main(int argc, char **argv)
{
    if (argc != 9)
        printf("argiment not valid\n");

        if (ft_validator(argc, argv) == 1)
			printf("Simulation start\n");
		else
			printf("Rejected: invalid inputs\n");
}