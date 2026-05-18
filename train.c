// #include <stdio.h>
// #include <pthread.h>

// void *routine(void *arg)
// {
// 	int	*nbr;

// 	nbr = (int*)arg;
// 	printf("Thread use %d\n", *nbr);
// 	return (NULL);
// }

// int main()
// {
//     pthread_t	tid;
//     int			nbr;

// 	nbr = 42;
// 	pthread_create(&tid, NULL, routine, &nbr);
// 	pthread_join(tid, NULL);
// }

// =========================================

// #include <stdio.h>
// #include <pthread.h>
// #include <stdlib.h>
// #include <unistd.h>


// void *routine(void *index)
// {
// 	int	*i;
// 	i = (int *)index;

// 	int j = 0;
// 	while (j < 100000){
// 		(*i)++;
// 		j++;
// 	}
// 	return NULL;

// }

// int main()
// {
// 	pthread_t t1;
// 	pthread_t t2;
// 	int			i;

// 	i = 0;
// 	pthread_create(&t1, NULL, routine, &i);
// 	pthread_create(&t2, NULL, routine, &i);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// 	printf("%d\n", i);
// }


// #include <stdio.h>
// #include <pthread.h>
// #include <stdlib.h>
// #include <unistd.h>

// pthread_mutex_t mutex;

// void *routine(void *index)
// {
// 	int *i = (int *)index;

// 	int j = 0;
// 	while (j < 100)
// 	{
// 		printf("Oualid khiar\n");
// 		j++;
// 	}
// 	return NULL;
// }

// int main()
// {
// 	pthread_t t1;
// 	pthread_t t2;
// 	int i;

// 	i = 0;

// 	pthread_mutex_init(&mutex, NULL);

// 	pthread_create(&t1, NULL, routine, &i);
// 	pthread_create(&t2, NULL, routine, &i);

// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);

// 	pthread_mutex_destroy(&mutex);

// 	printf("%d\n", i);
// }

// ##########################
// Exercise 1: Create one thread that prints "I am a thread", then join it.
// #include <stdio.h>
// #include <pthread.h>

// void *routine(void *str)
// {
// 	printf("Hello I am thread\n");
// 	return NULL;
// }

// int main()
// {
// 	pthread_t t1;
// 	pthread_create(&t1, NULL, routine, "I am a thread");
// 	pthread_join(t1, NULL);
// }


// ##########################
// Exercise 2: Create one thread that receives an int and prints "My number is X".
// #include <stdio.h>
// #include <pthread.h>

// void *routine(void *nbr)
// {
// 	printf("My number is %d\n", *(int *)nbr);
// 	return NULL;
// }

// int main()
// {
// 	pthread_t t1;
// 	int nbr = 15;
// 	pthread_create(&t1, NULL, routine, &nbr);
// 	pthread_join(t1, NULL);
// }

// ##########################
// Exercise 3: Create 3 threads. Give each a different string ("Alice", "Bob", "Carol").
// #include <stdio.h>
// #include <pthread.h>

// void *routine(void *nbr)
// {
// 	printf("Hello, I am [%s]\n", (char *)nbr);
// 	return NULL;
// }
 
// int main()
// {
// 	pthread_t t1;
// 	pthread_t t2;
// 	pthread_t t3;
// 	int nbr = 15;
// 	pthread_create(&t1, NULL, routine, "Alice");
// 	pthread_create(&t2, NULL, routine, "Bob");
// 	pthread_create(&t3, NULL, routine, "Carol");
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// 	pthread_join(t3, NULL);
// }

// Exercise 4: Create 5 threads, each receives an int id (1–5). Each prints "Coder [id] started". Join all. Run multiple times.
// #include <stdio.h>
// #include <pthread.h>
// void *routine(void *nbr)
// {
// 	printf("Coder [%d] started\n", *((int *)nbr));
// 	return NULL;
// }

// int main()
// {
// 	pthread_t	threads[5];
// 	int			ids[5];
// 	for (int i = 0; i < 5; i++)
// 	{
// 		ids[i] = i + 1;
// 		pthread_create(&threads[i], NULL, routine, &ids[i]);
// 	}
// 	for (int i = 0; i < 5; i++)
// 	{
// 		pthread_join(threads[i], NULL);
// 	}
// }

// visualisation the problem of concurency
// #include <stdio.h>
// #include <unistd.h>
// #include <pthread.h>

// int counter = 0;
// void *routin()
// {
// 	int i = 0;
// 	while (i < 200)
// 	{
// 		i++;
// 		counter++;
// 	}
// 	return NULL;
// }

// int main()
// {
// 	pthread_t	t1;
// 	pthread_t	t2;

// 	pthread_create(&t1, NULL, routin, NULL);
// 	pthread_create(&t2, NULL, routin, NULL);
// 	// sleep(5);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// 	printf("%d\n", counter);
// 	printf("%ld\n", sizeof(pthread_t));
// 	printf("%p\n",pthread_t);
// }


// #include <stdio.h>
// #include <pthread.h>
// #include <stdlib.h>
// #include <time.h>

// void *routine()
// {
// 	int res;
// 	res = (rand() % 6) + 1;
// 	// printf("%d\n", res);
// 	int *ptr = malloc(sizeof(int));
// 	*ptr = res;
// 	return (void *)ptr;
// }

// int main()
// {
// 	pthread_t t1;
// 	int *ret;
// 	srand(time(NULL));
// 	pthread_create(&t1, NULL, routine, NULL);
// 	pthread_join(t1, (void**)&ret);
// 	printf("%d\n", *ret);
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <string.h>


// int arr[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// void *routine(void *arg)
// {
// 	// sleep(1);
// 	int index = *(int *)arg;
// 	printf("index = %d\n", index);
// 	int acc = 0;

// 	for (int i = 0; i < 5; i++)
// 		acc += arr[i + index];
// 	printf("acc = %d\n", acc);
// 	free(arg);
// 	return NULL;
// }

// int main()
// {
// 	pthread_t threads[2];
// 	for (int i = 0; i < 2; i++)
// 	{
// 		int *a = malloc(sizeof(int));
// 		*a = i * 5;
// 		pthread_create(threads + i, NULL, &routine, a);
// 	}
// 	for (int i = 0; i < 2; i++)
// 		pthread_join(threads[i], NULL);
// }

// #include <stdio.h>
// #include <unistd.h>
// #include <pthread.h>

// int mails = 0;

// void *routine()
// {
// 	for (int i = 0; i < 10000; i++)
// 		mails += 1;
// 	return NULL;
// }

// int main()
// {
// 	pthread_t t1, t2;

// 	pthread_create(&t1, NULL, routine, NULL);
// 	pthread_create(&t2, NULL, routine, NULL);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// 	printf("%d\n", mails);
// }

// Exercise 1 — fix the race condition
// #include <stdio.h>
// #include <pthread.h>

// int counter = 0;
// pthread_mutex_t mutex;

// void *routine()
// {
// 	int i;

// 	i = 0;
// 	while (i < 100000)
// 	{
// 		pthread_mutex_lock(&mutex);
// 		counter++;
// 		pthread_mutex_unlock(&mutex);
// 		i++;
// 	}
// 	return (NULL);
// }

// int main()
// {
// 	pthread_t t1;
// 	pthread_t t2;
// 	pthread_t t3;

// 	pthread_mutex_init(&mutex, NULL);
// 	pthread_create(&t1, NULL, &routine, NULL);
// 	pthread_create(&t2, NULL, &routine, NULL);
// 	pthread_create(&t3, NULL, routine, NULL);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// 	pthread_join(t3, NULL);
// 	pthread_mutex_destroy(&mutex);
// 	printf("%d\n", counter);
// }

// Exercise 2 — protect a print
// #include <stdio.h>
// #include <pthread.h>

// pthread_mutex_t mutxt;

// void *routine(void *arg)
// {
// 	int i;

// 	i = 0;
// 	while (i < 5)
// 	{
// 		pthread_mutex_lock(&mutxt);
// 		printf("thread: %d i = %d\n", *(int *)arg, i);
// 		pthread_mutex_unlock(&mutxt);
// 		i++;
// 	}
// 	return (NULL);
// }

// int main()
// {
// 	pthread_t th1;
// 	pthread_t th2;
// 	int id1 = 1;
// 	int id2 = 2;
// 	pthread_mutex_init(&mutxt, NULL);
// 	pthread_create(&th1, NULL, routine, &id1);
// 	pthread_create(&th2, NULL, routine, &id2);
// 	pthread_join(th1, NULL);
// 	pthread_join(th2, NULL);
// 	pthread_mutex_destroy(&mutxt);
// }

// #include <stdio.h>
// #include <pthread.h>

// typedef struct {
// 	int total;
// 	int count;
// } t_stats;

// t_stats stats = {0, 0};
// pthread_mutex_t mutex;

// void *routine(void *arg)
// {
// 	int i;

// 	i = 0;
// 	while (i < 10000)
// 	{
// 		pthread_mutex_lock(&mutex);
// 		stats.total += *(int *)arg;
// 		stats.count++;
// 		i++;
// 		pthread_mutex_unlock(&mutex);
// 	}
// 	return (NULL);
// }

// int main()
// {
// 	pthread_t th1;
// 	pthread_t th2;
// 	int v1 = 3;
// 	int v2 = 7;
// 	pthread_mutex_init(&mutex, NULL);
// 	pthread_create(&th1, NULL, routine, &v1);
// 	pthread_create(&th2, NULL, routine, &v2);
// 	pthread_join(th1, NULL);
// 	pthread_join(th2, NULL);
// 	pthread_mutex_destroy(&mutex);
// 	printf("total: %d\n", stats.total);
//     printf("count: %d\n", stats.count);
//     printf("expected total: %d\n", (10000 * 3) + (10000 * 7));
//     printf("expected count: %d\n", 20000);
// }

// #include <stdio.h>
// #include <pthread.h>

// typedef struct {
// 	int counter;
// 	pthread_mutex_t mutex;
// } t_shared;

// void *routine(void *arg)
// {
// 	t_shared *	dt;
// 	int i;

// 	i = 0;	
// 	dt = (t_shared *)arg;
// 	while (i < 50)
// 	{
// 		pthread_mutex_lock(&dt->mutex);
// 		dt->counter++;
// 		pthread_mutex_unlock(&dt->mutex);
// 		i++;
// 	}
// 	return (NULL);
// }

// int main()
// {
// 	t_shared dt;
// 	pthread_t t1;
// 	pthread_t t2;

// 	dt.counter = 0;
// 	pthread_mutex_init(&dt.mutex, NULL);
// 	pthread_create(&t1, NULL, routine, &dt);
// 	pthread_create(&t2, NULL, routine, &dt);
// 	pthread_join(t1, NULL);
// 	pthread_join(t2, NULL);
// 	pthread_mutex_destroy(&dt.mutex);
// 	printf("%d\n", dt.counter);
// 	printf("%lu\n", sizeof(pthread_mutex_t));
// }

#include <stdio.h>
#include <pthread.h>

int main()
{
    pthread_mutex_t m;

    pthread_mutex_init(&m, NULL);
    printf("after init:   __lock = %d\n", m.__data.__lock);

    pthread_mutex_lock(&m);
    printf("after lock:   __lock = %d\n", m.__data.__lock);

    pthread_mutex_unlock(&m);
    printf("after unlock: __lock = %d\n", m.__data.__lock);

    pthread_mutex_destroy(&m);
    return (0);
}