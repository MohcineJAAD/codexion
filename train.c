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
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int counter = 0;
void *routin()
{
	int i = 0;
	while (i < 200)
	{
		i++;
		counter++;
	}
	return NULL;
}

int main()
{
	pthread_t	t1;
	pthread_t	t2;

	pthread_create(&t1, NULL, routin, NULL);
	pthread_create(&t2, NULL, routin, NULL);
	// sleep(5);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("%d\n", counter);
	printf("%ld\n", sizeof(pthread_t));
	printf("%p\n",pthread_t);
}