#include <stdio.h>
#include <stdint.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: SIZE_MAX allocation\n");

    void *ptr = kvalloc(SIZE_MAX);

    if (ptr == NULL)
    {
        printf("SIZE_MAX rejected successfully\n");
    }
    else
    {
        printf("SIZE_MAX allocation unexpectedly succeeded\n");
        kvfree(ptr);
        return 1;
    }

    printf("\nTest 2: Very large allocation\n");

    ptr = kvalloc(SIZE_MAX - 1024);

    if (ptr == NULL)
    {
        printf("Very large allocation rejected successfully\n");
    }
    else
    {
        printf("Very large allocation unexpectedly succeeded\n");
        kvfree(ptr);
        return 1;
    }

    printf("\nAll overflow tests passed\n");

    return 0;
}