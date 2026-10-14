#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test: reuse a free block without coalescing\n\n");

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

    kvfree(b);

    printf("\nFreed B.\n");

    char *d = kvalloc(32);

    if (d == NULL)
    {
        fprintf(stderr, "FAIL: allocation D failed\n");
        
        kvfree(a);
        kvfree(c);
        
        return 1;
    }

    printf("D: %p\n", (void *)d);

    if (d != b)
    {
        fprintf(stderr, "FAIL: expected to reuse B\n");
        
        kvfree(a);
        kvfree(c);
        kvfree(d);
        
        return 1;
    }

    printf("PASS: D reused B's address\n");

    if (strcmp(a, "Block A") != 0 ||
        strcmp(c, "Block C") != 0)
    {
        fprintf(stderr, "FAIL: existing data was corrupted\n");
        
        kvfree(a);
        kvfree(d);
        kvfree(c);
        
        return 1;
    }

    printf("PASS: A and C data preserved\n");

    strcpy(d, "Block D");

    if (strcmp(d, "Block D") != 0)
    {
        fprintf(stderr, "FAIL: D data verification failed\n");
        
        kvfree(a);
        kvfree(d);
        kvfree(c);
        
        return 1;
    }

    printf("PASS: D is writable\n");

    kvfree(a);
    kvfree(d);
    kvfree(c);

    printf("\nAll checks passed.\n");

    return 0;
}