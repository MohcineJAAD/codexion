#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_dongle
{
    int in_cooldown;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} t_dongle;

void *coder(void *arg)
{
    t_dongle *dg;

    dg = (t_dongle *)arg;
    pthread_mutex_lock(&(dg->mutex));
    while (dg->in_cooldown)
        pthread_cond_wait(&(dg->cond), &(dg->mutex));
    printf("coder took dongle\n");
    pthread_mutex_unlock(&(dg->mutex));
    return (NULL);
}

void *cooldown_timer(void *arg)
{
    t_dongle *dg;

    dg = (t_dongle *)arg;
    sleep(2);
    pthread_mutex_lock(&(dg->mutex));
    dg->in_cooldown = 0;
    pthread_cond_signal(&(dg->cond));
    pthread_mutex_unlock(&(dg->mutex));
    return (NULL);
}

int main()
{
    t_dongle dg;
    pthread_t coder_t, cool_t;
    pthread_mutex_init(&(dg.mutex), NULL);
    pthread_cond_init(&(dg.cond), NULL);
    dg.in_cooldown = 1;
    pthread_create(&coder_t, NULL, coder, &dg);
    pthread_create(&cool_t, NULL, cooldown_timer, &dg);
    pthread_join(coder_t, NULL);
    pthread_join(cool_t, NULL);
    pthread_mutex_destroy(&(dg.mutex));
    pthread_cond_destroy(&(dg.cond));
}
