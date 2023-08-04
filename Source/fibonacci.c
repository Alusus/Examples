#include <stdio.h>

unsigned long long fibonacci(unsigned int n) {
    unsigned long long fib[n+1];

    fib[0] = 0;
    fib[1] = 1;

    for (unsigned int i = 2; i <= n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    return fib[n];
}

int main() {
    unsigned int n = 10;
    unsigned long long result = fibonacci(n);

    printf("Fibonacci number at index %u is %llu\n", n, result);

    return 0;
}
