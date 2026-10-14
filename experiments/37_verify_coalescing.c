#include <stdio.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test: verify block coalescing through metadata\n\n");

    char *a = kvalloc(32);
    char *b = kvalloc(32);
    char *c = kvalloc(32);

    if (a == NULL || b == NULL || c == NULL)
    {
        fprintf(stderr, "FAIL: initial allocation failed\n");
        
        kvfree(a);
        kvfree(b);
        kvfree(c);
        
        return 1;
    }

    struct block *block_a = (struct block *)((char *)a - offsetof(struct block, data));
    struct block *block_b = (struct block *)((char *)b - offsetof(struct block, data));
    struct block *block_c = (struct block *)((char *)c - offsetof(struct block, data));

    /* Save B's original size before freeing it. */
    size_t original_b_size = block_b->size;

    printf("Before freeing:\n");
    printf("A: address=%p size=%zu free=%d next=%p\n", (void *)block_a, block_a->size, block_a->free, (void *)block_a->next);
    printf("B: address=%p size=%zu free=%d next=%p\n", (void *)block_b, block_b->size, block_b->free, (void *)block_b->next);
    printf("C: address=%p size=%zu free=%d next=%p\n", (void *)block_c, block_c->size, block_c->free, (void *)block_c->next);

    kvfree(b);
    kvfree(a);

    printf("\nAfter freeing B and A:\n");
    printf("A: address=%p size=%zu free=%d next=%p\n", (void *)block_a, block_a->size, block_a->free, (void *)block_a->next);
    printf("C: address=%p size=%zu free=%d next=%p\n", (void *)block_c, block_c->size, block_c->free, (void *)block_c->next);

    if (block_a->free != 1)
    {
        fprintf(stderr, "FAIL: A is not marked free\n");
        kvfree(c);
        return 1;
    }

    if (block_a->next != block_c)
    {
        fprintf(stderr, "FAIL: A does not point directly to C\n");
        kvfree(c);
        return 1;
    }

    if (block_a->size <= original_b_size)
    {
        fprintf(stderr, "FAIL: A's size did not increase\n");
        kvfree(c);
        return 1;
    }

    printf("\nPASS: A is free\n");
    printf("PASS: A points directly to C\n");
    printf("PASS: A's size increased after coalescing\n");

    kvfree(c);

    printf("\nAll checks passed.\n");

    return 0;
}