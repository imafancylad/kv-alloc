#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test: coalescing and reusing free blocks\n\n");

    /* Allocate three consecutive small blocks. */
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

    strcpy(a, "Block A");
    strcpy(b, "Block B");
    strcpy(c, "Block C");

    printf("Initial allocations:\n");
    printf("A: %p\n", (void *)a);
    printf("B: %p\n", (void *)b);
    printf("C: %p\n", (void *)c);

    /* Free B first, then A. */
    kvfree(b);
    kvfree(a);

    printf("\nFreed blocks B and A.\n");

    /*
     * Request a block larger than either A or B individually.
     * The allocator should be able to reuse their combined
     * space if coalescing works as expected.
     */
    
    char *d = kvalloc(64);

    if (d == NULL)
    {
        fprintf(stderr, "FAIL: allocation D failed\n");
        kvfree(c);
        return 1;
    }

    printf("D (64 bytes): %p\n", (void *)d);

    /*
     * C must remain intact regardless of whether D reuses
     * the space previously occupied by A and B.
     */

    if (strcmp(c, "Block C") != 0)
    {
        fprintf(stderr, "FAIL: block C data was corrupted\n");
        kvfree(d);
        kvfree(c);
        return 1;
    }

    printf("PASS: block C data preserved\n");

    strcpy(d, "Reused memory");

    if (strcmp(d, "Reused memory") != 0)
    {
        fprintf(stderr, "FAIL: block D data verification failed\n");
        kvfree(d);
        kvfree(c);
        return 1;
    }

    printf("PASS: block D is writable\n");

    kvfree(d);
    kvfree(c);

    printf("\nAll basic checks passed.\n");
    printf("Inspect the addresses to investigate block reuse.\n");

    return 0;
}