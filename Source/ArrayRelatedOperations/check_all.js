// Criteria function that checks if a number is positive
function criteria(num) {
    return num > 0;
}

// Function to check all elements in an array
function check_all(arr) {
    for(let i = 0; i < arr.length; i++) {
        if(!criteria(arr[i])) {
            return false;
        }
    }
    return true;
}

// Define an array of integers
let arr = [1, 2, 3, 4, 5];

// Check if all elements in the array are positive
let result = check_all(arr);

// Print the result
if(result) {
    console.log("All elements in the array are positive.");
} else {
    console.log("Not all elements in the array are positive.");
}
