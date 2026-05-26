#include "memory_management.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *a = (char *)_malloc(32);
    int *b = (int *)_malloc(10 * sizeof(int));

    assert(a != NULL);
    assert(b != NULL);

    strcpy(a, "allocator test");

    for (int i = 0; i < 10; ++i) {
        b[i] = i * i;
    }

    assert(strcmp(a, "allocator test") == 0);
    assert(b[0] == 0);
    assert(b[3] == 9);
    assert(b[9] == 81);

    _free(a);
    _free(b);

    char *c = (char *)_malloc(16);
    assert(c != NULL);
    strcpy(c, "reuse");
    assert(strcmp(c, "reuse") == 0);
    _free(c);

    assert(_malloc(0) == NULL);

    puts("allocator tests passed");
    return 0;
}
