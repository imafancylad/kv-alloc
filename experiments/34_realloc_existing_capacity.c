#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test: kvrealloc with a size that fits the existing block\n");

    char *ptr = kvalloc(33);

    if (ptr == NULL)
    {
        fprintf(stderr, "FAIL: initial allocation failed\n");
        return 1;
    }

    strcpy(ptr, "Allocator test");

    printf("Original pointer: %p\n", (void *)ptr);

    void *new_ptr = kvrealloc(ptr, 40);

    if (new_ptr == NULL)
    {
        fprintf(stderr, "FAIL: realloc returned NULL\n");
        kvfree(ptr);
        return 1;
    }

    printf("New pointer:      %p\n", new_ptr);

    if (new_ptr != ptr)
    {
        fprintf(stderr, "FAIL: expected the same pointer\n");
        kvfree(new_ptr);
        return 1;
    }

    if (strcmp((char *)new_ptr, "Allocator test") != 0)
    {
        fprintf(stderr, "FAIL: data was corrupted\n");
        kvfree(new_ptr);
        return 1;
    }

    printf("PASS: pointer unchanged\n");
    printf("PASS: data preserved\n");

    kvfree(new_ptr);

    printf("All tests passed.\n");

    return 0;
}