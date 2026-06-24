#include "heap.h"

int ft_init_heap(t_heap *h, int capacity, int (*cmp)(t_coder*, t_coder*))
{
    h->heap = malloc(sizeof(t_coder *) * capacity);
    if (!(h->heap))
        return (-1);
    h->size = 0;
    h->capacity = capacity;
    h->cmp = cmp;
    return (1);
}

static void ft_heapify_up(t_heap *h, int i)
{
    t_coder *tmp;
    int     parent;

    if (i == 0)
        return ;
    tmp = h->heap[i];
    parent = (i - 1) / 2;
    if (h->cmp(h->heap[parent], tmp) > 0)
    {
        h->heap[i] = h->heap[parent];
        i = parent;
    }
    h->heap[i] = tmp;
}
void ft_insert(t_heap *h, t_coder *cd)
{
    h->heap[h->size] = cd;
    ft_heapify_up(h, h->size);
    h->size++;
}

t_coder *ft_extract_min(t_heap *h)
{
    t_coder *root;

    if (h->size == 0)
        return NULL;
    root = h->heap[0];
    h->size--;
    h->heap[0] = h->heap[h->size];
    return (root);
}

void ft_heap_destroy(t_heap *h)
{
    free(h->heap);
    h->heap = NULL;
    h->size = 0;
    h->capacity = 0;
}