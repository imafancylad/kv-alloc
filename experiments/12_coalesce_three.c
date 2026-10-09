#include <stddef.h>
#include <stdio.h>

struct block {
    size_t size;
    int free;
    struct block *next;
};

void *kvalloc(size_t size);
void kvfree(void *ptr);

// esta prueba supone que la estructura ocupa 24 bytes

int main(void)
{
    void *a = kvalloc(20);
    void *b = kvalloc(30);
    void *c = kvalloc(40);

    if (a == NULL || b == NULL || c == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    struct block *block_a = (struct block *)a - 1;

    printf("Initial blocks:\n");
    printf("A: size=%zu, next=%p\n", block_a->size, (void *)block_a->next);
    printf("\nFreeing A, B and C...\n");

    kvfree(a);
    kvfree(b);
    kvfree(c);

    printf("\nAfter coalescing:\n");
    printf("A size: %zu\n", block_a->size);
    printf("A free: %d\n", block_a->free);
    printf("A next: %p\n", (void *)block_a->next);

    // printf("Metadata: %zu\n", sizeof(struct block));

    if (block_a->size == 138 && block_a->free == 1 && block_a->next == NULL)
    {
        printf("\nTest passed!\n");
    }
    else
    {
        printf("\nTest failed!\n");
    }

    return 0;
}