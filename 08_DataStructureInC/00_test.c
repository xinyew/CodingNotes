#include <stdio.h>
#include <stddef.h>
#include <limits.h>
int main() {
    printf("%d\t%d\t%zu\n", sizeof(size_t), sizeof(uintptr_t), UINT_MAX);
}