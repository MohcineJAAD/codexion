#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int data = 0;
int ready = 0;
pthread_mutex_t mutex;
pthread_cond_t  cond;

void *producer(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    data = 42;
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return (NULL);
}

void *consumer(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    if (!ready)
        pthread_cond_wait(&cond, &mutex);
    printf("data: %d\n", data);
    pthread_mutex_unlock(&mutex);
    return (NULL);
}
int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&t1, NULL, consumer, NULL);
    usleep(1000);
    pthread_create(&t2, NULL, producer, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}