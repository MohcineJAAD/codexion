#include "codexion.h"

long ft_fifo_cmp(t_coder *cd1, t_coder *cd2)
{
	return (cd1->enqueue_time - cd2->enqueue_time);
}

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

static int ft_cooldown_remaining(t_dongle *dg, int time_cooldown)
{
    long	time_now;
    long	remaining;

	time_now = ft_get_time();
    remaining = time_cooldown - (time_now - dg->released_at) ;
    if (remaining > 0)
		return (remaining);
	return (0);
}

void	ft_fifo_release(t_dongle *dg)
{
	t_coder	*next;
	pthread_mutex_lock(&(dg->mutex));
	dg->released_at = ft_get_time();
	if (dg->heap.size > 0)
	{
		next = dg->heap.heap[0];
		pthread_cond_signal(&(next->cond));
	}
	pthread_mutex_unlock(&(dg->mutex));
}

static void ft_wait_cooldwon(t_coder *cd, t_dongle *dg)
{
	struct timeval	tv;
	struct timespec	ts;
	long			remaining;

	remaining = ft_cooldown_remaining(dg, cd->sim->dongle_cooldown);
}

int ft_fifo_acquire(t_coder *cd, t_dongle *dg)
{
	pthread_mutex_lock(&(dg->mutex));
	cd->enqueue_time = ft_get_time();
	ft_insert(&(dg->heap), cd);
	while(ft_is_running(cd->sim) && (ft_cooldown_remaining(dg,
		cd->sim->dongle_cooldown) || dg->heap.heap[0] != cd))
	{
		if (dg->heap.heap[0] == cd)
			ft_wait_cooldwon(&cd, &dg);
		else
			pthread_cond_wait(&(cd->cond), &(dg->mutex));

	}
	if (!ft_is_running(cd->sim))
	{
		ft_extract_min(&(dg->heap));
		pthread_mutex_unlock(&dg->mutex);
		return (1);
	}
	ft_extract_min(&(dg->heap));
	pthread_mutex_unlock(&dg->mutex);
	return 0;
}

