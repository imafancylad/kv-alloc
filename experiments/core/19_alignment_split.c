#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "../src/kvalloc.c"

int main(void)
{
    void *a = kvalloc(256);

    if (a == NULL)
    {
        printf("Error: allocation a failed\n");
        return 1;
    }

    kvfree(a);

    void *b = kvalloc(32);
    void *c = kvalloc(32);

    if (b == NULL || c == NULL)
    {
        printf("Error: allocation failed\n");
        return 1;
    }

    printf("Alignment required: %zu\n", _Alignof(max_align_t));
    printf("b: %p | rest: %zu\n", b, (uintptr_t)b % _Alignof(max_align_t));
    printf("c: %p | rest: %zu\n", c, (uintptr_t)c % _Alignof(max_align_t));

    printf("b reused a: %s\n", b == a ? "yes" : "no");
    printf("c differs from b: %s\n", c != b ? "yes" : "no");

    kvfree(b);
    kvfree(c);

    return 0;
}