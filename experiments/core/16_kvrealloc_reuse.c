#include <stdio.h>
#include <stddef.h>

void *kvalloc(size_t size);
void *kvrealloc(void *ptr, size_t new_size);
void kvfree(void *ptr);

int main(void)
{
    int *numbers = kvalloc(10 * sizeof(int));

    if (numbers == NULL)
    {
        printf("Allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        numbers[i] = (i + 1) * 10;
    }

    printf("Original pointer: %p\n", (void *)numbers);
    int *resized = kvrealloc(numbers, 5 * sizeof(int));

    if (resized == NULL)
    {
        printf("Reallocation failed\n");
        kvfree(numbers);
        return 1;
    }

    printf("Resized pointer: %p\n", (void *)resized);

    if (resized == numbers)
    {
        printf("Block reused!\n");
    } else {
        printf("A different block was returned\n");
    }

    for (int i = 0; i < 5; i++)
    {
        printf("numbers[%d] = %d\n", i, resized[i]);
    }

    kvfree(resized);
    return 0;
}