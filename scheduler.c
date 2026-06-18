#include "codexion.h"

int ft_init_fifo(t_environment *env)
{
    int i;

    i = 0;
    while (i < env->sm->number_of_coders)
    {
        env->dongles[i].queue.head = 0;
        env->dongles[i].queue.tail = 0;
        env->dongles[i].queue.size = 0;
        env->dongles[i].queue.queue = malloc(sizeof(t_coder *) * 2);
        if (!(env->dongles[i].queue.queue))
            return (-1);
        i++;
    }
    return (1);
}

void ft_init_edf(t_environment *env)
{
    (void)env;
}

int ft_fifo_manger(t_coder *cd, t_dongle *dg)
{
    pthread_mutex_lock(&(dg->mutex));
    dg->queue.queue[dg->queue.tail] = cd;
    dg->queue.tail = (dg->queue.tail + 1) % 2;
    dg->queue.size++;
    if (dg->queue.queue[dg->queue.head] != cd)
        pthread_cond_wait(&(cd->cond), &(dg->mutex));
}

int ft_init_scheduler(t_environment *env)
{
    if (strcmp(env->sm->scheduler, "fifo") == 0)
        return (ft_init_fifo(env));
    else
        ft_init_edf(env);
    return (1);
}
