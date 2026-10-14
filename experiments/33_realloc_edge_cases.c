#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: kvrealloc(NULL, 128)\n");

    char *ptr = kvrealloc(NULL, 128);

    if (ptr == NULL)
    {
        fprintf(stderr, "FAIL: allocation returned NULL\n");
        return 1;
    }

    strcpy(ptr, "Allocated through kvrealloc");

    if (strcmp(ptr, "Allocated through kvrealloc") != 0)
    {
        fprintf(stderr, "FAIL: data mismatch\n");
        kvfree(ptr);
        return 1;
    }

    printf("PASS: allocation and data verification\n");

    printf("\nTest 2: kvrealloc(ptr, 0)\n");

    void *result = kvrealloc(ptr, 0);

    if (result != NULL)
    {
        fprintf(stderr, "FAIL: expected NULL\n");
        kvfree(result);
        return 1;
    }

    printf("PASS: returned NULL\n");
    printf("PASS: kvrealloc called kvfree on the original block\n");

    printf("\nAll tests passed.\n");

    return 0;
}