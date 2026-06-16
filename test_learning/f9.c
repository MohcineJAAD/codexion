#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
int             ready = 0;

void *waiter(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    while (ready == 0)
        pthread_cond_wait(&cond, &mutex);
    printf("The waiter got the signal, ready = %d\n", ready);
    pthread_mutex_unlock(&mutex);
    return (NULL);
}

void *signaler(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    ready = 1;
    printf("signaler: setting ready and signaling\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return (NULL);
}

int main()
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, waiter, NULL);
    pthread_create(&t2, NULL, signaler, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return (0);
}
