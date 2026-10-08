#include <stddef.h>
#include <stdio.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque apunta al siguiente
};

void *kvalloc(size_t size);
void kvfree(void *ptr);

int main(void)
{
    int *a = kvalloc(sizeof(int));
    int *b = kvalloc(sizeof(int));
    int *c = kvalloc(sizeof(int));

    if (a == NULL || b == NULL || c == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    *a = 10;
    *b = 20;
    *c = 30;

    struct block * block_a = (struct block *)a - 1;
    struct block * block_b = (struct block *)b - 1;
    struct block * block_c = (struct block *)c - 1;

    printf("Before:\n");
    printf("A free: %d\n", block_a->free);
    printf("B free: %d\n", block_b->free);
    printf("C free: %d\n", block_c->free);

    kvfree(b);

    printf("After:\n");
    printf("A free: %d\n", block_a->free);
    printf("B free: %d\n", block_b->free);
    printf("C free: %d\n", block_c->free);

    return 0;
}