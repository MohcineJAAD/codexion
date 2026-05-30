#include "codexion.h"

int	is_overflowed(char *str)
{
	char	*max_int;
	int 	len;
	int		i;

	max_int = "2147483647";
	len = strlen(str);
	i = 0;
	if (len > 11)
		return 1;
	else if (len == 11)
	{
		while (i < 11)
		{
			if (str[i] > max_int[i])
				return 1;
			i++;
		}
	}
	return 0;
}
int ft_is_just_nums(char *str)
{
    int len;
    int i;

    i = 0;
    len = strlen(str);
	if (len == 0)
		return -1;
	if (str[i] == '+') {i++;}
	if (i == len) {
		return -1;
	}
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
	int *buffer;

	i = 1;
	buffer = malloc(sizeof(int) * 7);
	if (strcmp("fifo", argv[argc - 1]) != 0 && strcmp("edf", argv[argc - 1]) != 0)
		return ft_clear(buffer);
	while (i < argc - 1)
	{
		
		if (ft_is_just_nums(argv[i]) == -1 || is_overflowed(argv[i])) {
			ft_clear(buffer);
			return NULL;
		}
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

