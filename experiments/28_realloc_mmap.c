#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: Allocate a small block\n");

    char *ptr = kvalloc(64);

    if (ptr == NULL)
    {
        fprintf(stderr, "Initial allocation failed\n");
        return 1;
    }

    strcpy(ptr, "Data preserved across realloc");

    printf("Original data: %s\n", ptr);

    printf("\nTest 2: Grow to a large allocation\n");

    char *new_ptr = kvrealloc(ptr, 200 * 1024);

    if (new_ptr == NULL)
    {
        fprintf(stderr, "Reallocation failed\n");
        kvfree(ptr);
        return 1;
    }

    ptr = new_ptr;

    printf("Data after realloc: %s\n", ptr);

    if (strcmp(ptr, "Data preserved across realloc") != 0)
    {
        fprintf(stderr, "Data was corrupted\n");
        kvfree(ptr);
        return 1;
    }

    printf("Data preserved successfully\n");

    printf("\nTest 3: Grow the large allocation again\n");

    new_ptr = kvrealloc(ptr, 300 * 1024);

    if (new_ptr == NULL)
    {
        fprintf(stderr, "Second reallocation failed\n");
        kvfree(ptr);
        return 1;
    }

    ptr = new_ptr;

    printf("Data after second realloc: %s\n", ptr);

    if (strcmp(ptr, "Data preserved across realloc") != 0)
    {
        fprintf(stderr, "Data was corrupted after second realloc\n");
        kvfree(ptr);
        return 1;
    }

    printf("Data preserved after second realloc\n");

    kvfree(ptr);

    printf("\nAll realloc tests passed\n");

    return 0;
}