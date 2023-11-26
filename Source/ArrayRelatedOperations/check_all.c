#include <stdio.h>
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

int main() {
    // Define an array of integers
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    // Check if all elements in the array are positive
    bool result = check_all(arr, size);

    // Print the result
    if(result) {
        printf("All elements in the array are positive.\n");
    } else {
        printf("Not all elements in the array are positive.\n");
    }

    return 0;
}
