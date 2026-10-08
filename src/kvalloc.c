#include <unistd.h>
#include <stddef.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque puede apuntar al siguiente
};

void *kvalloc(size_t size)
{
    struct block *block = sbrk(sizeof(struct block) + size);

    if (block == (void *)-1)
    {
        return NULL; // sbrk falla
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;

    return (void *)(block + 1); // devuelve un puntero al espacio de memoria despues del bloque de metadatos
}