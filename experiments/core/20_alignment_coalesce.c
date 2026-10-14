#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "../src/kvalloc.c"

int main(void)
{
    void *a = kvalloc(32);
    void *b = kvalloc(32);
    void *c = kvalloc(32);

    if (a == NULL || b == NULL || c == NULL)
    {
        printf("Error: allocation failed\n");
        return 1;
    }

    struct block *block_a = (struct block *)((char *)a - offsetof(struct block, data));

    printf("Initial sizes: a=%zu, b=%zu, c=%zu\n", block_a->size, ((struct block *)((char *)b - offsetof(struct block, data)))->size, ((struct block *)((char *)c - offsetof(struct block, data)))->size);

    kvfree(a);
    kvfree(b);

    printf("After freeing a and b:\n");
    printf("a size: %zu\n", block_a->size);
    printf("a next is c: %s\n", block_a->next != NULL && block_a->next->data == c ? "yes" : "no");

    printf("a alignment: %zu\n", (uintptr_t)a % _Alignof(max_align_t));

    kvfree(c);

    printf("After freeing c:\n");
    printf("a size: %zu\n", block_a->size);
    printf("a next is NULL: %s\n", block_a->next == NULL ? "yes" : "no");

    return 0;
}