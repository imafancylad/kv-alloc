#include <stddef.h>
#include <stdio.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque puede apuntar al siguiente
};

void *kvalloc(size_t size);
void kvfree(void *ptr);

int main(void)
{
    int *a = kvalloc(sizeof(int));
    int *b = kvalloc(sizeof(int));

    if (a == NULL || b == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    *a = 10;
    *b = 20;

    printf("a: %p -> %d\n", (void *)a, *a);
    printf("b: %p -> %d\n", (void *)b, *b);

    kvfree(a);

    int *c = kvalloc(sizeof(int));

    if (c == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    *c = 30;

    printf("c: %p -> %d\n", (void *)c, *c);

    if (c == a)
    {
        printf("Block reused!\n");
    }
    else
    {
        printf("New block allocated!\n");
    }

    return 0;
}