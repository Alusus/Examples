#include <stdio.h>

int largestPowerOfTwo(int num) {
    if (num <= 0) {
        return 0; // Handle non-positive numbers
    }
    
    int power = 1;
    while (power * 2 <= num) {
        power *= 2;
    }
    
    return power;
}

int main() {
    int num = 27;

    int result = largestPowerOfTwo(num);

    printf("Largest power of 2 less than or equal to %d: %d\n", num, result);

    return 0;
}
