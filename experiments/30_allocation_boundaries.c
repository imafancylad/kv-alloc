#include <stdio.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: Zero-size allocation\n");

    void *zero = kvalloc(0);

    printf("kvalloc(0) returned: %p\n", zero);

    if (zero != NULL)
    {
        kvfree(zero);
    }

    printf("\nTest 2: One-byte allocation\n");

    char *small = kvalloc(1);

    if (small == NULL)
    {
        fprintf(stderr, "One-byte allocation failed\n");
        return 1;
    }

    small[0] = 'K';

    printf("Stored character: %c\n", small[0]);

    kvfree(small);

    printf("\nTest 3: Allocation above mmap threshold\n");

    size_t size = 128 * 1024 + 1;

    char *large = kvalloc(size);

    if (large == NULL)
    {
        fprintf(stderr, "Large allocation failed\n");
        return 1;
    }

    large[0] = 'A';
    large[size - 1] = 'Z';

    printf("First byte: %c\n", large[0]);
    printf("Last byte: %c\n", large[size - 1]);

    kvfree(large);

    printf("\nAll boundary tests passed\n");

    return 0;
}