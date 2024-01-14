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
