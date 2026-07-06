/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 20:13:24 by mjaad             #+#    #+#             */
/*   Updated: 2026/07/06 19:29:50 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_overflowed(char *str)
{
	char	*max_int;
	int		len;
	int		i;

	max_int = "2147483647";
	if (*str == '+')
		str++;
	len = strlen(str);
	i = 0;
	if (len > 10)
		return (1);
	else if (len == 10)
	{
		while (i < 10)
		{
			if (str[i] > max_int[i])
				return (1);
			else if (str[i] < max_int[i])
				return (0);
			i++;
		}
	}
	return (0);
}

int	ft_is_just_nums(char *str)
{
	int	len;
	int	i;

	i = 0;
	len = strlen(str);
	if (len == 0)
		return (-1);
	if (str[i] == '+')
		i++;
	if (i == len)
		return (-1);
	while (i < len)
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	return (1);
}

void	*ft_clear(int *location)
{
	free(location);
	return (NULL);
}

int	is_valid_scheduler(char *scheduler)
{
	if (strcmp("fifo", scheduler) == 0)
		return (1);
	else if (strcmp("edf", scheduler) == 0)
		return (1);
	return (0);
}

int	*ft_validator(int argc, char **argv)
{
	int	i;
	int	*buffer;

	i = 1;
	buffer = malloc(sizeof(int) * 7);
	if (!buffer)
		return (NULL);
	if (is_valid_scheduler("fifo", argv[argc - 1]))
		return (ft_clear(buffer));
	while (i < argc - 1)
	{
		if (ft_is_just_nums(argv[i]) == -1 || is_overflowed(argv[i]))
			return (ft_clear(buffer));
		else
		{
			buffer[i - 1] = atoi(argv[i]);
			if (buffer[i - 1] < 0)
				return (ft_clear(buffer));
		}
		i++;
	}
	if (buffer[0] < 1)
		return (ft_clear(buffer));
	return (buffer);
}
