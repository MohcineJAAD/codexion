#include <sys/mman.h>
#include <stdio.h>

int main()
{
    size_t stack_size;
    void *stack;
    stack_size = 8 * 1024 * 1024;
    stack = mmap(NULL, stack_size, PROT_WRITE|PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (stack == MAP_FAILED)
    {
        perror("mmap");
        return (1);
    }
    *((int *)stack) = 85;
    printf("%d\n", *((int *)stack));
    munmap(stack, stack_size);
}