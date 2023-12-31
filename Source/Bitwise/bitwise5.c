#include <stdio.h>

// Function to check if a number is a power of two
int isPowerOfTwo(unsigned int num) {
    return (num != 0) && ((num & (num - 1)) == 0);
}

// Function to find the next power of two greater than a given number
unsigned int nextPowerOfTwo(unsigned int num) {
    num--;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    return num + 1;
}

int main() {
    unsigned int n1 = 16;
    unsigned int n2 = 25;

    // Check if n1 is a power of two
    printf("%u is %s a power of two.\n", n1, isPowerOfTwo(n1) ? "" : "not");

    // Check if n2 is a power of two
    printf("%u is %s a power of two.\n", n2, isPowerOfTwo(n2) ? "" : "not");

    // Find the next power of two for n1
    unsigned int next_pow_2_n1 = nextPowerOfTwo(n1);
    printf("Next power of two after %u is %u.\n", n1, next_pow_2_n1);

    // Find the next power of two for n2
    unsigned int next_pow_2_n2 = nextPowerOfTwo(n2);
    printf("Next power of two after %u is %u.\n", n2, next_pow_2_n2);

    return 0;
}

