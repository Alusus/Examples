#include <stdio.h>

int main() {
    unsigned int a = 10; // Binary: 0000 1010
    unsigned int b = 5;  // Binary: 0000 0101

    // Bitwise AND
    unsigned int result_and = a & b; // Binary: 0000 0000 (Decimal: 0)
    printf("AND Result: %u\n", result_and);

    // Bitwise OR
    unsigned int result_or = a | b;  // Binary: 0000 1111 (Decimal: 15)
    printf("OR Result: %u\n", result_or);

    // Bitwise XOR
    unsigned int result_xor = a ^ b; // Binary: 0000 1111 (Decimal: 15)
    printf("XOR Result: %u\n", result_xor);

    // Bitwise NOT
    unsigned int result_not_a = ~a;  // Binary: 1111 0101 (Decimal: 245)
    printf("NOT Result (a): %u\n", result_not_a);

    // Left Shift
    unsigned int left_shifted = a << 2; // Binary: 0010 1000 (Decimal: 40)
    printf("Left Shift Result: %u\n", left_shifted);

    // Right Shift
    unsigned int right_shifted = b >> 1; // Binary: 0000 0010 (Decimal: 2)
    printf("Right Shift Result: %u\n", right_shifted);

    return 0;
}
