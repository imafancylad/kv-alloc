#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: Allocate a large block\n");

    char *ptr = kvalloc(200 * 1024);

    if (ptr == NULL)
    {
        fprintf(stderr, "Initial allocation failed\n");
        return 1;
    }

    strcpy(ptr, "Data survives shrinking");

    printf("Original data: %s\n", ptr);

    printf("\nTest 2: Shrink the allocation\n");

    char *new_ptr = kvrealloc(ptr, 1024);

    if (new_ptr == NULL)
    {
        fprintf(stderr, "Shrinking allocation failed\n");
        kvfree(ptr);
        return 1;
    }

    printf("Original pointer: %p\n", (void *)ptr);
    printf("New pointer:      %p\n", (void *)new_ptr);

    ptr = new_ptr;

    if (strcmp(ptr, "Data survives shrinking") != 0)
    {
        fprintf(stderr, "Data was corrupted after shrinking\n");
        kvfree(ptr);
        return 1;
    }

    printf("Data preserved after shrinking\n");

    kvfree(ptr);

    printf("\nShrinking test passed\n");

    return 0;
}