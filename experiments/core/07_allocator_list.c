#include <stddef.h>
#include <stdio.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque puede apuntar al siguiente
};

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

    struct block *block_a = (struct block *)a - 1;
    struct block *block_b = (struct block *)b - 1;
    struct block *block_c = (struct block *)c - 1;

    printf("A: %p | Size: %zu | Free: %d | Next: %p\n",
           (void *)block_a, block_a->size, block_a->free, (void *)block_a->next);

    printf("B: %p | Size: %zu | Free: %d | Next: %p\n",
           (void *)block_b, block_b->size, block_b->free, (void *)block_b->next);

    printf("C: %p | Size: %zu | Free: %d | Next: %p\n",
           (void *)block_c, block_c->size, block_c->free, (void *)block_c->next);

    printf("Values: %d %d %d\n", *a, *b, *c);

    return 0;
}