#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: Small allocation\n");

    char *small = kvalloc(64);

    if (small == NULL)
    {
        fprintf(stderr, "Small allocation failed\n");
        return 1;
    }

    strcpy(small, "Small allocation works");

    printf("Small data: %s\n", small);

    printf("\nTest 2: Large allocation\n");

    char *large = kvalloc(200 * 1024);

    if (large == NULL)
    {
        fprintf(stderr, "Large allocation failed\n");
        kvfree(small);
        return 1;
    }

    strcpy(large, "Large allocation works");

    printf("Large data: %s\n", large);

    printf("\nTest 3: Free large allocation\n");

    kvfree(large);

    printf("Large allocation freed\n");

    printf("\nTest 4: Reuse small allocation\n");

    printf("Small data after freeing large: %s\n", small);

    if (strcmp(small, "Small allocation works") != 0)
    {
        fprintf(stderr, "Small allocation was corrupted\n");
        kvfree(small);
        return 1;
    }

    printf("Small allocation remains valid\n");

    kvfree(small);

    printf("Small allocation freed\n");

    return 0;
}