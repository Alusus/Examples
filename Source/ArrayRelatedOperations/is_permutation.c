#include <stdbool.h>

// Function to check if arr2 is a permutation of arr1
bool is_permutation(int arr1[], int size1, int arr2[], int size2) {
    // If sizes are not equal, return false
    if(size1 != size2) {
        return false;
    }

    // Create an array to keep track of visited elements
    bool visited[size2];
    for(int i = 0; i < size2; i++) {
        visited[i] = false;
    }

    // For each element in arr1
    for(int i = 0; i < size1; i++) {
        bool found = false;

        // For each element in arr2
        for(int j = 0; j < size2; j++) {
            // If the element in arr1 is found in arr2 and it has not been visited
            if(arr1[i] == arr2[j] && !visited[j]) {
                found = true;
                visited[j] = true;  // Mark the element in arr2 as visited
                break;
            }
        }

        // If the element in arr1 is not found in arr2, return false
        if(!found) {
            return false;
        }
    }

    // If we reach here, then arr2 is a permutation of arr1
    return true;
}
