#include <stdio.h>
#include <stdlib.h>
typedef struct s_heap
{
    int *heap;
    int size;
    int capacity;
} t_heap;

int ft_create_heap(t_heap *h, int capaciter)
{
    h->heap = malloc(sizeof(int) * capaciter);
    if (!(h->heap))
        return (-1);
    h->size = 0;
    h->capacity = capaciter;
    return (1);
}

void ft_heapify_up(t_heap *h, int index)
{
    int parent;
    int tmp;
    int pos;

    parent = (index - 1)/2;
    tmp = h->heap[index];
    while(index > 0)
    {
        if (h->heap[parent] < tmp)
        {
            h->heap[index] = h->heap[parent];
            index = parent;
            parent = (index - 1) / 2;
        }
        else
            break;
    }
    h->heap[index] = tmp;
}

int ft_insert(t_heap *h, int value)
{
    int *tmp;

    if (h->size >= h->capacity)
    {
        tmp = realloc(h->heap, (h->capacity * 2) * sizeof(int) );
        if (!tmp)
            return (-1);
        h->capacity *= 2;
        h->heap = tmp;
    }
    h->heap[h->size] = value;
    ft_heapify_up(h, h->size);
    h->size += 1;
    return (1);
}