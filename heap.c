/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-03 19:40:32 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-03 19:40:32 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

int	ft_init_heap(t_heap *h, int capacity, long (*cmp)(t_coder*, t_coder*))
{
	h->heap = malloc(sizeof(t_coder *) * capacity);
	if (!(h->heap))
		return (-1);
	h->size = 0;
	h->capacity = capacity;
	h->cmp = cmp;
	return (1);
}

void	ft_heapify_up(t_heap *h, int i)
{
	t_coder	*tmp;
	int		parent;

	tmp = h->heap[i];
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->cmp(h->heap[parent], tmp) > 0)
		{
			h->heap[i] = h->heap[parent];
			i = parent;
		}
		else
			break ;
	}
	h->heap[i] = tmp;
}

void	ft_insert(t_heap *h, t_coder *cd)
{
	h->heap[h->size] = cd;
	ft_heapify_up(h, h->size);
	h->size++;
}
