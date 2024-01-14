#include <stdbool.h>

// Criteria function that checks if a number is positive
bool criteria(int num) {
    return num > 0;
}

// Function to check all elements in an array
bool check_all(int arr[], int size) {
    for(int i = 0; i < size; i++) {
        if(!criteria(arr[i])) {
            return false;
        }
    }
    return true;
}
