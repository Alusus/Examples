#include <stdio.h>

// Function to extract the nth bit from a number
int getBit(int num, int n) {
    return (num >> n) & 1;
}

// Function to set the nth bit in a number to 1
int setBit(int num, int n) {
    return num | (1 << n);
}

// Function to clear the nth bit in a number (set it to 0)
int clearBit(int num, int n) {
    return num & ~(1 << n);
}

// Function to toggle the nth bit in a number
int toggleBit(int num, int n) {
    return num ^ (1 << n);
}

int main() {
    int num = 42; // Binary: 101010

    printf("Original number: %d\n", num);

    int bitPosition = 3;
    printf("Bit at position %d: %d\n", bitPosition, getBit(num, bitPosition));

    num = setBit(num, 2); // Set 3rd bit to 1
    printf("Number after setting 3rd bit: %d\n", num);

    num = clearBit(num, 3); // Clear 4th bit (counting from 0)
    printf("Number after clearing 4th bit: %d\n", num);

    num = toggleBit(num, 2); // Toggle 3rd bit
    printf("Number after toggling 3rd bit: %d\n", num);

    return 0;
}

