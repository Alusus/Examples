#include <stdio.h>

// Function to count the number of set bits (1s) in an integer
int countSetBits(unsigned int num) {
    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

// Function to find the maximum of two numbers without using comparison operators
int findMax(int a, int b) {
    int diff = a - b;
    int sign = (diff >> 31) & 0x1;
    return a - diff * sign;
}

// Function to swap two variables without using a temporary variable
void swap(int *a, int *b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

int main() {
    // Count set bits in an integer
    unsigned int num = 142; // Binary: 10001110
    int num_set_bits = countSetBits(num);
    printf("Number of Set Bits in %u: %d\n", num, num_set_bits);

    // Find the maximum of two numbers
    int x = 25;
    int y = 38;
    int max_val = findMax(x, y);
    printf("Maximum of %d and %d: %d\n", x, y, max_val);

    // Swap two variables
    int num1 = 10;
    int num2 = 20;
    printf("Before Swap: num1 = %d, num2 = %d\n", num1, num2);
    swap(&num1, &num2);
    printf("After Swap: num1 = %d, num2 = %d\n", num1, num2);

    return 0;
}
