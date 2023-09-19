#include <stdio.h>

int setRightmostUnsetBit(int num) {
    return num | (num + 1);
}

int main() {
    int num = 43; // Binary: 101011

    printf("Original number: %d\n", num);

    int numWithSetBit = setRightmostUnsetBit(num);

    printf("Number with the rightmost unset bit set: %d\n", numWithSetBit);

    return 0;
}
