#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int *p = sbrk(sizeof(int));

    if (p == (void *)-1)
    {
        perror("sbrk failed");
        return 1;
    }

    *p = 42;
    printf("Address: %p\n", (void *)p);
    printf("Value: %d\n", *p);

    return 0;
}