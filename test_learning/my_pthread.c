#include "my_pthread.h"

typedef struct s_thread_desc
{
    int tid;
    void *stack;
} t_thread_desc;

#define STACK_SIZE (8 * 1024 * 1024)

int	ft_pthread_create(my_pthread_t *thread, void *(*start_routine)(void *), void *arg)
{
    void *stack;
    void *stack_top;
    t_thread_desc *desc;

    desc = malloc(sizeof(t_thread_desc));
    if (!desc)
        return -1;
    stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (stack == MAP_FAILED)
        return -1;
    stack_top = stack + STACK_SIZE;
    desc->tid = clone(start_routine);
    desc->stack = stack_top;

    return 0;
}