#include <stdio.h>

void greedyAlgorithm(int coins[], int size, int amount) {
    int remainingAmount = amount;

    for (int i = 0; i < size; i++) {
        int coinCount = remainingAmount / coins[i];
        remainingAmount = remainingAmount % coins[i];

        if (coinCount > 0) {
            printf("Number of %d coin(s): %d\n", coins[i], coinCount);
        }
    }

    if (remainingAmount > 0) {
        printf("Remaining amount unable to be changed completely: %d\n", remainingAmount);
    }
}

int main() {
    int coins[] = {10, 5, 2, 1};
    int size = sizeof(coins) / sizeof(coins[0]);
    int amount = 28;

    greedyAlgorithm(coins, size, amount);

    return 0;
}
