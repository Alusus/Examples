#include <stdio.h>

int swapBits(int num, int i, int j) {
    int bit_i = (num >> i) & 1;
    int bit_j = (num >> j) & 1;

    if (bit_i != bit_j) {
        num = num ^ (1 << i); // Toggle bit i
        num = num ^ (1 << j); // Toggle bit j
    }

    return num;
}

int main() {
    int num = 42; // Binary: 101010

    printf("Original number: %d\n", num);

    int swappedNum = swapBits(num, 1, 3);

    printf("Number after swapping bits 1 and 3: %d\n", swappedNum);

    return 0;
}
