#include <stdio.h>
#include <string.h>
#include "../src/kvalloc.c"

int main(void)
{
    size_t size = 200 * 1024;

    printf("Requesting %zu bytes ...\n", size);
    char *ptr = kvalloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "kvalloc failed\n");
        return 1;
    }

    printf("Memory allocated at: %p\n", (void *)ptr);
    strcpy(ptr, "Hello from kvalloc with mmap!");
    printf("Data written: %s\n", ptr);

    if (strcmp(ptr, "Hello from kvalloc with mmap!") != 0)
    {
        fprintf(stderr, "Data verification failed\n");
        kvfree(ptr);
        return 1;
    }

    printf("Data verification successful\n");
    kvfree(ptr);
    printf("Memory freed successfully\n");
    
    return 0;
}