#ifndef HEAP_H
# define HEAP_H
# include "codexion.h"

typedef struct s_coder t_coder;
typedef struct s_heap
{
    t_coder **heap;
    int size;
    int capacity;
    int (*cmp)(t_coder *, t_coder *);
} t_heap;

int ft_init_heap(t_heap *h, int capacity, int (*cmp)(t_coder*, t_coder*));
void ft_insert(t_heap *h, t_coder *cd);
t_coder* ft_extract_min(t_heap *h);
void ft_heap_destroy(t_heap *h);

#endif