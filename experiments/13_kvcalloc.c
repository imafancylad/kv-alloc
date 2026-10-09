#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

void *kvcalloc(size_t count, size_t size);
void kvfree(void *ptr);

int main(void)
{
    int *numbers = kvcalloc(5, sizeof(int));

    if (numbers == NULL)
    {
        printf("Error allocating memory\n");
        return 1;
    }

    printf("Values after kvcalloc:\n");

    for (int i = 0; i < 5; i++)
    {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    void *overflow = kvcalloc(SIZE_MAX, 2);
    printf("Overflow: %s\n", overflow == NULL ? "correctly rejected" : "unexpected allocation");
    kvfree(overflow);

    void *empty = kvcalloc(0, sizeof(int));
    printf("Zero elements: %s\n", empty == NULL ? "NULL" : "non-NULL");
    kvfree(empty);

    kvfree(numbers);
    return 0;
}