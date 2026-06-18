#ifndef QUEUE_H
# define QUEUE_H
# include <stdlib.h>

typedef struct s_queue
{
	void	**queue;
	int		head;
	int		tail;
	int		size;
	int		capacity;
} t_queue;

int		ft_init_queue(t_queue *q, int capacity);
void	ft_enqueue(t_queue *q, void *item);
void	*ft_dequeue(t_queue *q);
void	ft_destroy_queue(t_queue *q);
#endif