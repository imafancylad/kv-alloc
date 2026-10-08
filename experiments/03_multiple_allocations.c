#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int *a = sbrk(sizeof(int));
    int *b = sbrk(sizeof(int));
    int *c = sbrk(sizeof(int));

    if (a == (void *)-1 || b == (void *)-1 || c == (void *)-1)
    {
        perror("sbrk failed");
        return 1;
    }

    *a = 10;
    *b = 20;
    *c = 30;

    printf("a: %p -> %d\n", (void *)a, *a);
    printf("b: %p -> %d\n", (void *)b, *b);
    printf("c: %p -> %d\n", (void *)c, *c);

    return 0;
}