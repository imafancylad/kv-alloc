#include <stdio.h>
#include <stddef.h>

void *kvalloc(size_t size);
void *kvrealloc(void *ptr, size_t new_size);
void kvfree(void *ptr);

int main(void)
{
    int *numbers = kvalloc(5 * sizeof(int));

    if (numbers == NULL)
    {
        printf("Initial allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        numbers[i] = (i + 1) * 10;
    }

    printf("Before shrinking:\n");

    for (int i = 0; i < 5; i++)
    {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    int *resized = kvrealloc(numbers, 2 * sizeof(int));

    if (resized == NULL)
    {
        printf("Reallocation failed\n");
        kvfree(numbers);
        return 1;
    }

    numbers = resized;
    printf("\nAfter shrinking:\n");

    for (int i = 0; i < 2; i++)
    {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    kvfree(numbers);
    return 0;
}