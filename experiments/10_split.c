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
    int *a = kvalloc(100);

    if (a == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    kvfree(a);

    int *b = kvalloc(20);

    if (b == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    struct block *block_b = (struct block *)b - 1;

    printf("Used block:\n");
    printf("Address: %p\n", (void *)block_b);
    printf("Size: %zu\n", block_b->size);
    printf("Free: %d\n", block_b->free);
    printf("Next: %p\n", (void *)block_b->next);

    if (block_b->next != NULL)
    {
        printf("\nRemaining block:\n");
        printf("Address: %p\n", (void *)block_b->next);
        printf("Size: %zu\n", block_b->next->size);
        printf("Free: %d\n", block_b->next->free);
        printf("Next: %p\n", (void *)block_b->next->next);
    }

    return 0;
}