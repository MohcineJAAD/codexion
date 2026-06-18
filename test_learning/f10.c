#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int fuel = 0;
pthread_mutex_t pmt;
pthread_cond_t  cond;

void *full_gas(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&pmt);
        fuel += 60;
        printf("Filled fuel...%d\n", fuel);
        pthread_mutex_unlock(&pmt);
        pthread_cond_broadcast(&cond);
        sleep(1);
    }
    return (NULL);
}

void *car(void *arg)
{
    pthread_mutex_lock(&pmt);
    while (fuel < 40)
    {
        printf("no fuel... waiting\n");
        pthread_cond_wait(&cond, &pmt);
    }
    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&pmt);
    return (NULL);
}

int main()
{
    pthread_t th[5];

    pthread_mutex_init(&pmt, NULL);
    pthread_cond_init(&cond, NULL);
    for (int i = 0; i < 5; i++)
    {
        if (i != 4)
            pthread_create(&th[i], NULL, car, NULL);
        else
            pthread_create(&th[i], NULL, full_gas, NULL);
    }
    for (int i = 0; i < 5; i++)
        pthread_join(th[i], NULL);
    pthread_mutex_destroy(&pmt);
    pthread_cond_destroy(&cond);
}