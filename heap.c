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

static void	ft_heapify_up(t_heap *h, int i)
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

static void	ft_heapify_down(t_heap *h, int index)
{
	t_coder	*tmp;
	int		left;
	int		right;
	int		smallest;

	left = (index * 2) + 1;
	right = (index * 2) + 2;
	smallest = index;
	if (left < h->size && h->cmp(h->heap[left], h->heap[smallest]) < 0)
		smallest = left;
	if (right < h->size && h->cmp(h->heap[right], h->heap[smallest]) < 0)
		smallest = right;
	if (smallest != index)
	{
		tmp = h->heap[smallest];
		h->heap[smallest] = h->heap[index];
		h->heap[index] = tmp;
		ft_heapify_down(h, smallest);
	}
}

t_coder	*ft_extract_min(t_heap *h)
{
	t_coder	*root;

	if (h->size == 0)
		return (NULL);
	root = h->heap[0];
	h->size--;
	h->heap[0] = h->heap[h->size];
	ft_heapify_down(h, 0);
	return (root);
}

void	ft_heap_destroy(t_heap *h)
{
	free(h->heap);
	h->heap = NULL;
	h->size = 0;
	h->capacity = 0;
}
