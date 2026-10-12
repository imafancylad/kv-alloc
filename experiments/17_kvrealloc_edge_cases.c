#include <stdio.h>
#include <stddef.h>

void *kvalloc(size_t size);
void *kvrealloc(void *ptr, size_t new_size);
void kvfree(void *ptr);

int main(void)
{
    // 1. realloc con NULL
    int *a = kvrealloc(NULL, 5 * sizeof(int));

    if (a == NULL)
    {
        printf("Test 1 failed: allocation returned NULL\n");
        return 1;
    }

    printf("Test 1 passed: NULL pointer handled\n");

    // 2. conservar los datos
    a[0] = 42;

    int *b = kvrealloc(a, 10 * sizeof(int));

    if (b == NULL)
    {
        printf("Test 2 failed: reallocation returned NULL\n");
        kvfree(a);
        return 1;
    }

    if (b[0] != 42)
    {
        printf("Test 2 failed: data was not preserved\n");
        kvfree(b);
        return 1;
    }

    printf("Test 2 passed: data preserved\n");

    // 3. tamaño 0
    void *c = kvrealloc(b, 0);

    if (c != NULL)
    {
        printf("Test 3 failed: expected NULL\n");
        kvfree(c);
        return 1;
    }

    printf("Test 3 passed: zero size handled\n");
    return 0;
}