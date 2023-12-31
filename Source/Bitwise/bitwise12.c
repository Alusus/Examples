#include <stdio.h>

int countTrailingZeros(int num) {
    int count = 0;

    if (num == 0) {
        return 32; // Assuming 32-bit integers
    }

    while ((num & 1) == 0) {
        num >>= 1;
        count++;
    }

    return count;
}

int main() {
    int num = 40; // Binary: 101000

    int numTrailingZeros = countTrailingZeros(num);

    printf("Number of trailing zeros in %d: %d\n", num, numTrailingZeros);

    return 0;
}

