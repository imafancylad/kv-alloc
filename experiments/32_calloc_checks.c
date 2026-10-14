#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    printf("Test 1: Zero-initialized allocation\n");

    size_t count = 16;
    size_t element_size = sizeof(int);

    int *numbers = kvcalloc(count, element_size);

    if (numbers == NULL)
    {
        fprintf(stderr, "kvcalloc failed\n");
        return 1;
    }

    for (size_t i = 0; i < count; i++)
    {
        if (numbers[i] != 0)
        {
            fprintf(stderr, "Element %zu was not zero\n", i);
            kvfree(numbers);
            return 1;
        }
    }

    printf("All elements initialized to zero\n");

    numbers[0] = 42;

    printf("First element after writing: %d\n", numbers[0]);

    kvfree(numbers);

    printf("\nTest 2: Multiplication overflow\n");

    void *ptr = kvcalloc(SIZE_MAX, 2);

    if (ptr != NULL)
    {
        fprintf(stderr, "Overflow was not rejected\n");
        kvfree(ptr);
        return 1;
    }

    printf("Multiplication overflow rejected successfully\n");

    printf("\nAll calloc tests passed\n");

    return 0;
}