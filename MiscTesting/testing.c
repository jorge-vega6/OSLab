#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHARS 10000

int main() {

    int arr[5];
    arr[0] = 1;
    int size = sizeof(arr) / sizeof(arr[0]);
    printf("Size of arr: %d", size);

    return 0;
}
