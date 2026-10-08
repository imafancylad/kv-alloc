#include <stddef.h>
#include <stdio.h>

// el bug está recorriendo la lista enlazada antes de pedir el segundo bloque

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque apunta al siguiente
};

void *kvalloc(size_t size);
void kvfree(void *ptr);

int main(void)
{
    void *a = kvalloc(20);
    void *b = kvalloc(30);

    if (a == NULL || b == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    struct block *block_a = (struct block *)a - 1;
    struct block *block_b = (struct block *)b - 1;

    kvfree(a);
    kvfree(b);

    printf("\nAfter coalescing:\n");
    printf("A size: %zu\n", block_a->size);
    printf("A next: %p\n", (void *)block_a->next);

    printf("Block A:\n");
    printf("Address: %p\n", (void *)block_a);
    printf("Size: %zu\n", block_a->size);
    printf("Free: %d\n", block_a->free);
    printf("Next: %p\n", (void *)block_a->next);

    printf("\nBlock B:\n");
    printf("Address: %p\n", (void *)block_b);
    printf("Size: %zu\n", block_b->size);
    printf("Free: %d\n", block_b->free);
    printf("Next: %p\n", (void *)block_b->next);

    char *end_of_a = (char *)(block_a + 1) + block_a->size;

    printf("\nEnd of A data: %p\n", (void *)end_of_a);
    printf("Start of B: %p\n", (void *)block_b);

    if (end_of_a == (char *)block_b)
    {
        printf("\nBlocks are contiguous!\n");
    }
    else
    {
        printf("\nBlocks are NOT contiguous!\n");
    }
    return 0;
}
