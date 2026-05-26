#include "memory_management.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    char *message = (char *)_malloc(64);

    if (message == NULL) {
        fprintf(stderr, "allocation failed\n");
        return 1;
    }

    strcpy(message, "Hello from the custom allocator");
    printf("%s\n", message);

    _free(message);
    return 0;
}
