#define _GNU_SOURCE

#include <sched.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <stdio.h>
#include <unistd.h>

#define STACK_SIZE (8 * 1024 * 1024)
int done = 0;

int routine(void *arg)
{
    (void)arg;
    printf("hello\n");
    done = 1;
    syscall(SYS_futex, &done, FUTEX_WAKE, 1, NULL, NULL, 0);
    return 0;
}

int main(void)
{
    void *stack;
    void *stack_top;
    int flags;
    stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (stack == MAP_FAILED)
    {
        perror("error: mmap failed");
        return 1;
    }
    stack_top = stack + STACK_SIZE;
    flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    clone(routine, stack_top, flags, NULL);
    syscall(SYS_futex, &done, FUTEX_WAIT, 0, NULL, NULL, 0);
    munmap(stack, STACK_SIZE);
    return 0;

}