#include "../queue.h"
#include <stdio.h>

int main()
{
    t_queue qu;
    ft_init_queue(&qu, 3);
    ft_enqueue(&qu, "Mohcine");
    ft_enqueue(&qu, "jaad");
    ft_enqueue(&qu, ":)");
    printf("%s\n", (char *)ft_dequeue(&qu));
    printf("%s\n", (char *)ft_dequeue(&qu));
    printf("%s\n", (char *)ft_dequeue(&qu));
    ft_destroy_queue(&qu);
}