# define _GNU_SOURCE
#ifndef MY_PTHREAD_H
# define MY_PTHREAD_H
#include <sched.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <unistd.h>

typedef unsigned long my_pthread_t;

int	ft_pthread_create(my_pthread_t *thread, void *(*start_routine)(void *), void *arg);
int	ft_pthread_join(my_pthread_t thread, void **retval);
#endif