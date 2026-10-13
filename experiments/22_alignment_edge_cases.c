#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "../src/kvalloc.c"

int main(void)
{
    size_t alignment = _Alignof(max_align_t);

    // 1. tamaño de 1 byte
    void *a = kvalloc(1);

    printf("Test 1 - size 1: %s\n", a != NULL && (uintptr_t)a % alignment == 0 ? "passed" : "failed");

    // 2. tamaños pequeños consecutivos
    void *b = kvalloc(2);
    void *c = kvalloc(3);

    printf("Test 2 - small sizes: %s\n", b != NULL && c != NULL && (uintptr_t)b % alignment == 0 && (uintptr_t)c % alignment == 0 ? "passed" : "failed");

    // 3. tamaño 0
    void *d = kvalloc(0);

    printf("Test 3 - zero size: %s\n", d != NULL && (uintptr_t)d % alignment == 0 ? "passed" : "failed");

    // 4. desbordamiento al alinear
    void *e = kvalloc(SIZE_MAX);

    printf("Test 4 - overflow: %s\n", e == NULL ? "passed" : "failed");

    kvfree(a);
    kvfree(b);
    kvfree(c);
    kvfree(d);
    kvfree(e);

    return 0;
}