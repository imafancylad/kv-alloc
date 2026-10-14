#include <stddef.h>
#include <stdio.h>

struct block {
    size_t size;
    int free;
};

void *kvalloc(size_t size);

int main(void)
{
    int *p = kvalloc(sizeof(int));

    if (p == NULL)
    {
        perror("kvalloc failed");
        return 1;
    }

    *p = 42;

    struct block *block = (struct block *)p - 1; // obtener el bloque de metadatos asociado al puntero p

    printf("Data: %p\n", (void *)p);
    printf("Metadata: %p\n", (void *)block);
    printf("Size: %zu\n", block->size);
    printf("Free: %d\n", block->free);
    printf("Value: %d\n", *p);
    return 0;
}