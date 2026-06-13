#define _GNU_SOURCE
#include <sched.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

#define STACK_SIZE (8 * 1024 * 1024)

int done = 0;

int routine(void *arg)
{
	(void)arg;
    printf("hello\n");
    return (0);
}

int main(void)
{
    void	*stack;
	void	*stack_top;
	int		flags;

	stack = mmap(NULL, STACK_SIZE, PROT_WRITE|PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (stack == MAP_FAILED)
	{
		perror("mmap\n");
		return (1);
	}
	stack_top = stack + STACK_SIZE;
	flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD ;
	clone(routine, stack_top, flags, NULL);
	syscall(SYS_futex, );
	munmap(stack, STACK_SIZE);
}