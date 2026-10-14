#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

struct block {
    size_t size;
    int free;
    struct block *next;
};

int main(void)
{
    struct block *a = sbrk(sizeof(struct block) + 10);
    struct block *b = sbrk(sizeof(struct block) + 20);
    struct block *c = sbrk(sizeof(struct block) + 30);

    if (a == (void *)-1 || b == (void *)-1 || c == (void *)-1)
    {
        perror("sbrk failed");
        return 1;
    }

    a->size = 10;
    a->free = 0;
    a->next = b;

    b->size = 20;
    b->free = 0;
    b->next = c;

    c->size = 30;
    c->free = 0;
    c->next = NULL;

    struct block *current = a;

    while (current != NULL)
    {
        printf("Block: %p | Size: %zu | Free: %d | Next: %p\n",
               (void *)current, current->size, current->free, (void *)current->next);
        current = current->next;
    }
    return 0;
}