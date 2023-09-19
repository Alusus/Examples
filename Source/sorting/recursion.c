#include <stdio.h>

int factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

void recursivePrint(int n) {
    if (n > 0) {
        recursivePrint(n - 1);
        printf("%d ", n);
    }
}

int main() {
    int n = 5;

    printf("Factorial of %d: %d\n", n, factorial(n));

    printf("Fibonacci sequence up to %d: ", n);
    for (int i = 0; i <= n; i++) {
        printf("%d ", fibonacci(i));
    }
    printf("\n");

    printf("Recursive Print: ");
    recursivePrint(n);
    printf("\n");

    return 0;
}
