#include <stdio.h>

unsigned long long factorial(unsigned int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    unsigned int n = 5;
    unsigned long long result = factorial(n);

    printf("Factorial of %u is %llu\n", n, result);

    return 0;
}
