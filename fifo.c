#include "codexion.h"

int ft_init_fifo(t_environment *env)
{
    int i;

    i = 0;
    while (i < env->sm->number_of_coders)
    {
        if (ft_init_heap(&(env->dongles[i].heap), 2, ft_fifo_cmp) == -1)
            return (-1);
        i++;
    }
    return (1);
}

static int ft_is_in_cooldown(t_dongle *dg, int time_cooldown)
{
    long	time_now;
    long	in_cooldown;

	time_now = ft_get_time();
    in_cooldown = time_cooldown - (time_now - dg->released_at) ;
    if (in_cooldown > 0)
		return 1;
	return 0;
}

void	ft_fifo_release(t_dongle *dg)
{
	t_coder	*next;
	pthread_mutex_lock(&(dg->mutex));
	dg->released_at = ft_get_time();
	if (dg->queue.size > 0)
	{
		next = dg->queue.queue[dg->queue.head];
		pthread_cond_signal(&(next->cond));
	}
	pthread_mutex_unlock(&(dg->mutex));
}

int ft_fifo_acquire(t_coder *cd, t_dongle *dg)
{
	pthread_mutex_lock(&(dg->mutex));
	ft_enqueue(&(dg->queue), cd);
	while
	(
		ft_is_running(cd->sim) &&
		(
			ft_is_in_cooldown(dg, cd->sim->dongle_cooldown) || 
			dg->queue.queue[dg->queue.head] != cd
		)
	)
		pthread_cond_wait(&(cd->cond), &(dg->mutex));
	if (!ft_is_running(cd->sim))
	{
		ft_dequeue(&(dg->queue));
		pthread_mutex_unlock(&dg->mutex);
		return (1);
	}
	ft_dequeue(&(dg->queue));
	pthread_mutex_unlock(&dg->mutex);
	return 0;
}

int ft_fifo_cmp(t_coder *cd1, t_coder *cd2)
{
	return (cd1->enqueue_time - cd2->enqueue_time);
}