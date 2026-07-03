/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 19:50:25 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 19:50:25 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H
# include <stdlib.h>

typedef struct s_coder	t_coder;
typedef struct s_heap
{
	t_coder	**heap;
	int		size;
	int		capacity;
	long	(*cmp)(t_coder *, t_coder *);
}	t_heap;

t_coder	*ft_extract_min(t_heap *h);
int		ft_init_heap(t_heap *h, int capacity, long (*cmp)(t_coder*, t_coder*));
void	ft_insert(t_heap *h, t_coder *cd);
void	ft_heap_destroy(t_heap *h);

#endif