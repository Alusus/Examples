#include <stdio.h>

int haveOppositeSigns(int a, int b) {
    return (a ^ b) < 0;
}

int main() {
    int x = -5;
    int y = 10;

    if (haveOppositeSigns(x, y)) {
        printf("x and y have opposite signs.\n");
    } else {
        printf("x and y do not have opposite signs.\n");
    }

    return 0;
}
