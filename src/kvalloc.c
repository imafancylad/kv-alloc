#include <unistd.h>
#include <stddef.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque puede apuntar al siguiente
};

static struct block *head = NULL; // cabeza de la lista enlazada de bloques

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

    if (head == NULL)
    {
        head = block;
    }
    else
    {
        struct block *current = head;

        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = block;
    }

    return (void *)(block + 1); // devuelve un puntero al espacio de la memoria despues del bloque de metadatos
}

void kvfree(void *ptr)
{
    if (ptr == NULL)
    {
        return; // no hace nada
    }

    struct block *block = (struct block *)ptr - 1; // obtener el bloque de metadatos

    block->free = 1; // marca el bloque como libre
}