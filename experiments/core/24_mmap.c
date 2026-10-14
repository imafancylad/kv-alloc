#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    size_t size = 4096;

    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    printf("Memory address: %p\n", ptr);
    printf("Memory allocated: %zu bytes\n", size);

    char *data = ptr;
    data[0] = 'K';
    data[1] = 'V';
    data[2] = 'A';
    data[3] = 'L';

    printf("Data written: %c%c%c%c\n", data[0], data[1], data[2], data[3]);

    if (munmap(ptr, size) == -1)
    {
        perror("munmap");
        return 1;
    }

    printf("Memory unmapped successfully\n");

    return 0;
}