#include <stdio.h>
#include <stdbool.h>

// Criteria function that checks if a number is positive
bool criteria(int num) {
    return num > 0;
}

// Function to check all elements in an array
bool check_any(int arr[], int size) {
    for(int i = 0; i < size; i++) {
        if(criteria(arr[i])) {
            return true;
        }
    }
    return false;
}

int main() {
    // Define an array of integers
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    // Check if all elements in the array are positive
    bool result = check_any(arr, size);

    // Print the result
    if(result) {
        printf("There is at least an element in the array that is positive.\n");
    } else {
        printf("There is no element in the array that is positive.\n");
    }

    return 0;
}
