#include <stdio.h>

#define ROTL(x, n) ((x << n) | (x >> (32 - n)))

void printbit(unsigned x) {
    for (int i = 0; i < 32; i++) {
        printf(((x >> (31 - i)) & 1) ? "1" : "0");
    }
    printf("\n");
}

int main() {
    // Write C code here
    for (int i = 0; i < 32; i++) {
    printbit(ROTL(0x80000001, i));
    }

    return 0;
}
