// Function to check if arr2 is a permutation of arr1
function is_permutation(arr1, arr2) {
    // If sizes are not equal, return false
    if(arr1.length !== arr2.length) {
        return false;
    }

    // Create an array to keep track of visited elements
    let visited = new Array(arr2.length).fill(false);

    // For each element in arr1
    for(let i = 0; i < arr1.length; i++) {
        let found = false;

        // For each element in arr2
        for(let j = 0; j < arr2.length; j++) {
            // If the element in arr1 is found in arr2 and it has not been visited
            if(arr1[i] === arr2[j] && !visited[j]) {
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

// Example usage:
let arr1 = [1, 2, 3, 4, 5];
let arr2 = [5, 4, 3, 2, 1];
console.log(is_permutation(arr1, arr2));  // Outputs: true
