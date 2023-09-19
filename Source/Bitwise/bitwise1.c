#include <stdio.h>

void bitwiseOperations(int a, int b) {
    int bitwiseAnd = a & b;
    int bitwiseOr = a | b;
    int bitwiseXor = a ^ b;
    int bitwiseNotA = ~a;
    int leftShift = a << 2;
    int rightShift = a >> 2;

    printf("Bitwise AND: %d\n", bitwiseAnd);
    printf("Bitwise OR: %d\n", bitwiseOr);
    printf("Bitwise XOR: %d\n", bitwiseXor);
    printf("Bitwise NOT (of a): %d\n", bitwiseNotA);
    printf("Left Shift: %d\n", leftShift);
    printf("Right Shift: %d\n", rightShift);
}

int main() {
    int a = 5;  // binary: 00000101
    int b = 3;  // binary: 00000011

    printf("a: %d\n", a);
    printf("b: %d\n", b);

    bitwiseOperations(a, b);

    return 0;
}
