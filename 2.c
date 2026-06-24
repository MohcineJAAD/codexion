#include <stdlib.h>

typedef struct s_heap
{
    int *heap;
    int size;
    int capacity;
} t_heap;

int ft_create_heap(t_heap *h, int capacity)
{
    h->heap = malloc(sizeof(int) * capacity);
    if (!(h->heap))
        return (-1);
    h->capacity = capacity;
    h->size = 0;
    return 1;
}


void ft_heapify_down(t_heap *h, int index)
{
    int left;
    int right;
    int biggest;
    int tmp;

    left = (index * 2) + 1;
    right = (index * 2) + 2;
    biggest = index;
    if (left < h->size && h->heap[left] > h->heap[biggest])
        biggest = left;
    if (right < h->size && h->heap[right] > h->heap[biggest])
        biggest = right;
    if (biggest != index)
    {
        tmp = h->heap[biggest];
        h->heap[biggest] = h->heap[index];
        h->heap[index] = tmp;
        ft_heapify_down(h, biggest);
    }
}

int ft_extract_max(t_heap *h)
{
    int root;

    root = h->heap[0];
    h->heap[0] = h->heap[h->size - 1];
    h->size--;
    ft_heapify_down(h, 0);
    return root;
}

void ft_heapify_up(t_heap *h, int index)
{
    int tmp;
    int parent;

    tmp = h->heap[index];
    parent = (index - 1) / 2;
    while (index > 0)
    {
        if (h->heap[parent] > tmp)
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

int ft_insert(t_heap *h, int node)
{
    int *tmp;
    if (h->size >= h->capacity)
    {
        h->capacity *= 2;
        tmp = realloc(h->heap, sizeof(int) * h->capacity);
        if (!tmp)
            return (-1);
        h->heap = tmp;
    }
    h->heap[h->size] = node;
    ft_heapify_up(h, h->size);
    h->size++;
    return (1);
}

int main()
{
    t_heap *h;
    int arr[] = {3, 1, 6, 2, 9, 4};
    ft_create_heap(h, 10);
    for (int i = 0; i < 6; i++)
        ft_insert(h, arr[i]);
}