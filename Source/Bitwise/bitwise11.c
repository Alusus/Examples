#include <stdio.h>

#define HASH_SIZE 256 // Size of the hash table

unsigned int hashFunction(char *str) {
    unsigned int hash = 0;

    while (*str) {
        hash = (hash << 5) ^ *str++; // Left-shift and XOR
    }

    return hash % HASH_SIZE; // Modulo to fit within hash table size
}

int main() {
    char input[] = "hello";

    unsigned int hashValue = hashFunction(input);

    printf("Hash value for \"%s\": %u\n", input, hashValue);

    return 0;
}
