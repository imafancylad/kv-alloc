#include <stddef.h>
#include <stdio.h>

void *kvalloc(size_t size);

int main(void)
{
    int *a = kvalloc(sizeof(int));
    int *b = kvalloc(sizeof(int));
    int *c = kvalloc(sizeof(int));

    if (a == NULL || b == NULL || c == NULL)
    {
        perror("kvalloc failed");
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