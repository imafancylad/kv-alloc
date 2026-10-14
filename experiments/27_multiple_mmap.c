#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

#define ALLOCATION_COUNT 5
#define ALLOCATION_SIZE (200 * 1024)

int main(void)
{
    char *blocks[ALLOCATION_COUNT];

    printf("Allocating %d large blocks...\n", ALLOCATION_COUNT);

    for (int i = 0; i < ALLOCATION_COUNT; i++)
    {
        blocks[i] = kvalloc(ALLOCATION_SIZE);

        if (blocks[i] == NULL)
        {
            fprintf(stderr, "Allocation %d failed\n", i);

            for (int j = 0; j < i; j++)
            {
                kvfree(blocks[j]);
            }

            return 1;
        }

        snprintf(blocks[i], ALLOCATION_SIZE, "Data from block %d", i);

        printf("Block %d allocated at %p\n", i, (void *)blocks[i]);
    }

    printf("\nVerifying block contents...\n");

    for (int i = 0; i < ALLOCATION_COUNT; i++)
    {
        char expected[64];

        snprintf(expected, sizeof(expected), "Data from block %d", i);

        if (strcmp(blocks[i], expected) != 0)
        {
            fprintf(stderr, "Block %d verification failed\n", i);

            for (int j = 0; j < ALLOCATION_COUNT; j++)
            {
                kvfree(blocks[j]);
            }

            return 1;
        }

        printf("Block %d verified\n", i);
    }

    printf("\nFreeing all blocks...\n");

    for (int i = 0; i < ALLOCATION_COUNT; i++)
    {
        kvfree(blocks[i]);
        printf("Block %d freed\n", i);
    }

    printf("\nAll tests passed\n");

    return 0;
}