#include <stdio.h>
#include <stddef.h>

#include "../src/kvalloc.c"

int main(void)
{
    void *a = kvalloc(32);
    void *b = kvalloc(32);
    void *c = kvalloc(32);

    if (a == NULL || b == NULL || c == NULL)
    {
        printf("Allocation failed\n");
        return 1;
    }

    struct block *block_a = (struct block *)((char *)a - offsetof(struct block, data));
    struct block *block_b = (struct block *)((char *)b - offsetof(struct block, data));
    struct block *block_c = (struct block *)((char *)c - offsetof(struct block, data));

    printf("Initial list:\n");
    printf("a size=%zu, b size=%zu, c size=%zu\n", block_a->size, block_b->size, block_c->size);

    kvfree(b);

    printf("\nAfter freeing b:\n");
    printf("a next is b: %s\n", block_a->next == block_b ? "yes" : "no");

    kvfree(a);

    printf("\nAfter freeing a:\n");
    printf("a size=%zu\n", block_a->size);
    printf("a next is c: %s\n", block_a->next == block_c ? "yes" : "no");

    kvfree(c);

    printf("\nAfter freeing c:\n");
    printf("a size=%zu\n", block_a->size);
    printf("a next is NULL: %s\n", block_a->next == NULL ? "yes" : "no");

    return 0;
}