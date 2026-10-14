#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test: kvrealloc grows the allocation and preserves data\n");

    char *ptr = kvalloc(32);

    if (ptr == NULL)
    {
        fprintf(stderr, "FAIL: initial allocation failed\n");
        return 1;
    }

    strcpy(ptr, "Original data");

    printf("Original pointer: %p\n", (void *)ptr);
    printf("Original data: %s\n", ptr);

    char *new_ptr = kvrealloc(ptr, 256);

    if (new_ptr == NULL)
    {
        fprintf(stderr, "FAIL: realloc returned NULL\n");
        kvfree(ptr);
        return 1;
    }

    printf("New pointer: %p\n", (void *)new_ptr);
    printf("Preserved data: %s\n", new_ptr);

    if (strcmp(new_ptr, "Original data") != 0)
    {
        fprintf(stderr, "FAIL: data was not preserved\n");
        kvfree(new_ptr);
        return 1;
    }

    printf("PASS: data preserved\n");

    if (new_ptr != ptr)
    {
        printf("PASS: allocation moved to a new block\n");
    }
    else
    {
        printf("Same pointer returned; investigating allocator behavior.\n");
    }

    kvfree(new_ptr);

    printf("All tests passed.\n");

    return 0;
}