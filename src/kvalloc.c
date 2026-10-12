#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque puede apuntar al siguiente
};

static struct block *head = NULL; // cabeza de la lista enlazada de bloques

static size_t align_size(size_t size)
{
    size_t alignment = _Alignof(max_align_t);

    if (size > SIZE_MAX - (alignment - 1))
    {
        return 0;
    }

    return (size + alignment - 1) & ~(alignment - 1);
}

void *kvalloc(size_t size)
{

    struct block *current = head;

    while (current != NULL)
    {
        if (current->free && current->size >= size)
        {
            if (current->size >= size + sizeof(struct block) + 1)
            {
                struct block *new_block = (struct block *)((char *)(current + 1) + size);
                new_block->size = current->size - size - sizeof(struct block);
                new_block->free = 1;
                new_block->next = current->next;
                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (void *)(current + 1); // devuelve un puntero al espacio de la memoria despues del bloque de metadatos
        }
        current = current->next; // avanza al siguiente bloque
    }

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

    struct block *next = block->next; // fusiona con el siguiente bloque

    if (next != NULL && next->free)
    {
        char *end_of_block = (char *)(block + 1) + block->size;

        if (end_of_block == (char *)next)
        {
            block->size += sizeof(struct block) + next->size;
            block->next = next->next;
        }
    }

    struct block *previous = NULL;
    struct block *current = head;

    while (current != NULL && current != block)
    {
        previous = current;
        current = current->next;
    }

    /*fusionar con el bloque anterior*/
    if (previous != NULL && previous->free)
    {
        char *end_of_previous = (char *)(previous + 1) + previous->size;

        if (end_of_previous == (char *)block)
        {
            previous->size += sizeof(struct block) + block->size;
            previous->next = block->next;
        }
    }
}

void *kvcalloc(size_t count, size_t size)
{
    if (size != 0 && count > (size_t)-1 / size) // funcion para evitar desbordamiento
    {
        return NULL;
    }

    size_t total = count * size; // calcula el tamaño total
    void *ptr = kvalloc(total); // reserva memoria

    if (ptr == NULL)
    {
        return NULL;
    }

    memset(ptr, 0, total); // inicializar bytes a 0
    return ptr;
}

void *kvrealloc(void *ptr, size_t new_size)
{
    if (ptr == NULL)
    {
        return kvalloc(new_size); // reservar un nuevo bloque si no existe el anterior
    }

    if (new_size == 0)
    {
        kvfree(ptr);
        return NULL; // liberar el bloque y devolver NULL cuando el tamaño es 0
    }

    struct block *old_block = (struct block *)ptr - 1; // recuperar metadatos

    if (old_block->size >= new_size)
    {
        return ptr; // si el bloque tiene espacio devolvemos el mismo puntero
    }

    void *new_ptr = kvalloc(new_size); // reservar nuevo bloque

    if (new_ptr == NULL)
    {
        return NULL;
    }

    memcpy(new_ptr, ptr, old_block->size);
    kvfree(ptr);

    return new_ptr;
}