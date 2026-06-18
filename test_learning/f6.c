#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t  cond;
int             ready = 0;

void *waiter(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    printf("waiter: locked mutex\n");
    printf("waiter: checking ready\n");
    while (!ready)
    {
        printf("waiter: not ready, going to sleep\n");
        pthread_cond_wait(&cond, &mutex);
        printf("waiter: woke up");
    }
    printf("waiter: ready is 1, continuing\n");
    pthread_mutex_unlock(&mutex);
    return (NULL);
}

void *signaler(void *arg)
{
    (void)arg;
    sleep(1);
    printf("signaler: locking mutex\n");
    pthread_mutex_lock(&mutex);
    printf("signaler: setting ready = 1\n");
    ready = 1;
    printf("signaler: sending signal\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return (NULL);
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&t1, NULL, waiter, NULL);
    pthread_create(&t2, NULL, signaler, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}