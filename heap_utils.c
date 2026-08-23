/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjaad <mjaad@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-07-11 13:36:41 by mjaad             #+#    #+#             */
/*   Updated: 2026-07-11 13:36:41 by mjaad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

void	ft_heapify_down(t_heap *h, int index)
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

void	ft_remove_from_heap(t_heap *h, int index)
{
	h->size--;
	h->heap[index] = h->heap[h->size];
	ft_heapify_up(h, index);
	ft_heapify_down(h, index);
}
