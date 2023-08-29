#include <stdio.h>

unsigned int reverseBits(unsigned int num) {
    unsigned int reversed = 0;
    int bits = sizeof(num) * 8; // Total number of bits in an integer

    for (int i = 0; i < bits; i++) {
        if (num & (1 << i)) {
            reversed |= 1 << ((bits - 1) - i);
        }
    }

    return reversed;
}

int main() {
    unsigned int num = 123; // Binary: 1111011

    unsigned int reversedNum = reverseBits(num);

    printf("Original number: %u\n", num);
    printf("Reversed number: %u\n", reversedNum);

    return 0;
}
