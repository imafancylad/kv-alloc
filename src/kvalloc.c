#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct block {
    size_t size;
    int free;
    struct block *next; // cada bloque puede apuntar al siguiente
    _Alignas(max_align_t) unsigned char data[];
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

    size_t aligned_size = align_size(size);

    if (aligned_size == 0 && size != 0) // calculo del espacio que se debe reservar
    {
        return NULL;
    }

    struct block *current = head;

    while (current != NULL)
    {
        if (current->free && current->size >= aligned_size)
        {
            size_t remaining = current->size - aligned_size;
            size_t alignment = _Alignof(max_align_t);

            if (remaining >= sizeof(struct block) + alignment)
            {
                struct block *new_block = (struct block *)(current->data + aligned_size);
                new_block->size = remaining - sizeof(struct block);
                new_block->free = 1;
                new_block->next = current->next;

                current->size = aligned_size;
                current->next = new_block;
            }
            
            /*
            if (current->size >= aligned_size + sizeof(struct block) + _Alignof(max_align_t))
            {
                struct block *new_block = (struct block *)((char *)(current + 1) + aligned_size);
                new_block->size = current->size - aligned_size - sizeof(struct block);
                new_block->free = 1;
                new_block->next = current->next;
                current->size = aligned_size;
                current->next = new_block;
            } */

            current->free = 0;
            return current->data; // devuelve un puntero al espacio de la memoria despues del bloque de metadatos
        }

        current = current->next; // avanza al siguiente bloque
    }

    if (aligned_size > SIZE_MAX - sizeof(struct block))
    {
        return NULL;
    }

    size_t total_size = sizeof(struct block) + aligned_size;

    if (total_size > (size_t)INTPTR_MAX)
    {
        return NULL;
    }
    
    struct block *block = sbrk((intptr_t)total_size);

    if (block == (void *)-1)
    {
        return NULL; // sbrk falla
    }

    block->size = aligned_size;
    block->free = 0;
    block->next = NULL;

    if (head == NULL)
    {
        head = block;
    }
    else
    {
        current = head;

        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = block;
    }

    return block->data; // devuelve un puntero al espacio de la memoria despues del bloque de metadatos
}

void kvfree(void *ptr)
{
    if (ptr == NULL)
    {
        return; // no hace nada
    }

    struct block *block = (struct block *)((char *) ptr - offsetof(struct block, data)); // obtener el bloque de metadatos

    block->free = 1; // marca el bloque como libre

    struct block *next = block->next; // fusiona con el siguiente bloque

    if (next != NULL && next->free)
    {
        char *end_of_block = (char *)block->data + block->size;

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
        char *end_of_previous = (char *)previous->data + previous->size;

        if (end_of_previous == (char *)block)
        {
            previous->size += sizeof(struct block) + block->size;
            previous->next = block->next;
        }
    }
}

void *kvcalloc(size_t count, size_t size)
{
    if (size != 0 && count > SIZE_MAX / size) // funcion para evitar desbordamiento
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

    struct block *old_block = (struct block *)((char *)ptr - offsetof(struct block, data)); // recuperar metadatos

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