#include "queue.h"

int	ft_init_queue(t_queue *q, int capacity)
{
    q->queue = malloc(sizeof(void *) * capacity);
    if (!(q->queue))
        return -1;
    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    return 1;
}

void	ft_enqueue(t_queue *q, void *item)
{
	q->queue[q->tail] = item;
	q->tail = (q->tail + 1) % q->capacity;
	q->size++;
}

void	*ft_dequeue(t_queue *q)
{
	void	*to_ret;
	to_ret = q->queue[q->head];
	q->head = (q->head + 1) % q->capacity;
	q->size--;
	return (to_ret);
}

void	ft_destroy_queue(t_queue *q)
{
	free(q->queue);
	q->queue = NULL;
	q->head = 0;
	q->size = 0;
	q->tail = 0;
}