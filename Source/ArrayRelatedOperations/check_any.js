// Criteria function that checks if a number is positive
function criteria(num) {
    return num > 0;
}

// Function to check all elements in an array
function check_any(arr) {
    for(let i = 0; i < arr.length; i++) {
        if(criteria(arr[i])) {
            return true;
        }
    }
    return false;
}

// Define an array of integers
let arr = [1, 2, 3, 4, 5];

// Check if all elements in the array are positive
let result = check_any(arr);

// Print the result
if(result) {
    console.log("There is at least an element in the array that is positive.\n");
} else {
    console.log("There is no element in the array that is positive.\n");
}
