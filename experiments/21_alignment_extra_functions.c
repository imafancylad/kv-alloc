#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "../src/kvalloc.c"

int main(void)
{
    int *array = kvcalloc(5, sizeof(int));

    if (array == NULL)
    {
        printf("Error: kvcalloc failed\n");
        return 1;
    }

    int zeroed = 1;

    for (int i = 0; i < 5; i++)
    {
        if (array[i] != 0)
        {
            zeroed = 0;
        }
    }

    printf("kvcalloc zero initialization: %s\n", zeroed ? "passed" : "failed");
    printf("kvcalloc alignment: %zu\n", (uintptr_t)array % _Alignof(max_align_t));

    for (int i = 0; i < 5; i++)
    {
        array[i] = (i + 1) * 10;
    }

    int *resized = kvrealloc(array, 10 * sizeof(int));

    if (resized == NULL)
    {
        printf("Error: kvrealloc failed\n");
        kvfree(array);
        return 1;
    }

    int preserved = 1;

    for (int i = 0; i < 5; i++)
    {
        if (resized[i] != (i + 1) * 10)
        {
            preserved = 0;
        }
    }

    printf("kvrealloc data preservation: %s\n", preserved ? "passed" : "failed");
    printf("kvrealloc alignment: %zu\n", (uintptr_t)resized % _Alignof(max_align_t));
    kvfree(resized);
    
    return 0;
}