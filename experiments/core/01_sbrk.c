#include <stdio.h>
#include <unistd.h>

int main(void)
{
    void *before = sbrk(0);
    printf("Before: %p\n", before);

    void *result = sbrk(4096);
    printf("sbrk returned: %p\n", result);
    
    void *after = sbrk(0);
    printf("After: %p\n", after);
    
    printf("Difference: %ld bytes\n", (char *)after - (char *)result);

    return 0;
}