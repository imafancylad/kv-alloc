#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "../src/kvalloc.c"

int main(void)
{
    void *a = kvalloc(1);
    void *b = kvalloc(20);
    void *c = kvalloc(30);

    printf("a: %p | rest: %zu\n", a, (uintptr_t)a % _Alignof(max_align_t));
    printf("b: %p | rest: %zu\n", b, (uintptr_t)b % _Alignof(max_align_t));
    printf("c: %p | rest: %zu\n", c, (uintptr_t)c % _Alignof(max_align_t));

    return 0;
}